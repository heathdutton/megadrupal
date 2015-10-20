<?php

/**
 * @file
 *
 * Plugin to provide a new party context with a preconfigured set of hats.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('New Party (with Hats)'),
  'description' => t('Create a new party with some hats already selected.'),
  'context' => 'party_hat_context_create_party_with_hats',
  'edit form' => 'party_hat_context_party_with_hats_settings_form',
  'defaults' => array('hats' => array()),
  'keyword' => 'new_party',
  'context name' => 'party_new_with_hats',
);

/**
 * Create the context
 */
function party_hat_context_create_party_with_hats($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context(array('entity:party', 'entity', 'party'));
  $context->plugin = 'party_new_with_hats';

  if ($empty) {
    return $context;
  }

  // Create blank party
  $context->data = party_create();

  // Set hats
  if (!empty($data['hats'])) {
    foreach ($data['hats'] as $hat_name) {
      $context->data->party_hat[LANGUAGE_NONE][] = array(
        'hat_name' => $hat_name,
      );
    }
  }

  // Build context.
  if (!empty($context->data)) {
    $context->title = t('New Party');
    return $context;
  }
}

/**
 * Build settings form.
 */
function party_hat_context_party_with_hats_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['hats'] = array(
    '#title' => t('Hats'),
    '#type' => 'checkboxes',
    '#options' => party_hat_build_hat_options(NULL, NULL, 'edit'),
    '#default_value' => $conf['hats'],
  );

  return $form;
}

/**
 * Submit the settings form
 */
function party_hat_context_party_with_hats_settings_form_submit($form, &$form_state) {
  $form_state['conf']['hats'] = drupal_map_assoc($form_state['values']['hats']);
  unset($form_state['conf']['hats'][0]);
}
