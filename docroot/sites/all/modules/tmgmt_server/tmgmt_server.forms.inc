<?php
/**
 * @file
 * TMGMT server forms.
 */

/**
 * Directory settings form to enter DS auth keys.
 */
function tmgmt_server_ds_settings_form($form, &$form_state) {

  $ds_url = variable_get('tmgmt_ds_url', TMGMT_SERVER_DEFAULT_DS_URL);

  /**
   * @var TMGMTDirectoryServerController $ds_controller
   */
  $ds_controller = entity_get_controller('tmgmt_directory_server');

  // Try to load existing DS.
  $ds = $ds_controller->loadByUrl($ds_url);

  if (!empty($ds->sid)) {
    $key = tmgmt_auth_receiver_controller()->loadByEntity('tmgmt_directory_server', $ds->sid);
  }

  $form['ds'] = array(
    '#type' => 'fieldset',
    '#title' => t('TMGMT Directory connection settings'),
    '#prefix' => '<div id="ds-keys-config-wrapper">',
    '#suffix' => '</div>',
  );

  $form['ds']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('TMGMT Directory URL'),
    '#default_value' => !empty($ds->url) ? $ds->url : 'https://',
  );

  if (empty($key)) {
    $form['ds']['key_info'] = array(
      '#markup' => t('<strong>In case you do not have the TMGMT Directory key you can get one by registering your Translation Server at <a href="@url" target="_blank">@server</a>.</strong>',
        array('@server' => $ds_url, '@url' => $ds_url . '/register/ts-owner')),
    );
  }

  $form['ds']['pub'] = array(
    '#type' => 'textfield',
    '#title' => t('Public key'),
    '#default_value' => !empty($key) ? $key->pub : NULL,
  );
  $form['ds']['private'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key'),
    '#default_value' => !empty($key) ? $key->private : NULL,
  );
  // If we have a connection test result, display it.
  if (isset($form_state['connection_test_result'])) {
    if (isset($form_state['connection_test_result']['error'])) {
      $form['ds']['test_ds_connection_info'] = array(
        '#markup' => '<div class="messages error">' . $form_state['connection_test_result']['error'] . '</div>',
      );
    }
    elseif (isset($form_state['connection_test_result']['success'])) {
      $form['ds']['test_ds_connection_info'] = array(
        '#markup' => '<div class="messages status">' . $form_state['connection_test_result']['success'] . '</div>',
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Verify connection and save'),
    '#ajax' => array(
      'callback' => 'tmgmt_server_ds_settings_form_ajax',
      'wrapper' => 'ds-keys-config-wrapper',
    ),
  );

  return $form;
}

function tmgmt_server_ds_settings_form_ajax($form, &$form_state) {
  return $form['ds'];
}

function tmgmt_server_ds_settings_form_validate($form, &$form_state) {
  // Perform validation of the url entered.
  if (!filter_var($form_state['values']['url'], FILTER_VALIDATE_URL)) {
    form_set_error('url', t('Invalid URL provided.'));
  }

  if (strlen($form_state['values']['pub']) != TMGMT_AUTH_KEY_LENGTH) {
    form_set_error('pub', t('Invalid length of the public key.'));
  }

  if (strlen($form_state['values']['private']) != TMGMT_AUTH_KEY_LENGTH) {
    form_set_error('pub', t('Invalid length of the private key.'));
  }
}

function tmgmt_server_ds_settings_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  // Set DS url.
  variable_set('tmgmt_ds_url', $form_state['values']['url']);

  /**
   * @var TMGMTDirectoryServerController $ds_controller
   */
  $ds_controller = entity_get_controller('tmgmt_directory_server');
  $ds = $ds_controller->loadByUrl($form_state['values']['url']);

  if (!empty($ds)) {
    $key = tmgmt_auth_receiver_controller()->loadByEntity('tmgmt_directory_server', $ds->sid);
  }
  // If we do not have a DS record for provided url, create one.
  else {
    $ds = $ds_controller->create(array(
      'url' => $form_state['values']['url']
    ));
    $ds_controller->save($ds);
  }

  // Receive key only in case it is needed.
  if (empty($key) || $key->pub != $form_state['values']['pub'] || $key->private != $form_state['values']['private']) {

    // Build key info for the receiver controller.
    $key_info = array(
      'pub' => $form_state['values']['pub'],
      'private' => $form_state['values']['private'],
      'created' => time(),
      'expires' => 0,
    );

    // Try to receive the key.
    try {
      tmgmt_auth_receiver_controller()->receive($key_info, 'tmgmt_directory_server', $ds->sid, TRUE);
      drupal_set_message(t('TMGMT Directory settings saved.'));
      // When we received the key, sync lang definitions as well and enable
      // them as this is the initial setup.
      tmgmt_server_sync_ds_lang_definitions(1);
    }
    catch (TMGMTAuthReceiverException $e) {
      form_set_error('pub', t('System error occurred, could not store the key. Error: %error', array('%error' => $e->getMessage())));
    }
  }

  tmgmt_server_ds_settings_test();
}

