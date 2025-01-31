{ pkgs ? import <nixpkgs> {} }:

let
        opencvVersion = "4.11.0";
        
        opencvSrc = pkgs.fetchFromGitHub {
                owner   = "opencv";
                repo    = "opencv";
                rev     = opencvVersion;
                sha256  = "0hyc28whzf5hlqd75cj6rv6h44saafaqqvivlhdyazhc185kh9d2";
        };
        # opencvContribSrc = pkgs.fetchFromGitHub {
        #         owner       = "opencv";
        #         repo        = "opencv_contrib";
        #         rev         = opencvVersion;
        #         sha256      = "0wsvd7pnj0p6dvdl4x4r46dkrkxkd61v411ih30j3wa9l7m7vmv0";
        # };
        opencv = pkgs.stdenv.mkDerivation {
                pname = "opencv";
                version = opencvVersion;
                src = opencvSrc;

                nativeBuildInputs = [
                        pkgs.cmake
                        pkgs.pkg-config
                        pkgs.gcc
                ];

                buildInputs = [
                        pkgs.gtk3
                ];

                cmakeFlags = [
                        # "-DOPENCV_EXTRA_MODULES_PATH=${opencvContribSrc}/modules"
                        "-DBUILD_EXAMPLES=OFF"
                        "-DWITH_GTK=ON"
                        "-DWITH_QT=OFF"
                        "-DWITH_CUDA=OFF"
                        "-DBUILD_opencv_python2=OFF"
                        "-DBUILD_opencv_python3=OFF"
                ];

                enableParallelBuilding = true;
        };
in

pkgs.mkShell {
        buildInputs = [
                opencv
        ];

        shellHook = ''
                export PKG_CONFIG_PATH=${opencv}/lib/pkgconfig:$PKG_CONFIG_PATH
                echo ${opencv}
                echo "OpenCV version: $(${opencv}/bin/opencv_version)"
                echo "GCC version: $(gcc --version | head -n 1)"
                echo "CMake version: $(cmake --version | head -n 1)"
        '';
}
