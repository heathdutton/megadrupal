<?php
/**
 * @file
 * CMS theme settinggs file
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
function cms_theme_form_system_theme_settings_alter(&$form, &$form_state) {
  $path = drupal_get_path('theme', 'cms_theme');

  $form['cms_theme_information'] = array(
    '#markup' => t('Custom Theme Settings'),
    '#prefix' => '<h3 class="cms_theme-fake-title">',
    '#suffix' => '</h3>',
  );

  $form['cms_theme'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('cms_theme settings'),
  );

  // Manage of Breadcrumb.
  $form['cms_theme']['cms_theme_breadcrumb'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Breadcrumb'),
  );
  $form['cms_theme']['cms_theme_breadcrumb']['cms_theme_breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('cms_theme_breadcrumb_display'),
  );
  $form['cms_theme']['cms_theme_breadcrumb']['cms_theme_breadcrumb_label'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "You are here" in front of breadcrumb'),
    '#default_value' => theme_get_setting('cms_theme_breadcrumb_label'),
    '#description' => t('You must use Drupal core breadcrumb for this settings.'),
  );

  // Manage of Copyright.
  $form['cms_theme']['cms_theme_copyright'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Copyright'),
  );
  $form['cms_theme']['cms_theme_copyright']['cms_theme_copyright_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display copyright'),
    '#default_value' => theme_get_setting('cms_theme_copyright_display'),
  );
  $form['cms_theme']['cms_theme_copyright']['cms_theme_copyright_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => theme_get_setting('cms_theme_copyright_label'),
  );

  // Manage of Website Layout.
  $form['cms_theme']['cms_theme_layout'] = array(
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
      'attributes' => array('class' => 'cms_theme-layout-img'),
    ));
    $options[$key] = $layout . '' . $image;
  }
  $form['cms_theme']['cms_theme_layout']['cms_theme_layout_display'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#attributes' => array('class' => array('layout-grid-display')),
    '#default_value' => theme_get_setting('cms_theme_layout_display'),
  );
  $form['cms_theme']['cms_theme_layout']['cms_theme_layout_grid'] = array(
    '#markup' => t('The grid system utilizes 12 columns, making for a 940px wide container. The grid serves as an armature on which a designer can organize text and images in an easy manner.'),
    '#prefix' => '<p><strong>',
    '#suffix' => '</strong></p>',
  );
  $form['cms_theme']['cms_theme_layout']['cms_theme_layout_width_left'] = array(
    '#type' => 'textfield',
    '#title' => t('Width left sidebar (span-?)'),
    '#default_value' => theme_get_setting('cms_theme_layout_width_left'),
  );
  $form['cms_theme']['cms_theme_layout']['cms_theme_layout_width_right'] = array(
    '#type' => 'textfield',
    '#title' => t('Width right sidebar (span-?)'),
    '#default_value' => theme_get_setting('cms_theme_layout_width_right'),
  );
  $form['cms_theme']['cms_theme_layout']['cms_theme_layout_width_center'] = array(
    '#type' => 'textfield',
    '#title' => t('Width center content (span-?)'),
    '#default_value' => theme_get_setting('cms_theme_layout_width_center'),
  );

  // Manage of Form Class.
  $form['cms_theme']['cms_theme_form'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Form class'),
    '#description' => t('Add a custom class to form elements.'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_checkbox'] = array(
    '#type' => 'textfield',
    '#title' => t('Checkbox'),
    '#default_value' => theme_get_setting('cms_theme_form_checkbox'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_file'] = array(
    '#type' => 'textfield',
    '#title' => t('File'),
    '#default_value' => theme_get_setting('cms_theme_form_file'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => theme_get_setting('cms_theme_form_password'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_radio'] = array(
    '#type' => 'textfield',
    '#title' => t('Radio'),
    '#default_value' => theme_get_setting('cms_theme_form_radio'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_select_single'] = array(
    '#type' => 'textfield',
    '#title' => t('Single select'),
    '#default_value' => theme_get_setting('cms_theme_form_select_single'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_select_multiple'] = array(
    '#type' => 'textfield',
    '#title' => t('Multiple select'),
    '#default_value' => theme_get_setting('cms_theme_form_select_multiple'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#default_value' => theme_get_setting('cms_theme_form_text'),
  );
  $form['cms_theme']['cms_theme_form']['cms_theme_form_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#default_value' => theme_get_setting('cms_theme_form_date'),
  );

  // Manage of Toggle Element.
  $form['cms_theme']['cms_theme_toggle'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Toggle Element'),
    '#description' => t('Enable or disable the display of certain page elements.'),
  );
  $form['cms_theme']['cms_theme_toggle']['cms_theme_toggle_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Messages'),
    '#default_value' => theme_get_setting('cms_theme_toggle_message'),
  );
  $form['cms_theme']['cms_theme_toggle']['cms_theme_toggle_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabs'),
    '#default_value' => theme_get_setting('cms_theme_toggle_tabs'),
  );
  $form['cms_theme']['cms_theme_toggle']['cms_theme_toggle_actions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Actions links'),
    '#default_value' => theme_get_setting('cms_theme_toggle_actions'),
  );
  $form['cms_theme']['cms_theme_toggle']['cms_theme_toggle_sponsor'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sponsor'),
    '#default_value' => theme_get_setting('cms_theme_toggle_sponsor'),
  );
  $form['cms_theme']['cms_theme_toggle']['cms_theme_toggle_sponsor_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Sponsor label'),
    '#default_value' => theme_get_setting('cms_theme_toggle_sponsor_label'),
  );

  // Manage of Browser.
  $form['cms_theme']['cms_theme_browser'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Browser Client'),
    '#description' => t('Enable or disable the display of certain elements.'),
  );
  $form['cms_theme']['cms_theme_browser']['cms_theme_browser_ie6'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE6 Warning'),
    '#default_value' => theme_get_setting('cms_theme_browser_ie6'),
  );
  $form['cms_theme']['cms_theme_browser']['cms_theme_browser_nojs'] = array(
    '#type' => 'checkbox',
    '#title' => t('No JS Warning'),
    '#default_value' => theme_get_setting('cms_theme_browser_nojs'),
  );
  $form['cms_theme']['cms_theme_browser']['cms_theme_browser_nojs_label'] = array(
    '#type' => 'textfield',
    '#title' => t('No JS Warning label'),
    '#default_value' => theme_get_setting('cms_theme_browser_nojs_label'),
  );
}
