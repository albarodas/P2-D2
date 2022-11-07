// NOMBRAMOS LIBRERIAS A UTILIZAR:
// INFORMACIÓN DE PROYECTO:
// CURSO: ELECTRÓNICA DIGITAL 2.
// CATEDRÁTICO: JOSÉ MORALES.
// INTEGRANTES DEL GRUPO: ALBA RODAS (20036), DANIELA GODINEZ ()
// VIDEOJUEGO: INSPIRADO EN LA TEMATICA DE ARCADE DANCE GAMES, PERO CON BOTONES AJA.
////////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANTE: BASADO EN LOS EJEMPLOS DISTRIBUIDOS EN CLASE, SE IMPLEMENTÓ EL SIGUIENTE CÓDIGO.
// NOTA IMPORTANTE: LA VARIABLE QUE ME INDICA CUANDO PASA QUE, ES LA DE "variable_SD"
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// NOMBRAMOS LIBRERIAS A USAR:
#include <SPI.h>
#include <SD.h>
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_trandom_01_04pes.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/srandom_01_04sctl.h"
#include "driverlib/timer.h"

// LIBRERIAS CREADAS ".h"
#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"


// DEFINICIÓN DE PINES DE LA LCD - ILI3941:
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
//
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
const int variable_CS = 12;

// CONFIGURACIÓN DE FUNCIONES PARA LCD:
// TOMADO DE LABORATORIOS DE DIGITAL 2 - CUANDO HICIMOS EL EDIFICIO, Y ASÍ...

void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int random_01_041, unsigned int x2, unsigned int random_01_042);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int random_01_04, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int random_01_04, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int random_01_04, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int random_01_04, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int random_01_04, int fontSivariable_SDe, int color, int background);
void LCD_Bitmap(unsigned int x, unsigned int random_01_04, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int random_01_04, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);
// FIN DE FUNCIONES A UTILIZAR.

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLES PARA CONTROLES:
// PLAYER #1: ESTE CONTROLA LA SELECCIÓN DE NIVEL, Y ASI:
// UP_P1 = UP, PLAYER #1.
// DOWN_P1 = DOWN, PLAYER #1.
// LEFT_P1 = LEFT, PLAYER #1.
// RIGHT_P1 = RIGHT, PLAYER #1.
// * LO MISMO APLICA PARA EL PLAYER#2
uint8_t UP_P1;             
uint8_t DOWN_P1;             
uint8_t LEFT_P1;             
uint8_t RIGHT_P1;             

// PLAYER #2: ESTE CONTROLA LA SELECCIÓN DE NIVEL, Y ASI:
// UP_P1 = UP, PLAYER #1.
// DOWN_P1 = DOWN, PLAYER #1.
// LEFT_P1 = LEFT, PLAYER #1.
// RIGHT_P1 = RIGHT, PLAY4ER #1.
uint8_t UP_P2;             
uint8_t DOWN_P2;            
uint8_t LEFT_P2;             
uint8_t RIGHT_P2;             

// PLAYER #2: ESTE BASICAMENTE ESPERA A QUE "PLAYER 1" DECIDA, JIJI.
// SECCION DE VARIABLES A UTILIZAR:
uint8_t COUNTER_P1 = 0;     
uint8_t COUNTER_P2 = 0; 
uint8_t variable_SD = 1;          
uint8_t velocidad_variable = 0;  
// JUST PROBANDO:
int BUZ = 18;    // PIN 18 --> PROBANDO EN TIVA

// LISTADO DE VALORES NUMÉRICOS: 
// ¿DONDE SE USAN?
// CUENTAS REGRESIVAS, ETC
const char* NUMBERS[10] = {"0","1","2","3","4","5","6","7","8","9"};  

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// DEFINIMOS BOTONES
// DEFINICIÓN DE VARIABLES:
// U = UP, D = DOWN, L = LEFT, R = RIGHT.
// ESTO APLICA PARA AMBOS JUGADORES.
// ESTOS PINES SON LOS QUE DEBES CAMBIAR:

#define UP_P1 PA_6
#define DOWN_P1 PA_7
#define LEFT_P1 PE_3
#define RIGHT_P1 PE_2
 
#define UP_P2 PD_7
#define DOWN_P2 PF_0
#define LEFT_P2 PA_3
#define RIGHT_P2 PA_2

// NOTAS MUSICALES Y FRECUENCIAS:
// INTENTO #1:
// EJEMPLOS VISTOS PARA MUSICA CON ARDUINO:
// https://www.automatizacionparatodos.com/musica-con-arduino-/
// NOTAS PRINCIPALES:

//int Do = 261;
//int Re = 293;
//int Mi = 329;
//int Fa = 349;
//int Sol = 392;
//int La = 440;
//int Si = 493;
// CANCIONCITA DEL INICIO:
// VALORES DE FRECUENCIA APROX.

#define NOTA_SI  990
#define NOTA_LA  880
#define NOTA_SOL 850
#define SOUND_01 520
#define SOUND_02 805

// CANCION DEL FINAL, DESPUÉS DE GANADO/PERDIDO
#define NOTA_MI   680
#define NOTA_LA   900
#define NOTA_DO_EXTRA  560
#define NOTA_MI   1320
#define NOTA_SOL 790
#define NOTA_DOOO  1150
#define NOTA_SI   995
#define NOTA_RE   1200
#define NOTA_FA  700
#define NOTA_RE_   560

