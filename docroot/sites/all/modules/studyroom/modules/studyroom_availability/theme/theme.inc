<?php

/**
 * @file
 * Theme preprocess functions for studyroom_availability module.
 */

/**
 * Theme output of studyroom_availability.
 */
function template_preprocess_studyroom_availability_day(&$variables) {
  $location = $variables['location'];
  $variables['loc'] = '<p>' . t('Spaces/availability for %location', array('%location' => $location->label)) . '</p>';
  $variables['spaces_header'] = $variables['availability']->spaces_header;

  // Calculate forward and back dates for calendar.
  $current = new DateTime("now");
  $forward_date = clone $variables['date'];
  $backward_date = clone $forward_date;
  $forward_date->add(new DateInterval('P1D'));
  $backward_date->sub(new DateInterval('P1D'));

  // Create links.
  $path = 'availability/' . $location->type . '/';

  $backward_path = $path . $backward_date->format('Y-m-d');
  $nav['links']['prev'] = l(t('Previous Day'), $backward_path);

  $current_path = $path . $current->format('Y-m-d');
  $nav['links']['current'] = l(t('Today'), $current_path);

  $forward_path = $path . $forward_date->format('Y-m-d');
  $nav['links']['next'] = l(t('Next Day'), $forward_path);

  $variables['page']['navigation'] = theme('studyroom_availability_week_navigation', $nav);
}

/**
 * Theme output of studyroom_availability.
 */
function template_preprocess_studyroom_availability_week(&$variables) {
  $availability = $variables['availability'];
  $space = $variables['space'];

  // Calculate forward and back dates for calendar.
  $current = new DateTime("now");
  $cal_week = date_week_range($availability->date['week'], $availability->date['year']);
  $forward_date = $cal_week[0];
  $backward_date = clone $forward_date;
  $forward_date->add(new DateInterval('P1W'));
  $backward_date->sub(new DateInterval('P1W'));

  // Calculate and format weeks.
  $current_week = date_week(date_format($current, DATE_FORMAT_DATE));
  $forward_date_week = date_week($forward_date->format('Y-m-d'));
  if ($forward_date_week < 10) {
    $forward_date_week = '0' . $forward_date_week;
  }
  $backward_date_week = date_week($backward_date->format('Y-m-d'));
  if ($backward_date_week < 10) {
    $backward_date_week = '0' . $backward_date_week;
  }

  // Create links.
  $forward_path = STUDYROOM_SPACE_PATH . $space->space_id . '/' . $forward_date->format('Y') . '/' . $forward_date_week;
  $nav['links']['next'] = l(t('Next Week'), $forward_path);

  $backward_path = STUDYROOM_SPACE_PATH . $space->space_id . '/' . $backward_date->format('Y') . '/' . $backward_date_week;
  $nav['links']['prev'] = l(t('Previous Week'), $backward_path);

  $current_path = STUDYROOM_SPACE_PATH . $space->space_id . '/' . $current->format('Y')  . '/' . $current_week;
  $nav['links']['current'] = l(t('Current Week'), $current_path);

  $variables['page']['navigation'] = theme('studyroom_availability_week_navigation', $nav);
  $variables['week_days'] = $availability->week_days;
}

/**
 * Generate the navigation for the availability week table.
 */
function theme_studyroom_availability_week_navigation($content) {
  $nav = array();
  $nav['#prefix'] = '<div class="navigation">';
  $nav['#suffix'] = '</div>';

  foreach ($content['links'] as $key => $link) {
    $nav[$key]['#type'] = 'markup';
    $nav[$key]['#markup'] = $link;
    $nav[$key]['#prefix'] = '<span class="' . $key . '-url">';
    $nav[$key]['#suffix'] = '</span>';
  }

  return $nav;
}
