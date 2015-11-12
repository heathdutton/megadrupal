DESCRIPTION
-----------
This module provides an easy way to import previously exported Panels definitions.
Imports mini-panels, pages and page variants from corresponding subfolders.

INSTALLATION
-----------
1. Create 'sites/all/imports' folder.
2. Create all necessary subfolders for panels:
	-> sites/all/imports/page_manager/handlers
	-> sites/all/imports/page_manager/pages
	-> sites/all/imports/page_manager/pm_existing_page
	-> sites/all/imports/panels/mini
	-> sites/all/imports/ctools/ctools_contents
3. Export the panels you wish to import and paste the export code into separate text files with .inc' extension.
4. Do not forget to add '<?php' line to the top of the files.
5. Move files to corresponding subfolders.
6. Install this module.

MAIN IDEA
-----------
The idea is that you could have a collection of commonly used panels in a folder.
Then, when you start a project, copy the required panels definitions into this module and install.
This will add all the panels in one go, saving you from using the Panels import form.
