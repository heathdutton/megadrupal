
                               +++ mojeID +++

                              A Drupal module

mojeID Module allows site users to authorize and authenticate through mojeID.cz
service which is based on OpenID protocol, but has some additional features:
 - user home address is physically validated
 - extended attribute exchange support (e.g. billing address, shipping address)

This module contains basic support for mojeID authentication and mojeID icons,
which let site users know that mojeID is available.

________________________________________________________________________________

INSTALLATION
________________________________________________________________________________

1. Check Requirements and Recommended modules first.

2. Configure at admin/settings/mojeid

3. Ready for action!

________________________________________________________________________________

REQUIREMENTS
________________________________________________________________________________

 - OpenID module (Drupal core optional module)

________________________________________________________________________________

OPTIONAL
________________________________________________________________________________

 - OpenID AX Client module http://drupal.org/project/openid_client_ax

 - UC Addresses http://drupal.org/project/uc_addresses

!!! Note: OpenID AX Client is required for attributte exchange, but there are
couple of issues which needs to be resolved. So NOW YOU NEED TO USE "PATCHED"
VERSION OF THE OPENID AX CLIENT MODULE or you need to patch it yourself.

How to apply patches: http://drupal.org/patch/apply

Issues with OpenID AX Client module which needs to be resolved:
 - Support hook_openid_client interaction when user is adding openid identities
   http://drupal.org/node/1036310
 - Support AX extensions
   http://drupal.org/node/1033292
 - AX not working with myopenid.com
   http://drupal.org/node/440418
________________________________________________________________________________

CREDITS
________________________________________________________________________________

Vojtech Kusy <wojtha@gmail.com>
http://vojtechkusy.com

Sponsored by NIC.CZ - the mojeID.cz provider
http://nic.cz
http://mojeid.cz
