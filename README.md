# DEF CON Furs 2023 Badge 

# ventorvar's boop nonsense

This is not a full source/build, but meant two files you can update that adds initial boop support. Get it going with something like:

```
rshell rsync ./src /
rshell "repl ~ import machine ~ machine.soft_reset() ~"
```

## Schematic

The Schematic is located in this PDF: [DCF31BadgeSchematic.pdf](https://github.com/defconfurs/dcfurs-badge-dc31-public/blob/main/DCF31BadgeSchematic.pdf)


## Bill of Materials

The BoM is on Google Sheets:  https://docs.google.com/spreadsheets/d/1LJeAcxZYNxy8RmYSu6KfBTrNyNzvA-suKKZmO6_C-Eg/edit?usp=sharing

## Builds Folder

### OSD.ino.uf2

This is an LED test for the badge.

### OSDColour.ino.uf2

We had issues with the OSD.ino program not detecting shorted LEDs, so this is a test program that works better at that.

### OSDColour500.ino.uf2

Same as above, just faster.

### DCFurs31_initial.ino.uf2

This is the initial release firmware on the badges.


### firmware.uf2 

This is the new MicroPython firmware.


