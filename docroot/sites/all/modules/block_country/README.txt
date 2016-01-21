CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * TROUBLESHOOTING (POSSIBLE PERFORMANCE ISSUES)
 * MAINTAINERS

INTRODUCTION
------------
This module Enhances block visibility settings by adding country options.

REQUIREMENTS
------------
Install IP-based determination of Country Module.
( https://www.drupal.org/project/ip2country )
Ip2country module uses a visitor's IP address to identify the geographical
location (country) of the user

INSTALLATION
------------
1. Check requirements section first.
2. Enable the module.
https://www.drupal.org/documentation/install/modules-themes/modules-7

CONFIGURATION
------------
To add country specific visibility to a block, Go to that block's configuration
country settings is listed under block "Visibility settings"

TROUBLESHOOTING (POSSIBLE PERFORMANCE ISSUES)
-----------
Block Country module implements hook_boot() to detect user geographical
(country) based on IP address.
block_country_boot() hook will be run for each page request - even for non
logged users and anonymous cache enabled - and this will give performance issues
for large scale websites

MAINTAINERS
-----------

Current maintainers:
 * Ashish Dalvi (Ashish.Dalvi) - https://drupal.org/user/1814722
 * Prateek Jain (prateekjain) - https://drupal.org/user/721062

This project has been sponsored by:
 * BLISSTERING SOLUTIONS
   Blisstering Solutions is a Drupal Services, Solutions and Products Company.
   It offer full range of Drupal Services from building your Drupal Solution -
   that include custom module development, theming, performance or testing – to
   extending your Drupal site or solution to mobile, tablet, Facebook and cloud.
