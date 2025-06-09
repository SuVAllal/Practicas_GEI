(* VERSION 1: power : int -> int -> int *)

let rec power x y =
	if y = 0 then 1
	else x * power x (y - 1);;


(* VERSION 2: power' : int -> int -> int *)

let rec power' x y =
	if y = 0 then 1
	else if (y mod 2 = 0) then power' (x * x) (y / 2)
	else x * power' (x * x) (y / 2);;

(* La función power' es más eficiente ya que en cada iteración el exponente se reduce
a la mitad, y no solo de uno en uno como en la función power. De esta forma, el número
de llamadas recursivas es menor y la operación es menos costosa (pues el número total 
de multiplicaciones se reduce). 
En cuanto a si merece la pena la ganancia operando en 'int' en lugar de 'Z' depende del
rango de números a utilizar, si los números son pequeños y no provocan desbordamiento,
entonces sí merece la pena la ganancia usando 'int', pero sacrificando la capacidad de
manejar números mayores sin desbordamientos.*)


(* VERSIÓN 3: powerf : float -> int -> float *)
let rec powerf x n =
	if n = 0 then 1.
	else if (n mod 2 = 0) then powerf (x *. x) (n / 2)
	else x *. powerf (x *. x) (n / 2);;