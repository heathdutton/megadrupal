<?php

/**
 * @file
 * Webform select image module select image component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_select_image() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'images' => '',
      'options' => '',
      'num_images' => 0,
      'multiple' => NULL,
      'optrand' => 0,
      'title_display' => 0,
      'description' => '',
      'custom_keys' => FALSE,
      'private' => FALSE,
      'image_style' => 'thumbnail',
      'layout' => 'vertically',
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_select_image() {
  return array(
    'webform_display_select_image' => array(
      'render element' => 'element',
      'path' => drupal_get_path('module', 'webform_select_image'),
      'file' => 'components/select_image.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_select_image($component) {

  // Get default images
  $images = $component['extra']['images'] ? explode('||', $component['extra']['images']) : array();
  $legends = array();
  if(!empty($component['extra']['legends'])) {
    $legends = explode('||', $component['extra']['legends']);
  }
  
  // Create form
  $form['#tree'] = TRUE;
  $form['#validate'] = array('_webform_select_image_edit_validate');
  $form['items'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images options'),
    '#prefix' => '<div id="select-images-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  
  if(empty($component['extra']['num_images'])) {
    $component['extra']['num_images'] = 1;
  }
  
  for ($i = 0; $i < $component['extra']['num_images']; $i++) {
    $form['items']['images'][$i] = array(
      '#type' => 'container',
    );
    $form['items']['images'][$i]['src'] = array(
      '#type' => 'managed_file',
      '#title' => t('Picture'),
      '#description' => t('Allowed extensions: gif png jpg jpeg'),
      '#default_value' => (isset($images[$i]) ? $images[$i] : ''),
      '#upload_location' => file_build_uri('webform/' . $component['nid'] . '/select_images'),
      '#upload_validators' => array(
        'file_validate_extensions' => array('gif png jpg jpeg'),
        'file_validate_size' => array(file_upload_max_size()),
      ),
      '#element_validate' => array('_webform_select_managed_file_validate'), 
    );

    $form['items']['images'][$i]['legend'] = array(
      '#type' => 'textfield',
      '#title' => t('Legend'),
      '#default_value' => (isset($legends[$i]) ? $legends[$i] : ''),
    );
  }

  $form['items']['add_image_option'] = array(
    '#type' => 'submit',
    '#value' => t('Add image option'),
    '#name' => 'add_image',
    '#submit' => array('_webform_add_select_image'),
    '#ajax' => array(
      'callback' => '_webform_add_select_image_callback',
      'wrapper' => 'select-images-fieldset-wrapper',
    ),
  );

  if ($component['extra']['num_images'] > 1) {
    $form['items']['remove_add_image_option'] = array(
      '#type' => 'submit',
      '#name' => 'remove_image',
      '#value' => t('Remove image option'),
      '#submit' => array('_webform_remove_select_image'),
      '#ajax' => array(
        'callback' => '_webform_add_select_image_callback',
        'wrapper' => 'select-images-fieldset-wrapper',
      ),
    );
  }

  $form['extra']['image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image style'),
    '#default_value' => $component['extra']['image_style'],
    '#description' => t('The image style used to display images.'),
    '#options' => image_style_options(),
    '#weight' => 0,
  );

  $form['value'] = array(
    '#type' => 'select',
    '#title' => t('Default value'),
    '#default_value' => !empty($component['value']) ? unserialize($component['value']) : '',
    '#description' => t('The default value of the field.'),
    '#options' => _webform_select_image_get_options_list($images),
    '#multiple' => $component['extra']['multiple'],
    '#weight' => 0,
  );

  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#default_value' => $component['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
    '#weight' => 0,
  );

  $form['display']['optrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize options'),
    '#default_value' => $component['extra']['optrand'],
    '#description' => t('Randomizes the order of the options when they are displayed in the form.'),
    '#parents' => array('extra', 'optrand'),
  );

  $form['display']['layout'] = array(
    '#type' => 'select',
    '#title' => t('Display options'),
    '#default_value' => $component['extra']['layout'],
    '#options' => array('vertically' => t('Vertically'), 'horizontally' => t('Horizontally')),
  );

  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the images in it.
 */
function _webform_add_select_image_callback($form, $form_state) {
  
  // Image preview @TODO => not here
  /*
  if(!empty($form['items']['images'][0]['#file']->uri)) {
    $form['items']['images'][0]['filename']['#markup'] = theme(
      'image_style',
      array(
        'style_name' => 'thumbnail',
        'path' => $form['items']['images'][0]['#file']->uri,
      )
    );
  }
  */
  
  return $form['items'];
}

