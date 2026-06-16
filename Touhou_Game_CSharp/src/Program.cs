using SFML.System;
using SFML.Graphics;
using SFML.Window;
using System;
using System.Collections.Generic;
using System.Linq;

namespace TouhouGameCS
{
    public enum GameState
    {
        MainMenu,
        Playing,
        Paused,
        GameOver,
        Victory
    }

    public enum BulletOwner
    {
        Player,
        Enemy,
        Boss
    }

    public enum BulletType
    {
        Small,
        Medium,
        Large,
        Star,
        Heart,
        Ring,
        Arrow,
        Butterfly
    }

    public enum MenuSelection
    {
        Play,
        Settings,
        HighScores,
        Exit
    }

    public class Tile
    {
        public Sprite Sprite;
        public Vector2f WorldPosition;
        public bool Solid;
    }

    public class Program
    {
        static void Main(string[] args)
        {
            Game game = new Game();
            game.Run();
        }
    }

    public class ResourceManager
    {
        private Dictionary<string, Texture> textures = new Dictionary<string, Texture>();
        private Dictionary<string, Font> fonts = new Dictionary<string, Font>();
        private Dictionary<string, SoundBuffer> soundBuffers = new Dictionary<string, SoundBuffer>();

        public bool LoadAll()
        {
            bool success = true;

            try
            {
                if (System.IO.File.Exists("assets/textures/bullet_sprites.jpg"))
                    textures["bullet_sprites"] = new Texture("assets/textures/bullet_sprites.jpg");
                else
                {
                    Image img = new Image(512, 512, Color.Transparent);
                    textures["bullet_sprites"] = new Texture(img);
                    Console.WriteLine("Warning: Could not load bullet_sprites.jpg");
                }
            }
            catch
            {
                Image img = new Image(512, 512, Color.Transparent);
                textures["bullet_sprites"] = new Texture(img);
            }

            try
            {
                if (System.IO.File.Exists("assets/textures/window_layout.jpg"))
                    textures["window_layout"] = new Texture("assets/textures/window_layout.jpg");
                else
                {
                    Image img = new Image(256, 256, Color.Blue);
                    textures["window_layout"] = new Texture(img);
                    Console.WriteLine("Warning: Could not load window_layout.jpg");
                }
            }
            catch
            {
                Image img = new Image(256, 256, Color.Blue);
                textures["window_layout"] = new Texture(img);
            }

            Image playerImg = new Image(64, 64, Color.Cyan);
            textures["player"] = new Texture(playerImg);

            Image terrainImg = new Image(128, 128, new Color(40, 40, 70));
            textures["terrain"] = new Texture(terrainImg);

            try
            {
                if (OperatingSystem.IsWindows())
                    fonts["default"] = new Font("C:/Windows/Fonts/arial.ttf");
                else if (OperatingSystem.IsLinux())
                    fonts["default"] = new Font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
                else if (OperatingSystem.IsMacOS())
                    fonts["default"] = new Font("/System/Library/Fonts/Arial.ttf");
            }
            catch
            {
                Console.WriteLine("Warning: Could not load system font");
            }

            return success;
        }

        public Texture GetTexture(string name) => textures[name];
        public Font GetFont(string name) => fonts[name];
        public SoundBuffer GetSoundBuffer(string name) => soundBuffers[name];
    }

    public class Player
    {
        private Sprite sprite;
        private Vector2f position;
        private Vector2f velocity;
        private float speed = 300.0f;
        private int lives = 3;
        private float invincibleTime = 0.0f;
        private float invincibleBlinkTime = 0.0f;

        public Player()
        {
            Image img = new Image(48, 48, Color.Cyan);
            for (int x = 10; x < 38; x++)
                for (int y = 10; y < 38; y++)
                    img.SetPixel((uint)x, (uint)y, new Color(0, 150, 200));
            Texture tex = new Texture(img);
            sprite = new Sprite(tex);
            sprite.Origin = new Vector2f(24, 24);
            position = new Vector2f(200, 500);
            velocity = new Vector2f(0, 0);
        }

