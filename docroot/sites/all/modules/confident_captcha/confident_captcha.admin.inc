<?php

/**
 * @file
 * Implementation of admin settings().
 */
function confident_captcha_admin_settings() {
  $form = array();
  $form['confident_captcha_customer_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer ID'),
    '#default_value' => variable_get('confident_captcha_customer_id', ''),
    '#maxlength' => 40,
    '#description' => t('The Customer ID given to you when you <a href="@url" target="_blank">registered at login.confidenttechnologies.com</a>.', array('@url' => url('https://login.confidenttechnologies.com'))),
    '#required' => TRUE,
   );
  $form['confident_captcha_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => variable_get('confident_captcha_site_id', ''),
    '#maxlength' => 40,
    '#description' => t('The SiteID given to you when you <a href="@url" target="_blank">registered at login.confidenttechnologies.com</a>.', array('@url' => url('https://login.confidenttechnologies.com'))),
    '#required' => TRUE,
  );
  $form['confident_captcha_api_username'] = array(

    '#type' => 'textfield',
    '#title' => t('API username'),
    '#default_value' => variable_get('confident_captcha_api_username', ''),
    '#maxlength' => 40,
    '#description' => t('The API username given to you when you <a href="@url" target="_blank">registered at login.confidenttechnologies.com</a>.', array('@url' => url('https://login.confidenttechnologies.com'))),
    '#required' => TRUE,
   );
  $form['confident_captcha_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('API password'),
    '#default_value' => variable_get('confident_captcha_api_password', ''),
    '#maxlength' => 40,
    '#description' => t('The API password given to you when you <a href="@url" target="_blank">registered at login.confidenttechnologies.com</a>.', array('@url' => url('https://login.confidenttechnologies.com'))),
    '#required' => TRUE,
  );

  $form['confident_captcha_theme_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Confident CAPTCHA settings'),
    '#collapsible' => TRUE,
  );

  $form['confident_captcha_theme_settings']['confident_captcha_grid_color'] = array(
    '#type' => 'select',
    '#title' => t('Image Code Color'),
    '#description' => t('Color of the round background behind the characters on the Confident CAPTCHA.'),
    '#options' => array(
      'Red' => t('Red'),
      'White' => t('White'),
      'Orange' => t('Orange'),
      'Yellow' => t('Yellow'),
      'Green' => t('Green'),
      'Teal' => t('Teal'),
      'Blue' => t('Blue'),
      'Indigo' => t('Indigo'),
      'Violet' => t('Violet'),
      'Gray' => t('Gray'),
    ),
    '#default_value' => variable_get('confident_captcha_grid_color', 'Blue'),
  );

  $form['confident_captcha_theme_settings']['confident_captcha_grid_height'] = array(
    '#type' => 'textfield',
    '#title' => t('ImageShield Height'),
    '#maxlength' => 2,
    '#default_value' => variable_get('confident_captcha_grid_height', 3),
    '#description' => t('Height of the Confident CAPTCHA in images. Width multiplied by height may not exceed 16.'),
  );

  $form['confident_captcha_theme_settings']['confident_captcha_grid_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#maxlength' => 2,
    '#default_value' => variable_get('confident_captcha_grid_width', 3),
    '#description' => t('Width of the Confident CAPTCHA in images. Width multiplied by height may not exceed 16.'),
  );

  $form['confident_captcha_theme_settings']['confident_captcha_captcha_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length'),
    '#maxlength' => 1,
    '#default_value' => variable_get('confident_captcha_captcha_length', 4),
    '#description' => t('Number of categories that must be entered to solve the Confident CAPTCHA.'),
  );

  $form['confident_captcha_theme_settings']['confident_captcha_display_style'] = array(
    '#type' => 'select',
    '#title' => t('Display style'),
    //'#options' => array('lightbox' => t('Lightbox'), 'flyout' => t('Flyout'), 'modal' => t('Modal')),
	'#options' => array('flyout' => t('Flyout')),
    '#default_value' => variable_get('confident_captcha_display_style', 'flyout'),
    '#description' => t('Indicates the UI style the Confident CAPTCHA uses.'),
  );

  $form['confident_captcha_theme_settings']['confident_captcha_logo_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Logo Name'),
    '#maxlength' => 40,
    '#default_value' => variable_get('confident_captcha_logo_name', ''),
    '#description' => t('Your site logo.'),
  );

  $form['confident_captcha_theme_settings']['confident_captcha_billboard_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Billboard Name'),
    '#maxlength' => 40,
    '#default_value' => variable_get('confident_captcha_billboard_name', ''),
    '#description' => t('Your site banner.'),
  );

  $form['confident_captcha_advanced_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['confident_captcha_advanced_options']['confident_captcha_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => variable_get('confident_captcha_server_url', 'http://captcha.confidenttechnologies.com'),
    '#maxlength' => 128,
    '#description' => t('The URL of the service, if you are running your own server.'),
    '#required' => TRUE,
  );
  $form['confident_captcha_advanced_options']['confident_captcha_fail_open'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fail Open'),
    '#default_value' => variable_get('confident_captcha_fail_open', TRUE),
    '#description' => t('If for some reason the Confident CAPTCHA API fails, should the user be allowed to continue?'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}


/**
 * The validation on the confident_captcha administration settings
 */
function confident_captcha_admin_settings_validate($form, &$form_state) {
  // Validate captcha length
  $len = $form_state['values']['confident_captcha_captcha_length'];
  if (!is_numeric($len)) {
    form_set_error('confident_captcha_captcha_length', t('CAPTCHA Length must be an integer.'));
  } else if($len <= 3) {
    form_set_error('confident_captcha_captcha_length', t('CAPTCHA Length must be 4 or greater'));
  }
  // Validate grid height
  $height = $form_state['values']['confident_captcha_grid_height'];
  if (!is_numeric($height)) {
    form_set_error('confident_captcha_grid_height', t('Height must be an integer.'));
  } else if($height <= 1) {
    form_set_error('confident_captcha_grid_height', t('Height Length must be 2 or greater'));
  }

  // Validate grid width
  $width = $form_state['values']['confident_captcha_grid_width'];
  if (!is_numeric($width)) {
    form_set_error('confident_captcha_grid_width', t('Width must be an integer.'));
  } else if($width <= 1) {
    form_set_error('confident_captcha_grid_width', t('Width must be 2 or greater'));
  }


  if (($form_state['values']['confident_captcha_grid_width'] * $form_state['values']['confident_captcha_grid_height']) > 26) {
    form_set_error('confident_captcha_grid_width');
    form_set_error('confident_captcha_grid_height');
    drupal_set_message(t('Width multiplied by height may not exceed 16.'), 'error');
  }
}