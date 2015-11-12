<?php
/**
 * @file
 * Administration page callbacks for the region_tokens module.
 *
 * @ingroup region_tokens
 * @{
 */

/**
 * Form builder. Configure my_module.
 *
 * @ingroup forms
 * @see system_settings_form()
 */

function region_tokens_settings($form, &$form_state) {
  $form = array();
  $form['#options'] = array();
  foreach (list_themes() as $theme_key => $data) {
    if ($data->status) {
      foreach (system_region_list($theme_key) as $key => $region) {
        $form['#options'][$theme_key . ':' . $key] = $data->info['name'] . ":$region";
      }
    }
  }

  $options = $form['#options'];
  foreach ($options as $key => $value) {
    $options[$key] = "$value <strong><code>[region:$key]</code></strong>";
  }

  $default = array_keys(variable_get('region_tokens:regions', array()));
  $form['region_tokens:regions'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose the region(s) to make available as tokens'),
    '#default_value' => $default,
    '#options' => $options,
  );

  $form['#attached']['js'][] = drupal_get_path('module', 'region_tokens') . '/js/region_tokens.js';

  $form['#submit'][] = 'region_tokens_settings_submit';
  return system_settings_form($form);
}

/**
 * Form submission handler
 */
function region_tokens_settings_submit($form, &$form_state) {
  $regions = &$form_state['values']['region_tokens:regions'];
  $regions = array_filter($form_state['values']['region_tokens:regions']);
  $regions = array_intersect_key($form['#options'], $regions);
  // We need to set this before clearing the cache
  variable_set('region_tokens:regions', $form_state['values']['region_tokens:regions']);
  token_clear_cache();
}

/** @} */ //end of group region_tokens1
