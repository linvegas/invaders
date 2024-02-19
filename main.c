#include <raylib.h>

#include <stdio.h>
#include <string.h>

#define N_BULLETS 10
#define N_ENEMIES 10

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

    Enemy enemies[N_ENEMIES];

    for (int i = 0; i < N_ENEMIES; i++) {
        enemies[i].rect.x = 40 * i;
        enemies[i].rect.y = SCREEN_H * 0.12;
        // enemies[i].rect.y = GetRandomValue(0, 60);
        enemies[i].rect.width = 30;
        enemies[i].rect.height = 30;
        enemies[i].speed = 2;
        enemies[i].activated = 1;
        enemies[i].color = LIGHTGRAY;
    }

    // Enemy enemy = {
    //     .rect.x = SCREEN_W/2, .rect.y = SCREEN_H * 0.12,
    //     .rect.width = 30,     .rect.height = 30,
    //     .speed = 2,
    //     .activated = 1,
    //     .color = LIGHTGRAY,
    // };

    Bullet bullets[N_BULLETS];

    for (int i = 0; i < N_BULLETS; i++) {
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

    while(!WindowShouldClose()) {

        // for (int i = 0; i < N_BULLETS; i++) {
        //     printf("%d ", bullets[i].activated);
        // }
        // printf("\n");

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L)) ship.rect.x += 2.0f * ship.speed;
        if (IsKeyDown(KEY_LEFT) ||  IsKeyDown(KEY_H)) ship.rect.x -= 2.0f * ship.speed;

        if (ship.rect.x + ship.rect.width >= SCREEN_W) ship.rect.x = SCREEN_W - ship.rect.width;
        if (ship.rect.x <= 0) ship.rect.x = 0;

        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 0; i < N_BULLETS; i++) {
                if (!bullets[i].activated) {
                    bullets[i].rect.x = ship.rect.x + ((ship.rect.width / 2) - (bullets[i].rect.width / 2));
                    bullets[i].rect.y = SCREEN_H - SCREEN_H * 0.1;
                    bullets[i].activated = 1;
                    break;
                }
            }
        }

        for (int i = 0; i < N_BULLETS; i++) {
            if (bullets[i].activated) {

                bullets[i].rect.y -= bullets[i].speed;

                if (bullets[i].rect.y + bullets[i].rect.height < 0) {
                    bullets[i].activated = 0;
                }

                for (int j = 0; j < N_ENEMIES; j++) {
                    if (enemies[j].activated) {
                        if (CheckCollisionRecs(bullets[i].rect, enemies[j].rect)) {
                            score++;
                            bullets[i].activated = 0;
                            enemies[j].activated = 0;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < N_ENEMIES; i++) {
            if (enemies[i].activated) {
                enemies[i].rect.x += enemies[i].speed;
                if ((enemies[i].rect.x + enemies[i].rect.width >= SCREEN_W) || (enemies[i].rect.x <= 0)) enemies[i].speed *= -1.0f;
            } /* else {
               enemies[i].rect.x = GetRandomValue(50, SCREEN_W - (50 + enemies[i].rect.width));
               enemies[i].rect.y = SCREEN_H * 0.12;
               enemies[i].speed = (GetRandomValue(0, 1) % 2 == 0) ? 2 : -2;
               enemies[i].activated = 1;
            } */
        }

        // if (enemy.activated) {
        //     enemy.rect.x += enemy.speed;
        //     if ((enemy.rect.x + enemy.rect.width >= (SCREEN_W - 50)) || (enemy.rect.x <= 50)) enemy.speed *= -1.0f;
        // } else {
        //     enemy.rect.x = GetRandomValue(50, SCREEN_W - (50 + enemy.rect.width));
        //     enemy.rect.y = SCREEN_H * 0.12;
        //     enemy.speed = (GetRandomValue(0, 1) % 2 == 0) ? 2 : -2;
        //     enemy.activated = 1;
        // }

        BeginDrawing();

        ClearBackground(GetColor(0x2f4f4fff));

        DrawRectangleRec(ship.rect, ship.color);

        for (int i = 0; i < N_BULLETS; i++)
            if (bullets[i].activated) DrawRectangleRec(bullets[i].rect, bullets[i].color);

        for (int i = 0; i < N_ENEMIES; i++)
            if (enemies[i].activated) DrawRectangleRec(enemies[i].rect, enemies[i].color);

        DrawText(TextFormat("Score: %i", score), 10, SCREEN_H - 45 + 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
