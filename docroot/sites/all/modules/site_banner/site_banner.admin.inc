<?php

/**
 * @file
 * Admin page callbacks for the site banner module.
 */

module_load_include('inc', 'site_banner', 'site_banner_options');

/**
 * Implements hook_form_FORM_ID() for node_type_form().
 */
function site_banner_admin_settings_form() {
  $form = array();

  // ****************
  // setup form structure
  // ****************
  $form = array_merge(site_banner_generate_banner_status_form_elements(variable_get('site_banner_status', FALSE), variable_get('site_banner_debug_status', FALSE)),
                      site_banner_generate_banner_text_form_elements(variable_get('site_banner_text', site_banner_get_default_text())),
                      site_banner_generate_bg_color_form_elements(variable_get('site_banner_background_color', site_banner_get_default_background_color())),
                      site_banner_generate_text_color_form_elements(variable_get('site_banner_text_color', site_banner_get_default_text_color())));

  // Alter form text to better suit admin page.
  $form['site_banner_text']['#description'] = t(
    'The text to be displayed on the site banner. If the !context_link module is being used, this will be the default text used when no context alters the site banner text.',
    array('!context_link' => l(t('Context'), 'https://drupal.org/project/context'))
  );

  $bg_suffix_description_text = '<br/>' . t(
    'If the !context_link module is being used, this will be the default background color used when no context alters the site banner background color.',
    array('!context_link' => l(t('Context'), 'https://drupal.org/project/context'))
  );
  $text_suffix_description_text = '<br/>' . t(
    'If the !context_link module is being used, this will be the default text color used when no context alters the site banner text color.',
    array('!context_link' => l(t('Context'), 'https://drupal.org/project/context'))
  );
  $form['site_banner_background_color_form']['#description'] .= $bg_suffix_description_text;
  $form['site_banner_text_color_form']['#description'] .= $text_suffix_description_text;
  $form['site_banner_background_custom_color_form']['#description'] .= $bg_suffix_description_text;
  $form['site_banner_text_custom_color_form']['#description'] .= $text_suffix_description_text;

  // Link to the submit handler below.
  $form['#submit'] = array('site_banner_admin_settings_form_submit');
  return system_settings_form($form);
}

/**
 * Implements hook_form_FORM_ID_validate() for node_type_form().
 */
function site_banner_admin_settings_form_validate($form, &$form_state) {
  site_banner_validate_background_color_code_form($form_state);
  site_banner_validate_text_color_code_form($form_state);
}

/**
 * Implements hook_form_FORM_ID_submit() for node_type_form().
 */
function site_banner_admin_settings_form_submit($form, &$form_state) {
  $banner_status = $form_state['values']['site_banner_status'];
  $banner_debug_status = $form_state['values']['site_banner_debug_status'];
  $background_color_code = site_banner_get_background_color_from_form($form_state);
  $text_color_code = site_banner_get_text_color_from_form($form_state);

  variable_set('site_banner_status', $banner_status);
  variable_set('site_banner_debug_status', $banner_debug_status);
  variable_set('site_banner_background_color', $background_color_code);
  variable_set('site_banner_text_color', $text_color_code);
}

/**
 * Generates the form elements for defining banner text.
 *
 * @param string $default_text
 *   the default text to populate banner text form control.
 *
 * @return array
 *   a form array populating form elements for banner background color.
 */
function site_banner_generate_banner_text_form_elements($default_text) {
  $form = array();

  $form['site_banner_text'] = array(
    '#title' => t('Banner text'),
    '#description' => t('The text to be displayed on the site banner.'),
    '#type' => 'textfield',
    '#size' => 50,
    '#maxlength' => 100,
    '#default_value' => $default_text,
  );

  return $form;
}
/**
 * Generates the form elements for enabling/disabling the site banner.
 *
 * @param bool $default_value_status
 *   banner display status
 * @param bool $default_value_debug_status
 *   banner context debug status
 *
 * @return array
 *   a form array populating form elements for banner background color.
 */
function site_banner_generate_banner_status_form_elements($default_value_status, $default_value_debug_status) {
  $form = array();

  $form['site_banner_status'] = array(
    '#title' => t('Banner active'),
    '#description' => t('Whether the site banner is active.'),
    '#type' => 'checkbox',
    '#default_value' => $default_value_status,
  );

  $form['site_banner_debug_status'] = array(
    '#title' => t('Banner debug mode active'),
    '#description' => t('Whether the debug mode is active: in this mode hyperlinks to administration pages are added to banner texts with links to active contexts to users with the "Administer site banner settings" permissions.'),
    '#type' => 'checkbox',
    '#default_value' => $default_value_debug_status,
  );

  return $form;
}

/**
 * Generates the form elements for defining background colours.
 *
 * @param string $existing_background_color
 *   the existing color as a HTML hexadecimal color code.
 *
 * @return array
 *   A Drupal form array populating form elements for banner background color.
 */
