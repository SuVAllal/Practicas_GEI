let come (i1, j1) (i2, j2) =
	i1 = i2 || j1 = j2 || abs(i2 - i1) = abs (j2 - j1);; (* se comen cuando están en la misma fila, en la misma columna o en la misma diagonal *)
	(* Están en la misma diagonal si la distancia entre filas es igual a la distancia horizontal *)

let compatible p l =
	not (List.exists (come p) l);; (* Miramos si es posible poner una reina en una casilla sin que resulte amenazada *)

(* FUNCION QUEENS *)
let queens n =
	let rec completa path i j =
		if i > n then [path]
		else if j > n then []
		else if compatible (i, j) path
			then completa ((i, j)::path) (i+1) 1 @ completa path i (j+1)
			else completa path i (j+1)
	in completa [] 1 1;;


(* FUNCIÓN IS_QUEENS_SOL *)
let is_queens_sol n sol =
	let rec validar_sol = function
		[] -> true
		| (i, j) :: t -> 
		i > 0 && i <= n && j > 0 && j <= n && 
		compatible (i, j) t && validar_sol t
	in List.length sol = n && validar_sol sol;;