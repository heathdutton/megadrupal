<?php

/**
 * @file
 * Styleswitcher configuration functionality.
 */

/**
 * Page callback: Constructs a form for the Styleswitcher configuration.
 *
 * @see styleswitcher_admin_submit()
 * @see styleswitcher_menu()
 *
 * @ingroup forms
 */
function styleswitcher_admin($form, &$form_state) {
  $styles = styleswitcher_custom_styles();
  ksort($styles);

  $header = array(
    t('Style'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );
  $rows = array();

  foreach ($styles as $name => $style) {
    $name_hyphenated = strtr($name, '_', '-');

    // Build the table row.
    $row = array();
    $row[]['data'] = array(
      '#theme' => 'styleswitcher_admin_style_overview',
      '#style' => $style,
    );
    $row[]['data'] = array(
      '#markup' => l(t('edit'), 'admin/config/user-interface/styleswitcher/' . $name_hyphenated),
    );
    $row[]['data'] = array(
      '#markup' => l(t('delete'), 'admin/config/user-interface/styleswitcher/' . $name_hyphenated . '/delete'),
      // Do not allow to delete the blank style.
      '#access' => isset($style['path']),
    );
    $rows[] = $row;
  }

  $form['styleswitcher_custom_styles'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $form['styleswitcher_enable_overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable overlay'),
    '#description' => t('Enable the overlay and fade when switching stylesheets.'),
    '#default_value' => variable_get('styleswitcher_enable_overlay', 1),
  );

  $form['#submit'][] = 'styleswitcher_admin_submit';

  return system_settings_form($form);
}

/**
 * Form submission handler for styleswitcher_admin().
 */
function styleswitcher_admin_submit($form, &$form_state) {
  _styleswitcher_configuration_changed();
}

/**
 * Page callback: Constructs a form for a theme-specific styles settings.
 *
 * @param string $theme
 *   Name of the theme to configure styles for.
 *
 * @see styleswitcher_config_theme_validate()
 * @see styleswitcher_config_theme_submit()
 * @see styleswitcher_menu()
 *
 * @ingroup forms
 */
function styleswitcher_config_theme($form, &$form_state, $theme) {
  $styles = styleswitcher_style_load_multiple($theme);
  uasort($styles, 'styleswitcher_sort');
  $options = array_fill_keys(array_keys($styles), '');

  $form['theme_name'] = array(
    '#type' => 'value',
    '#value' => $theme,
  );

  $form['settings'] = array(
    '#theme' => 'styleswitcher_admin_styles_table',
    '#tree' => TRUE,
  );
  foreach ($styles as $name => $style) {
    $form['settings']['weight'][$name] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @label', array('@label' => $style['label'])),
      '#title_display' => 'invisible',
      '#delta' => _styleswitcher_weight_delta($theme),
      '#default_value' => $style['weight'],
      '#weight' => $style['weight'],
      // Set special class for drag and drop updating.
      '#attributes' => array('class' => array('styleswitcher-style-weight')),
    );
    $form['settings']['name'][$name] = array(
      '#theme' => 'styleswitcher_admin_style_overview',
      '#style' => $style,
    );
    $form['settings']['label'][$name] = array(
      '#markup' => check_plain($style['label']),
    );
  }
  $form['settings']['enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => array_keys(styleswitcher_style_load_multiple($theme, array('status' => TRUE))),
  );
  $form['settings']['default'] = array(
    '#type' => 'radios',
    '#title' => t('Default'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => styleswitcher_default_style_key($theme),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form validation handler for styleswitcher_config_theme().
 *
 * @see styleswitcher_config_theme_submit()
 */
function styleswitcher_config_theme_validate($form, &$form_state) {
  $theme = $form_state['values']['theme_name'];
  $values = $form_state['values']['settings'];

  // Automatically enable the default style and the style which was default
  // previously because we will not get the value from that disabled checkbox.
  $values['enabled'][$values['default']] = 1;
  $values['enabled'][styleswitcher_default_style_key($theme)] = 1;

  form_set_value($form['settings'], $values, $form_state);
}

/**
 * Form submission handler for styleswitcher_config_theme().
 *
 * @see styleswitcher_config_theme_validate()
 */
function styleswitcher_config_theme_submit($form, &$form_state) {
  $theme = $form_state['values']['theme_name'];
  $values = $form_state['values']['settings'];
  $theme_settings = array();

  foreach (array_keys(styleswitcher_style_load_multiple($theme)) as $name) {
    $theme_settings[$name] = array(
      'weight' => $values['weight'][$name],
      'status' => !empty($values['enabled'][$name]),
      'is_default' => ($values['default'] == $name),
    );
  }

  // Get all settings (for all themes).
  $settings = variable_get('styleswitcher_styles_settings', array());
  $settings[$theme] = $theme_settings;
  variable_set('styleswitcher_styles_settings', $settings);

  drupal_set_message(t('The configuration options have been saved.'));

  _styleswitcher_configuration_changed();
}

/**
 * Page callback: Constructs a form to add/edit a style.
 *
 * @param array|null $style
 *   (optional) Style to edit. The structure of an array is the same as returned
 *   from styleswitcher_style_load().
 *
 * @see styleswitcher_style_form_validate()
 * @see styleswitcher_style_form_submit()
 * @see styleswitcher_menu()
 * @see styleswitcher_style_load()
 *
 * @ingroup forms
 */
function styleswitcher_style_form($form, &$form_state, $style = NULL) {
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Human-readable name for this style.'),
    '#default_value' => $style ? $style['label'] : '',
    '#required' => TRUE,
  );

  if ($style) {
    list(, $name_value) = explode('/', $style['name']);
  }
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $style ? $name_value : '',
    '#field_prefix' => 'custom/',
    '#machine_name' => array(
      'source' => array('label'),
      'exists' => '_styleswitcher_style_exists',
    ),
  );

  $form['old_name'] = array(
    '#type' => 'value',
    '#value' => $style ? $style['name'] : '',
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('The path to the stylesheet file relative to the site root or an external CSS file.'),
    '#default_value' => $style ? $style['path'] : '',
    '#required' => TRUE,
    '#access' => !$style || isset($style['path']),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    // Do not allow to delete the blank style.
    '#access' => isset($style['path']),
  );

  return $form;
}

/**
 * Form validation handler for styleswitcher_style_form().
 *
 * @see styleswitcher_style_form_submit()
 */
function styleswitcher_style_form_validate($form, &$form_state) {
  // Trim text values now, so submission handlers get them fully validated.
  // No need to trim name, because it's validated by a machine-name pattern.
  form_set_value($form['label'], trim($form_state['values']['label']), $form_state);

  $path = $form_state['values']['path'];

  if ($path === '') {
    // Set the path back to NULL.
    form_set_value($form['path'], NULL, $form_state);
  }
  else {
    $path = trim($path);
    form_set_value($form['path'], $path, $form_state);

    if (!is_file($path) && !url_is_external($path)) {
      form_set_error('path', t('Stylesheet file %path does not exist.', array('%path' => $path)));
    }
  }
}

/**
 * Form submission handler for styleswitcher_style_form().
 *
 * @see styleswitcher_style_form_validate()
 */
function styleswitcher_style_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $old_name = $form_state['values']['old_name'];

  if ($op == t('Delete')) {
    $form_state['redirect'] = 'admin/config/user-interface/styleswitcher/' . strtr($old_name, '_', '-') . '/delete';
    return;
  }

  $styles = styleswitcher_custom_styles();
  $style = array(
    'label' => $form_state['values']['label'],
    'name' => 'custom/' . $form_state['values']['name'],
    'path' => $form_state['values']['path'],
  );

  if ($old_name !== '') {
    unset($styles[$old_name]);

    // Update style keys in settings variable.
    if ($style['name'] != $old_name) {
      $settings = variable_get('styleswitcher_styles_settings', array());

      foreach (array_keys($settings) as $theme) {
        if (isset($settings[$theme][$old_name])) {
          $settings[$theme][$style['name']] = $settings[$theme][$old_name];
          unset($settings[$theme][$old_name]);
        }
      }

      variable_set('styleswitcher_styles_settings', $settings);
    }
  }

  $styles[$style['name']] = $style;
  variable_set('styleswitcher_custom_styles', $styles);

  drupal_set_message(t('The style %title has been saved.', array('%title' => $style['label'])));

  _styleswitcher_configuration_changed($form_state);
}

