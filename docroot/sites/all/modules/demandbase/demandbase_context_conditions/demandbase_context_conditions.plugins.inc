<?php

/**
 * @file
 * Serve demandbase context condition data from demandbase.
 */

/**
 * Context registry.
 */
function _demandbase_context_conditions_context_registry() {
  $registry = array();
  $path = l(t('Demandbase condition'), 'admin/config/services/demandbase/conditions');
  $registry['conditions'] = array(
    'demandbase_context_conditions' => array(
      'title' => t('Demandbase Conditions'),
      'description' => t('Please set the demandbase fields as context condition. These demandbase conditions are set at') . $path,
      'plugin' => 'context_condition_demandbase_context_conditions',
    ),
  );
  return $registry;
}

/**
 * Context plugins.
 */
function _demandbase_context_conditions_context_plugins() {
  $plugins = array();

  // Context condition.
  $plugins['context_condition_demandbase_context_conditions'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'demandbase_context_conditions') . '/plugins',
      'file' => 'demandbase_context_conditions.inc',
      'class' => 'demandbase_context_conditions',
      'parent' => 'context_condition',
    ),
  );
  return $plugins;
}
