/** @file
  Provides string functions, linked list functions, math functions, synchronization
  functions, file path functions, and CPU architecture-specific functions.

Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
Portions copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
Copyright (c) Microsoft Corporation.<BR>
Portions Copyright (c) 2020, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __BASE_LIB__
#define __BASE_LIB__

//
// Definitions for architecture-specific types
//
#if   defined (MDE_CPU_IA32)
///
/// The IA-32 architecture context buffer used by SetJump() and LongJump().
///
typedef struct {
  UINT32                            Ebx;
  UINT32                            Esi;
  UINT32                            Edi;
  UINT32                            Ebp;
  UINT32                            Esp;
  UINT32                            Eip;
  UINT32                            Ssp;
} BASE_LIBRARY_JUMP_BUFFER;

#define BASE_LIBRARY_JUMP_BUFFER_ALIGNMENT 4

#endif // defined (MDE_CPU_IA32)

#if defined (MDE_CPU_X64)
///
/// The x64 architecture context buffer used by SetJump() and LongJump().
///
typedef struct {
  UINT64                            Rbx;
  UINT64                            Rsp;
  UINT64                            Rbp;
  UINT64                            Rdi;
  UINT64                            Rsi;
  UINT64                            R12;
  UINT64                            R13;
  UINT64                            R14;
  UINT64                            R15;
  UINT64                            Rip;
  UINT64                            MxCsr;
  UINT8                             XmmBuffer[160]; ///< XMM6-XMM15.
  UINT64                            Ssp;
} BASE_LIBRARY_JUMP_BUFFER;

#define BASE_LIBRARY_JUMP_BUFFER_ALIGNMENT 8

#endif // defined (MDE_CPU_X64)

#if defined (MDE_CPU_EBC)
///
/// The EBC context buffer used by SetJump() and LongJump().
///
typedef struct {
  UINT64                            R0;
  UINT64                            R1;
  UINT64                            R2;
  UINT64                            R3;
  UINT64                            IP;
} BASE_LIBRARY_JUMP_BUFFER;

#define BASE_LIBRARY_JUMP_BUFFER_ALIGNMENT 8

#endif // defined (MDE_CPU_EBC)

#if defined (MDE_CPU_ARM)

typedef struct {
  UINT32    R3;  ///< A copy of R13.
  UINT32    R4;
  UINT32    R5;
  UINT32    R6;
  UINT32    R7;
  UINT32    R8;
  UINT32    R9;
  UINT32    R10;
  UINT32    R11;
  UINT32    R12;
  UINT32    R14;
} BASE_LIBRARY_JUMP_BUFFER;

#define BASE_LIBRARY_JUMP_BUFFER_ALIGNMENT 4

#endif  // defined (MDE_CPU_ARM)

#if defined (MDE_CPU_AARCH64)
typedef struct {
  // GP regs
  UINT64    X19;
  UINT64    X20;
  UINT64    X21;
  UINT64    X22;
  UINT64    X23;
  UINT64    X24;
  UINT64    X25;
  UINT64    X26;
  UINT64    X27;
  UINT64    X28;
  UINT64    FP;
  UINT64    LR;
  UINT64    IP0;

  // FP regs
  UINT64    D8;
  UINT64    D9;
  UINT64    D10;
  UINT64    D11;
  UINT64    D12;
  UINT64    D13;
  UINT64    D14;
  UINT64    D15;
} BASE_LIBRARY_JUMP_BUFFER;

#define BASE_LIBRARY_JUMP_BUFFER_ALIGNMENT 8

#endif  // defined (MDE_CPU_AARCH64)

#if defined (MDE_CPU_RISCV64)
///
/// The RISC-V architecture context buffer used by SetJump() and LongJump().
///
typedef struct {
  UINT64                            RA;
  UINT64                            S0;
  UINT64                            S1;
  UINT64                            S2;
  UINT64                            S3;
  UINT64                            S4;
  UINT64                            S5;
  UINT64                            S6;
  UINT64                            S7;
  UINT64                            S8;
  UINT64                            S9;
  UINT64                            S10;
  UINT64                            S11;
  UINT64                            SP;
} BASE_LIBRARY_JUMP_BUFFER;

#define BASE_LIBRARY_JUMP_BUFFER_ALIGNMENT 8

#endif // defined (MDE_CPU_RISCV64)

//
// Linked List Functions and Macros
//

/**
  Initializes the head node of a doubly linked list that is declared as a
  global variable in a module.

  Initializes the forward and backward links of a new linked list. After
  initializing a linked list with this macro, the other linked list functions
  may be used to add and remove nodes from the linked list. This macro results
  in smaller executables by initializing the linked list in the data section,
  instead if calling the InitializeListHead() function to perform the
  equivalent operation.

  @param  ListHead  The head note of a list to initialize.

**/
#define INITIALIZE_LIST_HEAD_VARIABLE(ListHead)  {&(ListHead), &(ListHead)}

