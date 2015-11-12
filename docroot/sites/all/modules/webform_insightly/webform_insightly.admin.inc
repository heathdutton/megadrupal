<?php

/**
 * @file
 * Functions to save/retrive mapping details and render configuration forms.
 */

/**
 * Callback from webform_insightly_menu().
 */
function webform_insightly_add_form($form, &$form_state) {
  $accepted_fields = array(
    'FIRST_NAME',
    'LAST_NAME',
    'BACKGROUND',
    'IMAGE_URL',
    'EMAIL',
    'PHONE',
    'BID_AMOUNT',
  );
  variable_set('webform_insightly_accepted_fields', $accepted_fields);
  $form['webform_insightly_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('webform_insightly_api_key', ''),
    '#description' => t('Enter the insightly API key.'),
    '#required' => TRUE,
  );
  $form['import'] = array(
    '#type' => 'markup',
    '#markup' => '<ul class="action-links">
      <li><a href="/admin/insightly-mapping/import">Import</a></li>
      </ul>',
    '#tree' => TRUE,
  );
  $form['#tree'] = TRUE;
  $header = array('FORM_IDs', 'OPERATIONS', '', '');
  $results = webform_insightly_get_all_field_mapping();
  foreach ($results as $result) {
    $rows[] = array(
      $result->form_id,
      l(t('edit'),
      'admin/insightly-mapping/edit/' . $result->id),
      l(t('delete'),
      'admin/insightly-mapping/delete/' . $result->id),
      l(t('export'),
      'admin/insightly-mapping/export/' . $result->id),
    );
  }
  if ($rows != NULL) {
    $form['insightly_table_title'] = array(
      '#type' => 'markup',
      '#markup' => '<h3>Currently Mapped forms</h3>',
      '#tree' => TRUE,
    );
    $form['mapped_form_table'] = array(
      '#type' => 'markup',
      '#title' => t('Current Mapped forms'),
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
      '#tree' => TRUE,
    );
  }
  $form['insightly_form_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Webform id'),
  );
  $field_list_string = '';
  foreach ($accepted_fields as $accepted_field) {
    $field_list_string .= $accepted_field . ', ';
  }
  $field_list_string = substr_replace(trim($field_list_string), "", -1);
  $form['insightly_fields_markup'] = array(
    '#markup' => t('Available fields in insightly to set target values are :field_list_string All other data field will be added as CUSTOMFIELDS (You need to add custom fields at System Settings > Custom Fields).', array(':field_list_string' => $field_list_string . '.')),
    '#prefix' => '<div class="clear">',
    '#suffix' => '</div>',
  );
  $form['insightly_mapping_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mapping'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="insightly-fieldset-wrapper" class="row">',
    '#suffix' => '</div>',
  );
  // Build the fieldset with the proper number of mappings. We'll use
  // $form_state['num_fields'] to determine the number of textfields to build.
  if (empty($form_state['num_fields'])) {
    $form_state['num_fields'] = 1;
  }
  for ($i = 0; $i < $form_state['num_fields']; $i++) {
    $form['insightly_mapping_fieldset'][$i]['insightly_source'] = array(
      '#type' => 'textfield',
      '#title' => t('Source'),
      '#prefix' => '<div class="col-source">',
      '#suffix' => '</div>',
    );
    $form['insightly_mapping_fieldset'][$i]['insightly_target'] = array(
      '#type' => 'textfield',
      '#title' => t('Target'),
      '#prefix' => '<div class="col-target">',
      '#suffix' => '</div>',
    );
    $form['insightly_mapping_fieldset'][$i]['insightly_remove'] = array(
      '#title' => t('Remove'),
      '#type' => 'checkbox',
      '#default_value' => 0,
      '#prefix' => '<div class="col-remove">',
      '#suffix' => '</div>',
    );
  }
  $form['insightly_add_field'] = array(
    '#type' => 'submit',
    '#value' => t('Add Field'),
    '#submit' => array('webform_insightly_add_more_fields'),
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => array(
      'callback' => 'webform_insightly_add_more_callback',
      'wrapper' => 'insightly-fieldset-wrapper',
    ),
  );
  $form['insightly_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#validate' => array('webform_insightly_add_form_validate'),
    '#submit' => array('webform_insightly_add_form_submit'),
  );
  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the mapping in it.
 */
function webform_insightly_add_more_callback($form, $form_state) {
  return $form['insightly_mapping_fieldset'];
}

/**
 * Function to validate webform_insightly_add_form.
 */
