<?php
/**
 * @file
 * This file contains the admin and GUI callbacks for the module that are not needed in day-to-day operations
 */

/**
 * Menu callback for node/%/webform/exacttarget_soap.
 */
function webform_exacttarget_soap_configure_page($node) {
  return drupal_get_form('webform_exacttarget_soap_configure_form', $node);
}

/**
 * Menu callback for admin/config/services/exact_target_soap/webform.
 */
function webform_exacttarget_soap_config() {
  $form = array();
  $options = array();

  $request = new ExactTarget_SystemStatusResponseMsg();

  try {
    $result = @ExactTarget::instance()->GetSystemStatus($request);
  } catch (Exception $e) {
    drupal_set_message(t('Your site failed to connect to the ExactTarget SOAP API.'), 'warning');
  }

  if (!isset($result->Results->Result->SystemStatus)) {
    drupal_set_message(t('Your site failed to connect to the ExactTarget SOAP API.'), 'warning');
  }
  elseif ($result->Results->Result->SystemStatus != 'OK') {
    drupal_set_message(t('There was a problem connecting to the ExactTarget SOAP API: @status: @message.'), array('@status' => filter_xss($result->Results->Result->SystemStatus), '@message' => filter_xss($result->Results->Result->StatusMessage)), 'warning');
  }
  else {
    drupal_set_message(t('Your site is connected to the ExactTarget SOAP API.'));
  }

  $rr = new ExactTarget_RetrieveRequest();
  $rr->ObjectType = "DataExtension";
  // Set the properties to return
  // @see http://help.exacttarget.com/en/technical_library/web_service_guide/objects/dataextension/
  $props = array("ObjectID", "CustomerKey");
  $rr->Properties = $props;
  // Return all MIDs
  $rr->Filter = NULL;

  $rrm = new ExactTarget_RetrieveRequestMsg();
  $rrm->RetrieveRequest = $rr;
  try {
    $results = @ExactTarget::instance()->Retrieve($rrm);
  } catch (Exception $e) {
    $results = FALSE;
  }

  if (!empty($results)) {
    if ($results->OverallStatus == 'OK') {
      foreach ($results->Results as $de) {
        $options[$de->CustomerKey] = $de->CustomerKey;
      }
    }
    else {
      drupal_set_message(filter_xss($results->OverallStatus), 'warning');
    }
  }

  $default_options = variable_get('webform_exacttarget_soap_enabled_data_extensions', array());

  $form['webform_exacttarget_soap_enabled_data_extensions'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Enabled Data Extensions'),
    '#title_display' => 'before',
    '#default_value' => $default_options,
    '#description' => t('Defines which Data Extensions are available for webform submission mapping on this site.'),
  );
  return system_settings_form($form);
}


/**
 * Implements callback for form construction.
 */
