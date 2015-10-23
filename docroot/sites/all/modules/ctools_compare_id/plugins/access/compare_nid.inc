<?php

/**
 * @file
 * Plugin to provide access control by checking for equality Node ID.
 */

/**
 * Plugins are described by creating a $plugin array which will be used by the system that includes this file.
 */
$plugin = array(
  'title' => t("Node: Compare ID"),
  'description' => t('Control access by checking for equality ID.'),
  'callback' => 'ctools_compare_id_compare_nid_access_check',
  'settings form' => 'ctools_compare_id_compare_nid_access_edit_form',
  'summary' => 'ctools_compare_id_compare_nid_access_summary',
  'required context' => array(
    new ctools_context_required(t('First Node ID'), 'node'),
  ),
);

/**
 * Settings form.
 */
function ctools_compare_id_compare_nid_access_edit_form($form, &$form_state, $conf) {
  $form['settings']['second_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Second Node ID'),
    '#description' => t('Enter a numeric value ID.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#default_value' => $conf['second_nid'],
  );

  return $form;
}

/**
 * Check for access.
 */
function ctools_compare_id_compare_nid_access_check($conf, $context) {
  if (empty($context) || empty($context[0]->data) || !isset($conf['second_nid']) || empty($conf['second_nid'])) {
    return FALSE;
  }

  $first_node = $context[0]->data;
  $second_node = $conf['second_nid'];

  return ($first_node->nid == $second_node);
}

/**
 * Provide a summary description based upon the specified context.
 */
function ctools_compare_id_compare_nid_access_summary($conf, $context) {
  return t('@id1 ID equality @id2', array('@id1' => $context[0]->identifier, '@id2' => $conf['second_nid']));
}