function site_banner_generate_bg_color_form_elements($existing_background_color) {
  $form = array();

  // Setup background color settings.
  $background_color_array = site_banner_get_background_colors();
  $background_color_array_names = array_values($background_color_array);
  $selected_background_index = 1;
  $using_custom_background_color = TRUE;

  // Check if background color is a preset color.
  if (in_array($existing_background_color, array_keys($background_color_array), TRUE)) {
    $using_custom_background_color = FALSE;

    // Using an existing color: find its name.
    $background_color_to_search = $background_color_array[$existing_background_color];
    $selected_background_index = array_search($background_color_to_search, $background_color_array_names, TRUE);
  }

  // Setup form elements.
  $form['site_banner_background_color_select'] = array(
    '#type' => 'checkbox',
    '#title' => t('Select a custom background color'),
    '#description' => t('Checking this box will allow you to define a custom background color for the site banner.'),
    '#default_value' => $using_custom_background_color,
  );
  $form['site_banner_background_color_form'] = array(
    '#title' => t('Banner background color'),
    '#description' => t('The background color for the site banner.'),
    '#type' => 'select',
    '#options' => $background_color_array_names,
    '#default_value' => $selected_background_index,
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="site_banner_background_color_select"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );
  $form['site_banner_background_custom_color_form'] = array(
    '#title' => t('Custom background color'),
    '#description' => t("A custom color code in hexadecimal format (for example #000000 for black) for the banner's custom background color."),
    '#type' => 'textfield',
    '#default_value' => $existing_background_color,
    '#size' => 7,
    '#maxlength' => 7,
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="site_banner_background_color_select"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  return $form;
}

/**
 * Generates the form elements for defining text colors.
 *
 * @param string $existing_text_color
 *   the existing color as a HTML hexadecimal color code.
 *
 * @return array
 *   A Drupal form array populating form elements for site banner text color
 */
function site_banner_generate_text_color_form_elements($existing_text_color) {
  // Setup text color settings.
  $text_color_array = site_banner_get_text_colors();
  $text_color_array_names = array_values($text_color_array);
  $selected_text_index = 1;
  $using_custom_text_color = TRUE;

  // Check if background color is a preset color.
  if (in_array($existing_text_color, array_keys($text_color_array), TRUE)) {
    $using_custom_text_color = FALSE;

    // Using an existing color: find its name.
    $text_color_to_search = $text_color_array[$existing_text_color];
    $selected_text_index = array_search($text_color_to_search, $text_color_array_names, TRUE);
  }

  // Setup form elements.
  $form['site_banner_text_color_select'] = array(
    '#type' => 'checkbox',
    '#title' => t('Select a custom text color'),
    '#description' => t('Checking this box will allow you to define a custom text color for the site banner.'),
    '#default_value' => $using_custom_text_color,
  );
  $form['site_banner_text_color_form'] = array(
    '#title' => t('Banner text color'),
    '#description' => t('The site banner text color.'),
    '#type' => 'select',
    '#options' => $text_color_array_names,
    '#default_value' => $selected_text_index,
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="site_banner_text_color_select"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );
  $form['site_banner_text_custom_color_form'] = array(
    '#title' => t('Custom text color'),
    '#description' => t("A custom color code in hexadecimal format (for example #000000 for black) for the banner's custom text color."),
    '#type' => 'textfield',
    '#default_value' => $existing_text_color,
    '#size' => 7,
    '#maxlength' => 7,
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="site_banner_text_color_select"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  return $form;
}

/**
 * Searches the form response to determine the background color to be saved.
 *
 * @param array $form_state
 *   the object containing user settings
 *
 * @return string
 *   the background color as HTML hex color code
 */
function site_banner_get_background_color_from_form($form_state) {
  return site_banner_get_background_color_code(
    $form_state['values']['site_banner_background_color_form'],
    $form_state['values']['site_banner_background_custom_color_form'],
    $form_state['values']['site_banner_background_color_select']
  );
}

/**
 * Searches the form response to determine the text color to be saved.
 *
 * @param array $form_state
 *   the object containing user settings
 *
 * @return string
 *   the text color as HTML hex color code
 */
function site_banner_get_text_color_from_form($form_state) {
  return site_banner_get_text_color_code(
    $form_state['values']['site_banner_text_color_form'],
    $form_state['values']['site_banner_text_custom_color_form'],
    $form_state['values']['site_banner_text_color_select']
  );
}

/**
 * Validate background color submissions in form content.
 *
 * @param array $form_state
 *   the object containing user settings
 */
function site_banner_validate_background_color_code_form($form_state) {
  $is_custom_background_color = $form_state['values']['site_banner_background_color_select'];
  $background_color_code_as_text = $form_state['values']['site_banner_background_custom_color_form'];

  // Test that if custom color is selected, then a valid color is entered.
  if ($is_custom_background_color) {
    if (preg_match(site_banner_get_html_color_regex_pattern(), $background_color_code_as_text) != 1) {
      form_set_error('site_banner_background_custom_color_form', t('Please enter a valid hexadecimal color code for the background color (for example #ff00ff) or select an existing color.'));
    }
  }
}

/**
 * Validate text color submissions in form content.
 *
 * @param array $form_state
 *   the form_sate object containing user settings
 */
function site_banner_validate_text_color_code_form($form_state) {
  $is_custom_text_color = $form_state['values']['site_banner_text_color_select'];
  $text_color_code_as_text = $form_state['values']['site_banner_text_custom_color_form'];

  // Test that if custom color is selected, then a valid color is entered.
  if ($is_custom_text_color) {
    if (preg_match(site_banner_get_html_color_regex_pattern(), $text_color_code_as_text) != 1) {
      form_set_error('site_banner_text_custom_color_form', t('Please enter a valid hexadecimal color code for the text color (for example #ff00ff) or select an existing color.'));
    }
  }
}
