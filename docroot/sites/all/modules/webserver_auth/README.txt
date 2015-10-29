Description
-----------
Admins can now enable access control their the Drupal directory via
their webserver (e.g. http auth, NTLM, .htaccess) and with this
module, Drupal honours the web server's authentication.

Install
----------
Enable access control on your your Drupal directory and
subdirectories. For Windows, you must turn 'windows integrated
authentication' for the Drupal directory in IIS (untested for 7.x
release) or use Apache SSPI module (see below).

Install this module as usual.

You probably want to disable the 'user login' block. If you choose to
leave it enabled, it will still work.
 
If you allow automatic registration in user module Admin, this module
automatically creates new users as they are encountered. otherwise,
you will have to create a user account manually before users can
login.  Note that if you are creating users beforehand you will need
to add them to the authmap table too.

IMPORTANT: The module config page has a few settings which control how
users will be created in your Drupal user table.  Ideally you want to
get these right before you open access up to everyone.  All the
options are described on the config page itself.

Configuring the Apache SSPI module
-----------
You may need to find and download the SSPI module .so file first.
In httpd.conf add the module:

  LoadModule sspi_auth_module modules/mod_auth_sspi.so

In Drupal's root .htaccess file, right at the bottom add the following
(replace "My Network Name" and "my.domain" as appropriate):

  # SSPI Auth settings
  #Options None
  Order allow,deny
  Allow from all

  AuthName "My Network Name"
  AuthType SSPI
  SSPIAuth On
  SSPIAuthoritative On
  SSPIDomain my.domain
  SSPIOfferSSPI On
  require valid-user

TODO
---------

- optionally grab elements from LDAP server (e.g. Windows Active
Directory) immediately after user login. this will auto populate
fields like phone number, birthday, etc.
  