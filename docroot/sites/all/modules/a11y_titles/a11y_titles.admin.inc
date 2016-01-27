<?php
/**
 * @file
 * Provides open access to content titles.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function _a11y_titles_form_block_admin_configure_alter(&$form, &$form_state) {
  // Retrieve the title settings.
  $blocks = variable_get('a11y_titles_blocks', array());

  // Don't mention the <hide> option.
  if ($form['module']['#value'] != 'block') {
    $form['settings']['title']['#description'] = t('Override the default title for the block.');
  }
  // Add the accessibility toggle.
  $form['settings']['a11y_title'] = array(
    '#type' => 'radios',
    '#title' => t('Block title'),
    '#default_value' => isset($blocks[$form['module']['#value'] . '--' . $form['delta']['#value']]) ? $blocks[$form['module']['#value'] . '--' . $form['delta']['#value']] : 'normal',
    '#options' => array(
      'normal'    => t('Display normally.'),
      'invisible' => t('Display accessibly, but hidden visually.'),
      'hidden'    => t('Do not display.'),
    ),
    '#description' => t('Block titles can provide context to users who can’t rely on visual cues.'),
    '#weight' => -17,
  );

  // Add an additional submit handler.
  $form['#submit'][] = 'a11y_titles_block_submit';
}

/**
 * Form submit handler.
 */
function a11y_titles_block_submit(&$form, &$form_state) {
  $blocks = variable_get('a11y_titles_blocks', array());
  if ($form_state['values']['module'] == 'normal') {
    unset($blocks[$form_state['values']['module'] . '--' . $form_state['values']['delta']]);
  }
  else {
    $blocks[$form_state['values']['module'] . '--' . $form_state['values']['delta']] = $form_state['values']['a11y_title'];
  }
  variable_set('a11y_titles_blocks', $blocks);
}