        public void Update(Time deltaTime, FloatRect bounds)
        {
            float dt = deltaTime.AsSeconds();
            if (invincibleTime > 0.0f)
            {
                invincibleTime -= dt;
                invincibleBlinkTime += dt;
            }
            velocity = new Vector2f(0, 0);

            if (Keyboard.IsKeyPressed(Keyboard.Key.Left) || Keyboard.IsKeyPressed(Keyboard.Key.A)) velocity.X = -speed;
            if (Keyboard.IsKeyPressed(Keyboard.Key.Right) || Keyboard.IsKeyPressed(Keyboard.Key.D)) velocity.X = speed;
            if (Keyboard.IsKeyPressed(Keyboard.Key.Up) || Keyboard.IsKeyPressed(Keyboard.Key.W)) velocity.Y = -speed;
            if (Keyboard.IsKeyPressed(Keyboard.Key.Down) || Keyboard.IsKeyPressed(Keyboard.Key.S)) velocity.Y = speed;

            position += velocity * dt;
            position.X = Math.Max(bounds.Left + 24, Math.Min(bounds.Left + bounds.Width - 24, position.X));
            position.Y = Math.Max(bounds.Top + 24, Math.Min(bounds.Top + bounds.Height - 24, position.Y));
            sprite.Position = position;
        }

        public void Render(RenderWindow window)
        {
            if (invincibleTime > 0.0f)
            {
                if ((int)(invincibleBlinkTime * 10) % 2 == 0)
                    window.Draw(sprite);
            }
            else
                window.Draw(sprite);
        }

        public void TakeDamage()
        {
            if (invincibleTime <= 0.0f)
            {
                lives--;
                invincibleTime = 2.0f;
                invincibleBlinkTime = 0.0f;
            }
        }

        public FloatRect GetBounds() => new FloatRect(position.X - 12, position.Y - 12, 24, 24);
        public Vector2f GetPosition() => position;
        public Vector2f GetShootPosition() => new Vector2f(position.X, position.Y - 20);
        public bool IsInvincible() => invincibleTime > 0.0f;
        public int GetLives() => lives;
    }

    public class Bullet
    {
        private Sprite sprite;
        private Vector2f position;
        private Vector2f velocity;
        private BulletOwner owner;
        private BulletType type;
        private float rotation;
        private float rotationSpeed;
        private static Random rand = new Random();

        public Bullet(Vector2f pos, Vector2f vel, BulletOwner owner, BulletType type)
        {
            this.position = pos;
            this.velocity = vel;
            this.owner = owner;
            this.type = type;
            rotation = 0.0f;

            Color bulletColor;
            float radius;

            switch (type)
            {
                case BulletType.Small:
                    radius = 6.0f;
                    bulletColor = owner == BulletOwner.Player ? Color.Cyan : new Color(255, 100, 100);
                    break;
                case BulletType.Medium:
                    radius = 10.0f;
                    bulletColor = owner == BulletOwner.Player ? new Color(0, 200, 255) : new Color(255, 150, 50);
                    break;
                case BulletType.Large:
                    radius = 15.0f;
                    bulletColor = owner == BulletOwner.Player ? new Color(0, 150, 255) : new Color(200, 50, 200);
                    break;
                case BulletType.Star:
                    radius = 12.0f;
                    bulletColor = Color.Yellow;
                    break;
                case BulletType.Heart:
                    radius = 14.0f;
                    bulletColor = new Color(255, 100, 150);
                    break;
                case BulletType.Ring:
                    radius = 18.0f;
                    bulletColor = new Color(150, 255, 100);
                    break;
                case BulletType.Arrow:
                    radius = 10.0f;
                    bulletColor = new Color(100, 100, 255);
                    break;
                case BulletType.Butterfly:
                    radius = 14.0f;
                    bulletColor = new Color(150, 100, 255);
                    break;
                default:
                    radius = 10.0f;
                    bulletColor = Color.White;
                    break;
            }

            CircleShape shape = new CircleShape(radius);
            shape.Origin = new Vector2f(radius, radius);
            shape.FillColor = bulletColor;
            shape.OutlineColor = Color.White;
            shape.OutlineThickness = 1;

            RenderTexture rt = new RenderTexture((uint)(radius * 2 + 4), (uint)(radius * 2 + 4));
            rt.Clear(Color.Transparent);
            shape.Position = new Vector2f(radius + 2, radius + 2);
            rt.Draw(shape);
            rt.Display();
            Texture tex = new Texture(rt.Texture);
            sprite = new Sprite(tex);
            sprite.Origin = new Vector2f(radius + 2, radius + 2);
            sprite.Position = position;

            rotationSpeed = owner == BulletOwner.Player ? 0.0f : (rand.Next(200) - 100);
        }

