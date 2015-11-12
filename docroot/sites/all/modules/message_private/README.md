CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Dependencies
 * Installation
 * Configuration
 * How to use
 * Security
 * Development and Test
 * Maintainers

INTRODUCTION
------------
A message type and entity reference fields, enabling sending and receiving 
private messages using The Message Stack. Messages of type "Private Message" can 
be sent by creating the private_message message instance with fields referencing 
user entities or OG group entities using the submodule message_private_og.

The message_private module includes the following.
+ A message type "Private Message" with entity reference field referencing users
+ A message view, message_private for "User Messages"

The sub-module message_private_og includes the following.
+ An entity reference field for groups to associate messages to groups
+ A "Group Messages" display for the message_private view


DEPENDENCIES
------------
The message_private module requires the following modules:
 * Message (https://drupal.org/project/message)
 * Message UI (https://drupal.org/project/message_ui)
 * Message Notify (https://drupal.org/project/message_notify)
 * Entity API (https://www.drupal.org/project/entity)
 * Entity Reference (https://www.drupal.org/project/entityreference)
 * Entityreference Prepopulate 
   (https://www.drupal.org/project/entityreference_prepopulate)
 * Ctools (https://www.drupal.org/project/ctools)
 * Views (https://www.drupal.org/project/views)
 * Token (https://www.drupal.org/project/token

The message_private_og module requires the following modules:
 * OG (https://drupal.org/project/og)


INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * If using message_private_og, make sure you have created an OG group content
   type named "Group". Otherwise, you will need to manually configure the entity 
   reference field to use your custom group content type.


CONFIGURATION
-------------
Enabling Permissions: 
"View a new message instance for Private Message" &
"Create a new message instance for Private Message" for users needing to create
and view private messages.
"View user profiles" for users needing to send messages to other users. This is
due to issue: 'Restricted access results with user entityreference' - #2153463.
This patch may also help if committed in future: #2247937
"Administer message types" permission shows the user all private messages on the 
admin/content/message view by default. To hide private messages from this view, 
you must override view provided by Message module:
admin/structure/views/view/message/edit

When enabled, the module will provide a new message type "Private Message" and a 
Message Private View.

Email Notifications:
Email notifications are sent using message_notify module to users referenced by 
the field_message_user_ref field and for all group members of the groups 
referenced by field_message_group_ref if using message_private_og. Users can
disable email notifications using the checkbox field 
field_private_message_notify attached to the user entity, on their profile. The
message notifications can be turned off globally on the module settings form.

Message Create Limits:
Message creation limits can be managed per role on the module settings form. A
message create limit can be set per interval per role. Users with more than one
role get the maximum limit by calculating the lowest time per message over each 
role. Users with the 'bypass private message access control' permission bypass
these limitations.

Message Private OG:
When using the Message Private OG sub-module, the Group Messages display has
a contextual filter which filters by group ID of the logged in user. This is
developed taking into account that a content type called "Group" exists for all
Group instances. Anything other than this setup requires user customisation. You
must manually set the field_message_group_ref contextual filter to use group
bundle to enable group filter on views.

HOW TO USE
----------
To Create messages:
Visit /admin/content/message/create/private-message and Save the message to send
or
Visit the "Messages" tab detailed below and find the "Create a new message" 
local action.

To View inbox, sent and group messages:
Visit your user page at /user and find the "Messages" tab which displays 
received messages (Inbox local task), the "Sent" local task under that tab which
displays sent messages and the "Group" local task which displays group messages.
  /user/USER_ID/messages/inbox
  /user/USER_ID/messages/sent
  /user/USER_ID/messages/group

SECURITY
--------
This module does not come with any security features out-of-the-box, but you can
easily configure your own, using methods and modules of your choice.

Examples:

Honeypot and timestamp methods: https://www.drupal.org/project/honeypot
CAPTCHA method: https://www.drupal.org/project/recaptcha


DEVELOPMENT AND TEST
--------------------
 * Threads will be created: https://www.drupal.org/node/2504863. 
   An admin setting can be used to enable or disable threads to turn on/off 
   replies on all private messages (Reply, Reply All). Perhaps a css file will 
   need to be added for indentation / presentation. Integrate with MessageJS,
   nodeJS or socket.io.
 * Flag module on user entity to block/unblock users from messaging them
 * Flag module on message entity to show/hide (delete) messages from users own 
   display
 * Allow Operations links to display correctly on views, i.e. - show 'View' for
   users with view permissions. Not showing currently due to custom permissions.
 * Integrate with rolereference module or similar to provide sending to users
   within a certain role.
 * Research the benefits of Rules module integration and what could be provided.
 * Tests should be created in a "tests" folder.

MAINTAINERS
-----------
Current maintainers:
 * Paul McCrodden - https://www.drupal.org/user/678774
