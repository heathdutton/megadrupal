Allows any entity to be accessible by its own subdomain by specifying a
subdomain name in a field on the entity.

All access to the specified subdomain will show the entity, and all generated
links to the entity will link directly to the subdomain of the entity.

To minimize the amount of database lookups when doing checks on the requested
domains and when processing outbound links, all used Drupal URLs and their
eventual subdomains are cached as soon as they are hit.

Prerequisities:

To use, edit your settings.php file and find the line:

  # $cookie_domain = '.example.com';

Remove the # and change the value to the domain name of your drupal site.
After this, you'll need to log in again on the site.

Also update your DNS and edit your webserver's vhost entry to allow wildcard
aliases by adding a server alias. It should look like this:

  <VirtualHost *:80>
    ServerName example.com
    ServerAlias *.example.com

Usage:

Then just add a field of the type "Subdomain Field" to any entity in the
system and edit an entity and enter a desired subdomain in the field.

The entity is now available by that subdomain.

IMPORTANT: DO NOT add more than one Subdomain Field instance to the same entity
bundle and DO NOT use a cardinality other than 1 in the field instance 1.
