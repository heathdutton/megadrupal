<?php

/**
 * @file
 * Field module functionality for the Embridge Field module.
 */

/**
 * Implements hook_field_info().
 */
function embridge_field_field_info() {
  return array(
    'entermediaasset' => array(
      'label' => t('EnterMedia Asset'),
      'description' => t('This field stores the ID of a file as an integer value.'),
      'settings' => array(
        'uri_scheme' => variable_get('file_default_scheme', 'public'),
      ),
      'instance_settings' => array(
        'file_extensions' => '',
        'file_directory' => '',
        'min_filesize' => '',
        'max_filesize' => '',
        'description_field' => 0,
        'asset_catalog' => EMBRIDGE_ALLOWED_ASSET_CATALOG_DEFAULT,
        'asset_path' => '',
        'asset_type' => EMBRIDGE_ALLOWED_ASSET_TYPE_DEFAULT,
        'widget_type' => 'both',
      ),
      'default_widget' => 'embridge_field_generic',
      'default_formatter' => 'embridge_field_default',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function embridge_field_field_settings_form($field, $instance, $has_data) {
  $defaults = field_info_field_settings($field['type']);
  $settings = array_merge($defaults, $field['settings']);

  $form['#attached']['js'][] = drupal_get_path('module', 'embridge_field') . '/embridge_field.js';

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $settings['uri_scheme'],
    '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
    '#disabled' => $has_data,
  );

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function embridge_field_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  // Make the extension list a little more human-friendly by comma-separation.
  $extensions = str_replace(' ', ', ', $settings['file_extensions']);
  $form['file_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed file extensions'),
    '#default_value' => $extensions,
    '#description' => t('Separate extensions with a space or comma and do not include the leading dot.'),
    '#element_validate' => array('embridge_field_generic_settings_extensions'),
    '#weight' => 1,
    // By making this field required, we prevent a potential security issue
    // that would allow files of any type to be uploaded.
    '#required' => TRUE,
  );
  $form['min_filesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum file size'),
    '#default_value' => $settings['min_filesize'],
    '#description' => t('Enter a value like "512" (bytes), "80 KB" (kilobytes) or "50 MB" (megabytes) in order to restrict the allowed file size. If left empty there is no minimum limit for file sizes.'),
    '#size' => 10,
    '#weight' => 2,
  );

  $form['max_filesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum upload size'),
    '#default_value' => $settings['max_filesize'],
    '#description' => t('Enter a value like "512" (bytes), "80 KB" (kilobytes) or "50 MB" (megabytes) in order to restrict the allowed file size. If left empty the file sizes will be limited only by PHP\'s maximum post and file upload sizes (current limit <strong>%limit</strong>).', array('%limit' => format_size(file_upload_max_size()))),
    '#size' => 10,
    '#element_validate' => array('embridge_field_generic_settings_max_filesize'),
    '#weight' => 5,
  );

  $catalogs = variable_get('embridge_catalogs', array());
  $form['asset_catalog'] = array(
    '#type' => 'select',
    '#title' => t('EnterMedia Catalog'),
    '#default_value' => !empty($settings['asset_catalog']) ? $settings['asset_catalog'] : EMBRIDGE_ALLOWED_ASSET_CATALOG_DEFAULT,
    '#options' => $catalogs,
    '#required' => TRUE,
    '#description' => t('Select EnterMedia catalog for asset.'),
  );

  $form['asset_type'] = array(
    '#type' => 'select',
    '#title' => t('Asset Type'),
    '#default_value' => !empty($settings['asset_type']) ? $settings['asset_type'] : EMBRIDGE_ALLOWED_ASSET_TYPE_DEFAULT,
    '#options' => array(
      'photo' => 'Photo',
      'audio' => 'Audio',
      'video' => 'Video',
      'interactive' => 'Interactive',
    ),
    '#required' => TRUE,
    '#description' => t('Select asset type for field.'),
  );

  $form['asset_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Asset Path'),
    '#default_value' => !empty($settings['asset_path']) ? $settings['asset_path'] : '[dam:yyyy]/[dam:mm]',
    '#required' => FALSE,
    '#description' => t('Used to create subfolders during asset upload. Use token [dam:yyyy] for current year and [dam:mm] for current month.'),
  );

  $form['widget_type'] = array(
    '#type' => 'radios',
    '#title' => t('Widget Type'),
    '#default_value' => !empty($settings['widget_type']) ? $settings['widget_type'] : 'both',
    '#options' => array('search' => t('Asset Search'), 'upload' => t('Asset Upload'), 'both' => t('Both')),
    '#required' => FALSE,
    '#description' => t('Select how the assets will be added to the field.'),
  );
  return $form;
}

