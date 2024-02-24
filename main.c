#include <raylib.h>

#include <stdio.h>
#include <string.h>

#define ENEMIES_ROW 4
#define ENEMIES_COL 10
#define ENEMIES_GAP 20
#define ENEMIES_COUNT ENEMIES_ROW*ENEMIES_COL
#define BULLETS_COUNT 10

typedef struct {
    Rectangle rect;
    Color     color;
    float     speed;
} Ship;

typedef struct {
    Rectangle rect;
    Color     color;
    float     speed;
    int       activated;
} Enemy;

typedef struct {
    Rectangle rect;
    Color     color;
    float     speed;
    float     thickness;
} EnemyArea;

typedef struct {
    Rectangle rect;
    Color     color;
    float     speed;
    int       activated;
} Bullet;

int main(void)
{
    const int SCREEN_W = 800;
    const int SCREEN_H = 600;

    InitWindow(SCREEN_W, SCREEN_H, "Invaders");

    Ship ship = {
        .rect.x = SCREEN_W/2,
        .rect.y = SCREEN_H - SCREEN_H * 0.12,
        .rect.width = 30,
        .rect.height = 30,
        .speed = 4,
        .color = GREEN,
    };

    const int SCREEN_PADDING_X = 50;
    const int SCREEN_PADDING_Y = 25;

    Enemy enemies[ENEMIES_ROW][ENEMIES_COL];

    for (int i = 0; i < ENEMIES_ROW; i++) {
        for (int j = 0; j < ENEMIES_COL; j++) {
            enemies[i][j].rect.width = 30;
            enemies[i][j].rect.height = 30;
            enemies[i][j].rect.x = (enemies[i][j].rect.width + ENEMIES_GAP) * j + SCREEN_PADDING_X;
            enemies[i][j].rect.y = (enemies[i][j].rect.height + ENEMIES_GAP) * i + SCREEN_PADDING_Y;
            enemies[i][j].speed = 1;
            enemies[i][j].activated = 1;
            enemies[i][j].color = LIGHTGRAY;
        }
    }

    EnemyArea enemy_area = {
        .rect.x = enemies[0][0].rect.x,
        .rect.y = enemies[0][0].rect.y,
        .rect.width = enemies[0][0].rect.width * ENEMIES_COL + (ENEMIES_GAP * ENEMIES_COL) - ENEMIES_GAP,
        .rect.height = enemies[0][0].rect.height * ENEMIES_ROW + (ENEMIES_GAP * ENEMIES_ROW) - ENEMIES_GAP,
        .thickness = 2,
        .speed = 1,
        .color = RED,
    };

    Bullet bullets[BULLETS_COUNT];

    for (int i = 0; i < BULLETS_COUNT; i++) {
        bullets[i].rect.x = ship.rect.x;
        bullets[i].rect.y = SCREEN_H - SCREEN_H * 0.1;
        bullets[i].rect.width = 3;
        bullets[i].rect.height = 15;
        bullets[i].speed = 10;
        bullets[i].activated = 0;
        bullets[i].color = WHITE;
    }

    SetTargetFPS(60);

    int score = 0;
    int enemies_alive = ENEMIES_COUNT;

    while(!WindowShouldClose()) {

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L)) ship.rect.x += 2.0f * ship.speed;
        if (IsKeyDown(KEY_LEFT) ||  IsKeyDown(KEY_H)) ship.rect.x -= 2.0f * ship.speed;

        if (ship.rect.x + ship.rect.width >= SCREEN_W) ship.rect.x = SCREEN_W - ship.rect.width;
        if (ship.rect.x <= 0) ship.rect.x = 0;

        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 0; i < BULLETS_COUNT; i++) {
                if (!bullets[i].activated) {
                    bullets[i].rect.x = ship.rect.x + ((ship.rect.width / 2) - (bullets[i].rect.width / 2));
                    bullets[i].rect.y = SCREEN_H - SCREEN_H * 0.1;
                    bullets[i].activated = 1;
                    break;
                }
            }
        }

        for (int i = 0; i < BULLETS_COUNT; i++) {
            if (bullets[i].activated) {

                bullets[i].rect.y -= bullets[i].speed;

                if (bullets[i].rect.y + bullets[i].rect.height < 0) {
                    bullets[i].activated = 0;
                }

                for (int j = 0; j < ENEMIES_ROW; j++) {
                    for (int k = 0; k < ENEMIES_COL; k++) {
                        if (enemies[j][k].activated) {
                            if (CheckCollisionRecs(bullets[i].rect, enemies[j][k].rect)) {
                                score++;
                                bullets[i].activated = 0;
                                enemies[j][k].activated = 0;
                                enemies_alive--;
                            }
                        }
                    }
                }
            }
        }

        enemy_area.rect.x += enemy_area.speed;
        if ((enemy_area.rect.x + enemy_area.rect.width >= SCREEN_W - SCREEN_PADDING_X) || (enemy_area.rect.x <= SCREEN_PADDING_X)) enemy_area.speed *= -1.0f;

        for (int i = 0; i < ENEMIES_ROW; i++) {
            for (int j = 0; j < ENEMIES_COL; j++) {
                if (enemies[i][j].activated) {
                    enemies[i][j].rect.x += enemies[i][j].speed;
                    if ((enemy_area.rect.x + enemy_area.rect.width >= SCREEN_W - SCREEN_PADDING_X) || (enemy_area.rect.x <= SCREEN_PADDING_X)) enemies[i][j].speed *= -1.0f;
                }
            }
        }

        if (enemies_alive <= 0) {
            for (int i = 0; i < ENEMIES_ROW; i++) {
                for (int j = 0; j < ENEMIES_COL; j++) {
                    enemies[i][j].rect.x = (enemies[i][j].rect.width + ENEMIES_GAP) * j + enemy_area.rect.x;
                    enemies[i][j].rect.y = (enemies[i][j].rect.height + ENEMIES_GAP) * i + enemy_area.rect.y;
                    enemies[i][j].speed = enemy_area.speed;
                    enemies[i][j].activated = 1;
                }
            }
            enemies_alive = ENEMIES_COUNT;
        }

        BeginDrawing();

        ClearBackground(GetColor(0x2f4f4fff));

        DrawRectangleRec(ship.rect, ship.color);

        for (int i = 0; i < BULLETS_COUNT; i++)
            if (bullets[i].activated) DrawRectangleRec(bullets[i].rect, bullets[i].color);

        for (int i = 0; i < ENEMIES_ROW; i++) {
            for (int j = 0; j < ENEMIES_COL; j++) {
                    if (enemies[i][j].activated) DrawRectangleRec(enemies[i][j].rect, enemies[i][j].color);
            }
        }

        DrawRectangleLinesEx(enemy_area.rect, enemy_area.thickness, enemy_area.color);

        DrawText(TextFormat("Score: %i", score), 10, SCREEN_H - 45 + 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