function webform_insightly_add_form_validate($form, &$form_state) {
  $form_id = $form_state['values']['insightly_form_id'];
  $results = db_query("SELECT id FROM {insightly}
    WHERE form_id = :form_id", array(':form_id' => $form_id));
  foreach ($results as $result) {
    form_set_error('insightly_form_id', t('The entered form is already mapped!'));
  }
}

/**
 * Submit handler to save the mapping configuration.
 */
function webform_insightly_add_form_submit($form, &$form_state) {
  variable_set('webform_insightly_api_key', $form_state['values']['webform_insightly_api_key']);
  $form_id = $form_state['values']['insightly_form_id'];
  if (!$form_id) {
    drupal_set_message(t("No Mapping details has been added! API key has been saved."), 'warning');
    return TRUE;
  }
  foreach ($form_state['values']['insightly_mapping_fieldset'] as $value) {
    if ($value['insightly_remove'] == 0) {
      if (trim($value['insightly_source']) != NULL) {
        $mapping[$value['insightly_source']] = $value['insightly_target'];
      }
    }
  }
  $mapping = json_encode($mapping);
  db_query("INSERT INTO {insightly} (form_id, mapping)
    VALUES (:form_id, :mapping)",
    array(
      ':form_id' => $form_id,
      ':mapping' => $mapping
    )
  );
}

/**
 * Function for edit webform_insightly_form.
 */
function webform_insightly_edit_form($form, &$form_state, $id = NULL) {
  $form['#tree'] = TRUE;
  $form_state['form_table_id'] = $id;
  if (empty($form_state['num_fields'])) {
    $form_state['num_fields'] = 0;
  }
  $mapping_details = webform_insightly_get_field_mapping($id);
  $mapping_details['mapping'] = json_decode($mapping_details['mapping'], TRUE);
  // If get wrong id then goto page not found.
  if (empty($mapping_details)) {
    drupal_goto('');
  }
  if ($mapping_details) {
    $form['insightly_edit_form_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Form id'),
      '#required' => TRUE,
      '#default_value' => $mapping_details['form_id'],
    );
    $accepted_fields = variable_get('webform_insightly_accepted_fields');
    $field_list_string = '';
    foreach ($accepted_fields as $accepted_field) {
      $field_list_string .= $accepted_field . ', ';
    }
    $field_list_string = substr_replace(trim($field_list_string), "", -1);
    $form['insightly_fields_markup'] = array(
      '#markup' => t('Available fields in insightly to set target values are :field_list_string All other data field will be added as CUSTOMFIELDS (You need to add custom fields at System Settings > Custom Fields).', array(':field_list_string' => $field_list_string . '.')),
      '#prefix' => '<div class="clear">',
      '#suffix' => '</div>',
    );
    $form['insightly_mapping_edit_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Mapping'),
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="mapping-fieldset-wrapper">',
      '#suffix' => '</div>',
    );
    $count = 0;
    foreach ($mapping_details['mapping'] as $source => $target) {
      $form['insightly_mapping_edit_fieldset'][$count]['source'] = array(
        '#type' => 'textfield',
        '#title' => t('Source'),
        '#default_value' => $source,
        '#prefix' => '<div class="col-source">',
        '#suffix' => '</div>',
      );
      $form['insightly_mapping_edit_fieldset'][$count]['target'] = array(
        '#type' => 'textfield',
        '#title' => t('Target'),
        '#default_value' => $target,
        '#prefix' => '<div class="col-target">',
        '#suffix' => '</div>',
      );
      $form['insightly_mapping_edit_fieldset'][$count]['remove'] = array(
        '#title' => t('Remove'),
        '#type' => 'checkbox',
        '#default_value' => 0,
        '#prefix' => '<div class="col-remove">',
        '#suffix' => '</div>',
      );
      $count++;
    }
  }
  for ($i = $count; $i < $form_state['num_fields'] + $count; $i++) {
    $form['insightly_mapping_edit_fieldset'][$i]['source'] = array(
      '#type' => 'textfield',
      '#title' => t('Source'),
      '#prefix' => '<div class="col-source">',
      '#suffix' => '</div>',
    );
    $form['insightly_mapping_edit_fieldset'][$i]['target'] = array(
      '#type' => 'textfield',
      '#title' => t('Target'),
      '#prefix' => '<div class="col-target">',
      '#suffix' => '</div>',
    );
    $form['insightly_mapping_edit_fieldset'][$i]['remove'] = array(
      '#title' => t('Remove'),
      '#type' => 'checkbox',
      '#default_value' => 0,
      '#prefix' => '<div class="col-remove">',
      '#suffix' => '</div>',
    );
  }
  $form['insightly_add_field'] = array(
    '#type' => 'submit',
    '#value' => t('Add Field'),
    '#submit' => array('webform_insightly_add_more_fields'),
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => array(
      'callback' => 'webform_insightly_edit_add_more_callback',
      'wrapper' => 'mapping-fieldset-wrapper',
    ),
  );
  $form['insightly_edit_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('webform_insightly_add_form_validate'),
    '#submit' => array('webform_insightly_edit_form_submit'),
  );
  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the mapping in it.
 */
