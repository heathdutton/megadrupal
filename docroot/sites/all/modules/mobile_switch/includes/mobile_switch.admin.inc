<?php
/**
 * @file
 * Administrative page callbacks for the Mobile Switch module.
 */

/**
 * Form constructor for the Basic settings form.
 *
 * @ingroup forms
 */
function mobile_switch_settings_form() {
  $form = array();

  // Current local Mobile Detect class version.
  $library['version'] = FALSE;
  $library_path = libraries_get_path(MOBILE_SWITCH_LIBRARY_NAME, $base_path = FALSE);
  $library_uri = $library_path . '/' . MOBILE_SWITCH_LIBRARY_FILE_NAME;

  if ($library_path) {
    $library['version'] = mobile_switch_mobile_detect_get_version($library_uri);
  }

  if (!$library_path || !file_exists($library_uri) || !$library['version']) {
    $form['library_missing'] = array(
      '#markup' => '<p>' . t('The Mobile Detect class could not be found as library. See README.txt for installation instructions.') . '</p>',
    );
    return $form;
  }
  else {
    $form['library_exist'] = array(
      '#markup' => '<p>' . t('Currently used Mobile Detect version: %fileversion', array('%fileversion' => $library['version'])) . '</p>',
    );
  }

  $module_path = drupal_get_path('module', 'mobile_switch');
  drupal_add_js($module_path . '/js/mobile_switch.admin.js', array('scope' => 'footer'));

  // Add operating modes.
  $active_themes = array_merge(array(
    'none' => t('Do not use'),
    'detectonly' => t('No theme switch - detect only'),
    'redirect' => t('No theme switch - redirect to website')
  ), mobile_switch_get_themes());

  // Check the imported/fetched Mobile Detect class version number.
  $version = variable_get('mobile_detect_import_version', 0);

  $form['global_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  // Operating mode.
  $form['global_settings']['mobile_switch_mobile_theme'] = array(
    '#type' => 'select',
    '#title' => t('Operating mode'),
    '#description' => t('Select the operating mode or the theme which should be used as default theme when a mobile device detected.'),
    '#options' => $active_themes,
    '#default_value' => variable_get('mobile_switch_mobile_theme', 'none')
  );
  // Redirect URL's usage message.
  $form['global_settings']['mobile_switch_redirect_url_message'] = array(
    '#markup' => '<div class="form-item form-item-mobile-switch-redirect-url-message" style="display: none;"><label for="form-item form-item-mobile-switch-redirect-url-message">' . t("Redirect URL's") . '</label><div class="description">' . t("Important: Use the same URL's that are defined in the setting.php files as <em>Base URL</em>.") . '</div></div>'
  );
  // Redirect URL to mobile.
  $form['global_settings']['mobile_switch_redirect_url_to_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL to mobile'),
    '#default_value' => variable_get('mobile_switch_redirect_url_to_mobile', ''),
    '#description' => t('The <em>Base URL</em> of the mobile website to which should be redirected. Must begin with http:// and not use a trailing slash.'),
    '#element_validate' => array('_mobile_switch_redirect_url_to_mobile_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="mobile_switch_mobile_theme"]' => array('value' => 'redirect')
      )
    )
  );
  // Redirect URL to desktop.
  $form['global_settings']['mobile_switch_redirect_url_to_desktop'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL to desktop'),
    '#default_value' => variable_get('mobile_switch_redirect_url_to_desktop', ''),
    '#description' => t('The <em>Base URL</em> of the desktop website to which should be redirected. Must begin with http:// and not use a trailing slash.'),
    '#element_validate' => array('_mobile_switch_redirect_url_to_desktop_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="mobile_switch_mobile_theme"]' => array('value' => 'redirect')
      )
    )
  );
  // Tablet usage.
  $form['global_settings']['mobile_switch_tablet_usage'] = array(
    '#type' => 'select',
    '#title' => t('Tablet device'),
    '#description' => t('Use the Mobile Switch functionality when a mobile device tablet detected.'),
    '#options' => array(
      TRUE => t('Yes'),
      FALSE => t('No')
    ),
    '#default_value' => variable_get('mobile_switch_tablet_usage', 1),
    // #states not usable here; it exists problems with mobile jQuery.
  );
  // Administration usage.
  $form['global_settings']['mobile_switch_admin_usage'] = array(
    '#type' => 'select',
    '#title' => t('Administration usage'),
    '#description' => t('Use the Mobile Switch functionality on administration pages when a mobile device detected.'),
    '#options' => array(
      FALSE => t('No'),
      TRUE => t('Yes')
    ),
    '#default_value' => variable_get('mobile_switch_admin_usage', 0),
    // #states not usable here; it exists problems with mobile jQuery.
  );

  $form['#submit'][] = 'mobile_switch_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Form constructor for the Advanced settings form.
 *
 * @ingroup forms
 */
