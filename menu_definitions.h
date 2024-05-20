#include "functions.h"
#include "botonera.h"


//#define INITIAL_MENU_TAG menu_principal
#define INITIAL_MENU_TAG menu_bienvenida

extern telemetry_t data_t;
extern telemetry_t data_h;

//extern double stored_number;
//extern double stored_alarm_treshold_h;
//extern double stored_alarm_treshold_t;
//extern bool alarma_prendida_t;
//extern bool alarma_prendida_h;


////////////////////////////////////////////////////////////////////////////////////
///////////////////////    Etiqueta de puntero a funcion   /////////////////////////
////////////////////////////////////////////////////////////////////////////////////
typedef void (*menu_action)(void *params);
// pointer to function blink_led(int led_pin)


////////////////////////////////////////////////////////////////////////////////////
///////////////    Estructura usada para definir un item del menu  /////////////////
////////////////////////////////////////////////////////////////////////////////////
typedef struct menu_item {
    
    char menu_item_name[DISPLAY_MAX_COLS-4];
    struct menu_item *submenu;
// el puntero a submenu se utilizara para ir al submenu o para ir al menu anterior del submenu
    menu_action ptr_function_action;
    void *params;
    
}menu_item_t;

////////////////////////////////////////////////////////////////////////////////////
/////////////    Estructura usada para definir el menu actualizado  ////////////////
////////////////////////////////////////////////////////////////////////////////////
typedef struct menu_status {
    
    menu_item_t *menu;
    int cursor_position;
    int cursor_display_position;
    int window_start_position;
    int list_end_position;
        
}menu_status_t;



////////////////////////////////////////////////////////////////////////////////////
/////////////////    (Puntero a) Funciones llamadas en el menu  ////////////////////
////////////////////////////////////////////////////////////////////////////////////
void change_alarm_state_on(void *params){
    *(bool*)params = true;
};

void toggle_bool(void *params) {
    bool value = *(bool*)params;
    *(bool*)params = ! value;
};


void print_elapsed_time(void *){
// Funcion que muestra el tiempo en segundos que transcurrio desde que se encendio el programa.
    lcd.clear();

    lcd.setCursor(2,0);
    lcd.print("HOLA! Han pasado");
    lcd.setCursor(2,1);
    lcd.print(millis()/1000);
    lcd.print(" segundos");

    lcd.setCursor(2,DISPLAY_MAX_ROW-1);
    lcd.print("Para volver: B");

    cursor_states cursor_action = cursor_standby;

    while(cursor_action != cursor_back){
        cursor_action = determinar_accion_cursor();
        lcd.setCursor(2,1);
        lcd.print(millis()/1000);
        lcd.print(" segundos");                       
    }   
};


void mostrar_read_temp_hum(void *){
// Funcion de telemetria de Temperatura y Humedad
//    lcd.clear();
    cursor_states cursor_action = cursor_standby;
    while(cursor_action != cursor_enter){

        print_telemetry();
        check_tresholds();
        alarm_lcd_blink_t(alarma_prendida_t);
        alarm_lcd_blink_h(alarma_prendida_h);      
                            
        cursor_action = determinar_accion_cursor();           
    }  
};

////////////////////////////////////////////////////////////////////////////////////
///////////    (Puntero a) Funcion de lectura numerica del keypad 4x4  /////////////
////////////////////////////////////////////////////////////////////////////////////
double get_num_keypad(int pos_col, int pos_row){
// Funcion que lee del teclado matricial 4x4 y guarda el numero leido.
// El numero leido se guarda luego de presionar A
// Se cancela la funcion presionando la tecla C 
// Lee numeros con coma, la coma se designa luego de presionar la tecla D
    double num = 0;
    double num_unit = 0;
    double num_frac = 0;
    int unit = 0;
    int frac = 0;
    int i = 0;
    char key = keypad.getKey();
    while(key != 'A'){
        lcd.setCursor(pos_col+i,pos_row);
        lcd.print("_");
        if(frac > 0){
            switch (key){
            case NO_KEY: case '#': case '*': case 'B': case 'D':
                break;
            
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                lcd.setCursor(pos_col+i,pos_row);
                lcd.print(key);
                num_frac = num_frac * 10 + (key - '0');
                frac++;
                i++;
                break;
                
            case 'C':
                num = -1;
                return num;
                break;
            }
        }
        else{
            switch (key){
            case NO_KEY: case '*': case '#': case 'A': case 'B':
                break;
            
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                lcd.setCursor(pos_col+i,pos_row);
                lcd.print(key);
                num_unit = num_unit * 10 + (key - '0');
                unit++;
                i++;
                break;

            case 'D':
                lcd.setCursor(pos_col+i,pos_row);
                lcd.print('.');
                frac++;
                i++;
                break;
                
            case 'C':
                num = -1;
                return num;
                break;
            }
        }
                
        key = keypad.getKey();

    }

    num = num_unit + (num_frac/pow(10,frac-1));
    
//    Serial.println(num_unit);
//    Serial.println(frac-1);
//    Serial.println(pow(10,frac-1));
//    Serial.println(num_frac);
//    Serial.println(num_frac/pow(10,frac-1));
//    Serial.println(num);

    return num;
    
};



