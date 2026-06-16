import pygame
import random
import math
import os

pygame.init()

# Configuración de la ventana
WIDTH, HEIGHT = 640, 800
win = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Touhou Style Shooter")

# Colores
BLACK = (20, 20, 40)
WHITE = (255, 255, 255)
CYAN = (0, 255, 255)
RED = (255, 50, 50)
YELLOW = (255, 255, 0)
PINK = (255, 180, 200)
GREEN = (100, 255, 100)

# Cargar assets
def load_image(name):
    path = os.path.join("archivos_originales", "assets", "sprites", name)
    try:
        img = pygame.image.load(path)
        return pygame.transform.scale(img, (64, 64))
    except:
        img = pygame.Surface((64, 64))
        if "Uta" in name:
            img.fill(CYAN)
        elif "Momone" in name:
            img.fill((180, 50, 120))
        elif "Akita" in name:
            img.fill(GREEN)
        elif "Megurine" in name:
            img.fill(PINK)
        return img

player_img = load_image("UtaUtane.jpg")
boss_img = load_image("MomoneMomo.jpg")
ally_img = load_image("AkitaNeru.jpg")
npc_img = load_image("MegurineLuka.jpg")
enemy_img = pygame.Surface((48, 48))
enemy_img.fill(RED)

# Fuente
try:
    font = pygame.font.SysFont("Arial", 24)
    font_large = pygame.font.SysFont("Arial", 48)
    font_small = pygame.font.SysFont("Arial", 18)
except:
    font = pygame.font.Font(None, 24)
    font_large = pygame.font.Font(None, 48)
    font_small = pygame.font.Font(None, 18)

# Clases
class Player:
    def __init__(self):
        self.image = player_img
        self.x = WIDTH // 2 - 32
        self.y = HEIGHT - 100
        self.speed = 5
        self.lives = 3
        self.ammo = 100
        self.max_ammo = 100
        self.shoot_cooldown = 0
        self.invincible = 0
    
    def update(self, keys):
        if self.shoot_cooldown > 0:
            self.shoot_cooldown -= 1
        if self.invincible > 0:
            self.invincible -= 1
        
        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            self.x -= self.speed
        if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            self.x += self.speed
        
        self.x = max(0, min(WIDTH - 64, self.x))
    
    def shoot(self):
        if self.shoot_cooldown == 0 and self.ammo > 0:
            self.shoot_cooldown = 10
            self.ammo -= 1
            return Bullet(self.x + 28, self.y, 0, -10, CYAN, "player")
        return None
    
    def draw(self, win):
        if self.invincible == 0 or self.invincible % 4 < 2:
            win.blit(self.image, (self.x, self.y))
    
    def get_rect(self):
        return pygame.Rect(self.x, self.y, 64, 64)

class Enemy:
    def __init__(self, x, y):
        self.image = enemy_img
        self.x = x
        self.y = y
        self.speed = 2 + random.random() * 2
        self.health = 20
        self.shoot_timer = 0
    
    def update(self):
        self.y += self.speed
        self.shoot_timer += 1
    
    def shoot(self):
        if self.shoot_timer > 60:
            self.shoot_timer = 0
            return Bullet(self.x + 20, self.y + 48, 0, 5, RED, "enemy")
        return None
    
    def draw(self, win):
        win.blit(self.image, (self.x, self.y))
    
    def get_rect(self):
        return pygame.Rect(self.x, self.y, 48, 48)