        public void Update(Time deltaTime)
        {
            float dt = deltaTime.AsSeconds();
            position += velocity * dt;
            rotation += rotationSpeed * dt;
            sprite.Position = position;
            sprite.Rotation = rotation;
        }

        public void Render(RenderWindow window)
        {
            window.Draw(sprite);
        }

        public FloatRect GetBounds() => sprite.GetGlobalBounds();
        public bool IsOffScreen(RenderWindow window)
        {
            Vector2u size = window.Size;
            return position.X < -50 || position.X > size.X + 50 || position.Y < -50 || position.Y > size.Y + 50;
        }

        public BulletOwner GetOwner() => owner;
    }

    public class BulletPatternGenerator
    {
        private static Random rand = new Random();

        public List<Bullet> GenerateSpiral(Vector2f origin, int count, float speed, float startAngle, float angleStep, int bulletsPerStep = 1)
        {
            List<Bullet> bullets = new List<Bullet>();
            for (int i = 0; i < count; i++)
            {
                float angle = startAngle + i * angleStep;
                Vector2f vel = new Vector2f((float)Math.Cos(angle) * speed, (float)Math.Sin(angle) * speed);
                bullets.Add(new Bullet(origin, vel, BulletOwner.Enemy, BulletType.Small));
            }
            return bullets;
        }

        public List<Bullet> GenerateRing(Vector2f origin, int count, float speed, float startAngle = 0.0f)
        {
            List<Bullet> bullets = new List<Bullet>();
            float angleStep = (2.0f * (float)Math.PI) / count;
            for (int i = 0; i < count; i++)
            {
                float angle = startAngle + i * angleStep;
                Vector2f vel = new Vector2f((float)Math.Cos(angle) * speed, (float)Math.Sin(angle) * speed);
                bullets.Add(new Bullet(origin, vel, BulletOwner.Enemy, BulletType.Medium));
            }
            return bullets;
        }

        public List<Bullet> GenerateAimed(Vector2f origin, Vector2f target, int count, float spread, float speed)
        {
            List<Bullet> bullets = new List<Bullet>();
            float baseAngle = (float)Math.Atan2(target.Y - origin.Y, target.X - origin.X);
            for (int i = 0; i < count; i++)
            {
                float angle = baseAngle + (i - count / 2.0f) * spread;
                Vector2f vel = new Vector2f((float)Math.Cos(angle) * speed, (float)Math.Sin(angle) * speed);
                bullets.Add(new Bullet(origin, vel, BulletOwner.Enemy, BulletType.Small));
            }
            return bullets;
        }

        public List<Bullet> GenerateWave(Vector2f origin, int count, float amplitude, float frequency, float speed)
        {
            List<Bullet> bullets = new List<Bullet>();
            for (int i = 0; i < count; i++)
            {
                float offset = (float)i * 0.3f;
                Vector2f vel = new Vector2f(50.0f * (float)Math.Sin(frequency * offset), speed);
                bullets.Add(new Bullet(origin + new Vector2f(amplitude * (float)Math.Sin(offset), 0), vel, BulletOwner.Enemy, BulletType.Star));
            }
            return bullets;
        }

        public List<Bullet> GenerateStar(Vector2f origin, int arms, float speed)
        {
            List<Bullet> bullets = new List<Bullet>();
            float angleStep = (2.0f * (float)Math.PI) / arms;
            for (int i = 0; i < arms; i++)
            {
                float angle = i * angleStep;
                Vector2f vel = new Vector2f((float)Math.Cos(angle) * speed, (float)Math.Sin(angle) * speed);
                bullets.Add(new Bullet(origin, vel, BulletOwner.Enemy, BulletType.Star));
            }
            return bullets;
        }

        public List<Bullet> GenerateRandom(Vector2f origin, int count, float speedMin, float speedMax)
        {
            List<Bullet> bullets = new List<Bullet>();
            for (int i = 0; i < count; i++)
            {
                float angle = (float)rand.NextDouble() * 2.0f * (float)Math.PI;
                float speed = speedMin + (float)rand.NextDouble() * (speedMax - speedMin);
                Vector2f vel = new Vector2f((float)Math.Cos(angle) * speed, (float)Math.Sin(angle) * speed);
                BulletType type = (BulletType)(rand.Next(4));
                bullets.Add(new Bullet(origin, vel, BulletOwner.Enemy, type));
            }
            return bullets;
        }
    }

