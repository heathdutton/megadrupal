INTRODUCTION
------------
The User recurring subscription module create and manage user recurring 
subscription profile. To create recurring profile user has to fill their
credit card information. The PayFlow Pro API is used in this module to 
create and manage recurring profiles. Other module may also the alter 
subscription field using hook_user_recurring_subscription_recurring_alter().
* For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/ratanphp/2304373

REQUIREMENTS
------------
This module requires the PayFlow Pro merchant account credentials.
https://registration.paypal.com/welcomePage.do?mode=try

INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
* Configure PayFlow Pro merchant account details under payment settings.
  (admin/config/subscription/setting)
* Create subscription plan under "Add subscription Plan".
  (admin/config/subscription/add)
* Manage subscription plan under config/subscription/manage
* Setup the cron duration for updating of recurring profile.
* Create user roles for grant and revoke user role as per the subscription 
  plan purchase and failure of recurring renewal.
   

FAQ
---
Q: How I can setup PayFlow Pro sandbox account? 
A: Please see this link
   https://registration.paypal.com/welcomePage.do?mode=try

Q: How I can test sandbox integration?
A: On user registration form you have to enter sandbox credit card deatils.
   The sandbox credit card detail is:
     CC Number : 378282246310005
     CVV :4685
     Expire : 02/17

MAINTAINERS
-----------
Current maintainers:
* ratanphp - https://www.drupal.org/user/458884
* er.pushpinderrana - https://www.drupal.org/u/er.pushpinderrana
