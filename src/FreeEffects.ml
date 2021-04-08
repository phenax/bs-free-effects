open FreeEffects__Utils;;

module type EFFECT = sig
  type ('a) t
  val map : ('a -> 'b) -> ('a) t -> ('b) t
end

module Make(Eff: EFFECT) = struct
  type ('a, 'ret) t = 
    | Pure of 'ret
    | Free of ((('a, 'ret) t) Eff.t)

  let pure x = Pure x

  let liftF fa = Free (Eff.map pure fa)

  let retract = function
    | Pure _ -> None
    | Free m -> Some(m)

  let hoist fn = function
    | Pure x -> Pure x
    | Free m -> Free (fn m)

  (* map : ('a -> 'b) -> 'a t -> 'b t *)

  (* join : ('a t) t -> 'a t *)

  (* Async helpers (promise) *)
  (* Use bastet *)
  (* Infix operators *)

  let rec flatMap fn = function
    | Pure a -> fn a
    | Free m -> Free (Eff.map (flatMap fn) m)

  let seq ef = flatMap (const ef)

  let rec fold fn = function
    | Pure x -> x
    | Free m -> fold fn (fn m)
end

(*module Compose(A: EFFECT)(B: EFFECT) = struct*)
(*type ('a) t = [ ('a) A.t | ('a) B.t ]*)

(*let map fn eff = match eff with*)
(*| #A.t as a -> A.map fn a*)
(*| #B.t as b -> B.map fn b*)
(*end*)

