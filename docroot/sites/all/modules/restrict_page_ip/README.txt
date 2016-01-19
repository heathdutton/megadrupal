INTRODUCTION
------------
The Restrict Page IP module provides site administrator to restrict/allow 
access to pages based on user IPs.
  * IPs can be an individual IP or range of Ips. 
  * Page url can have wild cards like 'blog/*'
  * Restricted user IPs will be denied showing 
    custom error message (can be modified on module's configuration page)

  Note : User 1 has been skipped from these restrictions.
  * For a full description of the module, visit the sandbox project page:
   https://www.drupal.org/sandbox/pkapils/2341587


REQUIREMENTS
------------
No special requirements.


INSTALLATION
------------
  * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
  * All configuration can be managed from the module configuration page 
    located at Administration > Configuration > People > Restrict Page by IP.
  * The configuration page will show the custom message to show for 
    blacklisted users and list of Blacklist/Whitelist IPs 
    for the corresponding page in a table.
  * An option has been given to add Blacklist/Whitelist IPs for a page .
  *  Access Restrict Page IP Configuration page
	- Users in roles with the "administer restrict page by ip" permission to 
          access module configuration page.


MAINTAINERS
-----------
Current maintainers:
  * Kapil Pandian (pkapils) - https://www.drupal.org/u/pkapils
  * Nitesh pawar (Nitesh Pawar) - https://www.drupal.org/u/nitesh-pawar
