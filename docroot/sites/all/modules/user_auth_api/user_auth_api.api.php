<?php

/**
 * @file
 * API functions.
 */

/**
 * Implements hook_user_auth_api_info().
 */
function hook_user_auth_api_info() {
  return array(
    'my_plugin' => 'MyPluginClassForUserAuthApi',
  );
}

/**
 * Implements hook_user_auth_api_info_alter().
 */
function hook_user_auth_api_info_alter(&$plugins) {
  $plugins['some_plugin']['weight'] = -100;
}
