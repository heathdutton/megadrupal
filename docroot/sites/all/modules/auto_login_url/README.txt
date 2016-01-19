README.txt
==========

This is a module that creates auto login URLs on demand or with tokens.

You may use
auto_login_url_create($uid, $destination, $absolute = FALSE)
To create an auto login link for a user.
Or
auto_login_url_convert_text($uid, $text)
To convert all links of a text to auto login for this user

Also there are two tokens:
['tokens']['user']['auto-login-token']
['tokens']['user']['auto-login-url-account-edit-token']

This may be used in mass emailing modules or anywhere user tokens are available.


AUTHOR/MAINTAINER
======================
Thanos Nokas(Matrixlord) (https://drupal.org/user/1538394)
