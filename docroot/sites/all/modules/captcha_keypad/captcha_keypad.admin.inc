<?php
/**
 * @file captcha_keypad.admin.inc
 * Admin settings.
 */

/**
 * Form builder.
 */
function captcha_keypad_settings_form($form, &$form_state) {
  $form['captcha_keypad_code_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Code size'),
    '#description' => t('Size of the code.'),
    '#size' => 2,
    '#maxlength' => 2,
    '#default_value' => variable_get('captcha_keypad_code_size', 4),
    '#required' => TRUE,
  );

  $form['captcha_keypad_shuffle_keypad'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shuffle keypad'),
    '#description' => t('Selecting this option will make the keys appear in random order.'),
    '#default_value' => variable_get('captcha_keypad_shuffle_keypad', TRUE),
  );

  $form_ids = array();
  if (module_exists('contact')) {
    $form_ids[] = 'contact_site_form';
    $form_ids[] = 'contact_personal_form';
  }
  if (module_exists('forum')) {
    $form_ids[] = 'forum_node_form';
  }
  if (module_exists('user')) {
    $form_ids[] = 'user_register_form';
    $form_ids[] = 'user_pass';
    $form_ids[] = 'user_login';
    $form_ids[] = 'user_login_block';
  }
  foreach (node_type_get_names() as $type => $name) {
    $form_ids[] = 'comment_node_' . $type . '_form';
  }
  sort($form_ids);
  $form['captcha_keypad_forms'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Forms'),
    '#options' => array_combine(array_values($form_ids), array_values($form_ids)),
    '#default_value' => variable_get('captcha_keypad_forms', array()),
    '#description' => t('Select which forms to add captcha keypad.'),
  );

  return system_settings_form($form);
}

/**
 * Form validate handler.
 */
function captcha_keypad_settings_form_validate($form, &$form_state) {
  variable_set('captcha_keypad_forms', $form_state['values']['captcha_keypad_forms']);
}

/**
 * Form submit handler.
 */
function captcha_keypad_settings_form_submit($form, &$form_state) {

}
