<?php

/**
 * @file
 * Plugin to provide an argument handler for tabs
 */

$plugin = array(
  'title' => t("Node Tab: Name"),
  'description' => t('Creates a node tab context from a tab machine name argument.'),
  'context' => 'node_tab_argument_context',
  'keyword' => 'node_tab',
  'default' => array(
    'node_type' => '',
  ),
  'settings form' => 'node_tab_argument_settings_form',
);

/**
 * Context callback: Create the tab context.
 */
function node_tab_argument_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('entity:node_tab');
  }
  // We can accept either an entity object or a pure id.
  if (is_object($arg)) {
    return ctools_context_create('entity:node_tab', $arg);
  }

  $tab = node_tab_load($conf['node_type'], $arg);
  if (!$tab) {
    return FALSE;
  }

  return ctools_context_create('entity:node_tab', $tab);
}

/**
 * Settings form callback: returns the settings for this argument.
 */
function node_tab_argument_settings_form(&$form, $form_state, $conf) {
  $types = node_type_get_types();
  $options = array();
  foreach ($types as $type => $info) {
    $options[$type] = check_plain($info->name);
  }

  $form['settings']['node_type'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#options' => array('- Please select -') + $options,
    '#default_value' => $conf['node_type'],
  );

  return $form;
}
