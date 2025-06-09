(*EXPRESION 1*)
let p = (1 + 1, asin 1.), true;;
(*val p : (int * float) * bool = ((2, 1.57079632679489656), true)*)

(*EXPRESIÓN 2*)
let (x, y), z = p;;
(*val x : int = 2
val y : float = 1.57079632679489656
val z : bool = true*)

(*EXPRESIÓN 3*)
let p1, p2 = p in let p11, _ = p1 in (p2, 2 * p11);;
(*- : bool * int = (true, 4)*)

(*EXPRESIÓN 4*)
let f (x, y) = 2 * x + y;;
(*val f : int * int -> int = <fun>*)

(*EXPRESIÓN 5*)
let f2 x y z = x + 2 * y + 3 * z;;
(*val f2 : int -> int -> int -> int = <fun>*)

(*EXPRESIÓN 6*)
let g x y z = x (y, z);;
(*val g : ('a * 'b * 'c) -> 'a -> 'b -> 'c = <fun>*)

(*EXPRESIÓN 7*)
g fst 1 "hola";;
(*- : int = 1*)

(*EXPRESIÓN 8*)
g snd fst true;;
(*- : bool = true*)

(*EXPRESIÓN 9*)
g f 2 3;;
(*- : int = 7*)

(*EXPRESIÓN 10*)
g (function (f, x) -> f (f x)) (function x -> x * x) 3;;
(*- : int = 81*)

(*EXPRESIÓN 11*)
let x, y, z = 1, 2, 3;;
(*val x : int = 1
val y : int = 2
val z : int = 3*)

(*EXPRESIÓN 12*)
f2 x y z;;
(*- : int = 14*)

(*EXPRESIÓN 13*)
let x, y, z = y, z, x in f2 x y z;;
(*- : int = 11*)

(*EXPRESIÓN 14*)
f2 x y z;;
(*- : int = 14*)

(*EXPRESIÓN 15*)
let swap (x, y) = y, x;;
(*val swap: 'a * 'b -> 'b * 'a = <fun>*)

(*EXPRESIÓN 16*)
let p = 1, 2;;
(*val p : int * int = (1, 2)*)

(*EXPRESIÓN 17*)
f p;;
(*- : int = 4*)

(*EXPRESIÓN 18*)
let p = swap p in f p;;
(*- : int = 5*)

(*EXPRESIÓN 19*)
f p;;
(*- : int = 4*)