function webform_insightly_edit_add_more_callback($form, $form_state) {
  return $form['insightly_mapping_edit_fieldset'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function webform_insightly_add_more_fields($form, &$form_state) {
  $form_state['num_fields']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler to update the mapping configuration.
 */
function webform_insightly_edit_form_submit($form, &$form_state) {
  $id = $form_state['form_table_id'];
  $form_id = $form_state['values']['insightly_edit_form_id'];
  foreach ($form_state['values']['insightly_mapping_edit_fieldset'] as $value) {
    if ($value['remove'] == 0) {
      if (trim($value['source']) != NULL) {
        $mapping[$value['source']] = $value['target'];
      }
    }
  }
  $mapping = json_encode($mapping);
  db_query(
    "UPDATE {insightly} SET form_id = :form_id, mapping = :mapping WHERE id = :id",
    array(
      ':form_id' => $form_id,
      ':mapping' => $mapping,
      ':id' => $id,
    )
  );
  drupal_goto('admin/config/insightly-mapping/add');
}

/**
 * Callback from webform_insightly_menu().
 *
 * Confirmation form before deleting mapping.
 */
function webform_insightly_delete_form($form, &$form_state, $id = NULL) {
  $form_state['insightly_table_form_id'] = $id;
  $form['insightly_delete'] = array(
    '#type' => 'markup',
    '#markup' => '<h3>Are you sure you want to delete this mapping?</h3>',
  );
  $form['insightly_delete_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('webform_insightly_delete_form_submit'),
    '#description' => t('This action cannot be undone.'),
  );
  $form['insightly_cancel'] = array(
    '#type' => 'markup',
    '#markup' => l(t('cancel'), 'admin/config/insightly-mapping/add'),
  );
  return $form;
}

/**
 * Submit handler to delete mapped forms.
 */
function webform_insightly_delete_form_submit($form, &$form_state) {
  $id = $form_state['insightly_table_form_id'];
  db_query("DELETE FROM {insightly} WHERE id = :id", array(':id' => $id));
  drupal_goto('admin/config/insightly-mapping/add');
}

/**
 * Helper functions to get all mapping details.
 */
function webform_insightly_get_all_field_mapping() {
  $results = db_query("SELECT id, form_id, mapping FROM {insightly}");
  return $results;
}

/**
 * Callback from webform_insightly_menu().
 *
 * Form for exporting mapping.
 *
 * @param $id
 *   Unique id for mapping details entry.
 */
function webform_insightly_export_form($form, &$form_state, $id = NULL) {
  $form_state['insightly_table_form_id'] = $id;
  $mapping_details = webform_insightly_get_field_mapping($id);
  $form['insightly_export'] = array(
    '#title' => t('Export'),
    '#type' => 'textarea',
    '#default_value' => $mapping_details['form_id'] . '|' .  $mapping_details['mapping'],
  );

  return $form;
}

/**
 * Helper functions to get mapping details for corresponding id.
 *
 * @param $id
 *   Unique id for mapping details entry.
 */
function webform_insightly_get_field_mapping($id) {
  $results = db_query("SELECT id, form_id, mapping
    FROM {insightly} WHERE id = :id", array(':id' => $id));
  foreach ($results as $result) {
    $mapping_details['form_id'] = $result->form_id;
    $mapping_details['mapping'] = $result->mapping;
  }
  return $mapping_details;
}

/**
 * Callback from webform_insightly_menu().
 *
 * Form for importing mapping.
 */
function webform_insightly_import_form($form, &$form_state) {
  $form['insightly_import'] = array(
    '#title' => t('Import'),
    '#type' => 'textarea',
    '#default_value' => NULL,
  );
  $form['insightly_override'] = array(
    '#title' => t('Replace an existing field mapping, if one exists with the same Form id'),
    '#type' => 'checkbox',
    '#default_value' => FALSE,
  );
   $form['insightly_import_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('webform_insightly_import_form_submit'),
  );
  return $form;
}

/**
 * Submit handler to save the mapping configuration.
 */
function webform_insightly_import_form_submit($form, &$form_state) {
  // Separate form_id and mapping field from import data.
  $insightly_data = explode("|", $form_state['values']['insightly_import']);
  $form_id = $insightly_data[0];
  $mapping = t($insightly_data[1]);
  $override_check = $form_state['values']['insightly_override'];

  // Get id if the entered form is already mapped.
  $id = db_select('insightly', 'i')
    ->fields('i', array('id'))
    ->condition('form_id', $form_id,'=')
    ->execute()
    ->fetchAssoc();print_r($id);
  if ($id) {
    // Allow override or not for import mapping fields.
    if ($override_check == TRUE ) {
      $query= db_query("UPDATE {insightly}
        SET mapping = :mapping WHERE form_id = :form_id",
        array(
          ':form_id' => $form_id,
          ':mapping' => $mapping,
        )
      );
      drupal_goto('admin/config/insightly-mapping/add');
    }
    else {
      drupal_set_message(t('The entered form is already mapped! Please click on checkbox to override the existing mapping.'), 'status');
    }
  }
  else {
    db_query("INSERT INTO {insightly} (form_id, mapping)
      VALUES (:form_id, :mapping)",
      array(
        ':form_id' => $form_id,
        ':mapping' => $mapping
      )
    );
    drupal_goto('admin/config/insightly-mapping/add');
  }
}
