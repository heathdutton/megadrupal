<?php
/**
 * @file
 * Provides admin settings page to adjust form submission settings.
 */

/**
 * Form constructor for the Hubspot settings form.
 *
 * @param $form
 * @param $form_state
 * @param object $node
 *   The node using Hubspot.
 *
 * @return array
 */
function hubspot_form_settings($form, $form_state, $node) {
  $form = array();

  $hubspot_forms = _hubspot_get_forms();

  if (isset($hubspot_forms['error'])) {
    $form['webforms']['#description'] = $hubspot_forms['error'];
  }
  else {
    if (empty($hubspot_forms['value'])) {
      $form['webforms']['#description'] = t('No HubSpot forms found. You will need to create a form on HubSpot before you can configure it here.');
    }
    else {
      $hubspot_form_options = array("--donotmap--" => "Do Not Map");
      $hubspot_field_options = array();
      foreach ($hubspot_forms['value'] as $hubspot_form) {
        $hubspot_form_options[$hubspot_form['guid']] = $hubspot_form['name'];
        $hubspot_field_options[$hubspot_form['guid']]['fields']['--donotmap--'] = "Do Not Map";

        foreach ($hubspot_form['fields'] as $hubspot_field) {
          $hubspot_field_options[$hubspot_form['guid']]['fields'][$hubspot_field['name']] = ($hubspot_field['label'] ? $hubspot_field['label'] : $hubspot_field['name']) . ' (' . $hubspot_field['fieldType'] . ')';
        }
      }

      $nid = $node->nid;

      $form['nid'] = array(
        '#type' => 'hidden',
        '#value' => $nid,
      );

      $form['hubspot_form'] = array(
        '#title' => t('HubSpot form'),
        '#type' => 'select',
        '#options' => $hubspot_form_options,
        '#default_value' => _hubspot_default_value($nid),
      );

      foreach ($hubspot_form_options as $key => $value) {
        if ($key != '--donotmap--') {
          $form[$key] = array(
            '#title' => t('Field mappings for @field', array('field' => $value)),
            '#type' => 'fieldset',
            '#tree' => TRUE,
            '#states' => array(
              'visible' => array(
                ':input[name="hubspot_form"]' => array('value' => $key),
              ),
            ),
          );

          foreach ($node->webform['components'] as $component) {
            if ($component['type'] == 'addressfield' && module_exists('addressfield_tokens')) {
              $addressfield_fields = addressfield_tokens_components();

              foreach ($addressfield_fields as $addressfield_key => $addressfield_value) {
                $form[$key][$component['form_key'] . '_' . $addressfield_key] = array(
                  '#title' => $component['name'] . ': ' . $addressfield_value . ' (' . $component['type'] . ')',
                  '#type' => 'select',
                  '#options' => $hubspot_field_options[$key]['fields'],
                  '#default_value' => _hubspot_default_value($nid, $key, $component['form_key'] . '_' . $addressfield_key),
                );
              }
            }
            elseif ($component['type'] !== 'markup') {
              $form[$key][$component['form_key']] = array(
                '#title' => $component['name'] . ' (' . $component['type'] . ')',
                '#type' => 'select',
                '#options' => $hubspot_field_options[$key]['fields'],
                '#default_value' => _hubspot_default_value($nid, $key, $component['form_key']),
              );
            }
          }
        }
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ('Save Configuration'),
  );

  return $form;
}

/**
 * Submit handler for hubspot_form_settings form.
 */
function hubspot_form_settings_submit($form, &$form_state) {
  $txn = db_transaction();

  db_delete('hubspot')->condition('nid', $form_state['values']['nid'])->execute();

  if ($form_state['values']['hubspot_form'] != '--donotmap--') {
    foreach ($form_state['values'][$form_state['values']['hubspot_form']] as $webform_field => $hubspot_field) {
      $fields = array(
        'nid' => $form_state['values']['nid'],
        'hubspot_guid' => $form_state['values']['hubspot_form'],
        'webform_field' => $webform_field,
        'hubspot_field' => $hubspot_field,
      );
      db_insert('hubspot')->fields($fields)->execute();
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}