/**
 * Entity form callback for the Client entity.
 */
function tmgmt_server_client_form($form, &$form_state, $entity = NULL) {

  $form_state['tmgmt_server_client'] = $entity;

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($entity->label) ? $entity->label : NULL,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#description' => t('Human readable identifier of the client system.'),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('You can provide a short description of the client system.'),
    '#default_value' => isset($entity->description) ? $entity->description : NULL,
    '#maxlength' => 255,
  );

  $form['uuid'] = array(
    '#type' => 'textfield',
    '#title' => t('UUID (URL)'),
    '#description' => t('Provide a unique identifier of the client system. It is recommended to use its URL if available.'),
    '#default_value' => isset($entity->uuid) ? $entity->uuid : NULL,
    '#maxlength' => 255,
    '#required' => empty($entity->cid),
    '#disabled' => (!empty($entity->cid) && !user_access('administer remote clients')),
    '#element_validate' => array('tmgmt_server_client_form_element_validate_uuid'),
  );

  if (!empty($entity->cid)) {
    $key = tmgmt_auth_issuer_controller()->loadByEntity('tmgmt_server_client', $entity->cid);
    $form['pub'] = array(
      '#type' => 'textfield',
      '#title' => t('Public key'),
      '#default_value' => $key->pub,
      '#disabled' => TRUE,
    );
    $form['private'] = array(
      '#type' => 'textfield',
      '#title' => t('Private key'),
      '#default_value' => $key->private,
      '#disabled' => TRUE,
    );
  }

  if ((!isset($entity->cid) && isset($entity->uid)) || !user_access('administer remote clients')) {
    $form['uid'] = array(
      '#type' => 'value',
      '#value' => !empty($entity->uid) ? $entity->uid : $GLOBALS['user']->uid,
    );
  }
  elseif (user_access('administer remote clients')) {
    $form['uid'] = array(
      '#type' => 'textfield',
      '#title' => t('Owner'),
      '#autocomplete_path' => 'user/autocomplete',
      '#maxlength' => '60',
      '#description' => t('The owner of the client entity.'),
      '#required' => TRUE,
      '#element_validate' => array('tmgmt_server_client_form_element_validate_uid'),
    );

    if (!empty($entity->uid) && $user = user_load($entity->uid)) {
      $form['uid']['#default_value'] = $user->name;
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => !empty($entity->cid) ? t('Save client') : t('Add client'),
  );

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => !empty($entity->cid),
    '#limit_validation_errors' => array(),
    '#submit' => array('tmgmt_server_client_form_delete_submit'),
    '#attributes' => array('onclick' => 'return confirm(\'' . t('This action will remove the client.') . '\');')
  );

  return $form;
}

/**
 * Form submit handler for the Client entity form.
 */
function tmgmt_server_client_form_submit($form, &$form_state) {
  $entity = $form_state['tmgmt_server_client'];
  $form_state['redirect'] = 'user/' . arg(1) . '/remote-clients';

  // Process the submitted form values and save the entity.
  entity_form_submit_build_entity('tmgmt_server_client', $entity, $form, $form_state);
  entity_get_controller('tmgmt_server_client')->save($entity);

  // Output a success message to the user.
  drupal_set_message(t('The client has been saved.'));
}

/**
 * Delete submit callback.
 */
function tmgmt_server_client_form_delete_submit($form, &$form_state) {
  $entity = $form_state['tmgmt_server_client'];
  entity_get_controller('tmgmt_server_client')->delete(array($entity->cid));
  drupal_set_message(t('The client was deleted.'));
}