/**
 * Page callback: Constructs a form to delete a single style.
 *
 * @param array $style
 *   Style to delete. The structure of an array is the same as returned from
 *   styleswitcher_style_load().
 *
 * @see styleswitcher_style_delete_form_submit()
 * @see styleswitcher_menu()
 * @see styleswitcher_style_load()
 *
 * @ingroup forms
 */
function styleswitcher_style_delete_form($form, &$form_state, array $style) {
  $form['label'] = array('#type' => 'value', '#value' => $style['label']);
  $form['name'] = array('#type' => 'value', '#value' => $style['name']);

  $message = t('Are you sure you want to delete the style %title?', array('%title' => $style['label']));
  $description = t('The style %title will be permanently deleted.', array('%title' => $style['label']));

  return confirm_form($form, $message, 'admin/config/user-interface/styleswitcher', $description);
}

/**
 * Form submission handler for styleswitcher_style_delete_form().
 */
function styleswitcher_style_delete_form_submit($form, &$form_state) {
  $name = $form_state['values']['name'];
  $styles = styleswitcher_custom_styles();

  if (isset($styles[$name]['path'])) {
    unset($styles[$name]);
    variable_set('styleswitcher_custom_styles', $styles);

    drupal_set_message(t('The style %title has been deleted.', array('%title' => $form_state['values']['label'])));
  }
  else {
    drupal_set_message(t('The blank style cannot be deleted.'), 'warning');
  }

  _styleswitcher_configuration_changed($form_state);
}

