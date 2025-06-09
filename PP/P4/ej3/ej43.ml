(* EJERCICIO 1: reverse : int -> int *)
let rec reverse n =
	let rec aux_reverse reversed n =
		if n < 10 then reversed * 10 + n
		else let last_digit = n mod 10 in
		let new_n = reversed * 10 + last_digit in
		aux_reverse new_n (n / 10)
	in aux_reverse 0 n;;


(* EJERCICIO 2: palindromo : string -> bool *)
let rec palindromo cadena =
	let rec cnt i =
		if i >= (String.length cadena - i) then true
		else if cadena.[i] <> cadena.[String.length cadena - i - 1] then false
		else cnt (i + 1)
	in cnt 0;;


(* EJERCICIO 3: mcd : int * int -> int *)
let rec mcd (x, y) =
	if y <= 0 then x
	else mcd(y, x mod y);;