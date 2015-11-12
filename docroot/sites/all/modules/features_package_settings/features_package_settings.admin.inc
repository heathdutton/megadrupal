<?php

/**
 * @file
 * Admin callbacks for features_weight.
 */

/**
 * Admin callback for the weight form.
 */
function features_package_settings_admin_form($form, $form_state) {
  $packages = _features_package_settings_get_packages();
  $settings = variable_get('features_package_settings_settings', array());

  // Iterate through each database result.
  foreach ($packages as $key => $package) {
    // Default weight values.
    $default_weight = 0;
    $default_state = TRUE;

    if (isset($settings[$package])) {
      if (isset($settings[$package]['weight'])) {
        $default_weight = $settings[$package]['weight'];
      }
      if (isset($settings[$package]['state'])) {
        $default_state = $settings[$package]['state'];
      }
    }

    $form['packages'][$package] = array(
      'name' => array(
        '#markup' => check_plain($package),
      ),
      'state' => array(
        '#type' => 'checkbox',
        '#title' => t('Check State'),
        '#default_value' => $default_state,
        '#title_display' => 'invisible',
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $default_weight,
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
    );
  }
  // Data was not ordered by weight when loading, so order the form elements by
  // weight now to appear in correct order.
  uasort($form['packages'], '_features_package_settings_compare');

  $form['packages']['#tree'] = TRUE;

  $form['rebuild'] = array(
    '#type' => 'fieldset',
  );
  $form['rebuild']['description'] = array(
    '#markup' => t('The list of packages on this page is cached for performance reasons, you can rebuild the caches when necessary.') . '<br /><br />',
  );
  $form['rebuild']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Package List'),
    '#submit' => array('features_package_settings_admin_form_refresh'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
    '#submit' => array('features_package_settings_admin_form_submit'),
  );

  return $form;
}

/**
 * Submit function for the admin form, simply saves the data in variable.
 *
 * @param array $form
 *   The submitted form.
 * @param array $form_state
 *   The submitted values.
 */
function features_package_settings_admin_form_submit(array $form, array &$form_state) {
  variable_set('features_package_settings_settings', $form_state['values']['packages']);
}

/**
 * Submit function for the refresh button, refreshes the caches.
 *
 * @param array $form
 *   The submitted form.
 * @param array $form_state
 *   The submitted values.
 */
function features_package_settings_admin_form_refresh(array $form, array &$form_state) {
  cache_clear_all('features_package_settings:packages', 'cache');
}

/**
 * Theme function for the admin form, adds table drag.
 *
 * @param array $variables
 *   Array of variables as defined in hook_theme.
 *
 * @return string
 *   The rendered form.
 */
function theme_features_package_settings_admin_form(array $variables) {
  $output = '';
  $form = $variables['form'];

  // Top of the form - the rebuild section.
  $output .= drupal_render($form['rebuild']);

  $rows = array();
  // Iterate over each element in our $form['example_items'] array.
  foreach (element_children($form['packages']) as $id) {
    $form['packages'][$id]['weight']['#attributes']['class'] = array('package-item-weight');

    $rows[] = array(
      'data' => array(
        drupal_render($form['packages'][$id]['name']),
        drupal_render($form['packages'][$id]['state']),
        drupal_render($form['packages'][$id]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(t('Package'), t('Check State'), t('Weight'));
  $table_id = 'package-items-table';

  // We can render our tabledrag table for output.
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  // And then render any remaining form elements (such as our submit button).
  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'package-item-weight');

  return $output;
}

/**
 * Compares two form elements based on weight default value.
 *
 * @param array $a
 *   The first array to compare.
 * @param array $b
 *   The second array to compare.
 *
 * @return int
 *   The result of comparison.
 */
function _features_package_settings_compare(array $a, array $b) {
  if ($a['weight']['#default_value'] == $b['weight']['#default_value']) {
    return 0;
  }
  return ($a['weight']['#default_value'] < $b['weight']['#default_value']) ? -1 : 1;
}

/**
 * Retrieves the packages from cache if exists.
 */
function _features_package_settings_get_packages() {
  $cache = cache_get('features_package_settings:packages');

  if ($cache) {
    $packages = $cache->data;
  }
  else {
    // Clear & rebuild key caches.
    features_get_info(NULL, NULL, TRUE);
    features_rebuild();

    $features = features_get_features();
    $packages = array();

    foreach ($features as $feature) {
      $pkg = $feature->info['package'];

      if (!in_array($pkg, $packages)) {
        $packages[] = $pkg;
      }
    }

    cache_set('features_package_settings:packages', $packages);
  }

  return $packages;
}