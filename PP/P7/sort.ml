(* IMPLEMENTACIÓN DEL MÉTODO DE ORDENACIÓN POR INSERCIÓN *)
(* funciones recursivas no terminales *)
let rec insert x = function
	[] -> [x]
	| h::t -> if x <= h then x::h::t
			  else h::insert x t;;

let rec isort = function
	[] -> []
	| h::t -> insert h (isort t);;

let bigl = List.init 1_000_000 succ;;

(* FUNCIONES RECURSIVAS TERMINALES *)
(* insert_t: ‘a -> ‘a list -> ‘a list  *)
let insert_t x l =
	let rec aux_insert aux = function
		[] -> List.rev (x::aux)
		| h::t -> if x <= h then List.rev_append aux (x::h::t)
				  else aux_insert (h::aux) t
	in aux_insert [] l;;

(* isort_t: ‘a list -> ‘a list *)
let isort_t l =
	let rec aux_isort aux = function
		[] -> aux
		| h::t -> aux_isort (insert_t h aux) t
	in aux_isort [] l;;

(* LISTA ALEATORIA *)
(* rlist: int -> int list *)
let rlist n =
	let rec aux_rlist aux remaining =
		if remaining = 0 then aux
		else 
			let random = Random.int n in
				if not (List.mem random aux) then aux_rlist (random::aux) (remaining - 1)
				else aux_rlist aux remaining
	in aux_rlist [] n;;

(* CRONO *)
let crono f x =
	let t = Sys.time () in
	let _ = f x in
	Sys.time () -. t;;

let lc1 = List.init 10_000 succ;; 
(* TIEMPO DE EJECUCION:  
# crono isort lc1;;
- : float = 0.00344599999999672946
#crono isort_t lc1;;
- : float = 1.9916419999999988 *)

let lc2 = List.init 20_000 succ;;
(* TIEMPO DE EJECUCION:  
# crono isort lc2;;
- : float = 0.00420300000000395357
#crono isort_t lc2;;
- : float = 9.02697700000000225 *)

let ld1 = List.rev (List.init 10_000 succ);;
(* TIEMPO DE EJECUCION:  
# crono isort ld1;;
- : float = 1.36688400000001309
#crono isort_t ld1;;
- : float = 0.00292100000000061755 *)

let ld2 = List.rev (List.init 20_000 succ);;
(* TIEMPO DE EJECUCION:  
# crono isort ld2;;
- : float = 5.9601159999999993
#crono isort_t ld2;;
- : float = 0.0061589999999966949 *)

let lr1 = rlist 10_000;;
(* TIEMPO DE EJECUCION:
# crono isort lr1;;
- : float = 0.664740999999992255
#crono isort_t lr1;;
- : float = 0.949164000000010333 *)

let lr2 = rlist 20_000;;
(* TIEMPO DE EJECUCION:  
# crono isort lr2;;
- : float = 3.12108100000000377
#crono isort_t lr2;;
- : float = 4.40310499999999649 *)

(* El rendimiento de isort y isort_t varia segun el tipo de lista, siendo
isort generalmente mas rapido para listas crecientes, mientras que isort_t
lo es para listas decrecientes. Esto se debe a que la recursion no terminal
de isort puede ser mas eficiente en terminos de uso de la pila, pues su
recursion se resuelve hacia el final de la lista para listas crecientes.

De esta forma, el rendimiento en listas "aleatorias" depende del patron
especifico de la lista y de la distribucion de los elementos. *)


(* isort_g: ('a -> 'a -> bool) -> 'a list -> 'a list *)
let insert_g cmp x l =
	let rec aux_insert aux = function
		[] -> List.rev_append aux [x]
		| h::t -> if cmp x h then List.rev_append aux (x::h::t)
				  else aux_insert (h::aux) t
	in aux_insert [] l;;

let isort_g cmp l =
	let rec aux_isort aux = function
		[] -> aux
		| h::t -> aux_isort (insert_g cmp h aux) t
	in aux_isort [] l;;



(* METODO DE ORDENACION POR MEZCLA *)
let rec split l = match l with
	h1::h2::t -> let t1, t2 = split t
				 in h1::t1, h2::t2
	| _ -> l, [];;