/**
 * Element validate callback for the maximum upload size field.
 *
 * Ensure a size that can be parsed by parse_size() has been entered.
 */
function embridge_field_generic_settings_max_filesize($element, &$form_state) {
  if (!empty($element['#value']) && !is_numeric(parse_size($element['#value']))) {
    form_error($element, t('The "@name" option must contain a valid value. You may either leave the text field empty or enter a string like "512" (bytes), "80 KB" (kilobytes) or "50 MB" (megabytes).', array('@name' => t($element['title']))));
  }
}

/**
 * Element validate callback for the allowed file extensions field.
 *
 * This doubles as a convenience clean-up function and a validation routine.
 * Commas are allowed by the end-user, but ultimately the value will be stored
 * as a space-separated list for compatibility with file_validate_extensions().
 */
function embridge_field_generic_settings_extensions($element, &$form_state) {
  if (!empty($element['#value'])) {
    $extensions = preg_replace('/([, ]+\.?)/', ' ', trim(drupal_strtolower($element['#value'])));
    $extensions = array_filter(explode(' ', $extensions));
    $extensions = implode(' ', array_unique($extensions));
    if (!preg_match('/^([a-z0-9]+([.][a-z0-9])* ?)+$/', $extensions)) {
      form_error($element, t('The list of allowed extensions is not valid, be sure to exclude leading dots and to separate extensions with a comma or space.'));
    }
    else {
      form_set_value($element, $extensions, $form_state);
    }
  }
}

/**
 * Element validate callback for the file destination field.
 *
 * Remove slashes from the beginning and end of the destination value and ensure
 * that the file directory path is not included at the beginning of the value.
 */
function embridge_field_generic_settings_file_directory_validate($element, &$form_state) {
  // Strip slashes from the beginning and end of $widget['file_directory'].
  $value = trim($element['#value'], '\\/');
  form_set_value($element, $value, $form_state);
}

/**
 * Implements hook_field_load().
 */
function embridge_field_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {

  $aids = array();
  foreach ($entities as $id => $entity) {
    // Load the files from the files table.
    foreach ($items[$id] as $delta => $item) {
      if (!empty($item['aid'])) {
        $aids[] = $item['aid'];
      }
    }
  }

  $files = embridge_field_asset_load_multiple($aids);

  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      // If the file does not exist, mark the entire item as empty.
      if (empty($item['aid']) || !isset($files[$item['aid']])) {
        $items[$id][$delta] = NULL;
      }
      else {
        $items[$id][$delta] = array_merge($item, (array) $files[$item['aid']]);
      }
    }
  }
}

/**
 * Implements hook_field_prepare_view().
 */
function embridge_field_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // Remove files specified to not be displayed.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      // Ensure consecutive deltas.
      $items[$id] = array_values($items[$id]);
    }
  }
}

/**
 * Implements hook_field_presave().
 */
function embridge_field_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
}

/**
 * Implements hook_field_insert().
 */
function embridge_field_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
}

/**
 * Implements hook_field_update().
 *
 * Checks for files that have been removed from the object.
 */
function embridge_field_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  if (!empty($entity->revision)) {
    return;
  }

  // Build a display of the current FIDs.
  $current_aids = array();
  foreach ($items as $item) {
    $current_aids[] = $item['aid'];
  }

  // Create a bare-bones entity so that we can load its previous values.
  $original = entity_create_stub_entity($entity_type, array($id, $vid, $bundle));
  field_attach_load($entity_type, array($id => $original), FIELD_LOAD_CURRENT, array('field_id' => $field['id']));

  // Compare the original field values with the ones that are being saved.
  $original_aids = array();
  if (!empty($original->{$field['field_name']}[$langcode])) {
    foreach ($original->{$field['field_name']}[$langcode] as $original_item) {
      $original_aids[] = $original_item['aid'];
      if (isset($original_item['aid']) && !in_array($original_item['aid'], $current_aids)) {
        // Decrement the file usage count by 1 and delete the file if possible.
        embridge_field_delete_file($original_item, $field, $entity_type, $id);
      }
    }
  }
}

