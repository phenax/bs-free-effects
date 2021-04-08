open TestFramework;
open TestUtils;
open State.Effect;

let getTagName = [%raw "x => x.NAME"];

describe("#flatMap", ({ test }) => {
  test("should flatten return with ops", ({ expect }) => {
    let eff = return("heklo")
      |> flatMap(State.set)
      |> flatMap(_ => State.get);
    let result = eff |> State.run(createStateHandler("foobar"));
    expect.string(result).toEqual("heklo");
  });

  test("should any two operation effects", ({ expect }) => {
    let eff = State.set("fuck")
      |> flatMap(_ => State.get);
    let result = eff |> State.run(createStateHandler("foobar"));
    expect.string(result).toEqual("fuck");
  });
});

describe("#seq", ({ test }) => {
  test("should chain return with State.get", ({ expect }) => {
    let eff = return("heklo") |> seq(State.get);
    let result = eff |> State.run(createStateHandler("foobar"));
    expect.string(result).toEqual("foobar");
  });

  test("should any two operation effects", ({ expect }) => {
    let eff = State.set("fuck") |> seq(State.get);
    let result = eff |> State.run(createStateHandler("foobar"));
    expect.string(result).toEqual("fuck");
  });
});

describe("#retract", ({ test }) => {
  test("should retract effect operation", ({ expect }) => {
    let op = State.get |> retract;
    expect.bool(op |> Belt.Option.isSome).toBe(true);
    expect.string(op |> getTagName).toEqual("Get");
  });

  test("should not retract return operation", ({ expect }) => {
    let op = return("foobar") |> retract;
    expect.bool(op |> Belt.Option.isNone).toBe(true);
  });
});

describe("#hoist", ({ test }) => {
  let switcheroo = fun
    | `Get(next) => `Set("haha", () => next("haha"))
    | `Set(_, next) => `Get(_ => next(()));

  test("should map over the operation", ({ expect }) => {
    let eff = State.get |> hoist(switcheroo);
    expect.string(eff |> retract |> getTagName).toEqual("Set");
  });

  test("should not affect return effect", ({ expect }) => {
    let eff = return("foobar") |> hoist(switcheroo);
    expect.bool(eff |> retract |> Belt.Option.isNone).toBe(true);
  });
});

