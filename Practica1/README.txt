Ejercicio1:

Compilamos con: gcc -Wall Ej1.c -o Ej1

Ejecutamos con: ./Ej1

En otra terminal: pidof Ej1
                  kill -SIGKILL "numero de proceso"   
                    
                    OR

                  kill -SIGKILL `pidof Ej1` 

    Sustituimos SIGKILL por cualquier otra senal.

    Algunas senales son:

    SIGTERM (15): Senal generica para parar un programa, puede ser bloqueada, handled o ignorada. Forma correcta de pedirle a un programa   que termine. 
    SIGINT (2): Senal de interrupcion de programa normalmente control c.
    SIGQUIT (3): Senal parecida a SIGINT pero que produce un core dump cuando termina el proceso, como una senal de error de programa.
    SIGKILL (9): Senal utilizada para finalizar un programa inmediatamente. Siempre es fatal y no puede ser bloqueada. Debe ser usada como ultimo recurso tras probar SIGTERM.
    SIGHUP (1): Senal para reportar que la terminal del proceso ha sido desconectada.

Ejercicio2: 

    Compilamos con: gcc -Wall Ej2.c -o Ej2
                    gcc -Wall kill.c -o kill

    Ejecutamos ambos en distinatas terminales: ./Ej2 
                                                pidof Ej2 --> Nos da el pid de Ej2
                                                kill -L --> Nos da las senales ordenadas con sus numeros
                                                ./kill "numero de senal" "pid"


Ejercicio 3:

       Compilamos con: gcc -Wall Ej3.c -o Ej3
       Ejecutamos: ./Ej3
                   pidof Ej3
                   kill -"Senal" "pid"


Ejercicio 4:

           Ej4RESTART: 

                Si no activamos el flag SA_RESTART si llega una senal mientras el programa esta ejecutando una llamada al sistema, este aborta y devueve un error EINTR. Al activar SA_RESTART, la llamada se reinicia cuando el handler acabe.

           Ej4RESETHAND:
                
                Si activamos esta flag, el handler de la senal se reinicia a SIG_DFL una vez la senal es procesada por el handler.
                P.ej: Si llega SIGUSR1 y luego SIGUSR1, al llegar la segunda senal se va a procesar con el handler por defecto.
                      Si llega SIGUSR1, luego SIGUSR2 y luego SIGUSR2, la senal SIGUSR1 y la primera SIGUSR2 se procesaran con el handler modificado, pero la segunda SIGUSR2 con el handler por defecto. Si llegara luego otra SIGUSR1 se     proceasaria tambien con el por defecto.

           Mascara: Bloquea el resto de senales durante la ejecucion del handler.


Ejercicios 5-6-7: 

            Compilamos y buscamos el pid del ascensor con pidof ascensor          








