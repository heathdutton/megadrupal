<?php

/**
 * @file page callbacks for the pixgather module.
 */

/**
 * Callback for photo uploading.  Security check against secret code.
 *
 * @param string $secret_code the secret code passed to the URL.
 */
function pixgather_upload_photo() {
  // Extract the file information from the POST data.
  $file = $_POST['file'];
  $name = $_POST['name'];
  $secret_code = $_POST['secretcode'];
  if ($secret_code === variable_get('pixgather_secret_code', false)) {

    // Grab the file data from the posted array and save it as a new drupal
    // file.
    $file_data64 = $file['file'];
    $file_data = base64_decode($file_data64);
    $file = file_save_data($file_data, 'public://pixgather' . md5(uniqid(rand(), true)) . '.jpg');

    // Create a new node for the photo.
    rules_invoke_event('pixgather_photo_uploaded', $file, $name);

    // Send the new file url to the pixgather App
    echo image_style_url('pixgather_phone_image', $file->uri);
  }
  else {
    // If they got the code wrong, let them know.
    echo "WRONGCODE";
  }
  exit;
}

/**
 *  Settings form.
 */
function pixgather_upload_settings_form($form, $form_state) {
  
  $form['pixgather_secret_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Code'),
    '#description' => t('Enter a secret code that guests will be required to input into the app (spaces, slashes, etc are not allowed)'),
    '#required' => true,
    '#default_value' => variable_get('pixgather_secret_code', 'pixgathercode'),
  );

  $infos = entity_get_info();
  $options = array();
  foreach ($infos as $entity => $info) {
    $options[$entity] = $info['label'];
  }

  $form['pixgather_entity_type'] = array(
    '#title' => t('Entity Type to Which Uploaded Photos Will be Attached'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('pixgather_entity_type')
  );


  if (variable_get('pixgather_entity_type', false)) {

    $info = entity_get_info(variable_get('pixgather_entity_type', false));
    $bundles = array();

    foreach ($info['bundles'] as $machine_name => $bundle) {
      $bundles[$machine_name] = $bundle['label'];
    }


    $form['pixgather_bundle'] = array(
      '#title' => t('Bundle to Which Uploaded Photos Will be Attached'),
      '#type' => 'select',
      '#options' => $bundles,
      '#default_value' => variable_get('pixgather_bundle', false),
    );

    if (variable_get('pixgather_bundle', false)) {
      $image_field_options = array();
      $name_field_options = array();
      $fields = field_info_instances(variable_get('pixgather_entity_type', false), variable_get('pixgather_bundle', false));
      foreach ($fields as $field_name => $field) {
        $info = field_info_field($field_name);
        if ($info['type'] == 'image') {
          $image_field_options[$field_name] = $field['label'] . " (" . $field_name . ")";
        }
        else if ($info['type'] == 'text') {
          $name_field_options[$field_name] = $field['label'] . " (" . $field_name . ")";
        }
      }

      $form['pixgather_image_field'] = array(
        '#title' => t('Image Field to Which Uploaded Photos Will be Attached'),
        '#description' => t('Only image fields will be shown here'),
        '#options' => $image_field_options,
        '#type' => 'select',
        '#default_value' => variable_get('pixgather_image_field', false),
      );

      $form['pixgather_name_field'] = array(
        '#title' => t('Text Field to Which Uploaded Photos Attributions Will be Attached'),
        '#description' => t('Only text fields will be shown here'),
        '#options' => $name_field_options,
        '#type' => 'select',
        '#default_value' => variable_get('pixgather_name_field', false),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Save the secret code setting to the variable table on submit.
 */
function pixgather_upload_settings_form_submit(&$form, &$form_state) {
  $code = urlencode($form_state['values']['pixgather_secret_code']);
  variable_set('pixgather_secret_code', $code);

  if (!empty($form_state['values']['pixgather_entity_type'])) {
    variable_set('pixgather_entity_type', $form_state['values']['pixgather_entity_type']);
  }

  if (!empty($form_state['values']['pixgather_bundle'])) {
    variable_set('pixgather_bundle', $form_state['values']['pixgather_bundle']);
  }

  if (!empty($form_state['values']['pixgather_image_field'])) {
    variable_set('pixgather_image_field', $form_state['values']['pixgather_image_field']);
  }

  if (!empty($form_state['values']['pixgather_name_field'])) {
    variable_set('pixgather_name_field', $form_state['values']['pixgather_name_field']);
  }
  
  drupal_set_message(t('PixGather settings have been saved.'));

  $form_state['rebuild'] = true;
  
  // Reset the default rule (if it hasn't been overridden) to use the latest
  // node, field, etc.
  entity_defaults_rebuild(array('rules_config'));
}

/**
 * Callback for App requests for photos.
 *
 * @param integer $lowest_id the lowest entity id of the photo that the app
 * already has.  We want ids lower than this (older photos).
 */
function pixgather_view_photos($lowest_id) {

  // If the lowest id is 0, this is a first request.
  if ($lowest_id == 0) {
    $lowest_id = PHP_INT_MAX;
  }
  
  $entity_type = variable_get('pixgather_entity_type', NULL);
  $bundle = variable_get('pixgather_bundle', NULL);
  $info = entity_get_info($entity_type);

  $id_key = $info['entity keys']['id'];
  // Use EFQ to find older photos.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', $entity_type)
      ->entityCondition('bundle', $bundle)
      ->propertyCondition($id_key, $lowest_id, '<')
      ->propertyOrderBy($id_key, 'DESC')
      ->range(0, 4)
      ->execute();

  // If we have olders photos...
  if (isset($result[$entity_type])) {
    $json = array();
    $entities = entity_load_multiple_by_name($entity_type, array_keys($result[$entity_type]));
    
    // Set up the response JSON with url to the file, nid an author information.
    $photo_field = variable_get('pixgather_image_field', 'blagh');
    $name_field = variable_get('pixgather_name_field', NULL);
    foreach ($entities as $id => $entity) {
      $entity = (array) $entity;
      if (isset($entity[$photo_field][LANGUAGE_NONE][0]['uri']) && isset($entity[$name_field][LANGUAGE_NONE][0]['value'])) {
        $json[$id] = array(
          'url' => image_style_url('pixgather_phone_image', $entity[$photo_field][LANGUAGE_NONE][0]['uri']),
          'author' => $entity[$name_field][LANGUAGE_NONE][0]['value'],
        );
      }
    }

    if (sizeof($json) == 0) {
      echo "NONE";
    }
    else {
      // print the json response.
      echo json_encode($json);
    }

    exit;
  }
  else {

    // If there are no new photos, tell the app we're all set.
    echo "NONE";
  }
}
