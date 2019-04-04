FROM archlinux/base

RUN pacman --noconfirm -Sy base-devel git cmake wget vim desktop-file-utils cairo fuse boost qt5-base qt5-xmlpatterns attica libappimage

RUN useradd build
RUN mkdir /build && chmod a+rwx /build

USER build
WORKDIR /build
VOLUME  ["/sources", "/build"]