void beep(int note, int duration)     
{                                    
  tone(BUZ, note, duration/2);
  delay(duration/2);
  noTone(BUZ);
  delay(duration/2 + 20);  
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// SETUP
void setup(void)
{
  // DEFINIMOS BOTONES COMO INPUTS:
  // BOTONES PARA JUGADOR #1:
  pinMode(UP_P1, INPUT);
  pinMode(DOWN_P1, INPUT);
  pinMode(LEFT_P1, INPUT);
  pinMode(RIGHT_P1, INPUT);

// BOTONES PARA JUGADOR #2:
  pinMode(UP_P2, INPUT);
  pinMode(DOWN_P2, INPUT);
  pinMode(LEFT_P2, INPUT);
  pinMode(RIGHT_P2, INPUT);
  
// PARA EL BUZZER, DEFINIMOS COMO SALIDA:
  pinMode(BUZ,OUTPUT); // Pin conectado al BUvariable_SD PF2

  // INICIAMOS COMUNICACIÓN SERIAL
  Serial.begin(9600);

  // SETEAMOS LA LCD LA LCD
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  // INICIALIZAMOS EL SERIAL:
  Serial.begin(9600);
  // SETEAMOS LAS FUNCIONES PERTINENTES DE LA TIVA:
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  // INICIALIZAR LA LCD, CON LIBRERIA random_01_04A INCLUÍDA:
  LCD_Init();
  // LIMPIAMOS LA LCD:
  LCD_Clear(0xFFFF);
}
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void loop (void) 
{
// ESTE 'TAG', ME PERMITE VOLVER AL FINALIZAR MIS RUTINAS, random_01_04 EVITAR ERRORES EN EL LOOP.
// ESTO, Y QUE NO QUIERO QUE SE REPITA TOOODOOO, SOLO DESDE CIERTO PUNTO
empezar:

 // INICIAMOS EN EL LOOP:
 // PARA MOSTRAR LA PANTALLA DE INICIO - PRIMER MENÚ:
  while (variable_SD == 1)              
  {
    // SETEAMOS TODO EN CERO, VARIBLE PARA ANTIRREBOTE, LA we_read DE LOS BOTONES, MOSTREO DE CONTEO REGRESIVO...
    uint8_t we_read = LOW;    
    uint8_t anti_rebote = 0;            
    uint8_t NUMBERS = 0;
// SIEMPRE Y CUANDO YS HAYAMOS REALIZADO LO ANTEIROR, DE MOSTRAR LA PANTALLA, PROCEDEMOS A:
    while (variable_SD == 1) 
    { 
      // DAMOS LA BIENVENIDA AL USUARIO, MOSTRAMOS EL NOMBRE DEL JUEGO Y PEDIMOS RESPUESTA DEL USUARIO:
      // PRIMER TITULO:
      String titulo_01 = "<- JUST DANCE ->";  
      // SEGUNDO TITULO:
      String titulo_listo = "READY?";   
      // TERCER TITULO:              
      String texto_ans = "PRESS BOTÓN #1";   
         
      // MOSTRAMOS POR FULAS LOS DISTINTOS TITULOS:      
      LCD_Print(titulo_01, 30, 70, 2,  0x0000, 0xFFFF);    
      // LOS TITULOS VAN EN NEGRO, FONDO BLANCO, VAMOS VARIANDO LA ALTURA DE MOSTREO DE ESTOS TITULOS:
      LCD_Print(titulo_listo, 120, 100, 2,  0x0000, 0xFFFF); 
      // LOS TITULOS VAN EN NEGRO, FONDO BLANCO, VAMOS VARIANDO LA ALTURA DE MOSTREO DE ESTOS TITULOS:   
      LCD_Print(texto_ans, 90, 130, 2,  0x0000, 0xFFFF);    

      // AL NO HABER INICIADO LA CUENTA REGRESIVA, TENEMOS QUE ESPERAR A QUE LA CANCIÓN TERMINE, PARA PODER INICIAR A JUGAR. 
      // ESTO ES VITAAAAAAAAAAAAAAAAAAL:
      if (NUMBERS == 0)    
      {
        // EL BEEP LO VI EN UN EJEMPLO EN YOUTUBE.
        beep(NOTA_SI,500);
        beep(NOTA_LA,500);
        beep(NOTA_SOL,500);
        delay(500);
      
        beep(NOTA_SI,500);
        beep(NOTA_LA,500);
        beep(NOTA_SOL,500);
        delay(500);
      
        beep(NOTA_SOL,500);
        beep(NOTA_SOL,500);
        beep(NOTA_LA,500);
        beep(NOTA_LA,500);
        beep(NOTA_SI,500);
        beep(NOTA_LA,500);
        beep(NOTA_SOL,500);
        delay(500);
        // ACTIVAMOS 'BANDERA', PARA QUE LA CANCIÓN Y TERMINE DE SONAR:
        NUMBERS = 1;
      }
      // AHORA ESPERAMOS RESPUESTA DEL USUARIO, PARA ESTO SE DEBE PRESIONAR EL BOTON 1, DEL JUGADOR #1:
      we_read = digitalRead(UP_P1);        1
      // ANTIRREBOTE PARA LOS BOTONES INTEGRADOS EN LOS PINES DE LA TIVA PARA PROBAR EL FUNCIONAMIENTO:
      if (we_read == HIGH && anti_rebote == 0)    
      {
        anti_rebote = 1;                          
      }
      // CUANDO EL JUGADOR random_01_04A HAY SOLTADO EL BOTÓN:
      if (we_read == LOW && anti_rebote == 1)     
      {
        // PARA QUE LAS LETRAS SE NOTEN, EL FONDO SE LIMPIA Y SETEA EN BLANCO:}
        // CLEAR A TODO, INCLUSO A VARIABLE DEL ANTIRREBOTE.
        LCD_Clear(0xFFFF);                            
        anti_rebote = 0;                                          
        variable_SD = 2;
        // DAMOS COLOR AL MENÚ DE VELOCIDADES: BUSCAR COLORES EN CALCULADORA VISTA EN CLASE.
        // RELLENO DE COLOR, FUSCIA Y VERDE AZULADO.
        FillRect(0, 0, 160, 120, 0x7FF);              
        FillRect(160, 120, 160, 120, 0xF81F);           

      }
    }
  }
  // FIN A LA PRIMERA PARTE DEL LOOP :)
  // INICIA CONFIGURACION DEL MENU DE VELOCIDADES:
  // ASIGNAMOS VALOR DE 2 PARA ACTIVAR EL SEGUNDO MENU:
  while (variable_SD == 2) 
  {
    // LAS VARIABLES A RECIBIR SE INTERPRETAN DE TAMAÑO DE 8 BITS, SEGÚN LO ESPERADO POR LOS CONTROLES (MANDOS):
    // INICIALIZAMOS EN CERO CADA VARIABLE, DE ACUERDO A LO QUE REPRESENTAN:
    uint8_t velocidad_FACIL = LOW;   
    uint8_t velocidad_NORMAL = LOW;   
    uint8_t velocidad_DIFICIL = LOW;   
    uint8_t velocidad_HARDCORE = LOW;   
    uint8_t anti_rebote = 0;    
    
  // MIENTRAS SE MUESTRE EL MENU DE VELOCIDAEDS:
    while (variable_SD == 2) 
    {
      // EL BOTON DE UP, DEL JUGADOR 1, SIGUE FUNCIONANDO PARA EL SEGUNDO MENU:
      String titulo_listo = "MODE: FACIL";                       
      String texto_ans = "UP BUTTON";                    
      // MOSTRAMOS LOS STRINGS DE ARRIBA, DE COLOR NEGRO random_01_04 EN FONDO FUSCIA, UBICACIÓN: ESQUINA SUPERIOR IZQUIERDA, TEXTO NEGRO:   
      LCD_Print(titulo_listo, 45, 45, 1,  0x0000, 0x7FF);     
      LCD_Print(texto_ans, 45, 55, 1,  0x0000, 0x7FF);     

      // MOSTRAMOS LA SEGUNDA VELOCIDAD EN EL MENU, PERO AHORA EN LA ESQUINA SUPERIOR DERECHA:
      String text3 = "MODE: NORMAL";                           
      String text4 = "RIGHT BUTTON";                          
      // MOSTRAMOS LOS STRINGS DE ARRIBA, DE COLOR NEGRO Y EN FONDO BLANCO, UBICACIÓN: ESQUINA SUPERIOR IZQUIERDA, TEXTO NEGRO:   
      LCD_Print(text3, 40 + 155, 45, 1,  0x0000, 0xFFFF);     
      LCD_Print(text4, 45 + 151, 55, 1,  0x0000, 0xFFFF);     

      // MOSTRAMOS LA TERCERA VELOCIDAD EN EL MENU, PERO AHORA EN LA ESQUINA INFERIOR IZQUIERDA:
      String text5 = "MODE: DIFICIL";                             
      String text6 = "DOWN BUTTON";                           
      // MOSTRAMOS LOS STRINGS DE ARRIBA, DE COLOR NEGRO Y EN FONDO FUSCIA, UBICACIÓN: ESQUINA SUPERIOR IZQUIERDA, TEXTO NEGRO:   
      LCD_Print(text5, 30, 45 + 120, 1,  0x0000, 0xFFFF);     
      LCD_Print(text6, 38, 55 + 120, 1,  0x0000, 0xFFFF);    

      // MOSTRAMOS LA TERCERA VELOCIDAD EN EL MENU, PERO AHORA EN LA ESQUINA INFERIOR DERECHA: ESQUINA SUPERIOR IZQUIERDA, TEXTO NEGRO:   
      String text7 = "MODE: EXTREMO";                              
      String text8 = "LEFT BUTTON";     
      // MOSTRAMOS LOS STRINGS DE ARRIBA, DE COLOR NEGRO Y EN FONDO FUSCIA, UBICACIÓN: ESQUINA SUPERIOR IZQUIERDA, TEXTO NEGRO:                            
      LCD_Print(text7, 45 + 149, 45 + 120, 1,  0x0000, 0xF81F);   
      LCD_Print(text8, 45 + 153, 55 + 120, 1,  0x0000, 0xF81F);   

      // AQUI ASIGNAMOS, DEPENDIENDO LA LECTURA DE LOS BOTONES DEL JUGADOR #1:
      velocidad_FACIL = digitalRead(UP_P1);          
      velocidad_NORMAL = digitalRead(RIGHT_P1);         
      velocidad_DIFICIL = digitalRead(DOWN_P1);          
      velocidad_HARDCORE = digitalRead(LEFT_P1);          

      // AL SELECCIONAR LA VELOCIDAD, Y SI EL BOTON AÚN HA PERDIDO EL ANTIRREBOTE
      if (velocidad_FACIL == HIGH && anti_rebote == 0)     
      {
        // ACTIVAMOS BANDERA DE ANTIRREBOTE:
        anti_rebote = 1;                     
      }
      // AL SELECCIONAR LA VELOCIDAD, Y SI EL BOTON AÚN HA PERDIDO EL ANTIRREBOTE:
      if (velocidad_FACIL == LOW && anti_rebote == 1)              
      {
        // AL SER EL FACTOR DE AUMENTO DE DIFICULTAD, LA VELOCIDAD DEL JUEGO: DAMOS UN DELAY MAYOR A LAS FLECHAS, PARA QUE APAREZCAN EN PANTALLA:            
        velocidad_variable = 1000; 
        // CLEAR AL ANTIRREBOTE:                  
        anti_rebote = 0;                              
        // INICIALIZAMOS LA VARIABLE PARA EL COUNTDOWN:
        variable_SD = 3;            
        // SUBRAYAMOS DE AMARILLO, LA OPCIÓN SELECCIONADA:                  
        FillRect(0, 0, 160, 120, 0xFFE0);   
        // DAMOS UN DELAY/MOSTRAR LA OPCIÓN SELECTED POR 0.5s:
        delay(500);                         
      }

      // AL SELECCIONAR LA VELOCIDAD, random_01_04 SI EL BOTON AÚN HA PERDIDO EL ANTIRREBOTE:
      if (velocidad_NORMAL == HIGH && anti_rebote == 0)   // SELECCIONÓ MODO NORMAL
      {
        // AL SER EL FACTOR DE AUMENTO DE DIFICULTAD, LA VELOCIDAD DEL JUEGO: DAMOS UN DELAY MAYOR A LAS FLECHAS, PARA QUE APAREZCAN EN PANTALLA: 
        anti_rebote = 2;                    
      }
      if (velocidad_NORMAL == LOW && anti_rebote == 2) 
      { 
        // A MEDIDA QUE LA OPCIÓN INCREMENTA EL NIVEL DED DIFICULTAD, EL DELAY DE APARICIÓN DE LAS FLECHAS SE REDUCE:            
        velocidad_variable = 600;                   
        anti_rebote = 0;     
        // AL SER EL FACTOR DE AUMENTO DE DIFICULTAD, LA VELOCIDAD DEL JUEGO: DAMOS UN DELAY MAYOR A LAS FLECHAS, PARA QUE APAREZCAN EN PANTALLA:                          
        variable_SD = 3;         
        // SUBRAYAMOS DE AMARILLO, LA OPCIÓN SELECCIONADA:                     
        FillRect(160, 0, 160, 120, 0xFFE0);  
        // DAMOS UN DELAY/MOSTRAR LA OPCIÓN SELECTED POR 0.5s:
        delay(500);                         
      }
      // AL SELECCIONAR LA VELOCIDAD, Y SI EL BOTON AÚN HA PERDIDO EL ANTIRREBOTE:
      if (velocidad_DIFICIL == HIGH && anti_rebote == 0) 
      {        
        // ACTUALIZAMOS LA VARIABLE DEL ANTIREBOTE:
        anti_rebote = 3;                              
      }

      if (velocidad_DIFICIL == LOW && anti_rebote == 3) 
      {            
       // DISMINUIMOS AÚN MÁS EL FACTOR DE DELAY DE LA APARICIÓN DE LAS FLECHAS:
        velocidad_variable = 400;                    
        anti_rebote = 0;                              
        variable_SD = 3;                    
        // MANTENEMOS LA MISMA COFIGURACION QUE LOS NIVELES PASADOS:          
        FillRect(0, 120, 160, 120, 0xFFE0);  
        // DAMOS UN DELAY/MOSTRAR LA OPCIÓN SELECTED POR 0.5s:
        delay(500);                         
      }
      // AL SELECCIONAR LA VELOCIDAD, Y SI EL BOTON AÚN HA PERDIDO EL ANTIRREBOTE:
      if (velocidad_HARDCORE == HIGH && anti_rebote == 0) {                       
        anti_rebote = 4;                         
      }
      // AL SELECCIONAR LA VELOCIDAD, Y SI EL BOTON AÚN HA PERDIDO EL ANTIRREBOTE:
      if (velocidad_HARDCORE == LOW && anti_rebote == 4) 
      {          
        velocidad_variable = 1;                     
        anti_rebote = 0;   
        // AL SER EL FACTOR DE AUMENTO DE DIFICULTAD, LA VELOCIDAD DEL JUEGO: DAMOS UN DELAY MAYOR A LAS FLECHAS, PARA QUE APAREZCAN EN PANTALLA:                           
        variable_SD = 3;   
         // MANTENEMOS LA MISMA COFIGURACION QUE LOS NIVELES PASADOS:                           
        FillRect(160, 120, 160, 120, 0xFFE0);  
        // DAMOS UN DELAY/MOSTRAR LA OPCIÓN SELECTED POR 0.5s:
        delay(500);                        
      }
    }
  }
  
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  // CONFIGURACION DE LA CUENTA REGRESIVA:
  // INICIAR LA CUENTA REGRESIVA, CUANDO UNO DE LOS NIVELES HAY SIDO SELECCIONADO:
  if (variable_SD == 3) 
  {            
    // CADA VEZ QUE APARECE UN NÚMERO, LIMPIAMOS LA PANTALLA, SETEANDOLA NUEVAMENTE EN CERO.               
    LCD_Clear(0xFFFF);                    
    String text_countdown_03 = "3";                  
    LCD_Print(text_countdown_03, 158, 116, 2,  0x0000, 0xFFFF);  
    delay(200);    
                          
    // CADA VEZ QUE APARECE UN NÚMERO, LIMPIAMOS LA PANTALLA, SETEANDOLA NUEVAMENTE EN CERO.     
    LCD_Clear(0xFFFF);  
    // CHAR A MOSTRAR:                  
    String text_countdown_02 = "2";                  
    // DEFINIMOS LA POSICIÓN DONDE APARECERÁ EL NÚMERO, LO QUIERO EN EL CENTRO:
    LCD_Print(text_countdown_02, 158, 116, 2,  0x0000, 0xFFFF);  
    delay(200);                         
    
    // CADA VEZ QUE APARECE UN NÚMERO, LIMPIAMOS LA PANTALLA, SETEANDOLA NUEVAMENTE EN CERO.     
    LCD_Clear(0xFFFF);         
    // CHAR A MOSTRAR:                
    String text_countdown_01 = "1";                 
    // DEFINIMOS LA POSICIÓN DONDE APARECERÁ EL NÚMERO, LO QUIERO EN EL CENTRO:
    LCD_Print(text_countdown_01, 158, 116, 2,  0x0000, 0xFFFF);  
    delay(200);                           
    
    // CADA VEZ QUE APARECE UN NÚMERO, LIMPIAMOS LA PANTALLA, SETEANDOLA NUEVAMENTE EN CERO.     
    LCD_Clear(0xFFFF);                    
    // CHAR A MOSTRAR:      
    String text_countdown_GO = "¡GO!";                 
    LCD_Print(text_countdown_GO, 151, 116, 2,  0x0000, 0xFFFF);  
    // delay DE 0.2s
    delay(200);                           

    // VARIABLE QUE ME PERMITE VOLVER AL MENU:
    variable_SD = 4;                               
  }
  //
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // DEFINICION DE FORMATO PARA PANTALLA PRINCIPAL DEL JUEGO:
  // UNA VEZ SELECCIONADO EL NIVEL DE DIFICULTAD:
  // PROCEDEMOS A PASAR A LA PANTALLA PRINCIPAL DEL JUEGO:
  if (variable_SD == 4) 
  {                           
    // CLEAR A LA PANTALLA:
    LCD_Clear(0x0000);                    
    // DEFINICIÓN DE FLECHAS AT THE BOTTOM OF LA PANTALLA:
    LCD_Bitmap(290, 0, 30, 30, LEFT_01);       
    LCD_Bitmap(290, 30, 30, 30, UP_FLECHA_01);     
    LCD_Bitmap(290, 60, 30, 30, DOWN_FLECHA_01);      
    LCD_Bitmap(290, 90, 30, 30, RIGHT_FLECHA_01); 

    // LINEA DIVISORA DE SECCIÓN DE CADA JUGADOR:
    
    FillRect(0, 120, 290, 1, 0xFFFF);      
    LCD_Bitmap(290, 121, 30, 30, LEFT_01);     
    LCD_Bitmap(290, 150, 30, 30, UP_FLECHA_01);     
    LCD_Bitmap(290, 180, 30, 30, DOWN_FLECHA_01);     
    LCD_Bitmap(290, 210, 30, 30, RIGHT_FLECHA_01);     

    // RANDOMIZER DE VALORES ENTRE 0-4:
    uint8_t random_01_04; 
    // RANDOMIZER PARA EL PLAYER 1:           
    uint8_t random_P1 = 0;   
    // RANDOMIZER PARA EL PLAYER 2:         
    uint8_t random_P2 = 0;      
    // CERO GENERAL:  
    uint8_t general = 0;    
    // VARIABLE PARA UBICACIÓN EN X:
    int x = 0;        
    // INICIALIZACION DE VARIABLES DE ESTADO:      
    uint8_t state_01;
    uint8_t state_02;

    // MIENTRAS SIGAMOS DENTRO DEL JEUGO:
    while (variable_SD == 4) 
    {
      // POR PARTIDA, SE MUESTRAN UN TOTAL DE 20 FLECHAS, 10 POR CADA LADO DE JUGADOR:
      for (general = 0; general < 5; general++) 
      {     
        randomSeed(analogRead(0));
        random_01_04 = random(1, 5);                             
        
        switch (random_01_04) 
        // ESTE RANDOM ME SIRVE PARA ALTERNAR QUE FLECHA ME SALE Y CUANDO
        {      
          // FLECHA IZQUIERDA:                           
          case 1:    
            
            for (x = 11; x <= 290; x++) 
            {             
              // DEFINO MIS BOUNDARIES Y HASTA DONDE PUEDEN LLEGAR LAS FLECHAS:
              FillRect(x - 30, 0, 30, 30, 0x00);      
              FillRect(x - 30, 122, 30, 30, 0x00);          
              // DEFINIMOS DICHOS LIMITES EN LA LCD, TOO.       
              LCD_Bitmap(x, 0, 30, 30, LEFT_01);           
              LCD_Bitmap(x, 122, 30, 30, LEFT_01);      
              // EN CUANTO A LA POSICIÓN:   
              x = x + 30;        
              // DEPENDIENDO DE LA VELOCIDAD SELECCIONADA, EL DELAYQ UE SE TENDRÁ:                     
              delay(velocidad_variable);               

              // LECTURA DIGITAL AL BOTON:
              random_P1 = digitalRead(LEFT_P1);         
              random_P2 = digitalRead(LEFT_P2);

              // PROGRAMACION DEFENSIVA:
              // ---------------------- PLAYER #1 ------------------------------------------------------------------------------------------
              // AQUI, BUSCAMOS QUE AUNQUE EL JUGADOR MANTENGA PRESIONADO EL BOTON, EL PUNTO NO VALE A MENOS QUE LO HAGA EN EL MOMENTO JUSTO:
              if (random_P1 == HIGH && x <= 260)   
              {                     
                // ESTAMOS EN ESTADO 1:        
                state_01 = 1;
              }
              // EL PUNTO OCURRE CUANDO EL JUGADOR PRESIONA JUSTO ANTES DE QUE LA FLECHA LLEGUE A LA "META".
              if (random_P1 == LOW && x > 260)     
              {                    
                // THEN, ESTO ES CERO:        
                state_01 = 0;
              }
               // ---------------------- PLAYER #2 ------------------------------------------------------------------------------------------
               // AQUI, BUSCAMOS QUE AUNQUE EL JUGADOR MANTENGA PRESIONADO EL BOTON, EL PUNTO NO VALE A MENOS QUE LO HAGA EN EL MOMENTO JUSTO:
              if (random_P2 == HIGH && x <= 260)   // Lo mismo para el jugador 2
              {
                 // ESTAMOS EN ESTADO 1: 
                state_02 = 1;
              }
              // EL PUNTO OCURRE CUANDO EL JUGADOR PRESIONA JUSTO ANTES DE QUE LA FLECHA LLEGUE A LA "META".
              if (random_P2 == LOW && x > 260)
              {
                // THEN, ESTO ES CERO:  
                state_02 = 0;
              }
            } 

            // LEEMOS NUEVAMENTE EL ESTADO DE LOS BOTONES:
            random_P1 = digitalRead(LEFT_P1);   
            random_P2 = digitalRead(LEFT_P2);
            
            // BOTON SE MANTIENE PRESIONADO: PUNTO NO SE ACREDITA.
            if (random_P1 == HIGH && state_01 == 0)  
            {                          
              // ACTUALIZAMOS COUNTERS:      
              COUNTER_P1++;                       
              LCD_Bitmap(290, 0, 30, 30, LEFT_FLECHA);
              // SI BOTONAZO FUE ACERTADO, SE ESCUCHA UN SONIDITO QUE DURA 0.5s:        
              beep(SOUND_01,500);               
            }

            if (random_P2 == HIGH && state_02 == 0)  
            {
               // ACTUALIZAMOS COUNTERS:    
              COUNTER_P2++;
              LCD_Bitmap(290, 120, 30, 30, LEFT_FLECHA);         
              // SI BOTONAZO FUE ACERTADO, SE ESCUCHA UN SONIDITO QUE DURA 0.5s:   
              beep(SOUND_02,500);
            }
            // SETEAMOOS BOTONES A CERO.
            random_P1 = LOW;  
            random_P2 = LOW;  
            break;
            
          // FLECHA HACIA ARRIBA:
          case 2:   
          
            for (x = 11; x <= 290; x++)
            {             
              FillRect(x - 30, 30, 30, 30, 0x00);  
              // PARA BORRAR EL RASTRO DE LA FLECHA ANTERIORMENTE MOSTRADA:  
              FillRect(x - 30, 150, 30, 30, 0x00);    
              // MOSTRAMOS FLECHA PARA AMBOS JUGADORES:
              LCD_Bitmap(x, 30, 30, 30, UP_FLECHA_01);        
              LCD_Bitmap(x, 150, 30, 30, UP_FLECHA_01);      
              // MOVEMOS/DESPLAZAR FLECHA A NEXT UBICACIÓN:  
              x = x + 30;                  
              // ESPERAMOS PARA MOSTRAR LA OTRA FLECHA:        
              delay(velocidad_variable);                      

              // LEEMOS ESTADO DE LOS BOTONES 'UP', DE AMBOS JUGADORES:
              random_P1 = digitalRead(UP_P1);
              random_P2 = digitalRead(UP_P2);
              
              // PROGRAMACION DEFENSIVA:
              // ---------------------- PLAYER #1 ------------------------------------------------------------------------------------------
              // AQUI, BUSCAMOS QUE AUNQUE EL JUGADOR MANTENGA PRESIONADO EL BOTON, EL PUNTO NO VALE A MENOS QUE LO HAGA EN EL MOMENTO JUSTO:
              if (random_P1 == HIGH && x <= 260)
              {
                // MISMO PROCESO ANTERIOR, (FLECHA IZQUIERDA):
                state_01 = 1;
              }
              // EL PUNTO OCURRE CUANDO EL JUGADOR PRESIONA JUSTO ANTES DE QUE LA FLECHA LLEGUE A LA "META".
              if (random_P1 == LOW && x > 260)
              {
                // THEN, ESTO ES CERO:
                state_01 = 0;
              }
              // EL PUNTO OCURRE CUANDO EL JUGADOR PRESIONA JUSTO ANTES DE QUE LA FLECHA LLEGUE A LA "META".
              if (random_P2 == HIGH && x <= 260)
              {
                // PASAMOS A ESTADO 1:
                state_02 = 1;
              }
              // DE LO CONTRARIO, NOS QUEDAMOS EN ESTE ESTADO:
              if (random_P2 == LOW && x > 260)
              {
                state_02 = 0;
              }
            } 
            
            // LECTURA DE LOS BOTONES REQUERIDOS:
            random_P1 = digitalRead(UP_P1);
            random_P2 = digitalRead(UP_P2);
            
            // MUESTRO FLECHA PARA INDICADORA DE PRESIONAR
            if (random_P1 == HIGH && state_01 == 0)
            {
              // ACTUALIZAMOS CONTADORES, PARA PLAYER 1:
              COUNTER_P1++;
              LCD_Bitmap(290, 30, 30, 30, UP_FLECHA);           
              beep(SOUND_01,500);
            }
            // ACTUALIZAMOS CONTADORES, PARA PLAYER 2:
            if (random_P2 == HIGH && state_02 == 0)
            {
              COUNTER_P2++;
              LCD_Bitmap(290, 150, 30, 30, UP_FLECHA); 
              // SI SE ACERTÓ, ENTONCES SUENA UN BEEP CORTO:       
              beep(SOUND_02,500);
            }

            // VOLVEMOS A SET LOS ESTADOS DE LOS BOTONES A CERO.
            random_P1 = LOW;  
            random_P2 = LOW;  
            break;

         // FLECHA ABAJO:
          case 3:   
            // PARA APARICIÓN DE FLECHAS:
            for (x = 11; x <= 290; x++)
            {             
              FillRect(x - 30, 60, 30, 30, 0x00);     
              // HIDE FLECHA ANTERIOR, RASTRO:
              FillRect(x - 30, 180, 30, 30, 0x00);  
              // LLAMAMOS A ARRAY EN --> bitmaps.h  
              LCD_Bitmap(x, 60, 30, 30, DOWN_FLECHA);          
              LCD_Bitmap(x, 180, 30, 30, DOWN_FLECHA);    
              // DESPLAZAMOS LA FLECHA / ACTUALIZAMOS POSICION:     
              x = x + 30;                           
              // ESPERAMOS PARA MOSTRAR LA SIGUIENTE FLECHA:
              delay(velocidad_variable);                       

              // LECTURA DIGITAL DE BOTONES:
              random_P1 = digitalRead(DOWN_P1);
              random_P2 = digitalRead(DOWN_P2);

              // DEPENDIENDO DEL TIPO DE BOTONAZO, ES ACIERTO O MISS. 
              if (random_P1 == HIGH && x <= 260)
              {
                // SI NO ACIERTA:
                state_01 = 1;
              }
              // SI ES OTRA POSICIÓN:
              if (random_P1 == LOW && x > 260)
              {
                // ENTONCES SI ACIERTA, SE SUMA PUNTO A CONTADOR.
                state_01 = 0;
              }
              // LO MISMO APLICA PARA EL PLAYER #2:
              if (random_P2 == HIGH && x <= 260)
              {
                state_02 = 1;
              }
              // DEPENDIENDO DEL TIPO DE BOTONAZO, ES ACIERTO O MISS. 
              if (random_P2 == LOW && x > 260)
              {
                 // ENTONCES NO ACIERTA, Y NO SE SUMA PUNTO A CONTADOR.
                state_02 = 0;
              }
            } 
            // HACEMOS LECTURA DIGITAL DE LOS BOTONES --> ABAJO:
            random_P1 = digitalRead(DOWN_P1);
            random_P2 = digitalRead(DOWN_P2);
            // SI 
            if (random_P1 == HIGH && state_01 == 0)
            {
              // SI ACERTÓ Y STATE_01 = 0, ENTONCES SE SUMA PUNTO AL PLAYER.
              COUNTER_P1++;
              // SHOW OTRA FLECHA SI SE CUMPLE LO ANTERIOR HECHO:
              LCD_Bitmap(290, 60, 30, 30, DOWN_FLECHA);  
              // SUENA EL BEEP EN CASO DE ACIERTO:        
              beep(SOUND_01,500);
            }
            // SI ACIERTA EL PLAYER #2, ENTONCES SE SUMA AL CONTADOR EL PUNTO:
            if (random_P2 == HIGH && state_02 == 0)
            {
              COUNTER_P2++;
              // MUESTRO OTRA FLECHA:
              LCD_Bitmap(290, 180, 30, 30, DOWN_FLECHA);       
              // SUENA EL BEEP:  
              beep(SOUND_02,500);
            }
            
            // RESET A LA LECTURA DIGITAL:
            random_P1 = LOW;  // Reiniciamos la we_read digital
            random_P2 = LOW;  // Reiniciamos la we_read digital
            break;
            
          // FLECHA DERECHA:
          case 4:  
            for (x = 11; x <= 290; x++) 
            {            
              // PLANTEAMOS MOSTREO DE FLECHAS:
              FillRect(x - 30, 90, 30, 30, 0x00);     
              FillRect(x - 30, 210, 30, 30, 0x00);    
              // MUESTRO FLECHA PARA P1:
              LCD_Bitmap(x, 90, 30, 30, RIGHT_FLECHA_01);      
              // MUESTRO FLECHA PARA P2:   
              LCD_Bitmap(x, 210, 30, 30, RIGHT_FLECHA_01);  
              // DOY PASO A QUE LA SIGUIENTE FLECHA APAREZA:      
              x = x + 30;                            
              delay(velocidad_variable);                       

              random_P1 = digitalRead(RIGHT_P1);
              random_P2 = digitalRead(RIGHT_P2);
              
              // DEPENDIENDO DEL TIPO DE BOTONAZO, ES ACIERTO O MISS. 
              if (random_P1 == HIGH && x <= 260)
              {
                // PASAMOS A ESTADO 1, SI ES PUNTO:
                state_01 = 1;
              }

              if (random_P1 == LOW && x > 260)
              {
                // NO ES PUNTO:
                state_01 = 0;
              }
               // MISMO CASO PARA P2:
              if (random_P2 == HIGH && x <= 260)
              {
                // HUBO PUNTO:
                state_02 = 1;
              }
              // RESET A VARIABLE:
              if (random_P2 == LOW && x > 260)
              {
                // NO ES PUNTO:
                state_02 = 0;
              }
            } 
            // LECTURA DIGITAL DE LOS BOTONES, PARA LA DERECHA OJO:
            random_P1 = digitalRead(RIGHT_P1);
            random_P2 = digitalRead(RIGHT_P2);
            
            // SI HUBO PUNTO, EN APARTADO ANTERIOR, SE SUMA A CONTADOR DEL JUGADOR1:
            if (random_P1 == HIGH && state_01 == 0)
            {
              COUNTER_P1++;
              LCD_Bitmap(290, 90, 30, 30, DR);   
              // SE REPRODUCE EL SONIDO DE ACIERTO:       
              beep(SOUND_01,500);
            }
            // SI HUBO PUNTO, EN APARTADO ANTERIOR, SE SUMA A CONTADOR DEL JUGADOR 2:
            if (random_P2 == HIGH && state_02 == 0)
            {
              COUNTER_P2++;
              LCD_Bitmap(290, 210, 30, 30, DR);   
               // SE REPRODUCE EL SONIDO DE ACIERTO:             
              beep(SOUND_02,500);
            }
            // REGRESAR A CERO EL ESTADO DE LOS BOTONES:
            random_P1 = LOW;  
            random_P2 = LOW;  
            break;
        }

      }
      variable_SD = 5; // --> VARIABLE IMPORTANTE SE ACTUALIZA, PARA CAMBIAR DE PANTALLA.
    }
  }

  // DAMOS UN DELAY DE UN SEGUNDO PARA MOSTRAR LA PANTALLA QUE CONTIENE AL GANADOR:
  delay(1000);
  if (variable_SD == 5) 
  {
    // UNA VEZ TERMINADO EL JUEGO, PROCEDEMOS A COMPARAR LOS VALORES DE COUNTERS:
    while (variable_SD == 5) 
    {
      // LE ASIGNO UN COLOR A LA PANTALLA, AL IGUAL QUE EN PARTES ANTERIORES:
      LCD_Clear(0xFFFF);  
                                          
      // COMPARACIÓN DE CONTADORES:
       // EN CASO GANE EL JUGADOR #1:
      if (COUNTER_P1 > COUNTER_P2) {                                    
        String text14 = "JUGADOR 1";                          
        String text15 = "GANADOR";                              
        const char* PASS_01 = NUMBERS[COUNTER_P1];
        const char* PASS_02 = NUMBERS[COUNTER_P2];
        
        // IMPRIMIMOS LAS PALABRAS:
        LCD_Print(text15, 130, 103, 2,  0x0000, 0xFFFF);      
        LCD_Print(text14, 95, 133, 2,  0x0000, 0xFFFF);       
        LCD_Print(PASS_01, 125, 163, 2,  0x0000, 0xFFFF);       
        LCD_Print(PASS_02, 165, 163, 2,  0x0000, 0xFFFF);      

        // CLEAR A LA VARIABLE PARA SIGUIENTE CASO:
        uint8_t NUMBERS = 0;

        // UNA VEZ LIMPIA LA PANTALLA, GANADOR O EMPATE DECLARADO, SUENA CANCIÓN FINAL.
        if (NUMBERS == 0)      
        {
          // PRIMER FRAGMENTO
          beep(NOTA_MI,110);
          beep(NOTA_RE,115);
          beep(NOTA_FA,230);
          beep(NOTA_SOL,270);
          
          // SEGUNDO FRAGMENTO
          
          beep(NOTA_DOOO,150);
          beep(NOTA_SI,125);
          beep(NOTA_RE,250);
          beep(NOTA_MI,255);
          beep(NOTA_SI,100);

          // TERCER FRAGMENTO
          
          beep(NOTA_LA,105);
          beep(NOTA_DO_EXTRA,200);
          beep(NOTA_MI,350);
          beep(NOTA_LA,800);
  
          NUMBERS = 1;
        }
        // ESPERAMOS UN SEGUNDO PARA CONTINUAR:
        delay(1000);                                          
      }
      // EN CASO GANE EL JUGADOR #2:
      
      if (COUNTER_P2 > COUNTER_P1) {                                      
        String text17 = "JUGADOR 2";                          
        String text16 = "GANADOR";                               
        const char* PASS_01 = NUMBERS[COUNTER_P1];
        const char* PASS_02 = NUMBERS[COUNTER_P2];
        LCD_Print(text16, 130, 103, 2,  0x0000, 0xFFFF);      
        LCD_Print(text17, 95, 133, 2,  0x0000, 0xFFFF);       
        LCD_Print(PASS_01, 125, 163, 2,  0x0000, 0xFFFF);      
        LCD_Print(PASS_02, 165, 163, 2,  0x0000, 0xFFFF);      
        uint8_t NUMBERS = 0;
        
        if (NUMBERS == 0)         // Suena melodia NOKIA de 1994 una sola vevariable_SD
        {
          beep(NOTA_MI,120);
          beep(NOTA_RE,125);
          beep(NOTA_FA,250);
          beep(NOTA_SOL,250);
          beep(NOTA_DOOO,120);
          beep(NOTA_SI,125);
          beep(NOTA_RE,250);
          beep(NOTA_MI,200);
          beep(NOTA_SI,150);
          beep(NOTA_LA,125);
          beep(NOTA_DO_EXTRA,200);
          beep(NOTA_MI,300);
          beep(NOTA_LA,750);
  
          NUMBERS = 1;
        }
        // ESPERAMOS UN SEGUNDO:
        delay(1000);                                          
      }
      // EN CASO QUEDARAN LO SJUGADORES EN EMPATE:
      if (COUNTER_P1 == COUNTER_P2) {                                   

        // MOSTRAMOS EN PANTALLA:
        String draw = "EMPATE";                            
        const char* PASS_01 = NUMBERS[COUNTER_P1];
        const char* PASS_02 = NUMBERS[COUNTER_P2];

        // CONFIG. DE QUE MOSTRAMOS:
        LCD_Print(draw, 110, 113, 2,  0x0000, 0xFFFF);      
        LCD_Print(PASS_01, 125, 163, 2,  0x0000, 0xFFFF);       
        LCD_Print(PASS_02, 165, 163, 2,  0x0000, 0xFFFF);   
            
        // RESET A VARIABLE DE LISTADO DE NUMEROS:
        uint8_t NUMBERS = 0;
        // REPRODUCIMOS SONIDO:
        if (NUMBERS == 0)             
        {
          //PRIMER FRAGMENTO:
          beep(NOTA_MI,210);
          beep(NOTA_RE,15);
          beep(NOTA_FA,290);
          beep(NOTA_SOL,255);
          // SEGUNDO FRAGMENTO:
          beep(NOTA_DOOO,120);
          beep(NOTA_SI,160);
          beep(NOTA_RE,260);
          beep(NOTA_MI,290);
          beep(NOTA_SI,170);
          beep(NOTA_LA,135);
          // TERCER FRAGMENTO:
          beep(NOTA_DO_EXTRA,210);
          beep(NOTA_MI,350);
          beep(NOTA_LA,800);

          // STOP A LA MUSICA, UNA VEZ REPRODUCIDA TODA:
          NUMBERS = 1;
        }
        // WAIT UN SEGUNDO
        delay(1000);                                         
      }
      // ACTUALIZACION DE VARIABLE DE CAMBIO DE PANTALLAS
      variable_SD = 6;                     
    }
  }
  // MOSTRAMOS UN MEDU INTERMEDIO, PARA REGRESAR AL PRINCIPAL:
  
  String draw = "PRESIONE B1, PARA JUGAR OTRA VEZ.";   
  // LO IMPRIMIMOS EN LA PANTALLA: 
  LCD_Print(draw, 20, 218, 1,  0x0000, 0xFFFF);          

  // PARTE DE REINICIO DE JUEGO:
  while (variable_SD == 6)               
  {
    // RESET A LA VARIABLE, PARA PODER REINCIAR EL JUEGO
    uint8_t reinicio = LOW;      
    // CHECK EL ANTIRREBOTE:
    uint8_t anti_rebote = 0;
    
    // CUANDO ESTEMOS LISTOS PARA REINICIAR:
    while (variable_SD == 6) 
    {
      // SOLICITAR AL USUARIO OPRIMIR 'UP' DEL MANDO #1
      reinicio = digitalRead(UP_P1);        

      // SI EL USUARIO PRESIONÓ EL BOTON SOLICITADO, SE ACTIVA ANTIRREBOTE Y PROCEDEMOS A REINICIAR:
      if (reinicio == HIGH && anti_rebote == 0)    
      {
        // INICIAMOS JUEGO DESDE CERO, NADIE TIENE PUNTOS, TODO CERO:
        velocidad_variable = 0;                 
        COUNTER_P1 = 0;                       
        COUNTER_P2 = 0;                  
        omg = 1;
      }

      if (reinicio == LOW && omg == 1)
      {
        omg = 0;
        variable_SD = 1;                            // FOTOD DE INICIO random_01_04 CARGAR MEMORIA
      }
    }
   // LIMPIAMOS PANTALL POR ULTIMA VEZ, PREVIO A REGRESAR AL INICIO:
    FillRect(0 , 0, 320, 240, 0xFFFF);   
    delay(500);                        
    goto empezar;                        
  }
}

// FINNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN.
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialivariable_SDación
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xDOWN_P1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER)
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORrandom_01_04ACCESS)
  LCD_DATA(0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROLEFT_P2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROLEFT_P1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROLEFT_P1)
  LCD_DATA(0x04); // 72Hvariable_SD
  //****************************************
  LCD_CMD(0xDOWN_P2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
  //  LCD_DATA(0x8F);
  LCD_CMD(0x29); //displarandom_01_04 on
  LCD_CMD(0x2C); //displarandom_01_04 on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int random_01_041, unsigned int x2, unsigned int random_01_042) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(random_01_041 >> 8);
  LCD_DATA(random_01_041);
  LCD_DATA(random_01_042 >> 8);
  LCD_DATA(random_01_042);
  LCD_CMD(0x2c); // Write_memorrandom_01_04_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c) {
  unsigned int x, random_01_04;
  LCD_CMD(0x02c); // write_memorrandom_01_04_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (random_01_04 = 0; random_01_04 < 240; random_01_04++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
    }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea horivariable_SDontal - parámetros ( coordenada x, cordenada random_01_04, longitud, color)
