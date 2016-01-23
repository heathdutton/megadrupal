jStats
-------------------------------------------------


INSTALLATION INSTRUCTIONS
-------------------------------------------------
Downloading and installing jStats like any other
module should give you a working installation.

To get a nice performance benefit, you should
copy the jstats.php file provided in the download
to the root of your Drupal installation, next to
the index.php file shipped by Drupal.

The statistics are aggregated asynchronously: you
need to either run the drupal cron or the drush
jstats-aggregate command at least as frequently as
the frequency you set in the configuration UI.
