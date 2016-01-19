-------------------------------------------------------------------------------
SN Quick Multisite
This module helps to add up multisite enviornment without human intervention
however it requires the host file to be write mode enabled.
-------------------------------------------------------------------------------
DESCRIPTION:
The Quick Multisite module simplyfy the process of creation of new sites 
with subdomains.
This is very helpful in environments where multiple applications needs to run in 
parallel. 
-------------------------------------------------------------------------------
NOTE: 
It is recommneded to use this module only on development environment, as it 
requires some of settings(writable sites folder and database user with CREATE &
DROP database privileges), which ideally should not be done on
production environment.   
-------------------------------------------------------------------------------
FEATURES:

* Very simple to create multi-sites;
* Does host entry in the hosts file of the server to make subdomain site run;
   Hosts file refers to file on the server, where we have entries for all 
   hosts on that server. For e.g. usually on Linux severs we have a hosts file 
   located at /etc/hosts location.	
* Creates site's main directory (e.g. sudomain.example.com) 
  under Drupal's sites directory;
* Creates separate settings.php file for each site
* Creates one new separate database for each new site;
* Allows users to clone an existing site with cloning options;

INSTALLATION:

* Put the module in your drupal modules directory and enable it at 
  admin/modules. 
* Use the module at admin/config/development/quick-multisite

-------------------------------------------------------------------------------
