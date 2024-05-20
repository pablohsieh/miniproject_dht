


//********************* NAV BUTTONS **********************//
#define KEY_NADA    0
#define KEY_ENTER   1
#define KEY_BACK    2
#define KEY_RIGHT   3
#define KEY_LEFT    4
#define KEY_DOWN    5
#define KEY_UP      6


int devolver_boton_presionado(){
    // BOTONERA ARMADA A MANO CON PULSADORES
    // DEVOLVER UN NUMERO DEL 0 AL 6 (Etiquetas en las macros)
    // DEVUELVO KEY_NADA    SI SE PRESIONA    NADA
    // DEVUELVO KEY_ENTER   SI SE PRESIONA    ENTER
    // DEVUELVO KEY_BACK    SI SE PRESIONA    BACK
    // DEVUELVO KEY_RIGHT   SI SE PRESIONA    RIGHT
    // DEVUELVO KEY_LEFT    SI SE PRESIONA    LEFT
    // DEVUELVO KEY_DOWN    SI SE PRESIONA    DOWN
    // DEVUELVO KEY_UP      SI SE PRESIONA    UP
      
    bool buttonState_up = digitalRead(BUTTON_UP);
    bool buttonState_down = digitalRead(BUTTON_DOWN);
    bool buttonState_enter = digitalRead(BUTTON_ENTER);
    bool buttonState_back = digitalRead(BUTTON_BACK);
    bool buttonState_right = digitalRead(BUTTON_RIGHT);
    bool buttonState_left = digitalRead(BUTTON_LEFT);
    
    if(buttonState_up){
        Serial.println(F("devolver_boton_presionado = KEY_UP\n"));
        return KEY_UP;
    }
    else if(buttonState_down){
        Serial.println(F("devolver_boton_presionado = KEY_DOWN\n"));
        return KEY_DOWN;
    }
    else if(buttonState_enter){
        Serial.println(F("devolver_boton_presionado = KEY_ENTER\n"));
        return KEY_ENTER;
    }
    else if(buttonState_back){
        Serial.println(F("devolver_boton_presionado = KEY_BACK\n"));
        return KEY_BACK;
    }
    else if(buttonState_right){
        Serial.println(F("devolver_boton_presionado = KEY_RIGHT\n"));
        return KEY_RIGHT;
    }
    else if(buttonState_left){
        Serial.println(F("devolver_boton_presionado = KEY_LEFT\n"));
        return KEY_LEFT;
    }
//    Serial.println(F("devolver_boton_presionado = KEY_NADA\n"));
    return KEY_NADA;

};



int devolver_keypad_presionado(){
    // KEYPAD matricial 4x4 ARDUINO
    // DEVOLVER UN NUMERO DEL 0 AL 6 (Etiquetas en las macros)
    // DEVUELVO KEY_NADA    SI SE PRESIONA    NADA
    // DEVUELVO KEY_ENTER   SI SE PRESIONA    'A'
    // DEVUELVO KEY_BACK    SI SE PRESIONA    'B'
    // DEVUELVO KEY_RIGHT   SI SE PRESIONA    '6'
    // DEVUELVO KEY_LEFT    SI SE PRESIONA    '4'
    // DEVUELVO KEY_DOWN    SI SE PRESIONA    '8'
    // DEVUELVO KEY_UP      SI SE PRESIONA    '2'
      
    char key = keypad.getKey();

    switch (key){
        case NO_KEY:
//            Serial.println(F("devolver_boton_presionado = KEY_NADA\n"));
            return KEY_NADA; 
        
        case 'A': 
            Serial.println(F("devolver_boton_presionado = KEY_ENTER\n"));
            return KEY_ENTER;        
            
        case 'B': 
            Serial.println(F("devolver_boton_presionado = KEY_BACK\n"));
            return KEY_BACK;        

        case '2': 
            Serial.println(F("devolver_boton_presionado = KEY_UP\n"));
            return KEY_UP;        
        case '4': 
            Serial.println(F("devolver_boton_presionado = KEY_LEFT\n"));
            return KEY_LEFT;        
        
        case '6': 
            Serial.println(F("devolver_boton_presionado = KEY_RIGHT\n"));
            return KEY_RIGHT;        
        
        case '8': 
            Serial.println(F("devolver_boton_presionado = KEY_DOWN\n"));
            return KEY_DOWN;
    
    }

};


cursor_states determinar_accion_cursor(){

//    int pressed_button = devolver_boton_presionado();
    int pressed_button = devolver_keypad_presionado();
    
    switch(pressed_button){ 
        case KEY_DOWN: 
            return cursor_down;
        case KEY_UP: 
            return cursor_up;
        case KEY_RIGHT: 
            return cursor_right;
        case KEY_LEFT: 
            return cursor_left;
        case KEY_ENTER:
            return cursor_enter;
        case KEY_BACK:
            return cursor_back;
        case KEY_NADA: 
            return cursor_standby;
    }
}
