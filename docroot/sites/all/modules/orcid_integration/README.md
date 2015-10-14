CONTENTS OF THIS FILE
---------------------

 * summary
 * requirements
 * installation
 * configuration
 * troubleshooting
 * sponsors

SUMMARY
-------

ORCID provides a simple connector between Drupal and ORCID, a service to
provide unique global identifiers to researchers and their activities
(see http://orcid.org).

REQUIREMENTS
------------

Drupal Core 7.x

INSTALLATION
------------

Download and enable module.

CONFIGURATION
-------------

You need to provide the necessary configuration settings at /admin/config/services/orcid

Required:

1. Main site url (since you could work with production or sandbox)
1. Public ORCID API url (at minimum, as a fallback if the member api is not defined)

Optional:

1. ORCID Members API url
1. Client ID
1. Client Secret
1. Redirect URL

And set up the permissions (from the user permissions page)

DRUSH INTEGRATION
-----------------

The drush integration allows you to provision new accounts and map accounts to
existing ORCIDs (if they can be matched by the account's email address). This
integration is provided by the orcid_integration_provision module. To learn
more about the  drush command, type the following into the command line:

    drush help orcid-integration-provision

You are required to provide a path to a CSV file that contains colums with the
following headers (in any order): first_name, last_name, username, email.

TROUBLESHOOTING
---------------

Use the issue queue!

FAQ
---

What are the plans for the module?

Longer term plans (which are also in short term) are to clean up the code to
make it more organized (make it OO), and to add any new features that make
sense.

SPONSORS
--------

Cherry Hill
UCLA Digital Libraries
