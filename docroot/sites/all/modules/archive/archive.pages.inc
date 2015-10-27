<?php
// $Id: archive.pages.inc,v 1.18.2.4.2.1 2010/06/16 07:10:25 rmuilwijk Exp $

/**
 * @file
 * Pages for the archive module.
 */

/**
 * Fetch nodes for the selected date, or current date if none selected.
 *
 * @param $year
 *   Number of year.
 * @param $month
 *   Number of month.
 * @param $day
 *   Number of day.
 * @return
 *   A string with the themed page.
 */
function archive_page($type = 'all', $year = 0, $month = 0, $day = 0) {

  // Make sure all values are secure.
  $day = (int) $day;
  $year = (int) $year;
  $month = (int) $month;
  if ($month < 0 || $month > 12) {
    // Ensure that we have a proper array index later.
    $month = 0;
  }
  if (!_archive_validate_type($type)) {
    $type = 'all';
  }

  drupal_set_title(theme('archive_page_title', array('type' => $type, 'year' => $year, 'month' => $month, 'day' => $day)));

  $date = _archive_date($type, $year, $month, $day);
  $query = _archive_query($type, $date);
  $nodes = variable_get('default_nodes_main', 10);
  $result = $query->extend('PagerDefault')->limit($nodes)->execute();
  $nodes = $query->countQuery()->execute()->fetchField();

  drupal_add_css(drupal_get_path('module', 'archive') .'/archive.css');
  $output = theme('archive_navigation', array('type' => $type, 'date' => $date));

  if (!$nodes && $type == 'all') {
    $output .= t('No content found.');
    return $output;
  }

  $found_rows = FALSE;
  $node_date = 0;
  foreach ($result as $o) {
    $node = node_load($o->nid);
    $node_created = $node->created + $date->tz;
    // Determine which separators are needed
    $separators = array('year'  => 0,
                        'month' => 0,
                        'day'   => 0);
    if (!$year) {
      $created_year = format_date($node_created, 'custom', 'Y');
      $last_year = format_date($node_date, 'custom', 'Y');
      if ($created_year != $last_year) {
        $separators['year'] = 1;
      }
    }
    // Print month separaters
    if (!$month) {
      $created_month = format_date($node_created, 'custom', 'n');
      $last_month = format_date($node_date, 'custom', 'n');
      if ($created_month != $last_month) {
        $separators['month'] = 1;
      }
    }
    // Print day separaters
    if (!$day) {
      $created_day = format_date($node_created, 'custom', 'j');
      $last_day = format_date($node_date, 'custom', 'j');
      if ($created_day != $last_day) {
        $separators['day'] = 1;
      }
    }
    $output .= theme('archive_separator', array('date_created' => $node_created, 'separators' => $separators));
    $output .= drupal_render(node_view($node, 'teaser'));

    $found_rows = TRUE;
    $node_date = $node->created + $date->tz;
  }
  if ($found_rows) {
    $output .= theme('pager');
  }
  // Handle URLs that are incorrectly typed and try to parse info out of them
  else {
    if ($date->days[$date->day]) {
      drupal_goto(_archive_url($type, $date->year, $date->month, $date->day));
    }
    elseif ($date->months[$date->month]) {
      drupal_goto(_archive_url($type, $date->year, $date->month));
    }
    elseif ($date->years[$date->year]) {
      drupal_goto(_archive_url($type, $date->year));
    }
    else {
      drupal_goto(_archive_url($type));
    }
  }

  return $output;
}

/**
 * Builds an archive SQL query with its parameters for the specified date.
 *
 * @param $date
 *   A date object obtained from _archive_date().
 * @return
 *   An array of (query, param_start, param_end).
 */
function _archive_query($type, $date) {
  // Confine the display interval to only one day
  if ($date->day) {
    $start = mktime(0, 0, 0, $date->month, $date->day, $date->year);
    $end   = mktime(0, 0, 0, $date->month, $date->day + 1, $date->year);
  }
  // Confine the display interval to one month
  elseif ($date->month) {
    $start = mktime(0, 0, 0, $date->month, 1, $date->year);
    $end   = mktime(0, 0, 0, $date->month + 1, 1, $date->year);
  }
  // Confine the display interval to one year
  elseif ($date->year) {
    $start = mktime(0, 0, 0, 1, 1, $date->year);
    $end   = mktime(0, 0, 0, 1, 1, $date->year + 1);
  }
  else {
    $start = 0;
    $end = 0;
  }

  // Grab limits on node types if exist
  $final_types = _archive_types_sql_array($type);

  // Allow viewing all nodes, not just nodes by year
  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'type'));
  $query->condition('n.status', 1);
  $query->orderBy('n.created', 'DESC');
  $query->condition('n.type', $final_types, 'IN');
  $query->addTag('node_access');

  if ($start && $end) {
    $query->condition('n.created', $start - $date->tz, '>=');
    $query->condition('n.created', $end - $date->tz, '<=');
  }

  return $query;
}