    public class HUD
    {
        private Text scoreText;
        private Text livesText;
        private Text bombsText;
        private Text fpsText;
        private RectangleShape hudBackground;
        private RectangleShape bossHealthBar;
        private RectangleShape bossHealthBg;

        public HUD()
        {
            hudBackground = new RectangleShape(new Vector2f(300, 120));
            hudBackground.FillColor = new Color(20, 20, 50, 200);
            hudBackground.Position = new Vector2f(10, 10);
            hudBackground.OutlineColor = Color.Magenta;
            hudBackground.OutlineThickness = 2;

            bossHealthBg = new RectangleShape(new Vector2f(400, 30));
            bossHealthBg.FillColor = new Color(50, 30, 50, 200);
            bossHealthBg.Position = new Vector2f(600, 750);

            bossHealthBar = new RectangleShape(new Vector2f(400, 30));
            bossHealthBar.FillColor = new Color(200, 50, 100);
            bossHealthBar.Position = new Vector2f(600, 750);

            scoreText = new Text();
            livesText = new Text();
            bombsText = new Text();
            fpsText = new Text();
        }

        public void Update(int score, int lives, int bombs, float fps)
        {
            scoreText.DisplayedString = "Score: " + score;
            livesText.DisplayedString = "Lives: " + lives;
            bombsText.DisplayedString = "Bombs: " + bombs;
            fpsText.DisplayedString = "FPS: " + (int)fps;
        }

        public void Render(RenderWindow window)
        {
            window.Draw(hudBackground);
            window.Draw(scoreText);
            window.Draw(livesText);
            window.Draw(bombsText);
            window.Draw(fpsText);
            window.Draw(bossHealthBg);
            window.Draw(bossHealthBar);
        }

        public void SetFont(Font font)
        {
            scoreText.Font = font;
            livesText.Font = font;
            bombsText.Font = font;
            fpsText.Font = font;

            scoreText.CharacterSize = 24;
            livesText.CharacterSize = 24;
            bombsText.CharacterSize = 24;
            fpsText.CharacterSize = 20;

            scoreText.FillColor = Color.Cyan;
            livesText.FillColor = Color.Green;
            bombsText.FillColor = new Color(255, 150, 255);
            fpsText.FillColor = Color.Yellow;

            scoreText.Position = new Vector2f(20, 20);
            livesText.Position = new Vector2f(20, 50);
            bombsText.Position = new Vector2f(20, 80);
            fpsText.Position = new Vector2f(20, 110);
        }
    }

    public class Menu
    {
        private List<Text> menuItems;
        private MenuSelection currentSelection;
        private Text title;
        private RectangleShape background;

        public Menu()
        {
            currentSelection = MenuSelection.Play;
            background = new RectangleShape();
            background.FillColor = new Color(10, 10, 30, 240);

            title = new Text();
            title.DisplayedString = "Touhou Style\n  Bullet Hell";
            title.CharacterSize = 50;
            title.FillColor = Color.Magenta;
            title.Style = Text.Styles.Bold;

            menuItems = new List<Text>();
            for (int i = 0; i < 4; i++) menuItems.Add(new Text());
            menuItems[0].DisplayedString = "Play Game";
            menuItems[1].DisplayedString = "Settings";
            menuItems[2].DisplayedString = "High Scores";
            menuItems[3].DisplayedString = "Exit";

            for (int i = 0; i < menuItems.Count; i++)
            {
                menuItems[i].CharacterSize = 30;
                menuItems[i].Style = Text.Styles.Bold;
                menuItems[i].FillColor = i == 0 ? Color.Yellow : Color.White;
            }
        }

        public void Update()
        {
            for (int i = 0; i < menuItems.Count; i++)
            {
                if ((MenuSelection)i == currentSelection)
                    menuItems[i].FillColor = Color.Yellow;
                else
                    menuItems[i].FillColor = Color.White;
            }
        }

