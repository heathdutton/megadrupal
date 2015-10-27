***********
* README: *
***********

INTRODUCTION
------------
This module adds ability to send email attachment from 

* Welcome (new user created by administrator)
* Welcome (awaiting approval)
* Welcome (no approval required)
* Account activation
* Account blocked
* Account cancellation confirmation
* Account canceled
* Password recovery

This module will allow you to add email attachment at 
URL: admin/config/people/accounts
When each specified mail will send with that attachment will also send.

REQUIREMENTS
------------
* Mime Mail (URL: https://www.drupal.org/project/mimemail)



INSTALLATION
-------------
* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

* Enable the account_settings_email_attachment module by navigate to 
  <SITENAME>/admin/modules


CONFIGURATION
-------------

* Configure allowed extension from

  you can set allowed extension at 
  (<SITENAME>/admin/config/account_settings_email_attachment) 

* Add mail attachment from

  Once account_settings_email_attachment module is installed. 
  you need to navigate to Account settings page 
  (<SITENAME>/admin/config/people/accounts).
  There you will able to add email attachment
  

* When each specified mail will send with that attachment will also send.


MAINTAINERS
-----------

Current maintainers:
* Arijit Sarkar (riju) - https://www.drupal.org/u/arijits.drush

* email:riju.srk@gmail.com ,arijits.drush@gmail.com