int leer_numero(void *params){

    lcd.clear();
    cursor_states cursor_action = cursor_standby;       
    
    lcd.setCursor(2,0);
    lcd.print("Ingrese numero:");
    
    lcd.setCursor(2,2);
    lcd.print("guardar: A");

    lcd.setCursor(2,3);
    lcd.print("cancelar: C");      

    double numero_old = *(double*)params; 
//    Serial.println(numero_old);
//    char key = keypad.getKey();
    
    double numero_new = get_num_keypad(2,1);
           
    if(numero_new != -1){       
        *(double*)params = numero_new;  
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Numero guardado");
        delay(DELAY_TIME*2);
    }
    else{
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Cancelado");
        delay(DELAY_TIME*2);
    }
    
//    Serial.println(numero);

};


////////////////////////////////////////////////////////////////////////////////////
////////////    (Puntero a) Funcion de muestreo del numero guardado  ///////////////
////////////////////////////////////////////////////////////////////////////////////
void mostrar_numero(void *params){
    lcd.clear();
    
    lcd.setCursor(2,0);
    lcd.print("Numero guardado:");
    lcd.setCursor(2,3);
    lcd.print("para salir: B");
    
//    long *p = params; 
    double value = *(double*)params;
    lcd.setCursor(2,1);
    lcd.print(value);

    cursor_states cursor_action = cursor_standby;

    while(cursor_action != cursor_back){                  
        cursor_action = determinar_accion_cursor();           
    }  
};


void mostrar_numero_struct_data(void *params){
    lcd.clear();
    
    lcd.setCursor(2,0);
    lcd.print("min:");    
    lcd.setCursor(2,1);
    lcd.print("max:");
    
    lcd.setCursor(2,3);
    lcd.print("para salir: B");
    
//    long *p = params; 
    telemetry_t value = *(telemetry_t*)params;
    lcd.setCursor(7,0);
    lcd.print(value.min_treshold);
    lcd.setCursor(7,1);
    lcd.print(value.max_treshold);


    cursor_states cursor_action = cursor_standby;

    while(cursor_action != cursor_back){
        check_tresholds();
        alarm_lcd_blink_t(alarma_prendida_t);
        alarm_lcd_blink_h(alarma_prendida_h);                  
        cursor_action = determinar_accion_cursor();           
    }  
};


//void reset_number(void *params){
//    double value = *(double*)params;
//    *(double*)params = 0;
//    lcd.clear();
//    lcd.setCursor(2,0);
//    lcd.print("Numero reseteado a");
//    lcd.setCursor(2,1);
//    lcd.print(stored_number);
//    delay(DELAY_TIME*2);
//};


void reset_number_struct(void *params){
    
    telemetry_t value = *(telemetry_t*)params;
    telemetry_t aux = {value.data , 0 , 0}; 
    
    *(telemetry_t*)params = aux;
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Treshold reset");
    lcd.setCursor(2,1);
    lcd.print("min:");    
    lcd.setCursor(2,2);
    lcd.print("max:");
    
    lcd.setCursor(7,1);
    lcd.print(aux.min_treshold);
    lcd.setCursor(7,2);
    lcd.print(aux.max_treshold);

    
    delay(DELAY_TIME*2);
};


////////////////////////////////////////////////////////////////////////////////////
/////////////////////    Definicion de los arreglos del Menu    ////////////////////  
////////////////////////////////////////////////////////////////////////////////////
menu_item_t submenu_alarmas_tyh_t[] = {
    //  {"char menu",pointer to submenu, pointer to function, parameters for function}
        {"Set treshold min", NULL, leer_numero, &data_t.min_treshold},
        {"Set treshold max", NULL, leer_numero, &data_t.max_treshold},
        {"Mostrar treshold",NULL, mostrar_numero_struct_data, &data_t},
        {"Reset tresholds", NULL, reset_number_struct, &data_t},
//        {"Modificar treshold", NULL, leer_numero, &stored_alarm_treshold_t},
//        {"Mostrar treshold",NULL, mostrar_numero, &stored_alarm_treshold_t},
//        {"Reset treshold", NULL, reset_number, &stored_alarm_treshold_t},     
    //  {"", pointer to parent menu, NULL}, LAST OF LIST
        {"", NULL, NULL, NULL},
    };
