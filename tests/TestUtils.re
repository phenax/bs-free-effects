open FreeEffects;
open FreeEffects__Utils;

module StateEffect' = {
  type t('a) = [ `Get(string => 'a) | `Set(string, unit => 'a) ];

  let map = fn => fun
    | `Get(next) => `Get(e => e->next->fn)
    | `Set(v, next) => `Set(v, e => e->next->fn)
};

module State = {
  module Effect = Make(StateEffect');
  let run = Effect.fold;
  let get = Effect.liftF(`Get(id));
  let set = value => Effect.liftF(`Set(value, id));
};

let createStateHandler = (initialState) => {
  let state = ref(initialState);

  fun
  | `Get(resume) => resume(state.contents)
  | `Set(value, resume) => {
    state.contents = value;
    resume(());
  }
};

