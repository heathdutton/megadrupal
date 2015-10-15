MailDigest
==========

The MailDigest module allows administrators to setup templates for digests, which are sent periodically
to subscribed users by e-mail, when cron runs. Developers can implement tokens to be replaced when the digest
is evaluated. Those tokens can, for example, list the most commented nodes during the digest period.
All tokens are available when building the digest template.

This module depends on [CTools](https://drupal.org/project/ctools) and [Token](https://drupal.org/project/token).

Features:

* In order to create, edit, update or remove digests, the user must have the "administer maildigest" permission
* You can have multiple digests with different periodicity
* When a digest is removed, all subscriptions are removed also, and also the variables related to that digest
* The CTools API was used, so some methods from the API were implemented to load the records as objects
* Some helper methods were implemented, to make it easier to extend the module
* A user can choose to which digest he wants to subscribe to when his account is being created or updated. Only enabled digests are listed and can be subscribed
* When a user is removed, he is unsubscribed from all digests
* Possibilities for digest frequency are: monthly, weekly and daily
* For all frequency types, the admin must choose which time the digest is sent
* For the weekly frequency, he must choose on which week day the digest is sent
* For the monthly frequency, he must choose on which week day of the month the digest is sent (for example, "second Tuesday" or "first day" or "third Wednesday", for example (the module does the calculations based on this)
* A digest is sent for all subscribed users on the user's chosen language
* English is chosen when there is no template for the user's language
* Digests can be managed through "Site building" > "Maildigest Digests"
* It's possible to choose the input format for the digest template, so PHP code can be inserted by choosing the PHP Code input format, if available
* It's possible to use tokens in the digest template (token replacement is run before markup checking).
* Mail digest is sent as a multi-part message (both plain text and HTML), this way, all clients are supported
* After the digest is sent, the next sending time is calculated
* Digest templates can be exported to Features

Screenshots:

Administrative UI (creating a digest):

![UI](http://ca.ios.ba/files/drupal/maildigest-ui.png "UI")

Subscribing to a digest from the edit profile page:

![Subscription](http://ca.ios.ba/files/drupal/maildigest-user.png "Subscription")

Digest received through e-mail:

![E-mail](http://ca.ios.ba/files/drupal/maildigest-mail.png "E-mail")

Check the module on [Drupal.org](https://www.drupal.org/project/maildigest).

This module was fully sponsored by [Meedan](http://meedan.org).
