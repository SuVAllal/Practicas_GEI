(* IMPLEMENTACIÓN COLA DE PRIORIDAD MEDIANTE LISTAS *)

let insert (n, c) l = 
  let rec aux_insert aux = function
    [] -> List.rev ((n,c)::aux)
    | (a, b)::t -> if c <= b then List.rev_append aux ((n,c)::(a,b)::t)
          else aux_insert ((a,b)::aux) t
  in aux_insert [] l;;


let remove_min = function 
  [] -> None
  | (a,b)::t -> Some ((a, b), t);;


(* ALGORITMO DE DIJKSTRA *)
let dijkstra w =
  let n = Array.length w in (* conseguimos el tamaño del array *)

  (* verificamos que la matriz es cuadrada y que no contiene valores negativos *)
  if not (Array.for_all (fun row -> Array.length row = n) w) 
    || not (Array.for_all (fun row -> Array.for_all (fun x -> 
        match x with
        | None -> true
        | Some v -> v >= 0
        ) row) w) 
  then raise (Invalid_argument "dijkstra");

  let rec aux q visited costs =
    match remove_min q with
    | None -> costs
    | Some ((nodo, c), new_q) -> (* conseguimos el nodo origen *)
      if visited.(nodo) then aux new_q visited costs else (
      visited.(nodo) <- true;
      costs.(nodo) <- Some c;
      (* Actualizar los costos y añadir los nodos vecinos a la cola de prioridad *)
      let updated_q =
        List.fold_left (fun acc (j, cost_nodo_j) ->
            if not visited.(j) then (* si el nodo que estamos mirando no esta visitado *)
              match cost_nodo_j with
              | Some weight -> ( (* si su arista tiene peso *)
                let new_cost = c + weight in (* calculamos el nuevo peso *)
                match costs.(j) with
                  | None -> (costs.(j) <- Some new_cost;
                    insert (j, new_cost) acc) (* insertamos el nuevo peso en la lista *)
                  | Some cost_j -> if new_cost < cost_j then (* si el peso es menor que el anterior a su posicion en la lista *)
                    (costs.(j) <- Some new_cost;
                    insert (j, new_cost) acc) (* lo guardamos *)
                    else
                      acc) (* sino devolvemos la lista sin cambios *)
              | None -> acc
            else acc
          )
          new_q
          (List.init n (fun i -> (i, w.(nodo).(i))))
      in
      aux updated_q visited costs)
  in

  Array.init n (fun src -> (* inicializaciones *)
    let initial_queue = insert (src, 0) [] in
    let initial_visited = Array.make n false in
    let costs = Array.init n (fun _ -> None) in 

    aux initial_queue initial_visited costs
  )
;;
