let
  crossSystem = {
    config = "i686-w64-mingw32";
    arch = "x86";
    libc = "msvcrt";
    platform = {};
    openssl.system = "mingw";
  };
in import <nixpkgs> {
  inherit crossSystem;
  config = {
    packageOverrides = super: let self = super.pkgs; in {
      petool = self.callPackage (self.fetchFromGitHub {
        owner = "cnc-patch";
        repo = "petool";
        rev = "a7754d0362ff3a8600c859a2dc4c3a9d92bbe586";
        sha256 = "0qlp79gpn6xfngdmqrpcb8xdv25ady3ww545v7jhcff98g16znjh";
      }) {};
      mkCncGame = self.callPackage ./template.nix {};
    };
  };
}
