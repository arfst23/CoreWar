# Core War

[Core War](https://en.wikipedia.org/wiki/Core_War) is a 1984 programming game in
which two or more battle programs compete for control of a virtual computer.
These battle programs are written in an abstract assembly language called
Redcode and executed on a virtual machine called mars (memory array redcode
simulator)

At the beginning of a game, each battle program is loaded into memory at a
random location, after which each program executes one instruction in turn.  The
goal of the game is to cause the processes of opposing programs to terminate
(which happens if they execute a cell with no instruction).

The memory is addressed in units of one instruction.  The memory space (or core)
is of finite size, only relative addressing is used.  The memory has 256x200
memory cells which are displayed as a matrix.  A program can modify memory cells
up to 256x20 addresses

Each user can a maximum of 4 threads in parallel.  If both players have running
threads after 256x200x12 cycles the game is counted as draw.

## COMMAND REFERENCE

### dat
```
dat
dat	val
```
Numeric value val at address, if value is omitted it defaults to 0, terminates
the thread when executed.

### set
```
set	adr, val
```
Set the content of the relative address adr to the value val

Example before:
```
  set foo, 23		<-- execution
  ...

foo:
  dat 42
```
Example after:
```
  set foo, 23
  ...				<-- execution

foo:
  dat 23
```

Example before:
```
  set foo, 23		<-- execution
  ...

foo:
  jmp foo
```
Example after:
```
  set foo, 23
  ...				<-- execution

foo:
  dat 23
```

### inc
```
inc	adr
inc	adr,	off
```
Increase the content of relative address by the offset off, if offset is omitted
it defaults to 1

Example before:
```
  inc foo, 19		<-- execution
  ...

foo:
  dat 23
```
Example after:
```
  inc foo, 19
  ...				<-- execution

foo:
  dat 42
```

Example before:
```
  inc foo, 23		<-- execution
  ...

foo:
  jmp foo
```
Example after:
```
  inc foo, 23
  ...				<-- execution

foo:
  dat 23
```

### dec
```
dec	adr
dec	adr.	off
```
Decrease the content of relative address by the offset off, if offset is omitted
it defaults to 1

Example before:
```
  dec foo, 19		<-- execution
  ...

foo:
  dat 42
```
Example after:
```
  dec foo, 19
  ...				<-- execution

foo:
  dat 23
```

Example before:
```
  dec foo, 23		<-- execution
  ...

foo:
  jmp foo
```
Example after:
```
  dec foo, 23
  ...				<-- execution

foo:
  dat -23
```

### jmp
```
jmp	off
```
Jump to offset off relative to the current address

Example before:
```
  jmp foo			<-- execution
  ...

foo:
  ...
```
Example after:
```
  jmp foo
  ...

foo:
  ...				<-- execution
```

```
jlt	off.	adr.	val
jle	off.	adr.	val
jgt	off.	adr.	val
jge	off.	adr.	val
jeq	off.	adr.	val
jne	off.	adr.	val
```
Jump to offset off relative to the current address if the content of relative
address adr fulfills the comparison to the value val

Example before:
```
  jlt foo, bar, 23	<-- execution
  ...

foo:
  ...

bar:
  dat 42
```
Example after:
```
  jlt foo, bar, 23
  ...				<-- execution

foo:
  ...

bar:
  dat 42
```

Example before:
```
  jlt foo, bar, 23	<-- execution
  ...

foo:
  ...

bar:
  dat 5
```
Example after:
```
  jlt foo, bar, 23
  ...

foo:
  ...				<-- execution

bar:
  dat 5
```

### frk
```
frk	off
```
Fork to offset off relative to the current address

Example before:
```
  frk foo			<-- execution
  ...

foo:
  ...
```
Example after:
```
  jmp foo
  ...				<-- execution

foo:
  ...				<-- execution

```

### cpy
```
cpy	dst.	src.	off
```
Copy the content of relative addess src plus the content of the relative address
off to the relative address dst plus the content of the relative address off

Example before:
```
  cpy dst, src, index	<-- execution
  ...

src:
  ...
  set foo, 23

foo:
  dat

dst:
  ...
  dat

bar:
  dat
```
Example after:
```
  cpy dst, src, index
  ...					<-- execution

src:
  ...
  set foo, 23

foo:
  dat

dst:
  ...
  set bar, 23

bar:
  dat
```

### cpi
```
cpi	dst.	src.	off
```
Copy the content of relative addess src to the relative address dst plus the
content of the relative address off

Example before:
```
  cpy dst, src, index	<-- execution
  ...

src:
  dat 23

dst:
  ...
  jmp dst
```
Example after:
```
  cpy dst, src, index
  ...					<-- execution

src:
  dat 23

dst:
  ...
  dat 23
```

## Assembler Reference

`rcas`: the red code assembler

```
# ignored
```
Everything from a `#` to the end of the line is ignored

```
lbl:
```
Define the label lbl at the current address

```
lbl = exp
```
Define the label lbl evaluating the expression exp

Example:
```
    foo = bar + 23
```

```
cmd
```
A command from the above list, when a label is provided to a relative address it
is evaluated relative to the current address

Example:
```
  jmp foo
  ...

foo:
  ...  # jumps here

```

## Code Examples

```
# create a copy and jump to it

distance = 256
dst = src + distance
length = end - src

src:
  set counter, 0
loop:
  cpy dst, src, counter
  inc counter
  jle loop, counter, length

  # jump to copy
  jmp dst
end:

counter:
  dat
```

```
# overwite every nth memory location

step = 8
length = 256
dest = end + step

start:
  set counter, step
loop:
  cpi end, halt, counter
  inc counter, step
  jle loop, counter, length
  jmp start

counter:
  dat
halt:
  dat
end:
```

```
# interprocess communication: remote switch

  frk remote

loop:
  # do something

switch:
  jmp loop

  # continue with something else
  ...

remote:
  # do something

  # to switch the switch
  cpy switch, cont, cont

cont:
  jmp cont + 1
```

```
# interprocess communication: watchdog timer

  frk watchdog

loop:
  # do something

  # reset timer
  set guard, 1

  jmp loop

watchdog:
  set guard, 0
  # so something els that takes longer
  jeq watchdog, guard, 1

  # timer expired
  
guard:
  dat
```

## Results

### 2 Players

| Name       | Wins | Score |
|------------|------|-------|
| omicron    |   90 |   56  |
| zyclon     |   86 |   58  |
| xenon      |   81 |   53  |
| mercury    |   72 |   51  |
| cyan       |   68 |   33  |
| pandora    |   52 |   27  |
| janus      |   50 |   26  |
| horus      |   45 |   37  |
| erebos     |   45 |   21  |
| sojus      |   43 |   22  |
| orion      |   40 |   34  |
| thunder    |   40 |   24  |
| nemesis    |   38 |   16  |
| zodiak     |   36 |   28  |
| magenta    |   36 |   21  |
| sixtus     |   34 |   34  |
| gondwana   |   34 |   16  |
| ophiuchus  |   31 |   22  |
| laniakea   |   27 |   22  |
| gemini     |   22 |   11  |
| pangea     |   18 |   12  |
| prometheus |   18 |    5  |
| sirius     |   11 |    4  |
| amber      |   -2 |    1  |
| delta      |   -4 |    3  |
| nautilus   |   -4 |    0  |
| phoebus    |   -6 |    0  |
| andromeda  |  -11 |   -4  |
| argon      |  -20 |  -13  |
| agua       |  -20 |  -15  |
| purple     |  -22 |  -11  |
| taifun     |  -34 |  -21  |
| okapi      |  -40 |  -17  |
| discordia  |  -40 |  -17  |
| sputnik    |  -45 |  -27  |
| krypton    |  -63 |  -32  |
| apollo     |  -65 |  -24  |
| ahoy       |  -68 |  -64  |
| alpha      |  -68 |  -65  |
| ckloader   |  -77 |  -62  |
| loop       |  -84 |  -29  |
| dummy      |  -84 |  -59  |
| forward    |  -84 |  -80  |
| backward   |  -86 |  -79  |
| run        |  -90 |  -24  |

### 3 Players

| Name       | Wins | Score |
|------------|------|-------|
| omicron    |   87 |    8  |
| xenon      |   79 |    6  |
| zyclon     |   77 |    7  |
| cyan       |   67 |    3  |
| mercury    |   64 |    5  |
| orion      |   53 |    4  |
| janus      |   46 |    3  |
| prometheus |   45 |    1  |
| pandora    |   42 |    3  |
| erebos     |   41 |    2  |
| horus      |   40 |    3  |
| zodiak     |   39 |    2  |
| sixtus     |   38 |    3  |
| nemesis    |   33 |    2  |
| thunder    |   32 |    2  |
| sojus      |   28 |    1  |
| ophiuchus  |   28 |    1  |
| gemini     |   22 |    1  |
| pangea     |   22 |    1  |
| gondwana   |   18 |    1  |
| laniakea   |   17 |    1  |
| magenta    |   14 |    0  |
| sirius     |   -4 |    0  |
| amber      |   -4 |    0  |
| delta      |   -8 |    0  |
| phoebus    |  -10 |    0  |
| argon      |  -11 |   -1  |
| discordia  |  -13 |   -1  |
| nautilus   |  -14 |   -1  |
| purple     |  -18 |   -1  |
| andromeda  |  -21 |   -1  |
| agua       |  -22 |   -1  |
| loop       |  -29 |   -1  |
| taifun     |  -29 |   -2  |
| okapi      |  -40 |   -2  |
| sputnik    |  -41 |   -3  |
| run        |  -46 |   -1  |
| krypton    |  -50 |   -4  |
| apollo     |  -51 |   -2  |
| dummy      |  -65 |   -5  |
| backward   |  -71 |   -6  |
| forward    |  -71 |   -6  |
| ahoy       |  -71 |   -6  |
| ckloader   |  -72 |   -5  |
| alpha      |  -74 |   -6  |


## Build

### Configure

`visualizer.cc`:

For QHD 2560x1440 screen resolution set
```
  hscale = 4,
  vscale = 3,
```

For FHD 1920x1080 screen resolution set
```
  hscale = 3,
  vscale = 2,
```

`redcode.h`:

To enable runtime traces set
```
  trace = 1,
```

### Build

- `make xmars`: build the vm with graphical display
- `make mars`: build the fast batch version of the vm
- `make rx`: assemble red code programs
- `make test`: build and run all tests
