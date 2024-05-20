#include "TyH.h"
#include "hardware_definitions.h"


//extern bool alarma_prendida;
extern bool alarma_prendida_t;
extern bool alarma_prendida_h;

//extern double stored_alarm_treshold_h;
//extern double stored_alarm_treshold_t;


extern telemetry_t data_t;
extern telemetry_t data_h;




void blink_lcd(){
    lcd.noBacklight();
    delay(DELAY_TIME/5);
    lcd.backlight();
    delay(DELAY_TIME/5);
}

void blink_led(int led_pin){
    digitalWrite(led_pin, HIGH);
    delay(DELAY_TIME/5);
    digitalWrite(led_pin, LOW);
    delay(DELAY_TIME/5);
}

void alarm_lcd_blink_t(bool state){
    if(state == true){
        blink_lcd();
//        blink_led(LED_WHITE_PIN);
    }
    else{
        return;
    }

};


void alarm_lcd_blink_h(bool state){
    if(state == true){
//        blink_lcd();
        blink_led(LED_WHITE_PIN);
    }
    else{
        return;
    }
};



void check_tresholds(){
    
    float current_h = get_hum(NULL);
    float current_t = get_temp(NULL);
    
    if( (data_t.min_treshold >= data_t.data) || (data_t.data >= data_t.max_treshold) ){
        alarma_prendida_t = true;
    }
    else{
        alarma_prendida_t = false;
    }

    if( (data_h.min_treshold >= data_h.data) || (data_h.data >= data_h.max_treshold) ){
        alarma_prendida_h = true;
    }
    else{
        alarma_prendida_h = false;
    }

    
}



void get_telemetry(){
    data_h.data = get_hum(NULL);
    data_t.data = get_temp(NULL);
}


//double t_last;
//double h_last;

void print_telemetry(){

//    double h_new = get_hum(NULL);
//    double t_new = get_temp(NULL);
    
//    if(h_new != h_last){
//        lcd.setCursor(6,2);
//        lcd.print("     ");
//        
//        h_last = h_new;
//        data_h.data = h_new;
//
//        lcd.setCursor(6,2);
//        lcd.print(String(data_h.data));
//    }
//
//    if(t_new != t_last){
//        lcd.setCursor(6,0);
//        lcd.print("      ");
//        
//        t_last = t_new;
//        data_t.data = t_new;
//
//        lcd.setCursor(6,0);
//        lcd.print(String(data_t.data));
//    }
    
//    lcd.clear();

    get_telemetry();
//    check_tresholds();
//    alarm_lcd_blink_t(alarma_prendida_t);
//    alarm_lcd_blink_h(alarma_prendida_h); 

    lcd.setCursor(6,0);
    lcd.print(String(data_t.data));
    lcd.setCursor(6,1);
    lcd.print(String(data_h.data));
            
    lcd.setCursor(0,0);
    lcd.print("  T = ");
    lcd.setCursor(11,0);
    lcd.print(String((char)223)+String("C        "));
    
    lcd.setCursor(0,1);
    lcd.print("  H = ");
    lcd.setCursor(11,1);
    lcd.print(String(" %        "));

    lcd.setCursor(0,2);
    lcd.print("  Alarmas: ");

    lcd.setCursor(0,3);
    lcd.print("  Ingresar menu: A");

    
    if(alarma_prendida_t){
        lcd.setCursor(11,2);
        lcd.print("T-S ");      
    }
    else{
        lcd.setCursor(11,2);
        lcd.print("T-N ");       
    }


    if(alarma_prendida_h){
        lcd.setCursor(15,2);
        lcd.print("H-S  ");      
    }
    else{
        lcd.setCursor(15,2);
        lcd.print("H-N  ");       
    }

};
