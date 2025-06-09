(*EXPRESIÓN 1*)
(*x - y*);;
(*Error: unbound value x*)


(*EXPRESIÓN 2*)
let x = 1;;
(*val x : int = 1*)


(*EXPRESIÓN 3*)
(*x - y*);;
(*Error: unbound value y*)


(*EXPRESIÓN 4*)
let y = 2;;
(*val y : int = 2*)


(*EXPRESIÓN 5*)
x - y;;
(*- : int = -1*)


(*EXPRESIÓN 6*)
let x = y in x - y;;
(*- : int = 0*)


(*EXPRESIÓN 7*)
x - y;;
(*- : int = -1*)


(*EXPRESIÓN 8*)
(*z*);;
(*Error: unbound value z*)


(*EXPRESIÓN 9*)
let z = x + y;;
(*val z : int = 3*)


(*EXPRESIÓN 10*)
z;;
(*- : int = 3*)


(*EXPRESIÓN 11*)
let x = 5;;
(*val x : int = 5*)


(*EXPRESIÓN 12*)
x + y;;
(*- : int = 7*)


(*EXPRESIÓN 13*)
z;;
(*- : int = 3*)


(*EXPRESIÓN 14*)
let y = 5 in x + y;;
(*- : int = 10*)


(*EXPRESIÓN 15*)
x + y;;
(*- : int = 7*)


(*EXPRESIÓN 16*)
let x = x + y in let y = x * y in x + y + z;;
(*- : int = 24*)


(*EXPRESIÓN 17*)
x + y + z;;
(*- : int = 10*)


(*EXPRESIÓN 18*)
function x -> 2 * x;;
(*- : int -> int = <fun>*)


(*EXPRESIÓN 19*)
(function x -> 2 * x) (2 + 1);;
(*- : int = 6*)


(*EXPRESIÓN 20*)
(function x -> 2 * x) 2 + 1;;
(*- : int = 5*)


(*EXPRESIÓN 21*)
let f = function x -> 2 * x;;
(* (val f : int -> int = <fun>) *)


(*EXPRESIÓN 22*)
f;;
(* (- : int -> int = <fun>) *)


(*EXPRESIÓN 23*)
f (2 + 1);;
(* (- : int = 6) *)


(*EXPRESIÓN 24*)
f 2 + 1;;
(* (- : int = 5) *)


(*EXPRESIÓN 25*)
f x;;
(* (- : int = 10) *)


(*EXPRESIÓN 26*)
let x = 100;;
(*val x : int = 100*)


(*EXPRESIÓN 27*)
f x;;
(*- : int = 200*)


(*EXPRESIÓN 28*)
let m = 1000;;
(*val m : int = 1000*)


(*EXPRESIÓN 29*)
let g = function x -> x + m;;
(* (val g : int -> int = <fun>) *)


(*EXPRESIÓN 30*)
g;;
(* (- : int -> int = <fun>) *)


(*EXPRESIÓN 31*)
g 3;;
(* (- : int = 1003) *)


(*EXPRESIÓN 32*)
let istrue = function true -> true;;
(* Warning: esta función no es exhaustiva, no tiene en cuenta el caso "false" *)
(* val istrue : bool -> bool = <fun> *)


(*EXPRESIÓN 33*)
(*istrue;;*)
(*- : bool -> bool = <fun>*)


(*EXPRESIÓN 34*)
(*istrue (1 < 2);;*)
(* - : bool = true *)


(*EXPRESIÓN 35*)
istrue (2 < 1);;
(*Exception: Match_failure // La función istrue no tiene en cuenta el caso "false"*)


(*EXPRESIÓN 36*)
(*istrue 0;;*)
(*Error de tipos: la función recibe un entero cuando espera un bool*)


(*EXPRESIÓN 37*)
let iscero_v1 = function 0 -> true;;
(* (Warning: esta función no es exhaustiva, solo tiene en cuenta el caso 0, no los demás enteros val iscero_v1 : int -> bool = <fun>) *)


(*EXPRESIÓN 38*)
iscero_v1 0;;
(* (- : bool = true) *)


(*EXPRESIÓN 39*)
(*iscero_v1 0.;;*)
(* (Error de tipos: la función recibe un float cuando espera un entero) *)


(*EXPRESIÓN 40*)
iscero_v1 1;;
(* (Exception: Match_failure // La función iscero_v1 no tiene en cuenta el caso de otros enteros aparte de 0) *)


(*EXPRESIÓN 41*)
let iscero_v2 = function 0 -> true | _ -> false;;
(*val iscero_v2 : int -> bool = <fun>*)


(*EXPRESIÓN 42*)
iscero_v2 0;;
(*- : bool = true*)


(*EXPRESIÓN 43*)
iscero_v2 1;;
(*- : bool = false*)


(*EXPRESIÓN 44*)
(*iscero_v2 0.;;*)
(*Error de tipos: la función recibe un float cuando espera un entero*)


(*EXPRESIÓN 45*)
let all_to_true = function true -> true | false -> true;;
(* (val all_to_true : bool -> bool = <fun>) *)


(*EXPRESIÓN 46*)
all_to_true (1 < 2);;
(* (- : bool = true) *)


(*EXPRESIÓN 47*)
all_to_true (2 < 1);;
(*(- : bool = true)*)


(*EXPRESIÓN 48*)
(*all_to_true 0;;*)
(* (Error de tipos: la función recibe un int cuando espera un bool) *)


(*EXPRESIÓN 49*)
let first_all_to_true = all_to_true;;
(*val first_all_of_true : bool -> bool = <fun>*)


(*EXPRESIÓN 50*)
let all_to_true = function x -> true;;
(*(val all_to_true : 'a -> bool = <fun>)*)


(*EXPRESIÓN 51*)
all_to_true (1 < 2);;
(*(- : bool = true)*)


(*EXPRESIÓN 52*)
all_to_true (2 < 1);;
(*(- : bool = true)*)


(*EXPRESIÓN 53*)
all_to_true 0;;
(*(- : bool = true)*)


(*EXPRESIÓN 54*)
(*first_all_to_true 0;;*)
(* (Error de tipos: la función recibe un int cuando espera un bool) *)	