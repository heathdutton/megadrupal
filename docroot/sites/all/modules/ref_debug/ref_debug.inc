<?php
/**
 * @file
 * Page callback function(s)
 */

/**
 * Display the ref debug.
 */
function ref_debug_load($type, $object, $name = NULL) {
  // If the debug is set to be displayed above output.
  if (variable_get('ref_debug_admin_display') === 0) {
    $output = '<div class="ref-debug-object">';
    $output .= @r($object);
    $previous_page = $_SERVER['HTTP_REFERER'];
    $output .= '<p class="ref-debug-object-info-content">';
    $output .= t('Hit x to fold/unfold, or click arrows.');
    $output .= '</p>';
    $output .= '</div>';
  }
  // If it is not set, render it in content area.
  else {
    $output = r($object);
    $output .= '<p class="ref-debug-object-info-above">';
    $output .= t('Output of debug is above. Hit x to fold/unfold, or click arrows.');
    $output .= '</p>';
  }
  return $output;
}

/**
 * Settings form for ref debug.
 */
function ref_debug_admin_settings() {
  $form = array();
  $form['ref_debug_admin_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display debug info above page for entities'),
    '#default_value' => variable_get('ref_debug_admin_display', 0),
    '#description' => t("If checked debug info will be displayed above page, if not checked it is displayed in content area."),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
