#include <keypadc.h>
#include <graphx.h>
#include <tice.h>

#include "gfx/gfx.h" 

//screen resolution is 320 x 240
//alien color is rgb(140, 194,  74)

struct Alien {
    int Xcoor;
    int Ycoor;
    int dead;
};

struct Shot {
    int Xcoor;
    int Ycoor;
};

struct Player {
    int Xcoor;
};

void draw_menu();
int game_over();
void p_shoot();
void a_shoot();
void update();
void draw();
int stage = 0;
int level = 2;
int ticker = 0;
int shot_tick = 0;
char direction = 'R';
int game_over_var = 0;
const char game_name0[] = "SPACE";
const char game_name1[] = "INVADERS";
const char creator[] = "space invaders clone created by mes3n";
struct Player player;
struct Shot p_shots[15];
struct Shot a_shots[30];
struct Alien army[4][10];


int main(void) {

    os_ClrHome();
    gfx_Begin();
    gfx_SetDrawBuffer();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(1);

    kb_key_t key;

    do {
        player.Xcoor = 160;

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 10; x++) {
                army[y][x].Xcoor = (x*23)+11;
                army[y][x].Ycoor = (y*19)+11;
                army[y][x].dead = 0;
            }
        }

        for (int i = 0; i < 30; i++) {
            a_shots[i].Xcoor = 0;
            a_shots[i].Ycoor = 0;
        }
        for (int i = 0; i < 15; i++) {
            p_shots[i].Xcoor = 0;
            p_shots[i].Ycoor = 0;
        }

        ticker = 0;
        shot_tick = 0;
        direction = 'R';
        game_over_var = 0;

        draw_menu();
        if (!stage)
            stage = 10;
        delay(1000);
        do {
            draw_menu();
            kb_Scan();

            if (kb_Data[3] == kb_0)
                level = -2;
            if (kb_Data[3] == kb_1)
                level = 2;
            if (kb_Data[4] == kb_2)
                level = 3;
            if (kb_Data[5] == kb_3)
                level = 6;



        } while (kb_Data[7] != kb_Up && kb_Data[7] != kb_Left && kb_Data[7] != kb_Right && kb_Data[6] != kb_Clear);


        do {
            kb_Scan();

            if (kb_Data[6] == kb_Clear)
                return 0;

            key = kb_Data[7];

            (key & kb_Right ? player.Xcoor += 4  : false);
            (key & kb_Left ? player.Xcoor -= 4 : false);
            (key & kb_Up ? p_shoot(player.Xcoor+10, 200, ticker) : false);

            if (player.Xcoor <= 10)
                player.Xcoor = 10;
            else if (player.Xcoor >= 285)
                player.Xcoor = 285;

            if (ticker % (10 - level) == 0) {
                a_shoot();
            }

            ticker++;
            update();

        } while (!game_over() && !game_over_var);

        /*This will be the death animation*/

    } while (kb_Data[6] != kb_Clear);

    gfx_End();
    return 0;
}


