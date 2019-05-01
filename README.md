# Simulacion-feria-con-hilos-y-procesos.
Este es el proyecto final del curso de programación concurrente.

Se hace una simulación de las actividades de un parque de diversiones con hilos y procesos. 
Los clientes son hilos o procesos según corresponda. 
Se establece una capacidad máxima de clientes permitidos, haciendo que la feria no admita más del límite hasta que algún cliente salga.
Los clientes pueden estar en 6 posibles estados:
	1- Esperando entrar a la feria.
	2- En los pasillos dentro de la feria 
	3- Formados para subir la montaña rusa 
	4- Dentro de la montaña. 
	5-Formado para subir al carrusel.
	6-Dentro del carrusel.

Cuando ingresan los clientes pueden tomar deciciones:
	-Ir a la montaña rusa
	-Ir al carrusel
	-Estar en los pasillos
	-Irse
Los juegos tienen un tiempo fijado de duración, estos solo funcionan cuando están completamente llenos o ha pasado un tiempo establecido y no se han podido llenar. 

Hay una hora apertura y otra de cierre. Lo que quiere decir que los clientes se pueden formar desde antes de la hora de apertura para ingresar. Y 10 minutos antes de la hora de cierre se empieza a sacar a toda la clientela.
