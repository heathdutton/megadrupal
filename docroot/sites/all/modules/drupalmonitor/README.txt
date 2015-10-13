
-- SUMMARY --

The module connects any Drupal 7 site with drupalmonitor.com. As the name
says, Drupalmonitor is a tool to actively monitor Drupal sites. It's not
only a "heartbeat" kind of monitoring. It's much more powerful. Using the
drupalmonitor connector, the tool can gather data from the drupal site 
like how many users, nodes, last cron run, page requests, etc. On 
drupalmonitor.com, the data will be processed and shown in graphs over 
time.

For a full description of the module, visit the project page:
  http://drupal.org/project/drupalmonitor_connector

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/drupalmonitor_connector


-- REQUIREMENTS --

* An active account on www.drupalmonitor.com is required for this module
  to work. Please consider the install & configuration sections below.


-- INSTALLATION --

* Install the module as usual.
  See http://drupal.org/documentation/install/modules-themes/modules-7
* Enable permission "access drupalmonitor api" for anonymous users.


-- CONFIGURATION --

* Configure Drupalmonitor settings:
  - Go to Configuration » System » Drupalmonitor.com

  - You likely want to enable or disable drupalmonitor connector to store
    request data. You can also set a security hash.
    

-- CUSTOMIZATION --

* Drupalmonitor connector module offers hook_drupalmonitor(). Most recent
  documentation can be found on http://drupalmonitor.com/faq.

-- CONTACT --

Current maintainers:
* Lukas Fischer - http://drupal.org/user/204430

This project has been sponsored by:
* netnode
  Specialized in consulting and planning of Drupal powered sites, netnode
  offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.netnode.ch for more information.

* drupalmonitor.com
  A tool to easily monitor & maintain one or 100 or 1000 Drupal websites.
  Visit http://www.drupalmonitor.com
