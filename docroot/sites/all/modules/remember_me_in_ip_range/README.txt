
-- SUMMARY --

This module is for providing an option to the users that the user will be 
remembered with his Login if he is logged-in from a IP range given by the Site 
Administers. Only the site administer have the permission to add IP Ranges for 
the user.

This module have dependency on Remember me Module.
http://drupal.org/project/remember_me

If you enable Remember Me in IP Range Module you won't see Remember me check 
box in Login page or block.

This module supports various format for IP or IP ranges. Accepted IP Range 
formats are:

  Single IP matches like 123.123.123.123

  Wildcards using an asterisk (*) in any quadrant except the first one, 
  for example 123.123.123.* or 100.*.*.* etc.

  Ranges using a hyphen (-) in any quadrant except the first one, 
  for example 123.123.123.100-200 etc.

  Any number of comma-separated IP addresses or ranges 
  like 10.11.12.13, 123.123.123.100-200, 123.123.124-125.* etc.

For a full description of the module, visit the project page:
  http://drupal.org/project/remember_me_in_ip_range

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/remember_me_in_ip_range


-- REQUIREMENTS --

You must have to be install the Remember Me module to use this module.
Remember Me
  http://drupal.org/project/remember_me


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

Go Home » Administration » Configuration » System » Remember Me in IP Range 
settings

Give an IP range you want following any format given bellow:
  
  Single IP matches like 123.123.123.123

  Wildcards using an asterisk (*) in any quadrant except the first one, 
  for example 123.123.123.* or 100.*.*.* etc.

  Ranges using a hyphen (-) in any quadrant except the first one, 
  for example 123.123.123.100-200 etc.

  Any number of comma-separated IP addresses or ranges 
  like 10.11.12.13, 123.123.123.100-200, 123.123.124-125.* etc.


-- CONTACT --
behestee@gmail.com

Current maintainers:
* Hussain KMR Behestee - http://drupal.org/user/345066/

For more information see: http://behestee.wordpress.com/
or http://behestee.com/
