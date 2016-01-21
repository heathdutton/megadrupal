================================================================================
 INTRODUCTION
================================================================================

Original author and current maintainer: 
- Jordan Magnuson <http://drupal.org/user/269983>

This module provides deep integration with the UserVoice feedback and online 
help desk service for widget placement, single sign-on authentication, custom 
gadgets, and more.

Maintenance and development is sponsored by UserVoice.

================================================================================
 Features
================================================================================

This module allows you to:

  - Customize and place the UserVoice widget script anywhere on your site 
    (choosing either the "new" widget or "classic" widget style), with page and
    role visibility settings.
  - Replace Drupal's default site-wide contact form with an embedded version of 
    the classic UserVoice widget.
  - Automatically pass user information (like email, name, uid) from Drupal to 
    UserVoice.
  - Automatically pass relevant ecommerce data from Drupal to UserVoice, 
    including customer total lifetime value and order history. Comes with 
    built-in support for Membership Suite (moneyscripts.net), with additional 
    integrations planned.
  - Automatically provide single sign-on (SSO) functionality (if your UserVoice 
    account supports it), so that users who are signed into your Drupal site 
    will automatically be signed into your UserVoice support area as well. 
    
This module also provides:    
    
  - A script caching mechanism for improved page building performance.  
  - Hooks which allow you to easily alter how profile, account, and customer 
    value information is passed between Drupal and UserVoice.
  - Custom gadgets, so that you can see relevant profile information and order 
    history when examining any user within the UserVoice admin console. These 
    gadgets use preprocessing functions and template files for easy 
    customization.
  - Web hook integration, so that UserVoice ticket information can be saved 
    directly to your Drupal database, so that developers can link users to their 
    support tickets, run data analysis on the tickets, etc.

================================================================================
 Integrations
================================================================================

Currently this module integrates with the following eCommerce modules to provide
relevant order history and customer lifetime value information:

  - Membership Suite <http://moneyscripts.net>
  
Further integrations may be requested via the issue queue.

================================================================================
 INSTALLATION
================================================================================

This module has no special installation requirements. For general instruction on 
how to install and update Drupal modules see 
<http://drupal.org/getting-started/install-contrib>.

================================================================================
 CONFIGURATION
================================================================================

This module can be configured by visiting admin/config/services/uservoice.

You will need to have an account at UserVoice.com (a free plan is available),
and have your UserVoice subdomain and SSO key available (your Javascript API key 
can be found in your UserVoice  admin console, under the 'Advanced setup' 
portion of the 'Widgets' tab, and your SSO key under the 'User Authentication' 
portion of the 'Web Portal' tab).
