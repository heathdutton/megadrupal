--------------------------------------------------------------------------------
Node Access Keys
--------------------------------------------------------------------------------

Description
--------------------------------------------------------------------------------
Node Access Keys helps to grant users temporary view permissions to selected
content types on a per user role basis. You can have multiple Access Keys for
different content types and user roles.

As an example, this module could be used on a registration site that requires
content to be hidden from a non-registrant but visible to users who have
registered. An Access Key could be emailed to them which would grant them view
permission to the content types specified. This makes it so users are not
required to have individual user accounts.

This module has built-in integration for use with the Views module.

Installation
--------------------------------------------------------------------------------
1. Extract and enable Node Access Keys.
2. Set a default content type for Node Access Keys to use on your site's system
   pages by visiting admin/config/people/nodeaccesskeys/settings.
3. Visit admin/config/people/nodeaccesskeys to set up your Node Access Keys.

Features
--------------------------------------------------------------------------------
You may want to give users a way to end their session (log out). For that you
should use the URI nodeaccesskeys/clear to clobber all the Access Keys in the
user's session variable. This page is really intended for anonymous users as the
core log out path (user/logout) will already do this.

Possible Conflicts
--------------------------------------------------------------------------------
Node Access Keys works with user sessions and thus problems may arise if you are
running reverse proxies like Varnish where pages are cached for anonymous users.

Support
--------------------------------------------------------------------------------
This open source project is supported by the Drupal.org community. To report a
bug, request a feature, or upgrade to the latest version, please visit the
project page:

  http://drupal.org/project/nodeaccesskeys
