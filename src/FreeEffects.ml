open FreeEffects__Utils;;

module type EFFECT = sig
  type 'a t
  val map : ('a -> 'b) -> 'a t -> 'b t
end

module Make: functor(Eff: EFFECT) -> sig
  type 'a t

  val pure : 'a -> 'a t
  val return : 'a -> 'a t

  (* Lift an operation into an effect *)
  val liftF : 'a Eff.t -> 'a t

  (* The opposite of liftF *)
  val retract : 'a t -> 'a t Eff.t option

  (* map over an effect to change the operation inside *)
  val hoist : ('a t Eff.t -> 'a t Eff.t) -> 'a t -> 'a t

  (* Run two effects in sequence *)
  val flatMap : ('a -> 'b t) -> 'a t -> 'b t

  (* Run two effects in sequence ignoring the result of the first one *)
  val seq : 'a t -> 'b t -> 'a t

  (* Alias to seq *)
  val then_ : 'a t -> 'b t -> 'a t

  (* Fold the sequence of effects into a result using a handler function *)
  val fold : ('a t Eff.t -> 'a t) -> 'a t -> 'a

end = functor(Eff: EFFECT) -> struct
  type 'a t = 
    | Pure of 'a
    | Free of 'a t Eff.t
  [@@bs.deriving accessors]

  let return = pure

  let liftF fa = Free (Eff.map pure fa)

  let retract = function
    | Pure _ -> None
    | Free m -> Some(m)

  let hoist fn = function
    | Pure x -> Pure x
    | Free m -> Free (fn m)

  let rec flatMap fn = function
    | Pure a -> fn a
    | Free m -> Free (Eff.map (flatMap fn) m)

  let seq ef = flatMap (const ef)
  let then_ = seq

  let rec fold fn = function
    | Pure x -> x
    | Free m -> fn m |> fold fn
end

(*module Compose(A: EFFECT)(B: EFFECT) = struct*)
(*type ('a) t = [ ('a) A.t | ('a) B.t ]*)

(*let map fn eff = match eff with*)
(*| #A.t as a -> A.map fn a*)
(*| #B.t as b -> B.map fn b*)
(*end*)

