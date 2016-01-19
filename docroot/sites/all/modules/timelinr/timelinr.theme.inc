<?php

/**
 * @file
 * Theme functions for timeline.
 */

/**
 * Implementation of template_process for views-view-timelinr().
 */
function template_preprocess_views_view_timelinr(&$vars) {
  if (!empty($vars['view']->live_preview)) {
    drupal_set_message(t('The timeline cannot be viewed in the preview.'), 'warning');
  }
  $view = $vars['view'];
  $options = $vars['options'];

  // Check if data is available.
  if (count($view->result) >= 1) {
    // Create actual timeline.
    $vars['timelinr_id'] = 'timelinr-' . $view->name . '-' . $view->current_display;
    $vars['timelinr_dates_id'] = 'timelinr-dates-' . $view->name . '-' . $view->current_display;
    $vars['timelinr_issues_id'] = 'timelinr-issues-' . $view->name . '-' . $view->current_display;
    $reset_style = $vars['options']['reset_style'];
    $cfg = array();
    $params = array(
      'orientation' => $vars['options']['orientation'],
      'containerDiv' => '#' . $vars['timelinr_id'],
      'datesDiv' => '#' . $vars['timelinr_dates_id'],
      'issuesDiv' => '#' . $vars['timelinr_issues_id'],
      'issuesSpeed' => $options['issues_speed'],
      'issuesTransparency' => $options['issues_transparency'],
      'issuesTransparencySpeed' => $options['issues_transparency_speed'],
      'prevButton' => $options['prev_button'],
      'nextButton' => $options['next_button'],
      'arrowKeys' => $options['arrow_keys'],
      'startAt' => $options['start_at'],
      'autoPlay' => $options['auto_play'],
      'autoPlayDirection' => $options['auto_play_direction'],
      'autoPlayPause' => $options['auto_play_pause'],
    );
    $row_options = $view->style_plugin->row_plugin->options;
    $i = 1;
    $vars['row_dates'] = array();

    // Get data for display date-slider.
    foreach ($view->style_plugin->rendered_fields as $row) {
      $field_value = strip_tags($row[$row_options['dates_field']]);
      // Create anchor-link for issues.
      $vars['row_dates'][$i] = l($field_value, '', array('fragment' => $vars['timelinr_dates_id'] . '_' . $i, 'external' => TRUE));
      $i++;
    }

    $cfg[$vars['timelinr_id']] = $params;
    if (empty($vars['view']->live_preview)) {
      drupal_add_js(array('timelinr' => $cfg), 'setting');
      _timelinr_add_libraries($options['orientation'], $reset_style);
    }
  } else {
    theme('timelinr_nodata', array());
  }
}

/**
 * Overridable theme function - prints warning if no data is available.
 */
function theme_timelinr_nodata() {
  drupal_set_message(t('There is no data available to show on the timeline'), 'warning');
}

/**
 * Implementation of template preprocess for the view fields.
 */
function template_preprocess_timelinr_view_timelinrfields(&$vars) {
  $view = &$vars['view'];
  $options = $vars['options'];
  $row = $vars['row'];
  $img = '';
  $content = '';

  $attributes = NULL;

  foreach ($view->field as $id => $field) {
    $field_output = $view->field[$id]->theme($vars['row']);

    switch ($id) {
      case $options['image_field']:
        $field_image_name = "field_{$options['image_field']}";
        if (!empty($row->{$field_image_name}[0]['raw'])) {
          $image_uri = $row->{$field_image_name}[0]['raw']['uri'];
          $presetname = $row->{$field_image_name}[0]['rendered']['#image_style'];

          // If we have provided an image-style we render image with it.
          if (!empty($presetname)) {
            $params = array(
              'style_name' => $presetname,
              'path' => $image_uri,
              'attributes' => $attributes,
            );
            $img = theme('image_style', $params);
          }
          // If no image-style is provided we output original image.
          else {
            $params = array(
              'path' => $image_uri,
              'attributes' => $attributes,
            );
            $img = theme('image', $params);
          }
        }
        break;

      case $options['title_field']:
        $title = $field_output;
        break;

      case $options['body_field']:
        $content = $field_output;
        break;
    }
  }
  $vars['id_item'] = 'timelinr-dates-' . $view->name . '-' . $view->current_display . '_' . $vars['id'];
  $vars['image'] = $img;
  $vars['headline'] = $title;
  $vars['content'] = $content;
}

/**
 * Inlude all js and css files that needs the timeline.
 *
 * @return: TRUE if everything could be included properly
 *          FALSE if Simile Exhibit Libraries couldn't be included properly
 */
function _timelinr_add_libraries($orientation, $reset_style = FALSE) {
  static $loaded = FALSE;
  if (!$loaded) {
    // Check correct inclusion of local timeline library.
    $timelinr_library_status = timelinr_validate_library();
    if ($timelinr_library_status) {
      // Errors are available, because of detected errors during validation.
      if (is_string($timelinr_library_status)) {
        drupal_set_message(check_plain($timelinr_library_status));
        return FALSE;
      }
      // Timeline library has been validated and will be added.
      elseif ($timelinr_library_status == TRUE) {
        $lib_path = libraries_get_path('timelinr');
        // Default style.
        $css = '/css/style_h.css';
        if ($orientation == 'vertical') {
          $css = '/css/style_v.css';
        }

        drupal_add_css($lib_path . $css, array('group' => CSS_SYSTEM));

        // Add reset style.
        if ($reset_style) {
          drupal_add_css(drupal_get_path('module', 'timelinr') . '/css/reset.css', 'theme');
        }
        drupal_add_js($lib_path . '/js/jquery.timelinr-0.9.52.min.js');
        drupal_add_js(drupal_get_path('module', 'timelinr') . '/js/timelinr.js');
        $loaded = TRUE;
        return TRUE;
      }
    }
    drupal_set_message(t('timelinr Libraries not found'), 'error');
    return FALSE;
  }
}
