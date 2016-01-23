Description
-----------
This module provides elements for Rules, which will allow you to setup
confirmation of emails, that are sent to your website by using entityform.

This module isn't "plug and play", after enabling it, you will need to create
rules that will do the magic. It only provides:

Action: Generate confirmation URL
Event: User visited confirmation URL

Sending of confirmation e-mail will happen in Rules - after event of saving
new entityform submission:

1. Trigger action that will generate confirmation URL.
2. Put confirmation URL in content of e-mail message and send it to given
   e-mail in another action.

To mark e-mail address as confirmed:

1. Create on your entityform field, that will be hidden to visitors (you can
   use for example field_permissions for this), that will store information
   about our e-mail being confirmed or not.
2. Simply switch it in your rule reacting on event "User visited confirmation
   URL". After that you will be able to filter your submissions by e-mail
   confirmation status in Views, remove submissions with not confirmed e-mails
   at cron run with Rules etc.

Examples of useful things that you can do with this module and Rules:

* Confirm multiple e-mails.
* Send different confirmation e-mail based on role, language etc.
* Redirect user to custom node after confirmation.
* Make confirmation URLs to expire.
* A lot more...

Dependencies:
-----------
* Entityform
* Rules
