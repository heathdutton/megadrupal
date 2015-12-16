--------------
Export logs
--------------

-----------
DESCRIPTION
-----------

Sometimes you want to debug the code quickly and there is a need that you do not
have much time for it. If you go to Drupal`s log message report, it will contain
lot of pages and you need to walk through all of the pages. Here in this module;
you do not have to navigate anywhere and you have to set no configuration just
simply call a url and export the logs.
The logs will be in text file which is properly formatted. One can easliy find
the error or text for which developer looks mainly.
It saves lot of time rather than clicking multiple times and navigating through
multiple pages.
This module is flexible enough to handle 'n' number of records. It does little
calculation and within the seconds it generates the log file on the fly.

Very useful in fixing production issues and ideal for development.

-----------------------
Drupal required version
-----------------------
Drupal 7.x

----------
INSTALLING
----------
1. Copy the 'export_logs' folder into your sites/all/modules directory.

2. Go to admin/modules and enable the module.
Refer instalation modules at http://drupal.org/node/70151

-------------
CONFIGURATION
-------------
No configuration is required.

-------------
WORKING
-------------

1. Type simply explog in address bar.

Example: http://sitename/explog

2. For fetching n number of records. please type like below.

Examples:
http://sitename/explog?fetch=100
http://sitename/explog?fetch=500
http://sitename/explog?fetch=1000
