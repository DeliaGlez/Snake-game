#include <windows.h>
#include <iostream>
#include <conio.h>  //_getch, kbhit, gotoxy

using namespace std;

const int ancho = 30;

const int alto = 15;

int  score;

//Variables para direccion de movimiento/
int direccion=0; //0=derecha, 1=izquierda, 2=abajo, 3=arriba

//coordenadas de inicio/
int posx=12,posy=8;

char t;   //flecha presionada

//Esta funcion mueve el cursor a una coordenada en modo texto/
void gotoxy(int x,int y){
      HANDLE hcon;
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);
      COORD dwPos;
      dwPos.X = x;
      dwPos.Y= y;
      SetConsoleCursorPosition(hcon,dwPos);
 }


//Funcion que dibuja la vibora en posiciones x,y/
void dibujarVibora(){
//tablero
{

for(int i = 0; i < ancho+2; i++)
cout << "#";
cout << endl ;
for (int i = 0; i < alto ; i++) {
for (int j = 0; j < ancho; j++) {
if (j == 0)
cout << "# "; //paredes
else {
bool relleno = false;
if (!relleno) cout << " ";
}

if (j == ancho -1)
cout << "#";
}
cout << endl;
}

for (int i = 0; i< ancho+2; i++)
cout << "#";
cout << endl;
cout << "Score:" << score << endl ;
}
//vibora
gotoxy(posx,posy);
cout<<"*";


}

//funcion para inicializar movimiento y esperar que presionen
//una tecla
char moverVibora(){
char tecla;
//Hacer este ciclo mientras no se presione una tecla
do{
system("cls");  //borramos pantalla
switch(direccion){
case 0:  posx++; break;
case 1:  posx--; break;
case 2:  posy++; break;
case 3:  posy--; break;
}

//dibujamos la vibora
dibujarVibora();
Sleep(30);  //pausamos en milisegundos

}while(!kbhit());

tecla=_getch();  //al presionar se asigna a la variable tecla

return tecla;

}

/*
Funcion que checa flecha presionada para
cambiar direccion... el juego
*/
void iniciarJuego(){
do{
t = moverVibora();

switch(t){
case 72:  //si presionan flecha arriba
        direccion=3;
        break;
case 80:  //si presionan flecha abajo
        direccion=2;
        break;
case 77:  //si presionan flecha derecha
        direccion=0;
        break;
case 75:  //si presionan flecha izquierda
        direccion=1;
        break;
}
}while(t != 27); //mientras no presionen ESC
}



int main(){
iniciarJuego();

return 0;
}
