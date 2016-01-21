<?php

/**
 * @file
 * Admin only functionality for the peerjs module.
 */

/**
 * Menu form callback; Display the peerjs admin form.
 */
function peerjs_admin_form($form, $form_state) {
  $form = array();
  $settings = variable_get('peerjs', array(
    'general' => array(
      'server' => FALSE,
      'css' => TRUE,
    ),
    'server' => array(
      'host' => '',
      'port' => '80',
      'path2app' => '/',
    ),
    'constraints' => array(
      'audio' => TRUE,
      'video' => TRUE,
      'video_settings' => array(
        'minWidth' => 120,
        'maxWidth' => 240,
        'minHeight' => 120,
        'maxHeight' => 240,
        'minFrameRate' => 15,
        'maxFrameRate' => 15,
      ),
    ),
  ));

  // Check that all libraries exist.
  $required_libraries = array('peerjs');
  foreach ($required_libraries as $name) {
    $library = libraries_detect($name);
    if (!$library['installed']) {
      drupal_set_message($library['error message'], 'error');
    }
  }

  $form['peerjs'] = array(
    '#tree' => TRUE,
  );
  $form['peerjs']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
  );
  $form['peerjs']['general']['server'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to choose server when creating new chat rooms?'),
    '#description' => t('If disabled, all chat rooms will use the default server.'),
    '#default_value' => $settings['general']['server'],
  );
  $form['peerjs']['general']['css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use default CSS styling provided by PeerJS Chat module?'),
    '#options' => array(
      0 => 'Disabled',
      1 => 'Enabled',
    ),
    '#default_value' => $settings['general']['css'],
  );
  $form['peerjs']['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Server'),
  );
  $form['peerjs']['server']['host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#description' => t('Enter the server address of the peer server. Defaults to localhost.'),
    '#default_value' => $settings['server']['host'],
    '#attributes' => array(
      'placeholder' => t('example-peerjs.herokuapp.com'),
    ),
  );
  $form['peerjs']['server']['port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#description' => t('Enter the server port of the peer server. Defaults to 80.'),
    '#default_value' => $settings['server']['port'],
  );
  $form['peerjs']['server']['path2app'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Application'),
    '#description' => t('Enter the server port of the peer server. Defaults to /.'),
    '#default_value' => $settings['server']['path2app'],
  );
  $form['peerjs']['constraints'] = array(
    '#type' => 'fieldset',
    '#title' => t('getUserMedia Contraints'),
    '#description' => t('Set which media types are allowed when accessing user media.'),
  );
  $form['peerjs']['constraints']['audio'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Audio'),
    '#options' => array(
      0 => 'Disabled',
      1 => 'Enabled',
    ),
    '#default_value' => $settings['constraints']['audio'],
  );
  $form['peerjs']['constraints']['video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Video'),
    '#options' => array(
      0 => 'Disabled',
      1 => 'Enabled',
    ),
    '#default_value' => $settings['constraints']['video'],
  );
  $form['peerjs']['constraints']['video_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video Settings'),
    '#description' => t('Enter the default video constraint settings (getUserMedia).'),
  );
  $form['peerjs']['constraints']['video_settings']['minWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('minWidth'),
    '#default_value' => $settings['constraints']['video_settings']['minWidth'],
  );
  $form['peerjs']['constraints']['video_settings']['maxWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('maxWidth'),
    '#default_value' => $settings['constraints']['video_settings']['maxWidth'],
  );
  $form['peerjs']['constraints']['video_settings']['minHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('minHeight'),
    '#default_value' => $settings['constraints']['video_settings']['minHeight'],
  );
  $form['peerjs']['constraints']['video_settings']['maxHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('maxHeight'),
    '#default_value' => $settings['constraints']['video_settings']['maxHeight'],
  );
  $form['peerjs']['constraints']['video_settings']['minFrameRate'] = array(
    '#type' => 'textfield',
    '#title' => t('minFrameRate'),
    '#default_value' => $settings['constraints']['video_settings']['minFrameRate'],
  );
  $form['peerjs']['constraints']['video_settings']['maxFrameRate'] = array(
    '#type' => 'textfield',
    '#title' => t('maxFrameRate'),
    '#default_value' => $settings['constraints']['video_settings']['maxFrameRate'],
  );

  return system_settings_form($form);
}