function mobile_switch_advanced_settings_form() {
  $module_path = drupal_get_path('module', 'mobile_switch');
  drupal_add_js($module_path . '/js/mobile_switch.admin.js', array('scope' => 'footer'));

  $op_mode = mobile_switch_get_operating_mode();

  $form['preventing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preventing'),
    '#description' => t('Prevention of mobile devices. This allows to bypass mobile devices the Mobile Switch functionality.'),
    '#collapsible' => TRUE,
    '#collapsed' => variable_get('mobile_switch_prevent_devices', 0) ? FALSE : TRUE,
  );
  $form['preventing']['mobile_switch_prevent_devices'] = array(
    '#type' => 'select',
    '#title' => t('Use preventing'),
    '#options' => array(
      FALSE => t('No'),
      TRUE => t('Yes'),
    ),
    '#default_value' => variable_get('mobile_switch_prevent_devices', 0)
  );
  $description['mobile_switch_prevent_devices_strings'] = t("Configure user agent string parts. Use letters, single white spaces or underscores - no other characters! Do not use the '*' wildcard character! Enter one string per line. Example string parts: <pre>htc_flyer\niPad\nSony Tablet S\nXOOM</pre> The string detection is case insensitive.");
  $description['mobile_switch_prevent_devices_strings'] .= ' ' . t('Get a user agent string: In the !development-settings use the <em>Display debugging informations</em> option.', array('!development-settings' => l(t('Development settings'), 'admin/config/user-interface/mobile-switch/development')));
  $form['preventing']['mobile_switch_prevent_devices_strings'] = array(
    '#type' => 'textarea',
    '#title' => t('Mobile devices identification for preventing'),
    '#description' => $description['mobile_switch_prevent_devices_strings'],
    '#default_value' => variable_get('mobile_switch_prevent_devices_strings', ''),
    '#required' => FALSE,
    // #states not usable here; it exists problems with mobile jQuery.
    '#element_validate' => array('_mobile_switch_prevent_devices_strings_validate')
  );
  // Multisite settings.
  if ($op_mode === 'redirect') {
    $form['multisite'] = array(
      '#type' => 'fieldset',
      '#title' => t('Multisite settings'),
      //'#description' => 'Prevention of mobile devices. This allows to bypass mobile devices the Mobile Switch functionality.',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE
    );
    // Shared content.
    $form['multisite']['mobile_switch_redirect_shared_content'] = array(
      '#type' => 'checkbox',
      '#title' => t('Multisite shared content'),
      '#description' => t("This is relevant for the building of the redirect URL's. Unchecked: Always redirect to the configured URL's. Checked: Redirect to the URL corresponding to the current used URL."),
      '#default_value' => variable_get('mobile_switch_redirect_shared_content', 0)
    );

    // Mobile site landing path.
    $form['multisite']['mobile_switch_redirect_to_mobile_landing'] = array(
      '#type' => 'textfield',
      '#title' => 'Mobile landing',
      '#default_value' => variable_get('mobile_switch_redirect_to_mobile_landing', ''),
      '#description' => 'A path relative to the <em>Base URL</em> of the mobile website. Not use a trailing slash.',
      '#element_validate' => array('_mobile_switch_redirect_to_mobile_landing_validate'),
      '#states' => array(
        'invisible' => array(
          ':input[name="mobile_switch_redirect_shared_content"]' => array('checked' => TRUE)
        )
      )
    );
    // Desktop site landing path.
    $form['multisite']['mobile_switch_redirect_to_desktop_landing'] = array(
      '#type' => 'textfield',
      '#title' => 'Desktop landing',
      '#default_value' => variable_get('mobile_switch_redirect_to_desktop_landing', ''),
      '#description' => 'A path relative to the <em>Base URL</em> of the desktop website. Not use a trailing slash.',
      '#element_validate' => array('_mobile_switch_redirect_to_desktop_landing_validate'),
      '#states' => array(
        'invisible' => array(
          ':input[name="mobile_switch_redirect_shared_content"]' => array('checked' => TRUE)
        )
      )
    );
  }

  $form['#submit'][] = 'mobile_switch_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Form constructor for the Development settings form.
 *
 * @ingroup forms
 */
