{}:
with import <nixpkgs> {};
let
  ocamlDeps = with ocamlPackages; with nodePackages; [
    ocaml
    opam
    merlin
    ocp-indent
    ocaml-language-server
  ];
  systemDeps = with pkgs; [
    nodejs-15_x
    yarn
    docker-compose
  ];
in
stdenv.mkDerivation {
  name = "bs-optics";
  buildInputs = systemDeps ++ ocamlDeps;
}
