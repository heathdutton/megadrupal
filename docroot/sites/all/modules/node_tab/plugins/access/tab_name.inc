<?php

/**
 * @file
 * Plugin to provide an access control handler for tab names.
 */

$plugin = array(
  'title' => t('Node Tab: Tab name'),
  'description' => t('Control access by tab name.'),
  'callback' => 'node_tab_name_ctools_access_check',
  'default' => array('tab_name' => ''),
  'settings form' => 'node_tab_name_ctools_access_settings',
  'summary' => 'node_tab_name_ctools_access_summary',
  'required context' => array(
    new ctools_context_required(t('Node Tab'), 'entity:node_tab'),
    new ctools_context_required(t('Node'), 'entity:node'),
  ),
);

/**
 * Settings form.
 */
function node_tab_name_ctools_access_settings($form, &$form_state, $conf) {
  $form['settings']['tab_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Tab name'),
    '#default_value' => $conf['tab_name'],
  );
  return $form;
}

/**
 * Check for access.
 */
function node_tab_name_ctools_access_check($conf, $context) {
  $context_tab = $context[0]->data;
  $context_node = $context[1]->data;
  if ($conf['tab_name'] == $context_tab->name) {
    return node_tab_access('view', $context_tab, NULL, 'node_tab', $context_node);
  }
  else {
    return FALSE;
  }
}

/**
 * Provide a summary description based on the settings.
 */
function node_tab_name_ctools_access_summary($conf, $context) {
  return t('The tab name is @tab_name', array('@tab_name' => $conf['tab_name']));
}
