Login Disable
=============

Prevent users from logging in to your Drupal site unless they know the secret key to add to the end of the login form
 page. ( default: http://example.com/?q=user/login&admin )

If your site has clean urls enabled you may use http://example.com/user/login?admin instead.

If a user does find out about the secret key they will still have their user account role checked during
authentication. If they do not have the 'bypass disabled login' granted they will be refused access and displayed a
customisable "denied" message.

Credits
Developed by Budda / Mike Carter @ Ixis