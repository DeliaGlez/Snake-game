#include <allegro.h>
#include <iostream>
#include <ctime>
#include <sstream>

#define largo 1000
#define ancho 720
#define limite_nodos 200
#define arriba 0
#define abajo 1
#define derecha 2
#define izquierda 3

using namespace std;

//Bitmap's

BITMAP * menu;
BITMAP * menu2;
BITMAP * buffer;
BITMAP * fondo;
BITMAP * gameover;
BITMAP * cabezas;
BITMAP * cabeza;
BITMAP * nodo_cuerpo;
BITMAP * fruta;
BITMAP * cursor;
MIDI * backgroundmusic;
SAMPLE * Up;
SAMPLE * Down;
SAMPLE * Left;
SAMPLE * Right;
SAMPLE * mouse;
SAMPLE * Eat;
SAMPLE * Collision;
SAMPLE * click;

int limite_x = largo/40;
int limite_y = ancho/40;
int tam = 3;
int dir = derecha;
int fruta_x, fruta_y;
int score = 0;

//Struct's
struct Nodo{
    int x, y;
};

Nodo cuerpo[limite_nodos];

void inicio(){ //inicializa allegro y carga de imagenes
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,largo,ancho,0,0);
    srand(time(NULL));
    //install ALLEGRO SOUNDS
     if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }
    //ajustar el sonido
    set_volume(100, 100);

    buffer = create_bitmap(largo,ancho);
    menu = create_bitmap(largo,ancho);
    menu2 = create_bitmap(largo,ancho);
    fondo = create_bitmap(largo,ancho);
    gameover = create_bitmap(largo,ancho);
    cabezas = create_bitmap(160,40);
    cabeza = create_bitmap(40,40);
    nodo_cuerpo = create_bitmap(40,40);
    fruta = create_bitmap(40,40);
    cursor = create_bitmap(36,56);

    menu = load_bitmap("img/menu.bmp",NULL);
    menu2 = load_bitmap("img/menu2.bmp",NULL);
    fondo = load_bitmap("img/fondo.bmp",NULL);
    gameover = load_bitmap("img/gameover.bmp",NULL);
    //draw_sprite(menu, buffer,0,0);
    cursor = load_bitmap("img/cursor.bmp",NULL);
    cabezas = load_bitmap("img/cabezas.bmp",NULL);
    nodo_cuerpo = load_bitmap("img/cuerpo.bmp",NULL);
    fruta = load_bitmap("img/fruta.bmp",NULL);

    //songs
    backgroundmusic = load_midi("sounds/backgroundmusic.mid");
    Up = load_wav("sounds/Up.wav");
    Down = load_wav("sounds/Down.wav");
    Left = load_wav("sounds/Left.wav");
    Right = load_wav("sounds/Right.wav");
    mouse = load_sample("sounds/test.wav");
    Eat = load_wav("sounds/Eat.wav");
    Collision = load_wav("sounds/Collision.wav");
    click = load_wav("sounds/click.wav");

}

void MostrarMenu(){
    clear_keybuf();
    clear(screen);
    score = 0;
    dir = derecha;
    cuerpo[0].x = 10;
    cuerpo[0].y = 9;
    tam = 3;
    play_midi(backgroundmusic,1);
    bool salidaMenu=false;

    while (!salidaMenu and !key[KEY_ESC]){

        if (mouse_x > 372 and mouse_x < 632 and mouse_y > 576 and mouse_y < 664){
            blit(menu2,buffer,0,0,0,0,largo,ancho);

            if(mouse_b and 1){
                salidaMenu=true;
                play_sample(click,80, 200, 1000, 0);
            }

        }
        else  blit(menu,buffer,0,0,0,0,largo,ancho);
        masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,36,56);
        blit(buffer,screen,0,0,0,0,largo,ancho);
    }

}

void hitbox(){

    draw_sprite(buffer, fondo,0,0);

}

void actualizarPantalla(){
    blit(buffer, screen, 0, 0, 0, 0, largo, ancho);
}

void generar_snake(){
    //pos cabeza
    cuerpo[0].x = 10;
    cuerpo[0].y = 9;
    //generar cuerpo
    for(int i =1; i <= tam; i++){
        cuerpo[i].x = cuerpo[0].x - i;
        cuerpo[i].y  = cuerpo[0].y;
    }
}

