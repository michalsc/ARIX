install(FILES vmlinuz-${CMAKE_SYSTEM_PROCESSOR} DESTINATION ./ RENAME vmlinuz)
install(FILES arix_base_cpio-${CMAKE_SYSTEM_PROCESSOR}.img.gz DESTINATION ./ RENAME initrd)
