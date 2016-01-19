<?php

/**
 * @file
 * Include file for the Vimeo Video Uploader.
 */

/**
 * Function provide form for Vimeo Video Uploader Configuration page.
 */
function vimeo_video_uploader_admin_form($form, &$form_state) {
  $form['vimeo_auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vimeo Video Upload Configuration'),
    '#description' => "Before Configuring, Visit <a target='_blank' href='sites/all/modules/custom/vimeo_video_uploader/README.txt'>README.txt</a>.",
  );
  $form['vimeo_auth']['user_id'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('vimeo_user_id'),
    '#title' => t('Enter Vimeo User Id'),
  );
  $form['vimeo_auth']['api_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('vimeo_api_key'),
    '#title' => t('Enter API Key (Client ID )'),
  );
  $form['vimeo_auth']['api_secret'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('vimeo_api_secret'),
    '#title' => t('Enter API Secret (Client Secret )'),
  );
  $form['vimeo_auth']['oauth_access_token'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('vimeo_oauth_access_token'),
    '#title' => t('Enter Oauth Access Token (Access token)'),
  );
  $form['vimeo_auth']['oauth_request_token_secret'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('vimeo_oauth_request_token_secret'),
    '#title' => t('Enter Oauth Access Token Secret (Access token secret)'),
  );
  $contenttypes = array_keys(node_type_get_types());
  $contenttypes = node_type_get_types();
  $contenttypes_val = array('' => "-Select-");
  foreach ($contenttypes as $type) {
    $contenttypes_val[$type->type] = $type->name;
  }
  $form['vimeo_auth']['content_type_select'] = array(
    '#type' => 'select',
    '#title' => t('Select the Content Types from which you have to upload video to Vimeo'),
    '#options' => $contenttypes_val,
    '#default_value' => variable_get('vimeo_content_type_select'),
  );
  $form['#validate'][] = 'vimeo_video_uploader_admin_form_validate';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Function provide validate form for Vimeo Video Uploader Configuration page.
 */
function vimeo_video_uploader_admin_form_validate($form_id, $form_state) {
  if (empty($form_state["values"]["user_id"]) ||
    empty($form_state["values"]["api_key"]) ||
    empty($form_state["values"]["api_secret"]) ||
    empty($form_state["values"]["oauth_access_token"]) ||
    empty($form_state["values"]["oauth_request_token_secret"]) ||
    empty($form_state["values"]["content_type_select"])) {
    drupal_set_message(t('Please enter your Vimeo Auth information.'), 'error');
    drupal_goto('admin/config/media/vimeo_video_uploader');
    drupal_exit();
  }
}

/**
 * Function triggered on submit of Vimeo Video Uploader Configuration page.
 */
function vimeo_video_uploader_admin_form_submit($form, $form_state) {
  variable_set('vimeo_user_id', $form["vimeo_auth"]["user_id"]["#value"]);
  variable_set('vimeo_api_key', $form["vimeo_auth"]["api_key"]["#value"]);
  variable_set('vimeo_api_secret', $form["vimeo_auth"]["api_secret"]["#value"]);
  variable_set('vimeo_oauth_access_token', $form["vimeo_auth"]["oauth_access_token"]["#value"]);
  variable_set('vimeo_oauth_request_token_secret', $form["vimeo_auth"]["oauth_request_token_secret"]["#value"]);
  $message = "Saved the Vimeo configuration.";
  $sel_con_type = $form["vimeo_auth"]["content_type_select"]["#value"];
  $exist_set_con_type = variable_get("vimeo_content_type_select");
  if ($sel_con_type != $exist_set_con_type) {
    field_delete_field("field_vimeo_file_browse");
    field_delete_field("field_embeddedvideo");
  }
  variable_set('vimeo_content_type_select', $sel_con_type);
  $node_type = node_type_load($sel_con_type);
  $instances = field_info_instances('node', $sel_con_type);
  $fields = array();
  foreach (array_keys($instances) as $field_instance) {
    $fields[$field_instance] = $field_instance;
  }
  if (!in_array("field_vimeo_file_browse", $fields)) {
    $field_name = 'field_vimeo_file_browse';
    $field = array(
      'field_name' => $field_name,
      'type' => 'file',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    );
    field_create_field($field);
    $instance = array(
      'field_name' => $field_name,
      'entity_type' => 'node',
      'bundle' => $sel_con_type,
      'settings' => array('file_extensions' => 'txt mp4'),
      'display' => array(
        'default' => array('label' => 'hidden', 'type' => 'hidden'),
      ),
      'label' => 'Browse video for uploading to Vimeo',
    );
    field_create_instance($instance);
    $message .= "Created 'Browse video for uploading to Vimeo' field in '" . strtoupper($sel_con_type) . "' Content type.";
  }
  if (!in_array("field_embeddedvideo", $fields)) {
    $field_name = 'field_embeddedvideo';
    $field = array(
      'field_name' => $field_name,
      'type' => 'video_embed_field',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    );
    field_create_field($field);
    $instance = array(
      'field_name' => $field_name,
      'entity_type' => 'node',
      'bundle' => $sel_con_type,
    );
    field_create_instance($instance);
  }
  drupal_set_message(check_plain($message), 'status');
}