function mobile_switch_development_settings_form() {
  $module_path = drupal_get_path('module', 'mobile_switch');
  drupal_add_js($module_path . '/js/mobile_switch.admin.js', array('scope' => 'footer'));

  $form['development'] = array(
    '#type' => 'fieldset',
    '#title' => t('Development'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="development-settings">' . t('Mobile Switch can help to develop a mobile site.') . '</div>',
    '#states' => array(
      'invisible' => array(
        ':input[name="mobile_switch_mobile_theme"]' => array('value' => 'none')
      )
    )
  );
  $form['development']['mobile_switch_developer'] = array(
    '#type' => 'select',
    '#title' => t('Developer modus'),
    '#description' => t('Extends the browser detection for desktop emulators such as <em>Opera mobile</em>, <em>Fennec</em> or more other. Don\'t use it on production sites.'),
    '#options' => array(
      FALSE => t('No'),
      TRUE => t('Yes')
    ),
    '#default_value' => variable_get('mobile_switch_developer', 0)
  );
  $advanced_collapsed = FALSE;
  if (variable_get('mobile_switch_display_mobiledetectinfo', 0)) {
    $advanced_collapsed = TRUE;
  }
  $form['development']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced developer modus settings'),
    '#collapsible' => TRUE,
    '#collapsed' => $advanced_collapsed ? FALSE : TRUE
  );
  $form['development']['advanced']['mobile_switch_emulator_strings'] = array(
    '#type' => 'textarea',
    '#title' => t('Mobile emulator identification'),
    '#description' => t("Configure user agent string parts. Use letters, single white spaces or underscores - no other characters! Do not use the '*' wildcard character! Enter one string per line. As example the default string parts: <pre>Fennec\nAndroid\nTablet\nMobi</pre> The string detection is case insensitive."),
    '#default_value' => variable_get('mobile_switch_emulator_strings', "Fennec\nAndroid\nTablet\nMobi"),
    '#required' => TRUE,
    '#element_validate' => array('_mobile_switch_emulator_strings_validate')
  );
  if (variable_get('mobile_switch_mobile_theme', 'none') != 'redirect') {
    $form['development']['mobile_switch_deskbrowser'] = array(
      '#type' => 'select',
      '#title' => t('Desktop browser'),
      '#description' => t('Use the %mobile-theme with desktop browsers. Do not forget to turn off this option on production sites.', array('%mobile-theme' => t('Mobile theme'))),
      '#options' => array(
        FALSE => t('No'),
        TRUE => t('Yes')
      ),
      '#default_value' => variable_get('mobile_switch_deskbrowser', 0)
    );
  }

  $form['development']['mobile_switch_display_mobiledetectinfo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display debugging informations'),
    '#description' => t('Display of Mobile Switch informations on each page. Do not forget to turn off this option. For the display is required: the permissions %administer-site-configuration or %administer-themes.', array('%administer-site-configuration' => t('Administer site configuration'), '%administer-themes' => t('Administer themes'))),
    '#default_value' => variable_get('mobile_switch_display_mobiledetectinfo', 0)
  );

  $form['#submit'][] = 'mobile_switch_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Form submission handler for multiple mobile switch setting forms.
 *
 * @see mobile_switch_settings_form()
 * @see mobile_switch_advanced_settings_form()
 * @see mobile_switch_development_settings_form()
 */
function mobile_switch_settings_form_submit($form, &$form_state) {
  $mode = FALSE;

  if (isset($form_state['values']['mobile_switch_mobile_theme'])) {
    $mode = $form_state['values']['mobile_switch_mobile_theme'];
  }

  if (isset($mode)) {
    if ($mode === 'none') {
      variable_set('mobile_switch_developer', 0);
      variable_set('mobile_switch_deskbrowser', 0);
      variable_set('mobile_switch_display_mobiledetectinfo', 0);
    }
    // Check if enabled 'Cache pages for anonymous users'.
    if ($mode != 'none' && $mode != 'detectonly' && $mode != 'redirect' && variable_get('cache', 0) == TRUE) {
      drupal_set_message(t('Note: Cache pages for anonymous users produces problems with the current operating mode.'), 'warning');
    }
  }

  // It exist problems if used a "mobile jQuery" based theme as mobile theme.
  // This problems cannot be solved here.
  drupal_flush_all_caches();
  drupal_set_message(t('Caches cleared.'), 'status');
}

