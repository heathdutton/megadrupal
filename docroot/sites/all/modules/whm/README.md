## WebHost Manager (WHM) Integration for Drupal 7

* Original Author: Joe Turgeon [http://arithmetric.com]
* Current Author: Mikey O'Toole [http://mjco.ltd.uk]
* Original Sponsorship: Sundays Energy [http://sundaysenergy.com]
* Current Sponsorship: MJCO [http://mjco.ltd.uk]
* Licensed under GPL v2 or later

WebHost Manager (WHM) is a commercial server administration system
that manages hosted cPanel accounts. This module provides a user and
program interface to some of WHM's reseller functions, enabling Drupal
administrators, authorized users, or other modules to list, create,
and (un)suspend cPanel accounts.

This project is not authorized or supported by cPanel. For information
on the cPanel or WHM products, see:
http://www.cpanel.net

### Installation

Download the latest release of this module from:
http://drupal.org/project/whm or http://mjco.github.io/WHM

Uncompress the archive in your modules directory (sites/all/modules or
sites/[name]/modules).

In Drupal, go to the modules page (Site building >> Modules) and
enable the WHM module listed under the Hosting category.

To use the module, it must be configured with a WHM host and reseller
account. Go to the module's settings form (Admin > WHM or example.com/admin/whm)
and enter the hostname of your WHM server, the username for a reseller
account, and its remote access key. The remote access key is generated
by WHM, and can be found under 'Setup Remote Access Key' in the WHM
interface. Note: In WHM, the reseller account must be granted specific
permission to list, create, and suspend accounts.

### Features

This module enables three WHM functions through a user and programming
interface: listing, creating, and (un)suspending accounts managed by
the specified reseller. These functions are accessible to authorized 
users through the Administer >> WHM menu.

The list accounts page displays the username, domain name, and hosting
plan. Additionally, it allows accounts to be suspended or unsuspended.
The create account page allows a new account to be created with a
specified username, contact e-mail address, password, domain name, and
hosting plan.

User access to these functions is limited by the permissions set in
Admin > People > Permissions under the WHM heading. Access can be
granted by function (e.g: to list accounts, to create a new account, or 
to (un)suspend accounts).

Additionally, this module provides a direct programming interface to
these WHM functions. Note: There are no access restrictions to the
functions, as it is expected that programs utilizing these functions
would implement their own permissions system.
