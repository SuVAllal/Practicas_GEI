(* val hd: 'a list -> 'a *)
let hd l = match l with
	| h::_ -> h
	| [] -> raise (Failure "hd");;



(* val tl: 'a list -> 'a list *)
let tl l = match l with
	| _::t -> t
	| [] -> raise (Failure "tl");;



(* val length: 'a list -> int *)
let rec length = function
    [] -> 0
    | _::t -> 1 + length t;;



(* val compare_lengths : 'a list -> 'b list -> int *)
let rec compare_lengths l1 l2 =
    match (l1, l2) with
        ([],[]) -> 0
        | ([], _) -> -1
        | (_, []) -> 1
        | (_::t1, _::t2) -> compare_lengths t1 t2;;



(* val compare_length_with : 'a list -> int -> int *)
let rec compare_length_with l len = match l, len with
	[], 0 -> 0
	| _::t, n when n > 0 -> compare_length_with t (n - 1)
	| _::_, _ -> 1
	| _, _ -> -1;;



(* val append : 'a list -> 'a list -> 'a list *)
(* not tail recursive *)
let rec append l1 l2 = match l1 with
  [] -> l2
  | h::t -> h:: append t l2;;



(* val rev_append : 'a list -> 'a list -> 'a list *)
let rec rev_append l1 l2 = match l1 with
	[] -> l2
	| h::t -> rev_append t (h::l2);;



(* val rev : 'a list -> 'a list *)
(* Recursiva no terminal *)
let rec rev = function
	[] -> []
	| h::t -> append (rev t) [h];;

(* Recursiva terminal *)
let rev l = rev_append l [];;



(* val init : int -> (int -> 'a) -> 'a list *)
let init len f =
	if len < 0 then raise (Invalid_argument "init")
	else let rec aux_init (i, l) =
		if i = len then l else aux_init(i + 1, f i::l)
	in rev (aux_init (0, []));;



(* val nth : 'a list -> int -> 'a *)
let nth l n =
	if n < 0 then raise (Invalid_argument "nth")
	else let rec aux_nth = function
		([], _) -> raise (Failure "nth")
		| (h::_, 0) -> h
		| (_::t, i) -> aux_nth (t, i - 1) in aux_nth (l ,n);;



(* val concat : 'a list list -> 'a list *)
(* not tail recursive *)
let rec concat = function
	[] -> []
	| h::t -> append h (concat t);;



(* val flatten : 'a list list -> 'a list *)
(* not tail recursive *)
let flatten = concat;;



(* val split : ('a * 'b) list -> 'a list * 'b list *)
(* not tail recursive *)
let rec split = function
	[] -> ([], [])
	| (h1, h2)::t -> let t1, t2 = split t 
		in h1::t1, h2::t2;;



(* val combine : 'a list -> 'b list -> ('a * 'b) list *)
(* not tail recursive *)
let rec combine l1 l2 =
	match (l1, l2) with
		[], [] -> []
		| h1::t1, h2::t2 -> (h1, h2)::(combine (t1)(t2))
		| _ -> raise (Invalid_argument "combine");;



(* val map : ('a -> 'b) -> 'a list -> 'b *)
(* not tail recursive *)
let rec map f = function
	[] -> []
	| h::t -> (f h)::(map f t);;



(* val map2 : ('a -> 'b -> 'c) -> 'a list -> 'b list -> 'c list *)
(* not tail recursive *)
let rec map2 f l1 l2 =
	if(length l1 <> length l2) then raise (Invalid_argument "map2")
	else if (length l1 = 0) then []
	else (f(hd l1)(hd l2))::map2 f (tl l1)(tl l2);;



(* val rev_map : ('a -> 'b) -> 'a list -> 'b list *)
(* Recursiva no terminal *)
let rev_map f l = rev (map f l);;

(* Recursiva terminal *)
let rev_map f l =
	let rec aux_rev l auxl = match l with
		[] -> auxl
		| h::t -> aux_rev t (f(h)::auxl)
	in aux_rev l [];;



(* val for_all : ('a -> bool) -> 'a list -> bool *)
let rec for_all f = function
	[] -> true
	| h::t -> f h && for_all f t;;



(* val exists : ('a -> bool) -> 'a list -> bool *)
let rec exists f = function
	[] -> false
	| h::t -> f h || exists f t;;



(* val mem : 'a -> 'a list -> bool *)
let rec mem a = function
	[] -> false
	| h::t -> if (a = h) then true else (mem a t);;



(* val find : ('a -> bool) -> 'a list -> 'a *)
let rec find f = function
	[] -> raise (Not_found)
	| h::t -> if f h then h else find f t;;



(* val find_all : ('a -> bool) -> 'a list -> 'a list *)
let rec find_all f = function
	[] -> []
	| h::t -> if (f h) then h::(find_all f t) else find_all f t;;



(* val filter : ('a -> bool) -> 'a list -> 'a list *)
let filter = find_all;;



(* val partition : ('a -> bool) -> 'a list -> 'a list * 'a list *)
let partition f l =
	let rec aux_partition f l l1 l2 = match l with
		[] -> (rev l1, rev l2)
		| h::t -> if f h then aux_partition f t (h::l1) l2
				  else aux_partition f t l1 (h::l2) 
	in aux_partition f l [] [];;



(* val fold_left : ('a -> 'b -> 'a) -> 'a -> 'b list -> 'a *)
let rec fold_left f a l = match l with
	[] -> a
	| h::t -> fold_left f (f a h) t;;



(* val fold_right : ('a -> 'b -> 'b) -> 'a list -> 'b -> 'b *)
(* not tail recursive *)
let rec fold_right f l a = match l with
	[] -> a
	| h::t -> f h (fold_right f t a);;



(* val assoc : 'a -> ('a * 'b) list -> 'b *)
let rec assoc key l = match l with
	[] -> raise (Not_found)
	| (a, b)::t -> if a = key then b else assoc key t;;



(* val mem_assoc : 'a -> ('a * 'b) list -> bool *)
let rec mem_assoc key l = match l with
	[] -> false
	| (a, b)::t -> if a = key then true else mem_assoc key t;;


(* val remove_assoc : 'a -> ('a * 'b) list -> ('a * 'b) list *)
(* not tail recursive *)
let rec remove_assoc key l = match l with
	[] -> []
	| (a, b)::t -> if a = key then t else (a, b)::(remove_assoc key t);;