# Nix build recipe
# Copyright (C) 2023  Nguyễn Gia Phong
#
# This file is part of MSV.
#
# MSV is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# MSV is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with MSV.  If not, see <http://www.gnu.org/licenses/>.

{ lib, llvmPackages_11, autoreconfHook, makeWrapper, which
, cjson, explain, ncurses, python3 }:
let inherit (llvmPackages_11) clang libclang llvm stdenv;
in stdenv.mkDerivation rec {
  pname = "msv";
  version = "0.3";

  src = ./.;
  patches = [ ./llvm-cl-opt.patch ];
  postPatch = ''
    # When does the compiler building target programs not find the same paths?
    substituteInPlace configure.ac\
      --replace '`$srcdir/tools/test_clang_path.py`' '" "'
  '';

  enableParallelBuilding = true;
  nativeBuildInputs = [ autoreconfHook makeWrapper which ];
  buildInputs = [ cjson explain.dev libclang llvm ncurses ];
  propagatedBuildInputs = [ clang python3 ]; # for msv-*cc

  postInstall = ''
    wrapProgram $out/bin/msv-profilecc --set LD_LIBRARY_PATH $out/lib
  '';

  meta = with lib; {
    description = "Meta something ???";
    homepage = "https://github.com/Suresoft-GLaDOS/MSV";
    license = licenses.gpl3Plus;
    platforms = platforms.unix;
  };
}