/**
 * Implements hook_field_delete().
 */
function embridge_field_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Delete all file usages within this entity.
  foreach ($items as $delta => $item) {
    embridge_field_delete_file($item, $field, $entity_type, $id, 0);
  }
}

/**
 * Implements hook_field_delete_revision().
 */
function embridge_field_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  foreach ($items as $delta => $item) {
    // Decrement the file usage count by 1 and delete the file if possible.
    if (embridge_field_delete_file($item, $field, $entity_type, $id)) {
      $items[$delta] = NULL;
    }
  }
}

/**
 * Decrements a file usage count and attempts to delete it.
 *
 * This function only has an effect if the file being deleted is used only by
 * File module.
 *
 * @param array $item
 *   The field item that contains a file array.
 * @param array $field
 *   The field structure for the operation.
 * @param string $entity_type
 *   The type of $entity.
 * @param Int $id
 *   The entity ID which contains the file being deleted.
 * @param Int $count
 *   (optional) The number of references to decrement from the object
 *   containing the file. Defaults to 1.
 *
 * @return Bool
 *   Boolean TRUE if the file was deleted, or an array of remaining references
 *   if the file is still in use by other modules. Boolean FALSE if an error
 *   was encountered.
 */
function embridge_field_field_delete_file($item, $field, $entity_type, $id, $count = 1) {
  // To prevent the file field from deleting files it doesn't know about, check
  // the file reference count. Temporary files can be deleted because they
  // are not yet associated with any content at all.
  $file = (object) $item;
  $file_usage = embridge_field_usage_list($file);
  if ($file->status == 0 || !empty($file_usage['file'])) {
    entermediafield_usage_delete($file, 'file', $entity_type, $id, $count);
    return embridge_field_delete($file);
  }

  // Even if the file is not deleted, return TRUE to indicate the file field
  // record can be removed from the field database tables.
  return TRUE;
}

/**
 * Implements hook_field_is_empty().
 */
function embridge_field_field_is_empty($item, $field) {
  return empty($item['aid']);
}

/**
 * TODO: Need to revisit.
 *
 * Implements hook_field_formatter_info().
 */