/**
 * Form constructor for the peerjs_chat add/edit form.
 *
 * @see peerjs_chat_form_validate()
 * @see peerjs_chat_form_submit()
 * @see peerjs_chat_form_delete_submit()
 * @ingroup forms
 */
function peerjs_chat_form($form, &$form_state, $entity) {
  $settings = variable_get('peerjs', array());

  // Check if settings have been set.
  if (empty($settings)) {
    drupal_set_message(t('Chatroom default settings have not been set, please set them or contact the website administrator.'), 'warning');
    return $form;
  }

  // If users are not allowed to set server settings, check for default.
  if (!$settings['general']['server'] && (!isset($settings['server']['host']) || empty($settings['server']['host']))) {
    drupal_set_message(t('Chatroom default settings have not been set, please set them or contact the website administrator.'), 'warning');
    return $form;
  }

  $form['peerjs_chat'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => !empty($entity->label) ? $entity->label : '',
    '#maxlength' => 255,
  );

  if ($settings['general']['server']) {
    $form['server'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default Server'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['server']['host'] = array(
      '#type' => 'textfield',
      '#title' => t('Host'),
      '#description' => t('Enter the server address of the peer server. Defaults to localhost.'),
      '#default_value' => !empty($entity->host) ? $entity->host : $settings['server']['host'],
    );
    $form['server']['port'] = array(
      '#type' => 'textfield',
      '#title' => t('Port'),
      '#description' => t('Enter the server port of the peer server. Leave blank to automatically choose port (80 or 443).'),
      '#default_value' => !empty($entity->port) ? $entity->port : $settings['server']['port'],
    );
    $form['server']['path2app'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to Application'),
      '#description' => t('Enter the application path of the peer server. Defaults to /.'),
      '#default_value' => !empty($entity->path2app) ? $entity->path2app : $settings['server']['path2app'],
    );
  }
  else {
    $form['host'] = array(
      '#type' => 'hidden',
      '#default_value' => !empty($entity->host) ? $entity->host : $settings['server']['host'],
    );
    $form['port'] = array(
      '#type' => 'hidden',
      '#default_value' => !empty($entity->port) ? $entity->port : $settings['server']['port'],
    );
    $form['path2app'] = array(
      '#type' => 'hidden',
      '#default_value' => !empty($entity->path2app) ? $entity->path2app : $settings['server']['path2app'],
    );
  }

  field_attach_form('peerjs_chat', $entity, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (!empty($entity->cid) && peerjs_chat_access('delete', $entity)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('peerjs_chat_form_delete_submit'),
    );
  }

  return $form;
}

/**
 * Validation handler for peerjs_chat_add_form form.
 */
function peerjs_chat_form_validate($form, &$form_state) {
  field_attach_form_validate('peerjs_chat', $form_state['values']['peerjs_chat'], $form, $form_state);
}

/**
 * Submit handler for peerjs_chat_add_form form.
 */
function peerjs_chat_form_submit($form, &$form_state) {
  $entity = entity_ui_controller('peerjs_chat')->entityFormSubmitBuildEntity($form, $form_state);

  if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0) {
    $entity->created = time();
  }

  $entity->save();

  $form_state['redirect'] = $entity->uri();
}

/**
 * Delete handler for peerjs_chat_form form.
 */
function peerjs_chat_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = array(
    'peerjs_chat/' . $form_state['peerjs_chat']->cid . '/delete',
    array(
      'query' => array(
        'destination' => isset($_GET['destination']) ? $_GET['destination'] : '<front>',
      ),
    ),
  );
  unset($_GET['destination']);
}
