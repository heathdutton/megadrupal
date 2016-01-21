<?php
/**
 * @file
 * Contains configuration form for Recovery Password.
 */

/**
 * Callback to admin/config/people/recovery-pass.
 */
function recovery_pass_config_form($form, &$form_state) {
  $form = array();
  $form['recovery_pass_help_text'] = array(
    '#type' => 'item',
    '#markup' => t('Edit the e-mail messages sent to users who request a new password. The list of available tokens that can be used in e-mails is provided below. For displaying new password please use <strong>[user_new_password]</strong> placeholder.'),
  );

  $form['recovery_pass_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
    '#default_value' => _recovery_pass_mail_text('email_subject'),
  );

  $form['recovery_pass_email_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Body'),
    '#required' => TRUE,
    '#default_value' => _recovery_pass_mail_text('email_text'),
  );

  if (module_exists("htmlmail")) {
    // Adding description incase HTMLMAIL module exists.
    $form['recovery_pass_email_text']['#description'] = t('Supports HTML Mail provided HTMLMAIL module is enabled.');
  }

  if (module_exists("token")) {
    $form['token_help'] = array(
      '#type' => 'markup',
      '#token_types' => array('user'),
      '#theme' => 'token_tree_link',
    );
  }
  $form['recovery_pass_old_pass_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Warning message to users for trying old password at login form.'),
    '#default_value' => variable_get('recovery_pass_old_pass_show', 1),
  );

  $form['recovery_pass_old_pass_warning'] = array(
    '#type' => 'textarea',
    '#rows' => 2,
    '#title' => t('Old Password Warning Message'),
    '#description' => t('Warning message to be shown, if user after resetting the password uses the old password again.'),
    '#default_value' => _recovery_pass_mail_text('old_pass_warning'),
  );

  $form['recovery_pass_fpass_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Path after Forgot Password Page'),
    '#maxlength' => 255,
    '#default_value' => variable_get('recovery_pass_fpass_redirect', 'user'),
    '#description' => t('The path to redirect user, after forgot password form. This can be an internal Drupal path such as %add-node or an external URL such as %drupal. Enter %front to link to the front page.',
      array(
        '%front' => '<front>',
        '%add-node' => 'node/add',
        '%drupal' => 'http://drupal.org',
      )
    ),
    '#required' => TRUE,
    '#element_validate' => array('_recovery_pass_validate_path'),
  );

  $form['recovery_pass_expiry_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiry Period'),
    '#description' => t('Please enter expiry period in weeks. After these many weeks the record for old password warning to be shown to that particular user would be deleted.'),
    '#default_value' => variable_get('recovery_pass_expiry_period', '1'),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  return system_settings_form($form);
}

/**
 * Validates path entered by user.
 */
function _recovery_pass_validate_path($element, &$form_state, $form) {
  if (!empty($element['#value'])) {
    $path = $element['#value'];
    if (!drupal_valid_path($path) && !drupal_lookup_path('source', $path) && !url_is_external($path)) {
      form_error($element, t('Please provide valid redirect path.'));
    }
  }
}
