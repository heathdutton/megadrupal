NODEJS RULES CHATTERBOX
=========

Nodejs rules chatterbox allows users create real time notification blocks with the help of rules.

INSTALLATION
------------

1. Copy and paste the module in your sites modules folder.
2. Ensure you have enabled and configured the dependency modules.
   http://drupal.org/project/nodejs.
3. Enable the module at admin/modules.


USAGE
-----

1. After installation, create block channel from configuration page.
2. Create rules in admin with action to notify to nodejs channel. (admin/config/workflow/rules)
3. Get Notified!


API

1. Call nodejs_rules_chatterbox_notify($message,$uid = '') in your module to send a custom notification.
   @param StdClass $message
    ->channel - Channel name.
    ->message - Message.
   @param $uid
     Userid of sender. Default value is logged in user.

2. Implement hook_chatterbox_notify_alter($message) in your module, to modify the notification message before it get published.

Developer and Maintained by
Mark Koester mark@int3c.com
http://drupal.org/user/1094790/
Int3c.com | Custom Drupal Development
