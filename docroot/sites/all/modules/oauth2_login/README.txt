The module OAuth2 Login is used to allow the users of another Drupal
site to login to your site. This is done through OAuth2
authentication. On login the users are redirected to the login of the
other Drupal site, and after they login there, they are redirected
back to this site.

For this to work, the module OAuth2 Login Provider
(https://drupal.org/project/oauth2_loginprovider) must be installed on
the other site, and a client that corresponds to this site must be
registered there (on the path
'admin/structure/oauth2-servers/manage/oauth2/clients').  On that
client, you must set the Redirect URI to:
https://client.example.org/oauth2/authorized

See the file 'oauth2_login.make' for downloading the dependencies.
Copy 'libraries/hybridauth-drupaloauth2/DrupalOAuth2.php' to
'libraries/hybridauth/hybridauth/Hybrid/Providers/'.

The configuration of this module is done on:
'admin/config/people/oauth2_login'

The test file helps to understand the configurations that should be
done on the server and on the client:
https://github.com/dashohoxha/oauth2_login/blob/7.x-1.x/tests/oauth2_login.test#L22

Besides the module 'oauth2_login', there is also the sub-module
'oauth2_user', which can be enabled optionally. It provides some
functions that can be useful for managing the profile of the remote
oauth2 user. This module can be useful out-of-the-box, or it can just
serve as a template or example about how to manage the remote user
profile. For more details about the available functions look at
'oauth2_user.api.php'.
