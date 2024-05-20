//#include "functions.h"


////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for printing menu navigation symbols  ////////////////
////////////////////////////////////////////////////////////////////////////////////
void menu_print_cursor_position(int current_pos){
    // Clear menu current cursor selection
    for(int i=0; i<DISPLAY_MAX_ROW;i++){ 
        lcd.setCursor(0,i);  
        lcd.print(String(" "));
//        lcd.setCursor(DISPLAY_MAX_COLS-2,i);  
//        lcd.print(String(" "));
    }
    
    // Print actual menu cursor selection
    lcd.setCursor(0,current_pos);  
    lcd.print(String(char(126))); // Print RightArrow -> 
//    lcd.setCursor(DISPLAY_MAX_COLS-2,current_pos);  
//    lcd.print(String(char(127))); // Print LeftArrow <- 
}

void menu_print_uparrow(){
    // Navigation Up/Down Cursors
    lcd.setCursor(DISPLAY_MAX_COLS-1,0);  
    lcd.write(byte(STORE_CHAR_UPARROW));
}

void menu_print_downarrow(){
    // Navigation Up/Down Cursors
    lcd.setCursor(DISPLAY_MAX_COLS-1,3);  
    lcd.write(byte(STORE_CHAR_DOWNARROW)); 
}

void menu_print_check(){
    lcd.setCursor(DISPLAY_MAX_COLS-1,1);  
    lcd.write(byte(STORE_CHAR_CHECK)); 
}

void menu_print_cross(){
    lcd.setCursor(DISPLAY_MAX_COLS-1,2);  
    lcd.write(byte(STORE_CHAR_CROSS)); 
}

////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for printing menu navigation states  /////////////////
////////////////////////////////////////////////////////////////////////////////////
void print_states(menu_status_t current_menu, cursor_states cursor_action){
//  Para ver en pantalla la posicion real de los cursores y ventana de impresion.
    
    lcd.setCursor(16,0);  
    lcd.print("C");
    lcd.setCursor(17,0);  
    lcd.print(current_menu.cursor_position);

    lcd.setCursor(16,1);  
    lcd.print("D");
    lcd.setCursor(17,1);  
    lcd.print(current_menu.cursor_display_position);
      
    lcd.setCursor(13,0);  
    lcd.print("S");
    lcd.setCursor(14,0);  
    lcd.print(current_menu.window_start_position);

    lcd.setCursor(13,1);  
    lcd.print("E");
    lcd.setCursor(14,1);  
    lcd.print(current_menu.window_start_position + DISPLAY_MAX_ROW - 1);

    lcd.setCursor(13,3);  
    lcd.print("F");
    lcd.setCursor(14,3);  
    lcd.print(current_menu.list_end_position);

    lcd.setCursor(16,3);  
    lcd.print("A");
    
    switch(cursor_action){
        lcd.setCursor(17,3);

        case cursor_up: 
            lcd.write(byte(STORE_CHAR_UPARROW)); 
            break;
        case cursor_down:
            lcd.write(byte(STORE_CHAR_DOWNARROW));
            break;        
        case cursor_standby:
            lcd.print("-");
            break;        
    }
}




