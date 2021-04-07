open TestFramework;
open FreeEffects;

let id = x => x;

module StateEffect' = {
  type t('a) = [ `Get(string => 'a) | `Set(string, unit => 'a) ];

  let map = fn => fun
    | `Get(next) => `Get(e => e->next->fn)
    | `Set(v, next) => `Set(v, e => e->next->fn)
};

module State = {
  module Effect = Make(StateEffect');
  let get = Effect.liftEff(`Get(id));
  let set = value => Effect.liftEff(`Set(value, id));
  let run = Effect.interpreter;
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

describe("FreeEffects#State", ({ test }) => {
  test("should return a pure value", ({ expect }) => {
    let eff = State.Effect.pure("heklo");
    let result = eff |> State.run(createStateHandler("foobar"));
    expect.string(result).toEqual("heklo");
  });

  test("should get the current state", ({ expect }) => {
    let eff = State.get;
    let result = eff |> State.run(createStateHandler("googoo"));
    expect.string(result).toEqual("googoo");
  });

  test("should chain effects together", ({ expect }) => {
    open State.Effect;
    let eff = State.get
        |> flatMap(state => State.set(state ++ ", world!"))
        |> flatMap(_ => State.get);

    let result = eff |> State.run(createStateHandler("Hello"));
    expect.string(result).toEqual("Hello, world!");
  });
});