/**
 * Returns HTML for the styles settings overview form element.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: An element representing the form.
 *
 * @ingroup themeable
 */
function theme_styleswitcher_admin_styles_table(array $variables) {
  $form = $variables['form'];
  $header = array(
    t('Style'),
    t('Enabled'),
    t('Default'),
    t('Weight'),
  );
  $rows = array();

  if (!empty($form['weight'])) {
    foreach (element_children($form['weight']) as $key) {
      if ($key == $form['default']['#default_value']) {
        $form['enabled'][$key]['#attributes']['disabled'] = 'disabled';
      }

      $label = drupal_render($form['label'][$key]);
      $form['enabled'][$key]['#title'] = t('Enable !title', array('!title' => $label));
      $form['enabled'][$key]['#title_display'] = 'invisible';
      $form['default'][$key]['#title'] = t('Set !title as default', array('!title' => $label));
      $form['default'][$key]['#title_display'] = 'invisible';

      // Build the table row.
      $row = array(
        array('data' => $form['name'][$key]),
        array('data' => $form['enabled'][$key]),
        array('data' => $form['default'][$key]),
        array('data' => $form['weight'][$key]),
      );
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }

  $table_vars = array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No styles to switch.'),
    'attributes' => array('id' => 'styleswitcher-styles-table'),
  );
  $output = theme('table', $table_vars);

  drupal_add_tabledrag('styleswitcher-styles-table', 'order', 'sibling', 'styleswitcher-style-weight');

  return $output;
}

/**
 * Returns HTML for a style description on the styles overview page.
 *
 * @param array $variables
 *   An associative array containing:
 *   - style: A style array as returned from styleswitcher_style_load().
 *
 * @see styleswitcher_style_load()
 *
 * @ingroup themeable
 */
function theme_styleswitcher_admin_style_overview(array $variables) {
  $style = $variables['style'];

  $output = check_plain($style['label']);
  $output .= ' <small>' . t('(Machine name: @name)', array('@name' => $style['name'])) . '</small>';

  if (isset($style['path'])) {
    $description = $style['path'];
  }
  else {
    $description = t('Blank style which just removes effect of others.');
  }
  $output .= '<div class="description">' . $description . '</div>';

  return $output;
}

/**
 * Calculates #delta for style's weight element.
 *
 * The function does not use static variable because it's called only once.
 *
 * @param string $theme
 *   Name of the theme for which styles the delta is calculated.
 *
 * @return int
 *   Optimal #delta value.
 */
function _styleswitcher_weight_delta($theme) {
  foreach (styleswitcher_style_load_multiple($theme) as $style) {
    $weights[] = $style['weight'];
  }

  return max(abs(min($weights)), max($weights), floor(count($weights) / 2));
}

/**
 * Performs routine tasks after module's configuration has been changed.
 *
 * @param array &$form_state
 *   (optional) Form state array from the form being submitted.
 */
function _styleswitcher_configuration_changed(array &$form_state = array()) {
  $form_state['redirect'] = 'admin/config/user-interface/styleswitcher';

  // Clear blocks and pages from cache to reflect style changes.
  if (module_exists('block')) {
    cache_clear_all('styleswitcher:styleswitcher:', 'cache_block', TRUE);
  }
  cache_clear_all('*', 'cache_page', TRUE);
}

/**
 * Checks whether a submitted machine name value already exists.
 *
 * @param string $input
 *   User-submitted value.
 *
 * @return array|false
 *   Style array on success or FALSE otherwise. Style is an associative array as
 *   returned from styleswitcher_style_load().
 *
 * @see styleswitcher_style_load()
 */
function _styleswitcher_style_exists($input) {
  // It does not matter what theme to set in this load call, because all custom
  // styles exist in all themes. Let's set one from the current page just to
  // decrease calculations.
  return styleswitcher_style_load($input, $GLOBALS['theme'], 'custom');
}
