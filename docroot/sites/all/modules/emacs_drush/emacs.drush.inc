<?php

/**
 * @file
 * Drush commands and extensions for Emacs users.
 */

define('DRUSH_EMACS_ETAGS_EXTENSIONS', 'module,inc,php,profile,install');
define('DRUSH_EMACS_GTAGS_GTAGSCONF', dirname(__FILE__) . '/globalrc');

// The first version known to parse system.api.php without errors.
define('DRUSH_EMACS_GTAGS_FIRST_GOOD_VERSION', '6.2.7');

/**
 * Implements hook_drush_command().
 */
function emacs_drush_command() {
  $items = array();

  $items['etags'] = array(
    'description' => dt('Make/update Emacs TAGS file.'),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    'examples' => array(
      'drush etags' => dt('Make/update an Emacs TAGS file in DRUPAL_ROOT.'),
    ),
    'options' => array(
      'emacs-etags-extensions' => dt('On which file extensions to run etags. Defaults to "@extensions".', array('@extensions' => DRUSH_EMACS_ETAGS_EXTENSIONS)),
      'emacs-etags-etags-program' => dt('Name of the etags executable. Include path if executable is not in $PATH.'),
      'emacs-etags-find-program' => dt('Name of the find executable. Include path if executable is not in $PATH.'),
    ),
  );
  $items['gtags'] = array(
    'description' => dt('Make/update GNU GLOBAL files.'),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    'examples' => array(
      'drush gtags' => dt('Make/update GNU GLOBAL files in DRUPAL_ROOT.'),
    ),
    'options' => array(
      'emacs-gtags-gtags-program' => dt('Name of the gtags executable. Include path if executable is not in $PATH.'),
      'emacs-gtags-gtags-options' => dt('Options to add to gtags. Defaults to none.'),
      'emacs-gtags-gtagsconf' => dt('Gtags configuration file to use. Defaults to "@gtagsconf". Use "0" for gtags default configuration.', array('@gtagsconf' => DRUSH_EMACS_GTAGS_GTAGSCONF)),
      'emacs-gtags-gtagslabel' => dt('Gtags label to use. Defaults value depends on your version of GNU GLOBAL. From GNU GLOBAL version @gtagsversion it will default to "drupal". Older versions of GNU GLOBAL will default to "drupalX" where X is your Drupal major version number. Use "0" to indicate no label (gtags default label).', array('@gtagsversion' => DRUSH_EMACS_GTAGS_FIRST_GOOD_VERSION)),
    ),
  );

  return $items;
}

/**
 * Make/update an Emacs TAGS file in DRUPAL_ROOT.
 */
function drush_emacs_etags($root = NULL) {
  if (is_null($root)) {
    $root = drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT');
  }

  $extensions = explode(',', drush_get_option('emacs-etags-extensions', DRUSH_EMACS_ETAGS_EXTENSIONS));
  $etags = drush_get_option('emacs-etags-etags-program', 'etags');
  $find = drush_get_option('emacs-etags-find-program', 'find');

  $etags_command = "$find . -type f -name \*." . implode(' -or -name \*.', $extensions) . " | $etags --language=php -";

  if ($root) {
    chdir($root);
  }
  elseif (!file_exists('TAGS')) {
    if (!drush_confirm('No DRUPAL_ROOT and no TAGS in current dir. Create TAGS in current dir?')) {
      return drush_user_abort('User aborted.');
    };
  }

  drush_log('Creating/updating Emacs TAGS file...');
  system($etags_command, $return);

  if ($return) {
    return drush_set_error('DRUSH_EMACS', dt('Problem executing "@command".', array('@command' => $etags_command)));
  }

  drush_log('Creating/updating Emacs TAGS file...', 'success');
}

/**
 * Make/update GNU GLOBAL files in DRUPAL_ROOT.
 */