/**
 * Submit handler for the "add-image-option" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function _webform_add_select_image($form, &$form_state) {
  if(empty($form_state['build_info']['args'][1]['extra']['num_images'])) {
    $form_state['build_info']['args'][1]['extra']['num_images'] = 1;
  }
  $form_state['build_info']['args'][1]['extra']['num_images']++;
  $form_state['rebuild'] = TRUE;  
}

/**
 * Submit handler for the "remove image option" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function _webform_remove_select_image($form, &$form_state) {
  if ($form_state['build_info']['args'][1]['extra']['num_images'] > 1) {
    $form_state['build_info']['args'][1]['extra']['num_images']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_select_image($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#theme_wrappers' => array('webform_element_select_image'),
    '#pre_render' => array(), // Needed to disable double-wrapping of radios and checkboxes.
    '#translatable' => array('title', 'description', 'options'),
  );

  // Convert the user-entered options list into an array.
  $default_value = unserialize($component['value']);
  $options = _webform_select_image_options($component);
  if ($component['extra']['optrand']) {
    module_load_include('inc', 'webform', 'components/select');
    _webform_shuffle_options($options);
  }

  // Set the component options.
  $element['#options'] = $options;

  // Set the default value.
  if (isset($value)) {
    if ($component['extra']['multiple']) {
      // Set the value as an array.
      $element['#default_value'] = array();
      foreach ((array) $value as $key => $option_value) {
        $element['#default_value'][] = $option_value;
      }
    }
    else {
      // Set the value as a single string.
      $element['#default_value'] = '';
      foreach ((array) $value as $option_value) {
        $element['#default_value'] = $option_value;
      }
    }
  }
  elseif ($default_value !== '') {
    // Convert default value to a list if necessary.
    if ($component['extra']['multiple']) {
      foreach ($default_value as $key => $v) {
        $v = trim($v);
        if ($v !== '') {
          $element['#default_value'][] = $v;
        }
      }
    }
    else {
      $element['#default_value'] = !empty($default_value) ? current($default_value) : '';
    }
  }
  elseif ($component['extra']['multiple']) {
    $element['#default_value'] = array();
  }

  if ($component['extra']['multiple']) {
    // Set display as a checkbox set.
    $element['#type'] = 'checkboxes';
    $element['#theme_wrappers'] = array_merge(array('checkboxes'), $element['#theme_wrappers']);
    $element['#process'] = array_merge(element_info_property('checkboxes', '#process'), array('webform_expand_select_image_ids'));

    // Entirely replace the normal expand checkboxes with our custom version.
    // This helps render checkboxes in multipage forms.
    $process_key = array_search('form_process_checkboxes', $element['#process']);
    $element['#process'][$process_key] = 'webform_expand_checkboxes_image';
  }
  else {
    // Set display as a radio set.
    $element['#type'] = 'radios';
    $element['#theme_wrappers'] = array_merge(array('radios'), $element['#theme_wrappers']);
    $element['#process'] = array_merge(element_info_property('radios', '#process'), array('webform_expand_select_image_ids'));
  }

  return $element;
}

/**
 * Drupal 6 hack that properly *renders* checkboxes in multistep forms. This is
 * different than the value hack needed in Drupal 5, which is no longer needed.
 */
function webform_expand_checkboxes_image($element) {
  // Elements that have a value set are already in the form structure cause
  // them not to be written when the expand_checkboxes function is called.
  $default_value = array();
  foreach (element_children($element) as $key) {
    if (isset($element[$key]['#default_value'])) {
      $default_value[$key] = $element[$key]['#default_value'];
      unset($element[$key]);
    }
  }

  $element = form_process_checkboxes($element);

  // Escape the values of checkboxes.
  foreach (element_children($element) as $key) {
    $element[$key]['#return_value'] = check_plain($element[$key]['#return_value']);
    $element[$key]['#name'] = $element['#name'] . '[' . $element[$key]['#return_value'] . ']';
  }

  foreach ($default_value as $key => $val) {
    $element[$key]['#default_value'] = $val;
  }
  return $element;
}

/**
 * FAPI process function to rename IDs attached to checkboxes and radios.
 */
