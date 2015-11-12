<?php

/**
 * @file
 * Provision/Drush hooks for the provision_drulenium module.
 *
 * These are the hooks that will be executed by the drush_invoke function.
 */

/**
 * Implementation of hook_drush_command().
 */
function provision_drulenium_drush_command() {
  $items['provision-dr_pre_code_deploy'] = array(
    'description' => 'Pre deployment steps for Drulenium',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['provision-dr_post_code_deploy'] = array(
    'description' => 'Post deployment steps for Drulenium',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Implements the provision-drulenium_pre_code_deploy command.
 */
function drush_provision_drulenium_provision_dr_pre_code_deploy() {
  drush_errors_on();
  drush_log(dt('Calling Drulenium baseline snapshots creation task'));
  provision_backend_invoke('@self', 'provision-sync @stage @dev');
  provision_backend_invoke('@self', 'cache-clear all');
  //enable drulenium module if it isn't already
  provision_backend_invoke('@self', 'en drulenium_visual_regression');
  provision_backend_invoke('@self', 'vrc');
}

/**
 * Implements the provision-drulenium_post_code_deploy command.
 */
function drush_provision_drulenium_provision_dr_post_code_deploy() {
  drush_errors_on();
  //provision_backend_invoke(d()->name, 'cache-clear all');
  provision_backend_invoke('@self', 'en drulenium_visual_regression');
  provision_backend_invoke('@self', 'vrc');
  provision_backend_invoke('@self', 'vrd');
  drush_log(dt('Calling Drulenium snapshots creation task'));
  drush_log(dt('Calling Drulenium snapshots diff task'));
}

/*
 * Hook executes before code deployment
 */
function drush_provision_drulenium_pre_provision_devshop_deploy() {
  drush_log(dt("[Drulenium] Run drush_provision_drulenium_provision_dr_pre_code_deploy"));
  drush_provision_drulenium_provision_dr_pre_code_deploy();
}
