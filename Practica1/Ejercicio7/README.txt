
Ejercicio 7 :

        Compilamos cada .c con gcc -Wall "ascensor.c" -o "ascensor"
        Ejecutamos cada linea en una terminal:  1) ./ascensor
                                                2) ./sensor "pid_ascensor" "piso al que pertenece (entre 0-15)" 
                                                    Repetimos el paso 2 para el numero de pisos que tengamos.
                                                3) ./pulsador "pid_ascensor" "numero de pisos contando el piso 0"   
                                                    Cuando nos pidan en pulsador los pid de los sensores los escribimos por orden empezando desde el sensor del piso 0.
    
        El numero de sensores que tenemos tiene que ser igual al numero de pisos declarados en el pulsador.
