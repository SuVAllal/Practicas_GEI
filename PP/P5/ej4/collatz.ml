let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1;;


let rec orbit n =
	if n <= 1 then "1"
	else string_of_int n ^ ", " ^ orbit (f n);;


let length n =
	let rec length_contador n cnt =
		if n <= 1 then cnt
		else length_contador (f n) (cnt + 1)
	in length_contador n 0;;


let top n =
	let rec top_contador n maximo =
		if n <= 1 then maximo
		else (top_contador (f n) (max n maximo))
	in top_contador n 0;;


let rec length'n'top n =
	if n <= 1 then (0,n)
	else let l, t = length'n'top (f n)
	in (l + 1, max n t);;


let rec longest_in m n =
	let rec aux_longest num2 =
		if num2 <= m then (m, length m)
		else let (num1, l1) = aux_longest (num2 - 1) in
			let l2 = length num2 in
			if l1 >= l2 then (num1, l1) else (num2, l2)
	in aux_longest n;;


let rec highest_in m n =
	let rec aux_highest num2 =
		if num2 <= m then (m, top m)
		else let (num1, t1) = aux_highest (num2 - 1) in
			let t2 = top num2 in
			if t1 >= t2 then (num1, t1) else (num2, t2)
	in aux_highest n;;