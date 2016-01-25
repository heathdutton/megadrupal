<?php
/**
 * @file
 * Provides the Bazaarvoice Products administrative interface.
 */

/**
 * Settings for Bazaarvoice productfeed.
 */
function bazaarvoice_productfeed_settings_form($form, &$form_state) {
  $form['feed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product Feed Settings'),
    '#description' => t('Settings generating a product feed.'),
    '#collapsible' => TRUE,
  );
  $form['feed']['bazaarvoice_feed_location'] = array(
    '#type' => 'textfield',
    '#title' => t('File path'),
    '#description' => t('Directory within files directory to save product feed files. (relative to files directory). <br /> Do not include begning or trailing slash.'),
    '#default_value' => variable_get('bazaarvoice_feed_location', ''),
  );
  $form['feed']['bazaarvoice_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Name'),
    '#description' => t('Site name to pass in the product feed.'),
    '#maxlength' => 255,
    '#default_value' => variable_get('bazaarvoice_site_name', variable_get('site_name', '')),
  );
  $form['feed']['bazaarvoice_feed_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed Name'),
    '#maxlength' => 255,
    '#description' => t('Feed name, supplied by Bazaarvoice'),
    '#default_value' => variable_get('bazaarvoice_feed_name', ''),
  );
  $form['ftp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product Feed FTP Settings'),
    '#description' => t('Settings for connecting to the Bazaarvoice FTP for uploading product data. It is recommended not configuring these values in this form, instead configure them using $conf variables in your settings.php'),
    '#collapsible' => TRUE,
  );
  $form['ftp']['bazaarvoice_ftp_mode'] = array(
    '#type' => 'select',
    '#title' => t('FTP mode'),
    '#description' => t('Method for FTPing product feed to Bazaarvoice.'),
    '#options' => array(
      'sftp' => t('SFTP'),
      'ftp' => t('FTP'),
    ),
    '#default_value' => variable_get('bazaarvoice_ftp_mode', 'ftp'),
  );
  $form['ftp']['bazaarvoice_ftp_username'] = array(
    '#type' => 'textfield',
    '#title' => t('FTP Username'),
    '#description' => t('Username to connect to the FTP server.'),
    '#default_value' => variable_get('bazaarvoice_ftp_username', ''),
  );
  $form['ftp']['bazaarvoice_ftp_password'] = array(
    '#type' => 'textfield',
    '#title' => t('FTP Password'),
    '#description' => t('Password for connecting to the FTP server.'),
    '#default_value' => variable_get('bazaarvoice_ftp_password'),
  );
  $form['ftp']['bazaarvoice_ftp_frequency'] = array(
    '#type' => 'select',
    '#title' => t('FTP Upload Frequency'),
    '#description' => t('Select frequency of FTPing Product Feed to Bazaarvoice. Timing may not be exact as it is limited to the frequency of when cron is run.'),
    '#options' => array(
      0 => t('Never'),
      3600 => t('Hourly'),
      43200 => t('Twice a Day'),
      86400 => t('Daily'),
      604800 => t('Weekly'),
    ),
    '#default_value' => variable_get('bazaarvoice_ftp_frequency', 0),
  );
  $form = system_settings_form($form);
  $form['#validate'][] = 'bazaarvoice_productfeed_admin_validate';

  return $form;
}

/**
 * Validation callback for productfeed settings form.
 */
function bazaarvoice_productfeed_admin_validate($form, &$form_state) {
  // Get the file location selected.
  if ($directory = $form_state['values']['bazaarvoice_feed_location']) {
    // Sanitize name.
    $directory_safe = check_plain(trim(str_replace(' ', '-', $directory)));
    if ($directory_safe != $directory) {
      form_set_error('bazaarvoice_feed_location', t('Invalid directory name provided.'));
    }
    else {
      $directory = file_default_scheme() . '://' . $directory;
      // Check that directory exists and is writable.
      if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        form_set_error('bazaarvoice_feed_location', t('File directory does not exist or is not writable.'));
      }
    }
  }
}

/**
 * Displays a table of custom productfeed attributes.
 */