////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for obtaining the length of the menu list   //////////
////////////////////////////////////////////////////////////////////////////////////
int menu_last_row_number(menu_item_t *menu) {
// Get menu list qty
    int count = 0;
    while (menu[count].menu_item_name[0] != '\0'){
        count++;
    }
    return count-1;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for checking if there is any submenu   ///////////////
////////////////////////////////////////////////////////////////////////////////////
bool have_submenu(menu_item_t current_menu){
    if(current_menu.submenu != NULL){
        return true;
    }
    else{
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for printing the currently viewed menu   /////////////
////////////////////////////////////////////////////////////////////////////////////
void menu_print_list_in_window(menu_status_t current_menu){
    
    lcd.clear();
    menu_print_cursor_position(current_menu.cursor_display_position);

    int print_qty = 0;
    // determining how much to print on the lcd screen depending of the menu length
    if(current_menu.list_end_position < DISPLAY_MAX_ROW - 1){
    // the menu is shorter than the maximum display rows qty => just print till the end of the menu
        print_qty = current_menu.list_end_position + 1;      
    }
    else{
    // the menu is larger than the maximum display rows qty => print DISPLAY_MAX_ROW and then resolve depending of the navigation
        print_qty = DISPLAY_MAX_ROW;
    }
    

    // Print menu in window
    for(int i=0; i<print_qty;i++){
//        lcd.setCursor(2,i);
//        lcd.print("                ");
        lcd.setCursor(2,i);
        lcd.print(current_menu.menu[current_menu.window_start_position+i].menu_item_name);
    }


    // Print arrows if there is more menu up or down.
    if(current_menu.window_start_position < current_menu.list_end_position - DISPLAY_MAX_ROW + 1){
        menu_print_downarrow(); 
    }
    else{
        lcd.setCursor(DISPLAY_MAX_COLS-1,3);
        lcd.print(" ");  
    }

    if(current_menu.window_start_position > 0){
        menu_print_uparrow();
    }
    else{
        lcd.setCursor(DISPLAY_MAX_COLS-1,0);
        lcd.print(" ");
    }
  
}



////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for initialize the current menu   ////////////////////
////////////////////////////////////////////////////////////////////////////////////
void menu_initialization(menu_status_t *current_menu){
    current_menu->cursor_position = 0;  
    current_menu->cursor_display_position = 0;
    current_menu->window_start_position = 0;
    current_menu->list_end_position = menu_last_row_number(current_menu->menu);    
}
                

////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for navigating the current menu   ////////////////////
////////////////////////////////////////////////////////////////////////////////////
void navegate_menu(menu_status_t* current_menu, cursor_states cursor_action){
    
    int i = 0;
  
    switch(cursor_action){
        case cursor_up: 
            i = -1; 
            break;
        
        case cursor_down:
            i = 1;
            break;        
            
        case cursor_right: case cursor_enter: 
        // Al seleccionar se ejecuta la funcion o ingreso al submennu
//            blink_led(LED_WHITE_PIN);
            if(current_menu->menu[current_menu->cursor_position].submenu != NULL){
            // Si entra aca es porque existe un submenu y por lo tanto no existe una accion a ejecutar
                current_menu->menu = current_menu->menu[current_menu->cursor_position].submenu;
                menu_initialization(current_menu);
                return; 
            }
            if(current_menu->menu[current_menu->cursor_position].ptr_function_action != NULL){
            // Si entra aca es porque existe una accion a ejecutar y por lo tanto no existe un submenu
                (*current_menu->menu[current_menu->cursor_position].ptr_function_action)(current_menu->menu[current_menu->cursor_position].params);                 
                return;
            }
            else{
                return;
            } 
          
        case cursor_left:
        // Al seleccionar se vuelve al menu padre del submennu
//            blink_led(LED_WHITE_PIN); 
            current_menu->menu = current_menu->menu[current_menu->list_end_position+1].submenu;
            menu_initialization(current_menu);           
            return;
              
        case cursor_back: 
        // al presionar se vuelve al menu principal
//            blink_led(LED_WHITE_PIN);
            current_menu->menu = INITIAL_MENU_TAG;
//            current_menu->menu = menu_bienvenida;
//            current_menu->menu = menu_principal;
            menu_initialization(current_menu);
            return;  
                
        case cursor_standby:
//            i = 0;
            return;
//            break;   
    }

    int next_cursor_position = current_menu->cursor_position + i ;
    int aux_window_start = current_menu->window_start_position;
    int aux_window_end = aux_window_start + DISPLAY_MAX_ROW - 1;


///////////////////////////////////////////////////////////////////////////
// Caso en que el cursor se mueva fuera de la ventana del menu Y el menu sea mas corto que el display.
///////////////////////////////////////////////////////////////////////////
    if(next_cursor_position >= current_menu->list_end_position && current_menu->list_end_position < DISPLAY_MAX_ROW - 1){
    // si entra aca es porque la lista del menu es menor que las filas de la pantalla
        current_menu->cursor_position = current_menu->list_end_position;
        current_menu->cursor_display_position = current_menu->list_end_position;
        return;
    }   

///////////////////////////////////////////////////////////////////////////
// Caso en que el cursor se mueva dentro de la ventana del menu.
// mientras que el cursor este dentro de la ventana, no muevo la ventana sino solo el cursor.
///////////////////////////////////////////////////////////////////////////
    if(aux_window_start < next_cursor_position && next_cursor_position < aux_window_end){
    //   aux_window_start < next_cursor_position < aux_window_end
    //   aux_window_start < next_cursor_position < aux_window_start + DISPLAY_MAX_ROW - 1
    //   0 < next_cursor_position - aux_window_start < DISPLAY_MAX_ROW - 1
        current_menu->cursor_position = current_menu->cursor_position + i;
        current_menu->cursor_display_position = current_menu->cursor_display_position + i;
        return;
    } 

///////////////////////////////////////////////////////////////////////////
// Caso en que el cursor se vaya fuera del inicio o fin del menu
///////////////////////////////////////////////////////////////////////////
    if(next_cursor_position <= 0){
    // si la proxima seleccion es menor o igual al inicio del menu, el inicio de la ventana del menu debe ser 0 
        current_menu->window_start_position = 0;
        current_menu->cursor_display_position = 0;
        current_menu->cursor_position = 0;
        return ;
    }

    if(next_cursor_position >= current_menu->list_end_position){
    // si la proxima seleccion es mayor o igual al fin del menu, el inicio de la ventana del menu debe ser el fin del menu - DISPLAY_MAX_ROW + 1
        current_menu->window_start_position = current_menu->list_end_position - (DISPLAY_MAX_ROW - 1);
        current_menu->cursor_display_position = DISPLAY_MAX_ROW - 1;
        current_menu->cursor_position = current_menu->list_end_position;
        return;            
    }
    

///////////////////////////////////////////////////////////////////////////
// Caso en que el cursor se vaya fuera del rango de la ventana del menu.
// Caso 1: Hay que mover la ventana del menu para arriba o para abajo.
// Caso 2: No se mueve el menu porque ya esta en la posicion 0 o en la posicion final.
///////////////////////////////////////////////////////////////////////////
    if(next_cursor_position >= aux_window_end){
    // next_cursor_position > aux_window_end || next_cursor_position < aux_window_start
    // si el cursor se va de rango por abajo del ultimo item de la ventana, hay que mover la ventana hacia abajo.
        current_menu->window_start_position = current_menu->window_start_position + i;
        current_menu->cursor_position = current_menu->cursor_position + i;
        current_menu->cursor_display_position = 2 ;
        return;
    }

    if(next_cursor_position <= aux_window_start){
    // si el cursor se va de rango por arriba del primer item de la ventana, hay que mover la ventana hacia arriba.
        current_menu->window_start_position = current_menu->window_start_position + i;
        current_menu->cursor_position = current_menu->cursor_position + i;
        current_menu->cursor_display_position = 1 ;
        return;
    }
      
}

////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for navigate the menu    /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void access_menu(menu_status_t* current_menu, cursor_states cursor_action){
    
    cursor_action = determinar_accion_cursor();
    
    if(cursor_action != cursor_standby){
        cursor_action = determinar_accion_cursor();
        navegate_menu(current_menu,cursor_action);
        menu_print_list_in_window(*current_menu);
        cursor_action = cursor_standby;
//        print_states(*current_menu,cursor_action);
//        delay(DELAY_TIME);

    }
}




////////////////////////////////////////////////////////////////////////////////////
////////////////    Functions for testing the navigation of the menu   /////////////
////////////////////////////////////////////////////////////////////////////////////
void prueba_recorrer_menu(menu_status_t current_menu){
    
    int i = 0;
    int end_pos = current_menu.list_end_position;
    int mid_pos = end_pos/2;
    int mid_pos_2 = (mid_pos + end_pos)/2;
    
    cursor_states cursor_action = cursor_standby;

    for(i=0;i<end_pos;i++){
    
        cursor_action = cursor_down;
        navegate_menu(&current_menu,cursor_action);
        menu_print_list_in_window(current_menu);
        print_states(current_menu,cursor_action);

        delay(DELAY_TIME);
 
    }

    int j = i;
    
    for(j=i;j>mid_pos;j--){
        
        cursor_action = cursor_up;
        navegate_menu(&current_menu,cursor_action);
        menu_print_list_in_window(current_menu);
        print_states(current_menu,cursor_action);

        delay(DELAY_TIME);
        
    }

    for(i=j;i<mid_pos_2;i++){
    
        cursor_action = cursor_down;
        navegate_menu(&current_menu,cursor_action);
        menu_print_list_in_window(current_menu);
        print_states(current_menu,cursor_action);

        delay(DELAY_TIME);
    
    }
    
    for(j=i;j>0;j--){
        
        cursor_action = cursor_up;
        navegate_menu(&current_menu,cursor_action);
        menu_print_list_in_window(current_menu);
        print_states(current_menu,cursor_action);

        delay(DELAY_TIME);
        
    }

}
