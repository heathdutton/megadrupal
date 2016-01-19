<?php
/**
 * @file
 * File to theme admin foe=rm sections.
 */

/**
 * Theme callback for countdown admin form.
 */
function theme_form_submission_timeout_config_form($variables) {
  $fieldset = $variables['form'];
  $children = element_children($fieldset);

  // Remove the input section from the form.
  array_pop($children);

  // Table header.
  $header = array(
    t('Form ID'),
    t('Show Countdown Timer'),
    t('Timeout period (seconds)'),
    t('Timeout message'),
    t('Operation')
  );

  // Table rows.
  foreach ($children as $child) {
    $row = array();
    $row[] = drupal_render($fieldset[$child]['sub_out_form_id_display']);
    $row[] = drupal_render($fieldset[$child]['sub_out_show_timer']);
    $row[] = drupal_render($fieldset[$child]['sub_out_timeout_period']);
    $row[] = drupal_render($fieldset[$child]['sub_out_timeout_message']);
    $row[] = l(t('Remove'), 'admin/config/development/form_submission_timeout/remove/sub_out_form_ids/' . $child . '/countdown', array(
      'attributes' => array(
        'class' => 'sub-out-remove-form-id'
      )
    ));
    $rows[] = $row;
  }

  // Additional row for new form_id input.
  $rows[] = array(
    drupal_render($fieldset['new_form_entry']['sub_out_new_form_id']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_show_timer']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_timeout_period']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_timeout_message']),
    ''
  );
  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'class' => array(
        'form-submission-timeout-table'
      )
    )
  ));
}

/**
 * Theme callback for timed admin form.
 */
function theme_form_submission_timeout_timed_config_form($variables) {
  $fieldset = $variables['form'];
  $children = element_children($fieldset);

  // Remove the input section from the form.
  array_pop($children);

  // Table header.
  $header = array(
    t('Form ID'),
    t('Frequency'),
    t('Date from'),
    t('Time start'),
    t('Date to'),
    t('Time stop'),
    t('Timeout message'),
    t('Operation')
  );

  // Table rows.
  foreach ($children as $child) {
    $row = array();
    $row[] = drupal_render($fieldset[$child]['sub_out_stop_form_id_display']);
    $row[] = drupal_render($fieldset[$child]['sub_out_timeout_frequency']);
    $row[] = drupal_render($fieldset[$child]['sub_out_start_date']);
    $row[] = drupal_render($fieldset[$child]['sub_out_start_timeout_period']);
    $row[] = drupal_render($fieldset[$child]['sub_out_stop_date']);
    $row[] = drupal_render($fieldset[$child]['sub_out_stop_timeout_period']);
    $row[] = drupal_render($fieldset[$child]['sub_out_stop_timeout_message']);
    $row[] = l(t('Remove'), 'admin/config/development/form_submission_timeout/remove/sub_out_stop_form_ids/' . $child . '/timed', array(
      'attributes' => array(
        'class' => 'sub-out-remove-form-id'
      )
    ));
    $rows[] = $row;
  }

  // Additional row for new form_id input.
  $rows[] = array(
    drupal_render($fieldset['new_form_entry']['sub_out_new_stop_form_id']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_timeout_frequency']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_start_date']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_start_timeout_period']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_stop_date']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_stop_timeout_period']),
    drupal_render($fieldset['new_form_entry']['sub_out_new_stop_timeout_message']),
    ''
  );
  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'class' => array(
        'form-submission-timeout-table'
      )
    )
  ));
}
