
DESCRIPTION
===========
This module allows the determination of whether to show a CAPTCHA on specific forms to be based on whether the user matches or doesn't match a 
specific IP range in CIDR format.

This module is designed to work with Drupal 6.x.

Installation
=====
1. Copy this directory to a suitable modules directory, such as 
     sites/all/modules
2. Default settings may be adjusted in the captcha_by_ip.defaults.inc file. These defaults are loaded on install. This is optional to assist in deployment. Settings may be adjusted through the UI as well.
3. Enable the module at: Administer > Site building > Modules
   
USAGE
=====
1.) All configuration can be managed from administration pages located at Administer > User Management > CAPTCHA > CAPTCHA By IP
2.) Only forms that currently have CAPTCHA enabled at Administer > User Management > CAPTCHA > CAPTCHA > General Settings will be listed 
3.) IP ranges must be entered in CIDR format. To find out more about CIDR, go to: http://www.brassy.net/2007/mar/cidr_basic_subnetting

AUTHOR
======
James Gross (jamesrgross [at] gmail.com)

SPONSORED BY
===========
UNC Charlotte (http://www.uncc.edu)
