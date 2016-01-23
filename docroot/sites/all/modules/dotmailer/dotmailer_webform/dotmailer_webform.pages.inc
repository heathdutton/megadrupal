<?php

/**
 * @file
 *
 */

/**
 * Provides a form for adding dotmailer mappings to a webform. So that you can
 * map fields in the webform to fields in dotMailer
 */
function dotmailer_webform_settings($form, &$form_state, $node) {

  $string_select_options = array();

  $accounts = dotmailer_get_accounts();
  foreach ($accounts as $account) {
    $labels = dotmailer_listcontactdatalabels($account['variable_username'], $account['variable_password']);
    $labels = $labels->ContactDataLabel;

    foreach ($labels as $label) {
      switch ($label->Type) {
        // TODO Add support for other dotMailer field types here.
        case 'String':
          $string_select_options[$label->Name] = $label->Name;
          break;
      }
    }
  }

  if (!$string_select_options) {
    $form['error']['#markup'] = t('No data labels could be retrieved from dotMailer so there is nothing to map against.');
    return;
  }

  $current_settings = isset($node->dotmailer_webform) ? $node->dotmailer_webform['mappings'] : array();
  $webform = $node->webform;

  $mappings = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  foreach ($webform['components'] as $component) {
    switch ($component['type']) {

      // These are all considered 'strings' and will be available to map to
      // string_select_options
      case 'select':
      case 'textfield':
      case 'textarea':
        $mappings[$component['cid']] = array(
          '#type' => 'select',
          '#options' => $string_select_options,
          '#title' => $component['name'],
          '#default_value' => isset($current_settings[$component['cid']]) ? $current_settings[$component['cid']] : '',
          '#empty_value' => '',
        );
        break;
    }
  }

  $form['mappings'] = $mappings;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;

}

function dotmailer_webform_settings_submit($form, $form_state) {
  $node = $form_state['build_info']['args'][0];

  $values = $form_state['values'];
  $node->dotmailer_webform = array(
    'mappings' => $values['mappings'],
  );

  node_save($node);

  drupal_set_message(t('Your changes have been saved'));
}
