# UM-32

This is a C implementation of the UM-32 virtual machine from the
[2006 ICFP Programming Contest](http://boundvariable.org).

## Components

### Intermediate representation

For ease of debugging, I have created an intermediate representation for the
UM-32 bytecode.

The eight general-purpose registers are denoted `r0` through `r7`. The standard
operators are denoted as follows:

| Opcode | UM-32 instruction | Keyword | Description |
|:------:|:-----------------:|:-------:|-------------|
| `0x0` | Conditional move | `cmov` | register conditional move |
| `0x1` | Array index | `getp` | heap dereference pointer |
| `0x2` | Array amendment | `setp` | heap assign via pointer |
| `0x3` | Addition | `add` | reigster add |
| `0x4` | Multiplication | `mul` | register multiply |
| `0x5` | Division | `div` | reigster divide |
| `0x6` | Not-and | `nand` | register bitwise NAND |
| `0x7` | Halt | `halt` | halt execution |
| `0x8` | Allocation | `alloc` | heap allocation |
| `0x9` | Abandonment | `free` | heap deallocation |
| `0xa` | Output | `outb` | byte output |
| `0xb` | Input | `inb` | byte input |
| `0xc` | Load program | `load` | load heap object into program segment |
| `0xd` | Orthography | `li` | load immediate literal into register |

#### Example

Here are the first few instructions from the `sandmark.umz` profiler program,
translated from the bytecode (left) to the intermediate representation (right):

```asm
080000d0: cmov  r3 r2 r0
300000c0: add   r3 r0 r0

# The li instruction takes an immediate value supplied in hexadecimal
d2000014: li    r1 $0x14
d400005b: li    r2 $0x5b
d6000035: li    r3 $0x35
d000000d: li    r0 $0xd

c0000030: load  r6 r0
00000000: cmov  r0 r0 r0
5f0000d0: div   r3 r2 r0
300000c0: add   r3 r0 r0
c0000031: load  r6 r1
c0000031: load  r6 r1
```

### Shared library (`libum`)

The core UM-32 virtual machine is implemented in the shared library `libum.so`.

| Header | Description |
|--------|-------------|
| `fault.h` | specifies failure conditions |
| `heap.h`  | implements dynamic memory allocation |
| `instruction.h` | specifies the instruction layout |
| `machine.h` | provides the virtual machine interface |
| `opcode.h` | specifies the standard operators |
| `reader.h` | specifies the byte input interface |
| `writer.h` | specifies the byte output interface |

### Assembler (`umasm`)

I have supplied a UM-32 assembler for the intermediate representation in the
program `umasm` (perhaps pronounced "yoo masm").

```
Usage: ./umasm FILE
```

The program takes the instructions to assemble from the standard input stream
(`stdin`) and writes the binary output to the `FILE` provided.

### Disassembler (`umdasm`)

I have also created a UM-32 disassembler for the intermediate representation in
the program `umdasm` (pronounced "yoom dasm").

```
Usage: ./umdasm FILE
```

The program takes the bytecode from the binary `FILE` provided and writes the
intermediate representation of the instructions to the standard output stream
(`stdout`).

### Virtual machine (`umvm`)

Finally, the main UM-32 virtual machine is provided in the program `umvm` ("yoo
em vee em").

```
Usage: ./umvm FILE
```

The program executes the bytecode from the binary `FILE` provided.
