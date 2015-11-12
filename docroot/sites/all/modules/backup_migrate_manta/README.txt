-------------------------------------------------------------------------------
Backup and Migrate Manta
  by arpieb
-------------------------------------------------------------------------------

DESCRIPTION:
This module provides a destination for the Backup and Migrate module to utilize
the Joyent Manta REST API to talk to Joyent's cloud storage systems.

-------------------------------------------------------------------------------

INSTALLATION:
* Put the module in your drupal modules directory and enable it

* You will need to download the php-manta library here:

  http://git.octobang.com/php-manta

  Install it into sites/all/libraries/php-manta or another location that
  the Libraries module is configured to search.

* The Manta endpoint URL as of Jul 16, 2013 is https://us-east.manta.joyent.com

-------------------------------------------------------------------------------