        public void Render(RenderWindow window)
        {
            Vector2u size = window.Size;
            background.Size = new Vector2f(size.X, size.Y);
            window.Draw(background);

            FloatRect bounds = title.GetLocalBounds();
            title.Origin = new Vector2f(bounds.Width / 2, bounds.Height / 2);
            title.Position = new Vector2f(size.X / 2, 150);
            window.Draw(title);

            for (int i = 0; i < menuItems.Count; i++)
            {
                bounds = menuItems[i].GetLocalBounds();
                menuItems[i].Origin = new Vector2f(bounds.Width / 2, bounds.Height / 2);
                menuItems[i].Position = new Vector2f(size.X / 2, 300 + i * 70);
                window.Draw(menuItems[i]);
            }
        }

        public void SetFont(Font font)
        {
            title.Font = font;
            foreach (var item in menuItems) item.Font = font;
        }

        public MenuSelection GetSelection() => currentSelection;

        public void MoveUp()
        {
            if (currentSelection == MenuSelection.Play)
                currentSelection = MenuSelection.Exit;
            else
                currentSelection = (MenuSelection)((int)currentSelection - 1);
        }

        public void MoveDown()
        {
            if (currentSelection == MenuSelection.Exit)
                currentSelection = MenuSelection.Play;
            else
                currentSelection = (MenuSelection)((int)currentSelection + 1);
        }
    }

    public class World
    {
        private List<Tile> tiles;
        private Texture terrainTexture;
        private View camera;
        private float worldWidth = 8000.0f;
        private float worldHeight = 6000.0f;
        private static Random rand = new Random();

        public World()
        {
            tiles = new List<Tile>();
            camera = new View(new Vector2f(4000, 3000), new Vector2f(1920, 1080));
            Image img = new Image(128, 128, new Color(40, 40, 70));
            terrainTexture = new Texture(img);
        }

        public void GenerateWorld()
        {
            tiles.Clear();
            const int gridSize = 64;
            for (float x = 0; x < worldWidth; x += gridSize)
            {
                for (float y = 0; y < worldHeight; y += gridSize)
                {
                    Tile tile = new Tile();
                    tile.WorldPosition = new Vector2f(x, y);

                    Sprite tSprite = new Sprite(terrainTexture);
                    tSprite.Position = tile.WorldPosition;

                    if (rand.Next(10) < 2)
                        tSprite.Color = new Color(60, 60, 90);
                    else if (rand.Next(10) < 4)
                        tSprite.Color = new Color(35, 35, 65);

                    tile.Sprite = tSprite;
                    tile.Solid = false;
                    tiles.Add(tile);
                }
            }
        }

        public void Update(Time deltaTime) { }

        public void Render(RenderWindow window, View cam)
        {
            window.SetView(cam);
            foreach (var tile in tiles)
                window.Draw(tile.Sprite);
            window.SetView(window.DefaultView);
        }

        public bool CheckCollision(FloatRect bounds)
        {
            foreach (var tile in tiles)
                if (tile.Solid && tile.Sprite.GetGlobalBounds().Intersects(bounds))
                    return true;
            return false;
        }

        public View GetCamera() => camera;

        public void SetCameraCenter(Vector2f center)
        {
            center.X = Math.Max(camera.Size.X / 2, Math.Min(worldWidth - camera.Size.X / 2, center.X));
            center.Y = Math.Max(camera.Size.Y / 2, Math.Min(worldHeight - camera.Size.Y / 2, center.Y));
            camera.Center = center;
        }
    }

    public class Game
    {
        private RenderWindow window;
        private GameState state;
        private ResourceManager resources;
        private Player player;
        private List<Bullet> enemyBullets;
        private List<Bullet> playerBullets;
        private World world;
        private HUD hud;
        private Menu menu;
        private int score;
        private int lives;
        private int bombCount;
        private float difficultyMultiplier;
        private Clock gameClock;
        private BulletPatternGenerator patternGen;
        private FloatRect playerZone;
        private FloatRect combatZone;
        private static Random rand = new Random();

