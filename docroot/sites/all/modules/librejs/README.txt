LibreJS module provides compatibility with the LibreJS browser plugin. 
It allows site administrators to configure the license of each 
JavaScript file used by the site, and adds metadata to pages indicating 
that all inline JavaScript in the page is GPLv3.

After installing the module, visit admin/config/development/librejs to 
configure the license and source code URL for each JavaScript file.  
LibreJS module will only discover JavaScript files as they are loaded, 
so you will have to visit this page repeatedly to configure additional 
JavaScript files as they are detected.

Only roles granted the "access JavaScript license information"
permission can access the JavaScript license information page. Warning:
This permission allows users to identify and read all JavaScript files
used by the site.

Note: You should only install this module if you are sure all inline 
JavaScript on your site is in fact GPLv3 compatible.