function bazaarvoice_productfeed_admin_attributes() {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  $table = array(
    '#theme' => 'table',
    '#header' => array(
      array('data' => t('Attribute')),
      array('data' => t('Operations'), 'colspan' => 2),
    ),
    '#rows' => array(),
  );

  // Fetch attributes from code.
  if ($attributes = bazaarvoice_productfeed_get_custom_attributes()) {
    foreach ($attributes as $name => $attribute) {
      $table['#rows'][] = array(
        array('data' => $attribute['label'] . ' (' . $name . ')'),
        array('data' => l(t('Edit'), 'admin/config/services/bazaarvoice/productfeed/attributes/edit/' . $name)),
        array('data' => l(t('Delete'), 'admin/config/services/bazaarvoice/productfeed/attributes/delete/' . $name)),
      );
    }
  }
  // Add additional row to create an attribute.
  $table['#rows'][] = array(
    array(
      'data' => l(t('Add Custom Attribute'), 'admin/config/services/bazaarvoice/productfeed/attributes/add'),
      'colspan' => 3,
    ),
  );
  return render($table);
}

/**
 * Form to create a custom attribute.
 */
function bazaarvoice_productfeed_admin_attributes_add_form($form, &$form_state) {
  $form['attribute_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#description' => t('The human readable label'),
    '#default_value' => isset($form_state['values']['attribute_label']) ? $form_state['values']['attribute_label'] : '',
  );
  $form['attribute_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Attribute ID'),
    '#required' => TRUE,
    '#description' => t('The ID value to be used in the Product Feed. Once set this cannot be changed.'),
    '#default_value' => isset($form_state['values']['attribute_id']) ? $form_state['values']['attribute_id'] : '',
  );
  $field_options = array('text' => 'text');
  // Get list of all field types.
  if ($fields = field_info_field_map()) {
    foreach ($fields as $field) {
      if (!isset($field_options[$field['type']])) {
        $field_options[$field['type']] = $field['type'];
      }
    }
  }
  $form['field_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Field Types'),
    '#required' => TRUE,
    '#description' => t('What field types can be mapped to this attribute'),
    '#options' => $field_options,
  );
  // Have a value passed?
  if (isset($form_state['values']['field_types'])) {
    $form['field_types']['#default_value'] = $form_state['values']['field_types'];
  }
  // Have field options?
  if (count($field_options) == 0) {
    $form['field_types']['#type'] = 'markup';
    $form['field_types']['#value'] = t('There are no field types available.');
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Attribute'),
  );
  return $form;
}

/**
 * Validation callback for creating a custom attribute.
 */
function bazaarvoice_productfeed_admin_attributes_add_form_validate($form, &$form_state) {
  // Check that the label is a plain text label.
  $label = check_plain(trim(strip_tags($form_state['values']['attribute_label'])));
  // Have a valid label?
  if (!$label) {
    form_set_error('attribute_label', 'Invalid label provided');
  }
  // Check that the ID is not already in use.
  $attribute_id = check_plain(trim(strip_tags($form_state['values']['attribute_id'])));
  // @TODO: some string formatting for ID?
  if ($attribute_id) {
    // Check DB to see if ID already exists.
    $id_exists = db_select('bazaarvoice_productfeed_custom_attributes', 'bpfca')
      ->fields('bpfca', array('attribute'))
      ->condition('attribute', $attribute_id)
      ->range(0, 1)
      ->execute()
      ->rowCount();
    // Id already exists in the Database?
    if ($id_exists) {
      form_set_error('attribute_id', 'A custom attribute with that ID already exists');
    }
  }
  // No attribute ID or invalid attribute ID.
  else {
    form_set_error('attribute_id', 'Invalid attribute ID provided.');
  }
}

/**
 * Submit callback for creating a custom attribute.
 */
function bazaarvoice_productfeed_admin_attributes_add_form_submit($form, &$form_state) {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  $attribute_id = check_plain(trim(strip_tags($form_state['values']['attribute_id'])));
  $values = array(
    'label' => check_plain(trim(strip_tags($form_state['values']['attribute_label']))),
    'field_types' => array(),
  );
  // Loop through the submitted field types.
  foreach ($form_state['values']['field_types'] as $value => $state) {
    // Was the field type selected?
    if ($value === $state) {
      $values['field_types'][] = $value;
    }
  }
  bazaarvoice_productfeed_set_custom_attribute($attribute_id, $values);
  // Alert that attribute was created.
  drupal_set_message(t('Attribute "@attribute" created.', array('@attribute' => $values['label'])));
}

