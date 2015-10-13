<?php
/**
 * @file
 * Admin interfaces for viewport module.
 */

/**
 * Configuration form for viewport tag values and presence settings.
 *
 * Presented at: admin/config/user-interface/viewport.
 */
function viewport_settings_form() {
  $form = array();

  $form['viewport'] = array(
    '#type' => 'fieldset',
    '#description' => t('The values for the viewport metatag will be output in the same way as entered here
      for the selected pages below. <br/>
      For the numeric values with decimal digits, use the decimal point "." instead of a comma "," <br/>
      If you don\'t know what are these values for, you should visit the !help-page to learn more about the Viewport metatag',
        array(
          '!help-page' => l(t("Viewport's help page"), 'admin/help/viewport'),
        )
    ),
  );
  $form['viewport']['viewport_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => variable_get('viewport_width', ''),
    '#description' => t('You probably want this to be %device-width, but a fixed number of pixels (only the number) is accepted too.',
      array('%device-width' => 'device-width')),
  );
  $form['viewport']['viewport_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => variable_get('viewport_height', ''),
    '#description' => t('%device-height, or a fixed number of pixels (only the number).',
      array('%device-height' => 'device-height')),
  );
  $form['viewport']['viewport_initial_scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial Scale'),
    '#default_value' => variable_get('viewport_initial_scale', ''),
    '#description' => t('Any value in the range (0, 10.0]. Usually this is set to 1.0'),
  );
  $form['viewport']['viewport_minimum_scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum Scale'),
    '#default_value' => variable_get('viewport_minimum_scale', ''),
    '#description' => t('Any value in the range (0, 10.0]. Usually this is set to the same value as the %initial-scale property',
      array('%initial-scale' => 'initial-scale')),
  );
  $form['viewport']['viewport_maximum_scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Scale'),
    '#default_value' => variable_get('viewport_maximum_scale', ''),
    '#description' => t('Any value in the range (0, 10.0]. Usually this is set to 1.0'),
  );
  $form['viewport']['viewport_user_scalable'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Scalable'),
    '#default_value' => variable_get('viewport_user_scalable', TRUE),
  );
  $form['viewport']['viewport_selected_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Selected pages'),
    '#default_value' => variable_get('viewport_selected_pages'),
    '#description' => t("The viewport settings will be applied to the following paths. <br/>
    Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard.
    Example paths are %blog for the blog page and %blog-wildcard for every personal blog.
    %front is the front page.", array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    )),
  );

  return system_settings_form($form);
}

/**
 * Validates the values entered for the viewport properties.
 *
 * At the time being, it only provides a basic validation, looking for the
 * existence of commas.
 */
function viewport_settings_form_validate(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    // User_scalable is a checkbox, no need to check for commas.
    if (strstr($key, 'viewport_') && $key != 'viewport_user_scalable') {
      if (strstr($value, ',')) {
        form_set_error($key, t('Commas are not allowed for the %field_name field.
        Please, ensure you are using dots (".") when entering decimal values,
        and avoid any commas after the values', array(
          '%field_name' => $form['viewport'][$key]['#title'],
        ))
        );
      }
    }
  }
}
