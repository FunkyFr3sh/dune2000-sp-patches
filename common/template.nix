{ lib, stdenv, nasm, petool }:

{ name, version, homepage }:

stdenv.mkDerivation {
  inherit name;
  inherit version;

  nativeBuildInputs = [ nasm petool ];
  src = ./..;
  preBuild = "makeFlagsArray=(" + lib.concatStringsSep " " [
    # Miscellaneous
    "REV=${version}"
    "CP=cp"
    # GNU Compiler Collection
    "CC=${stdenv.cross.config}-gcc"
    "CXX=${stdenv.cross.config}-g++"
    # GNU Binutils
    "AS=${stdenv.cross.config}-as"
    "WINDRES=${stdenv.cross.config}-windres"
    "LD=${stdenv.cross.config}-ld"
  ] + ")";

  enableParallelBuilding = true;

  meta = with stdenv.lib; {
    inherit homepage;
    description = "";
    # maintainers =
    license = map (builtins.getAttr "shortName") [ licenses.mit ];
    # Buildable on any platform, runable only on Windows
    platforms = stdenv.lib.platforms.all;
  };
}
