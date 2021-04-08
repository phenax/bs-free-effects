open TestFramework;
open TestUtils;
open State.Effect;

let getTagName = [%raw "x => x.NAME"];

describe("#flatMap", ({ test }) => {
  test("should flatten pure with State.get", ({ expect }) => {
    let eff = pure("heklo") |> flatMap(_ => State.get);
    let result = eff |> State.run(createStateHandler("foobar"));
    expect.string(result).toEqual("foobar");
  });
});

describe("#retract", ({ test }) => {
  test("should retract effect operation", ({ expect }) => {
    let op = State.get |> retract;
    expect.bool(op |> Belt.Option.isSome).toBe(true);
    expect.string(op |> getTagName).toEqual("Get");
  });

  test("should not retract pure operation", ({ expect }) => {
    let op = pure("foobar") |> retract;
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

  test("should not affect pure effect", ({ expect }) => {
    let eff = pure("foobar") |> hoist(switcheroo);
    expect.bool(eff |> retract |> Belt.Option.isNone).toBe(true);
  });
});

