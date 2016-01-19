<?php
/**
 * @file
 * Form callbacks for sending e-mails.
 */

/**
 * AJAX handler for sending e-mails.
 *
 * Opens a modal window with Batch API integrated.
 * Mimic the settings generated for the Drupal batch page.
 *
 * @see _batch_progress_page_js()
 * @see includes/batch.inc
 */
function _manymail_send_form_ajax($form, &$form_state) {
  if (!form_get_errors()) {
    require_once DRUPAL_ROOT . '/includes/batch.inc';

    $batch = batch_get();
    $batch['url_options']['query']['id'] = $batch['id'];

    $current_set = _batch_current_set();

    $js_setting = array(
      'ManyMail' => array(
        'sendError' => FALSE,
      ),
      'batch' => array(
        'errorMessage' => $current_set['error_message'] . '<br />' . $batch['error_message'],
         // Overwrite 'Initializing' again so it doesn't have the line break.
         // See batch_set() in includes/batch.inc.
        'initMessage' => t('Initializing.'),
        'sendMessage' => t('Sending mail.'),
        'uri' => url($batch['url'], $batch['url_options']),
        'throttlePause' => variable_get('manymail_options_throttle_pause', 5) * 1000,
      ),
    );
    drupal_add_js($js_setting, 'setting');

    // Add jQuery UI Dialog.
    drupal_add_library('system', 'ui.dialog');

    // Add own version of batch.js.
    drupal_add_library('system', 'drupal.progress');
    drupal_add_js(drupal_get_path('module', 'manymail') . '/misc/batch.js');

    // Add special batch styling.
    drupal_add_css(drupal_get_path('module', 'manymail') . '/theme/css/manymail.dialog.css');

    // Return the div that fires batch processing and a styling element.
    return '<div id="manymail-dialog-logo"></div><div id="progress"></div>';
  }
  else {
    // Return errors and make sure the user sees them by scrolling to the top.
    $js_setting['ManyMail'] = array(
      'sendError' => TRUE,
    );
    drupal_add_js($js_setting, 'setting');
    drupal_add_js(drupal_get_path('module', 'manymail') . '/misc/scrolltop.js');

    return theme_status_messages();
  }
}

/**
 * Base form function for sending e-mails.
 *
 * @ingroup forms
 */
function manymail_send_form($form, &$form_state) {
  $admin = user_access('manymail admin');

  // Provide a div for the AJAX callback.
  $form['batch'] = array(
    '#markup' => '<div id="manymail-overlay"></div>',
  );

  $form['sender'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sender'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if ($admin) {
    // Add a tooltip only if people can actually go to the configuration page.
    $form['sender']['#description'] = t('The default values for the fields below can be changed on the <a href="@config_page">general configuration page</a>.', array('@config_page' => url('admin/config/manymail/config')));
  }

  $form['sender']['from'] = array(
    '#type' => 'textfield',
    '#title' => t('From address'),
    '#default_value' => variable_get('manymail_default_from', ''),
    '#description' => t('The e-mail address that e-mails appear to be sent from.'),
    '#required' => TRUE,
  );

  $form['sender']['from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#default_value' => variable_get('manymail_default_from_name', ''),
    '#description' => t('The name that e-mails appear to be sent from.'),
    '#required' => TRUE,
  );

  $form['sender']['reply_to'] = array(
    '#title' => t('Reply-to information'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Expand the reply-to fieldset if there are defaults.
  $reply_to = variable_get('manymail_default_reply_to');
  $reply_to_name = variable_get('manymail_default_reply_to_name');
  if (!(empty($reply_to) && empty($reply_to_name))) {
    $form['sender']['reply_to']['#collapsed'] = FALSE;
  }

  $form['sender']['reply_to']['reply_to_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply-to address'),
    '#default_value' => variable_get('manymail_default_reply_to', ''),
    '#description' => t('The e-mail address you want recipients to reply to.<br />Leave empty if you want this to be the same as the From address.'),
  );

  $form['sender']['reply_to']['reply_to_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply-to name'),
    '#default_value' => variable_get('manymail_default_reply_to_name', ''),
    '#description' => t('The name for the reply-to address.<br />Leave empty if you want this to be the same as the From name.'),
  );

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if ($admin) {
    // Add a tooltip only if people can actually go to the configuration page.
    $form['content']['#description'] = t('The default values for the fields below can be changed on the <a href="@config_page">general configuration page</a>.', array('@config_page' => url('admin/config/manymail/config')));
  }

  $form['content']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('manymail_default_subject', ''),
    '#description' => t('The e-mail subject.'),
    '#required' => TRUE,
  );

  $form['content']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => variable_get('manymail_default_body', ''),
    '#description' => t('The e-mail body.'),
    '#required' => TRUE,
  );

  $signature = variable_get('manymail_default_signature', '');
  if (!empty($signature)) {
    // Only show the signature fieldset if there is actually a signature set.
    $form['signature'] = array(
      '#type' => 'fieldset',
      '#title' => t('Signature'),
      '#weight' => 4,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['signature']['include_signature'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include signature.'),
      '#default_value' => 1,
      '#description' => t('When checked, the following text will automatically be appended to the body.'),
    );

    $form['signature']['signature_example'] = array(
      '#type' => 'textarea',
      '#default_value' => $signature,
      '#disabled' => 1,
      '#description' => t('The content of this signature can only be changed on the general configuration page.'),
    );

    if ($admin) {
      // Add a link only if people can actually go to the configuration page.
      $form['signature']['signature_example']['#description'] = t('The content of this signature can only be changed on the  <a href="@config_page">general configuration page</a>.', array('@config_page' => url('admin/config/manymail/config')));
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 5,
    '#value' => t('Send e-mail'),
    '#ajax' => array(
      'callback' => '_manymail_send_form_ajax',
      'wrapper' => 'manymail-overlay',
      'method' => 'html',
    ),
  );

  return $form;
}

/**
 * Implements submission from the Form API.
 */
function manymail_send_form_submit($form, &$form_state) {
  global $user;
  $uid = (isset($user->uid)) ? $user->uid : 0;

  // Create the mass e-mail in the database for logging and recovery.
  $mlog = db_insert('manymail_log')
    ->fields(array(
      'uid' => $uid,
      'send_state' => serialize($form_state),
      'send_time' => REQUEST_TIME,
      'last_activity' => REQUEST_TIME,
    ))
    ->execute();

  // Start the send process.
  manymail_setup_mail_batch($mlog, $form_state);
}
