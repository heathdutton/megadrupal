<?php
/**
 * @file
 */

function watchdog_filtering_settings_form() {
  $form = array();
  $defaults = variable_get('watchdog_filtering_default_severity', array_keys(watchdog_severity_levels()));
  $form['watchdog_filtering_default_severity'] = array(
    '#type' => 'select',
    '#title' => t('Default severity level'),
    '#description' => t('Log levels not selected here will be ignored by default.'),
    '#options' => watchdog_severity_levels(),
    '#default_value' => $defaults,
    '#multiple' => TRUE,
    '#size' => count(watchdog_severity_levels()),
  );
  $form['watchdog_filtering_deduplicate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Deduplicate'),
    '#description' => t('If checked duplicate log entries within the same request will be suppressed.'),
    '#default_value' => variable_get('watchdog_filtering_deduplicate', FALSE),
  );
  $form['watchdog_filtering'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Filters'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $levels = _watchdog_filtering_get_levels();
  foreach ($levels as $type => $severity) {
    $enc_type = $type;
    $type = rawurldecode($type);
    $form['watchdog_filtering']["type_$enc_type"] = array(
      '#type' => 'select',
      '#title' => $type,
      '#options' => array('default' => t('Default'), 'custom' => t('Custom')),
      '#default_value' => is_array($severity) ? 'custom' : 'default',
    );
    $form['watchdog_filtering']["severity_$enc_type"] = array(
      '#type' => 'select',
      '#options' => watchdog_severity_levels(),
      '#default_value' => is_array($severity) ? $severity : $defaults,
      '#multiple' => TRUE,
      '#states' => array(
        'visible' => array(':input[name="watchdog_filtering[type_' . $enc_type . ']"]' => array('value' => 'custom')),
      ),
      '#size' => count(watchdog_severity_levels()),
    );
  }
  $form['#validate'][] = 'watchdog_filtering_settings_form_validate';

  return system_settings_form($form);
}

function watchdog_filtering_settings_form_validate($form, &$form_state) {
  foreach ($form_state['values']['watchdog_filtering'] as $key => $value) {
    if (preg_match('/^type_(.*)$/', $key, $matches)) {
      $value = $value === 'default' ? NULL : $form_state['values']['watchdog_filtering']['severity_' . $matches[1]];
      $form_state['values']['watchdog_filtering_severity_' . $matches[1]] = $value;
    }
  }
  unset($form_state['values']['watchdog_filtering']);
}
