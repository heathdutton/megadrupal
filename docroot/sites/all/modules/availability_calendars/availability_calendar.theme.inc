<?php
/**
 * Availability Calendar theming code.
 * - Theme calendar field:
 *   - straight.
 *   - in a viewport.
 * - Theme a number of months of a calendar.
 * - Theme 1 month of a calendar.
 * - Theme the key of availability states.
 */

module_load_include('inc', 'availability_calendar');

/**
 * Themes the availability calendar field.
 *
 * @param array $variables
 *
 * @return string
 */
function theme_availability_calendar($variables) {
  availability_calendar_add_full_calendar_js($variables);
  $output = '';
  $output .= "<div id=\"cal-view-{$variables['cvid']}\" class=\"cal-{$variables['cid']} cal clearfix\">\n";
  if ($variables['settings']['add_name'] == '1' && !empty($variables['name'])) {
    $output .= '<div class="field-label">' . $variables['name'] . ':&nbsp;</div>' . "\n";
  }
  $output .= theme('availability_calendar_months', $variables);
  $output .= "</div>\n";
  return $output;
}

/**
 * Themes the availability calendar field.
 *
 * @param array $variables
 *
 * @return string
 */
function theme_availability_calendar_colorbox($variables) {
  availability_calendar_add_full_calendar_js($variables);
  $colorbox_settings = $variables['settings']['colorbox'];
  $urlParams = '?inline=true';
  if (!empty($colorbox_settings['width'])) {
    $urlParams .= '&width=' .  check_plain($colorbox_settings['width']);
  }
  if (!empty($colorbox_settings['height'])) {
    $urlParams .= '&height=' .  check_plain($colorbox_settings['height']);
  }
  $link_text = availability_calendar_get_customizable_text('availability_calendar_colorbox_link_text');
  $link_title = availability_calendar_get_customizable_text('availability_calendar_colorbox_link_title');
  if ($link_title != '') {
    $link_title = " title='$link_title'";
  }
  $output = '';
  $output .= "<div><a class='colorbox-inline' href='$urlParams#cal-view-{$variables['cvid']}'$link_title>$link_text</a></div>\n";
  $output .= "<div style='display: none;'>";
  $output .= theme('availability_calendar', $variables);
  $output .= "</div>\n";
  return $output;
}

/**
 * Themes the availability calendar field in a viewport.
 *
 * @param array $variables
 *
 * @return string
 */
function theme_availability_calendar_viewport($variables) {
  // Add the necessary js for the viewport functionality.
  availability_calendar_add_viewport_js($variables);
  $output = '';

  // Theme the buttons.
  $viewport_settings = $variables['settings']['viewport'];
  $placement = $viewport_settings['button_placement'];
  if ($placement !== 'not') {
    $responsive = $viewport_settings['dimensions_calculation'] === 'responsive_block' || $viewport_settings['dimensions_calculation'] === 'responsive_inline';
    $scroll = (int) ($viewport_settings['rows'] > 1 ? $viewport_settings['scroll'] * $viewport_settings['cols'] : $viewport_settings['scroll']);
    $button_back = theme('availability_calendar_viewport_button', array('direction' => 'back', 'scroll' => $scroll, 'responsive' => $responsive));
    $button_forward = theme('availability_calendar_viewport_button', array('direction' => 'forward', 'scroll' => $scroll, 'responsive' => $responsive));
  }
  else {
    $button_back = '';
    $button_forward = '';
  }

  // Theme the calendar in its viewport and place the buttons before and/or
  // after it.
  $output .= "<div id=\"cal-view-{$variables['cvid']}\" class=\"cal-{$variables['cid']} cal clearfix\">\n";
  if ($variables['settings']['add_name'] == '1' && !empty($variables['name'])) {
    $output .= '<div class="field-label">' . $variables['name'] . ':&nbsp;</div>' . "\n";
  }
  if ($placement === 'before') {
    $output .= "<div class=\"cal-buttons\">$button_back $button_forward</div>";
  }
  else if ($placement === 'before_after') {
    $output .= "<div class=\"cal-buttons cal-buttons-before\">$button_back</div>";
  }
  $output .= '<div class="cal-viewport"><div class="cal-viewport-inner">' . "\n";
  $output .= theme('availability_calendar_months', $variables);
  $output .= "</div></div>\n";
  if ($placement === 'before_after') {
    $output .= "<div class=\"cal-buttons cal-buttons-after\">$button_forward</div>";
  }
  else if ($placement === 'after') {
    $output .= "<div class=\"cal-buttons\">$button_back$button_forward</div>";
  }
  $output .= "</div>\n";
  return $output;
}

