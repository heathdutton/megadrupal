Description
--------------------------------------------------------------------------------
This module works in conjuction with the Recurly module to add/remove a role(s)
from a user account depending on the status of a recurly subscription.

The module responds to ping backs from Recurly regarding the status of new or
existing subscriptions making it easy to create a subscriber role on your site
and have people be added/removed from that role as necessary.

Requirements
--------------------------------------------------------------------------------
- Recurly module: http://drupal.org/project/recurly
- An active Recurly.com account

Installation and Configuration
--------------------------------------------------------------------------------
After the Recurly module is installed you can navigate to
admin/config/services/recurly under the Recurly account section choose 'User'
from the select list under "Send Recurly account updates for each:" which will
expose a new set of options. Here you can configure the list of roles that
should be added/removed when a new subscription is created or an existing one
is either cancelled or expires.

There is also a new option to display subscription plans on the user
registration form. If you turn this feature on all currently enabled plans will
be displayed at the bottom of the user registration form and a user will be able
to choose one during registration. After their Drupal account has been created
they will be directed to the page to complete their Recurly subscription based
on the selected plan. Note that this does not guarantee that someone subscribes
to a plan when creating an account but it does make it easier to get them going
in the right direction.

Support
--------------------------------------------------------------------------------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/recurly_roles?categories=All