/**
 * Returns the different node types that have nodes.
 *
 * @param $date
 *   A date object obtained from _archive_date().
 * @return
 *   An array of node-types to number of posts of that type.
 */
function _archive_node_types($date) {

  $types = variable_get('archive_type_filters', array());

  // Confine the display interval to only one day
  if ($date->day) {
    $start = mktime(0, 0, 0, $date->month, $date->day, $date->year);
    $end   = mktime(0, 0, 0, $date->month, $date->day + 1, $date->year);
  }
  // Confine the display interval to one month
  elseif ($date->month) {
    $start = mktime(0, 0, 0, $date->month, 1, $date->year);
    $end   = mktime(0, 0, 0, $date->month + 1, 1, $date->year);
  }
  // Confine the display interval to one year
  elseif ($date->year) {
    $start = mktime(0, 0, 0, 1, 1, $date->year);
    $end   = mktime(0, 0, 0, 1, 1, $date->year + 1);
  }
  else {
    $start = 0;
    $end = 0;
  }

  // Setup the query.
  $query = db_select('node', 'n');
  $query->innerJoin('node_type', 't', 't.type = n.type');
  $query->fields('t', array('type', 'name'));
  $query->addExpression('COUNT(n.nid)', 'node_count');
  $query->condition('n.status', 1);
  $query->condition('t.type', $types, 'IN');
  $query->groupBy('n.type');
  $query->orderBy('n.created', 'ASC');
  $query->addTag('node_acces');

  if ($start && $end) {
    $query->condition('n.created', array($start - $date->tz, $end - $date->tz), 'BETWEEN');
  }
  $result = $query->execute();


  $n_types = array();
  foreach ($result as $row) {
    $n_types[$row->type] = array('count' => $row->node_count,
                                 'name'  => $row->name);
  }

  ksort($n_types);
  return $n_types;
}

/**
 * Theme function for the title of the archive page.
 *
 * @param $variables
 *   An associative array containing:
 *   - type: The node type for the archive page.
 *   - year: The year of the archivepage.
 *   - month: The month of the archive page.
 *   - day: The day of the archive page.
 *
 * @ingroup themeable
 */
