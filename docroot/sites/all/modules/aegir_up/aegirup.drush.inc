<?php

/**
 * Implementation of hook_vagrant_blueprints
 */
function aegirup_vagrant_blueprints() {
  $blueprints = array(
    'utility' => array(
      'name' => dt('Utility'),
      'description' => dt('A utility server including Git, Jenkins, BIND and Squid servers.'),
      'path' => 'blueprints/utility',
      'build_callback' => 'vagrant_default_build',
      ),
    'maintainer' => array(
      'name' => dt('Maintainer'),
      'description' => dt('An Aegir server with a platform built containing Git repos of a maintainers modules and themes.'),
      'path' => 'blueprints/maintainer',
      'build_callback' => 'aegirup_build_project',
      ),
    'aegir' => array(
      'name' => dt('Aegir'),
      'description' => dt('An Aegir Hosting System server.'),
      'path' => 'blueprints/aegir',
      'build_callback' => 'aegirup_build_project',
    ),
    'aegir2' => array(
      'name' => dt('Aegir 2'),
      'description' => dt('An Aegir Hosting System server, built from the latest 2.x release.'),
      'path' => 'blueprints/aegir2',
      'build_callback' => 'aegirup_build_project',
    ),
    'aegir2-nginx' => array(
      'name' => dt('Aegir 2 (on Nginx)'),
      'description' => dt('An Aegir Hosting System server, built from the latest 2.x release, running on the Nginx webserver.'),
      'path' => 'blueprints/aegir2-nginx',
      'build_callback' => 'aegirup_build_project',
    ),
    'aegir2-dev' => array(
      'name' => dt('Aegir 2 (dev)'),
      'description' => dt('An Aegir Hosting System server, built from the latest 2.x sources.'),
      'path' => 'blueprints/aegir2-dev',
      'build_callback' => 'aegirup_build_project',
    ),
    'aegir3-dev' => array(
      'name' => dt('Aegir 3 (dev)'),
      'description' => dt('An Aegir Hosting System server, built from the latest 3.x sources.'),
      'path' => 'blueprints/aegir3-dev',
      'build_callback' => 'aegirup_build_project',
    ),
    'aegir-dev' => array(
      'name' => dt('Aegir (Dev)'),
      'description' => dt('An Aegir server installed "manually" from Git repos.'),
      'path' => 'blueprints/aegir-dev',
      'build_callback' => 'aegirup_build_project',
    ),
    'aegir-tests' => array(
      'name' => dt('Aegir (Testing)'),
      'description' => dt('An Aegir server w/Jenkins installed to run Selenium test suite.'),
      'path' => 'blueprints/aegir-tests',
      'build_callback' => 'aegirup_build_project',
    ),
    'valkyrie' => array(
      'name' => dt('Valkyrie (dev)'),
      'description' => dt('An Aegir server with DevShop and Valkyrie installed.'),
      'path' => 'blueprints/valkyrie',
      'build_callback' => 'aegirup_build_project',
    ),
    #'civi-dev' => array(
    #  'name' => dt('CiviCRM (Dev)'),
    #  'description' => dt('An Aegir server for developing CiviCRM and extensions.'),
    #  'path' => 'blueprints/civi-dev',
    #  'build_callback' => 'aegirup_build_project',
    #),
  );
  return $blueprints;
}

/**
 * Implementation of hook_drush_command().
 *
 * Req'd for Drush to recognize this file as a Drush extension.
 */
function aegirup_drush_command() {
  $items = array();

  $items['aegirup'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'hidden' => TRUE,
  );

  // Drush topics
  $items['docs-aegirup-readme'] = array(
    'description' => dt('Aegir-up README.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/README.md'),
  );
  $items['docs-aegirup-install'] = array(
    'description' => dt('Aegir-up INSTALL.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/README.md'),
  );

  return $items;
}

/**
 * Custom drush-vagrant build process to instantiate an existing client project
 */
function aegirup_build_project() {
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
  vagrant_default_build_record_blueprint($build);
  vagrant_default_build_user_data($build);
  vagrant_default_build_user_dotfiles($build);
  vagrant_default_build_git_init($build);
}
