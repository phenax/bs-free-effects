module type EFFECT = sig
  type ('a) t
  val map : ('a -> 'b) -> ('a) t -> ('b) t
end

module Make(Eff: EFFECT) = struct
  type ('a, 'ret) t = 
    | Pure of 'ret
    | Free of ((('a, 'ret) t) Eff.t)

  let pure x = Pure x

  let liftEff fa = Free (Eff.map pure fa)

  let rec flatMap fn = function
    | Pure a -> fn a
    | Free m -> Free (Eff.map (flatMap fn) m)

  let rec interpreter handler = function
    | Pure x -> x
    | Free m -> interpreter handler (handler m)
end

(*let (<<) f g x = f (g x)*)

(*module Compose(A: EFFECT)(B: EFFECT) = struct*)
  (*type ('a) t = [ ('a) A.t | ('a) B.t ]*)

  (*let map fn eff = match eff with*)
    (*| #A.t as a -> A.map fn a*)
    (*| #B.t as b -> B.map fn b*)
(*end*)

