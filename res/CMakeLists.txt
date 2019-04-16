install(FILES appimage_logo.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps COMPONENT APPIMAGE_USER_TOOL)
install(FILES org.appimage.user-tool.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications COMPONENT APPIMAGE_USER_TOOL)
install(FILES org.appimage.user-tool-settings.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications COMPONENT APPIMAGE_USER_TOOL_SETTINGS)

# Sources settings
install(FILES appimage-providers.yaml DESTINATION ${CMAKE_INSTALL_SYSCONFDIR} COMPONENT APPIMAGE_USER_TOOL)
configure_file("appimage-providers.yaml" "${PROJECT_BINARY_DIR}/appimage-providers.yaml" COPYONLY)
