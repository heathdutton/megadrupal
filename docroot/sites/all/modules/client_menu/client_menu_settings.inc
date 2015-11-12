<?php

/**
 * @file
 * Settings page for Client menu.
 */

/**
 * Create the settings page form.
 */
function client_menu_settings($form, &$form_state) {

	// Get Available menus.
	$available_menus =  menu_get_menus();

  // Create the fieldset for basic settings.
  $form['basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
  );

	// Choose menu to render.
  $form['basic_settings']['client_menu'] = array(
    '#title' => t('Client menu'),
    '#description' => t('Select the menu that will be used as client menu.'),
    '#type' => 'select',
    '#options' => $available_menus,
    '#default_value' => variable_get('client_menu', 'management'),
    '#required' => TRUE,
  );

  // Create the fieldset for the display options.
  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Settings'),
  );

	$form['display_settings']['client_menu_home_icon'] = array(
	  '#type' => 'checkbox',
	  '#default_value' => variable_get('client_menu_home_icon', TRUE),
	  '#title' => t('Display Home link icon'),
	  '#description' => t('<strong>Default value => Checked</strong>. 
	  Enabling this option will display a small home icon linking to your site homepage.'),
	);

	$form['display_settings']['client_menu_logout'] = array(
	  '#type' => 'checkbox',
	  '#default_value' => variable_get('client_menu_logout', TRUE),
	  '#title' => t('Display User logout link'),
	  '#description' => t('<strong>Default value => Checked</strong>. 
	  Enabling this option will display the user logout link.'),
	);

	$form['display_settings']['client_menu_sticky'] = array(
	  '#type' => 'checkbox',
	  '#default_value' => variable_get('client_menu_sticky', TRUE),
	  '#title' => t('Sticky menu'),
	  '#description' => t('<strong>Default value => Checked</strong>. 
	  Enabling this option will make the menu sticky and it will stand on top even when you scroll the page.'),
	);

  // Create the fieldset for the advanced settings.
  $form['mobile_menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Menu'),
    '#description' => '',
  );

	$form['mobile_menu']['client_menu_mobile'] = array(
	  '#type' => 'checkbox',
	  '#default_value' => variable_get('client_menu_mobile', TRUE),
	  '#title' => t('Enable MMenu (jQuery Mobile Menu)'),
	  '#description' => t('<strong>Default value => Checked</strong>. 
	  Enable this option if you want to use the jQuery !url plugin. 
	  This will include some extra js code 
	  but it will improve the overal mobile experiense.
	  If your "client menu" has no sub-menus it is better to disable this option for better performance.', 
	   array(
	     '!url' => l('mmenu', 'http://mmenu.frebsite.nl', array('attributes'=> array('target'=>'blank'))))
       ),
	);

	$form['mobile_menu']['client_menu_jquery_no_conflict'] = array(
	  '#type' => 'checkbox',
	  '#default_value' => variable_get('client_menu_jquery_no_conflict', TRUE),
	  '#title' => t('Load the requred jQuery 1.7 library automagically.'),
	  '#description' => t('<strong>Default value => Checked</strong>. Because the "jQuery update" module is known to be very buggy, 
	  this option will load the newer jQuery version 1.7 using the no-conflict method 
	  so it wont interfere with any existing jQuery or other java-script libraries. 
	  The only reason to uncheck this if you are already using a newer version of jQuery site-wide and its globally accessible by the "$" variable. 
	  Unchekig this option could save you some KBs, but it may also break your javasctipt if not used correctly.'),
    '#states' => array(
      // Hide the settings when the cancel notify checkbox is disabled.
      'visible' => array(
       ':input[name="client_menu_mobile"]' => array('checked' => TRUE),
      ),
      'unchecked' => array(
       variable_get('client_menu_jquery', 'TRUE') => FALSE,
      ),
     ),
	);

  // Create the submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;

}

/**
 * Submit handler for client_menu_settings().
 */
function client_menu_settings_submit($form, &$form_state) {

  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }

  // Display a message to the user.
  drupal_set_message(t('The configuration options have been saved.'));
}