function theme_archive_page_title($variables) {
  $title = t('Archive');
  $month_names = array('', 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec');
  if ($variables['day']) {
    $title .= ' - '. t($month_names[$variables['month']]) .' '. $variables['day'] .', '. $variables['year'];
  }
  elseif ($variables['month']) {
    $title .= ' - '. t($month_names[$variables['month']]) .' '. $variables['year'];
  }
  elseif ($variables['year']) {
    $title .= ' - '. $variables['year'];
  }

  if ($variables['type'] != 'all') {
    $type_name = db_query("SELECT name FROM {node_type} WHERE type = '%s'", $variables['type'])->fetchField();

    $title .= ' - '. ($type_name ? t($type_name) : $variables['type']);
  }

  return $title;
}

/**
 * Theme the archive navigation with years, months and dates by default.
 *
 * @ingroup themeable
 */
function theme_archive_navigation($variables) {
  $output  = "<div id=\"archive-container\"><dl><dt>". t('Date') ."</dt><dd>\n";
  $output .= theme('archive_navigation_years', array('type' => $variables['type'], 'date' => $variables['date']));
  if (_archive_validate_date($variables['date']->year)) {
    $output .= theme('archive_navigation_months', array('type' => $variables['type'], 'date' => $variables['date']));
  }
  if (_archive_validate_date($variables['date']->year, $variables['date']->month)) {
    $output .= theme('archive_navigation_days', array('type' => $variables['type'], 'date' => $variables['date']));
  }
  $output .= "</dd>";

  // Only display node type filter if more than one node type represented
  if (sizeof(_archive_node_types($variables['date'])) > 1) {
    $output .= "<dt>". t('Type') ."</dt><dd>\n";
    $output .= theme('archive_navigation_node_types', array('type' => $variables['type'], 'date' => $variables['date']));
    $output .= "</dd>";
  }
  $output .= "</dl></div>\n";
  return $output;
}

/**
 * Theme the list of years for the archive navigation.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_years($variables) {
  $output = "<ul id=\"archive-years\">\n";

  $all_count = 0;
  foreach ($variables['date']->years as $year_count) {
    $all_count += $year_count;
  }

  $output .= '<li'. ($variables['date']->year?'':' class="selected"') .'>'. l(t('All'), _archive_url($variables['type']), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  foreach ($variables['date']->years as $year => $year_count) {
    $class = '';
    if ($year == $variables['date']->year) {
      $class = ' class="selected"';
    }
    $output .= '<li'. $class .'>'. l($year, _archive_url($variables['type'], $year), array('attributes' => array('title' => format_plural($year_count, '1 post', '@count posts')))) ."</li>\n";
  }
  $output .= "</ul>\n";

  return $output;
}

/**
 * Theme the list of months for the archive navigation.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_months($variables) {
  $output = "<ul id=\"archive-months\">\n";

  $all_count = 0;
  foreach ($variables['date']->months as $month) {
    $all_count += $month;
  }

  $output .= '<li'. ($variables['date']->month?'':' class="selected"') .'>'. l(t('All'), _archive_url($variables['type'], $variables['date']->year), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  $curr_month = format_date(time(), 'custom', 'n');
  $curr_year = format_date(time(), 'custom', 'Y');
  foreach (range(1, 12) as $month) {
    $posts = !empty($variables['date']->months[$month]) ? $variables['date']->months[$month] : 0;
    $class = '';
    if ($month == $variables['date']->month) {
      $class = ' class="selected"';
    }
    elseif ($curr_year == $variables['date']->year && $month > $curr_month) {
      $class = ' class="future"';
    }
    $month_names = array('', 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec');
    $output .= "<li$class>". ($posts > 0 ? l(t($month_names[$month]), _archive_url($variables['type'], $variables['date']->year, $month), array('attributes' => array('title' => format_plural($posts, "1 post", "@count posts")))) : t($month_names[$month])) ."</li>\n";
  }
  $output .= "</ul>\n";
  return $output;
}

/**
 * Theme the list of days for the archive navigation.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_days($variables) {
  $output = "<ul id=\"archive-days\">\n";

  $all_count = 0;
  foreach ($variables['date']->days as $day) {
    $all_count += $day;
  }

  $output .= '<li'. ($variables['date']->day?'':' class="selected"') .'>'. l(t('All'), _archive_url($variables['type'], $variables['date']->year, $variables['date']->month), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  $curr_month = format_date(time(), 'custom', 'n');
  $curr_year = format_date(time(), 'custom', 'Y');
  $curr_day = format_date(time(), 'custom', 'j');

  $day_stop = gmdate('t', gmmktime(0, 0, 0, $variables['date']->month, 1, $variables['date']->year));
  for ($day = 1; $day <= $day_stop; $day++) {
    $posts = array_key_exists($day, $variables['date']->days) ? $variables['date']->days[$day] : 0;
    $class = '';
    if ($day == $variables['date']->day) {
      $class = ' class="selected"';
    }
    elseif ($curr_year == $variables['date']->year && $curr_month == $variables['date']->month && $day > $curr_day) {
      $class = ' class="future"';
    }
    $output .= "<li$class>". ($posts ? l($day, _archive_url($variables['type'], $variables['date']->year, $variables['date']->month, $day), array('attributes' => array("title" => format_plural($posts, "1 post", "@count posts")))) : $day) ."</li>\n";
  }
  $output .= "</ul>\n";
  return $output;
}

/**
 * Theme the list of node types for the archives.
 *
 * @ingroup themeable
 */
function theme_archive_navigation_node_types($variables) {
  $output = "<ul id=\"archive-node_types\">\n";
  $types_count = _archive_node_types($variables['date']);

  $all_count = 0;
  foreach ($types_count as $t) {
    $all_count += $t['count'];
  }

  $output .= '<li'. ($variables['type'] && $variables['type'] != 'all'?'':' class="selected"') .'>'. l(t('All'), _archive_url('all', $variables['date']->year, $variables['date']->month, $variables['date']->day), array('attributes' => array('title' => format_plural($all_count, '1 post', '@count posts')))) ."</li>\n";
  foreach ($types_count as $ft_key => $ft_value) {
    if (!$ft_value['count']) {
      continue;
    }
    $class = ($ft_key == $variables['type'] ? ' class="selected"' : '');
    $name = $ft_value['name'];
    if ($types_count[$ft_key]['count'] > 0) {
      $output .= "<li$class>". l($name, _archive_url($ft_key, $variables['date']->year, $variables['date']->month, $variables['date']->day), array('attributes' => array("title" => format_plural($types_count[$ft_key]['count'], "1 post", "@count posts")))) ."</li>\n";
    }
    else {
      $output .= "<li$class>$name</li>\n";
    }
  }
  $output .= "</ul>\n";
  return $output;
}

/**
 * Theme the date separators between nodes of different year/month/day.
 *
 * @param $date_created
 *   A UNIX timestamp.
 * @param $separators
 *   An array with 'year', 'month', and 'day' keys. A value of 1 for any
 *   of those keys means a transition for that unit of time.
 *   Ex. array('year' => 0, 'month' => 1, 'day' => 1)
 *       ^  Means the month has transitioned
 *
 * @ingroup themeable
 */
function theme_archive_separator($variables) {
  $date_sep = '';
  if ($variables['separators']['year'] && $variables['separators']['month'] && $variables['separators']['day']) {
    $date_sep = format_date($variables['date_created'], 'custom', 'F jS, Y');
  }
  elseif ($variables['separators']['month'] && $variables['separators']['day']) {
    $date_sep = format_date($variables['date_created'], 'custom', 'F jS');
  }
  elseif ($variables['separators']['day']) {
    $date_sep = format_date($variables['date_created'], 'custom', 'F jS');
  }

  return '<h3>'. $date_sep .'</h3>';
}