//***************************************************************************************************************************************
void H_line(unsigned int x, unsigned int random_01_04, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memorrandom_01_04_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, random_01_04, l, random_01_04);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada random_01_04, longitud, color)
//***************************************************************************************************************************************
void V_line(unsigned int x, unsigned int random_01_04, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memorrandom_01_04_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + random_01_04;
  SetWindows(x, random_01_04, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada random_01_04, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int random_01_04, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , random_01_04  , w, c);
  H_line(x  , random_01_04 + h, w, c);
  V_line(x  , random_01_04  , h, c);
  V_line(x + w, random_01_04  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada random_01_04, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int random_01_04, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , random_01_04  , w, c);
    H_line(x  , random_01_04 + i, w, c);
  }
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada random_01_04, color, background)
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int random_01_04, int fontSivariable_SDe, int color, int background) {
  int fontXSivariable_SDe ;
  int fontrandom_01_04Sivariable_SDe ;

  if (fontSivariable_SDe == 1) {
    fontXSivariable_SDe = fontXSivariable_SDeSmal ;
    fontrandom_01_04Sivariable_SDe = fontrandom_01_04Sivariable_SDeSmal ;
  }
  if (fontSivariable_SDe == 2) {
    fontXSivariable_SDe = fontXSivariable_SDeBig ;
    fontrandom_01_04Sivariable_SDe = fontrandom_01_04Sivariable_SDeBig ;
  }

  char charInput ;
  int cLength = text.length();
  Serial.println(cLength, DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_arrarandom_01_04[cLength + 1];
  text.toCharArrarandom_01_04(char_arrarandom_01_04, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_arrarandom_01_04[i];
    Serial.println(char_arrarandom_01_04[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSivariable_SDe), random_01_04, x + (i * fontXSivariable_SDe) + fontXSivariable_SDe - 1, random_01_04 + fontrandom_01_04Sivariable_SDe );
    long charHex1 ;
    for ( int n = 0 ; n < fontrandom_01_04Sivariable_SDe ; n++ ) {
      if (fontSivariable_SDe == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontrandom_01_04Sivariable_SDe) + n);
      }
      if (fontSivariable_SDe == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontrandom_01_04Sivariable_SDe) + n);
      }
      for (int t = 1; t < fontXSivariable_SDe + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSivariable_SDe - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int random_01_04, unsigned int width, unsigned int height, unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memorrandom_01_04_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, random_01_042;
  x2 = x + width;
  random_01_042 = random_01_04 + height;
  SetWindows(x, random_01_04, x2 - 1, random_01_042 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int random_01_04, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memorrandom_01_04_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, random_01_042;
  x2 =   x + width;
  random_01_042 =    random_01_04 + height;
  SetWindows(x, random_01_04, x2 - 1, random_01_042 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  } else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }


  }
  digitalWrite(LCD_CS, HIGH);
}
