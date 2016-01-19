CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------

The LinkedIn Company API Suite is intended to be a addon suite of modules to the
[LinkedIn Integration module](https://drupal.org/project/linkedin), and is
intended to encapsulate the functionality contained within the
LinkedIn Company API.

It currently contains two modules

LinkedIn Company
This module creates the functionality to authenticate as an organization with
LinkedIn. Unlike the LinkedIn module which forces users to authenticate at the
individual profile level, this module adds button that allows an admin with
access to the company profile to authenticate and generate an OAuth token
specific to the company that works sitewide.

LinkedIn Company Shares
This module pulls in the recent LinkedIn company shares and puts them into a
themeable block. You can adjust the number of shares to show.

We intend to add additional modules in the future to support the other
company API integrations such as jobs and product postings.

REQUIREMENTS
------------
This module requires the following modules

 * [LinkedIn](https://drupal.org/project/linkedin)

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   [here](https://drupal.org/documentation/install/modules-themes/modules-7)
   for further information.
 * If you haven't already, install and configure the LinkedIn module.
 * If you don't already have one, create a
   [developer account](https://www.linkedin.com/secure/developer)
   for LinkedIn and use that account to register your sites application.
 * In your application settings, where it says "OAuth 2.0 Redirect URLs" be sure
   to add "http://your-domain.com/linkedin/company/token" to the list of urls
   replacing your-domain.com with your sites domain.
 * If they don't already have entries, add your API Key and Secret Key from the
   LinkedIn app to there respective places under "Base Settings" on the
   configuration screen.
 * Add your LinkedIn company ID under admin/config/services/linkedin. You can
   use the LinkedIn company lookup tool in order to determine your company ID.
   The lookup tool can be found
   [here](https://developer.linkedin.com/apply-getting-started#company-lookup).
 * Click authorize and follow the instructions in order to authorize your
   application.
 * From there assign the LinkedIn Company Shares block to the region of your
   choice.

CONFIGURATION
-------------
 * Configure user permissions in Administration » People » Permissions:
   - Administer LinkedIn Company Settings
     - Allows users to customize the LinkedIn Company administrative settings.
 * Customize module settings including number of shares to display and share
   caching time in admin/config/services/linkedin

MAINTAINERS
-----------
Current maintainers

[Michael D. Hodge Jr (michaelhodgejr)](https://www.drupal.org/u/michael-hodge-jr)

[Bruce Clingan (astrocling)](https://www.drupal.org/u/astrocling)

Initial development of this project has been sponsored by

[LightSky](http://www.lightsky.com)

LightSky is a full service web design and digital marketing agency,
focusing on leveraging the power of Drupal to create innovative web
experiences.  Whether it is helping clients with their digital marketing
plans, creating websites to represent their brand, or workflow automation
solutions to help their business be more efficient, LightSky has the
capabilities to conquer your organization's digital obstacles.
[Learn more](http://www.lightsky.com).