class Boss:
    def __init__(self):
        self.image = pygame.transform.scale(boss_img, (128, 128))
        self.x = WIDTH // 2 - 64
        self.y = -150
        self.speed = 2
        self.health = 500
        self.max_health = 500
        self.phase = 1
        self.attack_timer = 0
        self.current_attack = 0
        self.move_dir = 1
        self.entering = True
    
    def update(self, player_x):
        if self.entering:
            self.y += 2
            if self.y >= 50:
                self.entering = False
            return
        
        self.x += self.move_dir * self.speed
        if self.x <= 50 or self.x >= WIDTH - 180:
            self.move_dir *= -1
        
        if self.health <= 170 and self.phase < 3:
            self.phase = 3
        elif self.health <= 340 and self.phase < 2:
            self.phase = 2
        
        self.attack_timer += 1
        attack_interval = 60 // self.phase
        
        if self.attack_timer >= attack_interval:
            self.attack_timer = 0
            self.current_attack = (self.current_attack + 1) % 3
            return self.attack(player_x)
        return []
    
    def attack(self, player_x):
        bullets = []
        if self.current_attack == 0:
            for i in range(-3, 4):
                angle = math.radians(90 + i * 15)
                bullets.append(Bullet(self.x + 56, self.y + 120, 
                    math.cos(angle)*4, math.sin(angle)*4, RED, "enemy"))
        elif self.current_attack == 1:
            dx = player_x - self.x
            dy = 200
            angle = math.atan2(dy, dx)
            bullets.append(Bullet(self.x + 56, self.y + 120, 
                math.cos(angle)*6, math.sin(angle)*6, RED, "enemy"))
        else:
            for i in range(8):
                angle = math.radians(i * 45) + self.attack_timer * 0.05
                bullets.append(Bullet(self.x + 56, self.y + 120, 
                    math.cos(angle)*3, math.sin(angle)*3, RED, "enemy"))
        return bullets
    
    def draw(self, win):
        win.blit(self.image, (self.x, self.y))
        pygame.draw.rect(win, (50,50,50), (120, 750, 400, 25))
        hp_width = int(400 * (self.health / self.max_health))
        pygame.draw.rect(win, RED, (120, 750, hp_width, 25))
        boss_label = font_small.render("JEFE FINAL", True, WHITE)
        win.blit(boss_label, (120, 725))
    
    def get_rect(self):
        return pygame.Rect(self.x, self.y, 128, 128)

class Ally:
    def __init__(self):
        self.image = ally_img
        self.x = WIDTH // 2 - 32
        self.y = 400
        self.collected = False
        self.fade = 255
    
    def update(self):
        if self.collected:
            self.fade -= 5
    
    def draw(self, win):
        if self.fade > 0:
            img = self.image.copy()
            img.set_alpha(self.fade)
            win.blit(img, (self.x, self.y))
    
    def get_rect(self):
        return pygame.Rect(self.x, self.y, 64, 64)

class NPC:
    def __init__(self):
        self.image = npc_img
        self.x = 100 + random.randint(0, 440)
        self.y = 300 + random.randint(0, 200)
        self.show_msg = False
        self.msg_timer = 0
    
    def update(self):
        if self.show_msg:
            self.msg_timer += 1
            if self.msg_timer > 240:
                self.show_msg = False
                self.msg_timer = 0
    
    def draw(self, win):
        win.blit(self.image, (self.x, self.y))
        if self.show_msg:
            msg = font_small.render("Ya tu sabe, continua por aqui", True, WHITE)
            pygame.draw.rect(win, BLACK, (self.x - 60, self.y - 40, 280, 30))
            win.blit(msg, (self.x - 50, self.y - 35))
    
    def get_rect(self):
        return pygame.Rect(self.x, self.y, 64, 64)

class Bullet:
    def __init__(self, x, y, dx, dy, color, owner):
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.color = color
        self.owner = owner
        self.radius = 6 if owner == "player" else 8
    
    def update(self):
        self.x += self.dx
        self.y += self.dy
    
    def draw(self, win):
        pygame.draw.circle(win, self.color, (int(self.x), int(self.y)), self.radius)
    
    def get_rect(self):
        return pygame.Rect(self.x - self.radius, self.y - self.radius, 
                          self.radius * 2, self.radius * 2)
    
    def is_off_screen(self):
        return self.y < -20 or self.y > HEIGHT + 20 or \
               self.x < -20 or self.x > WIDTH + 20

