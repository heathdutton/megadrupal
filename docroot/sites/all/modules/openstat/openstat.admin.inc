<?php
/**
 * @file
 * Code for admin page for Openstat module.
 */

/**
 * General configuration page for LiveInternet module.
 */
function openstat_admin_settings_form($form, &$form_state) {

  global $user;
  $arg = array(
    '!Openstat' => l(t('Openstat'), 'https://www.openstat.ru'),
    '%email' => $user->mail,
  );

  $form['openstat_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#description' => t('The ID is unique to each site. If you do not have account or have not registered this site on !Openstat press the button below. It will register the site for %email email.', $arg),
  );

  $form['openstat_general']['openstat_button'] = array(
    '#type' => 'button',
    '#value' => t('Get Openstat counter ID'),
    '#ajax' => array(
      'callback' => 'openstat_get_counter_id_ajax_callback',
      'wrapper' => 'openstat-wrapper',
    ),
  );

  // If Ajax is done.
  if (isset($form_state['values']['openstat_id'])) {
    // Overwrite Openstat ID with new one.
    $form_state['input']['openstat_id'] = check_plain(openstat_get_countet_id());
  }

  $form['openstat_general']['openstat_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Counter ID'),
    '#size' => 7,
    '#required' => TRUE,
    '#description' => t('If you already have registered your site you can put the ID here.'),
    '#default_value' => variable_get('openstat_id', NULL),
    '#element_validate' => array('openstat_id_validate'),
    '#prefix' => '<div id="openstat-wrapper">',
    '#suffix' => '</div>',
  );

  $form['openstat_addition'] = array(
    '#type' => 'fieldset',
    '#title' => t('Counter settings'),
  );

  $form['openstat_addition']['openstat_type'] = array(
    '#type' => 'radios',
    '#title' => t('Counter type'),
    '#options' => array(
      t('Invisible counter'),
      t('Counter with image'),
    ),
    '#description' => t('Select the counter type. You can control the visibility of counter through Openstat block configuration.'),
    '#default_value' => variable_get('openstat_type', 0),
  );

  $form['openstat_addition']['openstat_image_type'] = array(
    '#type' => 'radios',
    '#title' => t('Image type'),
    '#options' => array(
      87 => t('Big image with Openstat logo'),
      5081 => t('Big image with tracking data'),
      5085 => t('Small image with Openstat logo and tracking data'),
      91 => t('Small image with people logo and with border'),
      93 => t('Small image with people logo and without border'),
    ),
    '#description' => t('Select the counter image type.'),
    '#default_value' => variable_get('openstat_image_type', 87),
    '#states' => array(
      'visible' => array(
        ':input[name="openstat_type"]' => array(
          'value' => 1,
        ),
      ),
    ),
  );

  $form['openstat_addition']['openstat_color'] = array(
    '#type' => 'select',
    '#title' => t('Counter color'),
    '#options' => array(
      'c3c3c3' => t('Silver'),
      '828282' => t('Gray'),
      '000000' => t('Black'),
      '3400cd' => t('Navy'),
      '458efc' => t('Blu'),
      '258559' => t('Green'),
      '00d43c' => t('Lime'),
      'c0f890' => t('Chartreuse'),
      'fdd127' => t('Gold'),
      'ff9822' => t('Orange'),
      'ff5f1e' => t('Orange-red'),
      'ff001c' => t('Red'),
      '9c0037' => t('Maroon'),
      '8f46b9' => t('Blue violet'),
      'c044b6' => t('Purple'),
      'ff86fb' => t('Magenta'),
      'custom' => t('Custom'),
    ),
    '#description' => t('Select the main counter color.'),
    '#default_value' => variable_get('openstat_color', 'ff9822'),
    '#states' => array(
      'visible' => array(
        ':input[name="openstat_type"]' => array(
          'value' => 1,
        ),
      ),
    ),
  );

  $form['openstat_addition']['openstat_custom_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom color'),
    '#size' => 6,
    '#description' => t('Enter color in hex format.'),
    '#field_prefix' => '#',
    '#default_value' => variable_get('openstat_custom_color', NULL),
    '#element_validate' => array('openstat_custom_color_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="openstat_color"]' => array(
          'value' => 'custom',
        ),
      ),
    ),
  );

  $form['openstat_addition']['openstat_gradient'] = array(
    '#type' => 'checkbox',
    '#title' => t('Gradient'),
    '#description' => t('Select the gradient for counter image.'),
    '#default_value' => variable_get('openstat_gradient', 0),
    '#states' => array(
      'visible' => array(
        ':input[name="openstat_type"]' => array(
          'value' => 1,
        ),
      ),
    ),
  );

  $form['openstat_addition']['openstat_font_color'] = array(
    '#type' => 'radios',
    '#title' => t('Font color'),
    '#options' => array(
      t('Dark'),
      t('Light'),
    ),
    '#description' => t('Select font color.'),
    '#default_value' => variable_get('openstat_font_color', 0),
    '#states' => array(
      'visible' => array(
        ':input[name="openstat_type"]' => array(
          'value' => 1,
        ),
      ),
    ),
  );

  $form['openstat_addition']['openstat_track_links'] = array(
    '#type' => 'radios',
    '#title' => t('Track click on links'),
    '#options' => array(
      '0' => t('Not'),
      'ext' => t('Only on external links'),
      'all' => t('On external and internal link'),
    ),
    '#description' => t('Select type of tracking clicks on links.'),
    '#default_value' => variable_get('openstat_track_links', 0),
    '#states' => array(
      'visible' => array(
        ':input[name="openstat_type"]' => array(
          'value' => 1,
        ),
      ),
    ),
  );

  $form['openstat_page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking specific pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['openstat_page_vis_settings']['openstat_visibility_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking to specific pages'),
    '#options' => array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    ),
    '#default_value' => variable_get('openstat_visibility_pages', 0),
  );
  $form['openstat_page_vis_settings']['openstat_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('openstat_pages', "admin\nadmin/*\nbatch\nnode/add*\nnode/*/*\nuser/*/*"),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
      array(
        '%blog' => 'blog',
        '%blog-wildcard' => 'blog/*',
        '%front' => '<front>',
      )
    ),
    '#rows' => 10,
  );

  $form['openstat_role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking specific roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['openstat_role_vis_settings']['openstat_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('openstat_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['openstat_role_vis_settings']['openstat_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('openstat_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  return system_settings_form($form);
}

/**
 * Openstat ID validation.
 */
function openstat_id_validate($element, &$form_state, $form) {
  if (isset($element['#value'])) {
    if (!preg_match('/^\d{7}$/', $element['#value'])) {
      form_error($element, t('The ID must contain 7 numbers.'));
    }
  }
}

/**
 * Openstat custom color validation.
 */
function openstat_custom_color_validate($element, &$form_state, $form) {
  if (($form_state['values']['openstat_color'] == 'custom') && (isset($element['#value']))) {
    $subject = strtolower($element['#value']);
    if (!preg_match('/^[a-f0-9]{6}$/i', $subject)) {
      form_error($element, t('Custom color value must be in hex format.'));
    }
  }
}

/**
 * Return Openstat ID form element.
 *
 * @return array
 *   Renderable array (the textfields element)
 */
function openstat_get_counter_id_ajax_callback($form, $form_state) {
  return $form['openstat_general']['openstat_id'];
}
