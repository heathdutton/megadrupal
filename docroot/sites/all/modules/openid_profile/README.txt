; $Id $

OpenID Profile
==============


DESCRIPTION
-----------

The OpenID Profile Module maps OpenID AX fields to the user and drupal core's profile 
Module. It can be used either on an OpenID relying party (client) or an OpenID provider.

OpenID Provider: A site that provides OpenID authentication.
OpenID Client:   Also called "relying party" or "consumer". A site that a user 
                 logs into using OpenID.
OpenID fields:   A field according to the OpenID Simple Registration standard 
                 or the OpenID Attribute Exchange Standard.


REQUIREMENTS
-------------

OpenID Clients

* OpenID (core module) - provides basic OpenID authentication for OpenID clients.
* OpenID Client AX - implements OpenID Attribute Exchange.


OpenID Providers

* OpenID Provider - provides OpenID authentication to other sites.
* OpenID Provider AX - implements OpenID Attribute Exchange provider side.


INSTALLATION
-------------

1) Install and configure requiered modules.
2) Map OpenID-AX fields to user's data or profile fields on both parties
   (provider and client)

After these mappings are defined: 

* On the Client newly logged in users will have their data and profile 
  populated with fields form the OpenID provider.
* The OpenID provided by this site is being used, Drupal will expose OpenID AX fields 
  according to the mapping.


CREDITS
---------------
Written and maintained by:
Stefan Auditor (sanduhrs) [1] and Felix Delattre (xamanu) [2]

Sponsored by:
Erdfisch [3]

Inspired (and initially copied) by OpenID Content Profile Field [3]

[1] http://drupal.org/user/28074
[2] http://drupal.org/user/359937
[3] http://www.erdfisch.de
[4] http://drupal.org/project/openid_cp_field