/**
 * Themes a button to scroll an availability calendar field within its viewport.
 *
 * @param array $variables
 *
 * @return string
 */
function theme_availability_calendar_viewport_button($variables) {
  $direction = $variables['direction'] === 'forward' ? 'forward' : 'backward';
  $months = (int) $variables['scroll'];
  $responsive = $variables['responsive'];
  $key_part1 = $direction === 'backward' ? 'prev' : 'next';
  $key_part2 = $months === 1 ? '1' : ($responsive ? 'unknown' : 'n');
  $key_start = "availability_calendar_viewport_button_{$key_part1}_{$key_part2}_";
  $button_text = availability_calendar_get_customizable_formatted_text($key_start . 'text', array('@count' => $months));
  $button_title = availability_calendar_get_customizable_title($key_start . 'title');
  $output = "<button type=\"button\" class=\"cal-$direction\"$button_title>$button_text</button>";
  return $output;
}

/**
 * Implements hook_process_HOOK for theme availability_calendar
 * @link http://api.drupal.org/api/drupal/modules--system--theme.api.php/function/hook_process_HOOK/7
 *
 * This process function adds:
 * - year (if not set)
 * - month (if not set)
 * - availability for all months to show
 */
function availability_calendar_process_availability_calendar_months(&$variables) {
  if (empty($variables['year'])) {
    $variables['year'] = (int) date('Y');
  }
  if (empty($variables['month'])) {
    $variables['month'] = (int) date('n');
  }
  if (empty($variables['availability'])) {
    $months_to_render = $variables['settings']['show_number_of_months'];
    $from = new DateTime();
    $from->setDate($variables['year'], $variables['month'], 1);
    $to = clone $from;
    $to->modify("+$months_to_render months");
    $to->modify('-1 day');
    if ($variables['settings']['allocation_type'] === AC_ALLOCATION_TYPE_OVERNIGHT
     && $variables['settings']['show_split_day']) {
      // We may want to show the state of the first morning of the first month.
      // And we need the state from the last day of the previous month for that.
      $from->modify('-1 day');
    }
    // Convert new cid's to 0.
    $variables['availability'] = availability_calendar_get_availability((int) $variables['cid'], $from, $to, $variables['settings']['default_state']);
  }
}

/**
 * Themes a number of months of an availability calendar field.
 *
 * @param array $variables
 *
 * @return string
 */
function theme_availability_calendar_months($variables) {
  $year = &$variables['year'];
  $month = &$variables['month'];
  $months_to_render = $variables['settings']['show_number_of_months'];

  $output = '';
  for($i = 0; $i < $months_to_render; $i++) {
    $output .= theme('availability_calendar_month', $variables);
    $month++;
    if ($month > 12) {
      $month = 1;
      $year++;
    }
  }
  return $output;
}

/**
 * Themes the calendar for a given month.
 *
 * @param array $variables
 *
 * @return string
 */
