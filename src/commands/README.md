# Command Documentation
## Command: rand
### Takes 0 arg(s).
Fills the selected cells with random numbers between 0 and INT::MAX (usually 2^32).
## Command: randr
### Takes 2 arg(s).
Fills the selected cells with random numbers between arg1 and arg2. <br />
Arg2 must be greater than arg1 or else nothing will happen.
## Command: randw
### Takes 2 arg(s).
Fills the selected cells with random WHOLE numbers between arg1 and arg2. <br />
Arg2 must be greater than arg1 or else nothing will happen. <br />
If arg1 or arg2 are not whole numbers they are floored.
## Command: exit, q
### Takes 0 arg(s).
Exits the spreadsheet. Make sure to save changes!
## Command: jmp
### Takes 2 arg(s).
Jumps to the specified coordinate. <br />
Arg1 is the x coordinate and arg2 is the y coordinate.
## Command: jmpx
### Takes 1 arg(s).
Jumps to the specified x coordinate while preserving the current y coordinate.
## Command: jmpy
### Takes 1 arg(s).
Jumps to the specified y coordinate while preserving the current x coordinate.
## Command: ref
### Takes 0 arg(s).
Clears the screen and redraws everything.
## Command: save, s
### Takes 0 arg(s).
WARNING: doesn't seem to work properly at the moment. <br />
Saves changes made to the spreadsheet.
## Command: swap
### Takes 1 arg(s).
Swaps between two randomly selected cells repeating this arg1 amount of times.<br />
If arg1 is a non whole number, arg1 is floored.
## Command: shuffle
### Takes 1 arg(s).
Randomly reorders the selected cells saving it to the cursor's clipboard.<br />
This doesn't affect the selected cells.
