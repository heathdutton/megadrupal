Ces Message manages the sending of notifications to users.

Ces Message and notifications keep users to
they can see and manage them from the same web.

It also allows to decide whether you want the notification is sent to you by
email or just saved in database.

The decision can be made by sending the notice from the code or more
forward could be a user option to receive emails and that
kind. (Pending study).

Default templates
-----------------

The default templates are in includes/actions/.

Each language has its own folder and file names are
the action performed.

example:

- Includes/actions/en/account_activated.inc
- Includes/actions/ca/account_activated.inc

Implement a new action
----------------------

To implement a new action must be included in ces_message_install().

And add the template by default.

The module to manage is to be able to manage the tokens.
