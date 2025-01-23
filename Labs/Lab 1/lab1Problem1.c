// Author: Rahma Seid
// Course: CSCI 3240
// Lab 1

#include <stdio.h>
int main(){
    char s_scale; //starting temperature scale
    char t_scale; //targeting temperature scale
    float temp; //temperature
    float con_temp; //converted temperature
    
    
    printf("Enter starting temperature scale (F/C/K): ");
    scanf(" %c", &s_scale);
    
    printf("Enter target temperature scale (F/C/K): ");
    scanf(" %c", &t_scale);
    
    printf("Enter temperature value: ");
    scanf("%f", &temp);

    if ((s_scale == 'F', 'C', 'K') && (t_scale == 'F', 'C', 'K')){
        switch (s_scale){
            case 'F': //case for Fahrenheit
                if (t_scale == 'K'){
                    con_temp = ((temp-32)*(5/9)+273.15); //Fahrenheit converts to Kelvin
                }
                else if (t_scale == 'C'){
                    con_temp = ((temp-32)*5/9); //Fahrenheit converts to Celcius
                }
                else{
                    con_temp == temp; //temp. doesn't convert
                }
                break;
            
            case 'K':
                if (t_scale == 'F'){
                    con_temp = ((temp - 273.15)*(9/5)+32); //Kelvin converts to Fahrenheit
                }
                else if (t_scale == 'C'){
                    con_temp = (temp - 273.15); //Kelvin converts to Celcius
                }
                else{
                    con_temp = temp; //temp. doesn't convert
                }
                break;

            case 'C':
                if (t_scale == 'F'){
                    con_temp = ((temp * (9/5)) + 32); // Celcius converts to Fahrenheit
                }
                else if (t_scale == 'K'){
                    con_temp = (temp + 273.15); // Celcius converts to Kelvin
                }
                else{
                    con_temp = temp; //temp. doesn't convert
                }
                break;
        }

    printf("%.2f° %c is equivalent to %.2f° %c.\n", temp, s_scale, con_temp, t_scale);

    }
    return 0;
}