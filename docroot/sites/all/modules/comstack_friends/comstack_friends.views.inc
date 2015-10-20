<?php

/**
 * @file
 * Comstack Friends extra views stuff.
 */

/**
 * Implements hook_views_plugins().
 */
function comstack_friends_views_plugins() {
  $plugins = array(
    'access' => array(
      'comstack_friends_current_perm' => array(
        'title' => t('Current user and permission or bypass permission'),
        'help' => t('Access will be granted to the current user with a selected permission or if they have a bypass permission.'),
        'handler' => 'comstack_friends_plugin_access_current_perm',
        'uses options' => TRUE,
        'help topic' => 'access-perm',
        'base' => array('users', 'user_relationships'),
      ),
    )
  );
  return $plugins;
}

/**
 * Implements hook_views_data_alter().
 */
function comstack_friends_views_data_alter(&$data) {
  $data['users']['comstack_friends_link_block'] = array(
    'real field' => 'uid',
    'field' => array(
      'title' => t('Comstack Friends user relationship block action link'),
      'help' => t('This will generate a link to add or remove a user relationship with a given user.'),
      'handler' => 'comstack_friends_handler_field_users_link_block',
    ),
    'group' => t('User relationships'),
  );
  $data['users']['comstack_friends_link_friend'] = array(
    'real field' => 'uid',
    'field' => array(
      'title' => t('Comstack Friends user relationship friend action link'),
      'help' => t('This will generate a link to add or remove a user relationship with a given user.'),
      'handler' => 'comstack_friends_handler_field_users_link_friend',
    ),
    'group' => t('User relationships'),
  );
  $data['users_roles']['comstack_friends_filter_exclude_roles'] = array(
    'real field' => 'rid',
    'filter' => array(
      'title' => t('Comstack Friends exclude roles'),
      'help' => t('Exclude any users with the role(s) that are not allowed to use Comstack friends functionality.'),
      'handler' => 'comstack_friends_handler_filter_exclude_roles',
      'numeric' => TRUE,
      'allow empty' => FALSE,
    ),
    'group' => t('User relationships'),
  );
}
