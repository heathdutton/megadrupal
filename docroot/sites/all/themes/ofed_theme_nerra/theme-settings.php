<?php

/**
 * @file
 * Needs to be documented.
 */

/**
 * Allow themes to alter the theme-specific settings form.
 *
 * With this hook, themes can alter the theme-specific settings form in any way
 * allowable by Drupal's Form API, such as adding form elements, changing
 * default values and removing form elements. See the Form API documentation on
 * api.drupal.org for detailed information.
 *
 * Note that the base theme's form alterations will be run before any sub-theme
 * alterations.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function nerra_form_system_theme_settings_alter(&$form, &$form_state) {
  $path = drupal_get_path('theme', 'nerra');

  $form['nerra_information'] = array(
    '#markup' => t('Custom Theme Settings'),
    '#prefix' => '<h3 class="nerra-fake-title">',
    '#suffix' => '</h3>',
  );

  $form['nerra'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('nerra settings'),
  );

  // Manage of Breadcrumb.
  $form['nerra']['nerra_breadcrumb'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Breadcrumb'),
  );
  $form['nerra']['nerra_breadcrumb']['nerra_breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('nerra_breadcrumb_display'),
  );
  $form['nerra']['nerra_breadcrumb']['nerra_breadcrumb_homepage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb on homepage'),
    '#default_value' => theme_get_setting('nerra_breadcrumb_homepage'),
  );
  $form['nerra']['nerra_breadcrumb']['nerra_breadcrumb_label'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "You are here" in front of breadcrumb'),
    '#default_value' => theme_get_setting('nerra_breadcrumb_label'),
    '#description' => t('You must use Drupal core breadcrumb for this settings.'),
  );

  // Manage of Copyright.
  $form['nerra']['nerra_copyright'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Copyright'),
  );
  $form['nerra']['nerra_copyright']['nerra_copyright_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display copyright'),
    '#default_value' => theme_get_setting('nerra_copyright_display'),
  );
  $form['nerra']['nerra_copyright']['nerra_copyright_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => theme_get_setting('nerra_copyright_label'),
  );

  // Manage of Website Layout.
  $form['nerra']['nerra_layout'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Grid Display'),
  );
  $layouts = array(
    'no_sidebar' => 'No sidebars',
    'left_sidebar' => 'Left sidebar',
    'right_sidebar' => 'Right sidebar',
    'two_sidebar' => 'Sidebars left and right',
  );
  $options = array();
  foreach ($layouts as $key => $layout) {
    $image = theme('image', array(
      'path' => $path . '/assets/images/layouts/' . $key . '.png',
      'attributes' => array('class' => 'nerra-layout-img'),
    ));
    $options[$key] = $layout . '' . $image;
  }
  $form['nerra']['nerra_layout']['nerra_layout_display'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#attributes' => array('class' => array('layout-grid-display')),
    '#default_value' => theme_get_setting('nerra_layout_display'),
  );
  $form['nerra']['nerra_layout']['nerra_layout_grid'] = array(
    '#markup' => t('The grid system utilizes 12 columns, making for a 940px wide container. The grid serves as an armature on which a designer can organize text and images in an easy manner.'),
    '#prefix' => '<p><strong>',
    '#suffix' => '</strong></p>',
  );
  $form['nerra']['nerra_layout']['nerra_layout_width_left'] = array(
    '#type' => 'textfield',
    '#title' => t('Width left sidebar (span-?)'),
    '#default_value' => theme_get_setting('nerra_layout_width_left'),
  );
  $form['nerra']['nerra_layout']['nerra_layout_width_right'] = array(
    '#type' => 'textfield',
    '#title' => t('Width right sidebar (span-?)'),
    '#default_value' => theme_get_setting('nerra_layout_width_right'),
  );
  $form['nerra']['nerra_layout']['nerra_layout_width_center'] = array(
    '#type' => 'textfield',
    '#title' => t('Width center content (span-?)'),
    '#default_value' => theme_get_setting('nerra_layout_width_center'),
  );

  // Manage of Form Class.
  $form['nerra']['nerra_form'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Form class'),
    '#description' => t('Add a custom class to form elements.'),
  );
  $form['nerra']['nerra_form']['nerra_form_checkbox'] = array(
    '#type' => 'textfield',
    '#title' => t('Checkbox'),
    '#default_value' => theme_get_setting('nerra_form_checkbox'),
  );
  $form['nerra']['nerra_form']['nerra_form_file'] = array(
    '#type' => 'textfield',
    '#title' => t('File'),
    '#default_value' => theme_get_setting('nerra_form_file'),
  );
  $form['nerra']['nerra_form']['nerra_form_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => theme_get_setting('nerra_form_password'),
  );
  $form['nerra']['nerra_form']['nerra_form_radio'] = array(
    '#type' => 'textfield',
    '#title' => t('Radio'),
    '#default_value' => theme_get_setting('nerra_form_radio'),
  );
  $form['nerra']['nerra_form']['nerra_form_select_single'] = array(
    '#type' => 'textfield',
    '#title' => t('Single select'),
    '#default_value' => theme_get_setting('nerra_form_select_single'),
  );
  $form['nerra']['nerra_form']['nerra_form_select_multiple'] = array(
    '#type' => 'textfield',
    '#title' => t('Multiple select'),
    '#default_value' => theme_get_setting('nerra_form_select_multiple'),
  );
  $form['nerra']['nerra_form']['nerra_form_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#default_value' => theme_get_setting('nerra_form_text'),
  );
  $form['nerra']['nerra_form']['nerra_form_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#default_value' => theme_get_setting('nerra_form_date'),
  );

  // Manage of Toggle Element.
  $form['nerra']['nerra_toggle'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Toggle Element'),
    '#description' => t('Enable or disable the display of certain page elements.'),
  );
  $form['nerra']['nerra_toggle']['nerra_toggle_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Messages'),
    '#default_value' => theme_get_setting('nerra_toggle_message'),
  );
  $form['nerra']['nerra_toggle']['nerra_toggle_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabs'),
    '#default_value' => theme_get_setting('nerra_toggle_tabs'),
  );
  $form['nerra']['nerra_toggle']['nerra_toggle_actions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Actions links'),
    '#default_value' => theme_get_setting('nerra_toggle_actions'),
  );
  $form['nerra']['nerra_toggle']['nerra_toggle_sponsor'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sponsor'),
    '#default_value' => theme_get_setting('nerra_toggle_sponsor'),
  );
  $form['nerra']['nerra_toggle']['nerra_sponsor_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Sponsor label'),
    '#default_value' => theme_get_setting('nerra_sponsor_label'),
  );

  // Manage of Browser.
  $form['nerra']['nerra_browser'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Browser Client'),
    '#description' => t('Enable or disable the display of certain elements.'),
  );
  $form['nerra']['nerra_browser']['nerra_browser_ie6'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE6 Warning'),
    '#default_value' => theme_get_setting('nerra_browser_ie6'),
  );
  $form['nerra']['nerra_browser']['nerra_browser_nojs'] = array(
    '#type' => 'checkbox',
    '#title' => t('No JS Warning'),
    '#default_value' => theme_get_setting('nerra_browser_nojs'),
  );
  $form['nerra']['nerra_browser']['nerra_browser_nojs_label'] = array(
    '#type' => 'textfield',
    '#title' => t('No JS Warning label'),
    '#default_value' => theme_get_setting('nerra_browser_nojs_label'),
  );
}
