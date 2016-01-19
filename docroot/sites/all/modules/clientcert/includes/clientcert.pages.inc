<?php

/**
 * Menu form callback for admin page.
 *
 * Path: 'admin/config/people/clientcert'.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 */
function clientcert_admin_form($form, &$form_state) {
  $form['#id'] = 'clientcert-admin-form';

  $form['clientcert_encryption'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable encryption mode (Currently useless on production sites).'),
    '#weight' => -5,
    '#required' => FALSE,
    '#description' => t("Encryption is a planned feature. If enabled the encryption flag on clientcert item can be edited."),
    '#default_value' => variable_get('clientcert_encryption', FALSE),
  );

  $form['clientcert_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable clientcert debug mode.'),
    '#weight' => -5,
    '#required' => FALSE,
    '#description' => t("Currently only used on special url paths 'clientcert/require/%' and 'clientcert/optional/%'."),
    '#default_value' => variable_get('clientcert_debug', FALSE),
  );

  return system_settings_form($form);
}

/**
 * Menu callback: Page for special operating url paths to trigger rules event.
 *
 * Paths: 'clientcert/require/%' and 'clientcert/optional/%'
 */
function clientcert_page_verify($argument) {
  $content = '';
  $custom_argument = check_plain($argument);
  $content .= t('Clientcert operation with rules.');

  $debug = variable_get('clientcert_debug', FALSE);
  if ($debug) {
    foreach ($_SERVER as $key => $value) {
      $message = '[clientcert-debug] ';
      $message .= '$_SERVER['. "'" . $key . "'" . '] => '. $value;
      drupal_set_message($message, 'status');
    }
  }
  rules_invoke_event('clientcert_verify', $custom_argument);

  return $content;
}

/**
 * Menu callback: Present an clientcert item submission form.
 *
 * Path: 'clientcert/add/item'
 */
function clientcert_page_item_add() {
  $values = array();
  $values['status_login'] = 1;
  $values['status_encrypt'] = 0;
  $clientcert = entity_create('clientcert', $values);

  drupal_set_title(t('Create clientcert item manually.'), PASS_THROUGH);
  return drupal_get_form('clientcert_item_form', $clientcert);
}

/**
 * Menu callback.
 *
 * Path: 'clientcert/item/%/edit'.
 * Presents the clientcert item editing form, or redirects to delete
 * confirmation.
 *
 * @param object $clientcert
 *   The clientcert item object to edit.
 */
function clientcert_page_item_edit($clientcert) {
  drupal_set_title(t('<em>Edit clientcert item</em> @title', array('@title' => $clientcert->title)), PASS_THROUGH);

  return drupal_get_form('clientcert_item_form', $clientcert);
}

/**
 * Form builder; Displays the clientcert item add/edit form.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 * @param object $clientcert
 *   The clientcert object to edit, which may be new or manually created.
 */
function clientcert_item_form($form, &$form_state, $clientcert) {
  global $user;
  $form['#id'] = 'clientcert-item-form';

  $form['#clientcert'] = $clientcert;
  $form_state['clientcert'] = $clientcert;

  $title = t('Manual entry, to be customized');

  if (isset($clientcert->title)) {
    $title = $clientcert->title;
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Clientcert item title'),
    '#weight' => -5,
    '#required' => TRUE,
    '#default_value' => $title,
  );

  if (empty($clientcert->ccid)) {
    $form['cert'] = array(
      '#access' => clientcert_access('create'),
      '#type' => 'textarea',
      '#cols' => 67,
      '#rows ' => 31,
      '#resizable' => FALSE,
      '#maxlength' => 5000,
      '#title' => t('Manual adding a client cert in PEM format.'),
      '#default_value' => '',
      '#required' => TRUE,
      '#description' => t("You can export a client certificate in your browser and paste the data in PEM format here."),
    );
  }

  $form['status_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('active for login'),
    '#default_value' => $clientcert->status_login,
    '#description' => t("Only when enabled this certificate can be used for login."),
  );
  if (variable_get('clientcert_encryption', FALSE)) {
      $form['status_encrypt'] = array(
      '#type' => 'checkbox',
      '#title' => t('active for encryption'),
      '#default_value' => $clientcert->status_encrypt,
    );
  }

  $default_uid = 0;
  if (isset($clientcert->uid)) {
    $default_uid = $clientcert->uid;
  }
  else {
    $default_uid = $user->uid;
  }

  if ((user_access('administer clientcert'))
      && (empty($clientcert->uid) || $clientcert->uid == 0)) {
    $form['uid'] = array(
      '#access' => clientcert_access('create'),
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Administer: Manual adding or modifying the user id.'),
      '#default_value' => $default_uid,
      '#description' => t("With the certificate added below it's possible to login as user with this ID. The default value is ID of your current logged in account. Choose '0' to add an item for later change (can not be loaded for login)."),
    );
  }
  $form['status'] = array(
    '#access' => user_access('administer clientcert'),
    '#type' => 'checkbox',
    '#title' => t('Administer: clientcert item is active/verfied.'),
    '#default_value' => $clientcert->status,
    '#description' => t("This value is mainly used for automatic validation. Be careful with activation."),
  );
  // Add the buttons.
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;

  if (empty($clientcert->ccid)) {
    $form['buttons']['submit'] = array(
      '#access' => clientcert_access('create'),
      '#type' => 'submit',
      '#value' => t('create new clientcert item'),
      '#weight' => 5,
      '#submit' => array('clientcert_item_form_submit'),
    );
  }
  else {
    $form['buttons']['submit'] = array(
      '#access' => clientcert_access('update', $clientcert),
      '#type' => 'submit',
      '#value' => t('save clientcert item'),
      '#weight' => 5,
      '#submit' => array('clientcert_item_form_submit'),
    );

    $form['buttons']['delete'] = array(
      '#access' => clientcert_access('delete', $clientcert),
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('clientcert_item_form_delete_submit'),
    );
  }

  $form['#validate'][] = 'clientcert_item_form_validate';

  field_attach_form('clientcert', $clientcert, $form, $form_state);

  return $form;
}

