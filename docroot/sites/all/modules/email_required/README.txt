EMAIL REQUIRED:
========================

This module is for site owners that do not want to require that users
validate their email address when creating an account - because we all
know that's extremely annoying for users to do. But, we still want to
try to minimize spam as much as possible.

With this module enabled, you should not be requiring email verification
upon user registration. This module allows you to set any number of 
page paths that will require the user to verify their email address in 
order to access. Some examples could be node/add/* or messages/new, etc. 
If the user has not yet verfied their email address, when they navigate 
to those pages, they will be redirected to a page where they can request 
a verification link.

Clicking that sends a tokenized link to the user's email address. Once
clicked, the user now has a validated email address. They will never have 
to do this again, unless they change their email address.

*Be aware* that anonymous users will never get redirected because we 
can't verify their email address. The purpose of this module is to limit
features that authenticated users can use until they verify their email
address. Act accordingly!

----

To get set up, install the module and configure it at:
 admin/config/people/email-required
 
There are permissions available that can allow you to exclude certain
roles from having to verify.

See email_recurring.api.php for API functions.
