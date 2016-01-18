<?php

/**
 * Admin form
 */
function selectmenu_admin_form($form_state) {
  $form = array();
  $form['selectmenu_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable jQuery UI seletmenu'),
    '#default_value' => variable_get('selectmenu_enabled', TRUE),
    '#description' => t('Use this option to toggle jQuery UI selectmenu on and off globally.'),
  );
  $form['selectmenu_css_whitelist'] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelist: CSS selectors to include'),
    '#description' => t('Enter the CSS selectors of the select fields to apply jQuery Selectmenu to. This option SUPERCEDES all other rules on this page.'),
    '#rows' => 3,
    '#cols' => 40,
    '#default_value' => variable_get('selectmenu_css_whitelist', '#example-div .something span select'),
  );
  $form['selectmenu_form_id_exceptions'] = array(
    '#type' => 'textarea',
    '#title' => t('Blacklist: form CSS IDs to ignore'),
    '#description' => t('Enter the CSS IDs of the forms to not enable jQuery UI selectmenu on. One per line. Do not include the hash symbol, ie: "my-form-id" and not "#my-form-id"'),
    '#rows' => 3,
    '#cols' => 40,
    '#default_value' => variable_get('selectmenu_form_id_exceptions', ''),
  );
  $form['selectmenu_ignore_system_settings_forms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore system settings forms'),
    '#description' => t('Do not use jQuery UI selectmenu for Drupal system settings form.'),
    '#default_value' => variable_get('selectmenu_ignore_system_settings_forms', TRUE),
  );
  $form['selectmenu_ignore_overlay_forms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore forms inside Overlay'),
    '#description' => t('Do not use jQuery UI selectmenu for forms that are inside of the Drupal 7 Overlay.'),
    '#default_value' => variable_get('selectmenu_ignore_overlay_forms', TRUE),
  );
  $form['selectmenu_ignore_node_add_forms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore node add forms'),
    '#description' => t('Do not use jQuery UI selectmenu for Drupal node add forms.'),
    '#default_value' => variable_get('selectmenu_ignore_node_add_forms', TRUE),
  );
  $form['selectmenu_disable_for_admin_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable selectmenu for admin theme'),
    '#description' => t('Do not use jQuery UI selectmenu for any page that is using the admin theme set in Appearance settings. Only check this box if the site theme is different from the admin theme. Otherwise selectmenu will be disabled throughout the site.'),
    '#default_value' => variable_get('selectmenu_disable_for_admin_theme', FALSE),
  );

  // Script options fieldset.
  $selectmenu_script_options = variable_get('selectmenu_script_options', array());
  $form['selectmenu_script_options'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('jQuery UI Selectmenu script options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['selectmenu_script_options']['width'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#field_suffix' => 'px',
    '#default_value' => empty($selectmenu_script_options['width']) ? '' : $selectmenu_script_options['width'],
    '#title' => t('width'),
  );
  $form['selectmenu_script_options']['maxHeight'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#field_suffix' => 'px',
    '#default_value' => empty($selectmenu_script_options['maxHeight']) ? '' : $selectmenu_script_options['maxHeight'],
    '#title' => t('maxHeight'),
  );

  return system_settings_form($form);
}

/**
 * Admin form validation callback.
 */
function selectmenu_admin_form_validate(&$form, &$form_state) {
  // Check to see if the supplied numerical script options are in fact numerical.
  $expected_numbers = array('width', 'maxHeight');
  foreach ($form_state['values']['selectmenu_script_options'] as $option_key => $option_val) {
    if (in_array($option_key, $expected_numbers)) {
      $clean_option_val = str_ireplace('px', '', $option_val);
      if (!is_numeric($clean_option_val) && $clean_option_val !== '') {
        form_set_error("selectmenu_script_options][$option_key", t('Invalid setting supplied: This field needs to be a number or left blank.'));
      }

      if ($clean_option_val < 1 && $clean_option_val !== '') {
        form_set_error("selectmenu_script_options][$option_key", t('Invalid setting supplied: A positive number is required.'));
      }
    }
  }
}
