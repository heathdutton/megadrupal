<?php
/**
 * @file
 * Commerce Offline Payment Admin Settings.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_osp_settings_form($settings = NULL) {
  $languages = array();
  foreach(language_list() as $code => $values) {
    $languages[$code] = $values->name;
  }

  $form = array();

  $form['#tree'] = TRUE;
  $form['payment_settings_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="payment-settings-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($settings['method_count'])) {
    $settings['method_count'] = 1;
  }

  $form['method_count'] = array(
    '#type' => 'hidden',
    '#value' => $settings['method_count'],
    '#default_value' => $settings['method_count'],
  );

  $form['payment_settings_wrapper']['title'] = array(
    '#type' => 'textfield',
    '#title' => t("Payment Method Title"),
    '#description' => t("This is going to be the method name shown on the checkout payment pane. This value is translatable."),
    '#default_value' => $settings['payment_settings_wrapper']['title'],
    '#required' => TRUE,
  );

  for ($i = 0; $i < $settings['method_count']; $i++) {
    $form['payment_settings_wrapper']["method_$i"] = array(
      '#type' => 'fieldset',
      '#title' => t('Method %count', array('%count' => $i+1)),
      '#collapsible' => TRUE,
    );

    $form['payment_settings_wrapper']["method_$i"]['language'] = array(
      '#type' => 'checkboxes',
      '#title' => t("Language"),
      '#description' => t("This payment details will show up only for the selected language."),
      '#options' => $languages,
      '#default_value' => $settings['payment_settings_wrapper']["method_$i"]['language'],
      '#required' => TRUE,
    );

    $form['payment_settings_wrapper']["method_$i"]['details'] = array(
      '#type'=>'text_format',
      '#title' => t('Payment Details'),
      '#default_value' => $settings['payment_settings_wrapper']["method_$i"]['details']['value'],
      '#format'=>'full_html',
      '#required' => TRUE,
    );
  }
  $form['payment_settings_wrapper']['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add another payment details'),
    '#submit' => array('commerce_osp_add_more_add_details'),
    '#ajax' => array(
      'callback' => 'commerce_osp_add_more_callback',
      'wrapper' => 'payment-settings-wrapper',
    ),
  );
  if ($settings['method_count'] > 1) {
    $form['payment_settings_wrapper']['remove_details'] = array(
      '#type' => 'submit',
      '#value' => t('Remove payment details'),
      '#submit' => array('commerce_osp_add_more_remove_details'),
      '#ajax' => array(
        'callback' => 'commerce_osp_add_more_callback',
        'wrapper' => 'payment-settings-wrapper',
      ),
    );
  }

  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 */
function commerce_osp_add_more_callback($form, $form_state) {
  return $form['parameter']['payment_method']['settings']['payment_method']['settings']['payment_settings_wrapper'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function commerce_osp_add_more_add_details($form, &$form_state) {
  $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings']['method_count']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function commerce_osp_add_more_remove_details($form, &$form_state) {
  if ($form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings']['method_count'] > 1) {
    $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings']['method_count']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function commerce_osp_form_rules_ui_edit_element_alter(&$form, &$form_state) {
  // Reconstruct form only when ajax triggers it.
  if (!empty($form_state['triggering_element']['#submit'][0])) {
    $trigger = $form_state['triggering_element']['#submit'][0];
    $ajax_triggers = array('commerce_osp_add_more_add_details', 'commerce_osp_add_more_remove_details');

    if (in_array($trigger, $ajax_triggers)) {
      $settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
      $form['parameter']['payment_method']['settings']['payment_method']['settings'] = commerce_osp_settings_form($settings);
    }
  }
}

// TODO: Add a condition to this payment method rule checking if there is at least one method option language
// equals to the session language.
