type 'a bintree = Empty | Node of 'a * 'a bintree * 'a bintree;;

(* in_order: 'a bintree -> 'a list *)
let rec in_order = function
	Empty -> []
	| Node (r, i, d) -> (in_order i) @ [r] @ (in_order d);;

(* insert: ('a -> 'a -> bool) -> 'a bintree -> 'a -> 'a bintree *)
let rec insert ord tree x = match tree with
	Empty -> Node (x, Empty, Empty)
	| Node (r, i, d) -> if ord x r then Node (r, insert ord i x, d)
						else Node (r, i, insert ord d x);;

(* bst: ('a -> 'a -> bool) -> 'a list -> 'a bintree *)
let bst ord l =
	List.fold_left (fun tree x -> insert ord tree x) Empty l;;

(* qsort: ('a -> 'a -> bool) -> 'a list -> 'a list *)
let qsort ord l =
	let tree = bst ord l in
	in_order tree;;