void update() {
    if (army[3][9].Xcoor >= 290) {
        direction = 'L';
        for (int z = 0; z < 3; z++) {
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 10; x++) {
                    army[y][x].Ycoor += (level == 6 ? 2 : (level == -2 ? 0 : 1));
                }
            }
            draw();
        }
    }

    else if (army[0][0].Xcoor <= 10) {
        direction = 'R';
        for (int z = 0; z < 3; z++) {
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 10; x++) {
                    army[y][x].Ycoor += (level == 6 ? 2 : (level == -2 ? 0 : 1));
                }
            }
            draw();
        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 10; x++) {
            switch(direction) {
            case 'R':
                army[y][x].Xcoor++;
                break;
            case 'L':
                army[y][x].Xcoor--;
                break;
            }
        }
    }

    for (int i = 0; i < 15; i++) {
        if (p_shots[i].Xcoor) {
            p_shots[i].Ycoor -= 5;
            if (p_shots[i].Ycoor <= 0) {
                p_shots[i].Xcoor = 0;
                p_shots[i].Ycoor = 0;
            }
            if (p_shots[i].Ycoor <= army[3][9].Ycoor + 18) {
                for (int y = 0; y < 4; y++) {
                    for (int x = 0; x < 10; x++) {
                        if (((army[y][x].Xcoor < p_shots[i].Xcoor && p_shots[i].Xcoor < army[y][x].Xcoor + 22) || (army[y][x].Xcoor < p_shots[i].Xcoor + 4 && p_shots[i].Xcoor + 4 < army[y][x].Xcoor + 22)) \
                        && ((army[y][x].Ycoor < p_shots[i].Ycoor && p_shots[i].Ycoor < army[y][x].Ycoor + 16) || (army[y][x].Ycoor < p_shots[i].Ycoor + 4 && p_shots[i].Ycoor + 4 < army[y][x].Ycoor + 16)) \
                        && !army[y][x].dead) {
                            army[y][x].dead = 1;
                            p_shots[i].Xcoor = 0;
                            p_shots[i].Ycoor = 0;

                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 30; i++) {
        if (a_shots[i].Xcoor) {
            a_shots[i].Ycoor += 5;
            if (a_shots[i].Ycoor >= 218) {
                a_shots[i].Xcoor = 0;
                a_shots[i].Ycoor = 0;
            }
            if (a_shots[i].Ycoor >= 190) {
                if (((player.Xcoor < a_shots[i].Xcoor && a_shots[i].Xcoor < player.Xcoor + 22) || (player.Xcoor < a_shots[i].Xcoor + 4 && a_shots[i].Xcoor + 4 < player.Xcoor + 22)) \
                && ((200 < a_shots[i].Ycoor && a_shots[i].Ycoor < 218) || (200 < a_shots[i].Ycoor + 4 && a_shots[i].Ycoor + 4 < 218))) {
                    a_shots[i].Xcoor = 0;
                    a_shots[i].Ycoor = 0;
                    stage = 3;
                    game_over_var = 1;
                }
            }
        }
    }

    draw();
}



void p_shoot(x, y, tick) {
    if (shot_tick + 10 <= tick) {
        for (int i = 0; i < 15; i++) {
            if (!p_shots[i].Xcoor) {
                p_shots[i].Xcoor = x;
                p_shots[i].Ycoor = y;
                shot_tick = tick;
                return;
            }
        }
    }
}


void a_shoot() {
    int Xspot = randInt(0, 9);
    for (int y = 3; y >= 0; y--) {
        if (!army[y][Xspot].dead) {
            for (int i = 0; i < 30; i++) {
                if (!a_shots[i].Xcoor) {
                    a_shots[i].Xcoor = army[y][Xspot].Xcoor+10;
                    a_shots[i].Ycoor = army[y][Xspot].Ycoor+16;
                    return;
                    }
                }
            }
        }
    }


int game_over () {
    if (army[3][9].Ycoor >= 180) {
        stage = 2;
        return 1;
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 10; x++) {
            if (!army[y][x].dead) {
                return 0;
            }
        }
    }

    stage = 1;
    return 1;
}

void draw_menu () {
    int x_val0;
    int x_val1;
    int x_val2;
    int x_val3;

    gfx_SetTextScale(3, 3);

    x_val0 = (320-gfx_GetStringWidth(game_name0))/2;
    x_val1 = (320-gfx_GetStringWidth(game_name1))/2;

    gfx_ZeroScreen();
    if (stage == 0) {
        for (int i = 40; i > -1; i--) {
            gfx_ZeroScreen();
            gfx_SetTextFGColor(8);
            gfx_PrintStringXY(game_name0, x_val0-((x_val0 / 40)*i)-1, 30);
            gfx_PrintStringXY(game_name1, x_val1-((x_val1 / 40)*i)-1, 60);
            gfx_SetTextFGColor(4);
            gfx_PrintStringXY(game_name0, x_val0+(((x_val0) / 40)*i)+1, 30);
            gfx_PrintStringXY(game_name1, x_val1+(((x_val1) / 40)*i)+1, 60);
            gfx_BlitBuffer();
            delay(3);
        }
    }
    if (stage != 0) {
        gfx_SetTextFGColor(8);
        gfx_PrintStringXY(game_name0, x_val0-1, 30);
        gfx_PrintStringXY(game_name1, x_val1-1, 60);
        gfx_SetTextFGColor(4);
        gfx_PrintStringXY(game_name0, x_val0+1, 30);
        gfx_PrintStringXY(game_name1, x_val1+1, 60);
    }

    gfx_TransparentSprite(ship, 140, 95);
    gfx_TransparentSprite(alien, 165, 95);

    gfx_SetTextFGColor(9);
    gfx_SetTextScale(1, 1);

    x_val2 = (320-gfx_GetStringWidth(creator))/2;
    gfx_PrintStringXY(creator, x_val2, 220);

    switch (stage) {
        case 0:
        case 10:
        x_val3 = (320-gfx_GetStringWidth("start playing with the arrows"))/2;
        gfx_PrintStringXY("start playing with the arrows", x_val3, 140);
        x_val3 = (320-gfx_GetStringWidth("set difficulty with 1, 2, 3"))/2;
        gfx_PrintStringXY("set difficulty with 1, 2, 3", x_val3, 160);
        break;

        case 1:
        gfx_SetTextFGColor(4);
        x_val3 = (320-gfx_GetStringWidth("You beat the aliens"))/2;
        gfx_PrintStringXY("You beat the aliens", x_val3, 150);
        break;

        case 2:
        gfx_SetTextFGColor(11);
        x_val3 = (320-gfx_GetStringWidth("Your planet has fallen"))/2;
        gfx_PrintStringXY("Your planet has fallen", x_val3, 150);
        break;

        case 3:
        gfx_SetTextFGColor(11);
        x_val3 = (320-gfx_GetStringWidth("Your planet lost its best defender"))/2;
        gfx_PrintStringXY("Your planet lost its best defender", x_val3, 150);
        break;
    }

    gfx_SetTextFGColor(9);
    switch (level) {
        case -2:
        x_val3 = (320-gfx_GetStringWidth("Playing as mom"))/2;
        gfx_PrintStringXY("Playing as mom", x_val3, 200);
        gfx_SetColor(4);
        break;

        case 2:
        x_val3 = (320-gfx_GetStringWidth("Easy"))/2;
        gfx_PrintStringXY("Easy", x_val3, 200);
        gfx_SetColor(4);
        break;

        case 3:
        x_val3 = (320-gfx_GetStringWidth("Medium"))/2;
        gfx_PrintStringXY("Medium", x_val3, 200);
        gfx_SetColor(6);
        break;

        case 6:
        x_val3 = (320-gfx_GetStringWidth("Hard"))/2;
        gfx_PrintStringXY("Hard", x_val3, 200);
        gfx_SetColor(11);
        break;
    }
    gfx_Rectangle(x_val3-3-50, 195, 320 - (x_val3*2) + 6+100, 20);

    gfx_BlitBuffer();
}


void draw () {
    gfx_ZeroScreen();

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 10; x++) {
            (!army[y][x].dead ? gfx_TransparentSprite(alien, army[y][x].Xcoor, army[y][x].Ycoor) : false);
        }
    }
    gfx_SetColor(6);
    for (int i = 0; i < 15; i++) {
        if (p_shots[i].Xcoor) {
            gfx_FillCircle(p_shots[i].Xcoor, p_shots[i].Ycoor, 2);
        }
    }
    gfx_TransparentSprite(ship, player.Xcoor, 200);

    gfx_SetColor(5);
    for (int i = 0; i < 30; i++) {
        if (a_shots[i].Xcoor) {
            gfx_FillCircle(a_shots[i].Xcoor, a_shots[i].Ycoor, 2);
        }
    }

    gfx_SetColor(2);
    gfx_HorizLine(9, 218, 298);

    gfx_BlitBuffer();
}
