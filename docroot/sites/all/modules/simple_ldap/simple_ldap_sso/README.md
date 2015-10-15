# Simple LDAP SSO
Simple LDAP SSO is a Single-Sign-On implementation that uses your LDAP server to authenticate each session.

## How does it work?
When a user logs in to any site using this module, two things occur. First, the unique session ID that Drupal assigns to the user is [hashed](http://en.wikipedia.org/wiki/Hash_function) and stored in an attribute you deem on LDAP. Then, the session information—including the user's name and session id—is encrypted and stored in a cookie.

When a user then navigates to another website configured with this SSO module, and before the session handling occurs that determines whether a user is logged in or not, the SSO cookie is decrypted, and the session information is saved to the database. Then, the normal session handling occurs, and the Drupal session cookie is recognized and used. Finally, at the end of the bootstrap, the session ID is validated against the hashed value stored in LDAP. If the values do not match, the user is immediately logged out and errors are logged.

## Requirements
- A common base domain to use.
- The [PHP mcrypt extension](http://php.net/mcrypt) installed on the server.
- Read/write credentials to LDAP

## Installation
1. Install the module at admin/modules, or using drush.
2. Configure the module at admin/config/people/simple_ldap/sso.
3. Go to admin/reports/status to see if Simple LDAP SSO is marked as 'Configured'.

**NOTE**: All sites must use the same encryption key, cookie domain, LDAP attribute, and session ID hashing algorithm.