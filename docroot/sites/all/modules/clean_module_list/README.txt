# README file for clean_module_list
# Copyright (C) 2011 Stewart Adam <s.adam at diffingo.com>
# License: GNU GPLv2+

What does clean_module_list do?
-------------------------------
* Hides the module dependency information (the "Depends on:" and "Required by:"
  fields) for each module when loading the module list in order to make it
  easier to search through
* Adds Show/Hide radio buttons to the module list page to dynamically control
  the display of the dependency information with JavaScript


Installation
------------
Simply copy the clean_module_list folder into your sites/all/modules folder and
then browse to admin/build/module to enable it.


Configuration
-------------
None required.


Caveats and known bugs
----------------------
* JavaScript must be enabled for this module to work.
