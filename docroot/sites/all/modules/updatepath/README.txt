DESCRIPTION
===========

This project implements a drush command that runs the a set of common steps
to get a database up to date with the exported configuration in code.

The .module and .info files are there in order to be able to make releases
at drupal.org. This is not a module, it is a Drush command. Drush can find
commands in certain directories such as $HOME/.drush/
or sites/all/drush. Run `drush topic docs-commands` on a terminal to see other
places where this project can be placed so Drush can discover it.

REQUIREMENTS
============

* Drush 6 or higher.
* Registry Rebuild.
* Features.

INSTALLATION
============

First, make sure that you meet the requirements by reading the previous section.

Then, go to your project's root directory and run the following command to
download updatepath:

$ drush dl --destination=sites/all/drush updatepath

USAGE
=====

See drush help updatepath for usage examples.

CUSTOMIZING
===========

Drush's API is very flexible. You can hook into before and after the command, plus
take action when errors occur. Have a look at drush topic docs-api for further
details on which hooks you can implement from your commands to alter the
updatepath.
