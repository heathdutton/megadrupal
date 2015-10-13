<?php

/**
 * @file vkxp.forms.inc
 * Contains node form alters.
 */

/**
 * Add vkxp settings to node settings form.
 *
 * @param $form
 *   Node settings form.
 */
function _vkxp_process_node_settings_form(&$form) {

  // Do not process node form if it do not contain node type.
  if (empty($form['#node_type']->type)) {
    return;
  }

  $node_type = $form['#node_type']->type;

  $form['vkxp'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('VKontakte CrossPoster settings'),
    '#group'  => 'additional_settings',
  );

  // Enable setting.
  $form['vkxp']['vkxp_node_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable VKontakte CrossPoster'),
    '#default_value' => variable_get('vkxp_node_enabled_' . $node_type),
    '#description'   => t('Enable or disable crosspost for this node type.')
  );

  // Enable default setting.
  $form['vkxp']['vkxp_node_enabled_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Checkbox "Post this node to VK" are checked by default'),
    '#default_value' => variable_get('vkxp_node_enabled_default_' . $node_type),
    '#description' => t('Check this if you want checkbox "Post this node to VK" in node forms was checked by default.')
  );

  // Get information about node fields.
  $fields_info = field_info_instances('node', $node_type);

  // Collect all text and image fields.
  $text_fields  = array('title' => t('Title'));
  $image_fields = array(0 => t('- None -'));
  foreach ($fields_info as $field_name => $field_instance) {
    if ($field_instance['widget']['module'] == 'text') {
      $text_fields[$field_name] = $field_instance['label'] . ' (' . $field_name . ')';
    }
    elseif ($field_instance['widget']['module'] == 'image') {
      $image_fields[$field_name] = $field_instance['label'] . ' (' . $field_name . ')';
    }
  }

  // Message.
  $form['vkxp']['vkxp_body'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Message'),
  );

  // Message field.
  $form['vkxp']['vkxp_body']['vkxp_node_message_field'] = array(
    '#type'          => 'select',
    '#title'         => t('Message source'),
    '#description'   => t('Select field from which to take the text to crosspost.'),
    '#options'       => $text_fields,
    '#default_value' => variable_get('vkxp_node_message_field_' . $node_type, 'title'),
  );

  // Message length.
  $form['vkxp']['vkxp_body']['vkxp_node_message_length'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Cut message'),
    '#field_prefix'  => t('Cut message if it has more than'),
    '#field_suffix'  => t('symbols'),
    '#size'          => 4,
    '#default_value' => variable_get('vkxp_node_message_length_' . $node_type, 255),
    '#description'   => t('Leave this field empty to not cut a message.'),
    '#attributes'    => array('style' => 'width:auto;'), // We need to break default Drupal style for this field.
  );

  // Image.
  $form['vkxp']['vkxp_image'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Image'),
  );

  // Image source.
  $form['vkxp']['vkxp_image']['vkxp_node_image_field'] = array(
    '#type'          => 'select',
    '#title'         => t('Image source'),
    '#description'   => t('Select field from which to take the image to crosspost.'),
    '#options'       => $image_fields,
    '#default_value' => variable_get('vkxp_node_image_field_' . $node_type, 0),
  );

  // Image limit.
  $form['vkxp']['vkxp_image']['vkxp_node_image_limit'] = array(
    '#type'          => 'select',
    '#title'         => t('Images limit'),
    '#description'   => t('Select maximum amount of images to crosspost.'),
    '#options'       => drupal_map_assoc(range(0, 10)),
    '#default_value' => variable_get('vkxp_node_image_limit_' . $node_type, 0),
  );
}

/**
 * Add vkxp fieldset to node add/edit form.
 *
 * @param $form
 *   Node add/edit form.
 */
function _vkxp_process_node_form(&$form) {

  $form['vkxp'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('VKontakte CrossPoster'),
    '#group'  => 'additional_settings',
    '#weight' => -10,
  );

  $form['vkxp']['vkxp_post_this_node'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Post this node to VK'),
    '#default_value' => variable_get('vkxp_node_enabled_default_' . $form['#node']->type),
    '#description'   => t('After you submit this node it will be posted to VK.'),
  );

  // Disable crosspost if cURL library is not installed on a server.
  if (!function_exists('curl_init')) {
    $form['vkxp']['vkxp_post_this_node']['#disabled']    = TRUE;
    $form['vkxp']['vkxp_post_this_node']['#value']       = 0;
    $form['vkxp']['vkxp_post_this_node']['#description'] = t("You can't crosspost nodes until cURL library is not installed on your server.");
  }

  // Disable crosspost if no access token recieved.
  if (!variable_get('vkxp_access_token')) {
    $form['vkxp']['vkxp_post_this_node']['#disabled']    = TRUE;
    $form['vkxp']['vkxp_post_this_node']['#value']       = 0;
    $form['vkxp']['vkxp_post_this_node']['#description'] = t("You can't crosspost nodes until access token not recieved from VK.");
  }
}
