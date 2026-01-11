# Mile.Uefi Release Notes

**Mile.Uefi 1.0.1729.0**

- Add UEFI PI definitions.
- Provide UEFI Specification definitions based on [EDK II 202511 Release]
- Update Mile.Project.Configurations to 1.0.1827.

[EDK II 202511 Release]: https://github.com/tianocore/edk2/tree/edk2-stable202511

**Mile.Uefi 1.0.1484.0**

- Disable actually unavailable runtime security checks in the bare-metal to
  reduce the compiler and linker warnings.
- Remove the specific optimizations to follow the related settings in
  Mile.Project.Configurations.
- Add MileUefiDisableUefiHeaders feature support.
- Add MileUefiDisableUefiGuidSources feature support.
- Update Mile.Project.Configurations to 1.0.1622.

**Mile.Uefi 1.0.1438.0**

- Initial release.
- Provide UEFI Specification definitions based on [EDK II 202502 Release].
- Provide NuGet package for Visual Studio out-of-box development experience.
- Provide x64 and ARM64 target support.

[EDK II 202502 Release]: https://github.com/tianocore/edk2/tree/edk2-stable202502
