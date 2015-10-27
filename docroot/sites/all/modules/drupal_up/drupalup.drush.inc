<?php

define("DRUPAL_UP_ROOT", dirname(__FILE__));

/**
 * Implementation of hook_vagrant_blueprints
 */
function drupalup_vagrant_blueprints() {
  $blueprints = array(
    'drupal6' => array(
      'name' => dt('Drupal 6.x'),
      'description' => dt('The latest release of Drupal 6, built on a standard LAMP stack.'),
      'path' => 'blueprints/drupal6',
      'build_callback' => 'drupalup_build_project',
    ),
    'drupal7' => array(
      'name' => dt('Drupal 7.x'),
      'description' => dt('The latest release of Drupal 7, built on a standard LAMP stack.'),
      'path' => 'blueprints/drupal7',
      'build_callback' => 'drupalup_build_project',
    ),
    'drupal8' => array(
      'name' => dt('Drupal 8.x'),
      'description' => dt('A checkout of the latest Drupal 8.x code, built on a standard LAMP stack.'),
      'path' => 'blueprints/drupal8',
      'build_callback' => 'drupalup_build_project',
    ),
  );

  return $blueprints;
}

/**
 * Implementation of hook_drush_command().
 *
 * Req'd for Drush to recognize this file as a Drush extension.
 */
function drupalup_drush_command() {
  $items = array();

  $items['drupalup'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'hidden' => TRUE,
  );

  // Drush topics
  $items['docs-drupalup-readme'] = array(
    'description' => dt('Drupal-up README.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(DRUPAL_UP_ROOT . '/README.md'),
  );

  return $items;
}

/**
 * Custom drush-vagrant build process to instantiate an existing client project
 */
function drupalup_build_project() {
  _drupalup_clone_drupal();

  $build = vagrant_default_build_vars();
  $build['modules']['dependencies'] = dirname(__FILE__) . '/lib/puppet-modules/';
  vagrant_default_build_project_dir($build);
  // Check for support of 64-bit client OSes
  if (drush_is_windows()) {
    drush_shell_exec("egrep '(vmx|svm)' /proc/cpuinfo");
    $output = drush_shell_exec_output();

    if (empty($output) || count($output) == 0) {
      $settings_file = file_get_contents($build['project_path'] . '/settings.rb');
      $settings_file = str_replace('debian-LAMP-20', 'debian-LAMP-i386-20', $settings_file);
      $settings_file = str_replace('debian-LAMP-current.box', 'debian-LAMP-i386-current.box', $settings_file);
      file_put_contents($build['project_path'] . '/settings.rb', $settings_file);
    }
  }
  vagrant_default_build_project_setup($build);
  vagrant_default_build_config_dir($build);
  
  _drupalup_copy_drupal($build);

  vagrant_default_build_record_blueprint($build);
  vagrant_default_build_user_data($build);
  vagrant_default_build_user_dotfiles($build);
//  vagrant_default_build_git_init($build);
}

function _drupalup_clone_drupal() {
  // Get our target directory we'll clone to
  $target = dirname(__FILE__) . '/lib/drupal';
  // Clone Drupal into it
  if (!is_dir($target)) {
    drush_shell_exec_interactive("git clone --recursive --branch 8.x http://git.drupal.org/project/drupal.git $target");
  }
}

function _drupalup_copy_drupal($build) {
  // Get our extension's directory
  $source = dirname(__FILE__) . '/lib/drupal';
  $target = $build[project_path] . '/drupal.git';
  // Copy Drupal from our dir to the project dir
  if (is_dir($source) && is_dir($build[project_path])) {
    drush_copy_dir($source, $target);
  }
}