function theme_availability_calendar_month($variables) {
  $year = $variables['year'];
  $month = $variables['month'];
  $settings = $variables['settings'];
  $availability = $variables['availability'];

  // Get the parts of the table.
  $caption = format_date(mktime(12, 0, 0, $month, 1, $year), 'availability_calendar_month_caption');
  $header = availability_calendar_month_header($settings);
  $cells = availability_calendar_month_cells($year, $month, $availability, $settings);
  $rows = array_chunk($cells, 7, TRUE);
  // Ensure that each month has 6 rows. The last rows can be completely empty.
  while (count($rows) < 6) {
    $rows[] = array(array('data' => '<div></div>', 'class' => array('cal-empty'), 'colspan' => 7));
  }

  // Prepend week numbers: we work with ISO-8601 week numbers that assume that
  // a week starts on a monday. If the first_day_of_week is a friday, saturday,
  // or sunday, we take the week number of the last day in that row, otherwise
  // we take the week number of the first day in that row.
  if ($settings['show_week_number']) {
    array_unshift($header, array('data' => t('Nr.'), 'class' => array('cal-weekno-header')));
    foreach ($rows as &$row) {
      if (count($row) > 1) {
        if ($settings['first_day_of_week'] < 5) {
          reset($row);
        }
        else {
          end($row);
        }
        $day_to_use = new DateTime(key($row));
        array_unshift($row, array('data' => $day_to_use->format('W'), 'header' => TRUE));
      }
      else {
        array_unshift($row, array('data' => '&nbsp;', 'header' => TRUE, 'class' => array('cal-empty')));
      }
    }
  }

  // Prevent the striping that most themes add.
   foreach ($rows as &$row) {
    $row = array('no_striping' => TRUE, 'data' => $row);
  }

  // Theme the table and wrap it to allow for better styling.
  $month2 = sprintf('%02d', $month);
  $output = "<div class=\"cal-month\" data-cal-id=\"{$variables['cid']}\" data-cal-year=\"$year\" data-cal-month=\"$month2\">\n";
  $output .= theme('table', array('caption' => $caption, 'header' => $header, 'rows' => $rows, 'sticky' => FALSE));
  $output .= "\n</div>\n";
  return $output;
}

/**
 * Helper function that returns the header row containing the names of the days.
 *
 * The order depends on the setting 'first_day_of_week'. The output is either
 * the abbreviated day name or just the first letter of that, depending on the
 * setting 'show_only_first_letter'
 *
 * @param array $settings
 *
 * @return array
 */
function availability_calendar_month_header($settings) {
  // Index of this array is the ISO-8601 numeric representation of the day of
  // the week modulo 7.
  $day_names_abbr = array(t('Sun'), t('Mon'), t('Tue'), t('Wed'), t('Thu'), t('Fri'), t('Sat'));
  // Container for header row.
  $headers = array();
  for ($i = 0; $i < 7; $i++) {
    $header = $day_names_abbr[($settings['first_day_of_week'] + $i) % 7];
    if ($settings['show_only_first_letter']) {
      $header = substr($header, 0, 1);
    }
    $headers[] = $header;
  }
  return $headers;
}

/**
 * Helper function that returns the cells for a month table.
 *
 * The result will be a 1-dimensional array with
 * - A cell for each day of the month.
 * - Prepended by a cell for days of the previous month to start at the
 *   configured first day of the week.
 * - Appended with days of the next month to arrive at a whole number of weeks.
 * - Appended with empty weeks to let all calendars have 6 weeks.
 *
 * @param int $year
 * @param int $month
 * @param array $availability
 * @param array $settings
 *
 * @return array
 */
function availability_calendar_month_cells($year, $month, $availability, $settings) {
  // Date arithmetic.
  // Number of days in the month.
  $day = new DateTime();
  $day->setDate($year, $month, 1);
  $days_in_month = (int) $day->format('t');
  // Number of days (from the previous month) to add to first row.
  $days_to_prepend = (int) $day->format('N') - $settings['first_day_of_week'];
  if ($days_to_prepend < 0) {
    $days_to_prepend += 7;
  }
  // Number of days (of the next month) to append to the last row (with dates).
  $days_to_append = ($days_to_prepend + $days_in_month) % 7;
  if ($days_to_append > 0) {
    $days_to_append = 7 - $days_to_append;
  }

  // Show split states?
  $showSplitDays = $settings['allocation_type'] === AC_ALLOCATION_TYPE_OVERNIGHT && $settings['show_split_day'];

  // We are going to create an array with all days we want to show.
  $states = availability_calendar_get_states();
  $total_days = $days_to_prepend + $days_in_month + $days_to_append;
  $today_iso = date(AC_ISODATE);
  $day->modify('-' . ($days_to_prepend + 1) . ' days');
  $previous_day_iso = $day->format(AC_ISODATE);
  $day->modify('+1 day');
  $cells = array();
  for($i = 0; $i < $total_days; $i++) {
    $day_iso = $day->format(AC_ISODATE);
    $classes = array();
    if (empty($availability[$day_iso]) || (int) $day->format('n') != $month) {
      $classes[] = 'cal-other';
    }
    else if ($day_iso < $today_iso) {
      $classes[] = 'cal-pastdate';
    }
    else {
      // Special state for today and don't show yesterday's state as that is a past date.
      if ($day_iso == $today_iso) {
        $classes[] = 'cal-today';
        if ($showSplitDays) {
          $classes[] = 'cal-pastdate-am';
        }
      }
      else if ($showSplitDays && isset($availability[$previous_day_iso])) {
        $classes[] = $states[$availability[$previous_day_iso]]['css_class'] . '-am';
      }

      $state = $states[$availability[$day_iso]]['css_class'];
      if ($showSplitDays) {
        $state .= '-pm';
      }
      $classes[] = $state;
    }
    $cell_contents = availability_calendar_theme_day((int) $day->format('j'), $settings);
    $cells[$day_iso] = array('data' => $cell_contents, 'class' => $classes);
    $day->modify('+1 day');
    $previous_day_iso = $day_iso;
  }
  return $cells;
}

