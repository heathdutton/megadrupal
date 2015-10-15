Summary
=======

   13 - Description
   27 - Requirements
   48 - Quick installation
   63 - Step-by-step installation
  118 - Making the client user friendly
  153 - Security considerations
  162 - Support


Description
===========
This module allows to run Drush without being granted access to the machine it relies on.
It may be useful on shared hosts that do not provide SSH access to servers.

It provides 2 functionalities:
 * Server: exposes Drush as a webservice secured with OAuth.
 * Client: extends Drush by providing a client able to communicate with above mentioned webservice.

Example:
 * With the traditional way, one connects to a server using SSH and runs the following: 'drush pm-enable field'.
 * With this module, one uses Drush directly on a workstation and runs the following: 'drush @mysite web pm-enable field'.


Requirements
============
I - Server
----------
 * ctools
 * features
 * oauth
 * services
 * libraries
 * PHP not in safe-mode (if PHP < 5.4.0)
 * Drush 4.0.0+
 * Drush works best on Unix-like OS (Linux, OS X)

II - Client
-----------
 * PHP 5.3.3+
 * PHP compiled with curl
 * Drush 5.0.0+
 * Drush works best on Unix-like OS (Linux, OS X)


Quick installation
==================
1. On the server, enable drushweb and oauth_common_providerui.
2. On the server, install Drush as a library so this path is valid: site/all/libraries/drush/drush.info.
3. On the server, create a user with role 'Drush Webservice OAuth user'.
4. On the server, go to /user/[nid]/oauth/consumer and add a consumer with 'http://example.net' as a callback. Note the key and the secret.
5. On the client, enter:
     drush web pm-enable field
     --uri=<your-server-uri>
     --oauth_key=<your-oauth-key>
     --oauth_secret=<your-oauth-secret>
   The result of 'drush pm-enable field' should show up.
6. Don't forget to check the sections 'Making the client user friendly' and 'Security considerations'.


Step-by-step installation
=========================
I - On the server: enable drushweb
----------------------------------
1. Copy the drushweb/ directory to sites/all/modules/.
2. Sign in a administrator.
3. Go to /admin/modules and enable the module 'Drush Webservice'.

II - On the server: install Drush as a library
----------------------------------------------
1. Go to /admin/reports/status and note that 'Drush as a library' is not detected.
2. Download Drush from https://github.com/drush-ops/drush.
   You may select the branch/tag you wish and then click on the 'Download ZIP' button.
   e.g. https://github.com/drush-ops/drush/archive/7.x-5.9.zip
3. Unzip the Drush archive in site/all/libraries/.
   The 'drush.info' file must be located at sites/all/libraries/drush/drush.info.
4. Go to /admin/reports/status and note that 'Drush as a library' is now detected.

III - On the server: generate oauth credentials
-----------------------------------------------
1. Go to /admin/people/create and create a user with these values:
   * Name => 'Drush Webservice OAuth User' (suggestion)
   * Password => (whatever, it won't be used)
   * Email => (whatever, it won't be used)
   * Status => active 
   You may also use an existing user, but this is not recommended.
2. Grant the user the role 'Drush Webservice OAuth user'.
3. Go to /admin/modules and enable the module 'OAuth Provider UI' (from project OAuth).
4. Go to /user/[nid]/oauth/consumer (or edit the user and go to the 'OAuth consumers' tab).
5. Click on 'Add consumer'.
6. Use these values:
   * Consumer name => 'Drush Webservice client' (suggestion)
   * Callback => 'http://example.net' (it won't be used)
   * Application context => 'Drush Webservice'
7. Go to /user/[nid]/oauth/consumer (or edit the user and go to the 'OAuth consumers' tab).
8. Edit the consumer ('Drush Webservice client').
9. Note the key and the secret.
10. You may disable the module 'OAuth Provider UI'.

IV - On the client: Test a command
----------------------------------
1. Install Drush as described on https://github.com/drush-ops/drush.
2. Copy the drushweb/ directory to ~/.drush/ so the client can be called from anywhere in the filesystem.
   You may also copy drushweb/ to /usr/share/drush/commands/ to make it available system-wide.
3. Clear Drush's caches: 'drush cache-clear drush'.
4. Have a look at the command help: 'drush help web'.
5. Test a command:
     drush web pm-enable field
     --uri=<server-uri>
     --oauth_key=<oauth-key>
     --oauth_secret=<oauth-secret>
   The result of 'drush pm-enable field' should show up.
6. Don't forget to check the section 'Making the client user friendly'.


Making the client user friendly
===============================
I - Global deployment
---------------------
The 'drush web' command becomes available using any of these contexts:
 * Available inside a Drupal instance when drushweb module is enabled.
 * Available anywhere in the filesystem when drushweb/ folder is in ~/.drush/.
 * Available system-wide when drushweb/ folder is in /usr/share/drush/commands/.
Copying drushweb/ to ~/.drush/ is probably the most convenient and secure way to use this module.

II - Interactivity
------------------
This module does not support interactivity, which means you can only send a command and see its result.
If a command prompts additional data or asks for a confirmation, 'no' will be inserted by default.
For this reason you will probably often use Drush's '--yes' option with this module.
Be sure to understand the implication of using the '--yes' option.

III - Aliases
-------------
The 3 required options (--uri, --oauth_key, --oauth_secret) can make this module a bit tedious.
A convenient way to get rid of this is to use Drush aliases.
An example of alias file can be found at http://drush.ws/examples/example.aliases.drushrc.php.

Example: if your '~/.drush/aliases.drushrc.php' file looks like this:
  <?php
  $aliases['mysite'] = array(
    'uri' => 'http://example.net/mysite', // Your server URI.
    'oauth_key' => 'y4ZYMrw8VNMDsnkibMxwu8ZybS4GPzHV', // Your OAuth key.
    'oauth_secret' => 'JHjnXtfpLxrPCVwnjyn8dCQmsxNmgfEZ', // Your OAuth secret.
  );
then this command: 'drush web pm-enable field --uri=http://example.net/mysite --oauth_key=y4...HV --oauth_secret=JH...EZ'
can be simplified like this: 'drush @mysite web pm-enable field'.
Note that the order matters: '@mysite' must come right after 'drush' on the command line.


Security considerations
=======================
Keep in mind, despite the fact that it is secured with OAuth, that this module performs system() calls and exposes them to the Internet.
It is strongly recommended to observe the following:
 * Only run this module on HTTPS connections. Otherwise your OAuth credentials will be sent as clear text on the Internet. Running this module on HTTPS connections can be achieved by installing the 'securepages' module and adding 'drushweb/drush/execute' to the list of secure pages. You should accordingly set an https uri on the client.
 * Do not grant new permissions to role 'Drush Webservice OAuth user'. Hardening this role's permissions is a warranty that it can do nothing except executing Drush.
 * For similar reasons, use a dedicated user to generate OAuth credentials.


Support
=======
Fengtan
http://drupal.org/user/847318
