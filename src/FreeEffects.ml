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

  let rec flatMap fn eff = match eff with
    | Pure a -> fn a
    | Free m -> Free (Eff.map (flatMap fn) m)

  let rec interpreter handler eff = match eff with
    | Pure x -> x
    | Free m -> interpreter handler (handler m)
end

