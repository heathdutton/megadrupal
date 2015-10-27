<?php

/**
 * @file
 * YUI grid theme settings file.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function yui_grid_form_system_theme_settings_alter(&$form, $form_state) {
  $form['theme_settings']['yui_breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Breadcrumbs'),
    '#default_value' => theme_get_setting('yui_breadcrumbs'),
  );
  $form['yui_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Location and width',
    '#weight' => -2,
  );
  $form['yui_settings']['yui_responsive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Responsive layout'),
    '#default_value' => theme_get_setting('yui_responsive'),
  );
  $form['yui_settings']['yui_page_width'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Page width'),
    '#field_suffix' => t('px'),
    '#description' => t('Use 0 for leave empty for fluid width.'),
    '#default_value' => theme_get_setting('yui_page_width'),
  );
  $form['yui_settings']['yui_sidebar_location'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar location'),
    '#default_value' => theme_get_setting('yui_sidebar_location'),
    '#options' => array(
      0 => t('No sidebar'),
      1 => t('Left hand side'),
      2 => t('Right hand side')),
  );
  $form['yui_settings']['yui_sidebar_width'] = array(
    '#type' => 'select',
    '#title' => t('Sidebar width'),
    '#description' => t('Fill fraction of available width.'),
    '#default_value' => theme_get_setting('yui_sidebar_width'),
    '#options' => yui_grid_unit_sizes(),
    '#states' => array(
      'invisible' => array(
        ':input[name="yui_sidebar_location"]' => array('value' => '0'),
      ),
    ),
  );
}

/**
 * Build all YUI CSS Grids unit sizes.
 *
 * We only need half of the available units as the sidebar won't be more
 * than 50% of the total width.
 *
 * See: http://yuilibrary.com/yui/docs/cssgrids/.
 */
function yui_grid_unit_sizes() {
  $options = array();
  $options['1-2'] = '1/2 (50%)';
  $options['1-3'] = '1/3 (33%)';
  $options['1-4'] = '1/4 (25%)';
  $options['1-5'] = '1/5 (20%)';
  $options['2-5'] = '2/5 (40%)';
  $options['1-6'] = '1/6 (17%)';
  $options['1-8'] = '1/8 (12%)';
  $options['3-8'] = '3/8 (38%)';
  $options['1-12'] = '1/12 (8%)';
  $options['5-12'] = '5/12 (42%)';
  $options['1-24'] = '1/24 (4%)';
  $options['5-24'] = '5/24 (21%)';
  $options['7-24'] = '7/24 (29%)';
  $options['11-24'] = '11/24 (46%)';

  return $options;
}