        public Game()
        {
            state = GameState.MainMenu;
            score = 0;
            lives = 3;
            bombCount = 3;
            difficultyMultiplier = 1.0f;

            VideoMode desktop = VideoMode.DesktopMode;
            window = new RenderWindow(desktop, "Touhou Style Bullet Hell", Styles.Fullscreen);
            window.SetFramerateLimit(60);
            window.SetVerticalSyncEnabled(true);

            float windowWidth = desktop.Width;
            float windowHeight = desktop.Height;

            playerZone = new FloatRect(windowWidth * 0.05f, windowHeight * 0.1f, windowWidth * 0.3f, windowHeight * 0.8f);
            combatZone = new FloatRect(windowWidth * 0.38f, windowHeight * 0.05f, windowWidth * 0.57f, windowHeight * 0.9f);

            resources = new ResourceManager();
            if (!resources.LoadAll())
                Console.WriteLine("Failed to load resources!");

            player = new Player();
            enemyBullets = new List<Bullet>();
            playerBullets = new List<Bullet>();
            world = new World();
            hud = new HUD();
            menu = new Menu();
            gameClock = new Clock();
            patternGen = new BulletPatternGenerator();

            menu.SetFont(resources.GetFont("default"));
            hud.SetFont(resources.GetFont("default"));
            world.GenerateWorld();

            window.Closed += (sender, e) => window.Close();
            window.KeyPressed += OnKeyPressed;
        }

        private void OnKeyPressed(object sender, KeyEventArgs e)
        {
            if (state == GameState.MainMenu)
            {
                if (e.Code == Keyboard.Key.Up)
                    menu.MoveUp();
                else if (e.Code == Keyboard.Key.Down)
                    menu.MoveDown();
                else if (e.Code == Keyboard.Key.Return)
                {
                    if (menu.GetSelection() == MenuSelection.Play)
                    {
                        state = GameState.Playing;
                        score = 0;
                        lives = 3;
                        bombCount = 3;
                        player = new Player();
                        enemyBullets.Clear();
                        playerBullets.Clear();
                    }
                    else if (menu.GetSelection() == MenuSelection.Exit)
                        window.Close();
                }
            }
            else if (state == GameState.Playing || state == GameState.Paused)
            {
                if (e.Code == Keyboard.Key.Escape)
                    state = state == GameState.Playing ? GameState.Paused : GameState.Playing;
            }
        }

        public void Run()
        {
            Clock clock = new Clock();
            Time timeSinceLastUpdate = Time.Zero;
            Time timePerFrame = Time.FromSeconds(1.0f / 60.0f);

            while (window.IsOpen)
            {
                window.DispatchEvents();
                timeSinceLastUpdate += clock.Restart();

                while (timeSinceLastUpdate >= timePerFrame)
                {
                    timeSinceLastUpdate -= timePerFrame;
                    Update(timePerFrame);
                }
                Render();
            }
        }

        private float shootTimer = 0.0f;
        private float spawnTimer = 0.0f;

        private void Update(Time deltaTime)
        {
            if (state == GameState.Playing)
            {
                player.Update(deltaTime, playerZone);

                if (Keyboard.IsKeyPressed(Keyboard.Key.Space))
                {
                    shootTimer += deltaTime.AsSeconds();
                    if (shootTimer > 0.1f)
                    {
                        shootTimer = 0.0f;
                        Vector2f shootPos = player.GetShootPosition();
                        playerBullets.Add(new Bullet(shootPos, new Vector2f(0, -700.0f), BulletOwner.Player, BulletType.Small));
                        playerBullets.Add(new Bullet(shootPos + new Vector2f(-15, 0), new Vector2f(-30, -600.0f), BulletOwner.Player, BulletType.Small));
                        playerBullets.Add(new Bullet(shootPos + new Vector2f(15, 0), new Vector2f(30, -600.0f), BulletOwner.Player, BulletType.Small));
                    }
                }

                spawnTimer += deltaTime.AsSeconds();
                if (spawnTimer > 0.5f)
                {
                    spawnTimer = 0.0f;
                    Vector2f enemyPos = new Vector2f(combatZone.Left + combatZone.Width / 2, combatZone.Top + 100);
                    int pattern = rand.Next(4);
                    switch (pattern)
                    {
                        case 0: enemyBullets.AddRange(patternGen.GenerateRing(enemyPos, 12, 200.0f)); break;
                        case 1: enemyBullets.AddRange(patternGen.GenerateSpiral(enemyPos, 5, 250.0f, 0.0f, 0.3f)); break;
                        case 2: enemyBullets.AddRange(patternGen.GenerateStar(enemyPos, 8, 180.0f)); break;
                        case 3: enemyBullets.AddRange(patternGen.GenerateAimed(enemyPos, player.GetPosition(), 5, 0.2f, 300.0f)); break;
                    }
                }

                foreach (var bullet in enemyBullets) bullet.Update(deltaTime);
                foreach (var bullet in playerBullets) bullet.Update(deltaTime);

                enemyBullets = enemyBullets.Where(b => !b.IsOffScreen(window)).ToList();
                playerBullets = playerBullets.Where(b => !b.IsOffScreen(window)).ToList();

                HandleCollisions();

                world.SetCameraCenter(player.GetPosition());
                world.Update(deltaTime);
                hud.Update(score, lives, bombCount, 60.0f);
            }
        }

