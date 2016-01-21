<?php

/**
 * @file
 * Provides features for webform multiple file upload component.
 */

/**
 * Describes multiple_file component.
 *
 * @see webform_component_invoke()
 */
function _webform_defaults_multiple_file() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'filtering' => array(
        'types' => array('gif', 'jpg', 'png'),
        'addextensions' => '',
        'size' => '2 MB',
      ),
      'scheme' => 'public',
      'directory' => '',
      'progress_indicator' => 'throbber',
      'title_display' => 0,
      'description' => '',
      'attributes' => array(),
      'private' => FALSE,
    ),
  );
}

/**
 * Sets theme function for multiple_file component.
 *
 * @see webform_component_invoke()
 */
function _webform_theme_multiple_file() {
  return array(
    'webform_display_multiple_files_set' => array(
      'render element' => 'element',
    ),
  );
}


/**
 * Settings form for multiple_file component.
 *
 * @see webform_component_invoke()
 */
function _webform_edit_multiple_file($component) {
  $form = array();
  $form['#element_validate'] = array('_webform_edit_file_check_directory');
  $form['#after_build'] = array('_webform_edit_file_check_directory');

  $form['validation']['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Max upload size'),
    '#default_value' => $component['extra']['filtering']['size'],
    '#description' => t('Enter the max file size a user may upload such as 2 MB or 800 KB. Your server has a max upload size of @size.', array('@size' => format_size(file_upload_max_size()))),
    '#size' => 10,
    '#parents' => array('extra', 'filtering', 'size'),
    '#element_validate' => array('_webform_edit_file_size_validate'),
    '#weight' => 1,
  );

  $form['validation']['extensions'] = array(
    '#element_validate' => array('_webform_edit_file_extensions_validate'),
    '#parents' => array('extra', 'filtering'),
    '#theme' => 'webform_edit_file_extensions',
    '#theme_wrappers' => array('form_element'),
    '#title' => t('Allowed file extensions'),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'webform') . '/js/webform-admin.js'),
      'css' => array(drupal_get_path('module', 'webform') . '/css/webform-admin.css'),
    ),
    '#weight' => 2,
  );

  // List of all currently valid extensions.
  $current_types = isset($component['extra']['filtering']['types']) ? $component['extra']['filtering']['types'] : array();

  $types = array('gif', 'jpg', 'png');
  $form['validation']['extensions']['types']['webimages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Web images'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('bmp', 'eps', 'tif', 'pict', 'psd');
  $form['validation']['extensions']['types']['desktopimages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Desktop images'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('txt', 'rtf', 'html', 'odf', 'pdf',
    'doc', 'docx', 'ppt', 'pptx', 'xls', 'xlsx', 'xml');
  $form['validation']['extensions']['types']['documents'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Documents'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('avi', 'mov', 'mp3', 'ogg', 'wav');
  $form['validation']['extensions']['types']['media'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Media'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('bz2', 'dmg', 'gz', 'jar', 'rar', 'sit', 'tar', 'zip');
  $form['validation']['extensions']['types']['archives'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Archives'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $form['validation']['extensions']['addextensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional extensions'),
    '#default_value' => $component['extra']['filtering']['addextensions'],
    '#description' => t('Enter a list of additional file extensions for this upload field, separated by commas.<br /> Entered extensions will be appended to checked items above.'),
    '#size' => 20,
    '#weight' => 3,
  );

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['extra']['scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $component['extra']['scheme'],
    '#description' => t('Private file storage has significantly more overhead than public files, but restricts file access to users who can view submissions.'),
    '#weight' => 4,
    '#access' => count($scheme_options) > 1,
  );
  $form['extra']['directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Upload directory'),
    '#default_value' => $component['extra']['directory'],
    '#description' => t('You may optionally specify a sub-directory to store your files.'),
    '#weight' => 5,
    '#field_prefix' => 'webform/',
  );
  $options = array(WEBFORM_MULTIPLE_FILE_CARDINALITY_UNLIMITED => t('Unlimited')) +
    drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  $form['extra']['cardinality'] = array(
    '#type' => 'select',
    '#title' => t('Number of values'),
    '#options' => $options,
    '#default_value' => isset($component['extra']['cardinality']) ? $component['extra']['cardinality'] : WEBFORM_MULTIPLE_FILE_CARDINALITY_UNLIMITED,
    '#weight' => 5,
  );

  $form['display']['progress_indicator'] = array(
    '#type' => 'radios',
    '#title' => t('Progress indicator'),
    '#options' => array(
      'throbber' => t('Throbber'),
      'bar' => t('Bar with progress meter'),
    ),
    '#default_value' => $component['extra']['progress_indicator'],
    '#description' => t('The throbber display does not show the status of uploads but takes up less space. The progress bar is helpful for monitoring progress on large uploads.'),
    '#weight' => 16,
    '#access' => file_progress_implementation(),
    '#parents' => array('extra', 'progress_indicator'),
  );

  return $form;
}

/**
 * Render the multiple_file component.
 *
 * Works similar to file field widget.
 *
 * @see webform_component_invoke()
 */
function _webform_render_multiple_file($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  // Cap the upload size according to the PHP limit.
  $max_filesize = parse_size(file_upload_max_size());
  $set_filesize = $component['extra']['filtering']['size'];
  if (!empty($set_filesize) && parse_size($set_filesize) < $max_filesize) {
    $max_filesize = parse_size($set_filesize);
  }

  $element_info = element_info('managed_file');
  $element = array(
    '#type' => 'managed_file',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => empty($value) ? $component['required'] : FALSE,
    '#default_value' => isset($value[0]) ? $value[0] : NULL,
    '#attributes' => $component['extra']['attributes'],
    '#upload_validators' => array(
      'file_validate_size' => array($max_filesize),
      'file_validate_extensions' => array(implode(' ', $component['extra']['filtering']['types'])),
    ),
    '#pre_render' => array_merge(element_info_property('managed_file', '#pre_render'), array('webform_file_allow_access')),
    '#upload_location' => $component['extra']['scheme'] . '://webform/' . $component['extra']['directory'],
    '#progress_indicator' => $component['extra']['progress_indicator'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#weight' => $component['weight'],
    '#process' => array_merge($element_info['#process'], array('webform_multiple_file_process_element')),
    '#theme_wrappers' => array('webform_element'),
    '#webform_component' => $component,
    '#webform_multiple_file_form_key' => array(
      '#type' => 'value',
      '#value' => $component['form_key'],
    ),
  );

  if ($component['extra']['cardinality'] == 1) {
    // Set the default value.
    if (!empty($value)) {
      if (is_array($value[0])) {
        $fid = $value[0]['fid'];
      }
      else {
        $fid = $value[0];
      }
    }
    $element['#default_value'] = !empty($fid) ? $fid : NULL;
    // If there's only one field, return it as delta 0.
    if (empty($element['#default_value'])) {
      $element['#description'] = theme('file_upload_help', array('description' => $element['#description'], 'upload_validators' => $element['#upload_validators']));
    }
    $elements = array($element);
  }
  else {
    $delta = 0;
    if (!empty($value)) {
      foreach ($value as $item) {
        if (is_array($item)) {
          $fid = $item['fid'];
        }
        else {
          $fid = $item;
        }

        $elements[$delta] = $element;
        $elements[$delta]['#default_value'] = !empty($fid) ? $fid : NULL;;
        $elements[$delta]['#weight'] = $delta;
        $delta++;
      }
    }

    // Adds one more empty row for new file uploads.
    if (($component['extra']['cardinality'] == FIELD_CARDINALITY_UNLIMITED || $delta < $component['extra']['cardinality'])) {
      $elements[$delta] = $element;
      $elements[$delta]['#default_value'] = array();
      $elements[$delta]['#weight'] = $delta;
    }

    // Extra functionality for the group of elements.
    $elements['#file_upload_delta'] = $delta;
    $elements['#theme'] = 'file_widget_multiple';
    $elements['#theme_wrappers'] = array('fieldset');
    $elements['#process'] = array('file_field_widget_process_multiple');
    $elements['#title'] = $element['#title'];
    $elements['#title_display'] = 'invisible';
    $elements['#display_field'] = 0;
    $elements['#description'] = $element['#description'];
    $elements['#weight'] = $component['weight'];
    $elements['#attributes']['class'][] = 'webform-component--' . $component['form_key'];

    // Add some properties that will eventually be added to the
    // file upload field.
    $elements['#file_upload_title'] = t('Add a new file');
    $elements['#file_upload_description'] = theme('file_upload_help', array('description' => '', 'upload_validators' => $elements[0]['#upload_validators']));
  }

  return $elements;
}

/**
 * Submit value of multiple_file component.
 *
 * @see webform_component_invoke()
 */
function _webform_submit_multiple_file($component, $value) {
  return isset($value) ? array_filter($value) : NULL;
}

/**
 * Displays uploaded files through multiple_file component.
 *
 * @see webform_component_invoke()
 */
function _webform_display_multiple_file($component, $value, $format = 'html') {
  if (!empty($value)) {
    $files = array();
    foreach ($value as $key => $fid) {
      $files[] = array(
        '#value' => $fid ? webform_get_file($fid) : NULL,
        '#theme' => 'webform_display_file',
        '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_display_multiple_files_set'),
        '#format' => $format,
        '#parents' => array($key),
        '#webform_component' => $component,
      );
    }
    $files['#translatable'] = array('title');
    $files['#weight'] = $component['weight'];
    $files['#title'] = $component['name'];
    $files['#theme_wrappers'] = $format == 'text' ? array('webform_element_text') : array('webform_element');
  }
  else{
    $files = array();
    $files[] = array(
      '#value' => NULL,
      '#theme' => 'webform_display_file',
      '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_display_multiple_files_set'),
      '#format' => $format,
      '#parents' => NULL,
      '#webform_component' => $component,
    );
    $files['#translatable'] = array('title');
    $files['#weight'] = $component['weight'];
    $files['#title'] = $component['name'];
    $files['#theme_wrappers'] = $format == 'text' ? array('webform_element_text') : array('webform_element');
  }

  return !empty($files) ? $files : NULL;
}

/**
 * Describes delete process of multiple_file component.
 *
 * @see webform_component_invoke()
 */
function _webform_delete_multiple_file($component, $value) {
  if (!empty($value)) {
    foreach ($value as $fid) {
      if (!empty($fid) && ($file = webform_get_file($fid))) {
        file_usage_delete($file, 'webform');
        file_delete($file);
      }
    }
  }
}

/**
 * Attaches multiple_file component data to mail.
 *
 * @see webform_component_invoke()
 */
function _webform_attachments_multiple_file($component, $value) {
  foreach ($value as $fid) {
    $file = (array) webform_get_file($fid);
    if (!empty($file)) {
      // This is necessary until the next release of mimemail is out,
      // see [#1388786].
      $file['filepath'] = $file['uri'];
      $files[] = $file;
    }
  }

  return !empty($files) ? $files : NULL;
}

/**
 * Processing multiple_file data for analysis page.
 *
 * @see webform_component_invoke()
 */
function _webform_analysis_multiple_file($component, $sids = array()) {
  // Get submissions with uploaded multifiles.
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('no', 'data', 'sid'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $files_count = 0;
  $sizetotal = 0;
  $submissions_with_files = array();

  $result = $query->execute();
  foreach ($result as $data) {
    $file = webform_get_file($data['data']);
    if (isset($file->filesize)) {
      $files_count++;
      $sizetotal += $file->filesize;
    }

    if (!in_array($data['sid'], $submissions_with_files)) {
      $submissions_with_files[] = $data['sid'];
    }
  }

  // Get count of submissions of current webform.
  $total = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('sid'))
    ->condition('nid', $component['nid'])
    ->groupBy('sid')
    ->countQuery()
    ->execute()
    ->fetchField();

  $rows[0] = array(t('Left Blank'), ($total - count($submissions_with_files)));
  $rows[1] = array(t('User uploaded files'), $files_count);
  $other[0] = array(t('Average uploaded file size'), ($sizetotal != 0 ? (int) (($sizetotal / $files_count) / 1024) . ' KB' : '0'));
  return array(
    'table_rows' => $rows,
    'other_data' => $other,
  );
}

/**
 * Provides multiple_file data for results webform table.
 *
 * @see webform_component_invoke()
 */
function _webform_table_multiple_file($component, $value) {
  $output = '';
  if (!empty($value)) {
    foreach ($value as $fid) {
      $file = webform_get_file($fid);
      if (!empty($file->fid)) {
        $output .= l(webform_file_name($file->uri), webform_file_url($file->uri));
        $output .= ' (' . (int) ($file->filesize / 1024) . ' KB)';
        $output .= "<br />\n";
      }
    }
  }

  return $output;
}

/**
 * Configuration of CSV headers for multiple_file data export.
 *
 * @see webform_component_invoke()
 */
function _webform_csv_headers_multiple_file($component, $export_options) {
  $header = array();
  // Two columns in header.
  $header[0] = array('', '');
  $header[1] = array($component['name'], '');
  $header[2] = array(t('Name'), t('Filesize (KB)'));
  return $header;
}

/**
 * Processing multiple_file data for CSV export.
 *
 * @see webform_component_invoke()
 */
function _webform_csv_data_multiple_file($component, $export_options, $value) {
  $files_size = 0;
  $file_urls = array();

  if (!empty($value)) {
    foreach ($value as $fid) {
      $file = webform_get_file($fid);
      if (!empty($file->filename)) {
        $file_urls[] = webform_file_url($file->uri);
        $files_size += $file->filesize / 1024;
      }
    }
  }

  return empty($files_size) ? array('', '') :
    array(implode("|", $file_urls), (int) $files_size);
}

/**
 * Process form element callback.
 *
 * Adds changes to managed_file element structure.
 *
 * @see file_field_widget_process()
 */
function webform_multiple_file_process_element($element, &$form_state, $form) {

  $element['#theme'] = 'file_widget';

  // Ajax settings to upload and remove of any individual file
  // and update group of already uploaded files.
  $parents = array_slice($element['#array_parents'], 0, -1);
  $cardinality = drupal_array_get_nested_value($form,
    array_merge($parents, array('#webform_component', 'extra', 'cardinality')));
  if ($cardinality != 1) {
    $new_path = 'file/ajax/' . implode('/', $parents) . '/' . $form['form_build_id']['#value'];
    $component_element = drupal_array_get_nested_value($form, $parents);
    $new_wrapper = $component_element['#id'] . '-ajax-wrapper';
    foreach (element_children($element) as $key) {
      if (isset($element[$key]['#ajax'])) {
        $element[$key]['#ajax']['path'] = $new_path;
        $element[$key]['#ajax']['wrapper'] = $new_wrapper;
      }
    }
    unset($element['#prefix'], $element['#suffix']);
  }

  // Add another submit handler to the upload and remove buttons,
  // to implement functionality needed by the webform component.
  foreach (array('upload_button', 'remove_button') as $key) {
    $element[$key]['#submit'][] = 'webform_multiple_file_managed_file_submit';
    $limit_validation_errors = array(array_slice($element['#parents'], 0, -1));
    $element[$key]['#limit_validation_errors'] = $limit_validation_errors;
  }

  return $element;
}

/**
 * Form submission handler.
 *
 * Form submission handler for upload/remove button of managed_file
 * in multiple_file webform component.
 * This runs in addition to and after file_managed_file_submit().
 * 
 * @see file_managed_file_submit()
 * @see webform_multiple_file_process_element()
 * @see file_field_widget_submit()
 */
function webform_multiple_file_managed_file_submit($form, &$form_state) {
  // During the form rebuild, _webform_render_multiple_file() will create
  // file elements using re-indexed deltas, so clear out
  // $form_state['input'] to avoid a mismatch between old and new deltas.
  // The rebuilt elements will have #default_value set appropriately
  // for the current state of the field, so nothing is lost in doing this.
  $button = $form_state['triggering_element'];
  $parents = array_slice($button['#parents'], 0, -2);

  // Reset input values.
  drupal_array_set_nested_value($form_state['input'], $parents, NULL);

  // Webform component info.
  $component = drupal_array_get_nested_value($form, array_merge($parents, array('#webform_component')));
  $form_key = $component['form_key'];

  // All submitted values.
  $submitted_values = drupal_array_get_nested_value($form_state['values'], array_slice($button['#array_parents'], 0, -3));

  // Processes values only for multiple files component.
  $component_values = isset($submitted_values[$form_key]) ? $submitted_values[$form_key] : array();
  foreach ($component_values as $delta => $submitted_value) {
    if (empty($submitted_value) || (is_array($submitted_value) &&  empty($submitted_value['fid']))) {
      unset($component_values[$delta]);
    }
  }

  // Re-index deltas after removing empty items.
  $submitted_values[$form_key] = array_values($component_values);
  // Retrieve parent ID for Webform component.
  $pid = $component['pid'];
  // Changes key from 'form_key' to 'cid' of webform component.
  $flatten_values = _webform_client_form_submit_flatten($form['#node'], $submitted_values, $pid);

  // Key 'submitted' is used in Webform module as constant all over the code.
  // Seems that is correct to use this key.
  $parents = array('submitted');
  // Update form_state values.
  drupal_array_set_nested_value($form_state['values'], $parents, $flatten_values);

  // Adds values to $form_state['storage'] to prevent missing values during another element AJAX-submit.
  $form_state['storage'] = isset($form_state['storage']) ? $form_state['storage'] : array();
  $storage_values = drupal_array_get_nested_value($form_state['storage'], $parents);
  $storage_values = isset($storage_values) ? $storage_values : array();
  foreach ($flatten_values as $cid => $value) {
    $storage_values[$cid] = $value;
  }
  drupal_array_set_nested_value($form_state['storage'], $parents, $storage_values);
}

/**
 * Theme function for multiple file wrapping.
 */
function theme_webform_display_multiple_files_set($variables) {
  $output = '<div class="webform-multiple-file">' . $variables['element']['#children'] . '</div>';
  return $output;
}