/**
 * Helper function that returns the html for 1 day.
 *
 * Because this function is called very often it is called directly and is thus
 * not a real theme function.
 *
 * @param int $day
 *   The day number.
 * @param array $settings
 * @return string
 */
function availability_calendar_theme_day($day, $settings) {
  // Represent whole days with 1 div, split days with 2 span's.
  // This allows to generate all css at once and then just switch settings
  if ($settings['allocation_type'] === AC_ALLOCATION_TYPE_OVERNIGHT && $settings['show_split_day']) {
    // Split day:
    // - The border-color of the outer span will take care of the coloring.
    //    the outer span will have zero sized content and full sized borders.
    // - The inner span will contain the day number and the selectable border
    //   It will be positioned absolute, half way up and left.
    // However, to be able to position it absolute, it needs to be within
    // another positioned element, and <td>s cannot be positioned.
    $result = "<span><span>$day</span></span>";
  }
  else {
    // Whole day, the background and border color will take care of the
    // coloring. Border color may change on hover for "selectable" cells.
    $result = "<div>$day</div>";
  }
  return $result;
}

/**
 * Themes the key for our calendars.
 *
 * @param array $variables
 * @return string
 */
function theme_availability_calendar_key($variables) {
  $caption = '';
  $rows = array();
  $states_to_show = isset($variables['states_to_show']) ? $variables['states_to_show'] : array();
  $states = availability_calendar_get_states(array_filter($states_to_show));
  foreach ($states as $state) {
    $rows[] = array(
      // Use the same classes here as in the calendar, so it styles the same.
      array(
        'data' => '<div>' . check_plain(t($state['label'])) . '</div>',
        'class' => $state['css_class'],
        'no_striping' => TRUE,
      ),
    );
  }
  $key = theme('table', array('caption' => $caption, 'rows' => $rows, 'sticky' => FALSE));
  return '<div class="cal-key">' . $key . '</div>';
}

/**
 * Adds the necessary javascript for a full calendar if it is interactive
 * (full in the sense of not placed in a viewport).
 *
 * When theming the widget (the user is going to edit the calendar), it should
 * always be interactive ('selectable' will not be set in this case).
 *
 * @param array $variables
 *   Array with arguments passed to the theme.
 */
function availability_calendar_add_full_calendar_js($variables) {
  if (!isset($variables['settings']['selectable']) || $variables['settings']['selectable']) {
    availability_calendar_add_calendar_view_js($variables['cvid'], $variables['cid'], $variables['name'], $variables['settings']);
  }
}

/**
 * Adds the necessary javascript to be able to show an (interactive) calendar
 * in a viewport.
 *
 * @param array $variables
 *   Array with arguments passed to the theme.
 *
 * @return string
 *   The element id to use for this view.
 */
function availability_calendar_add_viewport_js($variables) {
  $cvid = $variables['cvid'];
  availability_calendar_add_calendar_view_js($cvid, $variables['cid'], $variables['name'], $variables['settings']);
  drupal_add_js(drupal_get_path('module', 'availability_calendar') . '/availability_calendar.viewport.js');
  drupal_add_js(array(
      'availabilityCalendar' => array(
        'viewports' => array(
          $cvid => array(
            'cvid' => $cvid,
            'dimensionsCalculation' => $variables['settings']['viewport']['dimensions_calculation'],
            'cols' => (int) $variables['settings']['viewport']['cols'],
            'rows' => (int) $variables['settings']['viewport']['rows'],
            'scroll' => (int) $variables['settings']['viewport']['scroll'],
    )))), array('type' => 'setting'));
}
