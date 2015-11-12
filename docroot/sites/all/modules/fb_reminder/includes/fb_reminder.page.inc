<?php

/**
 * @file
 *  Page callback functions.
 */

/**
 * Form builder. Reminder page.
 */
function fb_reminder_reminder_page($ctools_js = NULL) {
  // Fall back if $js is not set.
  if (!$ctools_js) {
    return drupal_get_form('fb_reminder_reminder_form');
  }

  $redirect_uri = drupal_get_destination();
  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => t('Facebook reminder'),
    'ajax' => TRUE,
    'redirect' => $redirect_uri['destination'],
  );
  $output = ctools_modal_form_wrapper('fb_reminder_reminder_form', $form_state);
  print ajax_render($output);
  exit;
}

/**
 * Building form. Reminder form.
 */
function fb_reminder_reminder_form($form, &$form_state) {
  $message = _fb_reminder_get_message();
  if ($message) {
    $app_id = variable_get('fb_post_app_id', '');
    $path = 'https://www.facebook.com/dialog/oauth?client_id=' . $app_id .
      '&redirect_uri=' . $form_state['redirect'] . '&response_type=code&scope=publish_stream';
    $connect = l(t('Connect to Facebook account'), $path, array('attributes' => array('class' => 'fb-post-connect')));
    $close = l(t('Cancel'), '#', array('attributes' => array('onclick' => 'Drupal.CTools.Modal.dismiss();'), 'external' => TRUE));
    $options = array(
      'message' => $message,
      'connect' => $connect,
      'close' => $close,
    );
    $form['message'] = array(
      '#markup' => theme('fb_reminder_message', $options),
    );
  }

  return $form;
}
