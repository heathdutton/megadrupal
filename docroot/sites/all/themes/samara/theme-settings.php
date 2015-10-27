<?php

/**
 * @file
 * Theme setting callbacks for the Samara theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function samara_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['samara'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
    '#default_tab' => 'defaults',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('theme', 'samara') . '/scripts/theme_settings.js',
      ),
      // See http://drupal.org/node/1057912
      // TODO Remove this when the issue is fixed.
      'css' => array(
        'vertical-tabs' => drupal_get_path('theme', 'samara') . '/styles/theme_settings.css',
      ),
    ),
  );
  
  // Default theme settings.
  $form['samara']['theme_settings'] = $form['theme_settings'];
  unset($form['theme_settings']);
  
  // Common settings.
  $form['samara']['common'] = array(
    '#title' => t('Common settings'), 
    '#type' => 'fieldset',
  );

  $form['samara']['common']['color_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Color scheme'),
    '#default_value' => theme_get_setting('color_scheme'),
    '#options' => array(
      'default' => t('default'),
      'dark'   => t('dark'),
    ),
  );
  $form['samara']['common']['base_font_size'] = array(
    '#title' => t('Base font size'),
    '#type' => 'select', 
    '#default_value' => theme_get_setting('base_font_size'),
    '#options' => array(
      '9px'  => '9px',
      '10px' => '10px',
      '11px' => '11px',
      '12px' => '12px',
      '13px' => '13px',
      '14px' => '14px',
      '15px' => '15px',
      '16px' => '16px',
      '100%' => '100%',
    ),
  );
  $form['samara']['common']['sidebar_first_weight'] = array(
    '#title' => t('First sidebar position'),
    '#type' => 'select',
    '#default_value' => theme_get_setting('sidebar_first_weight'),
    '#options' => array(
      -2 => 'Far left',
      -1 => 'Left',
       1 => 'Right',
       2 => 'Far right',
    ),
  );
  $form['samara']['common']['sidebar_second_weight'] = array(
    '#title' => t('Second sidebar position'), 
    '#type' => 'select',
    '#default_value' => theme_get_setting('sidebar_second_weight'),
    '#options' => array(
      -2 => 'Far left',
      -1 => 'Left',
       1 => 'Right',
       2 => 'Far right',
    ),
  );
  $form['samara']['common']['copyright_information'] = array(
    '#title' => t('Copyright information'),
    '#description' => t('Information about copyright holder of the website - will show up at the bottom of the page'), 
    '#type' => 'textfield',
    '#default_value' => theme_get_setting('copyright_information'),
    '#size' => 60, 
    '#maxlength' => 128, 
    '#required' => FALSE,
  );
  foreach (array(1, 2, 3) as $count) {
    $form['samara']['layout_' . $count] = array(
      '#title' => t($count . '-column layout'), 
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
    );
    $form['samara']['layout_' . $count]['layout_' . $count . '_width'] = array(
      '#title' => t('Width'), 
      '#type' => 'select',
      '#default_value' => theme_get_setting('layout_' . $count . '_width'),
      '#options' => samara_generate_width_options(30, 100, 5, '%'),
    );
    $form['samara']['layout_' . $count]['layout_' . $count . '_min_width'] = array(
      '#title' => t('Min width'), 
      '#type' => 'select',
      '#default_value' => theme_get_setting('layout_' . $count . '_min_width'),
      '#options' => samara_generate_width_options(200, 1200, 10, 'px'),
    );
    $form['samara']['layout_' . $count]['layout_' . $count . '_max_width'] = array(
      '#title' => t('Max width'), 
      '#type' => 'select',
      '#default_value' => theme_get_setting('layout_' . $count . '_max_width'),
      '#options' => samara_generate_width_options(200, 1200, 10, 'px'),
    );
  }

  return $form;
}

/**
 * Generate width options array.
 */
function samara_generate_width_options($min, $max, $increment, $postfix) {
  for ($i = $min; $i <= $max; $i += $increment) {
    $options[$i . $postfix] = $i . $postfix;
  }
  return $options;
}
