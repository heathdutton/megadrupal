<?php

/**
 * @file
 * Context hooks.
 */

/**
 * Implements hook_context_page_condition().
 */
function domain_types_context_page_condition() {
  global $_domain;

  if ($plugin = context_get_plugin('condition', 'domain_type')) {
    $plugin->execute($_domain['x_type']);
  }
}

/**
 * Implements hook_context_registry().
 */
function domain_types_context_registry() {
  return array(
    'conditions' => array(
      'domain_type' => array(
        'title' => t('Active domain type'),
        'description' => t('Enables context if the active domain is of this type.'),
        'plugin' => 'domain_types_context_condition_domain_type',
      ),
    ),
  );
}

/**
 * Implements hook_context_plugins().
 */
function domain_types_context_plugins() {
  $plugins = array();
  $plugins['domain_types_context_condition_domain_type'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'domain_types'),
      'file' => 'domain_types.context.inc',
      'class' => 'domain_types_context_condition_domain_type',
      'parent' => 'context_condition',
    ),
  );
  return $plugins;
}
