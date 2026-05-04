# Mile.Uefi

[![NuGet Package](https://img.shields.io/nuget/vpre/Mile.Uefi)](https://www.nuget.org/packages/Mile.Uefi)

The lightweight UEFI toolchain for Visual Studio (MSBuild) C++ projects.

## Features

- Provide UEFI Specification definitions based on 
  [EDK II commit c00c1216ce7f6e02b00da95747a7f4d4c239c1b1 after 202602 Release].
- Provide NuGet package for Visual Studio out-of-box development experience.
- Provide x64 and ARM64 target support.

[EDK II commit c00c1216ce7f6e02b00da95747a7f4d4c239c1b1 after 202602 Release]: https://github.com/tianocore/edk2/tree/c00c1216ce7f6e02b00da95747a7f4d4c239c1b1

## Available MSBuild project options

If you don't want to use the UEFI header files, please set the following option.

```
<MileUefiDisableUefiHeaders>true</MileUefiDisableUefiHeaders>
```

If you don't want to use the UEFI GUID sources, please set the following option.

```
<MileUefiDisableUefiGuidSources>true</MileUefiDisableUefiGuidSources>
```

## Example

- https://github.com/MobilityOS/Mobility

## Documents

- [License](License.md)
- [Release Notes](ReleaseNotes.md)
- [Versioning](Versioning.md)
