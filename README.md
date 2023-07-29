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
| ---------- | ---- | ----- |
| omicron    | 100  |   59  |
| xenon      |  94  |   57  |
| zyclon     |  82  |   48  |
| mercury    |  65  |   48  |
| sixtus     |  54  |   39  |
| pandora    |  48  |   17  |
| zodiak     |  37  |   27  |
| apollo     |  37  |   10  |
| ophiuchus  |  31  |   21  |
| nemesis    |  31  |   15  |
| thunder    |  25  |   19  |
| laniakea   |  25  |   13  |
| janus      |  25  |   10  |
| horus      |  20  |   17  |
| pangea     |  20  |   12  |
| gondwana   |  20  |    6  |
| sojus      |  14  |   16  |
| erebos     |  14  |   14  |
| orion      |   8  |   12  |
| sirius     |   2  |    1  |
| phoebus    |   2  |   -5  |
| prometheus |  -2  |    0  |
| gemini     | - 8  |    4  |
| delta      | -14  |    0  |
| andromeda  | -20  |   -6  |
| argon      | -31  |  -18  |
| aquarius   | -42  |  -27  |
| taifun     | -42  |  -34  |
| okapi      | -48  |  -21  |
| discordia  | -48  |  -24  |
| sputnik    | -71  |  -40  |
| krypton    | -77  |  -47  |
| ahoy       | -82  |  -74  |
| dummy      | -85  |  -71  |
| alpha      | -88  |  -77  |
| minimal    | -97  |  -28  |

### 3 Players

| Name       | Wins | Score |
| ---------- | ---- | ----- |
| omicron    |  90  |   74  |
| xenon      |  84  |   69  |
| zyclon     |  70  |   54  |
| mercury    |  63  |   51  |
| sixtus     |  46  |   35  |
| erebos     |  39  |   22  |
| apollo     |  37  |   14  |
| prometheus |  36  |    8  |
| nemesis    |  35  |   18  |
| zodiak     |  33  |   23  |
| pandora    |  32  |   15  |
| sojus      |  21  |   15  |
| ophiuchus  |  20  |   14  |
| janus      |  20  |   10  |
| thunder    |  17  |   14  |
| pangea     |  13  |    9  |
| orion      |  12  |   10  |
| horus      |  10  |   10  |
| gemini     |   9  |    7  |
| laniakea   |   9  |    6  |
| gondwana   |   0  |    2  |
| sirius     |  -5  |   -6  |
| delta      |  -7  |   -5  |
| phoebus    | -20  |  -17  |
| andromeda  | -22  |  -13  |
| argon      | -24  |  -18  |
| discordia  | -26  |  -18  |
| aquarius   | -30  |  -21  |
| taifun     | -50  |  -33  |
| okapi      | -53  |  -32  |
| sputnik    | -64  |  -46  |
| krypton    | -69  |  -51  |
| minimal    | -71  |  -19  |
| dummy      | -80  |  -62  |
| ahoy       | -84  |  -71  |
| alpha      | -87  |  -69  |

## Build

### Configure

`visualizer.h`:

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

Define the number of programs to run
```
  users = 1,
  users = 2,
  users = 3,
```

### Build

- `make xmars`: build the vm with graphical display
- `make mars`: build the fast batch version of the vm
- `make rx`: assemble red code programs
- `make test`: build and run all tests
