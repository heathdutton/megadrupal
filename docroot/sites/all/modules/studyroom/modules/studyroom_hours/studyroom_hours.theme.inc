<?php
/**
 * @file
 * Themes the studyroom room_hours formatter and widget.
 */

/**
 * Returns HTML for a date element formatted as a range.
 */
function theme_hours_display($variables) {
  $start_time = $variables['start_time'];
  $end_time = $variables['end_time'];
  $weekday = $variables['weekday'];
  $spacer = $variables['display']['settings']['time_spacer'];

  // Wrap the result with the attributes.
  return t('!weekday: !start-date !spacer !end-date', array(
    '!weekday' => '<span class="studyroom-hours-weekday-display">' . $weekday . '</span>',
    '!spacer' => $spacer,
    '!start-date' => '<span class="studyroom-hours-display-start">' . $start_time . '</span>',
    '!end-date' => '<span class="studyroom-hours-display-end">' . $end_time . '</span>',
  ));
}

/**
 * Returns HTML for a element.
 */
function theme_studyroom_hours($variables) {
  $element = $variables['element'];

  $output = '';
  $table_id = drupal_html_id($element['#field_name'] . '_values');
  $required = !empty($element['#required']) ? theme('form_required_marker', $variables) : '';

  $header = array(
    t('Weekday'),
    t('Start time'),
    t('End time'),
  );
  $rows = array();
  $caption = $element['#title'];

  // Add the items as table rows.
  /* foreach (date_week_days_untranslated() as $delta) { */
  for ($delta = 0; $delta < 7; $delta++) {
    $cells = array();
    $cells[] = array('data' => $element[$delta]['#title'], 'class' => 'weekday');
    $cells[] = $element[$delta]['start_time']['#children'];
    $cells[] = $element[$delta]['end_time']['#children'];
    $rows[] = array(
      'data' => $cells,
    );
  }

  $output = '<div id="studyroom-hours-field" class="form-item">';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => $table_id,
      'class' => array('hours-table'),
    ),
    'caption' => $caption,
    ));
  $output .= $element['#description'] ? '<div class="description">' . $element['#description'] . '</div>' : '';
  $output .= isset($element['#limit_note']) ? '<div class="description">' . $element['#limit_note'] . '</div>' : '';

  return $output;
}
