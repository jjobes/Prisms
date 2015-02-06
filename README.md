Prisms
-----

Prisms is a simple game written in C++ where the player creates chain reactions. 

It utilizes the DarkGDK game programming library which relies on DirectX, 
and so it is only compatible with Windows.  It was developed using 
Visual C++ 2008 Express Edition. 

<img src="https://raw.githubusercontent.com/jjobes/Prisms/master/screenshots/screenshot_1.png" width="400">


Download
--------

There is a Windows installer (Install_Prisms.exe) in the 'installer' directory.

This will install the game along with a copy of the source code.

Importing the project into Visual C++
----------------------------------

You will need to have the following installed:

* Microsoft Visual C++ Express 2008
* Microsoft DirectX 9.0c SDK (August 2007)
* Dark GDK

    There are links to download these [here](http://www.thegamecreators.com/?m=view_product&id=2128&page=download).
    
    Detailed instructions on setting them up can be found [here](http://www.thegamecreators.com/?m=view_product&id=2128&page=install).

Clone the repository to your local machine using Git:

   ```
   git clone https://github.com/jjobes/Prisms.git
   ```

Start up Visual C++ and choose:

File -> Open -> Project/Solution 
    
Browse to the Prisms source directory and open Prisms.sln.
   
In order for it to compile correctly with the vector include, you may have to modify the Visual C++ settings in the following way:

Project -> Prisms Properties -> Configuration Properties ->  C/C++ Code Generation

Change the Runtime Library from /MTd (Multi-threaded debug) to /MT (Multi-threaded).                 

Run the application by choosing: Debug -> Start Debugging (or press F5).

License
-------

Apache License, Version 2.0
   