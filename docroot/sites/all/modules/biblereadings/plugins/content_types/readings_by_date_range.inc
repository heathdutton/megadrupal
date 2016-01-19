<?php

$plugin = array(
    'title' => t('Bible Readings by Date Range'),
    'render callback' => 'biblereadings_readings_by_date_range_content_type_render',
    'description' => t('Displays a list of Bible readings by date range.'),
    'edit form' => 'biblereadings_readings_by_date_range_content_type_edit_form',
    'all contexts' => TRUE,
    'category' => t('Bible Readings'),
    'no title override' => FALSE,
    'single' => TRUE,
);

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function biblereadings_readings_by_date_range_content_type_render($subtype, $conf, $args, $context) {
  $content = new stdClass();


  $range = biblereadings_get_date_range($conf['range_type']);

  $readings = biblereadings_readings_by_date_range($range['start'], $range['end']);

  $output = theme('biblereadings_readings_this_week', array('readings' => $readings, 'show_downloads' => $conf['show_downloads']));

  $content->content = $output;

  return $content;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function biblereadings_readings_by_date_range_content_type_edit_form($form, $form_state) {
  $conf = $form_state['conf'];

  $form['range_type'] = array(
      '#title' => t('Select a Date Range'),
      '#type' => 'select',
      '#options' => array(
          'this_week' => t('This Week'),
          'next_week' => t('Next Week'),
          'this_month' => t('This Month'),
          'next_month' => t('Next Month'),
          'next_30_days' => t('Next 30 Days'),
          'next_60_days' => t('Next 60 Days'),
          'next_90_days' => t('Next 90 Days'),
          'this_year' => t('This Year'),
      ),
      '#default_value' => $conf['range_type'],
  );

  $form['show_downloads'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show downloads'),
      '#default_value' => $conf['show_downloads'],
  );

  return $form;
}

function biblereadings_readings_by_date_range_content_type_edit_form_submit($form, $form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
