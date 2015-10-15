
http://drupal.org/project/cosign

Installation instructions
---------------------------
* you must be running your server within a cosign infrastructure.
	More info at: http://www.weblogin.org/
* download the cosign module
* read this README file, paying close attention to the Warning at bottom
* copy the modules into your drupal module directory
* enable the module through the admin/modules page

Htaccess
---------
If your Drupal site is installed in the web root of the server, you'll likely
need to add a line to your .htaccess file for the root directory of the Drupal
site.

If you have Drupal installed at the DocumentRoot level (/) and are using Clean
URLs, add the line

  RewriteCond %{REQUEST_URI} !=/cosign/valid

to the Clean URL rewrite rule to prevent the web browser from looping. Cosign
usese this path, and thus you need to add an exception so that Drupal does not
think this path is a Drupal path. If you are seeing the symptom whereby you're
running into an infinite redirect problem when logging in, this is likely the
cure.

For a stock Drupal 7 site's .htaccess file that has not been modified, your new
rule will look like this:

  # Pass all requests not referring directly to files in the filesystem to
  # index.php. Clean URLs are handled in drupal_environment_initialize().
  RewriteCond %{REQUEST_FILENAME} !-f
  RewriteCond %{REQUEST_FILENAME} !-d
  RewriteCond %{REQUEST_URI} !=/favicon.ico
  RewriteCond %{REQUEST_URI} !=/cosign/valid
  RewriteRule ^ index.php [L]


Configuration
--------------
The configuration page can be found at: /admin/config/system/cosign

Pay particular attention to the settings as they dictate key behavior regarding
how Cosign module will behave. Of particular importance are the following
settings:

* Logout Path: path to the Cosign logout script to use. Note that when using the
  local logout script (default), the user's service cookie will be destroyed
  immediately before being redirect to central Cosgin for logout, whereas when
  using the central Cosign server logout path the user's service cookie will
  remain active for up to a minute.

  The reason for this has to do with the architecture of Cosign. On every HTTP
  request, the local Cosign server checks the user's service cookie to see if
  it's more than 60 seconds old. If it is, it checks with the central Cosign
  server to see if the user is still logged in to Cosign. If the user is, the
  local Cosign server generates a new service cookie for the user.

  If the user is logged out of the central Cosign server, it may take the local
  server up to 60 seconds before it will check back in with the central Cosign
  server, and find out that the user is no longer authenticated.

* Logout users from Drupal when their Cosign session expires: Under certain
  circumstances it is possible for the user to logout of Cosign through a
  different Cosign protected application and still be able to access Drupal
  permissions protected pages because Cosign module does not automatically
  log the user out of Drupal when it no longer detects a remote_user from the
  service cookie. Whether or not this is possible depends on how your local
  Cosign server is configured and how Drupal is configured.

* Redirect for users without a Drupal account: A path or full url of the page
  to redirect to if the user authenticates through Cosign successfully but does
  not have a corresponding Drupal account. This setting is only relevant if you
  have selected 'No' for either the 'Auto-create Users' setting or the 'Allow
  friend accounts' setting. If either is set to 'No', it becomes possible for
  the user to be unable to login to Drupal. Along with this path for the
  redirect, both of these settings have their own message setting that allows
  you to customize the explanatory message displayed to users when they run into
  one of these conditions.

Warning
--------
The Drupal 7.x-1.x version of Cosign module does not include the method of
copying existing users to the authmap table. This means that this module should
be used with great care as Drupal usernames will be matched with Cosign login
names automatically when they login. Therefore, if a local Drupal username
matches a Cosign username, but the two usernames are for two different people,
the Cosign username holder will affectively take over the Drupal user account.

For example, I may have chosen the username 'willie' for my local
drupal user. Although when using cosign, my username is 'willn'. In
this case, by turning on cosign I've lost access to my former username,
and another user would have access to all of my previous documents,
comments, preferences, etc.  A more serious situation would be if your
admin user doesn't share your username.  If you turn on cosign without
changing your admin username, then you suddenly lose your
administration account.

Be sure to vet your user list before "opening the doors" to
your userbase. It may be wise to only enable cosign authentication
on a brand new drupal installation. Pay particular attention to the username of
user 1. It should match the Cosign username of the person who should have full
administrative access to the site.

---------------------------------
Written by Willie Northway and Kevin Champion
