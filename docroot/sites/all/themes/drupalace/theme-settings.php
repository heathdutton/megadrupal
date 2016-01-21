<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function drupalace_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['drupalace'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -50,
  );

  // Layout settings
  $form['drupalace']['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
  );

  $form['drupalace']['layout']['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page layout'),
    '#description' => t('Page layout'),
  );

  $form['drupalace']['layout']['page']['drupalace_page_unit'] = array(
    '#type' => 'select',
    '#title' => t('Page unit'),
    '#default_value' => theme_get_setting('drupalace_page_unit'),
    '#options' => drupal_map_assoc(array('%', 'px')),
  );

  $form['drupalace']['layout']['page']['drupalace_page_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Page width'),
    '#default_value' => theme_get_setting('drupalace_page_width'),
    '#field_suffix' => theme_get_setting('drupalace_page_unit'),
    '#size' => 5,
  );

  $form['drupalace']['layout']['page']['drupalace_page_max_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Page max width'),
    '#default_value' => theme_get_setting('drupalace_page_max_width'),
    '#field_suffix' => 'px',
    '#size' => 5,
  );

  $form['drupalace']['layout']['page']['drupalace_page_min_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Page min width'),
    '#default_value' => theme_get_setting('drupalace_page_min_width'),
    '#field_suffix' => 'px',
    '#size' => 5,
  );

  $form['drupalace']['layout']['sidebar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sidebar layout'),
    '#description' => t('Sidebar layout'),
  );

  $form['drupalace']['layout']['sidebar']['drupalace_sidebar_position'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar position'),
    '#options' => array(
      'left' => t('Left'),
      'right' => t('Right'),
    ),
    '#default_value' => theme_get_setting('drupalace_sidebar_position'),
  );

  $form['drupalace']['layout']['sidebar']['drupalace_sidebar_unit'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar unit'),
    '#default_value' => theme_get_setting('drupalace_sidebar_unit'),
    '#options' => drupal_map_assoc(array('%', 'px')),
  );

  $form['drupalace']['layout']['sidebar']['drupalace_sidebar_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Sidebar width'),
    '#default_value' => theme_get_setting('drupalace_sidebar_width'),
    '#field_suffix' => theme_get_setting('drupalace_sidebar_unit'),
    '#size' => 5,
  );

  // Social features
  $form['drupalace']['social'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social features'),
  );

  $form['drupalace']['social']['drupalace_social_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable social bookmarks for the next node types'),
    '#options' => node_type_get_names(),
    '#default_value' => (array) theme_get_setting('drupalace_social_node_types'),
  );

  $form['drupalace']['social']['drupalace_social_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Helper text for social block'),
    '#default_value' => theme_get_setting('drupalace_social_text'),
  );

  $form['drupalace']['social']['drupalace_social_button_size'] = array(
    '#type' => 'select',
    '#title' => t('Social buttons size'),
    '#options' => array(
      '16' => '16px',
      '32' => '32px',
    ),
    '#default_value' => theme_get_setting('drupalace_social_button_size'),
  );

  $form['drupalace']['social']['drupalace_social_services'] = array(
    '#type' => 'textarea',
    '#title' => t('Social services'),
    '#description' => t('Enter comma-separated list of social servises. See correct service names on !addthis', array('!addthis' => 'http://addthis.com')),
    '#default_value' => theme_get_setting('drupalace_social_services'),
  );

  // Node settings
  $form['drupalace']['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node settings'),
  );

  $form['drupalace']['node']['drupalace_node_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Disable node type before node title for the next node types'),
    '#options' => node_type_get_names(),
    '#default_value' => (array) theme_get_setting('drupalace_node_node_types'),
  );

  $form['drupalace']['node']['drupalace_node_navigation'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable node navigation for the next node types'),
    '#options' => node_type_get_names(),
    '#default_value' => (array) theme_get_setting('drupalace_node_navigation'),
  );

  // Login settings
  $form['drupalace']['login'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login settings'),
  );

  $form['drupalace']['login']['drupalace_popup_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable popup login'),
    '#default_value' => theme_get_setting('drupalace_popup_login'),
  );

  $form['drupalace']['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search block settings'),
  );

  $form['drupalace']['search']['drupalace_search_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Default search text'),
    '#default_value' => theme_get_setting('drupalace_search_text'),
  );

  $form['drupalace']['search']['drupalace_search_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Search button value'),
    '#default_value' => theme_get_setting('drupalace_search_button'),
  );

  $form['drupalace']['search']['drupalace_search_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Search block width'),
    '#default_value' => theme_get_setting('drupalace_search_width'),
    '#field_suffix' => 'px',
    '#size' => 4,
  );

  $form['#attached']['css'][] = drupal_get_path('theme', 'drupalace') . '/css/theme-settings.css';

  $form['#validate'][] = 'drupalace_theme_settings_validate';
  $form['#submit'][]   = 'drupalace_theme_settings_submit';
}

