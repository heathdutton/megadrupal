
Dev docs
====


Hooks
----

Node registration doesn't define any hooks. No lists are required for any
of its features.


Alters
----

Node registration creates a few `drupal_alter` interfaces. `drupal_alter`
rules, because it allows existing data to be altered by other modules
**and** themes.


### node_registration_default_values

Executed in the scope of the registration form. Goal: allow modules & themes
to set default values. E.g. the user's phone number or name. The e-mail
address is entered automatically by the module itself. **Only executed for
new registrations.**

Arguments:

* Array `&form` The Drupal form array
* Array `&values` Empty assoc default values for simple fields (text, email etc)
* Array `context`
  * stdClass `node`
  * NodeRegistrationEntity `registration`
  * stdClass `user`


### node_registration_submit

Executed in the scope of the registration form **validation and submit handler**.
Goal: allow modules and themes to add custom validation **and** to alter the
registration object before it's inserted. Likely example: set the `slots`
property based on form input values (like guests).

Arguments:

* NodeRegistrationEntity `registration` To use and alter
* Array `context`
  * Array `form` The original Drupal form array
  * Array `form_state` The original Drupal form_state array
  * stdClass `node`
  * stdClass `user`
* Array `&errors` Empty assoc array of form errors, keyed by field name


### node_registration_email

Executed inside `node_registration_send_broadcast` **if** an `alter` option was passed
to it. The alter option will be passed to the alter function. A specific alter is also
possible: `node_registration_email_ALTER_TYPE`.

Arguments:

* Array `&options` The options passed to `node_registration_send_broadcast` including `alter`
* Array `context`
  * stdClass `node`
  * NodeRegistrationEntity `registration`

To change the e-mail's recipient, alter `$registration->email`. (Will not be saved!) To
change the BCC, alter `$options['bcc']`.

See

* `node_registration_send_broadcast`
* `_node_registration_send_email`
* `node_registration_mail`


### node_registration_type_actions

Executed in the scope of Node registration admin page: `admin/structure/node_registration`.

See `node_registration.module`.


### node_registration_table_registrations

DEPRECATED in favour of Views. If you don't have Views (w000000t), you can still
use this. If you do have and use Views, you can enable the "Node registrations" view
and it will replace the admin page.

See `node_registration.module`.