void generar_fruta(){
    fruta_x = (rand()%(limite_x-2))+1;
    fruta_y = (rand()%(limite_y-3))+2;
    int cont = 0;
    while(cont <= tam){
        if(fruta_x == cuerpo[cont].x and fruta_y == cuerpo[cont].y){
            fruta_x = (rand()%(limite_x-2))+1;
            fruta_y = (rand()%(limite_y-3))+2;
            cont = 0;
        }
        else{
            cont++;
        }
    }

}

void dibujar_fruta(){
    draw_sprite(buffer,fruta,fruta_x*40,fruta_y*40);
}

void dibujar_snake(){
    //cabeza
    switch (dir){
        case arriba:
            blit(cabezas,cabeza,0,0,0,0,40,40);
            break;
        case abajo:
            blit(cabezas,cabeza,40,0,0,0,40,40);
            break;
        case derecha:
            blit(cabezas,cabeza,80,0,0,0,40,40);
            break;
        case izquierda:
            blit(cabezas,cabeza,120,0,0,0,40,40);
            break;
    }
    draw_sprite(buffer,cabeza,cuerpo[0].x*40,cuerpo[0].y*40);
    //dibujar cuerpo
    for (int i = 1; i <= tam; i++){
        draw_sprite(buffer,nodo_cuerpo,cuerpo[i].x*40,cuerpo[i].y*40);
    }
}

void cambiar_direccion(){
    if(key[KEY_UP]){
        if(dir != abajo){
            dir = arriba;
            play_sample(Up,80, 200, 1000, 0);
        }
    }
    else if(key[KEY_DOWN]){
        if(dir != arriba){
            dir = abajo;
            play_sample(Down,80, 200, 1000, 0);
        }
    }
    else if(key[KEY_LEFT]){
        if(dir != derecha){
            dir = izquierda;
            play_sample(Left,80, 200, 1000, 0);
        }
    }
    else if(key[KEY_RIGHT]){
        if(dir != izquierda){
            dir = derecha;
            play_sample(Right,80, 200, 1000, 0);
        }
    }
}

void game_over(){
    play_sample(Collision,80, 200, 1000, 0);
    clear_keybuf();
    clear(screen);
    draw_sprite(screen, gameover, 0,0);
    readkey();

    if(key[KEY_ESC]){
        exit(0);
    }
    else   {
        MostrarMenu();
    }

}

void mover_snake(){
     // para actualizar el cuerpo
     for(int i = tam; i > 0; i--){
        cuerpo[i] = cuerpo[i-1];
     }
    //para la posicion de la cabeza
    switch(dir){
        case arriba:
            cuerpo[0].y --;
            break;
        case abajo:
            cuerpo[0].y ++;
            break;
        case izquierda:
            cuerpo[0].x --;
            break;
        case derecha:
            cuerpo[0].x ++;
            break;
    }
    //rutinas para comer
    if(cuerpo[0].x == fruta_x and cuerpo[0].y == fruta_y){
        play_sample(Eat,80, 200, 1000, 0);
        generar_fruta();
        tam++;
        cuerpo[tam] = cuerpo[tam - 1];
        score = score + 10;
    }
    //colisiones
    if(cuerpo[0].x < 1 or cuerpo[0].x > (limite_x-2) or cuerpo[0].y < 2 or cuerpo[0].y > (limite_y-2)){
        game_over();
        generar_snake();
        //dir = derecha;
    }
    //colision del cuerpo
    for(int i = 1; i<=tam; i++){
        if(cuerpo[0].x == cuerpo[i].x and cuerpo[0].y == cuerpo[i].y){
            game_over();
            generar_snake();
            break;
        }
    }
}

void dibujar_puntos(){
    std::stringstream ceout;
    ceout << "SCORE: " << score;
    std::string s = ceout.str();
    char scores[20];
    strcpy(scores, s.c_str());
    textout_ex(buffer,font,scores,50,15,0xFFFFFF,0x79afe4);
}

int main(){

    inicio();
    MostrarMenu();
    hitbox();
    generar_snake();
    dibujar_snake();
    generar_fruta();
    dibujar_fruta();
    dibujar_puntos();
    actualizarPantalla();
    while(!key[KEY_ESC]){
        clear(buffer);
        cambiar_direccion();
        mover_snake();
        hitbox();
        dibujar_snake();
        dibujar_fruta();
        dibujar_puntos();
        actualizarPantalla();
        rest(80);
    }

return 0;
}

END_OF_MAIN();