/**
 * Validate function for theme setting
 */
function drupalace_theme_settings_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!ctype_digit($values['drupalace_page_max_width'])) {
    form_set_error('drupalace_page_max_width', t('Enter only digits in page max width field.'));
  }

  if (!ctype_digit($values['drupalace_page_min_width'])) {
    form_set_error('drupalace_page_min_width', t('Enter only digits in page min width field.'));
  }

  if (!ctype_digit($values['drupalace_page_width'])) {
    form_set_error('drupalace_page_width', t('Enter only digits in page width field.'));
  }

  if (!ctype_digit($values['drupalace_search_width'])) {
    form_set_error('drupalace_search_width', t('Enter only digits in search width field.'));
  }

  if ($values['drupalace_page_max_width'] < 1) {
    form_set_error('drupalace_page_max_width', t("Page max width value can't be less than 1"));
  }

  if ($values['drupalace_page_min_width'] < 1) {
    form_set_error('drupalace_page_min_width', t("Page min width value can't be less than 1"));
  }

  if ($values['drupalace_page_width'] < 1) {
    form_set_error('drupalace_page_width', t("Page width value can't be less than 1"));
  }

  if ($values['drupalace_search_width'] < 1) {
    form_set_error('drupalace_search_width', t("Search block width value can't be less than 1"));
  }

  if ($values['drupalace_page_unit'] == '%' && $values['drupalace_page_width'] > 100) {
    form_set_error('drupalace_page_width', t("Page width value can't be more than 100"));
  }

  if (!ctype_digit($values['drupalace_sidebar_width'])) {
    form_set_error('drupalace_sidebar_width', t('Enter only digits in sidebar width field.'));
  }

  if ($values['drupalace_sidebar_width'] < 1) {
    form_set_error('drupalace_sidebar_width', t("Sidebar width value can't be less than 1"));
  }

  if ($values['drupalace_sidebar_unit'] == '%' && $values['drupalace_sidebar_width'] > 100) {
    form_set_error('drupalace_sidebar_width', t("Sidebar width value can't be more than 100"));
  }
}

/**
 * Submit function for theme setting
 */
function drupalace_theme_settings_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Get page settings
  $page_width       = $values['drupalace_page_width'];
  $page_unit        = $values['drupalace_page_unit'];
  $page_max_width   = $values['drupalace_page_max_width'];
  $page_min_width   = $values['drupalace_page_min_width'];

  // Get sidebar settings
  $sidebar_width    = $values['drupalace_sidebar_width'];
  $sidebar_unit     = $values['drupalace_sidebar_unit'];
  $sidebar_position = $values['drupalace_sidebar_position'];
  
  $opposite = 'left';
  if ($sidebar_position == 'left') {
    $opposite = 'right';
  }

  // Get search style settings
  $search_width     = $values['drupalace_search_width'];

  // Build css
  $styles[] = '#container, #footer-menu-inner, #footer-inner, #header-inner { width: ' . $page_width . $page_unit . '; max-width: ' . $page_max_width . 'px; min-width: ' . $page_min_width . 'px; }';
  $styles[] = '#sidebar { width: ' . $sidebar_width . $sidebar_unit . '; float: ' . $sidebar_position . '; }';
  $styles[] = '#main-inner.with-sidebar { margin-' . $sidebar_position . ': ' . $sidebar_width . $sidebar_unit . '; margin-' . $opposite . ':0; padding-' . $sidebar_position . ': 25px; padding-' . $opposite . ': 0; }';
  $styles[] = '#header .region-header form .form-item .form-text { width: ' . $search_width . 'px; }';

  // Save css file
  $file  = 'drupalace.layout.css';
  $path  = "public://drupalace";
  $data  = implode("\n", $styles);

  file_prepare_directory($path, FILE_CREATE_DIRECTORY);
  $filepath = $path . '/' . $file;
  file_save_data($data, $filepath, FILE_EXISTS_REPLACE);
}
