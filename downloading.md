# Downloading
To download and use the spreadsheet editor clone this repo and `cd` into the folder and run the command below. <br />
Note: you cannot delete the directory after it finishes building or else cellulose won't work.
## Linux
`
# create the build directory
mkdir -p build
# builds the repo
make
# save the path of the spreadsheet executable into a environment variable
export CELLULOSE_EXE_DIR=$(pwd)/build/cellulose
# run cellulose into the command line to execute the spreadsheet editor. The alias cellulose can be set to any name of your choosing.
alias cellulose=CELLULOSE_EXE_DIR
`