function webform_expand_select_image_ids($element) {
  $id = $element['#id'] = str_replace('_', '-', _webform_safe_name(strip_tags($element['#id'])));
  $delta = 0;
  foreach (element_children($element) as $key) {
    $delta++;
    // Convert the #id for each child to a safe name, regardless of key.
    $element[$key]['#id'] = $id . '-' . $delta;

    // Prevent scripts or CSS in the labels for each checkbox or radio.
    // TODO: allow <p> tags. Not a good idea to add p into allowed tags from _webform_filter_xss
    // $element[$key]['#title'] = _webform_filter_xss($element[$key]['#title']);
  }
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_select_image($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_select_image',
    '#theme_wrappers' => $format == 'html' ? array('webform_element_select_image') : array('webform_element_text'),
    '#format' => $format,
    '#options' => _webform_select_image_options($component),
    '#value' => (array) $value,
    '#translatable' => array('title', 'options'),
  );
}


/**
 * Format the text output for this component.
 */
function theme_webform_display_select_image($variables) {
  $element = $variables['element'];
  $component = $element['#webform_component'];

  // Flatten the list of options so we can get values easily. These options
  // may be translated by hook_webform_display_component_alter().
  $options = array();
  foreach ($element['#options'] as $key => $value) {
    if (is_array($value)) {
      foreach ($value as $subkey => $subvalue) {
        $options[$subkey] = $subvalue;
      }
    }
    else {
      $options[$key] = $value;
    }
  }

  $items = array();
  if ($component['extra']['multiple']) {
    foreach ((array) $element['#value'] as $option_value) {
      if ($option_value !== '') {
        // Administer provided values.
        if (isset($options[$option_value])) {
          $items[] = $element['#format'] == 'html' ? _webform_filter_xss($options[$option_value]) : $options[$option_value];
        }
      }
    }
  }
  else {
    if (isset($element['#value'][0]) && $element['#value'][0] !== '') {
      // Administer provided values.
      if (isset($options[$element['#value'][0]])) {
        $items[] = $element['#format'] == 'html' ? _webform_filter_xss($options[$element['#value'][0]]) : $options[$element['#value'][0]];
      }
    }
  }

  if ($element['#format'] == 'html') {
    $output = count($items) > 1 ? theme('item_list', array('items' => $items)) : (isset($items[0]) ? $items[0] : ' ');
  }
  else {
    if (count($items) > 1) {
      foreach ($items as $key => $item) {
        $items[$key] = ' - ' . $item;
      }
      $output = implode("\n", $items);
    }
    else {
      $output = isset($items[0]) ? $items[0] : ' ';
    }
  }

  return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_select_image($component, $sids = array(), $single = FALSE) {
  $options = _webform_select_image_options($component, FALSE);
  $show_other_results = $single;

  // Gather the normal results first (not "other" options).
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>')
    ->groupBy('data');
  $query->addExpression('COUNT(data)', 'datacount');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  // Duplicate the query for other options.
  if ($show_other_results) {
    $other_query = clone($query);
    $other_query->condition('data', array_keys($options), 'NOT IN');
    $other_query->orderBy('datacount', 'DESC');
  }

  $query->condition('data', array_keys($options), 'IN');

  $result = $query->execute();
  $rows = array();
  $other = array();
  $normal_count = 0;
  foreach ($result as $data) {
    $display_option = isset($options[$data['data']]) ? $options[$data['data']] : $data['data'];
    $rows[$data['data']] = array(webform_filter_xss($display_option), $data['datacount']);
    $normal_count += $data['datacount'];
  }

  // Order the results according to the normal options array.
  $ordered_rows = array();
  foreach (array_intersect_key($options, $rows) as $key => $label) {
    $ordered_rows[] = $rows[$key];
  }
  $rows = $ordered_rows;

  return array('table_rows' => $rows);
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_select_image($component, $value) {
  // Convert submitted 'safe' values to un-edited, original form.
  $options = _webform_select_image_options($component, TRUE);

  $value = (array) $value;
  $items = array();
  // Set the value as a single string.
  foreach ($value as $option_value) {
    if ($option_value !== '') {
      if (isset($options[$option_value])) {
        $items[] = _webform_filter_xss($options[$option_value]);
      }
    }
  }

  return implode('<br />', $items);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_select_image($component, $export_options) {
  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  if ($component['extra']['multiple'] && $export_options['select_format'] == 'separate') {
    $headers[0][] = '';
    $headers[1][] = $component['name'];
    $options = _webform_select_image_options($component, FALSE);
    $count = 0;
    foreach ($options as $key => $item) {
      // Empty column per sub-field in main header.
      if ($count != 0) {
        $headers[0][] = '';
        $headers[1][] = '';
      }
      if ($export_options['select_keys']) {
        $headers[2][] = $key;
      }
      else {
        $headers[2][] = $item;
      }
      $count++;
    }
  }
  else {
    $headers[0][] = '';
    $headers[1][] = '';
    $headers[2][] = $component['name'];
  }
  return $headers;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_select_image($component, $export_options, $value) {
  $options = _webform_select_image_options($component, FALSE);
  $return = array();

  if ($component['extra']['multiple']) {
    foreach ($options as $key => $item) {
      $index = array_search($key, (array) $value);
      if ($index !== FALSE) {
        if ($export_options['select_format'] == 'separate') {
          $return[] = 'X';
        }
        else {
          $return[] = $export_options['select_keys'] ? $key : $item;
        }
        unset($value[$index]);
      }
      elseif ($export_options['select_format'] == 'separate') {
        $return[] = '';
      }
    }
  }
  else {
    $key = !empty($value) ? current($value) : '';
    if ($export_options['select_keys']) {
      $return = $key;
    }
    else {
      $return = isset($options[$key]) ? $options[$key] : $key;
    }
  }

  if ($component['extra']['multiple'] && $export_options['select_format'] == 'compact') {
    $return = implode(',', (array) $return);
  }

  return $return;
}

/**
 * Generate a list of options for a select image list.
 */
function _webform_select_image_options($component, $html = TRUE) {
  static $options_cache = array();

  // Keep each processed option block in an array indexed by the MD5 hash of
  // the option text and the value of the $flat variable.
  $md5 = md5($component['extra']['options'].$html);

  // Check if this option block has been previously processed.
  if (!isset($option_cache[$md5])) {
    $options = array();
    $rows = array_filter(explode("\n", trim($component['extra']['options'])));
    foreach ($rows as $row) {
      $values = preg_split("/[\|,]+/", $row);
      $fid = !empty($values[0]) ? $values[0] : NULL;
      $filename = !empty($values[1]) ? $values[1] : NULL;
      $legend = !empty($values[2]) ? $values[2] : NULL;
      if($file = file_load($fid)) {
        if($html) {
          if(!empty($component['extra']['image_style'])) {
            $options[$fid] = theme(
              'image_style',
              array(
                'style_name' => $component['extra']['image_style'],
                'path' => $file->uri,
              )
            );
          }
          else {
            $options[$fid] = theme('image',array('path' => $file->uri));
          }
          $options[$fid] .= !empty($legend) ? '<p class="webform-select-image-legend webform-select-image-legend-' . $fid . '">' . $legend . '</p>' : '';
        }
        else {
          $options[$fid] = $file->filename;
        }
      }
    }
    $options_cache[$md5] = $options;
  }

  // Return our options from the option_cache array.
  return isset($options_cache[$md5]) ? $options_cache[$md5] : array();
}

/**
 * Convert an array of images into text.
 */
function _webform_select_images_to_text($items) {
  $options = array();
  foreach ($items as $key => $value) {
    if(!empty($value['src']['fid'])) {
      $options[] = $value['src']['fid'];
    }
  }
  return  implode('||', $options);
}

/**
 * Convert an array of legends into text.
 */
function _webform_select_legends_to_text($items) {
  $legend = array();
  foreach ($items as $key => $value) {
    if(!empty($value['legend'])) {
      $legend[] = $value['legend'];
    }
  }
  return  implode('||', $legend);
}

/**
 * Convert an array of options into text.
 */
function _webform_select_image_options_to_text($items) {
  $output = '';
  foreach ($items as $key => $item) {
    if($file = file_load($item['src']['fid'])) {
      $output .= $file->fid . '||' . $file->filename . '||' . $item['legend'] . "\n";
    }
  }
  return $output;
}

/**
 * Get options list for default values.
 */
function _webform_select_image_get_options_list($items) {
  $options = array('' => t('- None -'));
  foreach($items as $fid) {
    if($file = file_load($fid)) {
      $options[$fid] = $file->filename;
    }
  }
  return $options;
}
