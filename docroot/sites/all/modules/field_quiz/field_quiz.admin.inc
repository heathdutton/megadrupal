<?php


/**
 * Admin settings form for the limelight field
 * @return mixed
 */
function field_quiz_admin_settings_form() {
  $form = array();
  $form['field_quiz_shuffle_answers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shuffle answers?'),
    '#description' => t('If checked the answers will be shuffled. Attention: If enabled, page caching is disabled for these pages!'),
    '#default_value' => variable_get('field_quiz_shuffle_answers', FALSE),
  );
  return system_settings_form($form);
}