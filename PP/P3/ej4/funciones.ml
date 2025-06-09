(*APARTADO 1*)
(*sumto: int -> int, de modo que sumto n devuelva la suma de todos los naturales hasta el n (incluido)*)
let rec sumto = function 0 -> 0 | n -> n + sumto (n-1);;


(*APARTADO 2*)
(*exp10: int -> int, de modo que, para cualquier n >= 0, exp10 devuelva el valor de 10^n*)
let rec exp10 = function 0 -> 1 | n ->  10 * exp10(n-1);;


(*APARTADO 3*)
(*num_cifras: int -> int, de modo que num_cifras n devuelva el numero de cifras de la representación decimal de n (el signo no cuenta como cifra)*)
let rec num_cifras n = if (n < 10) && (n >= 0) then 1 else (if n < 0 then num_cifras(-n) else 1 + num_cifras(n/10));;


(*APARTADO 4*)
(*sum_cifras: int -> int, de modo que, sum_cifras n devuelva la suma de las cifras correspondientes a la representación decimal de n*)
let rec sum_cifras n = if (n < 10) && (n >= 0) then n else (if n < 0 then sum_cifras(-n) else n mod 10 + sum_cifras(n/10));;
