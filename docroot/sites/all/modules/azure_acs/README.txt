Windows Azure ACS is a module for integrating your Drupal site with Windows
Azure Access Control Service.

With ACS, you can authenticate users from different identity providers such as
Facebook, Google and Windows Live.

Key features of this module:
* Authenticating and logging in through identity providers configured in your
ACS namespace.
* Option to create new users on the fly or direct them through Drupal
registration process.
* Option to link a single user account to several identity providers.
* A block that provides a login link for each identity provider configured in
ACS.

The module uses libraries (in lib directory) written by Microsoft for the ACS
WordPress plugin <http://acs.codeplex.com/wikipage?title=WordPress%20Plugin>

To configure your ACS, see the WordPress plugin documentation or this page:
http://www.windowsazure.com/en-us/develop/net/how-to-guides/access-control/

IMPORTANT NOTE: Only SWT 1.0 tokens are supported at the moment, so be sure to
select that as your token type in ACS.

The development of this module has been sposored by Omegawave
<http://www.omegawavesport.com>