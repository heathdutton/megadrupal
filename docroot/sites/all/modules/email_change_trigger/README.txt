CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Configuration


INTRODUCTION
------------
Email Change Trigger creates a trigger and an action that will allow an email
to be sent notifying the user that their email has changed. This adds some
extra security from unauthroized email changes.

INSTALLATION
------------
Install as usual, see http://drupal.org/node/70151 for further information.


CONFIGURATION
-------------
First we need to configure an action for this trigger.
Configuration > Actions (admin/config/system/actions)
- At the bottom of the page Create an advanced action
- Select 'Send email address change notification email...' and click Create
- Add the message you would like to send to both the old and new addresses.
- Click Save

Next, head to the Triggers page and click on the User tab
Structure > Triggers > Users  (or admin/structure/trigger/user)

Under TRIGGER: AFTER CHANGING USER EMAIL 
- select "Send email address change notification email".
- click 'Assign'

You are finished!