/**
 * Form validation handler for clientcert_item_form().
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @see clientcert_item_form()
 */
function clientcert_item_form_validate($form, &$form_state) {
  $clientcert = $form_state['clientcert'];

  // Field validation.
  field_attach_form_validate('clientcert', $clientcert, $form, $form_state);
}

/**
 * Form submission handler for node_form().
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @see clientcert_item_formm()
 */
function clientcert_item_form_submit($form, &$form_state) {
  global $user;
  $clientcert = &$form_state['clientcert'];

  $clientcert->title = $form_state['values']['title'];

  if (isset($form_state['values']['uid'])) {
    $clientcert->uid = $form_state['values']['uid'];
  }
  elseif (!isset($clientcert->uid)) {
    $clientcert->uid = $user->uid;
  }

  $clientcert->status = $form_state['values']['status'];

  $clientcert->status_login = $form_state['values']['status_login'];

  $clientcert->status_encrypt = $form_state['values']['status_encrypt'];

  if (isset($form_state['values']['cert'])) {
    $clientcert->cert = $form_state['values']['cert'];
    $cert_hash = sha1(_clientcert_decode_pem($form_state['values']['cert']));
    $clientcert->cert_hash = $cert_hash;
  }
  if (!isset($clientcert->verify)) {
    $clientcert->verify = '';
  }
  if (!isset($clientcert->serial)) {
    $clientcert->serial = '';
  }
  if (!isset($clientcert->dn_subject)) {
    $clientcert->dn_subject = '';
  }
  if (!isset($clientcert->dn_issuer)) {
    $clientcert->dn_issuer = '';
  }

  // Notify field widgets.
  field_attach_submit('clientcert', $clientcert, $form, $form_state);

  // Save the clientcert item.
  clientcert_save($clientcert);

  drupal_set_message(t('clientcert item saved.'));

  $form_state['redirect'] = 'clientcert/item/' . $clientcert->ccid;
}

/**
 * Form submission handler for clientcert_item_form().
 *
 * Handles the 'Delete' button on the clientcert item form .
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @see clientcert_item_form()
 */
function clientcert_item_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $clientcert = $form['#clientcert'];
  $form_state['redirect'] = array('clientcert/item/' . $clientcert->ccid . '/delete', array('query' => $destination));
}

/**
 * Displays a clientcert item.
 *
 * @param array $clientcert
 *   The clientcert object to display.
 * @param string $view_mode
 *   The view mode we want to display.
 */
function clientcert_page_item_view($clientcert, $view_mode = 'full') {
  $clientcert->content = array();

  drupal_set_title(t('%title', array('%title' => filter_xss($clientcert->title))), PASS_THROUGH);
  if ($view_mode == 'teaser') {
    $clientcert->content['title'] = array(
      '#markup' => filter_xss($clientcert->title),
      '#weight' => -5,
    );
  }
  // Build fields content.
  field_attach_prepare_view('clientcert', array($clientcert->ccid => $clientcert), $view_mode);

  entity_prepare_view('clientcert', array($clientcert->ccid => $clientcert));
  $clientcert->content += field_attach_view('clientcert', $clientcert, $view_mode);

  return $clientcert->content;
}

/**
 * Form bulder; Asks for confirmation of clientcert item deletion.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 * @param object $clientcert
 *   The clientcert object to delete.
 */
function clientcert_page_item_delete_confirm($form, &$form_state, $clientcert) {
  $form['#clientcert'] = $clientcert;

  $form['ccid'] = array('#type' => 'value', '#value' => $clientcert->ccid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $clientcert->title)),
    'clientcert/item/' . $clientcert->ccid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Executes clientcert item deletion.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 */
function clientcert_page_item_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $clientcert = clientcert_load($form_state['values']['ccid']);
    clientcert_delete($form_state['values']['ccid']);
    watchdog('clientcert', 'deleted %title.', array('%title' => $clientcert->title));

    drupal_set_message(t('%title has been deleted.', array('%title' => $clientcert->title)));
  }

  $form_state['redirect'] = '<front>';
}
