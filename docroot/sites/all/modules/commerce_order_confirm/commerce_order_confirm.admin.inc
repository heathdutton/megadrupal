<?php
/**
 * Settings form
 *
 */
function commerce_order_confirm_settings_form($form, &$form_state) {
  drupal_set_title(t('Order confirm settings'));

  $statuses = commerce_order_statuses();
  $options = array();
  foreach($statuses as $name => $status) {
    $options[$name] = $status['title'];
  }

  $form['statuses'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order statuses'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['statuses']['commerce_order_confirm_run_on'] = array(
    '#type' => 'radios',
    '#title' => t('Initial status'),
    '#default_value' => variable_get('commerce_order_confirm_run_on', 'pending'),
    '#options' => $options,
    '#description' => t('Status which on run the confirmation.'),
  );

  $form['statuses']['commerce_order_confirm_set_to'] = array(
    '#type' => 'radios',
    '#title' => t('Status after confirm'),
    '#default_value' => variable_get('commerce_order_confirm_set_to', 'completed'),
    '#options' => $options,
    '#description' => t('Status to set after the confirmation.'),
  );

  $form['redir'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirection'),
    '#weight' => 10,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $options = array(
    'none' => 'None, just display a confirmation message',
    'front' => 'The front page of the site',
    'order' => 'The commerce order page',
    'custom' => 'Custom url',
  );

  $form['redir']['commerce_order_confirm_redir'] = array(
    '#type' => 'radios',
    '#title' => t('Redirection target'),
    '#default_value' => variable_get('commerce_order_confirm_redir', 'none'),
    '#options' => $options,
    '#description' => t('Redirection target after the confirmation.'),
  );

  $form['redir']['commerce_order_confirm_redir_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom redirection path'),
    '#default_value' => variable_get('commerce_order_confirm_redir_custom', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="commerce_order_confirm_redir"]' => array('value' => 'custom'),
      ),
    ),
  );

  drupal_add_css(drupal_get_path('module', 'commerce_order_confirm') . '/css/commerce_order_confirm.admin.css');
  return system_settings_form($form);
}

/**
 * Form validate handler for the currency settings form.
 */
function commerce_order_confirm_settings_form_validate($form, &$form_state) {
  if ('custom' == $form_state['values']['commerce_order_confirm_redir']) {
    if (empty($form_state['values']['commerce_order_confirm_redir_custom'])) {
      //form_set_error($element['#name'], t('The custom path must given!'));
      form_set_error('commerce_order_confirm_redir_custom', t('The custom path must given!'));
    }
    if (!valid_url($form_state['values']['commerce_order_confirm_redir_custom'])) {
      form_set_error('commerce_order_confirm_redir_custom', t('The given url is not valid!'));
    }
  }
}