function webform_exacttarget_soap_configure_form($form, $form_state, $node) {
  $form = array();
  $form_cfg = webform_exacttarget_soap_get_map($node);

  // Filter out enabled data extensions and field/component combos
  $node_de_enabled = array();
  $node_de_map = array();
  foreach ($form_cfg as $mapping) {
    array_push($node_de_enabled, $mapping['de_key']);
    if (isset($mapping['de_property']) && isset($mapping['component'])) {
      $node_de_map[$mapping['de_property']] = $mapping['component'];
    }
  }
  // Make sure only unique values exist
  array_unique($node_de_enabled);

  // Get all the enabled ExactTarget data extensions for the site
  $data_extensions = webform_salesforce_soap_describe_enabled_de();
  $data_extensions_fields = array();

  if (!empty($node_de_enabled)) {
    foreach ($node_de_enabled as $ext) {
      $data_extensions_fields[$ext] = webform_exacttarget_soap_get_data_extension_fields($ext);
    }
  }

  // Data extensions enabled on the site
  if (!empty($data_extensions)) {
    $et_list = array();
    foreach ($data_extensions as $key => $data) {
      $et_list[$key] = $data['value'];
    }

    $form['et_setup'] = array(
      '#type' => 'container',
    );
    $form['et_setup']['et_enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($node_de_enabled) ? 1 : 0,
      '#title' => t('Enable form submissions integration with Exact Target SOAP API'),
      '#description' => t('Field mapping and data extension selection are only stored if ExactTarget is enabled for this Webform.'),
    );
    $form['et_setup']['et_de'] = array(
      '#type' => 'checkboxes',
      '#title' => t('ExactTarget Data Extension'),
      '#description' => t('Please select which data extension will be created in ExactTarget via this Webform.'),
      '#options' => $data_extensions,
      '#default_value' => $node_de_enabled,
      '#required' => FALSE,
    );

    // Get all available Webform fields and build option list for mapping
    if (!empty($node->webform['components'])) {
      $wf_fields = array('' => t('- Select -'));
      foreach ($node->webform['components'] as $field) {
        $name = $field['name'];
        if ($field['mandatory']) {
          $name .= ' *';
        }
        $wf_fields[$field['form_key']] = $name;
      }
    }
    else {
      drupal_set_message(t('Please add some webform form components!'), 'notice');
    }

    // Do not show mapping unless there are some webform components and enabled
    // data extensions for the node.
    if (isset($wf_fields) && !empty($node_de_enabled)) {
      $form['et_fields'] = array(
        '#type' => 'fieldset',
        '#title' => t('Field Mapping'),
        '#description' => t('Define mapping between ExactTarget Data Extension fields and Webform form components'),
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      foreach ($data_extensions_fields as $ext => $fields) {
        $key = 'set_' . str_replace(' ', '_', $ext);
        $form['et_fields'][$key] = array(
            '#type' => 'fieldset',
            '#title' => $ext,
            '#collapsible' => TRUE,
            '#description' => t('Mapping for fields in !ext Data Extension', array('!ext' => $ext)),
          );
        foreach ($fields as $attr) {
          $defval = (!empty($node_de_map[$attr['key']]) ? $node_de_map[$attr['key']] : '');

          $label =  $attr['label'];
          $required = $attr['required'];
          if (!empty($attr['primary_key'])) {
            $label .= ' ' . t('(Primary key)');
            $required = TRUE;
            // Hidden form element to pass primary keys
            $form['et_fields']['primary'][$attr['key']] = array(
              '#type' => 'value',
              '#value' => 1,
            );
          }
          $form['et_fields'][$key][$attr['key']] = array(
            '#type' => 'select',
            '#title' => $label,
            '#options' => $wf_fields,
            '#required' => $required,
            '#default_value' => $defval,
            // Hide the mapping when the data extension is not enabled
            'invisible' => array(
              ':input[name="et_de[' . $ext . ']"]' => array('checked' => FALSE),
            ),
          );
        }
      }

      $form['wfet_mappings_info'] = array(
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
  }
  else {
    $form['msg'] = array(
      '#type' => 'markup',
      '#value' => '<p>' . t('No fields are defined for this object, or else there was an error retrieving the fields.  Please !check_log_link for errors.', array('!check_log_link' => l(t('check the log'), 'admin/reports/dblog'))) . '</p>',
    );
  }
  return $form;
}


/**
 * Implement callback for submit handling of form 'webform_exacttarget_soap_configure_form'
 */
function webform_exacttarget_soap_configure_form_submit($form, &$form_state) {
 // Only store mapping if ET is enabled
  $values = $form_state['values'];

  $node = node_load($form_state['values']['nid'], $form_state['values']['vid']);
  if ($values['et_enabled'] == 1) {
    $mapping['de'] = $values['et_de'];
    if (!empty($values['et_fields'])) {
      foreach ($values['et_fields'] as $property => $fields) {
        if ($property == 'primary') {
          $mapping['primary_keys'] = array();
          if (!empty($fields)) {
            foreach ($fields as $key => $data) {
              array_push($mapping['primary_keys'], $key);
            }
          }
        }
        else {
          foreach ($fields as $key => $val) {
            if (!empty($val)) {
              $mapping['map'][$key] = $val;
            }
          }
        }
      }
    }

    _webform_exacttarget_soap_set_map($node, $mapping);
  }
  else {
    _webform_exacttarget_soap_delete_map($node);
  }
  drupal_set_message(t('Webform ExactTarget SOAP settings successfully saved.'), 'status');
}