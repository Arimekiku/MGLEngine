{
  description = "C++ render engine.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

    utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      ...
    }@inputs:
    inputs.utils.lib.eachSystem
      [
        # Add the system/architecture you would like to support here. Note that not
        # all packages in the official nixpkgs support all platforms.
        "x86_64-linux"
      ]
      (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
          };
          ld-library-path =
            with pkgs;
            lib.makeLibraryPath [
              libGL
              libxkbcommon
              wayland
            ];
        in
        {
          devShells.default = pkgs.mkShell.override { stdenv = pkgs.llvmPackages_14.stdenv; } {
            name = "RenderingEngineShell";

            packages = with pkgs; [
              llvmPackages_14.clang-tools
              cmake
              nixfmt-rfc-style
            ];

            inputsFrom = [ self.packages.${system}.default ];

            # Setting up the environment variables you need during
            # development.
            shellHook = ''
              export CXX=${pkgs.llvmPackages_14.clang}/bin/clang++
              export LD_LIBRARY_PATH="${ld-library-path}"
            '';
          };

          packages.default = pkgs.llvmPackages_14.stdenv.mkDerivation rec {
            pname = "RenderingEngine";
            version = "0.1.0";

            src = ./.;

            nativeBuildInputs = with pkgs; [
              cmake
              wayland-scanner
              pkg-config
              zlib
            ];

            buildInputs = with pkgs; [
              xorg.libX11
              xorg.libXrandr
              xorg.libXinerama
              xorg.libXcursor
              xorg.libXi
              xorg.libXext
              wayland
              wayland-protocols
              libxkbcommon
              libGL
            ];

            installPhase = ''
              runHook preInstall

              mkdir -p $out/bin
              cp RenderingEngine $out/bin
              cp -r Resources $out/bin

              runHook postInstall
            '';
          };

          formatter = pkgs.nixfmt-rfc-style;
        }
      );

}
