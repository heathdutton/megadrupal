<?php

/**
 * @file
 * Comment Counter
 */

/**
 * Settings form.
 *
 * @param array $form
 *   Form element.
 *
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Form element.
 */
function comment_counter_settings_form($form, &$form_state) {
  // Content counter settings.
  $form['content'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Comment counter settings'),
  );

  $form['content']['comment_counter_count_comments'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Count comments'),
    '#default_value' => variable_get('comment_counter_count_comments', 0),
    '#description'   => t('Increment a counter each time comment is saved.'),
  );

  return system_settings_form($form);
}
