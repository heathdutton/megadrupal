<?php

/**
 * @file
 * Admin page callbacks for the password strength module.
 */

/**
 * Form builder for password strength form.
 *
 * @see system_settings_form()
 * @ingroup forms
 */
function password_strength_settings() {
  if (!class_exists('ZxcvbnPhp\Zxcvbn')) {
    drupal_set_message(t('The password strength library Zxcvbn-Php is not available. Consult the README.md for installation instructions.'), 'error');
    return array('error' => array('#markup' => t('Unable to configure Password Strength, module requirements not met.')));
  }

  $form = array();
  $desc = "Password strengths are measured and based both on the amount of entropy (randomness) in the password and an estimate of the amount of time it could take to crack the password using brute-force attempts. The estimated time to crack at each level is about two orders of magnitude greater than the next lower level, so a Weak password can be cracked nearly instantly, while a Very strong password might take years.";
  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t($desc) . '</p>',
  );
  $form['password_strength_default_required_score'] = array(
    '#type' => 'select',
    '#title' => t('Minimum required password strength'),
    '#description' => t('The minimum strength required for user account passwords.'),
    '#default_value' => variable_get('password_strength_default_required_score', NULL),
    '#options' => password_strength_score_list(),
  );
  $form['test'] = array(
    '#type' => 'fieldset',
    '#title' => 'Test password strength',
    '#description' => t('Try different passwords to see what strength they are.'),
    '#collapsible' => TRUE,
    '#collapsed' => variable_get('password_strength_default_required_score', NULL),
  );
  $form['test']['password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password test'),
    '#attributes' => array(
      'class' => array('password-field'),
    ),
  );
  global $user;
  password_strength_form_password_js_attach($form['test']['password'], $user);

  $form['password_strength_default_password_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum password length'),
    '#size' => 4,
    '#description' => t('The minimum password length in number of characters.'),
    '#default_value' => variable_get('password_strength_default_password_length', 7),
  );

  return system_settings_form($form);
}