/**
  Iterates over each node in a doubly linked list using each node's forward link.

  @param  Entry     A pointer to a list node used as a loop cursor during iteration
  @param  ListHead  The head node of the doubly linked list

**/
#define BASE_LIST_FOR_EACH(Entry, ListHead)    \
  for(Entry = (ListHead)->ForwardLink; Entry != (ListHead); Entry = Entry->ForwardLink)

/**
  Iterates over each node in a doubly linked list using each node's forward link
  with safety against node removal.

  This macro uses NextEntry to temporarily store the next list node so the node
  pointed to by Entry may be deleted in the current loop iteration step and
  iteration can continue from the node pointed to by NextEntry.

  @param  Entry     A pointer to a list node used as a loop cursor during iteration
  @param  NextEntry A pointer to a list node used to temporarily store the next node
  @param  ListHead  The head node of the doubly linked list

**/
#define BASE_LIST_FOR_EACH_SAFE(Entry, NextEntry, ListHead)            \
  for(Entry = (ListHead)->ForwardLink, NextEntry = Entry->ForwardLink;\
      Entry != (ListHead); Entry = NextEntry, NextEntry = Entry->ForwardLink)

//
// Base Library CPU Functions
//

#if defined (MDE_CPU_IA32) || defined (MDE_CPU_X64)
///
/// IA32 and x64 Specific Functions.
/// Byte packed structure for 16-bit Real Mode EFLAGS.
///
typedef union {
  struct {
    UINT32  CF:1;           ///< Carry Flag.
    UINT32  Reserved_0:1;   ///< Reserved.
    UINT32  PF:1;           ///< Parity Flag.
    UINT32  Reserved_1:1;   ///< Reserved.
    UINT32  AF:1;           ///< Auxiliary Carry Flag.
    UINT32  Reserved_2:1;   ///< Reserved.
    UINT32  ZF:1;           ///< Zero Flag.
    UINT32  SF:1;           ///< Sign Flag.
    UINT32  TF:1;           ///< Trap Flag.
    UINT32  IF:1;           ///< Interrupt Enable Flag.
    UINT32  DF:1;           ///< Direction Flag.
    UINT32  OF:1;           ///< Overflow Flag.
    UINT32  IOPL:2;         ///< I/O Privilege Level.
    UINT32  NT:1;           ///< Nested Task.
    UINT32  Reserved_3:1;   ///< Reserved.
  } Bits;
  UINT16    Uint16;
} IA32_FLAGS16;

///
/// Byte packed structure for EFLAGS/RFLAGS.
/// 32-bits on IA-32.
/// 64-bits on x64.  The upper 32-bits on x64 are reserved.
///
typedef union {
  struct {
    UINT32  CF:1;           ///< Carry Flag.
    UINT32  Reserved_0:1;   ///< Reserved.
    UINT32  PF:1;           ///< Parity Flag.
    UINT32  Reserved_1:1;   ///< Reserved.
    UINT32  AF:1;           ///< Auxiliary Carry Flag.
    UINT32  Reserved_2:1;   ///< Reserved.
    UINT32  ZF:1;           ///< Zero Flag.
    UINT32  SF:1;           ///< Sign Flag.
    UINT32  TF:1;           ///< Trap Flag.
    UINT32  IF:1;           ///< Interrupt Enable Flag.
    UINT32  DF:1;           ///< Direction Flag.
    UINT32  OF:1;           ///< Overflow Flag.
    UINT32  IOPL:2;         ///< I/O Privilege Level.
    UINT32  NT:1;           ///< Nested Task.
    UINT32  Reserved_3:1;   ///< Reserved.
    UINT32  RF:1;           ///< Resume Flag.
    UINT32  VM:1;           ///< Virtual 8086 Mode.
    UINT32  AC:1;           ///< Alignment Check.
    UINT32  VIF:1;          ///< Virtual Interrupt Flag.
    UINT32  VIP:1;          ///< Virtual Interrupt Pending.
    UINT32  ID:1;           ///< ID Flag.
    UINT32  Reserved_4:10;  ///< Reserved.
  } Bits;
  UINTN     UintN;
} IA32_EFLAGS32;