/**
 * Form for editing a custom attribute.
 */
function bazaarvoice_productfeed_admin_attributes_edit_form($form, &$form_state, $attribute) {
  // Set default values based on attribute.
  if (!isset($form_state['values']['attribute_label'])) {
    $form_state['values']['attribute_label'] = $attribute['label'];
  }
  if (!isset($form_state['values']['attribute_id'])) {
    $form_state['values']['attribute_id'] = $attribute['attribute'];
  }
  if (!isset($form_state['values']['field_types'])) {
    $form_state['values']['field_types'] = $attribute['field_types'];
  }
  $form['#productfeed_attribute'] = $attribute;
  // Get the Add form.
  $form = bazaarvoice_productfeed_admin_attributes_add_form($form, $form_state);
  // Disable the ID field.
  $form['attribute_id']['#disabled'] = TRUE;
  return $form;
}

/**
 * Validation callback for editing an attribute.
 */
function bazaarvoice_productfeed_admin_attributes_edit_form_validate($form, &$form_state) {
  // Check that the label is a plain text label.
  $label = check_plain(trim(strip_tags($form_state['values']['attribute_label'])));
  // Label not provided?
  if (!$label) {
    form_set_error('attribute_label', 'Invalid label provided');
  }
  // Check that the ID is not already in use.
  $attribute_id = check_plain(trim(strip_tags($form_state['values']['attribute_id'])));
  // No attribute id?
  if (!$attribute_id) {
    form_set_error('attribute_id', 'Invalid attribute ID provided.');
  }
  // Else, attempting to chang the ID?
  elseif ($attribute_id != $form['#productfeed_attribute']['attribute']) {
    form_set_error('attribute_id', 'Attribute ID cannot be changed.');
  }
}

/**
 * Submit callback for editing an attribute.
 */
function bazaarvoice_productfeed_admin_attributes_edit_form_submit($form, &$form_state) {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  // Get attribute id.
  $attribute_id = check_plain(trim(strip_tags($form_state['values']['attribute_id'])));
  // Prep values.
  $values = array(
    'label' => check_plain(trim(strip_tags($form_state['values']['attribute_label']))),
    'field_types' => array(),
  );
  // Get field types selected.
  foreach ($form_state['values']['field_types'] as $value => $state) {
    if ($value === $state) {
      $values['field_types'][] = $value;
    }
  }
  bazaarvoice_productfeed_set_custom_attribute($attribute_id, $values);
  // Set message about updated attribute.
  drupal_set_message(t('Attribute "@attribute" updated.', array('@attribute' => $values['label'])));
}

/**
 * Confirmation form for deleting a custom attribute.
 */
function bazaarvoice_productfeed_admin_attributes_delete_form($form, &$form_state, $attribute) {
  $form = array();
  $form['#productfeed_attribute'] = $attribute;
  $form['attribute_id'] = array(
    '#type' => 'value',
    '#value' => $attribute['attribute'],
  );
  // Display a confirmation form.
  return confirm_form(
    $form,
    t('Are you sure you want to delete the custom attribute "@attribute"?', array('@attribute' => $attribute['label'])),
    'admin/config/services/bazaarvoice/productfeed/attributes'
  );
}

/**
 * Submit callback for attribute delete confirmation.
 */
function bazaarvoice_productfeed_admin_attributes_delete_form_submit($form, &$form_state) {
  $attribute = $form_state['values']['attribute_id'];
  // Delete attribute.
  bazaarvoice_productfeed_delete_attributes(array($attribute));
  // Let user know attribute was successfully deleted.
  drupal_set_message(t('Attribute "@attribute" has been deleted.', array('@attribute' => $form['#productfeed_attribute']['label'])));
  // Redirect to the attributes admin page.
  drupal_goto('admin/config/services/bazaarvoice/productfeed/attributes');
}