function tmgmt_server_ds_lang_sync_form($form, &$form_state) {

  $language_pairs = tmgmt_server_get_language_pairs();
  $predefined_languages = tmgmt_server_languages_predefined_list();
  $languages = language_list();
  $ds_connector = new TMGMTServerDSConnector(variable_get('tmgmt_ds_url', TMGMT_SERVER_DEFAULT_DS_URL));

  try {
    $ds_language_pairs = $ds_connector->getLangPairs();
    $ds_connector->reset();
    $ds_supported_languages = $form_state['ds_supported_languages'] = $ds_connector->getSupportedLanguages();
  }
  catch (Exception $e) {
    drupal_set_message(t('Cannot configure language capabilities synchronisation as connection to the TMGMT Directory failed. Please make sure you have configured the <a href="@link">TMGMT Directory key</a> correctly.',
      array('@link' => url('admin/directory-server'))), 'warning');
    return $form;
  }

  $ds_supported_languages_items = array();
  foreach ($ds_supported_languages as $lang_code => $lang_info) {
    $ds_supported_languages_items[] = "<strong>$lang_code</strong> {$lang_info['name']}";
  }

  $form['#attached']['js'][] = drupal_get_path('module', 'tmgmt_server') . '/js/tmgmt_server_lc_sync.js';

  $form['supported_lang_info'] = array(
    '#type' => 'item',
    '#title' => t('Languages supported by the TMGMT Directory'),
    '#markup' => theme('item_list', array('items' => $ds_supported_languages_items)),
  );

  $form['tmgmt_server_auto_lc_sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically synchronise all existing and all new language capabilities.'),
    '#default_value' => variable_get('tmgmt_server_auto_lc_sync'),
    '#description' => t('Note that this setting will automatically propagate to TMGMT Directory all new translators\' skills as your Translation server language capabilities.')
  );

  $form['language_pairs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Language capabilities'),
    '#description' => t('Identify which language capabilities you wish to propagate to the TMGMT Directory.'),
    '#tree' => TRUE,
  );

  $unsupported_languages = array();
  $lang_pairs_elements_names = array();

  $form_state['ds_language_pairs'] = $ds_language_pairs;

  foreach ($language_pairs as $language_pair) {

    $source_language = $language_pair['source_language'];
    $target_language = $language_pair['target_language'];

    $form['language_pairs'][$source_language]['#tree'] = TRUE;

    $unsupported = FALSE;

    if (!isset($ds_supported_languages[$source_language])) {
      $unsupported = TRUE;
      $unsupported_languages[$source_language] = $source_language;
    }
    if (!isset($ds_supported_languages[$target_language])) {
      $unsupported = TRUE;
      $unsupported_languages[$target_language] = $target_language;
    }

    $pair_exists = tmgmt_server_lang_pair_exists($form_state['ds_language_pairs'], $source_language, $target_language);

    $capability_element_name = 'capability_' . $source_language . '_' . $target_language;
    $lang_pairs_elements_names[] = $capability_element_name;

    $language_pair_exists_locally = isset($languages[$source_language]) && isset($languages[$target_language]);

    $form['language_pairs'][$source_language][$target_language] = array(
      '#type' => 'checkbox',
      '#title' => t('From %source_language to %target_language', array(
        '%source_language' => isset($predefined_languages[$source_language]) ? $predefined_languages[$source_language] : $source_language,
        '%target_language' => isset($predefined_languages[$target_language]) ? $predefined_languages[$target_language] : $target_language,
      )),
      '#default_value' => $pair_exists,
      '#disabled' => (!$language_pair_exists_locally || variable_get('tmgmt_server_auto_lc_sync')),
      '#attributes' => array('class' => array($capability_element_name)),
    );

    if (!$language_pair_exists_locally) {
      $form['language_pairs'][$source_language][$target_language]['#description'] = t('One or both of the pair languages are not installed locally.');
    }
    elseif ($pair_exists && $unsupported) {
      $form['language_pairs'][$source_language][$target_language]['#description'] = t('Not yet enabled at the TMGMT Directory.');
    }
    elseif ($unsupported) {
      $form['language_pairs'][$source_language][$target_language]['#description'] = t('The skill will not be enabled after sync at the TMGMT Directory.');
    }
  }

  drupal_add_js(array('tmgmt_server' => array('lang_pairs_elements_names' => $lang_pairs_elements_names)), 'setting');

  $unsupported_languages_names = array();
  foreach ($unsupported_languages as $unsupported_language) {
    $unsupported_languages_names[] = isset($predefined_languages[$unsupported_language]) ? $predefined_languages[$unsupported_language] : $unsupported_language;
  }

  if (!empty($unsupported_languages_names)) {
    drupal_set_message(format_plural(
      count($unsupported_languages),
      'Following language: @languages is not supported by the TMGMT Directory.',
      'Following languages: @languages are not supported by the TMGMT Directory.',
      array('@languages' => implode(', ', $unsupported_languages_names))
    ), 'warning');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function tmgmt_server_ds_lang_sync_form_submit($form, &$form_state) {
  variable_set('tmgmt_server_auto_lc_sync', $form_state['values']['tmgmt_server_auto_lc_sync']);

  $data = array();
  $local_language_pairs = tmgmt_server_get_language_pairs();

  // Search for DS language pairs which are abandoned and set them for removal.
  foreach ($form_state['ds_language_pairs'] as $ds_pair) {
    if (!tmgmt_server_lang_pair_exists($local_language_pairs, $ds_pair['source_language'], $ds_pair['target_language'])) {
      $data[] = array(
        'source_language' => $ds_pair['source_language'],
        'target_language' => $ds_pair['target_language'],
        'action' => 'delete',
      );
    }
  }

  if ($form_state['values']['tmgmt_server_auto_lc_sync']) {
    foreach ($local_language_pairs as $local_language_pair) {
      $language_pairs[$local_language_pair['source_language']][$local_language_pair['target_language']] = $local_language_pair['target_language'];
    }
  }
  else {
    $language_pairs = $form_state['values']['language_pairs'];
  }

  $languages = language_list();

  foreach ($language_pairs as $source_language => $target_languages) {

    if (isset($languages[$source_language]) && !in_array($source_language, array_keys($form_state['ds_supported_languages'])) && !isset($proposed_languages[$source_language])) {
      $proposed_languages[$source_language] = array(
        'code' => $languages[$source_language]->language,
        'name' => $languages[$source_language]->name,
        'native' => $languages[$source_language]->native,
        'direction' => $languages[$source_language]->direction,
      );
    }

    foreach ($target_languages as $target_language => $enabled) {

      // Skip if we do not want to propagate.
      if (!$enabled && !tmgmt_server_lang_pair_exists($form_state['ds_language_pairs'], $source_language, $target_language)) {
        continue;
      }

      // Skip if capability is already propagated.
      if ($enabled && tmgmt_server_lang_pair_exists($form_state['ds_language_pairs'], $source_language, $target_language)) {
        continue;
      }

      if (isset($languages[$target_language]) && !in_array($target_language, array_keys($form_state['ds_supported_languages'])) && !isset($proposed_languages[$target_language])) {
        $proposed_languages[$target_language] = array(
          'code' => $languages[$target_language]->language,
          'name' => $languages[$target_language]->name,
          'native' => $languages[$target_language]->native,
          'direction' => $languages[$target_language]->direction,
        );
      }

      $data[] = array(
        'source_language' => $source_language,
        'target_language' => $target_language,
        'action' => ($enabled ? 'create' : 'delete'),
      );
    }
  }
//debug($data);
debug($proposed_languages);
  $ds_connector = new TMGMTServerDSConnector(variable_get('tmgmt_ds_url', TMGMT_SERVER_DEFAULT_DS_URL));

  try {

    // Push languages that are not yet supported.
    if (!empty($proposed_languages)) {
      $ds_connector->pushProposedSupportedLanguages($proposed_languages);
      $ds_connector->reset();
    }

    $ds_connector->pushLangPairs($data);
    drupal_set_message(t('Language capabilities have been synchronised with the TMGMT Directory.'));
  }
  catch (TMGMTRemoteException $e) {
    drupal_set_message($e->getMessage(), 'error');
    watchdog_exception('tmgmt_server', $e);
  }


  tmgmt_server_sync_ds_lang_definitions();
}
