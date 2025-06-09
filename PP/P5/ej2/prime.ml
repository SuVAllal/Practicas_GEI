let is_prime n =
	let rec check_from i =
		i >= n ||
		(n mod i <> 0 && check_from (i+1))
	in check_from 2;;
	
(* La función is_prime estudia si el número n es primo o no haciendo uso
de una variable i para estudiar los posibles divisores de n.
No obstante, el uso de la recursividad en esta función no es la opción más
eficiente, ya que como la función es recursiva NO terminal (se realiza la
operación adicional && después de la llamada recursiva) esta puede generar
stack overflow en llamadas a números n muy grandes. *)

let rec next_prime n = (* n > 1 *)
	if is_prime (n+1) then n + 1
	else next_prime (n+1)


let rec last_prime_to n = 
	if is_prime(n) then n
	else last_prime_to (n-1);;


let is_prime2 n =
	let rec check_from i =
		i*i > n ||
		n mod i <> 0 && check_from(i+1)
	in check_from 2;;		