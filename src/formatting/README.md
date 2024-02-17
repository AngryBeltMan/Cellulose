# Formatting Documentation
## Formatting an Input
To format an input use `f|` at the very start.
## Parsing an Equation
To parse an equation in insert mode using formatting, add a percent symbol and put the equation inside square brackets. Example: `%[2+2]`. <br />
WARNING: The equation's order of opperation doesn't follow PEMDAS or GEMDAS and instead is evaluated from left to right. <br />
For example, the equation `3-2*2` would output 2 instead of -1. To remedy this use parenthesis.
## Changing the value based on position
You can change the input based on the selected cell position by using the letters x and y inside the equation. Example: `%[x+y]`. <br />
If you don't want to put it inside an equation, you can also just use %x and %y. This also has the added bonus of not showing the trailing decimal places.<br />
Example: `x: %x y: %y`.
## Changing the value based on other cell values
You can change the input based on other cell values by providing an x and y value seperated by commas. Examples: `%[x+1,y+2]`, `%[(x+2,y-2)*3]`.
## Using roots and powers
Currently roots and powers are not supported.

