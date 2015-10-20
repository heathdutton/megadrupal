Hosting CiviCRM Cron
====================

This module provides the ability to trigger CiviCRM cron jobs on sites
hosted on the Aegir Hosting System.

CiviCRM requires it own cron jobs, to trigger mass mailings in CiviMail,
for example. Since the URLs to trigger these jobs are different from
those of the associated Drupal sites' cron, they're normally handled
separately, in crontabs for example.

With Hosting CiviCRM Cron, these can now be managed in Aegir, as are
site deployments, backups, and upgrades.

Built with Aegir up [1] by Ergon Logic Enterprises [2] for use with
CiviCRM [3] and Provision CiviCRM [4] on the Aegir Hosting System [5].
Maintained and supported by Coop SymbioTIC [6] and Praxis Labs Coop [7].

[1] https://drupal.org/project/project/aegir-up
[2] http://ergonlogic.com
[3] http://civicrm.org
[4] https://drupal.org/project/provision_civicrm
[5] http://www.aegirproject.org
[6] https://www.symbiotic.coop
[7] http://praxis.coop

Installation
============

This module should be automatically enabled by hosting_civicrm.
If not, you can run:

    drush @hostmaster en hosting_civicrm_cron -y

If you are using the Debian package for Aegir and you would like to
automate the installation of the module on new Aegir installs, you 
should use a custom makefile so that the module is not lost after an upgrade:
http://community.aegirproject.org/upgrading/debian#Custom_distributions
