<?php

/**
 * @file
 * Admin page callbacks for the Nimble CRM Webform module.
 */

/**
 * Settings form for Nimble CRM Webform module.
 */
function nimble_crm_webform_settings_form($form, &$form_state, $node) {
  $form['#node'] = $node;

  $form_state['default_values'] = array(
    'nimble_crm_send' => $node->webform['nimble_crm_send'],
    'nimble_crm_tags' => $node->webform['nimble_crm_tags'],
    'nimble_crm_custom_fields' => unserialize($node->webform['nimble_crm_custom_fields']),
  );

  module_load_include('inc', 'nimble_crm', 'nimble_crm.admin');
  $form += drupal_retrieve_form('nimble_crm_common_contact_settings_form', $form_state);

  // Check token from database.
  $db_token = variable_get('nimble_crm_token');
  if (!empty($db_token['access_token'])) {
    $destination_url = drupal_get_destination();
    // Build fields mappings table.
    $rows = array();
    foreach ($node->webform['components'] as $key => $component) {
      if (!empty($component['extra']['nimble_crm_send'])) {
        if ($component['extra']['nimble_crm_field'] == 'first name') {
          $form_state['nimble_crm_field_first_name_isset'] = TRUE;
        }
        $rows[] = array(
          l($component['name'], 'node/' . $node->nid . '/webform/components/' . $key, array('query' => $destination_url)),
          $component['extra']['nimble_crm_field'],
        );
      }
    }
    $form['nimble_crm_fields_mapping'] = array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          ':input[name="nimble_crm_send"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['nimble_crm_fields_mapping']['table'] = array(
      '#markup' => theme('table', array(
        'caption' => t('Fields mapping'),
        'empty' => t('You need to map your !form_components in field settings.',
          array(
            '!form_components' => l(t('form components'),
              'node/' . $node->nid . '/webform',
              array('query' => $destination_url)),
          )),
        'header' => array(t('Webform Field'), t('Nimble CRM Field')),
        'rows' => $rows,
      )),
    );

  }

  return $form;
}

/**
 * Validate handler for Nimble CRM Webform settings form().
 */
function nimble_crm_webform_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $tags = preg_split("/\w,/", $values['nimble_crm_tags']);
  if (count($tags) > 5) {
    form_set_error('nimble_crm_tags', t('Maximum 5 tags are allowed during contact creation.'));
  }

  if (empty($form_state['nimble_crm_field_first_name_isset'])) {
    form_set_error('nimble_crm_fields_mapping',
      t('First name or last name field is required during contact creation. You need to map your !form_components in field settings.',
        array(
          '!form_components' => l(t('form components'),
            'node/' . $form['#node']->nid . '/webform',
            array('query' => drupal_get_destination())),
        )));
  }
}


/**
 * Submit handler for Nimble CRM Webform settings form().
 */
function nimble_crm_webform_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  // Edit the node by reference just to shorten it up.
  $node = &$form['#node'];
  $node->webform['nimble_crm_send'] = $values['nimble_crm_send'];
  $node->webform['nimble_crm_tags'] = $values['nimble_crm_tags'];
  // Custom fields processing.
  unset($values['nimble_crm_custom_fields']['add_field']);
  unset($values['nimble_crm_custom_fields']['remove_field']);
  $node->webform['nimble_crm_custom_fields'] = $values['nimble_crm_custom_fields'];
  node_save($node);
  drupal_set_message(t('The Nimble CRM form settings have been updated.'));
}
