README
======
The zendesk module allows you to integrate your drupal site with the zendesk
customer support system (http://www.zendesk.com)

Zendesk allows external authentication. This means that your Drupal site can serve
as authentication service. This way customers have a seamless experience without
having to log in in both the drupal site and the zendesk support system.

Installation is easy:
- install the module
- configure your zendesk account (go to 'Settings - Security - Single Sign-On')
  * Enable the "JSON Web Token" strategy.
  * insert the remote authentication url: http://yoursite.com/services/zendesk
  * optionally you can insert your logout url: http://yoursite.com/logout
- go to admin/config/people/zendesk and fill in the url of your zendesk support page (e.g. http://yourdomain.zendesk.com) together with the secret key

Local Development
=================
Zendesk remote authentication does not require your site to be public.


Zendesk PHP library
===================
This module depends on the Zendesk PHP library. For ease of use, this library is included with the module in the lib directory.
For more information on this library, please visit: http://code.google.com/p/zendesk-php-lib

=================
Originally developed for 6.x by twom <http://drupal.org/user/25564>
Ported to Drupal 7.x by markwk <http://drupal.org/user/1094790>
