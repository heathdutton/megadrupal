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
  $items['provision-dr_run_compare'] = array(
    'description' => 'Run compare for Drulenium',
    //`'drupal dependencies' => array('drulenium_visual_regression'),
    //'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'keep-from' => 'Do not update screenshots for the reference site',
      'keep-to' => 'Do not update screenshots for the site under test',
    ),
    'arguments' => array(
      'from' => 'site alias of the reference site.',
      'to' => 'site alias of the site under test.',
    ),

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

/**
 * Implements the provision-drulenium_dr_run_compare command.
 */
function drush_provision_drulenium_provision_dr_run_compare($alias_from = NULL, $alias_to = NULL) {
  if (d()->name == '@hostmaster') {
    hosting_drulenium_start_compare_task($alias_from, $alias_to);
  }
  else {
    // No OP, this runs in the context of $alias_from. And we want to work in @hostmaster.
  }
}

/**
 * Implements hook_post_hosting_TASK_TYPE_task().
 */
function hosting_drulenium_post_hosting_dr_run_compare_task($task) {
  drush_log(print_r($task, 1), 'info');
  $alias_from = $task->ref->title;
  $alias_to = $task->task_args['reference'];
  hosting_drulenium_start_compare_task($alias_from, $alias_to);

}
function hosting_drulenium_start_compare_task($alias_from = NULL, $alias_to = NULL) {
  drush_errors_on();
  drush_log("Comparing from:$alias_from - to:$alias_to", 'info');

  // Lookup if a release already exists for the aliases.
  $rid_from = _drush_alias_to_release_id($alias_from, 'from');
  $rid_to = _drush_alias_to_release_id($alias_to, 'to');

  // Cheat... assume the diff function can just take the last two create releases.
  //$rid_from = $rid_to = null;

  drush_drulenium_visual_regression_diff($rid_from, $rid_to, $reset = TRUE);
  // Altertative.
  //  provision_backend_invoke('@self', 'vr-diff', array($rid_from, $rid_to));

  // Tell hostmaster that no changes need to be saved to it's drush context.
  drush_set_option('provision_save_config', false);
}

function _drush_alias_to_release_id($alias, $x = 'from') {

  $alias = '@' . ltrim($alias, '@');
  $query = db_select('drulenium_vr_releases', 'vr_re');
  $release_id = $query
    ->fields('vr_re', array('rid'))
    ->condition('vr_re.name', $alias)
    ->execute()->fetchfield();

  if (empty($release_id)) {
    $context = d($alias);
    $auth = '';
    if (!empty($context->http_basic_auth_username)) {
      $auth = $context->http_basic_auth_username . ':' . $context->http_basic_auth_password . '@';
    }

    $scheme = 'http://';
    if (!empty($context->ssl_enabled) && $context->ssl_enabled > 0) {
      $scheme = 'https://';
    }

    $url = $scheme . $auth . $context->uri . '/';
    //$name = "test_release_${alias_context}_" . format_date(REQUEST_TIME);
    $name = $alias;
    provision_backend_invoke('@self', 'vr-create', array($url, $name));

    $query = db_select('drulenium_vr_releases', 'vr_re');
    $release_id = $query
      ->fields('vr_re', array('rid'))
      ->condition('vr_re.name', $name)
      ->execute()->fetchfield();

    // Alternate ... seems to have a batch isuse with multiple batches in one proces.. second screenshot fails.
    //$release_id = drush_drulenium_visual_regression_create($url, "test_release_${alias_context}_" . format_date(REQUEST_TIME));
  }
  elseif ($x == 'from' && !drush_get_option('keep-from', FALSE)
      || ($x == 'to'   && !drush_get_option('keep-to', FALSE))) {
    $name = $alias;
    provision_backend_invoke('@self', 'vr-update', array($name));
  }
  return $release_id;
}


/*
 * Hook executes before code deployment
 */
function drush_provision_drulenium_pre_provision_devshop_deploy() {
  drush_log(dt("[Drulenium] Run drush_provision_drulenium_provision_dr_pre_code_deploy"));
  drush_provision_drulenium_provision_dr_pre_code_deploy();
}
