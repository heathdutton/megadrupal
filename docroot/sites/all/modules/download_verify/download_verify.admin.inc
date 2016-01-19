<?php
/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Form Builder.
 *
 * @ingroup forms.
 * @see system_settings_form()
 */
function download_verify_configuration_form($form, &$form_state) {
  // Get the path to the module.
  $path = drupal_get_path('module', 'download_verify');
  // @todo: Add Honeypot protection, is the module available?.
  // @see https://drupal.org/project/honeypot
  // @todo: honeypot_add_form_protection($form, $form_state, array('honeypot', 'time_restriction'));
  // Attach the CSS to the form.
  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => $path . '/css/download_verify.css',
    ),
  );

  // Set up the mail field container.
  $form['download_verify_mail_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail configuration'),
    '#description' => t('Enter the e-mail address to be used for form submission configuration. This e-mail address will not be exposed to the site visitor.'),
    '#weight' => 1,
    '#collapsible' => FALSE,
  );
  // Single text field for email address entry.
  $form['download_verify_mail_settings']['download_verify_email'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('download_verify_email', 'yourname@example.com'),
    '#title' => t('Address to receive form mail.'),
    '#weight' => 2,
    '#collapsible' => FALSE,
  );

  // Set up the description for the CSS reference container.
  // HTML used to demonstrate code structure for less experienced end users.
  $download_verify_css_target_settings_description = '<p>Enter the CSS class name to be used for attaching the form to.<p><p>This can be any <em>wrapper</em> element that contains any number of HTML achor tags.</p>Suggested formats could be: <code>&lt;div class="<em>target-classname</em>"&gt;<br />&nbsp;&nbsp;&lt;a&gt;a link&lt;/a&gt;<br />&nbsp;&nbsp;&lt;a&gt;another link&lt;/a&gt;<br />&lt;/div&gt;</code><code>&lt;ul class="<em>target-classname</em>"&gt;<br />&nbsp;&nbsp;&lt;li&gt;&lt;a&gt;a link&lt;/a&gt;&lt;/li&gt;<br />&nbsp;&nbsp;&lt;li&gt;&lt;a&gt;another link&lt;/a&gt;&lt;/li&gt;<br />&lt;/ul&gt;</code> <code>&lt;div class="<em>target-classname</em>"&gt;<br />&nbsp;&nbsp;&lt;a&gt;&lt;img /&gt;&lt;/a&gt;<br />&lt;/div&gt;</code>';
  // Set up the CSS reference container.
  $form['download_verify_css_target_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS target configuration'),
    '#description' => filter_xss_admin($download_verify_css_target_settings_description),
    '#weight' => 3,
    '#collapsible' => FALSE,
  );
  // Single text field for CSS target reference entry.
  $form['download_verify_css_target_settings']['download_verify_css_target'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('download_verify_css_target', 'download-link-container'),
    '#title' => t('CSS class name.'),
    '#weight' => 4,
    '#collapsible' => FALSE,
  );

  // Form element for setting intro text.
  // Set up the intro text container.
  $form['download_verify_intro_text_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Intro text content'),
    '#description' => t('Enter the text you wish to be shown at the top of the form'),
    '#weight' => 5,
    '#collapsible' => FALSE,
  );
  // Single text field for intro text entry.
  $form['download_verify_intro_text_settings']['download_verify_intro_text'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('download_verify_intro_text', 'Please complete your details to continue the download.'),
    '#title' => t('The text to display'),
    '#weight' => 6,
    '#collapsible' => FALSE,
  );

  // Form element for setting footer text.
  // @todo: needs placeholder for link to privacy policy.
  $form['download_verify_footer_text_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer text content'),
    '#description' => t('Enter the text you wish to be shown at the bottom of the form'),
    '#weight' => 7,
    '#collapsible' => FALSE,
  );
  // Single text field for footer text entry.
  $form['download_verify_footer_text_settings']['download_verify_footer_text'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('download_verify_footer_text', 'Emails are collected for marketing purposes in line with our [privacy policy].'),
    '#title' => t('The text to display'),
    '#weight' => 8,
    '#collapsible' => FALSE,
  );

  // Set up fieldset for cookie options.
  $form['download_verify_cookie_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie options'),
    '#description' => t('Configurations to allow the setting of the cookie and its expiry time.'),
    '#weight' => 9,
    '#collapsible' => FALSE,
  );
  // Set up radio buttons for optional cookie setting.
  $form['download_verify_cookie_settings']['download_verify_cookie_display'] = array(
    '#type' => 'radios',
    '#title' => t('Set cookie on form submission.'),
    '#default_value' => variable_get('download_verify_cookie_display', 0),
    '#options' => array(
      0 => t('Off'),
      1 => t('On'),
    ),
    '#description' => t('Optional setting of the cookie on the users system. When this is set to <em>Off</em> then the module will not check for the presence of previous cookies either.'),
    '#weight' => 10,
    '#collapsible' => FALSE,
  );
  // Declare the cookie expiry options.
  $download_verify_cookie_expiry_values = drupal_map_assoc(array(
    86400,
    604800,
    2419200,
    7776000,
    15552000,
    31104000,
  ), 'format_interval');
  // Set up the select element for the cookie expiry time.
  $form['download_verify_cookie_settings']['download_verify_cookie_expiry'] = array(
    '#type' => 'select',
    '#title' => 'Cookie expiry time.',
    '#default_value' => variable_get('download_verify_cookie_expiry', 0),
    '#options' => $download_verify_cookie_expiry_values,
    '#description' => t('Set the expiry period for the cookie that is set on the users system.'),
    '#weight' => 11,
    '#collapsible' => FALSE,
  );
  // @todo: Additional textarea element for setting the body of the email sent.
  // @todo: Use placeholder tokens for details.
  return system_settings_form($form);
}