# Funciones del juego
def draw_menu(selected):
    win.fill(BLACK)
    title = font_large.render("TOUHOU STYLE SHOOTER", True, WHITE)
    win.blit(title, (WIDTH//2 - title.get_width()//2, 100))
    
    options = ["Jugar", "Mejores Puntuaciones", "Salir"]
    for i, opt in enumerate(options):
        color = YELLOW if i == selected else WHITE
        text = font.render(opt, True, color)
        win.blit(text, (WIDTH//2 - text.get_width()//2, 250 + i * 70))
    
    pygame.display.update()

def draw_hud(score, lives, ammo, max_ammo, stage):
    s_text = font_small.render(f"Puntuacion: {score}", True, WHITE)
    l_text = font_small.render(f"Vidas: {lives}", True, WHITE)
    a_text = font_small.render(f"Municion: {ammo}", True, WHITE)
    st_text = font_small.render(f"Fase: {stage}", True, WHITE)
    
    win.blit(s_text, (10, 10))
    win.blit(l_text, (10, 35))
    win.blit(a_text, (10, 60))
    win.blit(st_text, (10, 85))
    
    pygame.draw.rect(win, (50,50,50), (10, 110, 150, 15))
    ammo_w = int(150 * (ammo / max_ammo))
    pygame.draw.rect(win, CYAN, (10, 110, ammo_w, 15))

def draw_pause():
    overlay = pygame.Surface((WIDTH, HEIGHT))
    overlay.set_alpha(128)
    overlay.fill(BLACK)
    win.blit(overlay, (0,0))
    
    pause_txt = font_large.render("PAUSA", True, WHITE)
    esc_txt = font_small.render("ESC para continuar", True, WHITE)
    win.blit(pause_txt, (WIDTH//2 - pause_txt.get_width()//2, 300))
    win.blit(esc_txt, (WIDTH//2 - esc_txt.get_width()//2, 370))

def draw_game_over(score):
    overlay = pygame.Surface((WIDTH, HEIGHT))
    overlay.set_alpha(180)
    overlay.fill(BLACK)
    win.blit(overlay, (0,0))
    
    go_txt = font_large.render("GAME OVER", True, RED)
    s_txt = font.render(f"Puntuacion: {score}", True, WHITE)
    esc_txt = font_small.render("ESC para volver", True, WHITE)
    win.blit(go_txt, (WIDTH//2 - go_txt.get_width()//2, 200))
    win.blit(s_txt, (WIDTH//2 - s_txt.get_width()//2, 280))
    win.blit(esc_txt, (WIDTH//2 - esc_txt.get_width()//2, 450))

def draw_victory(score):
    overlay = pygame.Surface((WIDTH, HEIGHT))
    overlay.set_alpha(180)
    overlay.fill(BLACK)
    win.blit(overlay, (0,0))
    
    v_txt = font_large.render("VICTORIA!", True, GREEN)
    s_txt = font.render(f"Puntuacion: {score}", True, WHITE)
    esc_txt = font_small.render("ESC para volver", True, WHITE)
    win.blit(v_txt, (WIDTH//2 - v_txt.get_width()//2, 200))
    win.blit(s_txt, (WIDTH//2 - s_txt.get_width()//2, 280))
    win.blit(esc_txt, (WIDTH//2 - esc_txt.get_width()//2, 450))

def main():
    clock = pygame.time.Clock()
    state = "menu"  # menu, playing, paused, game_over, victory, scores
    selected_menu = 0
    
    # Variables del juego
    player = None
    enemies = []
    boss = None
    ally = None
    npc = None
    player_bullets = []
    enemy_bullets = []
    score = 0
    stage = 1
    spawn_timer = 0
    ally_spawned = False
    
    running = True
    while running:
        clock.tick(60)
        
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            
            if event.type == pygame.KEYDOWN:
                if state == "menu":
                    if event.key == pygame.K_UP:
                        selected_menu = (selected_menu - 1) % 3
                    if event.key == pygame.K_DOWN:
                        selected_menu = (selected_menu + 1) % 3
                    if event.key == pygame.K_RETURN:
                        if selected_menu == 0:
                            # Iniciar juego
                            player = Player()
                            enemies = []
                            boss = None
                            ally = None
                            npc = NPC()
                            player_bullets = []
                            enemy_bullets = []
                            score = 0
                            stage = 1
                            spawn_timer = 0
                            ally_spawned = False
                            state = "playing"
                        elif selected_menu == 2:
                            running = False
                elif state in ["playing", "paused"]:
                    if event.key == pygame.K_ESCAPE:
                        state = "paused" if state == "playing" else "playing"
                elif state in ["game_over", "victory", "scores"]:
                    if event.key == pygame.K_ESCAPE:
                        state = "menu"
        
        if state == "menu":
            draw_menu(selected_menu)
        
        elif state in ["playing", "paused"]:
            if state == "playing":
                keys = pygame.key.get_pressed()
                player.update(keys)
                
                # Disparo del jugador
                if keys[pygame.K_SPACE]:
                    b = player.shoot()
                    if b:
                        player_bullets.append(b)
                
                # Generar enemigos
                spawn_timer += 1
                spawn_rate = max(30, 150 - stage * 20)
                
                if stage < 5 and spawn_timer >= spawn_rate:
                    spawn_timer = 0
                    enemies.append(Enemy(random.randint(0, WIDTH-48), -50))
                    if stage > 2:
                        enemies.append(Enemy(random.randint(0, WIDTH-48), -50))
                
                # Generar aliado
                if stage >= 3 and not ally_spawned and not ally:
                    ally = Ally()
                    ally_spawned = True
                
                # Generar jefe
                if stage == 5 and not boss and len(enemies) == 0:
                    boss = Boss()
                
                # Avanzar fase
                if stage < 5 and len(enemies) == 0 and spawn_timer > 180:
                    stage += 1
                    score += 500
                    spawn_timer = 0
                
                # Actualizar enemigos
                for enemy in enemies[:]:
                    enemy.update()
                    b = enemy.shoot()
                    if b:
                        enemy_bullets.append(b)
                    if enemy.y > HEIGHT + 50:
                        enemies.remove(enemy)
                
                # Actualizar jefe
                if boss:
                    new_bullets = boss.update(player.x)
                    enemy_bullets.extend(new_bullets)
                    if boss.health <= 0:
                        score += 5000
                        state = "victory"
                
                # Actualizar aliado y NPC
                if ally:
                    ally.update()
                if npc:
                    npc.update()
                
                # Actualizar balas
                for b in player_bullets[:]:
                    b.update()
                    if b.is_off_screen():
                        player_bullets.remove(b)
                for b in enemy_bullets[:]:
                    b.update()
                    if b.is_off_screen():
                        enemy_bullets.remove(b)
                
                # Colisiones - Balas jugador vs enemigos
                for b in player_bullets[:]:
                    # vs enemigos
                    for enemy in enemies[:]:
                        if b.get_rect().colliderect(enemy.get_rect()):
                            enemy.health -= 20
                            if b in player_bullets: player_bullets.remove(b)
                            if enemy.health <= 0:
                                score += 100
                                if enemy in enemies: enemies.remove(enemy)
                            break
                    # vs jefe
                    if boss and b.get_rect().colliderect(boss.get_rect()):
                        boss.health -= 20
                        score += 10
                        if b in player_bullets: player_bullets.remove(b)
                
                # Colisiones - Balas enemigas vs jugador
                if player.invincible == 0:
                    for b in enemy_bullets[:]:
                        if b.get_rect().colliderect(player.get_rect()):
                            player.lives -= 1
                            player.invincible = 120
                            if b in enemy_bullets: enemy_bullets.remove(b)
                            if player.lives <= 0:
                                state = "game_over"
                            break
                
                # Colisiones - Enemigos vs jugador
                if player.invincible == 0:
                    for enemy in enemies[:]:
                        if enemy.get_rect().colliderect(player.get_rect()):
                            player.lives -= 1
                            player.invincible = 120
                            enemy.health = 0
                            if player.lives <= 0:
                                state = "game_over"
                
                # Colisiones - Jugador vs Aliado
                if ally and not ally.collected:
                    if player.get_rect().colliderect(ally.get_rect()):
                        ally.collected = True
                        player.ammo = player.max_ammo
                
                # Colisiones - Jugador vs NPC
                if npc and not npc.show_msg:
                    if player.get_rect().colliderect(npc.get_rect()):
                        npc.show_msg = True
            
            # Dibujar todo
            win.fill(BLACK)
            if npc: npc.draw(win)
            if ally: ally.draw(win)
            for enemy in enemies: enemy.draw(win)
            if boss: boss.draw(win)
            player.draw(win)
            for b in player_bullets: b.draw(win)
            for b in enemy_bullets: b.draw(win)
            draw_hud(score, player.lives, player.ammo, player.max_ammo, stage)
            
            if state == "paused":
                draw_pause()
            
            pygame.display.update()
        
        elif state == "game_over":
            draw_game_over(score)
            pygame.display.update()
        
        elif state == "victory":
            draw_victory(score)
            pygame.display.update()
    
    pygame.quit()

if __name__ == "__main__":
    main()
