<?php
/**
 * @file
 * You can configure how many columns the panel region should be.
 * Choose one of available grid systems: 960.gs, Foundation or Bootstrap.
 */

// Plugin definition.
$plugin = array(
  'title' => '--> ' . t('Circle: Grid system columns'),
  'description' => t('Span X columns'),
  'render region' => 'circle_column_style_render_region',
  'render pane' => 'circle_column_style_render_pane',
  'settings form' => 'circle_column_style_region_settings_form',
  'pane settings form' => 'circle_column_style_pane_settings_form',
  'hook theme' => array(
    'circle_column_pane_style' => array(
      'render element' => 'element',
      'template' => 'circle-column-pane-style',
      'path' => drupal_get_path('theme', 'circle') . '/plugins/styles',
      'variables' => array(
        'vars' => NULL,
      ),
    ),
    'circle_column_region_style' => array(
      'render element' => 'element',
      'template' => 'circle-column-region-style',
      'path' => drupal_get_path('theme', 'circle') . '/plugins/styles',
      'variables' => array(
        'vars' => NULL,
      ),
    ),
  ),
);


/**
 * Render callback for region.
 * @ingroup themeable
 */
function theme_circle_column_style_render_region($vars) {
  $panes = $vars['panes'];
  $settings = $vars['settings'];
  $output = array();

  // Rendering region panes.
  $items = array();
  foreach ($panes as $item) {
    $items[] = $item;
  }
  $rendered_items = implode($items);
  $grid_class = _circle_columns_get_grid_class($settings);
  $grid_class[] = 'circle-columns-column';

  if ($rendered_items) {
    $output = array(
      'items' => $items,
      'rendered_items' => $rendered_items,
      'wrapper_classes' => $grid_class,
      'wrapper_class' => implode(' ', $grid_class),
      'has_row' => $settings['grid_system'] != 'ninesixty',
    );
    return theme('circle_column_region_style', $output);
  }
  else {
    return $rendered_items;
  }
}

/**
 * Render callback for pane.
 */
function theme_circle_column_style_render_pane($vars) {
  $settings = $vars['settings'];
  $grid_class = _circle_columns_get_grid_class($settings);
  $grid_class[] = 'grid-system-column-inner';
  $output = array(
    'vars' => $vars,
    'pane' => theme('panels_pane', $vars),
    'wrapper_classes' => $grid_class,
    'wrapper_class' => implode(' ', $grid_class),
  );
  return theme('circle_column_pane_style', $output);
}

/**
 * Panel region settings form callback.
 */
function circle_column_style_region_settings_form($style_settings) {
  // Get the form with current settings.
  $form = _circle_columns_return_settings_form($style_settings);

  return $form;
}

/**
 * Panel pane settings form callback.
 */
function circle_column_style_pane_settings_form($style_settings) {
  // Get the form with current settings.
  $form = _circle_columns_return_settings_form($style_settings);

  return $form;
}

/**
 * Helper function. Returns settings form for region and pane.
 */
function _circle_columns_return_settings_form($style_settings) {
  $form = array();
  // Human readable grid size and offset.
  $columns_array = array('No Columns', '1 Column');
  for ($i = 2; $i <= 12; $i++) {
    $columns_array[$i] = $i . ' Columns';
  }
  $columns_push = array('No Offset', '1 Column');
  for ($i = 2; $i <= 12; $i++) {
    $columns_push[$i] = $i . ' Columns';
  }

  $form['#attached']['css'] = array(
    drupal_get_path('theme', 'circle') . '/plugins/styles/columns-form.css',
  );

  $default_theme = variable_get('theme_default');
  if (theme_get_setting('circle_css_framework', $default_theme)) {
    $default_framework = theme_get_setting('circle_css_framework', $default_theme);
  }
  else {
    $default_framework = 'bootstrap';
  }

  // Available Grid systems.
  $form['grid_system'] = array(
    '#type' => 'select',
    '#title' => t('Select Grid system'),
    '#options' => array(
      'ninesixty' => t('960 Grid System'),
      'bootstrap' => t('Bootstrap 3'),
      'foundation' => t('Foundation'),
    ),
    '#default_value' => (isset($style_settings['grid_system'])) ? $style_settings['grid_system'] : $default_framework,
  );

  // Settings for large screen.
  $form['large_grid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Large Screen'),
    '#attributes' => array('class' => array('grid-region-form')),
  );
  $form['large_grid']['grid_large_qty'] = array(
    '#type' => 'select',
    '#title' => t('Grid Size'),
    '#options' => $columns_array,
    '#default_value' => (isset($style_settings['large_grid']['grid_large_qty'])) ? $style_settings['large_grid']['grid_large_qty'] : '0',
  );
  $form['large_grid']['grid_large_offset'] = array(
    '#type' => 'select',
    '#title' => t('Grid Offset'),
    '#options' => $columns_push,
    '#default_value' => (isset($style_settings['large_grid']['grid_large_offset'])) ? $style_settings['large_grid']['grid_large_offset'] : '0',
  );

  // Settings for medium screen.
  $form['medium_grid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Medium Screen'),
    '#attributes' => array('class' => array('grid-region-form')),
    // Hide for 960 grid system.
    '#states' => array(
      'visible' => array(
        ':input[name="settings[grid_system]"]' => array('!value' => 'ninesixty'),
      ),
    ),
  );
  $form['medium_grid']['grid_medium_qty'] = array(
    '#type' => 'select',
    '#title' => t('Grid Size'),
    '#options' => $columns_array,
    '#default_value' => (isset($style_settings['medium_grid']['grid_medium_qty'])) ? $style_settings['medium_grid']['grid_medium_qty'] : '0',
  );
  $form['medium_grid']['grid_medium_offset'] = array(
    '#type' => 'select',
    '#title' => t('Grid Offset'),
    '#options' => $columns_push,
    '#default_value' => (isset($style_settings['medium_grid']['grid_medium_offset'])) ? $style_settings['medium_grid']['grid_medium_offset'] : '0',
  );

  // Settings for small screen.
  $form['small_grid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Small Screen'),
    '#attributes' => array('class' => array('grid-region-form')),
    // Hide for 960 grid system.
    '#states' => array(
      'visible' => array(
        ':input[name="settings[grid_system]"]' => array('!value' => 'ninesixty'),
      ),
    ),
  );
  $form['small_grid']['grid_small_qty'] = array(
    '#type' => 'select',
    '#title' => t('Grid Size'),
    '#options' => $columns_array,
    '#default_value' => (isset($style_settings['small_grid']['grid_small_qty'])) ? $style_settings['small_grid']['grid_small_qty'] : '0',
  );
  $form['small_grid']['grid_small_offset'] = array(
    '#type' => 'select',
    '#title' => t('Grid Offset'),
    '#options' => $columns_push,
    '#default_value' => (isset($style_settings['small_grid']['grid_small_offset'])) ? $style_settings['small_grid']['grid_small_offset'] : '0',
  );

  $form['additional_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional css class'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => (isset($style_settings['additional_class'])) ? $style_settings['additional_class'] : '',
  );

  return $form;
}

