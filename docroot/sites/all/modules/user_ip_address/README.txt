INTRODUCTION
------------
This module allows to add field to content types containing author IP address.
IP address saves only on creating content.

This field can be used with views module.

USAGE
------------
1) Download and enable module User IP Address
2) Add field "User IP address" to content type

Thats all. After every node creation this field will contain user ip address.

** IMPORTANT **
By default this field is visible for all visitors. If your visitors don't need 
to see this field you need to hide this field on content type display page. 


TROUBLESHOOTING
------------
* IP address already '127.0.0.1'
  - If your site behind reverse proxy (for example varnish, nginx) you need 
    to setup Reverse Proxy Configuration in settings.php for 
    correct detecting ip address.

MAINTAINERS
-----------
* Roman Agabekov (drupaladmin) - https://www.drupal.org/u/dr.admin
