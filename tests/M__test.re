open TestFramework;

describe("Hello", ({ test }) => {
  test("should", ({ expect }) => {
    expect.int(1).toBe(1);
  });
});

