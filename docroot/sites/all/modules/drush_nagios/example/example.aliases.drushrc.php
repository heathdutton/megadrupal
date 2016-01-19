<?php

/**
 * @file 
 * Drush site alias configuration 
 * with command specific options for nagios
 *
 * Drush will search for aliases in any of these files using
 * the alias search path.  The following locations are examined
 * for alias files:
 *
 *   - In any path set in $options['alias-path'] in drushrc.php,
 *    -  or (equivalently) any path passed in via --alias-path=...
 *    -  on the command line.
 *   - If 'alias-path' is not set, then in one of the default locations:
 *    - /etc/drush
 *    - In the drush installation folder
 *    - Inside the 'aliases' folder in the drush installation folder
 *    - $HOME/.drush
 *   - Inside the sites folder of any bootstrapped Drupal site,
 *      or any local Drupal site indicated by an alias used as
 *      a parameter to a command
 *      
 * @see http://drush.ws/examples/example.aliases.drushrc.php
 */


/**
 * Site alias 'dev' with command specific options for nagios
 * 
 * - ignore
 * @code $aliases['dev']['command-specific']['check-updates']['ignore'] = 'project1,project2,projectN';@endcode
 *  - Comma seperated list of projects which should be ignored, useful for patched, modified modules etc.
 * - ignore-locked
 * @code $aliases['dev']['command-specific']['check-updates']['ingnore-locked'] = 1';@endcode
 *  - Ignore projects which are locked by `drush pm-update --lock`.
 */
$aliases['dev'] = array(
  'uri' => 'dev.mydrupalsite.com',
  'root' => '/path/to/drupal/root',

  'command-specific' => array (
    'check-updates' => array(
      'ignore' => 'project1,project2,projectN',
      'ignore-locked' => 1,
    ),
  ),
);
