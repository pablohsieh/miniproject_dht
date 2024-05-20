//Mini Proyecto pre-TPP de Pablo Hsieh
//
//- Implementacion de un menu navegable.
//- Lectura de Temperatura y Humedad por DHT22.
//- Lectura de datos numericos por keypad matricial 4x4 arduino.
//- Navegacion de menu por keypad matricial 4x4 arduino.
//- Sistema de alarma por mediciones de TyH fuera de los umbrales definidos


#include "types.h"
#include "menu_definitions.h"
#include "menu_logic.h"


// Variables globales a ser utilizadas para triggerear acciones de alarma
bool alarma_prendida_t = false;
bool alarma_prendida_h = false;

#define T_MIN 10
#define T_MAX 20
#define H_MIN 40
#define H_MAX 70

// estructura de datos de telemetria telemetry_status = {*data, min_treshold, max_treshold}
telemetry_t data_t = {0 , T_MIN , T_MAX}; ;
telemetry_t data_h = {0 , H_MIN , H_MAX}; ;


//double stored_number = 0;
//double stored_alarm_treshold_h;
//double stored_alarm_treshold_t;


void setup() {

    Serial.begin(9600);
    Serial.print(F("\n"));
    Serial.print(F("\n"));
    Serial.println(F("Iniciando..."));
    
    dht.begin();
    hardware_init();
    
}



void loop() {

// Inicializacion del menu y asignacion de punteros a los submenu anteriores a cada uno.
    menu_status_t current_menu;
    
    current_menu.menu = menu_bienvenida;
    menu_bienvenida[menu_last_row_number(menu_bienvenida)+1].submenu = menu_principal;
    menu_principal[menu_last_row_number(menu_principal)+1].submenu = menu_bienvenida;

//    current_menu.menu = menu_principal;
//    menu_bienvenida[menu_last_row_number(menu_bienvenida)+1].submenu = menu_bienvenida;
//    menu_principal[menu_last_row_number(menu_principal)+1].submenu = menu_principal;

    submenu_alarmas_tyh[menu_last_row_number(submenu_alarmas_tyh)+1].submenu = menu_principal;  
    submenu_alarmas_tyh_t[menu_last_row_number(submenu_alarmas_tyh_t)+1].submenu = submenu_alarmas_tyh;
    submenu_alarmas_tyh_h[menu_last_row_number(submenu_alarmas_tyh_h)+1].submenu = submenu_alarmas_tyh; 
    
    menu_initialization(&current_menu);
//    menu_print_list_in_window(current_menu);

    cursor_states cursor_action = cursor_standby;

        while(1){

//            Serial.println(F("while(1)"));
//            Serial.println(current_menu.menu[0].menu_item_name); 
            
            while(current_menu.menu == menu_bienvenida){
//                Serial.println(F("while(menu_bienvenida)"));
//                Serial.println(current_menu.menu[0].menu_item_name); 
                               
                (*mostrar_read_temp_hum)(NULL);
                cursor_action = cursor_enter;               
                navegate_menu(&current_menu,cursor_action);
                menu_print_list_in_window(current_menu);
//                Serial.println(F("while(menu_bienvenida)"));
//                Serial.println(current_menu.menu[0].menu_item_name); 
    
            }
        
        
        check_tresholds();
        alarm_lcd_blink_t(alarma_prendida_t);
        alarm_lcd_blink_h(alarma_prendida_h);   
             
        cursor_action = determinar_accion_cursor();              
        
        if(cursor_action != cursor_standby){
            navegate_menu(&current_menu,cursor_action);
            cursor_action = cursor_standby;    
            menu_print_list_in_window(current_menu);
        }
               
    }

}