/**
 * Render API callback: Validates the user agent string parts.
 *
 * Ensures that only letters and no blank lines has been entered.
 *
 * This function is assigned as an #element_validate callback in
 * mobile_switch_settings_form().
 */
function _mobile_switch_prevent_devices_strings_validate($element, &$form_state) {
  if (preg_match("/[^a-z][^\na-z]|[a-z][^\na-z]$|.{0}\n$/i", $element['#value'])) {
    form_error($element, t('%title: Use only the described characters and no blank lines.', array('%title' => t($element['#title']))));
  }
}

/**
 * Render API callback: Validates the user agent string parts.
 *
 * Ensures that only letters and no blank lines has been entered.
 *
 * This function is assigned as an #element_validate callback in
 * mobile_switch_settings_form().
 */
function _mobile_switch_emulator_strings_validate($element, &$form_state) {
  if (preg_match("/[^a-z][^\na-z]|[a-z][^\na-z]$|.{0}\n$/i", $element['#value'])) {
    form_error($element, t('%title: Use only the described characters and no blank lines.', array('%title' => t($element['#title']))));
  }
}

/**
 * Render API callback: Validates the redirect URL to mobile.
 *
 * This function is assigned as an #element_validate callback in
 * mobile_switch_settings_form().
 */
function _mobile_switch_redirect_url_to_mobile_validate($element, &$form_state) {
  if ($form_state['values']['mobile_switch_mobile_theme'] === 'redirect' && empty($element['#value'])) {
    form_error($element, t('%title: The field must contain a value.', array('%title' => t($element['#title']))));
  }
  elseif ($form_state['values']['mobile_switch_mobile_theme'] === 'redirect' && !empty($element['#value'])) {
    $valid_url = valid_url($element['#value'], TRUE);

    if ($valid_url == FALSE) {
      form_error($element, t('%title: The field must contain a URL in a valid format.', array('%title' => t($element['#title']))));
    }
    if ($valid_url == TRUE) {
      if (preg_match('/\/$/', $element['#value'])) {
        form_error($element, t('%title: Do not use a trailing slash.', array('%title' => t($element['#title']))));
      }
    }
  }
}

/**
 * Render API callback: Validates the redirect URL to desktop.
 *
 * This function is assigned as an #element_validate callback in
 * mobile_switch_settings_form().
 */
function _mobile_switch_redirect_url_to_desktop_validate($element, &$form_state) {
  if ($form_state['values']['mobile_switch_mobile_theme'] === 'redirect' && empty($element['#value'])) {
    form_error($element, t('%title: The field must contain a value.', array('%title' => t($element['#title']))));
  }
  elseif ($form_state['values']['mobile_switch_mobile_theme'] === 'redirect' && !empty($element['#value'])) {
    $valid_url = valid_url($element['#value'], TRUE);

    if ($valid_url == FALSE) {
      form_error($element, t('%title: The field must contain a URL in a valid format.', array('%title' => t($element['#title']))));
    }
    if ($valid_url == TRUE) {
      if (preg_match('/\/$/', $element['#value'])) {
        form_error($element, t('%title: Do not use a trailing slash.', array('%title' => t($element['#title']))));
      }
    }
  }
}

/**
 * Render API callback: Validates the mobile landing path.
 */
function _mobile_switch_redirect_to_mobile_landing_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    if (!drupal_valid_path($element['#value'])) {
      form_error($element, t('%title: The field must contain a valid path.', array('%title' => t($element['#title']))));
    }
  }
}

/**
 * Render API callback: Validates the desktop landing path.
 */
function _mobile_switch_redirect_to_desktop_landing_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    if (!drupal_valid_path($element['#value'])) {
      form_error($element, t('%title: The field must contain a valid path.', array('%title' => t($element['#title']))));
    }
  }
}
