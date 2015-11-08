/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"

#define POSICION_RELOJES_F    46
#define POSICION_RELOJES_C_A   4
#define POSICION_RELOJES_C_B  60

#define DBG_OFF_X             25
#define DBG_OFF_Y              6
#define DBG_WIDTH             30
#define DBG_HEIGHT            38

extern int ultimo_cambio;

extern jugador_t jugadorA, jugadorB;

static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

const char reloj[] = "|/-\\";
#define reloj_size 4

debug_data_t debug_data;
uint debug_screen = FALSE;

// Memoria reservada para almacenar una copia de la pantalla
ca p_copia[VIDEO_FILS][VIDEO_COLS];

void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++)
     {
        screen_pintar(text[i], attr, y, x);

        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_dec(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}


void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
{
    int f, c;
    for (f = fila; f < fila + alto; f++)
    {
        for (c = columna; c < columna+ancho; c++)
            screen_pintar(letra, color, f, c);
    }
}

void screen_pintar_linea_h(uchar c, uchar color, int fila, int columna, int ancho)
{
    screen_pintar_rect(c, color, fila, columna, 1, ancho);
}

void screen_pintar_linea_v(uchar c, uchar color, int fila, int columna, int alto)
{
    screen_pintar_rect(c, color, fila, columna, alto, 1);
}

void screen_inicializar()
{
    screen_pintar_rect(' ', C_BG_BLACK | C_FG_WHITE, 0, 0, VIDEO_FILS, VIDEO_COLS);
    screen_pintar_rect(' ', C_BG_RED   | C_FG_WHITE, 45, 33, 5, 7);
    screen_pintar_rect(' ', C_BG_BLUE  | C_FG_WHITE, 45, 40, 5, 7);


    screen_pintar_relojes();
    screen_pintar_puntajes();

    int i, j;
    for (i = 0; i < MAPA_ALTO; i++)
    {
        for (j = 0; j < MAPA_ANCHO; j++)
            screen_actualizar_posicion_mapa(j, i);
    }
}

void screen_pintar_puntajes()
{
    print_dec(jugadorA.puntos, 3, 33+2, 45+2, C_BG_RED  | C_FG_WHITE);
    print_dec(jugadorB.puntos, 3, 40+2, 45+2, C_BG_BLUE | C_FG_WHITE);
}

uchar screen_color_jugador(jugador_t *j)
{
    if (j == NULL)
        return C_FG_LIGHT_GREY;

    if (j->index == JUGADOR_A)
        return C_FG_RED;
    else
        return C_FG_BLUE;
}

uchar screen_caracter_perro(uint tipo)
{
    if (tipo == TIPO_1) return '1';
    if (tipo == TIPO_2) return '2';
    while(1){};
    return '?';
}


void screen_pintar_reloj_perro(perro_t *perro)
{
    jugador_t *j = perro->jugador;
    uint columna = j->index == JUGADOR_A ? POSICION_RELOJES_C_A : POSICION_RELOJES_C_B;

    uchar col_libre = C_BG_BLACK | screen_color_jugador(j);

    uchar c, col;
    if (perro->libre == TRUE) {
        c = 'X';
        col = col_libre;
    } else {
        c = reloj[perro->indice_reloj];
        col = C_BW;
    }

    screen_pintar('1' + perro->index, C_BW, POSICION_RELOJES_F    , columna + perro->index * 2);
    screen_pintar(                  c,  col, POSICION_RELOJES_F + 2, columna + perro->index * 2);
}

void screen_pintar_reloj_perros(jugador_t *j)
{
    int i;
    for(i = 0; i < MAX_CANT_PERROS_VIVOS; i++)
        screen_pintar_reloj_perro(&j->perros[i]);
}

void screen_pintar_relojes()
{
    screen_pintar_reloj_perros(&jugadorA);
    screen_pintar_reloj_perros(&jugadorB);
}

void screen_actualizar_reloj_perro (perro_t *perro)
{
    perro->indice_reloj = (perro->indice_reloj + 1) % reloj_size;
    screen_pintar_reloj_perro(perro);
}


void screen_pintar_perro(perro_t *perro)
{
    uchar c     = screen_caracter_perro(perro->tipo);
    uchar color = C_MAKE_BG(screen_color_jugador(perro->jugador)) | C_FG_WHITE;

    screen_pintar(c, color, perro->y+1, perro->x);
}

void screen_borrar_perro(perro_t *perro)
{
    screen_pintar('.', C_BG_GREEN | C_FG_BLACK, perro->y+1, perro->x);
    screen_actualizar_posicion_mapa(perro->x, perro->y);
}

void screen_pintar_jugador(jugador_t *j)
{
    uchar c     = 'A' + j->index;
    uchar color = C_MAKE_BG(screen_color_jugador(j)) | C_FG_WHITE;

    screen_pintar(c, color, j->y+1, j->x);
}

void screen_borrar_jugador(jugador_t *j)
{
    screen_pintar('.', C_BG_GREEN | C_FG_BLACK, j->y+1, j->x);
    screen_actualizar_posicion_mapa(j->x, j->y);
}


uchar screen_caracter_tesoro(int valor) {
    if (valor > 100)
        return 'O';
    else
        return 'o';
}

void screen_actualizar_posicion_mapa(uint x, uint y)
{
    uchar bg = C_BG_GREEN;

    uchar letra;
    uint valor = game_huesos_en_posicion(x,y);
    perro_t *perro = game_perro_en_posicion(x, y);
    if (perro != NULL) {
        letra = screen_caracter_perro(perro->tipo);
    } else if (valor > 0) {
        letra = screen_caracter_tesoro(valor);
    } else if ((jugadorA.x_cucha == x && jugadorA.y_cucha == y) ||
        (jugadorB.x_cucha == x && jugadorB.y_cucha == y)) {
        letra = 'x';
    } else {
        letra = screen_valor_actual(y+1, x);
    }

    screen_pintar(letra, bg | C_FG_BLACK, y+1, x);

}


void screen_stop_game_show_winner(jugador_t *j) {
    int offy = 14; //(50/2 - 11);
    int offx = 20; //(80/2 - 20);

    int ancho = 40;
    int alto = 17;

    uchar color = screen_color_jugador(j);

    screen_pintar_rect(' ', C_MAKE_BG(color) | C_FG_WHITE, offy, offx, alto, ancho);

    offy++; offx++; alto -= 2; ancho-=2;

    screen_pintar_rect('*', C_BW, offy, offx, alto, ancho);

    offy++; offx++; alto -= 2; ancho-=2;

    screen_pintar_rect(' ', C_BG_LIGHT_GREY | C_FG_BLACK, offy, offx, alto, ancho);

    print("EL GANADOR ES EL JUGADOR",   offx+5  , offy+3 , C_BG_LIGHT_GREY | C_FG_BLACK);

    if(j == NULL)      print("EMPATE", offx+14, offy+6, C_MAKE_BG(color) | C_FG_BLACK);
    if(j == &jugadorA) print("<-- A",  offx+15, offy+6, C_BG_LIGHT_GREY  | color);
    if(j == &jugadorB) print("B -->",  offx+15, offy+6, C_BG_LIGHT_GREY  | color);
    // a partir de aca se termina el unviverso (STOP GAME)
    __asm __volatile( "cli\n" : : : );
    while(1){}
}

void screen_limpiar() {
    screen_pintar_rect(' ', 0, 0, 0, VIDEO_FILS, VIDEO_COLS);
}

void screen_guardar_copia() {
    int i;
    int j;
    for (i = 0; i < VIDEO_FILS; i++) {
        for (j = 0; j < VIDEO_COLS; j++) {
            p_copia[i][j] = p[i][j];
        }
    }
}

void screen_leer_copia() {
    int i;
    int j;
    for (i = 0; i < VIDEO_FILS; i++) {
        for (j = 0; j < VIDEO_COLS; j++) {
            p[i][j] = p_copia[i][j];
        }
    }
}

void screen_show_debug_mode() {
    if (debug_mode) {
        print("Modo DEBUG activado", 30, 0, C_BG_BLACK | C_FG_LIGHT_GREY);
    } else {
        print("                   ", 30, 0, C_BG_BLACK | C_FG_LIGHT_GREY);
    }
}


void screen_show_debug_info() {
    screen_guardar_copia();

    // Fondo
    screen_pintar_rect(' ', C_BG_RED, DBG_OFF_Y, DBG_OFF_X, 4, DBG_WIDTH);
    screen_pintar_rect(' ', C_BG_LIGHT_GREY, DBG_OFF_Y + 4, DBG_OFF_X, DBG_HEIGHT - 4, DBG_WIDTH);
    
    // Bordes
    screen_pintar_linea_v(' ', C_BG_BLACK, DBG_OFF_Y, DBG_OFF_X, DBG_HEIGHT);
    screen_pintar_linea_v(' ', C_BG_BLACK, DBG_OFF_Y, DBG_OFF_X + DBG_WIDTH - 1, DBG_HEIGHT);
    screen_pintar_linea_h(' ', C_BG_BLACK, DBG_OFF_Y, DBG_OFF_X, DBG_WIDTH);
    screen_pintar_linea_h(' ', C_BG_BLACK, DBG_OFF_Y + DBG_HEIGHT - 1, DBG_OFF_X, DBG_WIDTH);

    // Labels
    print("Excepcion de tipo", DBG_OFF_X + 2, DBG_OFF_Y + 2, C_BG_RED | C_FG_LIGHT_GREY);

    print("eax",        DBG_OFF_X +  2, DBG_OFF_Y +  5, C_BG_LIGHT_GREY | C_FG_RED);
    print("ebx",        DBG_OFF_X +  2, DBG_OFF_Y +  7, C_BG_LIGHT_GREY | C_FG_RED);
    print("ecx",        DBG_OFF_X +  2, DBG_OFF_Y +  9, C_BG_LIGHT_GREY | C_FG_RED);
    print("edx",        DBG_OFF_X +  2, DBG_OFF_Y + 11, C_BG_LIGHT_GREY | C_FG_RED);
    print("esi",        DBG_OFF_X +  2, DBG_OFF_Y + 13, C_BG_LIGHT_GREY | C_FG_RED);
    print("edi",        DBG_OFF_X +  2, DBG_OFF_Y + 15, C_BG_LIGHT_GREY | C_FG_RED);
    print("ebp",        DBG_OFF_X +  2, DBG_OFF_Y + 17, C_BG_LIGHT_GREY | C_FG_RED);
    print("esp",        DBG_OFF_X +  2, DBG_OFF_Y + 19, C_BG_LIGHT_GREY | C_FG_RED);
    print("eip",        DBG_OFF_X +  2, DBG_OFF_Y + 21, C_BG_LIGHT_GREY | C_FG_RED);
    print(" cs",        DBG_OFF_X +  2, DBG_OFF_Y + 23, C_BG_LIGHT_GREY | C_FG_RED);
    print(" ds",        DBG_OFF_X +  2, DBG_OFF_Y + 25, C_BG_LIGHT_GREY | C_FG_RED);
    print(" es",        DBG_OFF_X +  2, DBG_OFF_Y + 27, C_BG_LIGHT_GREY | C_FG_RED);
    print(" fs",        DBG_OFF_X +  2, DBG_OFF_Y + 29, C_BG_LIGHT_GREY | C_FG_RED);
    print(" gs",        DBG_OFF_X +  2, DBG_OFF_Y + 31, C_BG_LIGHT_GREY | C_FG_RED);
    print(" ss",        DBG_OFF_X +  2, DBG_OFF_Y + 33, C_BG_LIGHT_GREY | C_FG_RED);
    print("eflags",     DBG_OFF_X +  2, DBG_OFF_Y + 35, C_BG_LIGHT_GREY | C_FG_RED);

    print("cr0",        DBG_OFF_X + 16, DBG_OFF_Y +  5, C_BG_LIGHT_GREY | C_FG_RED);
    print("cr2",        DBG_OFF_X + 16, DBG_OFF_Y +  7, C_BG_LIGHT_GREY | C_FG_RED);
    print("cr3",        DBG_OFF_X + 16, DBG_OFF_Y +  9, C_BG_LIGHT_GREY | C_FG_RED);
    print("cr4",        DBG_OFF_X + 16, DBG_OFF_Y + 11, C_BG_LIGHT_GREY | C_FG_RED);
    print("error code", DBG_OFF_X + 16, DBG_OFF_Y + 14, C_BG_LIGHT_GREY | C_FG_RED);
    print("stack",      DBG_OFF_X + 16, DBG_OFF_Y + 22, C_BG_LIGHT_GREY | C_FG_RED);

    // Contenido
    print_dec(debug_data.tipo,   2, DBG_OFF_X + 20,            DBG_OFF_Y + 2, C_BG_RED | C_FG_LIGHT_GREY);  // tipo
    print    (debug_data.mnemonico, DBG_OFF_X + DBG_WIDTH - 5, DBG_OFF_Y + 2, C_BG_RED | C_FG_LIGHT_GREY);  // mnemonico

    print_hex(debug_data.eax,        8, DBG_OFF_X +  6, DBG_OFF_Y +  5, C_BG_LIGHT_GREY | C_FG_BLACK);  // eax
    print_hex(debug_data.ebx,        8, DBG_OFF_X +  6, DBG_OFF_Y +  7, C_BG_LIGHT_GREY | C_FG_BLACK);  // ebx
    print_hex(debug_data.ecx,        8, DBG_OFF_X +  6, DBG_OFF_Y +  9, C_BG_LIGHT_GREY | C_FG_BLACK);  // ecx
    print_hex(debug_data.edx,        8, DBG_OFF_X +  6, DBG_OFF_Y + 11, C_BG_LIGHT_GREY | C_FG_BLACK);  // edx
    print_hex(debug_data.esi,        8, DBG_OFF_X +  6, DBG_OFF_Y + 13, C_BG_LIGHT_GREY | C_FG_BLACK);  // esi
    print_hex(debug_data.edi,        8, DBG_OFF_X +  6, DBG_OFF_Y + 15, C_BG_LIGHT_GREY | C_FG_BLACK);  // edi
    print_hex(debug_data.ebp,        8, DBG_OFF_X +  6, DBG_OFF_Y + 17, C_BG_LIGHT_GREY | C_FG_BLACK);  // ebp
    print_hex(debug_data.esp,        8, DBG_OFF_X +  6, DBG_OFF_Y + 19, C_BG_LIGHT_GREY | C_FG_BLACK);  // esp
    print_hex(debug_data.eip,        8, DBG_OFF_X +  6, DBG_OFF_Y + 21, C_BG_LIGHT_GREY | C_FG_BLACK);  // eip
    print_hex(debug_data.cs,         4, DBG_OFF_X +  6, DBG_OFF_Y + 23, C_BG_LIGHT_GREY | C_FG_BLACK);  // cs
    print_hex(debug_data.ds,         4, DBG_OFF_X +  6, DBG_OFF_Y + 25, C_BG_LIGHT_GREY | C_FG_BLACK);  // ds
    print_hex(debug_data.es,         4, DBG_OFF_X +  6, DBG_OFF_Y + 27, C_BG_LIGHT_GREY | C_FG_BLACK);  // es
    print_hex(debug_data.fs,         4, DBG_OFF_X +  6, DBG_OFF_Y + 29, C_BG_LIGHT_GREY | C_FG_BLACK);  // fs
    print_hex(debug_data.gs,         4, DBG_OFF_X +  6, DBG_OFF_Y + 31, C_BG_LIGHT_GREY | C_FG_BLACK);  // gs
    print_hex(debug_data.ss,         4, DBG_OFF_X +  6, DBG_OFF_Y + 33, C_BG_LIGHT_GREY | C_FG_BLACK);  // ss
    print_hex(debug_data.eflags,     8, DBG_OFF_X +  9, DBG_OFF_Y + 35, C_BG_LIGHT_GREY | C_FG_BLACK);  // eflags

    print_hex(debug_data.cr0,        8, DBG_OFF_X + 20, DBG_OFF_Y +  5, C_BG_LIGHT_GREY | C_FG_BLACK);  // cr0
    print_hex(debug_data.cr2,        8, DBG_OFF_X + 20, DBG_OFF_Y +  7, C_BG_LIGHT_GREY | C_FG_BLACK);  // cr2
    print_hex(debug_data.cr3,        8, DBG_OFF_X + 20, DBG_OFF_Y +  9, C_BG_LIGHT_GREY | C_FG_BLACK);  // cr3
    print_hex(debug_data.cr4,        8, DBG_OFF_X + 20, DBG_OFF_Y + 11, C_BG_LIGHT_GREY | C_FG_BLACK);  // cr4
    print_hex(debug_data.error_code, 8, DBG_OFF_X + 16, DBG_OFF_Y + 16, C_BG_LIGHT_GREY | C_FG_BLACK);  // error code

    print_hex(debug_data.stack[0],   8, DBG_OFF_X + 16, DBG_OFF_Y + 24, C_BG_LIGHT_GREY | C_FG_BLACK);  // stack0
    print_hex(debug_data.stack[1],   8, DBG_OFF_X + 16, DBG_OFF_Y + 25, C_BG_LIGHT_GREY | C_FG_BLACK);  // stack1
    print_hex(debug_data.stack[2],   8, DBG_OFF_X + 16, DBG_OFF_Y + 26, C_BG_LIGHT_GREY | C_FG_BLACK);  // stack2
    print_hex(debug_data.stack[3],   8, DBG_OFF_X + 16, DBG_OFF_Y + 27, C_BG_LIGHT_GREY | C_FG_BLACK);  // stack3
    print_hex(debug_data.stack[4],   8, DBG_OFF_X + 16, DBG_OFF_Y + 28, C_BG_LIGHT_GREY | C_FG_BLACK);  // stack4

    debug_screen = TRUE;
}

void screen_hide_debug_info() {
    screen_leer_copia();
    debug_screen = FALSE;
}