let rec merge (l1,l2) = match l1, l2 with
	[], l | l, [] -> l
	| h1::t1, h2::t2 -> if h1 <= h2 then h1 :: merge (t1, l2)
						else h2 :: merge (l1, t2);;

let rec msort l = match l with
	[] | [_] -> l
	| _ -> let l1, l2 = split l
in merge (msort l1, msort l2);;


let bigl2 = List.init 1_000_000 succ;;


(* split_t: 'a list -> 'a list * 'a list *)
let split_t l =
	let rec aux_split l l1 l2 = match l with
		[] -> l1, l2
		| h1::h2::t -> aux_split t (h1::l1) (h2::l2)
		| h::t -> (h::l1), l2
	in aux_split l [] [];;


(* merge_t: 'a list * 'a list -> 'a list *)
let merge_t (l1, l2) = 
	let rec aux_merge aux l1 l2 = match l1, l2 with
		[], l | l, [] -> List.rev_append aux l
		| h1::t1, h2::t2 ->
			if h1 <= h2 then aux_merge (h1::aux) t1 l2
			else aux_merge (h2::aux) l1 t2
	in aux_merge [] l1 l2;;


(* msort': 'a list -> 'a list *)
let rec msort' l = match l with
	[] | [_] -> l
	| _ -> let l1, l2 = split_t l
in merge_t (msort' l1, msort' l2);;


let bigl3 = [];;

(* El motivo por el que msort' no produce stack overflow a pesar de no ser
recursiva terminal se debe a que, en la llamada a la función, se divide la
lista en dos partes (l1 y l2). De esta forma, la recursión no es directamente
acumulativa, pues cada llamada recursiva trabaja con sublistas más pequeñas
hasta llegar al caso base, donde no se producen más llamadas recursivas.
Así, la recursión se divide en dos ramas en cada paso, por lo que la altura
del árbol de llamadas recursivas no llega a ser lo suficientemente grande como
para causar stack overflow. *)


(* 
COMPARACIÓN DE LOS TIEMPOS DE EJECUCIÓN DE MSORT'
# crono msort' lc1;; 
- : float = 0.0147469999999998436
# crono msort' lc2;;
- : float = 0.0238949999999960028

# crono msort' ld1;;
- : float = 0.0125180000000000291
# crono msort' ld2;;
- : float = 0.0254540000000034183

# crono msort' lr1;;
- : float = 0.0134529999999983829
# crono msort' lr2;;
- : float = 0.0237849999999966144

Las listas más grandes (lc2, ld2, lr2) toman más tiempo para ordenar que 
las listas más pequeñas (lc1, ld1, lr1). Esto es esperado ya que el algoritmo 
de ordenamiento utilizado por msort' tiene una complejidad de tiempo O(n*logn), 
donde "n" es el tamaño de la lista. Por ello, a medida que el tamaño de la lista 
aumenta, el tiempo de ejecución también tiende a aumentar.


COMPARACIÓN DE MSORT Y MSORT':
# crono msort lr1;; 
- : float = 0.0102249999999983743
# crono msort' lr1;;
- : float = 0.0106380000000001473

# crono msort lr2;; 
- : float = 0.0381950000000017553
# crono msort' lr2;;
- : float = 0.0368229999999982738

Ambos algoritmos tienen un rendimiento bastante similar para las listas aleatorias,
ya que la naturaleza del algoritmo de ordenación utilizado por msort y msort'
(merge sort) no depende significativamente de la distribución inicial de los elementos.
*)

(* msort_g: ('a -> 'a bool) -> 'a list -> 'a list *)

let merge_g ord (l1, l2) =
	let rec aux_merge_g ord aux l1 l2 = match l1, l2 with
		[], l | l, [] -> List.rev_append aux l
		| h1::t1, h2::t2 ->
			if ord h1 h2 then aux_merge_g ord (h1::aux) t1 l2
			else aux_merge_g ord (h2::aux) l1 t2
	in aux_merge_g ord [] l1 l2;;

let rec msort_g ord l = match l with
	[] | [_] -> l
	| _ -> let l1, l2 = split_t l
in merge_g ord (msort_g ord l1, msort_g ord l2);;