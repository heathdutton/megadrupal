
RACKSPACE WEBMAIL INTEGRATION MODULE FOR DRUPAL 7.x
---------------------------------------------------

CONTENTS OF THIS README
-----------------------

   * Description
   * Requirements
   * Installation Instructions
   * Credit


DESCRIPTION:
------------
This module integrates with rackspace webmail
and provides a single sign-on for users.

It uses nusoap for Rackspace Webmail API integration.

It will create email account on rackspace webmail when new user
register on Drupal site.

Format: username@YOURSITENAME.COM

Example:--
if any user register with Drupal user: firstname.lastname
Rackspace webmail Inbox user will be:  firstname.lastname@YOURSITENAME.COM


REQUIREMENTS:
-------------
Follow these two steps for install the third party NuSOAP library:-

1. Create a folder name must be 'rswebmail_nusoap' in "sites/all/libraries"

2. Download and Extract the NuSOAP library into the 'rswebmail_nusoap' directory
   (usually "sites/all/libraries/rswebmail_nusoap").
   Link: http://sourceforge.net/projects/nusoap
   Direct link for Download:
   http://sourceforge.net/projects/nusoap/files/latest/download




INSTALLATION INSTRUCTIONS:
--------------------------
1. Put the module in your Drupal modules directory and enable it
   in admin/modules.

2. Go to /admin/config/services/rswebmail and put your 'Organization name',
   'Host Name/Domain name', 'Username' and 'Password'.

3. Go to admin/people/permissions and grant permission to any roles that
   need to be able to allow single sign-on for users.

4. It will create a "Mail information" tab on /user/* page.
   Use the module at /user/*/mail

5. Goto: /admin/structure/block and assign 'Webmail new message count' block
   to any region you needed.


CREDIT
-------
babusaheb.vikas - Douce Infotect Pvt. Ltd.
