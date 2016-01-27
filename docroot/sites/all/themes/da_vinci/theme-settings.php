<?php

/**
 * @file
 * Theme setting callbacks for the da vinci theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param object $form
 *   The form.
 * @param object $form_state
 *   The form state.
 */
function da_vinci_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['da_vinci_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Da Vinci Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['da_vinci_settings']['debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Grid Debug Settings'),
    '#default_value' => theme_get_setting('debug', 'da_vinci'),
    '#description'   => t("Check this option to show Grid Debug Button in page. Uncheck to hide. This will only be displayed if admin is logged."),
  );

  $form['da_vinci_settings']['styleguide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Style Guide Icon'),
    '#default_value' => theme_get_setting('styleguide', 'da_vinci'),
    '#description'   => t("Check this option to show Style Guide Button in page. Uncheck to hide. Require jquery_update and styleguide module."),
  );

  $form['da_vinci_settings']['masonry'] = array(
    '#type' => 'textfield',
    '#title' => t('Activate masonry view'),
    '#default_value' => theme_get_setting('masonry', 'da_vinci'),
    '#description'   => t("If you want to add the masonry style in views, you only have to insert the class name here. More info in masonry-readme.txt"),
  );

  $form['da_vinci_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs', 'da_vinci'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );

  $form['da_vinci_settings']['css_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude CSS files'),
    '#description' => t('Enter one file per line.'),
    // The paths are stored in an array.
    '#default_value' => implode("\n", (array) theme_get_setting('css_exclude', 'da_vinci')),
  );

  $form['da_vinci_settings']['js_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude JS files'),
    '#description' => t('Enter one file per line.'),
    // The paths are stored in an array.
    '#default_value' => implode("\n", (array) theme_get_setting('js_exclude', 'da_vinci')),
  );

  // Add our custom submit function:
  $form['#submit'][] = 'da_vinci_form_system_theme_settings_submit';

  if (!(module_exists('styleguide') && module_exists('jquery_update'))) {
    $form['da_vinci_settings']['styleguide']['#value'] = 0;
    $form['da_vinci_settings']['styleguide']['#disabled'] = TRUE;
  }
}

/**
 * Form submit for da_vinci_form_system_theme_settings_alter().
 */
function da_vinci_form_system_theme_settings_submit($form, &$form_state) {
  $exclude_css = array_filter(array_map('trim', explode("\n", $form_state['values']['css_exclude'])));
  $exclude_js = array_filter(array_map('trim', explode("\n", $form_state['values']['js_exclude'])));
  form_set_value(array('#parents' => array('css_exclude')), $exclude_css, $form_state);
  form_set_value(array('#parents' => array('js_exclude')), $exclude_js, $form_state);
}
