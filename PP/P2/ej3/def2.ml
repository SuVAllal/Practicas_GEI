(*APARTADO 1*)
let p = let pi = 2. *. asin 1. in function (radio: float) -> radio *. pi *. 2.;;

(*APARTADO 2*)
let area = let pi = 2. *. asin 1. in function (radio: float) -> pi *. radio *. radio;;

(*APARTADO 3*)
let absf = function (n: float) -> if n < 0. then (-.n) else n;;

(*APARTADO 4*)
let even = function (n: int) -> n mod 2 = 0;;

(*APARTADO 5*)
let next3 = function (n: int) -> (n + 2) / 3 * 3;;

(*APARTADO 6*)
let is_a_letter = function (n: char) -> (int_of_char n >= int_of_char 'a' && int_of_char n <= int_of_char 'z') || (int_of_char n >= int_of_char 'A' && int_of_char n <= int_of_char 'Z');;

(*APARTADO 7*)
let string_of_bool = function (b: bool) -> if b then "verdadero" else "false";;