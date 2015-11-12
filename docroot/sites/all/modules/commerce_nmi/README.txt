NMI (Network Merchants, Inc.) integration for the Drupal Commerce framework.
Requires the NMI-PHP library from https://github.com/AllPlayers/NMI-PHP.

Installation
============
Download and install the module as you would any other drupal module. This
module requires the NMI-PHP module which can be installed through composer. You
can use the composer_manager or composer_autoload module (or a custom solution
if you prefer). To utilize either the the aformentioned modules, you will to
install the composer module or composer itself
(http://getcomposer.org/download/).

To utilize composer_manager, download and install the module. Navigate to 
/admin/config/system/composer-manager to generate the composer.json file for
your site. In a terminal, navigate to the location of your site's json file and
run `drush composer install` (or just `composer install`).

To utilize composer_autoload, download and install the module. In a terminal,
navigate to the location of this module (typically
sites/all/modules/commerce_nmi) and run `drush composer install` (or just
`composer install`).
