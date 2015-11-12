<?php

/**
 * @file
 * Contains admin paths for Module Whitelist.
 */

/**
 * Settings form for Module Whitelist.
 */
function module_whitelist_admin_settings() {
  $whitelist_modules = variable_get('module_whitelist', array());
  $whitelist_modules = array_flip($whitelist_modules);

  $form = array('#theme' => 'system_settings_form');

  // Get current list of modules.
  $files = system_rebuild_module_data();

  // Remove hidden modules from display list.
  $visible_files = $files;
  foreach ($visible_files as $filename => $file) {
    if (!empty($file->info['hidden'])) {
      unset($visible_files[$filename]);
    }
  }

  // Remove module whitelist from display list.
  unset($visible_files['module_whitelist']);

  // Create module listing.
  $form['modules'] = array('#tree' => TRUE);

  // Iterate through each of the modules and add to form.
  foreach ($visible_files as $filename => $module) {
    $form['modules'][$module->info['package']][$filename] = array(
      '#tree' => TRUE,
      'name' => array(
        '#markup' => $module->info['name'],
      ),
      'version' => array(
        '#markup' => $module->info['version'],
      ),
      'description' => array(
        '#markup' => t('!description', array('!description' => $module->info['description'])),
      ),
      'whitelist' => array(
        '#type' => 'checkbox',
        '#title' => t('Whitelist'),
        '#default_value' => isset($whitelist_modules[$filename]),
      ),
    );
  }

  // Add basic information to the fieldsets.
  foreach (element_children($form['modules']) as $package) {
    $form['modules'][$package] += array(
      '#type' => 'fieldset',
      '#title' => t('!package', array('!package' => $package)),
      '#collapsible' => TRUE,
      '#theme' => 'module_whitelist_fieldset',
      '#header' => array(
        array('data' => t('Whitelist'), 'class' => array('checkbox')),
        t('Name'),
        t('Version'),
        t('Description'),
      ),
      // Ensure that the "Core" package fieldset comes first.
      '#weight' => $package == 'Core' ? -10 : NULL,
    );
  }

  // Lastly, sort all fieldsets by title.
  uasort($form['modules'], 'element_sort_by_title');

  // Setup form submission.
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#submit'][] = 'module_whitelist_form_submit';

  return $form;
}

/**
 * Returns HTML for the module whitelist form.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @return string
 *   HTML for module whitelist form.
 */
function theme_module_whitelist_fieldset($variables) {
  $form = $variables['form'];

  // Individual table headers.
  $rows = array();
  // Iterate through all the modules, which are children of this fieldset.
  foreach (element_children($form) as $key) {
    // Stick it into $module for easier accessing.
    $module = $form[$key];
    $row = array();
    unset($module['whitelist']['#title']);
    $row[] = array('class' => array('checkbox'), 'data' => drupal_render($module['whitelist']));
    $label = '<label';
    if (isset($module['whitelist']['#id'])) {
      $label .= ' for="' . $module['whitelist']['#id'] . '"';
    }
    $row[] = $label . '><strong>' . drupal_render($module['name']) . '</strong></label>';
    $row[] = drupal_render($module['version']);
    $description = drupal_render($module['description']);
    $row[] = array('data' => $description, 'class' => array('description'));
    $rows[] = $row;
  }

  return theme('table', array('header' => $form['#header'], 'rows' => $rows));
}

/**
 * Form submission handler for system_settings_form().
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 */
function module_whitelist_form_submit($form, &$form_state) {
  $whitelist_modules = array();
  foreach ($form_state['values']['modules'] as $modules) {
    foreach ($modules as $name => $values) {
      $whitelist = $values['whitelist'];
      if ($whitelist) {
        $whitelist_modules[] = $name;
      }
    }
  }
  variable_set('module_whitelist', $whitelist_modules);
}
