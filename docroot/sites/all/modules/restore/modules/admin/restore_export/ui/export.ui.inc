<?php
/**
 * @file
 * The export admin form.
 */

/**
 * The export form callback.
 */
function restore_export_ui_script_export($form, &$form_state, $script) {
  $form = array();

  $formats = array(t('- Select a format -'));
  $available = restore_export_formats();
  foreach ($available as $id => $format) {
    $formats[$id] = $format['title'];
  }

  $form['export_format'] = array(
    '#type' => 'select',
    '#title' => t('Export format'),
    '#options' => $formats,
    '#ajax' => array(
      'wrapper' => 'script-export-format-description',
      'callback' => 'restore_export_ui_script_export_output',
    ),
  );

  $form['export'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="script-export-format-description"><pre>',
    '#suffix' => '</pre></div>',
    '#markup' => t('Select an export option.'),
  );

  if (isset($form_state['values']['export_format'])) {
    $form['export']['#markup'] = htmlentities(restore_export_export($script, $form_state['values']['export_format']));
  }

  $form['#attached']['css'][] = drupal_get_path('module', 'restore_export') . '/assets/css/restore-export.css';

  return $form;
}

/**
 * The export format AJAX callback.
 */
function restore_export_ui_script_export_output($form, &$form_state) {
  return $form['export'];
}
