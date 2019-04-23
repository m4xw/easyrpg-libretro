# liblcf source code generator

Files in the `generated` subdirectory of `src` directory are regenerated
automatically when running the `generate.py` script.

These source code files are generated with the `templates` subfolder
structure based on the `csv` subfolder data files.


## Requirements

* Python interpreter version 2 or 3.
* CSV files can be modified with any text editor or (at your option) any
  spreadsheet editor.


## Usage

1. Open one of the .csv files in the `csv` subdirectory to edit or add new
   data then save file changes.
2. Run the script file `generate.py` from the `generator` folder.
3. Add any newly created .cpp and .h files to project files if needed.
4. Recompile liblcf.
