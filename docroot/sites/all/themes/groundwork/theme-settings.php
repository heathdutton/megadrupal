 <?php

/**
 * @file
 * Implements hook_form_system_theme_settings_alter() function.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */

function groundwork_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL) {

  // Double invoke bug in core: http://drupal.org/node/943212
  if (isset($form_id)) {
    return;
  }

  // Layout settings
  $form['groundwork'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
  );

  // General Settings
  $form['groundwork']['general_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('General settings'),
  );
  // Hide Site Name Branding
  $form['groundwork']['general_settings']['noceda_hide_sitename_branding'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Hide site name branding'),
    '#description'   => t('Useful if the site logo is used to display the site name. For accessibility, the site name is visually hidden but visible to screen readers.'),
    '#default_value' => theme_get_setting('noceda_hide_sitename_branding'),
  );
  // Hide Book Navigation
  $form['groundwork']['general_settings']['noceda_hide_book_navigation'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Hide book navigation'),
    '#description'   => t('Useful if a block is used to display the book navigation. Eg., on the Node Aside region.'),
    '#default_value' => theme_get_setting('noceda_hide_book_navigation'),
  );
  // Hide Theme Attribution
  $form['groundwork']['general_settings']['noceda_hide_attribution'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Hide theme attribution'),
    '#description'   => t('Useful if the theme is highly customized.'),
    '#default_value' => theme_get_setting('noceda_hide_attribution'),
  );

  // Theme Development
  $form['groundwork']['themedev'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Theme development settings'),
  );
  $form['groundwork']['themedev']['noceda_basic_functionality'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use basic functionality only. (<em>Not recommended for ultimate customization</em>)'),
    '#default_value' => theme_get_setting('noceda_basic_functionality'),
    '#description'   => t('Disables LESS preprocessor functionality thus deactivating  easy grid configuration, progressive enhancement, media query settings, mixins, bundles,  and more. Uses good ol\' CSS files where styles are set to Groundwork\'s default.'),
  );
  $form['groundwork']['themedev']['noceda_dev_block_id'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display the id of blocks for easy semantic styling'),
    '#default_value' => theme_get_setting('noceda_dev_block_id'),
  );

  $form['theme_settings']['#collapsible'] = TRUE;
  $form['theme_settings']['#collapsed'] = TRUE;
  $form['logo']['#collapsible'] = TRUE;
  $form['logo']['#collapsed'] = TRUE;
  $form['favicon']['#collapsible'] = TRUE;
  $form['favicon']['#collapsed'] = TRUE;
}
