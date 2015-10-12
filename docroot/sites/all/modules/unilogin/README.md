UNI•Login module for Drupal
===========================

This module provides allows you to create and authenticate Drupal users
through the external “[UNI•Login][]” service.

UNI•Login is a danish SSO service that is shared between educational institutes.


How does it work?
-----------------

1.  The module provides a block with a link to log in via UNI•Login.
2.  When the user clicks the link, he is redirected to the UNI•Login
    login page.
3.  Once logged in to UNI•Login, the user is redirected back to the
    Drupal site.
4.  This module verifies the login ticket returned from UNI•Login, and
    if valid, logs the user in, creating a new Drupal user in the process.


Install instructions
--------------------

1.  Read [UNI•Login documentation][], and if neccessary,
    the [technical description][].
2.  Fill in settings provided by UNI•Login in the configuration form.
3.  Activate UNI•Login block beneath the normal login form.

[UNI•Login]: http://www.uni-c.dk/produkter/support/uddannelse/uni-login/index.html
[UNI•Login documentation]: http://www.uni-c.dk/produkter/infrastruktur/brugere-index.html
[technical description]: http://www.uni-c.dk/produkter/infrastruktur/uni-login/technicaldescriptionofuni-loginforwebbasedapplications.pdf
