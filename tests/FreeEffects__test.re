open TestFramework;
open TestUtils;

describe("FreeEffects#State", ({ test }) => {
  test("should return a return value", ({ expect }) => {
    let eff = State.Effect.return("heklo");
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

  test("should allow return as the first part of the chain", ({ expect }) => {
    open State.Effect;
    let eff = return(())
      |> flatMap(_ => State.get)
      |> flatMap(state => State.set(state ++ ", world!"))
      |> flatMap(_ => State.get);

    let result = eff |> State.run(createStateHandler("Hello"));
    expect.string(result).toEqual("Hello, world!");
  });
});

