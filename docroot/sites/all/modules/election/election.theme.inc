<?php
/**
 * @file
 * Theme and preprocess functions for the Election module.
 */

/**
 * Theme function for displaying a piece of information which isn't a field.
 */
function theme_election_info_item($variables) {
  $label = $variables['label'];
  $value = $variables['value'];
  $html = $variables['html'];
  $classes = $variables['classes'];
  $children = $variables['children'] ? $variables['children'] : array();
  $classes[] = 'election-info';
  if (!empty($label)) {
    $classes[] = drupal_html_class('election-info-' . $label);
  }
  $content = '';
  if ($label) {
    $content .= '<span class="label">' . check_plain($label) . ':</span> ';
  }
  $content .= '<span class="value">'
    . ($html ? $value : check_plain($value))
    . drupal_render_children($children)
    . '</span>';
  $container = array(
    '#theme' => 'container',
    '#children' => $content,
    '#attributes' => array('class' => $classes),
  );
  return drupal_render($container);
}

/**
 * Determine the election's status, for theme_election_status().
 */
function template_preprocess_election_status(&$variables) {

  $election = $variables['election'];
  $status_prefix = $variables['status_prefix'];
  $short = $variables['short'];

  $status = $election->{$status_prefix . 'status'};
  $open_time = $election->{$status_prefix . 'open_time'};
  $close_time = $election->{$status_prefix . 'close_time'};

  // Provide Boolean variables for open, scheduled and future.
  $variables['open'] = _election_is_open($status, $open_time, $close_time);
  $variables['scheduled'] = ($status == ELECTION_STATUS_SCHEDULED);
  $variables['future'] = (REQUEST_TIME < $open_time);
  $variables['past'] = (REQUEST_TIME > $close_time);

  // Provide an array of CSS classes.
  $classes = array();
  if ($variables['open']) {
    $classes['open'] = 'election-status-open';
  }
  else {
    $classes['closed'] = 'election-status-closed';
  }
  if ($variables['scheduled']) {
    $classes['scheduled'] = 'election-status-scheduled';
    if ($variables['future']) {
      $classes['future'] = 'election-status-future';
    }
    elseif ($variables['past']) {
      $classes['past'] = 'election-status-past';
    }
  }

  $type = (empty($election->type_info)) ? _election_type_get_info($election->type) : $election->type_info;
  // Nominations aren't relevant to elections that don't have candidates.
  if ($status_prefix == 'n' && empty($type['has candidates'])) {
    $variables['status'] = t('N/A');
    $classes['na'] = 'election-status-na';
    unset($classes['closed']);
  }
  elseif ($variables['scheduled'] && $variables['future']) {
    if ($short) {
      $variables['status'] = t('In %interval', array(
        '%interval' => format_interval($open_time - REQUEST_TIME, 1),
      ));
    }
    else {
      $variables['status'] = t('From %date (in @interval)', array(
        '%date' => format_date($open_time),
        '@interval' => format_interval($open_time - REQUEST_TIME),
      ));
    }
  }
  elseif ($variables['scheduled'] && $variables['open']) {
    if ($short) {
      $variables['status'] = t('Scheduled (now open)');
    }
    else {
      $variables['status'] = t('Open until %date', array('%date' => format_date($close_time)));
    }
  }
  elseif ($variables['scheduled']) {
    $variables['status'] = t('Closed %interval ago', array(
      '%interval' => format_interval(REQUEST_TIME - $close_time, $short ? 1 : 2),
    ));
  }
  elseif ($variables['open']) {
    $variables['status'] = t('Open');
  }
  else {
    $variables['status'] = t('Closed');
  }

  $variables['classes'] = $classes;

}

/**
 * Display an election's voting or nominating status.
 *
 * Variables:
 *   $election      - An election object.
 *   $status_prefix - Type of status: 'v' = voting (default), 'n' = nominations.
 *   $status        - A string describing the status (sanitized).
 *   $open          - A Boolean indicating whether the status is open.
 *   $scheduled     - A Boolean indicating whether the status is scheduled.
 */
function theme_election_status($variables) {

  $attributes = array('class' => $variables['classes']);

  return '<span' . drupal_attributes($attributes) . '>'
    . $variables['status']
    . '</span>';

}

/**
 * Process variables for election.tpl.php.
 *
 * The $variables['elements'] array contains the following arguments:
 * - #election
 * - #view_mode
 *
 * @see election.tpl.php
 */
function template_preprocess_election(&$variables) {

  $election = $variables['elements']['#election'];
  $view_mode = $variables['elements']['#view_mode'];

  // Detect whether the election is on its own page.
  $page_election = menu_get_object('election');
  $is_page = (!empty($page_election) && $page_election->election_id == $election->election_id);

  // Create some basic useful variables.
  $variables['view_mode'] = $view_mode;
  $variables['teaser'] = ($view_mode == 'teaser');
  $variables['page'] = $is_page;
  $variables['election'] = $election;
  $variables['title'] = check_plain($election->title);
  $uri = election_uri($election);
  $variables['election_url'] = url($uri['path'], $uri['options']);

  // Flatten the $election object's member fields.
  $variables = array_merge((array) $election, $variables);

  // Create the $content variable to contain fields for templates.
  $variables += array('content' => array());
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('election', $election, $variables['content'], $variables);

  $variables['classes_array'][] = drupal_html_class('election-type-' . $election->type);
  if ($view_mode == 'teaser') {
    $variables['classes_array'][] = 'election-teaser';
  }
  if (!$election->published) {
    $variables['classes_array'][] = 'election-unpublished';
  }

  $variables['theme_hook_suggestions'][] = 'election__' . $election->type;
  $variables['theme_hook_suggestions'][] = 'election__' . $election->election_id;

}
