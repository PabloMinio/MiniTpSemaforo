# Informe MiniTP 3: Threads y Semáforos

## Pseucódigo

```
sem_mutex1 // inicializado en 1

cortar()  // cortar ajo, perejil
   v(sem_mezclar)

mezclar() // mezclar ajo, perefil y huevo
  p(sem_mezclar)
  v(sem_salar)

salar() // salar la mezcla
  p(sem_mutex1)
  p(sem_salar)
  v(sem_mutex1)
  v(sem_agregarCarne)
  
agregarCarne() // agregar carne a la mezcla
  p(sem_agregarCarne)
  v(sem_empanizar)

empanizar() // empanar la carne
  p(sem_empanizar)
  v(sem_fritar)

fritar() // cocinar la milanesa
  p(sem_mutex1)
  p(sem_fritar)
  v(sem_mutex1)

cortarVerduras() // cortar cebolla morada, lechuga, tomate, pepino
```

## Dificultades
Las dificultades que tuve fueron:
- Hornear el pan: intenté hacerlo guiándome con el ejemplo del barbero durmiente, pero cuando un pan entraba en el horno, todos los procesos dejaban de funcionar y el programa no dejaba de ejecutarse.
- Leer la receta desde un archivo: traté de realizarlo usando **struct** o **strtok**, pero utilizaba todas las palabras del texto y no solamente las que necesitaba.
- La salida sea escrita en otro archivo: no supe donde poder implementarlo para que pueda escribir paso por paso.
- La superposición de las tareas.

## Soluciones
Para la superposición de las tareas, aprendí como usar el mutex e implementarlo. Las demás dificultades no las pude resolver.