///
/// Byte packed structure for Control Register 0 (CR0).
/// 32-bits on IA-32.
/// 64-bits on x64.  The upper 32-bits on x64 are reserved.
///
typedef union {
  struct {
    UINT32  PE:1;           ///< Protection Enable.
    UINT32  MP:1;           ///< Monitor Coprocessor.
    UINT32  EM:1;           ///< Emulation.
    UINT32  TS:1;           ///< Task Switched.
    UINT32  ET:1;           ///< Extension Type.
    UINT32  NE:1;           ///< Numeric Error.
    UINT32  Reserved_0:10;  ///< Reserved.
    UINT32  WP:1;           ///< Write Protect.
    UINT32  Reserved_1:1;   ///< Reserved.
    UINT32  AM:1;           ///< Alignment Mask.
    UINT32  Reserved_2:10;  ///< Reserved.
    UINT32  NW:1;           ///< Mot Write-through.
    UINT32  CD:1;           ///< Cache Disable.
    UINT32  PG:1;           ///< Paging.
  } Bits;
  UINTN     UintN;
} IA32_CR0;

///
/// Byte packed structure for Control Register 4 (CR4).
/// 32-bits on IA-32.
/// 64-bits on x64.  The upper 32-bits on x64 are reserved.
///
typedef union {
  struct {
    UINT32  VME:1;          ///< Virtual-8086 Mode Extensions.
    UINT32  PVI:1;          ///< Protected-Mode Virtual Interrupts.
    UINT32  TSD:1;          ///< Time Stamp Disable.
    UINT32  DE:1;           ///< Debugging Extensions.
    UINT32  PSE:1;          ///< Page Size Extensions.
    UINT32  PAE:1;          ///< Physical Address Extension.
    UINT32  MCE:1;          ///< Machine Check Enable.
    UINT32  PGE:1;          ///< Page Global Enable.
    UINT32  PCE:1;          ///< Performance Monitoring Counter
                            ///< Enable.
    UINT32  OSFXSR:1;       ///< Operating System Support for
                            ///< FXSAVE and FXRSTOR instructions
    UINT32  OSXMMEXCPT:1;   ///< Operating System Support for
                            ///< Unmasked SIMD Floating Point
                            ///< Exceptions.
    UINT32  UMIP:1;         ///< User-Mode Instruction Prevention.
    UINT32  LA57:1;         ///< Linear Address 57bit.
    UINT32  VMXE:1;         ///< VMX Enable.
    UINT32  SMXE:1;         ///< SMX Enable.
    UINT32  Reserved_3:1;   ///< Reserved.
    UINT32  FSGSBASE:1;     ///< FSGSBASE Enable.
    UINT32  PCIDE:1;        ///< PCID Enable.
    UINT32  OSXSAVE:1;      ///< XSAVE and Processor Extended States Enable.
    UINT32  Reserved_4:1;   ///< Reserved.
    UINT32  SMEP:1;         ///< SMEP Enable.
    UINT32  SMAP:1;         ///< SMAP Enable.
    UINT32  PKE:1;          ///< Protection-Key Enable.
    UINT32  Reserved_5:9;   ///< Reserved.
  } Bits;
  UINTN     UintN;
} IA32_CR4;

///
/// Byte packed structure for a segment descriptor in a GDT/LDT.
///
typedef union {
  struct {
    UINT32  LimitLow:16;
    UINT32  BaseLow:16;
    UINT32  BaseMid:8;
    UINT32  Type:4;
    UINT32  S:1;
    UINT32  DPL:2;
    UINT32  P:1;
    UINT32  LimitHigh:4;
    UINT32  AVL:1;
    UINT32  L:1;
    UINT32  DB:1;
    UINT32  G:1;
    UINT32  BaseHigh:8;
  } Bits;
  UINT64  Uint64;
} IA32_SEGMENT_DESCRIPTOR;

///
/// Byte packed structure for an IDTR, GDTR, LDTR descriptor.
///
#pragma pack (1)
typedef struct {
  UINT16  Limit;
  UINTN   Base;
} IA32_DESCRIPTOR;
#pragma pack ()

#define IA32_IDT_GATE_TYPE_TASK          0x85
#define IA32_IDT_GATE_TYPE_INTERRUPT_16  0x86
#define IA32_IDT_GATE_TYPE_TRAP_16       0x87
#define IA32_IDT_GATE_TYPE_INTERRUPT_32  0x8E
#define IA32_IDT_GATE_TYPE_TRAP_32       0x8F

