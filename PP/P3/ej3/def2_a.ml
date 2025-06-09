(*APARTADO 1*)
let p radio = let pi = 2. *. asin 1. in if radio >= 0. then radio *. pi *. 2. else 0.;;

(*APARTADO 2*)
let area radio = let pi = 2. *. asin 1. in if radio >= 0. then pi *. radio**2. else 0.;;

(*APARTADO 3*)
let absf n = if n < 0. then (-.n) else n;;

(*APARTADO 4*)
let even n = n mod 2 = 0;;

(*APARTADO 5*)
let next3 n = (n + 2) / 3 * 3;;

(*APARTADO 6*)
let is_a_letter n = (int_of_char n >= int_of_char 'a' && int_of_char n <= int_of_char 'z') || (int_of_char n >= int_of_char 'A' && int_of_char n <= int_of_char 'Z');;

(*APARTADO 7*)
let string_of_bool b = if b then "verdadero" else "false";;