function drush_emacs_gtags($root = NULL) {
  if (is_null($root)) {
    $root = drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT');
  }

  $gtags = drush_get_option('emacs-gtags-gtags-program', 'gtags');
  $options = drush_get_option('emacs-gtags-gtags-options', FALSE);

  $gtagsconf = drush_get_option('emacs-gtags-gtagsconf', DRUSH_EMACS_GTAGS_GTAGSCONF);

  if (!drush_get_option('emacs-gtags-gtagslabel', FALSE) AND version_compare(_emacs_gtags_version(), DRUSH_EMACS_GTAGS_FIRST_GOOD_VERSION)) {
    $gtagslabel = 'drupal' . drush_drupal_major_version();
    if (drush_drupal_major_version() == 7) {
      drush_log('Gtags skipping modules/system/system.api.php since your are using Drupal 7 and gtags version prior to ' . DRUSH_EMACS_GTAGS_FIRST_GOOD_VERSION, 'warning');
    }
    elseif (drush_drupal_major_version() >= 8) {
      drush_log('Gtags skipping core/modules/system/system.api.php since your are using Drupal 8 and gtags version prior to ' . DRUSH_EMACS_GTAGS_FIRST_GOOD_VERSION, 'warning');
    }
  }
  else {
    $gtagslabel = drush_get_option('emacs-gtags-gtagslabel', 'drupal');
  }

  $gtags_command = array();
  $gtags_command[] = $gtags;
  if ($gtagsconf != FALSE) {
    $gtags_command[] = " --gtagsconf=$gtagsconf";
  }
  if ($gtagslabel != FALSE) {
    $gtags_command[] = " --gtagslabel=$gtagslabel";
  }
  if ($options != FALSE) {
    $gtags_command[] = " $options";
  }

  if (is_null($root)) {
    $root = drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT');
  }

  if ($root) {
    chdir($root);
  }
  elseif (!file_exists('GTAGS')) {
    if (!drush_confirm('No DRUPAL_ROOT and no GTAGS in current dir. Create GTAGS in current dir?')) {
      return drush_user_abort('User aborted.');
    };
  }

  drush_log('Creating/updating GNU GLOBAL files...');

  $gtags_command = trim(implode(' ', $gtags_command));
  system($gtags_command, $return);

  if ($return) {
    return drush_set_error('DRUSH_EMACS', dt('Problem executing "@command".', array('@command' => $gtags_command)));
  }

  drush_log('Creating/updating GNU GLOBAL files...', 'success');
}


/**
 * Implements hook_drush_pm_post_download().
 *
 * After downloading a module we run etags and/or gtags if we think etags/gtags
 * is in use (we think it is in use if a TAGS/GTAGS file is present in
 * DRUPAL_ROOT).
 *
 * You can disable this by setting either:
 *
 *  $options['emacs-pm-post-download-etags'] = FALSE;
 *  $options['emacs-pm-post-download-gtags'] = FALSE;
 *
 * in your drushrc.php or in your @site-alias.
 */
function emacs_drush_pm_post_download($project, $release) {
  global $_emacs_drush_update_tags;

  // Flag that we want an update to occur in hook_drush_exit().
  $_emacs_drush_update_tags = TRUE;
}

/**
 * Implements hook_pm_post_update().
 */
function emacs_pm_post_update($release_name, $release_candidate_version, $project_parent_path = NULL) {
  global $_emacs_drush_update_tags;

  // Flag that we want an update to occur in hook_drush_exit().
  $_emacs_drush_update_tags = TRUE;
}

/**
 * Implements hook_situs_hooks().
 */
function emacs_situs_plugin() {
  return array(
    'emacs' => array(
      'name' => 'Emacs Drush',
      'description' => 'Emacs Drush: Build etags/gtags after situs build.',
      'post_build' => 'emacs_situs_post_build',
    ),
  );
}

/**
 * Situs post_build. Build etags/gtags after situs build.
 *
 * Support Situs site building command for Drush.
 *
 * @see emacs_situs_plugin()
 * @see https://github.com/xendk/situs
 */
function emacs_situs_post_build() {
  global $_emacs_drush_update_tags;

  $root = drush_get_option('root', '');
  $build_path = drush_get_context('SITUS_BUILD_PATH');

  if (drush_get_option('emacs-pm-post-download-etags', TRUE) && file_exists($root . '/TAGS')) {
    drush_emacs_etags($build_path);
  }
  if (drush_get_option('emacs-pm-post-download-gtags', TRUE) && file_exists($root . '/GTAGS')) {
    drush_emacs_gtags($build_path);
  }

  // Flag that we want an update to occur in hook_drush_exit().
  $_emacs_drush_update_tags = TRUE;
}

/**
 * Implements hook_drush_exit().
 */
function emacs_drush_exit() {
  global $_emacs_drush_update_tags;

  if (isset($_emacs_drush_update_tags) && $_emacs_drush_update_tags) {
    if (drush_get_option('emacs-pm-post-download-etags', TRUE) && file_exists(drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT') . "/TAGS")) {
      drush_emacs_etags();
    }
    if (drush_get_option('emacs-pm-post-download-gtags', TRUE) && file_exists(drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT') . "/GTAGS")) {
      drush_emacs_gtags();
    }

    // Now we have updated the tags - so don't do it again (drush pm-update
    // apparently calls hook_drush_exit() twice).
    $_emacs_drush_update_tags = FALSE;
  }
}

/**
 * Gtags / GNU GLOBAL version number.
 *
 * @return string
 *   Version number of gtags.
 */
function _emacs_gtags_version() {
  $gtags = drush_get_option('emacs-gtags-gtags-program', 'gtags');
  return trim(`$gtags --quiet --version`);
}
