-- SUMMARY --

The mydigipass module allows to easily integrate the MYDIGIPASS.COM service in
a Drupal website. MYDIGIPASS.COM is an authentication service which uses
two-factor authentication and relies on one-time passwords to protect access
to your website.
By enabling this module on your website, your end-users can authenticate
themselves using MYDIGIPASS.COM instead of using a static password. The module
is flexible to allow for existing users to link their Drupal account with a
MYDIGIPASS.COM account and for new users to register a new Drupal account and
to link it immediately to their MYDIGIPASS.COM account.

For a full description of the module, visit the project page:
  http://drupal.org/project/mydigipass

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/mydigipass


-- REQUIREMENTS --

This module does not rely on other modules. However, the webserver on which the
Drupal website is hosted must be able to make connections to the MYDIGIPASS.COM
service on the Internet. If you don't know whether your webserver can make such
connections, a "Test connectivity" button has been foreseen in the module's
administration pages which allows to check whether it is possible to make
connections to MYDIGIPASS.COM or not. In order to connect to MYDIGIPASS.COM,
the module uses the same functionality as used by Drupal to check for updates.

When using this module on a production website, it is strongly recommended that
your website is reachable over https.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* In order to be able to use the MYDIGIPASS.COM service, you need to have your
  website registered at MYDIGIPASS.COM. For testing purposes, you can create
  a free account on https://developer.mydigipass.com/ and link your website.
  This will allow you to test the integration with MYDIGIPASS.COM. For
  production purposes, you need to contact MYDIGIPASS.COM to have your website
  registered in the production instance of MYDIGIPASS.COM.

* When registering your website at MYDIGIPASS.COM you are asked to submit the
  callback URL of your website. The default location for the callback when
  using this module is:

  - /mydigipass/callback          (when clean URLs are enabled)
  - /?q=mydigipass/callback       (when clean URLs are disabled)

  Therefore if your website's domain is www.example.com, then your callback URL
  is the following when you are using clean URLs:
  https://www.example.com/mydigipass/callback

* Configure the MYDIGIPASS.COM settings in
  Administration > Configuration > Web Services > MYDIGIPASS.COM:

  - Select whether you are using the sandbox instance or the production
    instance of MYDIGIPASS.COM.

    If you registered your website yourself on MYDIGIPASS.COM developer's
    website, then you are using the sandbox instance. If you had contact with
    MYDIGIPASS.COM and they registered your website for you, then you are using
    the production instance.

  - Enter your client_id and client_secret.

    You received these details from MYDIGIPASS.COM after registering your
    website.

  - Enable the integration by ticking the "Enable MYDIGIPASS.COM integration"
    checkbox.


-- CUSTOMIZATION --

* After enabling the module, the MYDIGIPASS.COM button is automatically added
  to the login form, to the registration form and to the user's profile. The
  latter allows existing users to link their Drupal account to MYDIGIPASS.COM.
  The style of the shown buttons can be configured in
  Administration > Configuration > Web Services > MYDIGIPASS.COM > Button style


-- TROUBLESHOOTING --

* If the module can't connect to MYDIGIPASS.COM, use the "Check connectivity"
  button to check whether your website has the necessary PHP extensions enabled
  and whether the webserver can make outbound connections to MYDIGIPASS.COM

* If the module can't authenticate to MYDIGIPASS.COM, check the following:

  - Is the client_id and the client_secret appropriately set?

  - Has the proper instance of MYDIGIPASS.COM been selected (i.e. sandbox
    instance versus production instance).

  - Have you registered the correct callback URL at MYDIGIPASS.COM? For example
    you might have registered WWW.example.com/mydigipass/callback whereas you
    might currently be accessing your website without the WWW prefix (being
    example.com).


-- CONTACT --

Current maintainer:
* Lode Vanstechelman - http://drupal.org/user/657472
