For more information about this module please visit http://drupal.org/project/status_message

This module allows a user to set a custom Drupal message.


Contents of this file
---------------------

 * How to use
 * Dismiss messages


*** How to use
---------------------------------------

- Go to admin/people/permissions and set the permissions under "Status Message".
- Go to admin/config/system/status_message to create a custom message.


*** Dismiss messages
---------------------------------------

The users in your site are able to dismiss a status message by clicking on the
close button in the message. The message is dismissed by AJAX call and will
disappear without page reload.

If a logged in user dismisses the message, then the information is stored on the
database* with the current user id.

If an anonymous user dismiss the message, then an entry about it will be stored
in the database*, but the database entry id is stored as a cookie in the user's
computer, so we can identify that just that user has dismissed the message.

Each time the module settings are saved (see "How to use" in this document)
then the database table* is truncated so everyone will see the message again.

* The table is called "status_message_dismiss"

 
---------------------------------------
by Dhavyd Vanderlei - http://www.dhavyd.com