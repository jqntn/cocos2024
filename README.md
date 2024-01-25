steps:
- run "python download-deps.py" in "\JuGame\cocos2d"
- run "cmake . -G"Visual Studio 17 2022" -Tv143 -A Win32" in "\JuGame"
- open the solution and run
- python 2.7 recommended (https://sourceforge.net/projects/portable-python/files/Portable%20Python%202.7/)

game:
- drag the character around with your mouse to avoid bombs
- click on bombs to destroy them
- the longer you survive the better score you get