#define IA32_GDT_TYPE_TSS               0x9
#define IA32_GDT_ALIGNMENT              8

#if defined (MDE_CPU_IA32)
///
/// Byte packed structure for an IA-32 Interrupt Gate Descriptor.
///
typedef union {
  struct {
    UINT32  OffsetLow:16;   ///< Offset bits 15..0.
    UINT32  Selector:16;    ///< Selector.
    UINT32  Reserved_0:8;   ///< Reserved.
    UINT32  GateType:8;     ///< Gate Type.  See #defines above.
    UINT32  OffsetHigh:16;  ///< Offset bits 31..16.
  } Bits;
  UINT64  Uint64;
} IA32_IDT_GATE_DESCRIPTOR;

#pragma pack (1)
//
// IA32 Task-State Segment Definition
//
typedef struct {
  UINT16    PreviousTaskLink;
  UINT16    Reserved_2;
  UINT32    ESP0;
  UINT16    SS0;
  UINT16    Reserved_10;
  UINT32    ESP1;
  UINT16    SS1;
  UINT16    Reserved_18;
  UINT32    ESP2;
  UINT16    SS2;
  UINT16    Reserved_26;
  UINT32    CR3;
  UINT32    EIP;
  UINT32    EFLAGS;
  UINT32    EAX;
  UINT32    ECX;
  UINT32    EDX;
  UINT32    EBX;
  UINT32    ESP;
  UINT32    EBP;
  UINT32    ESI;
  UINT32    EDI;
  UINT16    ES;
  UINT16    Reserved_74;
  UINT16    CS;
  UINT16    Reserved_78;
  UINT16    SS;
  UINT16    Reserved_82;
  UINT16    DS;
  UINT16    Reserved_86;
  UINT16    FS;
  UINT16    Reserved_90;
  UINT16    GS;
  UINT16    Reserved_94;
  UINT16    LDTSegmentSelector;
  UINT16    Reserved_98;
  UINT16    T;
  UINT16    IOMapBaseAddress;
} IA32_TASK_STATE_SEGMENT;

typedef union {
  struct {
    UINT32  LimitLow:16;    ///< Segment Limit 15..00
    UINT32  BaseLow:16;     ///< Base Address  15..00
    UINT32  BaseMid:8;      ///< Base Address  23..16
    UINT32  Type:4;         ///< Type (1 0 B 1)
    UINT32  Reserved_43:1;  ///< 0
    UINT32  DPL:2;          ///< Descriptor Privilege Level
    UINT32  P:1;            ///< Segment Present
    UINT32  LimitHigh:4;    ///< Segment Limit 19..16
    UINT32  AVL:1;          ///< Available for use by system software
    UINT32  Reserved_52:2;  ///< 0 0
    UINT32  G:1;            ///< Granularity
    UINT32  BaseHigh:8;     ///< Base Address 31..24
  } Bits;
  UINT64  Uint64;
} IA32_TSS_DESCRIPTOR;
#pragma pack ()

#endif // defined (MDE_CPU_IA32)

#if defined (MDE_CPU_X64)
///
/// Byte packed structure for an x64 Interrupt Gate Descriptor.
///
typedef union {
  struct {
    UINT32  OffsetLow:16;   ///< Offset bits 15..0.
    UINT32  Selector:16;    ///< Selector.
    UINT32  Reserved_0:8;   ///< Reserved.
    UINT32  GateType:8;     ///< Gate Type.  See #defines above.
    UINT32  OffsetHigh:16;  ///< Offset bits 31..16.
    UINT32  OffsetUpper:32; ///< Offset bits 63..32.
    UINT32  Reserved_1:32;  ///< Reserved.
  } Bits;
  struct {
    UINT64  Uint64;
    UINT64  Uint64_1;
  } Uint128;
} IA32_IDT_GATE_DESCRIPTOR;

#pragma pack (1)
//
// IA32 Task-State Segment Definition
//
typedef struct {
  UINT32    Reserved_0;
  UINT64    RSP0;
  UINT64    RSP1;
  UINT64    RSP2;
  UINT64    Reserved_28;
  UINT64    IST[7];
  UINT64    Reserved_92;
  UINT16    Reserved_100;
  UINT16    IOMapBaseAddress;
} IA32_TASK_STATE_SEGMENT;