/**
 * Helper function. Returns grid class.
 */
function _circle_columns_get_grid_class($settings) {
  if (!isset($settings['grid_system'])) {
    return;
  }
  $grid_class = array();
  $grid = $settings['grid_system'];
  $gs_classes = array();

  switch ($grid) {
    // 960.gs classes.
    case 'ninesixty':
      $gs_classes = array(
        'offset' => 'prefix_',
        'large' => 'grid_',
      );
      break;

    // Bootstrap classes.
    case 'bootstrap':
      $gs_classes = array(
        'offset' => 'offset-',
        'suffix' => '',
        'small' => 'col-xs-',
        'medium' => 'col-sm-',
        'large' => 'col-md-',
      );
      break;

    // Foundation classes.
    case 'foundation':
      $gs_classes = array(
        'offset' => 'offset-',
        'suffix' => 'columns',
        'small' => 'small-',
        'medium' => 'medium-',
        'large' => 'large-',
      );
      break;

  }

  if (!empty($settings['large_grid']['grid_large_qty'])) {
    $grid_class[] = $gs_classes['large'] . $settings['large_grid']['grid_large_qty'];
  }
  if ($grid != 'ninesixty') {
    if (!empty($settings['medium_grid']['grid_medium_qty'])) {
      $grid_class[] = $gs_classes['medium'] . $settings['medium_grid']['grid_medium_qty'];
    }
    if (!empty($settings['small_grid']['grid_small_qty'])) {
      $grid_class[] = $gs_classes['small'] . $settings['small_grid']['grid_small_qty'];
    }
  }

  // Adding offset classes if needed.
  if ($grid != 'ninesixty') {
    // Offset for bootstrap and foundation grid systems.
    if (isset($settings['large_grid']['grid_large_offset']) && $settings['large_grid']['grid_large_offset'] != '0') {
      $grid_class[] = $gs_classes['large'] . $gs_classes['offset'] . $settings['large_grid']['grid_large_offset'];
    }
    if (isset($settings['medium_grid']['grid_medium_offset']) && $settings['medium_grid']['grid_medium_offset'] != '0') {
      $grid_class[] = $gs_classes['medium'] . $gs_classes['offset'] . $settings['medium_grid']['grid_medium_offset'];
    }
    if (isset($settings['small_grid']['grid_small_offset']) && $settings['small_grid']['grid_small_offset'] != '0') {
      $grid_class[] = $gs_classes['small'] . $gs_classes['offset'] . $settings['small_grid']['grid_small_offset'];
    }
  }
  else {
    // Offset for 960 grid system.
    if (isset($settings['large_grid']['grid_large_offset']) && $settings['large_grid']['grid_large_offset'] != '0') {
      $grid_class[] = $gs_classes['offset'] . $settings['large_grid']['grid_large_offset'];
    }
  }

  if ($grid_class != '' && !empty($gs_classes['suffix'])) {
    $grid_class[] = $gs_classes['suffix'];
  }

  if (isset($settings['additional_class']) && !empty($settings['additional_class'])) {
    $grid_class[] = $settings['additional_class'];
  }

  return $grid_class;
}
