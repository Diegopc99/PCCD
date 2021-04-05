 
Practica 2:

Proceso zombie: Proceso hijo a la espera de que su padre sepa que ha muerto.

                SI un proceso padre muere antes que el hijo, el hijo no se mantiene en zombie y muere.
                
Limitadiones del Wait():
        Wait solo permite sicronizar un proceso padre con sus hijos. 
        No podemos sincronizar un hijo con un padre.
        Solo puede sincronizar una vez el hijo muera. No se puede hacer dos sincronizaciones dentro de un proceso hijo.
        
Fork():

    Crea un proceso con el mismo codigo que el padre, con un espacio de memoria independiente, y el mismo codigo. La diferencia es que coloca el contador de programa justo el la linea siguiente al fork() en vez de en el main().
    
Execl():
    
    Cambia todo el proceso(memoria,contador de programa..) excepto la jerarquia del padre (pid, ppid).
    Execl(path,nombre_programa,arg0,arg1..);
                    
    
