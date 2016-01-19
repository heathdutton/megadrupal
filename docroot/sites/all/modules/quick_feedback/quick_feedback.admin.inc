<?php
/**
 * @file
 * Separate include for admin settings.
 * NOTE: Updates here should be reflected also in quick_feedback.variable.inc
 */

/**
 * Implements settings for the module.
 */
function quick_feedback_settings_form() {
  // Groupings.
  $form = array(
    'demo' => array(
      '#type' => 'fieldset',
      '#title' => t('Demo'), 
      '#weight' => -5, 
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    ),
    'settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Settings'), 
      '#weight' => 0,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    ),
    'messaging' => array(
      '#type' => 'fieldset',
      '#title' => t('Messaging'), 
      '#weight' => 10, 
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    ),
    'forms' => array(
      '#type' => 'fieldset',
      '#title' => t('Forms'), 
      '#weight' => 15, 
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    ),
  );
  // Demo link.
  $link = _quick_feedback_build_output();
  $form['demo']['markup'] = array(
    '#markup' => $link . '<br><br><code>' . htmlentities($link) . '</code><br><br>' .
    t('This is how the link will act now with the current settings.'),
  );

  // Settings options...
  $settings = _quick_feedback_get_settings();

  $form['settings'][SETTINGS_PREFIX . 'link_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $settings['link_mail'],
    '#description' => t('Who should receive messsages? Default is the site admin (will stay updated). Recommended: use a no-reply@example.com type address.'),
  );
  $form['settings'][SETTINGS_PREFIX . 'link_obfuscate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Obfuscate address'),
    '#default_value' => $settings['link_obfuscate'],
    '#description' => t('Prevent spammers from getting your email address, kind of. It uses HTML entities which can still be scraped.'),
  );
  $form['settings'][SETTINGS_PREFIX . 'link_target'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open in a new tab'),
    '#default_value' => $settings['link_target'],
    '#description' => t('Add a target="blank" to feedback links to open a new tab. Useful when webmail is used as browser default.'),
  );

  // Debug options.
  $form['settings'][SETTINGS_PREFIX . 'debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug Info'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Debug options will be included in the email body.'),
  );
  $form['settings']['debug'][SETTINGS_PREFIX . 'mail_inc_browser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Browser'),
    '#default_value' => $settings['mail_inc_browser'],
  );
  $form['settings']['debug'][SETTINGS_PREFIX . 'mail_inc_os'] = array(
    '#type' => 'checkbox',
    '#title' => t('Operating System'),
    '#default_value' => $settings['mail_inc_os'],
  );
  $form['settings']['debug'][SETTINGS_PREFIX . 'mail_inc_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Page URL'),
    '#default_value' => $settings['mail_inc_url'],
  );

  // Messaging.
  $form['messaging'][SETTINGS_PREFIX . 'link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link Text'),
    '#default_value' => $settings['link_text'],
    '#description' => t('Text to be added to form and email linked.'),
  );
  $form['messaging'][SETTINGS_PREFIX . 'mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings['mail_subject'],
    '#description' => t('Provide a subject admins will notice.'),
  );
  $form['messaging'][SETTINGS_PREFIX . 'mail_body'] = array(
    '#type' => 'textarea',
    '#rows'=> 10,
    '#title' => t('Body'),
    // Form save seems to prune a line off when trying to leave blank lines at the top of a text area.
    '#default_value' => "\r\n" . $settings['mail_body'],
    '#description' => t('NOTE: Should include a debug header to accominate browser, OS, and URL information appending.'),
  );

  // Forms.
  $form['forms'][SETTINGS_PREFIX . 'forms_list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Forms that should include link'),
    '#options' => array(
      // Custom forms can be added here via form alter.
      'contact_site_form' => t('Contact Us'),
      'user_register_form' => t('User Registration'),
      'user_login' => t('User Login'),
      'user_login_block' => t('User Login Block'),
    ),
    '#default_value' => $settings['forms_list'],
    '#description' => t('These forms will get a link in the suffix to allow feedback in the case of difficulty.'),
  );
  // Additional forms are managed as an array.
  $forms_additional = implode("\r\n", $settings['forms_additional']);
  $form['forms'][SETTINGS_PREFIX . 'forms_additional'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional forms'),
    '#default_value' => $forms_additional,
    '#description' => t('Additional forms that should get the feedback link; one per line.'),
    '#attributes' => array(
      'wrap' => 'off',
    ),
  );

  $form['#submit'][] = 'quick_feedback_settings_submit';

  return system_settings_form($form);
}


/**
 * Extras processing for settings save.
 */
function quick_feedback_settings_submit($form, &$form_state) {
  // Save additional forms as an array.
  $form_state['values'][SETTINGS_PREFIX . 'forms_additional'] =
    explode("\r\n", $form_state['values'][SETTINGS_PREFIX . 'forms_additional']);

  // Don't save admin email if site default.
  $default_email = variable_get('site_mail', ini_get('sendmail_from'));
  if ($form_state['values'][SETTINGS_PREFIX . 'link_mail'] == $default_email) {
    unset($form_state['values'][SETTINGS_PREFIX . 'link_mail']);
  }
}