typedef union {
  struct {
    UINT32  LimitLow:16;    ///< Segment Limit 15..00
    UINT32  BaseLow:16;     ///< Base Address  15..00
    UINT32  BaseMidl:8;     ///< Base Address  23..16
    UINT32  Type:4;         ///< Type (1 0 B 1)
    UINT32  Reserved_43:1;  ///< 0
    UINT32  DPL:2;          ///< Descriptor Privilege Level
    UINT32  P:1;            ///< Segment Present
    UINT32  LimitHigh:4;    ///< Segment Limit 19..16
    UINT32  AVL:1;          ///< Available for use by system software
    UINT32  Reserved_52:2;  ///< 0 0
    UINT32  G:1;            ///< Granularity
    UINT32  BaseMidh:8;     ///< Base Address  31..24
    UINT32  BaseHigh:32;    ///< Base Address  63..32
    UINT32  Reserved_96:32; ///< Reserved
  } Bits;
  struct {
    UINT64  Uint64;
    UINT64  Uint64_1;
  } Uint128;
} IA32_TSS_DESCRIPTOR;
#pragma pack ()

#endif // defined (MDE_CPU_X64)

///
/// Byte packed structure for an FP/SSE/SSE2 context.
///
typedef struct {
  UINT8  Buffer[512];
} IA32_FX_BUFFER;

///
/// Structures for the 16-bit real mode thunks.
///
typedef struct {
  UINT32                            Reserved1;
  UINT32                            Reserved2;
  UINT32                            Reserved3;
  UINT32                            Reserved4;
  UINT8                             BL;
  UINT8                             BH;
  UINT16                            Reserved5;
  UINT8                             DL;
  UINT8                             DH;
  UINT16                            Reserved6;
  UINT8                             CL;
  UINT8                             CH;
  UINT16                            Reserved7;
  UINT8                             AL;
  UINT8                             AH;
  UINT16                            Reserved8;
} IA32_BYTE_REGS;

typedef struct {
  UINT16                            DI;
  UINT16                            Reserved1;
  UINT16                            SI;
  UINT16                            Reserved2;
  UINT16                            BP;
  UINT16                            Reserved3;
  UINT16                            SP;
  UINT16                            Reserved4;
  UINT16                            BX;
  UINT16                            Reserved5;
  UINT16                            DX;
  UINT16                            Reserved6;
  UINT16                            CX;
  UINT16                            Reserved7;
  UINT16                            AX;
  UINT16                            Reserved8;
} IA32_WORD_REGS;

typedef struct {
  UINT32                            EDI;
  UINT32                            ESI;
  UINT32                            EBP;
  UINT32                            ESP;
  UINT32                            EBX;
  UINT32                            EDX;
  UINT32                            ECX;
  UINT32                            EAX;
  UINT16                            DS;
  UINT16                            ES;
  UINT16                            FS;
  UINT16                            GS;
  IA32_EFLAGS32                     EFLAGS;
  UINT32                            Eip;
  UINT16                            CS;
  UINT16                            SS;
} IA32_DWORD_REGS;

typedef union {
  IA32_DWORD_REGS                   E;
  IA32_WORD_REGS                    X;
  IA32_BYTE_REGS                    H;
} IA32_REGISTER_SET;

///
/// Byte packed structure for an 16-bit real mode thunks.
///
typedef struct {
  IA32_REGISTER_SET                 *RealModeState;
  VOID                              *RealModeBuffer;
  UINT32                            RealModeBufferSize;
  UINT32                            ThunkAttributes;
} THUNK_CONTEXT;

#define THUNK_ATTRIBUTE_BIG_REAL_MODE             0x00000001
#define THUNK_ATTRIBUTE_DISABLE_A20_MASK_INT_15   0x00000002
#define THUNK_ATTRIBUTE_DISABLE_A20_MASK_KBD_CTRL 0x00000004

///
/// Type definition for representing labels in NASM source code that allow for
/// the patching of immediate operands of IA32 and X64 instructions.
///
/// While the type is technically defined as a function type (note: not a
/// pointer-to-function type), such labels in NASM source code never stand for
/// actual functions, and identifiers declared with this function type should
/// never be called. This is also why the EFIAPI calling convention specifier
/// is missing from the typedef, and why the typedef does not follow the usual
/// edk2 coding style for function (or pointer-to-function) typedefs. The VOID
/// return type and the VOID argument list are merely artifacts.
///
typedef VOID (X86_ASSEMBLY_PATCH_LABEL) (VOID);

#endif // defined (MDE_CPU_IA32) || defined (MDE_CPU_X64)
#endif // !defined (__BASE_LIB__)