        private void HandleCollisions()
        {
            if (!player.IsInvincible())
            {
                foreach (var bullet in enemyBullets)
                {
                    if (bullet.GetBounds().Intersects(player.GetBounds()))
                    {
                        player.TakeDamage();
                        score = Math.Max(0, score - 100);
                        lives = player.GetLives();
                        if (lives <= 0)
                            state = GameState.GameOver;
                    }
                }
            }

            foreach (var bullet in playerBullets)
            {
                if (bullet.GetBounds().Intersects(combatZone))
                    score += 10;
            }
        }

        private void Render()
        {
            window.Clear(new Color(20, 20, 50));
            world.Render(window, world.GetCamera());

            RectangleShape pZoneRect = new RectangleShape(new Vector2f(playerZone.Width, playerZone.Height));
            pZoneRect.Position = new Vector2f(playerZone.Left, playerZone.Top);
            pZoneRect.FillColor = new Color(30, 30, 70, 100);
            pZoneRect.OutlineColor = Color.Cyan;
            pZoneRect.OutlineThickness = 2;
            window.Draw(pZoneRect);

            RectangleShape cZoneRect = new RectangleShape(new Vector2f(combatZone.Width, combatZone.Height));
            cZoneRect.Position = new Vector2f(combatZone.Left, combatZone.Top);
            cZoneRect.FillColor = new Color(30, 20, 60, 150);
            cZoneRect.OutlineColor = Color.Magenta;
            cZoneRect.OutlineThickness = 2;
            window.Draw(cZoneRect);

            foreach (var bullet in enemyBullets) bullet.Render(window);
            foreach (var bullet in playerBullets) bullet.Render(window);
            player.Render(window);
            hud.Render(window);

            if (state == GameState.MainMenu)
            {
                menu.Render(window);
            }
            else if (state == GameState.Paused)
            {
                Text pauseText = new Text("PAUSED - Press ESC to Continue", resources.GetFont("default"), 40);
                pauseText.FillColor = Color.White;
                pauseText.Style = Text.Styles.Bold;
                FloatRect bounds = pauseText.GetLocalBounds();
                pauseText.Origin = new Vector2f(bounds.Width / 2, bounds.Height / 2);
                pauseText.Position = new Vector2f(window.Size.X / 2, window.Size.Y / 2);

                RectangleShape overlay = new RectangleShape(new Vector2f(window.Size.X, window.Size.Y));
                overlay.FillColor = new Color(0, 0, 0, 180);
                window.Draw(overlay);
                window.Draw(pauseText);
            }
            else if (state == GameState.GameOver)
            {
                Text goText = new Text("GAME OVER!", resources.GetFont("default"), 60);
                goText.FillColor = new Color(255, 80, 80);
                goText.Style = Text.Styles.Bold;
                FloatRect bounds = goText.GetLocalBounds();
                goText.Origin = new Vector2f(bounds.Width / 2, bounds.Height / 2);
                goText.Position = new Vector2f(window.Size.X / 2, window.Size.Y / 2 - 50);

                Text scoreText = new Text("Final Score: " + score, resources.GetFont("default"), 30);
                scoreText.FillColor = Color.White;
                bounds = scoreText.GetLocalBounds();
                scoreText.Origin = new Vector2f(bounds.Width / 2, bounds.Height / 2);
                scoreText.Position = new Vector2f(window.Size.X / 2, window.Size.Y / 2 + 50);

                RectangleShape overlay = new RectangleShape(new Vector2f(window.Size.X, window.Size.Y));
                overlay.FillColor = new Color(0, 0, 0, 200);
                window.Draw(overlay);
                window.Draw(goText);
                window.Draw(scoreText);
            }

            window.Display();
        }
    }
}
