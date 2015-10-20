<?php

/**
 * @file
 * Implement an razuna field, based on the file module's file field.
 */

/**
 * Implements hook_field_info().
 */
function razuna_field_info() {
  return array(
    'razuna' => array(
      'label' => t('Razuna File Field'),
      'description' => t('These files will directly be uploaded on Razuna Cloud.'),
      'default_widget' => 'razuna_widget',
      'default_formatter' => 'razuna_formatter',
      'settings' => array(
        'uri_scheme' => variable_get('file_default_scheme', 'public'),
      ),
      'instance_settings' => array(
        'asset_id_field' => 0,
        'folder_id_field' => 0,
        'folder_name_field' => 0,
        'file_extensions' => 'png gif jpg jpeg',
        'file_directory' => '',
        'max_filesize' => '',
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function razuna_field_widget_info() {
  return array(
    'razuna' => array(
      'label' => t('File'),
      'field types' => array('razuna'),
      'settings' => array(
        'progress_indicator' => 'throbber',
        'preview_image_style' => 'thumbnail',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function razuna_field_formatter_info() {
  return array(
    'razuna_formatter' => array(
      'label' => t('Simple Razuna Field formatter'),
      'field types' => array('razuna'),
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function razuna_field_instance_settings_form($field, $instance) {
  // Use the file field instance settings form as a basis.
  $form = file_field_instance_settings_form($field, $instance);
  // Remove the description option.
  unset($form['description_field']);
  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function razuna_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // Add display_field setting to field because file_field_widget_form()
  // assumes it is set.
  $field['settings']['display_field'] = 0;

  $elements = file_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  foreach (element_children($elements) as $delta) {
    // Add all extra functionality provided by the image widget.
    $elements[$delta]['#process'][] = 'razuna_field_widget_process';
  }
  $form['#submit'][] = 'razuna_field_submit';
  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array('description' => field_filter_xss($instance['description']), 'upload_validators' => $elements[0]['#upload_validators']));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));
  }
  $razuna_fields = array();

  foreach ($form_state['field'] as $field_name => $field_object) {
    if ($field_object[$langcode]['field']['type'] == 'razuna') {
      $razuna_fields[$field_name] = $field_object;
    }
  }
  $form_state['razuna'] = $razuna_fields;
  return $elements;
}


/**
 * An element #process callback for the Razuna field type.
 */
function razuna_field_widget_process($element, &$form_state, $form) {
  $item = $element['#value'];
  $item['fid'] = $element['fid']['#value'];

  $instance = field_widget_instance($element, $form_state);

  $settings = $instance['settings'];
  $widget_settings = $instance['widget']['settings'];

  $element['#theme'] = 'image_widget';
  $element['#attached']['css'][] = drupal_get_path('module', 'image') . '/image.css';

  // Add the image preview.
  if ($element['#file'] && $widget_settings['preview_image_style']) {
    $element['preview'] = array(
      '#theme' => 'image_style',
      '#path' => $element['#file']->uri,
      '#style_name' => $widget_settings['preview_image_style'],
    );
  }
  $element['asset_id'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($item['asset_id']) ? $item['asset_id'] : '',
    '#maxlength' => 1024,
    '#value' => '',
    '#access' => (bool) $item['fid'] && $settings['asset_id_field'],
  );

  $element['folder_id'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($item['folder_id']) ? $item['folder_id'] : '',
    '#maxlength' => 1024,
    '#value' => variable_get('razuna_folderid', ''),
    '#access' => (bool) $item['fid'] && $settings['folder_id_field'],
  );
  $element['folder_name'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($item['folder_name']) ? $item['folder_name'] : '',
    '#maxlength' => 1024,
    '#value' => 'My Folder',
    '#access' => (bool) $item['fid'] && $settings['folder_name_field'],
  );
  return $element;
}

/**
 * Custom Submit Handler.
 */
function razuna_field_submit($form, &$form_state) {
  $api_key = variable_get('razuna_api_key', '');
  if (!empty($api_key)) {
    $langcode = $form['language']['#value'];
    foreach ($form_state['razuna'] as $field_name => $field_object) {
      foreach ($form_state['values'][$field_name][$langcode] as $id => $item_object) {
        $file = file_load($form_state['values'][$field_name][$langcode][$id]['fid']);
        if (isset($file->uri)) {
          $post_data['fa'] = "c.apiupload";
          $post_data['api_key'] = variable_get('razuna_api_key', '');
          $post_data['destfolderid'] = variable_get('razuna_folderid', '');
          $post_data['debug'] = "1";
          $post_data['emailto'] = variable_get('razuna_emailid', '');
          // File you want to upload/post.
          $post_data['filedata'] = "@" . drupal_realpath($file->uri);
          // Initialize cURL.
          $url = variable_get('razuna_hostname', '');
          $url = str_replace('/index.cfm', "", $url);
          $url = $url . '/index.cfm';
          $ch = curl_init();
          curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:'));
          // Set URL on which you want to post the Form and/or data.
          curl_setopt($ch, CURLOPT_URL, $url);
          // Data+Files to be posted.
          if (function_exists('curl_file_create')) {
            unset($post_data['filedata']);
            $post_data['filedata'] = new CurlFile(drupal_realpath($file->uri), 'file/exgpd', $file->filename);
          }
          curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
          // Pass TRUE or 1 if you want to wait for the response.
          curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
          // Debug mode: shows up any error encountered during the operation.
          curl_setopt($ch, CURLOPT_VERBOSE, 1);
          // Execute the request.
          $response = curl_exec($ch);
          $array = explode("\n", $response);
          if (isset($array[4])) {
            $assetid = str_replace("&gt; ", "", $array[4]);
            $array_replace = array('assetid', '<', '>', '/');
            $assetid = str_replace($array_replace, "", $assetid);
            $parents = array($field_name, $langcode, $id, 'asset_id');
            // Update form_state values.
            drupal_array_set_nested_value($form_state['values'], $parents, $assetid);
          }
        }
      }
    }
  }
  else {
    drupal_set_message(t('Razuna is not configured properly! Please <a href="@curl_url">Configure Razuna</a>', array('@curl_url' => url('admin/config/media/razuna-settings'))), 'error', FALSE);
  }
}

/**
 * Implements hook_field_load().
 */
function razuna_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  file_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
}

/**
 * Implements hook_field_prepare_view().
 */
function razuna_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {

  foreach ($entities as $entity_id => $entity) {
    foreach ($items[$entity_id] as $id => $item) {
      if (!empty($items[$entity_id][$id]['asset_id'])) {
        $uri = razuna_get_assets($items[$entity_id][$id]['asset_id'], 'thumbnail', 'img');
        $items[$entity_id][$id]['uri'] = $uri;
      }
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function razuna_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    case 'razuna_formatter':
      foreach ($items as $delta => $item) {
        if (isset($item['asset_id'])) {
          $uri = razuna_get_assets($item['asset_id'], '', 'img');
        }
        // @TODO: Add #image_style so that image size can be controlled.
        $element[$delta] = array(
          '#theme' => 'image_formatter',
          '#item' => $item,
          '#path' => isset($uri) ? $uri : '',
        );
      }
      break;
  }
  return $element;
}

/**
 * Implements hook_field_presave().
 */
function razuna_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_presave($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_is_empty().
 */
function razuna_field_is_empty($item, $field) {
  return file_field_is_empty($item, $field);
}
