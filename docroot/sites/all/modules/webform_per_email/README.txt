------------------------------------------------------------------------------
                Webform Per Email (webform_per_email)
------------------------------------------------------------------------------

INTRODUCTION
------------
  This module implements a permit based access system to allow anonymous users
  submit a Webform only if they display a valid one-time access permit.

REQUIREMENTS
------------
  This module requires the following modules:
  Webform (https://drupal.org/project/webform)

INSTALLATION
------------
  Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-------------
  Go to Configuration -> Webform per Email.
 
USAGE
-------------
  * Go to the Webform you want to allow access by permits only.
  * Click on edit and select "Accessible by permit only"
    in the publishing options
  * Go to Configuration -> Webform per Email and Create new permits by entering
    a list of emails separated by spaces or newline as well as the path of the
    webform. Emails will be generated automatically and sent automatically.
  * Emails are queued and a configurable number of emails are sent per cron run
  * On the Settings tab, you can configure the email that is sent.
    A list of variable are shown in the description for the email
    body and subject.
  * You can add more variables to the email theme by implementing
    hook_webform_per_email_mail_alter(&$vars)
  * Users can click on the link in the emails to access the webform only once.
    The link becomes invalid once the webform is submitted.

HOOKS
-------------
  You can use hook_webform_per_email_mail_alter(&$vars) to add variable to the
  body and subject templates for the emails.
 
MAINTAINERS
-----------
  Current maintainers:
  Jaskaran Nagra - https://www.drupal.org/u/jaskaran.nagra
------------------------------------------------------------------------------
