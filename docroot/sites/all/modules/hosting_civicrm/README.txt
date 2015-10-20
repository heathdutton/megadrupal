Hosting CiviCRM
===============


Built with Aegir up [1] by Ergon Logic Enterprises [2] for use with
CiviCRM [3] and Provision CiviCRM [4] on the Aegir Hosting System [5].
Maintained and supported by Coop SymbioTIC [6] and Praxis Labs Coop [7].

[1] https://drupal.org/project/project/aegir-up
[2] http://ergonlogic.com
[3] https://civicrm.org
[4] https://drupal.org/project/provision_civicrm
[5] http://www.aegirproject.org/
[6] https://www.symbiotic.coop
[7] http://praxis.coop

Installation
============

- Copy the module to your /var/aegir/hostmaster/sites/aegir.example.org/modules/
- Enable the module: drush @hostmaster en hosting_civicrm -y
- In Aegir, give the 'configure site CiviCRM cron intervals' permission to admins.

If you are using the Debian package for Aegir and you would like to
automate the installation of the module on new Aegir installs, you 
should use a custom makefile so that the module is not lost after an upgrade:
http://community.aegirproject.org/upgrading/debian#Custom_distributions
