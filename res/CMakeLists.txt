install(FILES appimage_logo.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps COMPONENT APPIMAGE_INSTALLER)
install(FILES org.appimage.user-tool.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications COMPONENT APPIMAGE_INSTALLER)

# Sources settings
install(FILES appimage-providers.yaml DESTINATION /etc COMPONENT APPIMAGE_INSTALLER)
configure_file("appimage-providers.yaml" "${PROJECT_BINARY_DIR}/appimage-providers.yaml" COPYONLY)
