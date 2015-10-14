****************************************************************************
                S I N G L E   S I G N   O N   M U L T I P L E   D O M A I N
********************************************************************
Original Author: Varun Mishra
Current Maintainers: Varun Mishra
Email: varunmishra2006@gmail.com

****************************************************************************
DESCRIPTION:

   Single Sign On Multiple Domain module allows login to multiple drupal 
   websites from a single website. If you have multiple drupal website installed 
   at either on same server or other server, then you just need to login to one 
   and it will login you to other websites as configured in the configuration
   page. For this you have to install this module in all website and do required 
   settings. 

   This module also allows to create account for user if user do not exist in 
   other website. It also allows you to synchronize user role while creating 
   account.

   It provides you various configuration like enable/disable account creation, 
   enable/disable roles synchronization, message to display on processing page,
   loading image, text below loading image etc.

   This module is very secured and uses encryption. I have developed my own 
   custom encryption functions. 

   The difference between other single sign on modules and this module is that
   this allows cross domain login and it is very simple to use as compared to 
   other.


****************************************************************************

INSTALLATION:

1. Place the entire sso_multi_domain directory into sites modules directory
  (eg sites/all/modules).

2. Enable this module by navigating to:

     Administration > Modules


3) Please read the step by step instructions as an example to use this
   module below:-

a) Install the module.

b) Go to admin/config/system/sso_multi_domain.

c) Enter domains in domain names text area per line in which you want single 
   sign on to be enabled. For example, if you enter domain1.com, domain2.com, 
   then when users login to this current domain then they will be able to login 
   to these two domains as well.

d) Check "Allow Account Creation option" if you want to allow account creation 
   if user does not exists on your website when any requesting websites tries to 
   login via this module.

e) Check "Allow Roles Synchronization During Account Creation option" if you 
   want to allow synchronization of roles from requesting website when role does
   not exists. For example, if a user have role editor and he tries to login to 
   a sister website. When the request send to login to this website by this 
   module, the system will create "editor" role in this website and assign it to
   the user.

f) Enter Text for single sign on processing page.

g) Enter Text to display below loader image.

h) Upload loader image.

i) Repeat step a to h for all other domains.

For example, if you have 3 domains i.e., example1.com, example2.com, 
example3.com.  After installing module on example1.com go to 
http://example1.com/admin/config/system/sso_multi_domain page .
In Domain names text area enter example2.com and example3.com in individual 
lines. You need to skip to enter current domain name in that text area.

This is the first release, therefore it has basic and required feature. In the 
release I am going to include following features:-

a) Single Logout with configuration setting.
b) User synchronization during registration with configuration setting.

Please create an issue if you find any bug. Please create a feature request if 
you thing any feature is missing and that will be useful.
