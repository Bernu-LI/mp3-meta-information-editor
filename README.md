# MP3 file meta-information editor

**Task**

Implement an editor for text meta-information of an MP3 file.
ID3v2 is adopted as the meta-information standard.
The editor is a console program that accepts the file name as argument via the --filepath parameter, as well as one of the selected commands:
1. **--show** - display all meta-information in a table;
2. **--set=prop_name --value=prop_value** - set the value of a specified meta-information field named _prop_name_ to _prop_value_;
3. **--get=prop_name** - output a specific meta-information field named _prop_name_.


For example:

_app.exe --filepath=Song.mp3 --show_

_app.exe --filepath=Song.mp3 --get=TIT2_

_app.exe --filepath=Song.mp3 --set=COMM --value=Test_
