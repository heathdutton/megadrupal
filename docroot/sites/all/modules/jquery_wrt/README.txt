README.txt
----------
Adds "jQuery Plugin webks: Responsive Tables" (Project: https://github.com/JPustkuchen/jquery.webks-responsive-table, GitHub: https://github.com/JPustkuchen/jquery.webks-responsive-table) to table views in Drupal.
Query "webks Responsive Table" Plugin transforms less mobile compliant default HTML Tables into a flexible responsive (list) format. Furthermore it provides some nice configuration options to optimize it for your custom needs. 

JavaScript API
------------
You may get the created multiselect object from:
Drupal.behaviors.jquery_wrt.responsiveTable;

Furthermore you may of course implement your own responsive tables behaviour by calling
$('table').responsiveTable();
and others as documented on the project pages.

DEPENDENCIES
------------
- jQuery Update 7.x-2.x (http://drupal.org/project/jquery_update)
- jQuery 1.7 (selected in jQuery Update Module Settings)
- Libraries API 7.x-2.x (http://drupal.org/project/libraries)
- jQuery Plugin: jquery.webks-responsive-table (https://github.com/JPustkuchen/jquery.webks-responsive-table)

INSTALLATION
------------
1.    Download and enable this module
2.		Download the latest version of jquery.webks-responsive-table from Github (https://github.com/JPustkuchen/jquery.webks-responsive-table) and extract it to sites/all/libraries/jquery.webks-responsive-table
3.    Everything works with default settings now already! Admin pages are exlcuded by default just as some Drupal specials (drag + drop tables, etc.)
4.    Go to {YourURL}/admin/config/user-interface/jquery_wrt and configure it
5.    Done! Happy Using!
6.    Visit our Websites. Please.


AUTHOR/MAINTAINER
-----------------
- Julian Pustkuchen (http://Julian.Pustkuchen.com)
- Thomas Frobieter (http://blog.frobieter.de)
- Development proudly sponsored by:
    webks: websolutions kept simple (http://www.webks.de)
      and
    DROWL: Drupalbasierte Lösungen aus Ostwestfalen-Lippe (http://www.DROWL.de)