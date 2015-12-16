<?php

/**
 * @file
 * Provide Drush integration for more advanced make usage.
 */

/**
 * Implements hook_drush_command().
 */
function makesalot_drush_command() {
  $items['makesalot'] = array(
    'description' => 'Make all the things!',
    'aliases' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    'arguments' => array(
      'makefile' => 'Which makefile to use. Assumes project.make if none are specified.',
    ),
    'options' => array(
      'skipdump' => 'Do not create dump of MySQL database.',
      'buildname' => 'Name to use for this build, ignoring branch detection.',
    ),
  );
  return $items;
}

/**
 * Begins the build process.
 *
 * Wraps drush make, sql-dump.
 */
function drush_makesalot($makefile = "repository/project.make", $buildname = NULL) {
  // Use args if options aren't set correctly.
  // This mimics usage of 'drush make'.
  $buildname = drush_get_option('buildname', $buildname);
  $makefile  = drush_get_option('makefile', $makefile);

  // This assumes we are in the project root.
  $proj_root  = getcwd();
  echo "Currently working in " . $proj_root . "\n";

  $repository = $proj_root . "/repository";
  $builds     = $proj_root . "/builds";
  $shared     = $proj_root . "/shared";
  $sql_dumps  = $proj_root . "/sql-dumps";

  // ISO 8601 date (yyyy-mm-ddThh:mm:ss+00:00).
  $time = date("c");
  // Strip colons since some operating systems have problems with them.
  $time = str_replace(':', '', $time);
  

  // Make sure makefile exists.
  if (!file_exists($makefile)) {
    return drush_set_error('DRUSH_BAD_PATH', dt('Could not find makefile at !makefile.', array('!makefile' => $makefile)));
  }

  // Attempt to detect the git branch, or else use "nobranch".
  if (!is_null($buildname)) {
    $branch = $buildname;
  } else if (file_exists($repository . '/.git/HEAD')) {
    $branch = file($repository . '/.git/HEAD', FILE_IGNORE_NEW_LINES);
    $branch = end(explode('/', $branch[0]));
  }
  else {
    $branch = 'nobranch';
  }

  // Concatenate id from repository, timestamp, and buildname.
  $id = $branch . '--' . $time;

  // Dump database unless otherwise specified.
  if (!drush_get_option('skipdump')) {
    // Bootstrap to current build.
    define('DRUPAL_ROOT', trim(shell_exec('readlink www')));
    chdir(DRUPAL_ROOT);
    if (!include DRUPAL_ROOT . '/includes/bootstrap.inc') {
      echo "Unable to bootstrap to Drupal installation. Database will not be dumped.\n";
    }
    else {
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

      // Dump database.
      drush_invoke_process('@self', 'sql-dump', array(), array('gzip' => 1, 'result-file' => $sql_dumps . '/sql-dump--' . $id . '.sql.gz'));
    }
    chdir($proj_root);
  }

  // Make project.
  $new_build = $builds . '/build--' . $id;

  drush_invoke_process('@self', 'make', array($proj_root . '/' . $makefile, $new_build));

  // Symlink stuff.
  if (file_exists($new_build)) {
    unlink($proj_root . '/www');
    symlink($new_build, $proj_root . '/www');

    foreach (array('modules', 'themes', 'libraries') as $dir) {
      if (file_exists("$repository/$dir")) {
        symlink("$repository/$dir", "$new_build/sites/default/$dir");
      }
    }
    if (file_exists("$repository/settings.php")) {
      copy("$repository/settings.php", "$new_build/sites/default/settings.php");
    }

    if (file_exists("$shared/files")) {
      symlink("$shared/files", "$new_build/sites/default/files");
    }
    if (file_exists("$shared/settings.local.php")) {
      copy("$shared/settings.local.php", "$new_build/sites/default/settings.local.php");
    }
  }
  else {
    drush_log("Build failed. Check your makefile.\n");
  }
}
