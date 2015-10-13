Pluggable Mailers

Pluggable Mailers makes it easier for developers to write new email services for
Drupal, by providing an object-oriented wrapper around Drupal's hook_mail()
implementation.

Making a new pluggable mailer is easy, just create a new Ctools plugin of type
"pluggable_mailer", and implement the PluggableMailInterface. You can also
extend the base PluggableMailPlugin to get access to readily defined methods
for common tasks. A default mailer is included.

Pluggable mailers can also be used as a wrapper around the default Drupal mail
functionality, allowing you to send an email with just one function
(no hook_mail()) required.

API

\Drupal\PluggableMailers\PluggableMailInterface
Defines base functionality for a mail class, including setting of parameters,
choice of mail system, and preparing template variables.

\Drupal\PluggableMailers\PluggableMailPlugin
Defines some default implementations of the PluggableMailInterface.

pluggable_mailers_send_email($email_address, $params = array(), $plugin_name =
'default', $from_address = NULL)
Send an email to an email address.

This module was developed with the support of PreviousNext.
