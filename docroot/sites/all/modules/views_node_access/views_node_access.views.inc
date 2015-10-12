<?php

/**
 * @file
 * Declaration of views node access types.
 */

/**
 * Implements hook_views_plugins().
 */
function views_node_access_views_plugins() {
  $plugins = array(
    'access' => array(
      'node_types' => array(
        'title' => t('Node type'),
        'help' => t('Access will be granted to specified node types.'),
        'handler' => 'views_node_access_plugin_access_node_type',
        'help topic' => 'access-node-type',
        'uses options' => TRUE,
        'base' => array('node', 'comment')
      ),
      'node_types_perm' => array(
        'title' => t('Node type and permission'),
        'help' => t('Access will be granted to specified node types and permissions.'),
        'handler' => 'views_node_access_plugin_access_node_type_perm',
        'help topic' => 'access-node-type-perm',
        'uses options' => TRUE,
        'base' => array('node', 'comment')
      ),
      'node_types_role' => array(
        'title' => t('Node type and role'),
        'help' => t('Access will be granted to specified node types and roles.'),
        'handler' => 'views_node_access_plugin_access_node_type_role',
        'help topic' => 'access-node-type-role',
        'uses options' => TRUE,
        'base' => array('node', 'comment')
      ),
      'node_types_current_user' => array(
        'title' => t('Node type and current user'),
        'help' => t('Access will be granted to specified node types and the current user.'),
        'handler' => 'views_node_access_plugin_access_node_type_current_user',
        'help topic' => 'access-node-type-current-user',
        'uses options' => TRUE,
        'base' => array('node', 'comment')
      ),
      'current_user' => array(
        'title' => t('Current user'),
        'help' => t('Access will be granted to current user.'),
        'handler' => 'views_node_access_plugin_access_current_user',
        'help topic' => 'access-current-user',
        'uses options' => FALSE,
        'base' => array('node', 'comment')
      ),
    )
  );
  return $plugins;
}
