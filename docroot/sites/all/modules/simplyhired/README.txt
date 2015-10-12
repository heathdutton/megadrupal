-- SUMMARY --

SimplyHired is a great way to earn additional revenue from your existing website
and provide valuable content for your users with a free hosted job board. The
SimplyHired module suite uses SimplyHired's XML API to display job postings from 
their database directly within your Drupal site.

For a full description of the module, visit the project page:
  {https://drupal.org/project/simplyhired}


To see the full list of features and benefits of Job-a-matic, visit the
SimplyHired Partner website:
  http://www.simplypartner.com

NOTICE: The SimplyHired Terms of Service requires the attribution block on any
page that displays SimplyHired data. The official terms are located at:

https://simply-partner.com/partner-terms/1

-- REQUIREMENTS --

SimplyHired API Library (version 2) - https://github.com/r0nn1ef/simplyhired/tree/2.x

-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.

You must install the SimplyHired API Library in sites/all/libraries/simplyhired.
The modules will load this library to make any requests to the SimplyHired API.

-- CONFIGURATION --

* You must create an account with SimplyHired prior to using this module. To
  create an account, go to
  https://simply-partner.com/partners-signup.

* Once your account has been created, log in an click on the "API" tab.
  Under the Job Search GET Parameters section, locate your publisher ID (pshid).
  You will also need the API key at the top of the page. (The other parameters 
  are passed automatically by the module in api calls.)

* On your Drupal site, log in as a user with administer site configuration
  permission and go to Configuration > SimplyHired API Settings Settings and 
  enter your publisher ID and API key. You may also elect to change which of the
  SimplyHired attribution logos to use on your site depending on your theme.

-- SUBMODULES --

SimplyHired comes with two submodules with specific purposes:
 1. SimplyHired Search - integrates with the core Drupal search module to all
    for job searches.
 2. SimplyHired Listings - creates a new content type "Simply Hired Listing page"
    which allows the creation of node pages with pre-defined search criteria for
    displaying on your website.

-- TROUBLESHOOTING --

None.

-- FAQ --

None.

-- CONTACT --

Current maintainers:
* Ron Ferguson (r0nn1ef) - http://drupal.org/user/290065
