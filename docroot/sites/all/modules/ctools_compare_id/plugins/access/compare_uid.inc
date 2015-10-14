<?php

/**
 * @file
 * Plugin to provide access control by checking for equality User ID.
 */

/**
 * Plugins are described by creating a $plugin array which will be used by the system that includes this file.
 */
$plugin = array(
  'title' => t("User: Compare ID"),
  'description' => t('Control access by checking for equality ID.'),
  'callback' => 'ctools_compare_id_compare_uid_access_check',
  'settings form' => 'ctools_compare_id_compare_uid_access_edit_form',
  'summary' => 'ctools_compare_id_compare_uid_access_summary',
  'required context' => array(
    new ctools_context_required(t('First User ID'), 'user'),
  ),
);

/**
 * Settings form.
 */
function ctools_compare_id_compare_uid_access_edit_form($form, &$form_state, $conf) {
  $form['settings']['second_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('Second User ID'),
    '#description' => t('Enter a numeric value ID.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#default_value' => $conf['second_uid'],
  );

  return $form;
}

/**
 * Check for access.
 */
function ctools_compare_id_compare_uid_access_check($conf, $context) {
  if (empty($context) || empty($context[0]->data) || !isset($conf['second_uid']) || empty($conf['second_uid'])) {
    return FALSE;
  }

  $first_user = $context[0]->data;
  $second_user = $conf['second_uid'];

  return ($first_user->uid == $second_user);
}

/**
 * Provide a summary description based upon the specified context.
 */
function ctools_compare_id_compare_uid_access_summary($conf, $context) {
  return t('@id1 ID equality @id2', array('@id1' => $context[0]->identifier, '@id2' => $conf['second_uid']));
}
