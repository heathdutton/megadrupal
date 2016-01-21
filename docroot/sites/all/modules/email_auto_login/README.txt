-- SUMMARY --

When Drupal sends an email to a user, this module changes links so that said
user is automatically logged in if they click any of the links in the email.

For example, if you send them a subscription notification,
if they click on the link to the original post,
they will arrive at your website already logged in.

It works by appending a one-time login token to the end of each link.
The login token expires when it is used, or when 30 days passes.
In the case that it has expired, the user needs to log in in the normal way.

-- CREDITS --

Dmitry Demenchuk (mrded)  - http://demenchuk.me
Chris Muktar (chrism2671) - http://chriscentral.com