//  {"", pointer to parent menu, NULL}, LAST OF LIST    
//submenu_alarmas_tyh_t[ULTIMO].submenu = submenu_alarmas_tyh;
//submenu_alarmas_tyh_t[menu_last_row_number(submenu_alarmas_tyh_t)+1].submenu = submenu_alarmas_tyh;


menu_item_t submenu_alarmas_tyh_h[] = {
    //  {"char menu",pointer to submenu, pointer to function, parameters for function}
        {"Set treshold min", NULL, leer_numero, &data_h.min_treshold},
        {"Set treshold max", NULL, leer_numero, &data_h.max_treshold},
        {"Mostrar treshold",NULL, mostrar_numero_struct_data, &data_h},
        {"Reset tresholds", NULL, reset_number_struct, &data_h},
//        {"Modificar treshold", NULL, leer_numero, &stored_alarm_treshold_h},
//        {"Mostrar treshold",NULL, mostrar_numero, &stored_alarm_treshold_h},
//        {"Reset treshold", NULL, reset_number, &stored_alarm_treshold_h},     
    //  {"", pointer to parent menu, NULL}, LAST OF LIST
        {"", NULL, NULL, NULL},
    };
//  {"", pointer to parent menu, NULL}, LAST OF LIST    
//submenu_alarmas_tyh_h[ULTIMO].submenu = submenu_alarmas_tyh;
//submenu_alarmas_tyh_h[menu_last_row_number(submenu_alarmas_tyh_h)+1].submenu = submenu_alarmas_tyh;


menu_item_t submenu_alarmas_tyh[] = {
    //  {"char menu",pointer to submenu, pointer to function, parameters for function}
//        {"Cambiar alarma T", NULL, leer_numero, &stored_alarm_treshold_t},
//        {"Actual alarma T",NULL, mostrar_numero, &stored_alarm_treshold_t},
//        {"Reset alarma T", NULL, reset_number, &stored_alarm_treshold_t},
//        {"Cambiar alarma H", NULL, leer_numero, &stored_alarm_treshold_h},
//        {"Actual alarma H",NULL, mostrar_numero, &stored_alarm_treshold_h},
//        {"Reset alarma H", NULL, reset_number, &stored_alarm_treshold_h},
        {"Temperatura", submenu_alarmas_tyh_t, NULL, NULL},
        {"Humedad", submenu_alarmas_tyh_h, NULL, NULL},        
    //  {"", pointer to parent menu, NULL}, LAST OF LIST
        {"", NULL, NULL, NULL},
    };
//  {"", pointer to parent menu, NULL}, LAST OF LIST    
//submenu_alarmas_tyh[ULTIMO].submenu = menu_principal;
//submenu_alarmas_tyh[menu_last_row_number(submenu_alarmas_tyh)+1].submenu = menu_principal;


menu_item_t menu_principal[] = {
//  {"char menu",pointer to submenu, pointer to function, parameters for function}
//    {"Monitoreo", NULL, NULL, NULL},
//    {"Monitor T y H", NULL, mostrar_read_temp_hum, NULL},
    {"Alarmas T y H >", submenu_alarmas_tyh, NULL, NULL},
//    {"Ingresar numero", NULL, leer_numero, &stored_number},
//    {"Mostrar numero", NULL, mostrar_numero, &stored_number},
//    {"Reset numero", NULL, reset_number, &stored_number},
    {"Tiempo transcurrido", NULL, print_elapsed_time, NULL},
    {"Menu Monitor: B", NULL, mostrar_read_temp_hum, NULL},
//  {"", pointer to parent menu, NULL}, LAST OF LIST
    {"", NULL, NULL, NULL},
};
//  {"", pointer to parent menu, NULL}, LAST OF LIST  
//menu_principal[ULTIMO].submenu = menu_principal;
//menu_principal[menu_last_row_number(menu_principal)+1].submenu = menu_principal;
//menu_principal[menu_last_row_number(menu_principal)+1].submenu = menu_bienvenida;

menu_item_t menu_bienvenida[] = {
    {"Navegar Menu", menu_principal, NULL, NULL},
    {"", NULL, NULL, NULL},
};
//  {"", pointer to parent menu, NULL}, LAST OF LIST  
//menu_bienvenida[ULTIMO].submenu = menu_bienvenida;
//menu_bienvenida[menu_last_row_number(menu_bienvenida)+1].submenu = menu_bienvenida;
