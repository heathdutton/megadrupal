<?php
/**
 * @file
 * This file contains the admin and GUI callbacks for the module that are not needed in day-to-day operations
 */

function webform_salesforce_configure_page($node) {
  return drupal_get_form('webform_salesforce_configure_form', $node);
}

/*
 * Implements callback for form construction
 */
function webform_salesforce_configure_form($form, $form_state, $node) {
  $form = array();
  $form_cfg = webform_salesforce_get_map($node);
  $map = (!empty($form_cfg['map'])) ? $form_cfg['map'] : array();
  $def_obj = (!empty($form_cfg['object'])) ? $form_cfg['object'] : '';

  // Get all the allowed SF objects
  $sf_options = webform_salesforce_describe_objects();

  if (!empty($sf_options)) {
    // Get all available SF objects
    $sf_list = array();
    foreach ($sf_options as $key => $data) {
      $sf_list[$key] = $data['value'];
    }

    $form['sf_setup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Salesforce Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );
    $form['sf_setup']['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($def_obj) ? 1 : 0,
      '#title' => t('Enabled'),
      '#description' => t('Field mapping and object selection are only stored if Salesforce is enabled for this Webform.'),
    );
    $form['sf_setup']['sf_object'] = array(
      '#type' => 'radios',
      '#title' => t('Salesforce Object'),
      '#description' => t('Please select which object will be created in Salesform via this Webform.'),
      '#options' => $sf_list,
      '#default_value' => $def_obj,
      '#required' => FALSE,
    );

    // Get all available Webform fields and build option list for mapping
    if (!empty($node->webform['components'])) {
      $wf_fields = array('' => '- Select -');
      $wf_fields_req = array('' => '- Select -');
      foreach ($node->webform['components'] as $field) {
        $name = $field['name'];
        if ($field['mandatory']) {
          $name .= ' *';
        }
        $wf_fields[$field['form_key']] = $name;
        if ($field['mandatory']) {
          $wf_fields_req[$field['form_key']] = $name;
        }
      }
    }
    else {
      drupal_set_message(t('Please add some webform form components!'), 'notice');
    }

    // Do not show mapping unless there are some webform components
    if (isset($wf_fields) && !empty($def_obj)) {
      // Get field definitions for the selected object
      $sf_object_fields = webform_salesforce_get_available_fields($def_obj);

      $form['sf_fields'] = array(
        '#type' => 'fieldset',
        '#title' => t('Field Mapping'),
        '#description' => t('Define mapping between Salesforce fields and Webform form components'),
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      foreach ($sf_object_fields as $sf_object_type) {
        foreach ($sf_object_type as $attr) {
          $defval = (!empty($map[$attr['fieldid']]) ? $map[$attr['fieldid']] : '');
          
          $label = $attr['required'] ? check_plain($attr['label']) . ' <span class="form-required">(' . t('Required') . ')</span>' : check_plain($attr['label']);
          $form['sf_fields'][$attr['fieldid']] = array(
            '#type' => 'select',
            '#title' => $label,
            '#options' => ($attr['required']) ? $wf_fields_req : $wf_fields,
            '#default_value' => $defval,
          );
        }
      }
      
      $form['wfsf_mappings_info'] = array(
        '#type' => 'markup',
        '#value' => t('<p>Select the Webform field that should be mapped to the Salesforce field.<br />All required fields are designated with a <strong>*</strong> next to their name.</p>'),
      );
    }

    // Set node info as it doesn't get passed to the submit handler

    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid,
    );
    $form['vid'] = array(
      '#type' => 'hidden',
      '#value' => (!empty($node->vid)) ? $node->vid : 0,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => 'Reset Form',
      '#validate' => array('webform_salesforce_configure_form_clear'),
    );
  }
  else {
    $form['msg'] = array(
      '#type' => 'markup',
      '#value' => '<p>' . t('No fields are defined for this object, or else there was an error retrieving the fields.  Please !check_log_link for errors.', array('!check_log_link' => l(t('check the log'), 'admin/reports/dblog'))) . '</p>',
    );
  }
  return $form;
}

/*
 * Validate function for our reset button
 */
function webform_salesforce_configure_form_clear($form, &$form_state) {
  unset($form_state['values']);
  unset($form_state['storage']);

  $form_state['rebuild'] = TRUE;
}

/*
 * Implement callback for validation of form 'webform_exact_target_api_configure_form'
 */
function webform_salesforce_configure_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['sf_setup']['enabled'] == 1 && isset($values['sf_fields'])) {
    foreach ($values['sf_fields'] as $id => $val) {
      // Check that a source field was selected:

      if ($form['sf_fields'][$id]['#required'] && '0' == $val) {
        form_set_error($id, t('No source Webform field selected for required Salesforce field %field.  Please select a Webform field from the dropdown.', array('%field' => $form['sf_fields'][$id]['#title'])));
      }
    }
  }
}

/*
 * Implement callback for submit handling of form 'webform_exact_target_api_configure_form'
 */
function webform_salesforce_configure_form_submit($form, &$form_state) {
 // Only store mapping if SF is enabled
  $values = $form_state['values'];
  $node = node_load($form_state['values']['nid'], $form_state['values']['vid']);
  if ($values['sf_setup']['enabled'] == 1) {
    $mapping['object'] = $values['sf_setup']['sf_object'];
    if (!empty($values['sf_fields'])) {
      foreach ($values['sf_fields'] as $key => $val) {
        if (!empty($val)) {
          $mapping['map'][$key] = $val;
        }
      }
    }

    _webform_salesforce_set_map($node, $mapping);
  }
  else {
    _webform_salesforce_delete_map($node);
  }
  drupal_set_message(t('Webform Salesforce settings successfully saved.'), 'status');
}