function embridge_field_field_formatter_info() {
  return array(
    'embridge_field_default' => array(
      'label' => t('Default'),
      'field types' => array('entermediaasset'),
      'settings' => array('rendition' => 'thumbnail', 'link_to_content' => FALSE),
    ),
    'embridge_field_embed' => array(
      'label' => t('Embed View'),
      'field types' => array('entermediaasset'),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function embridge_field_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  
  $catalog_id = $instance['settings']['asset_catalog'];
  $catalog_tokens = explode('/', $catalog_id);
  $catalog_name = array_pop($catalog_tokens);
  $selected_renditions = _embridge_get_selected_rendition_types($catalog_name);

  $element = array();  
  if ($display['type'] == 'embridge_field_default') {
    $element['rendition'] = array(
      '#title' => t('Conversion'),
      '#type' => 'select',
      '#options' => $selected_renditions,
      '#default_value' => $settings['rendition'],
    );
    $element['link_to_content'] = array(
      '#title' => t('Link to Content'),
      '#type' => 'checkbox',
      '#default_value' => $settings['link_to_content'],
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function embridge_field_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';
  if ($display['type'] == 'embridge_field_default') {
    $summary .= '<br/>' . t('Conversion') . ': ' . $settings['rendition'];
    $summary .= '<br/>' . t('Link to Content') . ': ' . $settings['link_to_content'];
  }
  return $summary;
}

/**
 * Implements hook_field_widget_info().
 */
function embridge_field_field_widget_info() {
  return array(
    'embridge_field_generic' => array(
      'label' => t('Asset Upload'),
      'field types' => array('entermediaasset'),
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function embridge_field_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form['progress_indicator'] = array(
    '#type' => 'radios',
    '#title' => t('Progress indicator'),
    '#options' => array(
      'throbber' => t('Throbber'),
      'bar' => t('Bar with progress meter'),
    ),
    '#default_value' => $settings['progress_indicator'],
    '#description' => t('The throbber display does not show the status of uploads but takes up space. The progress bar is helpful for monitoring progress on large uploads.'),
    '#weight' => 16,
    '#access' => file_progress_implementation(),
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function embridge_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $defaults = array(
    'aid' => 0,
    'description' => '',
  );

  // Retrieve any values set in $form_state, as will be the case during AJAX
  // rebuilds of this form.
  if (isset($form_state['values'])) {
    $path = array_merge($element['#field_parents'], array($field['field_name'], $langcode));
    $path_exists = FALSE;
    $values = drupal_array_get_nested_value($form_state['values'], $path, $path_exists);
    if ($path_exists) {
      $items = $values;
      drupal_array_set_nested_value($form_state['values'], $path, NULL);
    }
  }

  foreach ($items as $delta => $item) {
    $items[$delta] = is_array($items[$delta]) ? array_merge($defaults, $items[$delta]) : $defaults;
    // Remove any items from being displayed that are not needed.
    if ($items[$delta]['aid'] == 0) {
      unset($items[$delta]);
    }
  }

  // Re-index deltas after removing empty items.
  $items = array_values($items);

  // Update order according to weight.
  $items = _field_sort_items($field, $items);

  // Essentially we use the managed_file type, extended with some enhancements.
  $element_info = element_info('managed_entermediaasset');
  $element += array(
    '#type' => 'managed_entermediaasset',
    '#default_value' => isset($items[$delta]) ? $items[$delta] : $defaults,
    '#upload_location' => embridge_field_widget_uri($field, $instance),
    '#upload_validators' => embridge_field_widget_upload_validators($field, $instance),
    '#value_callback' => 'embridge_field_field_widget_value',
    '#process' => array_merge($element_info['#process'], array('embridge_field_field_widget_process')),
    // Allows this field to return an array instead of a single value.
    '#extended' => TRUE,
  );

  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($element['#default_value']['aid'])) {
      $element['#description'] = theme('embridge_field_upload_help', array('description' => $element['#description'], 'upload_validators' => $element['#upload_validators']));
    }
    $elements = array($element);
  }
  else {
    // If there are multiple values, add an element for each existing one.
    $delta = -1;
    foreach ($items as $delta => $item) {
      $elements[$delta] = $element;
      $elements[$delta]['#default_value'] = $item;
      $elements[$delta]['#weight'] = $delta;
    }
    // And then add one more empty row for new uploads.
    $delta++;
    if ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED || $delta < $field['cardinality']) {
      $elements[$delta] = $element;
      $elements[$delta]['#default_value'] = $defaults;
      $elements[$delta]['#weight'] = $delta;
      $elements[$delta]['#required'] = ($element['#required'] && $delta == 0);
    }
    // The group of elements all-together need some extra functionality
    // after building up the full list (like draggable table rows).
    $elements['#file_upload_delta'] = $delta;
    $elements['#theme'] = 'embridge_field_widget_multiple';
    $elements['#theme_wrappers'] = array('fieldset');
    $elements['#process'] = array('embridge_field_field_widget_process_multiple');
    $elements['#title'] = $element['#title'];
    $elements['#description'] = $element['#description'];
    $elements['#field_name'] = $element['#field_name'];
    $elements['#language'] = $element['#language'];
    $elements['#display_field'] = (!empty($field['settings']['display_field']) ? $field['settings']['display_field'] : 1);

    // Add some properties that will eventually be added to the file upload
    // field. These are added here so that they may be referenced easily through
    // a hook_form_alter().
    $elements['#file_upload_title'] = t('Add a new file');
    $elements['#file_upload_description'] = theme('embridge_field_upload_help', array('description' => '', 'upload_validators' => $elements[0]['#upload_validators']));
  }

  return $elements;
}

/**
 * Get the upload validators for a file field.
 *
 * @param array $field
 *   A field array.
 *
 * @return array
 *   An array suitable for passing to file_save_upload() or the file field
 *   element's '#upload_validators' property.
 */
function embridge_field_field_widget_upload_validators($field, $instance) {
  // Cap the upload size according to the PHP limit.
  $max_filesize = parse_size(file_upload_max_size());
  if (!empty($instance['settings']['max_filesize']) && parse_size($instance['settings']['max_filesize']) < $max_filesize) {
    $max_filesize = parse_size($instance['settings']['max_filesize']);
  }

  $validators = array();

  // There is always a file size limit due to the PHP server limit.
  $validators['file_validate_size'] = array($max_filesize);

  // Add the extension check if necessary.
  if (!empty($instance['settings']['file_extensions'])) {
    $validators['file_validate_extensions'] = array($instance['settings']['file_extensions']);
  }
  // Add min filesize check if necessary.
  if (!empty($instance['settings']['min_filesize'])) {
    $validators['embridge_field_file_validate_min_filesize'] = array(parse_size($instance['settings']['min_filesize']));
  }

  return $validators;
}

/**
 * Determine the URI for a file field instance.
 *
 * @param array $field
 *   A field array.
 * @param array $instance
 *   A field instance array.
 *
 * @return string
 *   A file directory URI with tokens replaced.
 */
function embridge_field_field_widget_uri($field, $instance, $account = NULL) {
  $destination = trim($instance['settings']['file_directory'], '/');

  // Replace tokens.
  $data = array('user' => isset($account) ? $account : $GLOBALS['user']);
  $destination = token_replace($destination, $data);

  return $field['settings']['uri_scheme'] . '://' . $destination;
}

/**
 * The #value_callback for the file_generic field element.
 */
function embridge_field_field_widget_value($element, $input = FALSE, $form_state = array()) {
  $instance = field_widget_instance($element, $form_state);
  if ($input) {
    // Checkboxes lose their value when empty.
    // If the display field is present make sure its unchecked value is saved.
    $field = field_widget_field($element, $form_state);
  }

  // We depend on the managed file element to handle uploads.
  $return = embridge_field_managed_file_value($element, $input, $form_state, $instance);

  // Check if this is a search result.
  if (empty($return['aid']) && !empty($input['sourcepath'])) {

    // Prepare file array.
    $file = array();
    $file['asset_id'] = $input['asset_id'];
    if (!empty($input['title'])) {
      $file['title'] = $input['title'];
    }
    $file['filesize'] = isset($input['filesize']) ? $input['filesize'] : 0;
    $file['filemime'] = isset($input['fileformat']) ? $input['fileformat'] : '';
    $file['thumbnail'] = $input['thumbnail'];
    $file['width'] = isset($input['width']) ? $input['width'] : 0;
    $file['height'] = isset($input['height']) ? $input['width'] : 0;
    $file['sourcepath'] = $input['sourcepath'];
    $file['thumbnail'] = $input['thumbnail'];
    $file['preview'] = $input['preview'];
    $catalogid = _embridge_get_catalog_settings_by_name($input['catalog']);
    // Adding entermedia asset data to entermedia table.
    $file['aid'] = _embridge_asset_insert($file, $catalogid['id']);
    // We depend on the managed file element to handle uploads.
    $return = $file;
  }

  // Set embedcode if exists.
  if (!empty($input['embedcode']) && !empty($return['aid'])) {
    $catalog_id = $instance['settings']['asset_catalog'];
    embridge_field_managed_file_update_embed_code($return['aid'], $catalog_id, $input['embedcode']);
  }
  // Validate asset added from embridge search.
  if (!empty($input['from_search'])) {
    $errors = array();
    $source = implode('_', $element['#parents']);
    $validators = $element['#upload_validators'];
    $file = new stdClass();
    $file->filename = $return['filename'];
    $file->fileformat = empty($return['fileformat']) ? '' : $return['fileformat'];
    $file->filesize = $return['filesize'];
    
    // Validate fileformat.
    if (!empty($validators['file_validate_extensions'][0])) {
      $errors = file_validate_extensions($file, $validators['file_validate_extensions'][0]);
    }
    
    // Validate min filesize.
    if (!empty($validators['embridge_field_file_validate_min_filesize'][0])) {
      $errors = array_merge($errors, _embridge_field_file_validate_min_filesize($file, $validators['embridge_field_file_validate_min_filesize'][0]));
    }
    
    if (!empty($errors)) {
      $message = t('The specified asset %name could not be added from search.', array('%name' => $file->filename));
      if (count($errors) > 1) {
        $message .= theme('item_list', array('items' => $errors));
      }
      else {
        $message .= ' ' . array_pop($errors);
      }
      form_set_error($source, $message);
      return array(
        'aid' => 0,
        'description' => '',
      );
    } 
  }

  // Ensure that all the required properties are returned even if empty.
  $return += array(
    'aid' => 0,
    'description' => '',
  );

  return $return;
}
function _embridge_field_file_validate_min_filesize(stdClass $file, $file_limit = 0) {
  $errors = array();
  if ($file_limit && $file->filesize < $file_limit) {
      $errors[] = t('The file is %filesize less than the minimum file size of %minsize.', array('%filesize' => format_size($file->filesize), '%minsize' => format_size($file_limit)));
  }
    
  return $errors;
}

/**
 * An element #process callback for the file_generic field type.
 *
 * Expands the file_generic type to include the description and display fields.
 */
function embridge_field_field_widget_process($element, &$form_state, $form) {
  $item = $element['#value'];
  $item['aid'] = $element['aid']['#value'];
  // $item['aid'] = $element['#value']['aid'];
  $field = field_widget_field($element, $form_state);
  $instance = field_widget_instance($element, $form_state);
  $settings = $instance['widget']['settings'];

  $element['#theme'] = 'embridge_field_widget';

  // Add the description field if enabled.
  if (!empty($instance['settings']['description_field']) && $item['aid']) {
    $element['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#value' => isset($item['description']) ? $item['description'] : '',
      '#type' => variable_get('file_description_type', 'textfield'),
      '#maxlength' => variable_get('file_description_length', 128),
      '#description' => t('The description may be used as the label of the link to the file.'),
    );
  }

  // Adjust the AJAX settings so that on upload and remove of any individual
  // file, the entire group of file fields is updated together.
  if ($field['cardinality'] != 1) {
    $parents = array_slice($element['#array_parents'], 0, -1);
    $new_path = 'file/ajax/' . implode('/', $parents) . '/' . $form['form_build_id']['#value'];
    $field_element = drupal_array_get_nested_value($form, $parents);
    $new_wrapper = $field_element['#id'] . '-ajax-wrapper';
    foreach (element_children($element) as $key) {
      if (isset($element[$key]['#ajax'])) {
        $element[$key]['#ajax']['path'] = $new_path;
        $element[$key]['#ajax']['wrapper'] = $new_wrapper;
      }
    }
    unset($element['#prefix'], $element['#suffix']);
  }

  // Add another submit handler to the upload and remove buttons, to implement
  // functionality needed by the field widget. This submit handler, along with
  // the rebuild logic in file_field_widget_form() requires the entire field,
  // not just the individual item, to be valid.
  foreach (array('upload_button', 'remove_button') as $key) {
    $element[$key]['#submit'][] = 'embridge_field_field_widget_submit';
    $element[$key]['#limit_validation_errors'] = array(
      array_slice(
        $element['#parents'],
        0,
        -1
      ));
  }

  return $element;
}

/**
 * An element #process callback for a group of file_generic fields.
 *
 * Adds the weight field to each row so it can be ordered and adds a new AJAX
 * wrapper around the entire group so it can be replaced all at once.
 */
function embridge_field_field_widget_process_multiple($element, &$form_state, $form) {
  $element_children = element_children($element, TRUE);
  $count = count($element_children);
  foreach ($element_children as $delta => $key) {
    if ($key != $element['#file_upload_delta']) {
      $description = _file_field_get_description_from_element($element[$key]);
      $element[$key]['_weight'] = array(
        '#type' => 'weight',
        '#title' => $description ? t('Weight for @title', array('@title' => $description)) : t('Weight for new file'),
        '#title_display' => 'invisible',
        '#delta' => $count,
        '#default_value' => $delta,
      );
    }
    else {
      // The title needs to be assigned to the upload field so that validation
      // errors include the correct widget label.
      $element[$key]['#title'] = $element['#title'];
      $element[$key]['_weight'] = array(
        '#type' => 'hidden',
        '#default_value' => $delta,
      );
    }
  }

  // Add a new wrapper around all the elements for AJAX replacement.
  $element['#prefix'] = '<div id="' . $element['#id'] . '-ajax-wrapper">';
  $element['#suffix'] = '</div>';

  return $element;
}

/**
 * Helper function for file_field_widget_process_multiple().
 *
 * @param array $element
 *   The element being processed.
 *
 * @return string
 *   A description of the file suitable for use in the administrative interface.
 */
function embridge_field_field_get_description_from_element($element) {
  // Use the actual file description, if it's available.
  if (!empty($element['#default_value']['description'])) {
    return $element['#default_value']['description'];
  }
  // Otherwise, fall back to the filename.
  if (!empty($element['#default_value']['filename'])) {
    return $element['#default_value']['filename'];
  }
  // This is probably a newly uploaded file; no description is available.
  return FALSE;
}

/**
 * Submit handler for upload and remove buttons of file_generic fields.
 *
 * This runs in addition to and after file_managed_file_submit().
 *
 * @see file_managed_file_submit()
 * @see file_field_widget_form()
 * @see file_field_widget_process()
 */
function embridge_field_field_widget_submit($form, &$form_state) {
  // During the form rebuild, file_field_widget_form() will create field item
  // widget elements using re-indexed deltas, so clear out $form_state['input']
  // to avoid a mismatch between old and new deltas. The rebuilt elements will
  // have #default_value set appropriately for the current state of the field,
  // so nothing is lost in doing this.
  $parents = array_slice($form_state['triggering_element']['#parents'], 0, -2);
  drupal_array_set_nested_value($form_state['input'], $parents, NULL);
}

/**
 * Returns HTML for an individual file upload widget.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the widget.
 *
 * @ingroup themeable
 */
function theme_embridge_field_widget($variables) {
  $element = $variables['element'];
  $output = '';

  // The "form-managed-file" class is required for proper AJAX functionality.
  $output .= '<div class="file-widget form-managed-file clearfix" fid="'. implode('_', $element['#parents']) .'">';
  if ($element['aid']['#value'] != 0) {
    $asset = $element['#entermediaasset'];
    $renditions = _embridge_get_rendition_list($asset->asset_id, $asset->catalogid, $asset->sourcepath, $asset->filemime, TRUE);
    $preview = $asset->preview;
    // Invoke hooks to alter renditions.
    if (module_implements('embridge_renditions_alter')) {
    $renditions = module_invoke_all('embridge_renditions_alter', $renditions);
    }
    
    if (!empty($renditions['large'])) {
      $preview = $renditions['large'];
    }
    // Add the file size after the file name.
    $element['entermediaassetname']['#markup'] .= ' <div id="' . implode('_', $element['#parents'])  . '_preview_dialog" class="preview-dialog" style="display:none;" img_src="' . $preview . '"></div>';
    $element['entermediaassetname']['#markup'] .= ' <div class="file-info"> <span class="file-name">' . basename($asset->sourcepath) . '</span> ';
    $element['entermediaassetname']['#markup'] .= ' <span class="file-size">(' . $asset->filesize . ' bytes)</span> </div> ';
  }
  $output .= drupal_render_children($element);
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for a group of file upload widgets.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the widgets.
 *
 * @ingroup themeable
 */
function theme_embridge_field_widget_multiple($variables) {
  $element = $variables['element'];

  // Special ID and classes for draggable tables.
  $weight_class = $element['#id'] . '-weight';
  $table_id = $element['#id'] . '-table';

  // Build up a table of applicable fields.
  $headers = array();
  $headers[] = t('File information');
  $headers[] = t('Weight');
  $headers[] = t('Operations');

  // Get our list of widgets in order (needed when the form comes back after
  // preview or failed validation).
  $widgets = array();
  foreach (element_children($element) as $key) {
    $widgets[] = &$element[$key];
  }
  usort($widgets, '_field_sort_items_value_helper');

  $rows = array();
  foreach ($widgets as $key => &$widget) {
    // Save the uploading row for last.
    if ($widget['#entermediaasset'] == FALSE) {
      $widget['#title'] = $element['#file_upload_title'];
      $widget['#description'] = $element['#file_upload_description'];
      continue;
    }

    // Delay rendering of the buttons, so that they can be rendered later in the
    // "operations" column.
    $operations_elements = array();
    foreach (element_children($widget) as $sub_key) {
      if (isset($widget[$sub_key]['#type']) && $widget[$sub_key]['#type'] == 'submit') {
        hide($widget[$sub_key]);
        $operations_elements[] = &$widget[$sub_key];
      }
    }

    hide($widget['_weight']);

    // Render everything else together in a column, without the normal wrappers.
    $widget['#theme_wrappers'] = array();
    $information = drupal_render($widget);

    // Render the previously hidden elements, using render() instead of
    // drupal_render(), to undo the earlier hide().
    $operations = '';
    foreach ($operations_elements as $operation_element) {
      $operations .= render($operation_element);
    }
    
    $widget['_weight']['#attributes']['class'] = array($weight_class);
    $weight = render($widget['_weight']);

    // Arrange the row with all of the rendered columns.
    $row = array();
    $row[] = $information;
    $row[] = $weight;
    $row[] = $operations;
    $rows[] = array(
      'data' => $row,
      'class' => isset($widget['#attributes']['class']) ? array_merge($widget['#attributes']['class'], array('draggable')) : array('draggable'),
    );
  }

  drupal_add_tabledrag($table_id, 'order', 'sibling', $weight_class);

  $output = '';
  $output = empty($rows) ? '' : theme('table', array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array(
      'id' => $table_id,
    ))
  );
  $output .= drupal_render_children($element);
  return $output;
}


/**
 * Returns HTML for help text based on file upload validators.
 *
 * @param array $variables
 *   An associative array containing:
 *   - description: The normal description for this field, specified by the
 *     user.
 *   - upload_validators: An array of upload validators as used in
 *     $element['#upload_validators'].
 *
 * @ingroup themeable
 */
function theme_embridge_field_upload_help($variables) {
  $description = $variables['description'];
  $upload_validators = $variables['upload_validators'];

  $descriptions = array();

  if (drupal_strlen($description)) {
    $descriptions[] = $description;
  }
  if (isset($upload_validators['file_validate_size'])) {
    $descriptions[] = t('Files must be less than !size.', array('!size' => '<strong>' . format_size($upload_validators['file_validate_size'][0]) . '</strong>'));
  }
  if (isset($upload_validators['file_validate_extensions'])) {
    $descriptions[] = t('Allowed file types: !extensions.', array('!extensions' => '<strong>' . check_plain($upload_validators['file_validate_extensions'][0]) . '</strong>'));
  }
  if (isset($upload_validators['file_validate_image_resolution'])) {
    $max = $upload_validators['file_validate_image_resolution'][0];
    $min = $upload_validators['file_validate_image_resolution'][1];
    if ($min && $max && $min == $max) {
      $descriptions[] = t('Images must be exactly !size pixels.', array('!size' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min && $max) {
      $descriptions[] = t('Images must be between !min and !max pixels.', array('!min' => '<strong>' . $min . '</strong>', '!max' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min) {
      $descriptions[] = t('Images must be larger than !min pixels.', array('!min' => '<strong>' . $min . '</strong>'));
    }
    elseif ($max) {
      $descriptions[] = t('Images must be smaller than !max pixels.', array('!max' => '<strong>' . $max . '</strong>'));
    }
  }

  return implode('<br />', $descriptions);
}

/**
 * Implements hook_field_formatter_view().
 */
function embridge_field_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'embridge_field_default':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'embridge_field_link',
          '#entermediaasset' => (object) $item,
          '#rendition' => $display['settings']['rendition'],
          '#link_to_content' => $display['settings']['link_to_content'],
          '#entity' => $entity,
        );
      }
      break;

    case 'embridge_field_embed':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'embridge_field_embed',
          '#entermediaasset' => (object) $item,
        );
      }
      break;
  }

  return $element;
}

/**
 * Returns HTML for a file attachments table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - items: An array of file attachments.
 *
 * @ingroup themeable
 */
function theme_embridge_field_formatter_table($variables) {
  $header = array(t('Attachment'), t('Size'));
  $rows = array();
  foreach ($variables['items'] as $delta => $item) {
    $rows[] = array(
      theme('file_link', array('file' => (object) $item)),
      format_size($item['filesize']),
    );
  }

  return empty($rows) ? '' : theme('table', array('header' => $header, 'rows' => $rows));
}
