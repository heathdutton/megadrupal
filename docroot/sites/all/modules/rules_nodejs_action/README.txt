README.txt
____________________


DESCRIPTION
____________________

This module provides rules action for nodejs notifications.
You can apply this action to any rules event.
You can set up subject and body with different
text or pattern in nodejs action, then define which users can see this message.
It can be multiple users roles or user`s IDs. 
User will be able to see message in the
left-bottom corner of the page on the event.
All these messages will appear in real-time. We are thankful node.js for this :)

REQUIREMENTS
____________________

1. Rules (https://drupal.org/project/rules).
2. node.js (https://drupal.org/project/nodejs).

INSTALLATION
____________________

You should do the following to install this module:

1. Extract the tar ball which you downloaded from Drupal.org.
2. Upload the rules_nodejs directory and all its contents to your modules
   directory.
3. Visit admin/modules and enable the "Rules with node.js" module.


CONFIGURATION
____________________

You should do the following to configure this module:

1. It is necessary to configure Node.js integration module in the correct way-
   (http://drupal.org/node/1713530).
2. Also you need to configure module Rules in the correct way too.
   (https://drupal.org/documentation/modules/rules).
3. Also in the list of actions you can select action node.js for each rule and
   configure action message.
