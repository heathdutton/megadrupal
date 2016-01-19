<?php
/**
 * @file
 * Functions only needed on configuration page.
 */

/**
 * Implements hook_admin().
 *
 * Create admin settings for for icon Tabs.
 */
function icon_tabs_admin() {
  $form = array();

  // Icon Tabs general settings.
  $form['tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Local Tasks'),
    '#weight' => 1,
    '#collapsible' => FALSE,
  );
  $form['tabs']['icon_tabs_hide_default'] = array(
    '#type' => 'checkbox',
    '#title' => t("Hide Drupal's default tabs"),
    '#default_value' => variable_get('icon_tabs_hide_default', 1),
    '#description' => t("Hides the Default local tasks tabs provided by Drupal if Icon Tabs block is rendered."),
  );
  $form['tabs']['fred_tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fred Tabs theme'),
    '#weight' => 1,
    '#collapsible' => FALSE,
  );
  $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use 'Fred tabs' theme"),
    '#default_value' => variable_get('icon_tabs_fred_tabs_theme', 0),
    '#description' => t("Style tabs with the 'Fred tabs' theme as well as adding icons."),
  );
  $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_vertical'] = array(
    '#type' => 'checkbox',
    '#title' => t("Vertically stack tabs"),
    '#default_value' => variable_get('icon_tabs_fred_tabs_vertical', 0),
    '#description' => t("Vertically stack tabs for larger screen widths. This will not be ideal for all themes. For best results ensure the block is in a region that spans the full width of the content area or is at least always on the left side. <strong>NOTE: The <a href='/admin/structure/block/manage/icon_tabs/icon-tabs/configure'>Icon Tabs block</a> must be used for this setting.</strong>"),
    '#states' => array(
      'invisible' => array(
        ':input[name="icon_tabs_fred_tabs_theme"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_vertical_width'] = array(
    '#type' => 'textfield',
    '#title' => t("Vertical stack width"),
    '#default_value' => variable_get('icon_tabs_fred_tabs_vertical_width', 1260),
    '#description' => t("The screen pixel width at which the tabs will stack vertically. Do not include 'px'. Set to 0 always stack tabs."),
    '#states' => array(
      'invisible' => array(
        ':input[name="icon_tabs_fred_tabs_vertical"]' => array('checked' => FALSE),
      ),
    ),
  );

  // If Environment Indicator is enabled.
  if (module_exists('environment_indicator')) {
    $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_environment_indicator'] = array(
      '#type' => 'checkbox',
      '#title' => t("Environment Indicator integration"),
      '#default_value' => variable_get('icon_tabs_fred_tabs_environment_indicator', FALSE),
      '#description' => t("Style the tabs using the colors of the current environment."),
      '#states' => array(
        'invisible' => array(
          ':input[name="icon_tabs_fred_tabs_theme"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  // Icon font settings.
  $form['icon_font'] = array(
    '#type' => 'fieldset',
    '#title' => t('Icon Font.'),
    '#description' => t("You can use your own icon font instead of the default one that comes with Icon Tabs."),
    '#weight' => 2,
  );
  $form['icon_font']['icon_tabs_custom_icon_font'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use my own icon font."),
    '#default_value' => variable_get('icon_tabs_custom_icon_font', 0),
    '#description' => t("Use your own icon font that you have declared in your theme. <strong>NOTE: You will need to reset your tab icons if you change this setting.</strong>"),
  );
  $form['icon_font']['icon_tabs_css_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Icon font font-face Name"),
    '#default_value' => variable_get('icon_tabs_css_name', ''),
    '#description' => t("The font-face for your icon font <strong>as declared in your theme.</strong>"),
    '#states' => array(
      'invisible' => array(
        ':input[name="icon_tabs_custom_icon_font"]' => array('checked' => FALSE),
      ),
      'required' => array(
        ':input[name="icon_tabs_custom_icon_font"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['icon_font']['icon_tabs_svg_path'] = array(
    '#type' => 'textfield',
    '#title' => t("Icon font SVG Font"),
    '#default_value' => variable_get('icon_tabs_svg_path', ''),
    '#description' => t("To use the icon selection below, Icon Tabs needs to scan your icon font's SVG file. If your Icon fon't doesnt have an SVG file this feature will not be available. Example: <strong>sites/all/themes/bootstrap/fonts/glyphicons-halflings-regular.svg</strong>"),
    '#states' => array(
      'invisible' => array(
        ':input[name="icon_tabs_custom_icon_font"]' => array('checked' => FALSE),
      ),
      'required' => array(
        ':input[name="icon_tabs_custom_icon_font"]' => array('checked' => TRUE),
      ),
    ),
  );

  // The icon selection.
  $form['icons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Icons'),
    '#description' => t("You can automatically override the icon used for each tab with inline CSS by selecting an icon from the textfield popup below.</br><strong>NOTE: New tabs must have been generated at least once to show up on this list.</strong>"),
    '#weight' => 3,
  );

  // Get the current list of tabs that we know about.
  $tab_classes = _icon_tabs_get_tab_classes();

  // Clear input link.
  $clear_link = l(t('Clear'), '', array(
    'attributes' => array(
      'class' => array('icon-clear'),
    )));

  // Build an array or all of the tab titles.
  foreach ($tab_classes as $tab) {
    $count[] = $tab['title'];
  }
  // Count how many instances of each title there is.
  $count = array_count_values($count);

  // Go through the tabs and add a form element for each one.
  foreach ($tab_classes as $key => $tab) {
    // If tabs share a title, group them together or if its the deafult tab.
    if ($count[$tab['title']] > 1 || $key == 'default') {
      // The tab title.
      $category = strtolower($tab['title']);
      // Create groups for tabs with the same title.
      if (!isset($form['icons'][$category])) {
        $form['icons'][$category] = array(
          '#type' => 'fieldset',
          '#title' => $tab['title'],
          '#weight' => ($key == 'default') ? -1 : 1,
          '#collapsible' => ($key == 'default') ? FALSE : TRUE,
          '#collapsed' => ($key == 'default') ? FALSE : TRUE,
          '#description' => ($key == 'default') ? t('The default icon used for tabs. Unset this to not display any icon by default.') : FALSE,
        );
      }
      $form['icons'][$category][$key] = array(
        '#type' => 'textfield',
        '#title' => $tab['title'] . ' (' . $key . ')',
        '#default_value' => $tab['code'],
        '#attributes' => array('class' => array('icon-input')),
        '#prefix' => '<div class="icon-select-wrapper">',
        '#suffix' => _icon_tabs_generate_icon_dialog($tab['code']) . $clear_link . '</div>',
      );
    }
    else {
      $form['icons'][$key] = array(
        '#type' => 'textfield',
        '#title' => $tab['title'] . ' (' . $key . ')',
        '#weight' => 2,
        '#default_value' => $tab['code'],
        '#attributes' => array('class' => array('icon-input')),
        '#prefix' => '<div class="icon-select-wrapper">',
        '#suffix' => _icon_tabs_generate_icon_dialog($tab['code']) . $clear_link . '</div>',
      );
    }
  }

  // Contextual icon settings.
  $form['contextual'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contextual Links'),
    '#weight' => 4,
    '#collapsible' => FALSE,
  );
  $form['contextual']['icon_tabs_contextual_links'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use icon font for contextual links"),
    '#default_value' => variable_get('icon_tabs_contextual_links', 0),
    '#description' => t("Also use icon font for contextual links."),
  );

  // Adds the default icon font to the page if it is to be used.
  _icon_tabs_add_font();

  // Add CSS and JS to admin page to allow display of icons table.
  $form['#attached']['css'] = array(drupal_get_path('module', 'icon_tabs') . '/css/icon-tabs-admin.css');
  $form['#attached']['js'] = array(drupal_get_path('module', 'icon_tabs') . '/js/icon-tabs.js');
  $form['#attached']['library'] = array(array('system', 'ui.dialog'));

  // Custom submit and validate handlers to update icon codes.
  $form['#submit'][] = '_icon_tabs_admin_submit';
  $form['#validate'][] = '_icon_tabs_admin_validate';

  return system_settings_form($form);
}

/**
 * Custom validate handler for module admin form.
 */
function _icon_tabs_admin_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['icon_tabs_fred_tabs_vertical_width'])) {
    form_set_error('icon_tabs_fred_tabs_vertical_width', t("Verical stack width must be a number (do not include 'px')"));
  }
  if ($form_state['values']['icon_tabs_custom_icon_font'] == 1 && empty($form_state['values']['icon_tabs_css_name'])) {
    form_set_error('icon_tabs_css_name', t('Please enter the CSS name for your icon font.'));
  }
  if ($form_state['values']['icon_tabs_custom_icon_font'] == 1 && empty($form_state['values']['icon_tabs_svg_path'])) {
    form_set_error('icon_tabs_svg_path', t("Please enter enter the path to your font's svg file."));
  }
  if ($form_state['values']['icon_tabs_custom_icon_font'] && !empty($form_state['values']['icon_tabs_svg_path'])) {
    $path_info = pathinfo($form_state['values']['icon_tabs_svg_path']);
    if ($path_info['extension'] != 'svg') {
      form_set_error('icon_tabs_svg_path', t('Only files with the following extensions are allowed: svg'));
    }
    if (!file_exists($form_state['values']['icon_tabs_svg_path'])) {
      form_set_error('icon_tabs_svg_path', t('Drupal could not find this file, please ensure you have entered the path correctly.'));
    }
  }
}

/**
 * Custom submit handler for module admin form.
 */
function _icon_tabs_admin_submit($form, &$form_state) {
  // Get the current list of tabs that we know about.
  $tab_classes = _icon_tabs_get_tab_classes();

  // Loop through the tabs and update the code with the value from the form.
  foreach ($tab_classes as $key => $tab) {
    $tab_classes[$key]['code'] = $form_state['values'][$key];
  }

  // If the user changes the font they are using, unset the current icons and
  // let them know they need to update the icons.
  if (($form_state['values']['icon_tabs_custom_icon_font'] != $form['icon_font']['icon_tabs_custom_icon_font']['#default_value'])
   || ($form_state['values']['icon_tabs_svg_path'] != $form['icon_font']['icon_tabs_svg_path']['#default_value'])) {
    // Loop through the tabs and unset the icons.
    foreach ($tab_classes as $key => $tab) {
      $tab_classes[$key]['code'] = '';
    }
    // If we are going back to the default font, reset the defaults.
    if (!$form_state['values']['icon_tabs_custom_icon_font']) {
      // Get the default tabs.
      $defaults = _icon_tabs_get_tab_classes(TRUE);
      // Set the default tabs back to their original values.
      foreach ($defaults as $tab) {
        $tab_classes[$tab[0]] = array(
          'title' => $tab[1],
          'class' => array($tab[0]),
          'code' => $tab[2],
        );
      }
    }
    drupal_set_message(t('Looks like you have changed which icon font you are using! You will need to reset the icons you want your tabs to use.'), 'warning');
  }

  // If there have been any changes update the variable.
  if ($tab_classes != _icon_tabs_get_tab_classes()) {
    variable_set('icon_tabs_classes', $tab_classes);
  }

}
/**
 * Custom function to generate table of icons.
 */
function _icon_tabs_generate_icon_dialog($selected) {
  // Get the icon array.
  $icons = _icon_tabs_get_icons();

 // Used to ensure dialog is only rendered once.
  $static = &drupal_static('icon_dialog_rendered');

  $default = isset($icons[$selected]) ? $icons[$selected]['icon'] : '';

  $return = '<div class="icon-selector-default-icon">' . $default . '</div>';
  $icon_selection = '';

  // Add the dialog if it has not already been rendered.
  if (!isset($static['dialog_rendered'])) {
    foreach ($icons as $icon) {
      $icon_selection[] = array(
        '#type' => 'markup',
        '#markup' => $icon['icon'],
        '#prefix' => '<span class="icon-wrapper" data-option="' . $icon['code'] . '">',
        '#suffix' => '</span>',
      );
    }

    // Generate the dialog wrapper and filter input.
    $return .= '<div id="icon-dialog">';
    $return .= render($icon_selection) . '</div>';

    // Set this to show that the dialog has already been rendered.
    $static['dialog_rendered'] = TRUE;
  }
  // Add the dialog opener button.
  $return .= '<a class="icon-dialog-opener button">Select Icon</a>';

  return $return;
}


