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
| nautilus   |  100 |   65  |
| xenon      |   86 |   49  |
| cyan       |   81 |   39  |
| zyclon     |   79 |   52  |
| omicron    |   79 |   52  |
| mercury    |   65 |   44  |
| magenta    |   46 |   31  |
| sixtus     |   39 |   27  |
| erebos     |   34 |   19  |
| sojus      |   32 |   18  |
| horus      |   30 |   27  |
| zodiak     |   30 |   24  |
| janus      |   30 |   19  |
| thunder    |   30 |   18  |
| amber      |   30 |   14  |
| pangea     |   27 |   13  |
| nemesis    |   27 |   12  |
| orion      |   25 |   27  |
| pandora    |   23 |   19  |
| ophiuchus  |   23 |   18  |
| laniakea   |   16 |   14  |
| gondwana   |   13 |   13  |
| prometheus |   13 |    2  |
| gemini     |    2 |    5  |
| sirius     |   -2 |    0  |
| phoebus    |   -2 |   -3  |
| delta      |   -4 |    0  |
| purple     |   -6 |   -4  |
| agua       |  -16 |  -11  |
| andromeda  |  -18 |   -8  |
| argon      |  -23 |  -16  |
| taifun     |  -34 |  -27  |
| okapi      |  -44 |  -20  |
| discordia  |  -53 |  -24  |
| sputnik    |  -58 |  -34  |
| krypton    |  -58 |  -37  |
| apollo     |  -72 |  -27  |
| ahoy       |  -72 |  -65  |
| alpha      |  -72 |  -70  |
| forward    |  -81 |  -82  |
| loop       |  -83 |  -30  |
| dummy      |  -83 |  -62  |
| backward   |  -86 |  -81  |
| run        |  -95 |  -27  |

### 3 Players

| Name       | Wins | Score |
|------------|------|-------|
| nautilus   |   86 |    7  |
| omicron    |   84 |    7  |
| cyan       |   79 |    3  |
| xenon      |   74 |    5  |
| zyclon     |   70 |    6  |
| mercury    |   52 |    4  |
| orion      |   46 |    3  |
| erebos     |   38 |    2  |
| janus      |   38 |    2  |
| horus      |   32 |    2  |
| prometheus |   30 |    0  |
| sixtus     |   29 |    2  |
| magenta    |   28 |    2  |
| nemesis    |   28 |    1  |
| pandora    |   28 |    1  |
| zodiak     |   27 |    1  |
| thunder    |   24 |    1  |
| sojus      |   18 |    1  |
| ophiuchus  |   17 |    1  |
| gemini     |   16 |    0  |
| amber      |   14 |    1  |
| pangea     |   14 |    0  |
| gondwana   |   11 |    1  |
| laniakea   |    8 |    0  |
| delta      |  -10 |    0  |
| sirius     |  -10 |    0  |
| agua       |  -14 |   -1  |
| purple     |  -15 |   -1  |
| phoebus    |  -18 |   -1  |
| argon      |  -19 |   -1  |
| discordia  |  -23 |   -1  |
| andromeda  |  -25 |   -1  |
| loop       |  -34 |   -1  |
| taifun     |  -35 |   -3  |
| sputnik    |  -44 |   -3  |
| okapi      |  -46 |   -3  |
| apollo     |  -52 |   -2  |
| run        |  -53 |   -2  |
| krypton    |  -54 |   -4  |
| dummy      |  -67 |   -5  |
| ahoy       |  -72 |   -6  |
| forward    |  -72 |   -6  |
| backward   |  -73 |   -6  |
| alpha      |  -76 |   -6  |

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

### Evaluate

- 2 players tournament: `tournament2 *.rx | evaluate2`
- 3 players tournament: `tournament3 *.rx | evaluate3`
