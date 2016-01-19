<?php
/**
 * @file
 * Text Selection module - Variables declaration
 */

/**
 * Implements hook_variable_info().
 */
function text_selection_variable_info($options) {
  $variable['text_selection_bg_color'] = array(
    'title' => t('Text selection background color'),
    'type' => 'text',
  );
  $variable['text_selection_bg_color'] = array(
    'title' => t('Text selection font color'),
    'type' => 'text',
  );
  return $variable;
}
