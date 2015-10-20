<?php

/**
 * @file
 * Plugin to provide access control based upon hat context.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("User's Party Hat"),
  'description' => t("Control access by a user's party's hat."),
  'callback' => 'party_hat_user_party_hat_ctools_access_check',
  'settings form' => 'party_hat_user_party_hat_access_settings',
  'settings form validation' => 'ctools_term_ctools_access_settings_validate',
  'settings form submit' => 'ctools_term_ctools_access_settings_submit',
  'summary' => 'party_hat_user_party_hat_access_summary',
  'required context' => new ctools_context_required(t('User'), 'user'),
);

/**
 * Settings form for the User's Party Hat access plugin
 */
function party_hat_user_party_hat_access_settings($form, &$form_state, $conf) {
  $form['settings']['#tree'] = TRUE;
  $form['settings']['hat_name'] = array(
    '#title' => t('Hats'),
    '#description' => t('Select one or more hats.'),
    '#default_value' => !empty($conf['hat_name']) ? $conf['hat_name'] : '',
    '#multiple' => TRUE,
    '#type' => 'checkboxes',
    '#options' => array(),
  );
  // @todo: replace with party_het_get_tree when drupal.org/node/1613400 goes in
  foreach (party_hat_get_all_hats() as $hat_name => $hat) {
    $form['settings']['hat_name']['#options'][$hat->name] = $hat->label;
  }

  return $form;
}

/**
 * Check for access.
 *
 * This plugin returns true if a party has ATLEAST ONE of the specified hats.
 * It does not require the party to have all of them.
 */
function party_hat_user_party_hat_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data)) {
    return FALSE;
  }

  // Get the hats.
  if (!isset($conf['hat_name'])) {
    return FALSE;
  }

  $return = FALSE;

  // Get the party from the user context
  $party = party_user_get_party($context->data);
  $hats = array_filter($conf['hat_name']);
  $party_hats = party_hat_get_hats($party);

  $intersect = array_intersect($hats, array_keys($party_hats));

  // If the party has any hats that are also in the conf array, return true.
  if (!empty($intersect)) {
    $return = TRUE;
  }

  return $return;
}

/**
 * Provide a summary description based upon the checked terms.
 */
function party_hat_user_party_hat_access_summary($conf, $context) {
  $hats = array();
  foreach (array_filter($conf['hat_name']) as $hat_name) {
    $hat = party_hat_load($hat_name);
    $hats[$hat->name] = $hat->label;
  }

  return format_plural(count($hats),
    '@hat can be the hat "@hats"',
    '@hat can be one of these hats: @hats',
    array('@hats' => implode(', ', $hats),
      '@hat' => $context->identifier));
}
