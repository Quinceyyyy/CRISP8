{
  description = "Flake for CRISP8";

  inputs = { nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05"; };

  outputs = { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system} = {

        CRISP8 = pkgs.mkShell {
          packages = with pkgs; [
            clang-tools
            gnumake
            pkg-config
            bear
            gdb
            gcc
            inetutils
          ];

          shellHook = ''
            echo "Using C devShell for CRISP8"
            exec zsh -i
          '';
        };
    };
