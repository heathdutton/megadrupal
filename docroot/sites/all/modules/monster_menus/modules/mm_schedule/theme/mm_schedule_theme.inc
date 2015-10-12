<?php

/**
 * @file
 * Calendar theme functions
 */

function _mm_schedule_preprocess_calendar(&$vars) {
  $view = $vars['view'];
  $vars['calendar_mmtid'] = isset($view->mm_calendar_mmtid) ? $view->mm_calendar_mmtid : (isset($view->mmtid) ? $view->mmtid : 0);
  if ($vars['can_add_event'] = $vars['calendar_mmtid'] && _mm_schedule_can_add_event($vars['calendar_mmtid'], $view)) {
    $add_param = array(
      'title' => t('+'),
      'attributes' => array(
        'title' => t('Add a new event on this day'),   // Modify in the template, if needed.
      ),
      'href' => 'mm/' . $vars['calendar_mmtid'] . '/contents/add/' . $view->date_info->calendar_date_link,
      'query' => array(
        'destination' => 'mm/' . $vars['calendar_mmtid'] . '?' . drupal_http_build_query(drupal_get_query_parameters($_GET, array('q'))),
      ),
    );

    $vars['add_vars'] = array(
      'start_date' => NULL,   // To be filled-in by template.
      'increment' => NULL,    // To be filled-in by template.
      'param' => $add_param,
    );
  }
  drupal_add_css(drupal_get_path('module', 'mm_schedule') . '/mm_schedule.css');
  $vars['calendar_type'] = $view->date_info->calendar_type;

  if (!empty($view->mm_node) && in_array($vars['calendar_type'], array('week', 'day'))) {
    $vars['sub_increment'] = $view->mm_node->field_wd_increment[LANGUAGE_NONE][0]['value'];
    if ($view->mm_major_increment == 'half') {
      $group_time = 30;
      if ($vars['sub_increment'] > 30) {
        $vars['sub_increment'] = 30;
      }
    }
    elseif ($view->mm_major_increment == 'hour') {
      $group_time = 60;
    }
    $vars['subdivisions'] = empty($group_time) ? 1 : $group_time / $vars['sub_increment'];

    // Stock value for first_time is the earliest event, but we want to consider
    // the setting on the node, which might be an earlier time.
    $start_mins = empty($vars['first_time']) ? 24 * 60 : substr($vars['first_time'], 0, 2) * 60 + substr($vars['first_time'], 3, 2);
    $date = new DateTime('today');
    date_modify($date, '+' . min($start_mins, $view->mm_node->field_wd_start[LANGUAGE_NONE][0]['value']) . ' minutes');
    // Round down to nearest hour.
    $vars['first_time'] = date_format($date, 'H') . ':00:00';

    // Find the highest end time of an event.
    $vars['last_time'] = '00:00:00';
    foreach ($vars['items'] as $start_time => &$bucket) {
      $start_minute = intval(substr($start_time, 3, 2));
      foreach ($bucket['values'] as &$day_bucket) {
        foreach ($day_bucket as &$item) {
          $date = clone($item['item']->date_end);
          // Round up to nearest hour.
          date_modify($date, '+59 minutes');
          $end = date_format($date, 'H');
          $end = ($end == '00' ? '24' : $end) . ':00:00';
          if ($end > $vars['last_time']) {
            $vars['last_time'] = $end;
          }

          // Adjust class based on our own idea of time increment.
          if (!empty($group_time)) {
            $offset = round((date_format($item['item']->date_start, 'i') - $start_minute) / $vars['sub_increment'] * 2);
            $duration = round(($item['item']->date_end->format('U') - $item['item']->date_start->format('U')) / $vars['sub_increment'] / 60 * 2);
            $item['class'] = 'd_' . $duration . ' o_' . $offset . ' i_' . $item['indent'] . ' md_' . min($item['max_depth'], 5);
          }
        }
      }
    }
    // See if the node setting is greater than the highest end time.
    $date = new DateTime('today');
    date_modify($date, '+' . $view->mm_node->field_wd_end[LANGUAGE_NONE][0]['value'] . ' minutes');
    $end = date_format($date, 'H') . ':00:00';
    if ($end > $vars['last_time']) {
      $vars['last_time'] = $end;
    }
  }
}

function _mm_schedule_preprocess_calendar_datebox(&$vars) {
  $view = $vars['view'];
  $vars['calendar_mmtid'] = isset($view->mm_calendar_mmtid) ? $view->mm_calendar_mmtid : (isset($view->mmtid) ? $view->mmtid : 0);
  $vars['add_link'] = '';
  if ($vars['can_add_event'] = $vars['calendar_mmtid'] && _mm_schedule_can_add_event($vars['calendar_mmtid'], $view)) {
    $add_param = array(
      'title' => t('+'),
      'attributes' => array(
        'title' => t('Add a new event on this day'),
      ),
      'href' => 'mm/' . $vars['calendar_mmtid'] . '/contents/add/' . $view->date_info->calendar_date_link,
      'query' => array(
        'destination' => 'mm/' . $vars['calendar_mmtid'] . '?' . drupal_http_build_query(drupal_get_query_parameters($_GET, array('q'))),
      ),
    );

    $add_vars = array(
      'start_date' => date_create($vars['date']),
      'increment' => '+1 hour',
      'param' => $add_param,
    );
    $vars['add_link'] = theme('mm_schedule_add_link', $add_vars);
    drupal_add_css(drupal_get_path('module', 'mm_schedule') . '/mm_schedule.css');
  }
}

function mm_schedule_preprocess_calendar_week(&$vars) {
  // Allow a theme function to restyle the weekday headings.
  $date = clone $vars['view']->date_info->min_date;
  foreach (array_keys($vars['day_names']) as $day_index) {
    $vars['day_names'][$day_index]['data'] = theme('calendar_weekday', array('view' => $vars['view'], 'date' => $date));
    date_modify($date, '+1 day');
  }
}

function mm_schedule_preprocess_calendar_week_overlap(&$vars) {
  mm_schedule_preprocess_calendar_week($vars);
}

function _mm_schedule_preprocess_day(&$vars) {
  // Adjust the variables set for the day theme to work with the week theme.
  $rows =& $vars['rows'];
  $view = $vars['view'];
  $singleday_buckets = $multiday_buckets = array(array(), array(), array(), array(), array(), array(), array());

  foreach ($rows['all_day'] as $item) {
    $multiday_buckets[0][] = array(
      'item' => $item,
      'wday' => 0,
      'filled' => TRUE,
      'colspan' => 1,
      'entry' => theme('calendar_item', array('view' => $view, 'rendered_fields' => $item->rendered_fields, 'item' => $item)),
    );
  }

  foreach ($rows['items'] as $this_start_time => $items) {
    foreach ($items as $item) {
      if ($item->date_start < $view->date_info->min_date) {
        $item->date_start = $view->date_info->min_date;
      }

      $max = clone($view->date_info->min_date);
      date_modify($max, '+24 hour');
      if ($item->date_end > $max) {
        $item->date_end = $max;
      }

      $singleday_buckets[0][$this_start_time][] = array(
        'item' => $item,
        'wday' => 0,
        'entry' => theme('calendar_item', array('view' => $view, 'rendered_fields' => $item->rendered_fields, 'item' => $item)),
      );
    }
  }

  $rows = array(
    array(array('header' => 1, 'class' => 'foo', 'data' => 'dummy', 'header_id' => 'dummy')),
    'multiday_buckets' => $multiday_buckets,
    'singleday_buckets' => $singleday_buckets,
    'total_rows' => count($multiday_buckets[0]) + 1,
  );
}

function _mm_schedule_preprocess_date_views_pager(&$vars) {
  $view = $vars['plugin']->view;
  if (isset($view->mm_node) && $view->mm_node->type == 'mm_calendar') {
    $mmtid = $view->mmtid;
    $calendar_mmtid = isset($view->mm_calendar_mmtid) ? $view->mm_calendar_mmtid : $mmtid;

    if ($view->mm_display_mode == 'upcoming') {
      $vars['prev_url'] = $vars['next_url'] = '';
    }
    else {
      // For some reason, by the time we get here, Prev and Next are sometimes
      // absolute URLs. Since monster_menus_url_outbound_alter() won't parse
      // these, we have to make them relative again.
      if (!empty($vars['prev_url'])) {
        $vars['prev_url'] = preg_replace('{^https?://[^/]*?' . base_path() . '\.mm_calendar/}', '.mm_calendar/', $vars['prev_url']);
      }
      if (!empty($vars['next_url'])) {
        $vars['next_url'] = preg_replace('{^https?://[^/]*?' . base_path() . '\.mm_calendar/}', '.mm_calendar/', $vars['next_url']);
      }
    }

    $links = $selects = array();
    if (!empty($view->mm_node->field_allow_mode_change[LANGUAGE_NONE][0]['value'])) {
      // Year/Month/Week/Day
      if ($view->mm_display_mode != 'upcoming') {
        $date_info = clone $view->date_info;
        $now = date_now();
        $now_date = date_format($now, 'Y-m-d');
        $in_range = empty($date_info->min_date_date) || $now_date >= $date_info->min_date_date && $now_date <= $date_info->max_date_date;

        $options = array();
        $selected = '';
        $gran_names = date_granularity_names();
        $gran_names['week'] = t('Week', array(), array('context' => 'datetime'));
        foreach (array('day', 'week', 'month', 'year') as $granularity) {
          $date = $in_range ? $now : $date_info->orig_min_date;
          if ($granularity == 'year' && $view->mm_first_month > 1) {
            $offset = '-' . ($view->mm_first_month - 1) . ' month';
            date_modify($date, $offset);
          }
          $formatted = $granularity == 'week' ? date_format($date, 'Y') . '-W' . date_week(date_format($date, 'Y-m-d')) : date_format($date, date_granularity_format($granularity));
          $get = $_GET;
          $get['mm_calendar'] = $view->mm_display_mode . '/' . $formatted;
          $url = url("mm/$mmtid", array('query' => drupal_get_query_parameters($get, array('q')), 'absolute' => TRUE));
          $options[$url] = $gran_names[$granularity];
          if ($granularity == $view->mm_granularity) {
            $selected = $url;
          }
        }
        $selects[] = array(
          '#type' => 'select',
          '#title' => t('Period'),
          '#title_display' => 'invisible',
          '#options' => $options,
          '#value' => $selected,
        );
      }

      // Upcoming/List/Grid
      $options = array();
      $selected = '';
      $params = array(
        'upcoming' => t('Upcoming'),
        'list' => t('List'),
        'grid' => t('Grid'),
      );
      foreach ($params as $mode => $title) {
        $get = $_GET;
        $get['mm_calendar'] = join('/', array($mode, isset($get['mm_calendar']) ? preg_replace('{^[a-z]+(/|$)}', '', $get['mm_calendar']) : ''));
        $url = url("mm/$mmtid", array('query' => drupal_get_query_parameters($get, array('q')), 'absolute' => TRUE));
        $options[$url] = $title;
        if ($mode == $view->mm_display_mode) {
          $selected = $url;
        }
      }
      $selects[] = array(
        '#type' => 'select',
        '#title' => t('Mode'),
        '#title_display' => 'invisible',
        '#options' => $options,
        '#value' => $selected,
      );

      mm_static('mm_schedule_jump', TRUE);
    }

    // Export
    if (!empty($view->mm_node->field_show_export[LANGUAGE_NONE][0]['value'])) {
      $get = $_GET;
      $get['mm_calendar'] = join('/', array('ical', isset($get['mm_calendar']) ? preg_replace('{^[a-z]+(/|$)}', '', $get['mm_calendar']) : ''));
      $links[] = array(
        'title' => t('Export'),
        'href' => "mm/$mmtid",
        'query' => drupal_get_query_parameters($get, array('q')),
        'attributes' => array(
          'title' => t('Export this calendar view in iCal format'),
          'class' => array('schedule-link-export', 'schedule-link-space-before'),
          'rel' => 'nofollow',
        ),
      );
    }

    // Add event
    if (_mm_schedule_can_add_event($calendar_mmtid, $view)) {
      $query = array('destination' => "mm/$mmtid?" . drupal_http_build_query(drupal_get_query_parameters($_GET, array('q'))));
      if (!empty($_GET['mm_calendar'])) {
        $date = clone($view->date_info->min_date);
        date_modify($date, '+9 hours');
        $query['mm_event_start'] = date_format($date, DATE_FORMAT_ISO);
      }
      $links[] = array(
        'title' => t('Add event'),
        'href' => "mm/$calendar_mmtid/contents/add/" . $view->date_info->calendar_date_link,
        'query' => $query,
        'attributes' => array(
          'title' => t('Add a new event to this calendar'),
          'class' => array('schedule-link-add-event', 'schedule-link-space-before'),
        ),
      );
    }

    $out = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('container-inline mm-schedule-controls')),
      $selects,
      array(
        '#theme' => 'links',
        '#attributes' => array('class' => array('links', 'inline')),
        '#links' => $links,
      ),
    );

    $vars['pager_prefix'] = drupal_render($out);
    unset($view->feed_icon);
  }
}

/**
 * This function overrides theme_date_nav_title() in date/date_views/theme/theme.inc
 */
function _mm_schedule_date_nav_title($params) {
  $view = $params['view'];
  if (isset($view->mm_node)) {
    if ($view->mm_first_month > 1 && !empty($view->mm_granularity) && $view->mm_granularity == 'year') {
      if ($view->date_info->granularity == 'year' && $params['granularity'] == 'year') {
        // Main title on the calendar
        $year = date_format($view->date_info->min_date, 'Y');
        return "$year - " . ($year + 1);
      }
      elseif($view->date_info->granularity == 'month' && $params['granularity'] == 'month') {
        // Month names above each mini calendar:
        // First, add the offset.
        $offset = '+' . ($view->mm_first_month - 1) . ' month';
        $date = clone $view->date_info->min_date;
        date_modify($date, $offset);
        // Now generate the month name and link.
        $title = date_format_date($date, 'custom', 'F Y');
        $date_arg = date_format_date($date, 'custom', 'Y-m');
        $attributes = array('title' => t('View full page month'));
        $url = date_pager_url($view, NULL, $date_arg, TRUE);
        return l($title, $url, array('attributes' => $attributes));
      }
    }
    elseif ($view->mm_display_mode == 'upcoming') {
      // Use the title contained in the view
      return $view->display_handler->get_option('title');
    }
  }

  return theme_date_nav_title($params);
}

/* Overrides theme_jcalendar_view() in jcalendar/jcalendar.module */
function _mm_schedule_jcalendar_view($vars) {
  global $user;

  $node = $vars['node'];
  $output = node_view($node, 'teaser');
  $links = array();
  if (is_numeric($node->nid)) {
    $prefix = '';
    $query = array();
    $args = explode('.', $_GET['q']);
    if (!empty($args[5])) {
      $mmtid = $args[5];
      $mm_calendar = $args[6] . '/' . $args[7];
      $query = array('destination' => "mm/$mmtid?" . drupal_http_build_query(array('mm_calendar' => $mm_calendar)));
      $prefix = "mm/$mmtid/";
    }

    $links[] = array(
      'title' => t('Details'),
      'href' => $prefix . _mm_schedule_get_node_link($node),
      'query' => $query,
    );

    if (node_access('update', $node)) {
      $links[] = array(
        'title' => t('Edit'),
        'href' => "{$prefix}node/$node->nid/edit",
        'query' => $query,
      );
    }

    if (_mm_menu_access_node_delete($node)) {
      $links[] = array(
        'title' => mm_content_node_is_recycled($node) ? t('Delete permanently') : t('Delete'),
        'href' => "{$prefix}node/$node->nid/delete",
        'query' => $query,
      );
    }

    $is_signed_up = FALSE;
    if (!empty($node->signup_status) && module_exists('signup')) {
      $is_signed_up = db_select('signup_log', 'l')
        ->condition('l.nid', $node->nid)
        ->condition('l.uid', $user->uid)
        ->countQuery()->execute()->fetchField();
      $links[] = array(
        'title' => $is_signed_up ? t('Cancel sign up') : t('Sign up'),
        'href' => $prefix . _mm_schedule_get_node_link($node),
        'query' => $query,
      );
    }

    if (!$is_signed_up && !empty($args[8]) && ($cal_nid = intval($args[8])) != 0 && ($cal_node = node_load($cal_nid)) && _mm_schedule_remove_event_from_calendar_access($cal_node, $node)) {
      $url = 'mm_schedule/del_event/' . $cal_node->nid . '/' . $node->nid;
      $links[] = array(
        'title' => t('Remove from calendar'),
        'href' => $url,
        'query' => $query,
        'attributes' => array(
          'id' => 'del-from-cal',
          'title' => t('Remove this event from the current calendar')),
      );
      $js = _mm_schedule_subrequest_js('del-from-cal', "$url/js", "div[class^=calendar.$node->nid.]");
      $output['#attached']['js'][$js] = array('type' => 'inline', 'scope' => 'mm_schedule');
    }

    if (mm_schedule_my_calendar_access() && !mm_content_node_is_recycled($node)) {
      list(, $node_exists) = mm_schedule_my_calendar_exists();
      if (empty($node_exists) || mm_schedule_event_is_on_calendar($node_exists, 'node', $node) === FALSE) {
        $url = 'mm_schedule/add2mycalendar/node/' . $node->nid;
        $links[] = array(
          'title' => t('Add to My Calendar'),
          'href' => $url,
          'query' => $query,
          'attributes' => array(
            'id' => 'add-to-my-cal',
            'title' => t('Add all occurrences of this event to your personal calendar')),
        );
        $js = _mm_schedule_subrequest_js('add-to-my-cal', "$url/js");
        $output['#attached']['js'][$js] = array('type' => 'inline', 'scope' => 'mm_schedule');
      }
    }
  }
  else {
    $links[] = l(t('Details'), _mm_schedule_get_node_link($node));
  }
  $output['links']['node']['#links'] = $links;
  return drupal_render($output) . drupal_get_js('mm_schedule');
}

function _mm_schedule_subrequest_js($id, $url, $del_selector = '') {
  $del_js = $del_selector ? '$(\'' . $del_selector . '\').fadeOut(5000);' : '';
  return <<<EOJ
(function (\$) {
  \$('#$id:not(.$id-processed)')
    .addClass('$id-processed')
    .click(function() {
      this.blur();
      if (\$(this).attr('disabled')) return false;
      var body = \$('#calpopup-body');
      var div = \$('<div id="calpopup-body-result"><img src="' + Drupal.settings.jcalendarPath + '/images/throbber.gif" id="popthrobber-result" /></div>')
        .css('top', body[0].offsetTop)
        .insertAfter(body);
      \$.ajax({
        type:  "GET",
        url:   Drupal.settings.basePath + "$url",
        error: function(req, status) {
          div
            .html(status)
            .fadeOut(5000);
        },
        success: function(msg) {
          div
            .html(msg)
            .fadeOut(5000);
          \$('#$id')
            .attr('disabled', true)
            .fadeOut(5000);$del_js
        }
      });
      return false;
    });
})(jQuery);
EOJ;
}

function theme_mm_schedule_calendar_style_list($vars) {
  $output = array();
  $all_day = module_exists('date_all_day') ? theme('date_all_day_label') : t('All day');
  $today = date('Ymd');
  $date = new DateTime;
  $date->modify('+1 day');
  $tomorrow = $date->format('Ymd');
  foreach ($vars['rows'] as $date_data) {
    foreach ($date_data as $row_time => $events) {
      foreach ($events as $result) {
        if ($result->calendar_all_day) {
          if (date_format($result->date_start, 'Ymd') == date_format($result->date_end, 'Ymd')) {
            // all day on a single day
            $time = $all_day;
          }
          else {
            // all day, spanning multiple days
            $same_year = date_format($result->date_start, 'Y') == date_format($result->date_end, 'Y');
            $time = t('until !date', array('!date' => date_format($result->date_end, $same_year ? 'F j' : 'F j, Y')));
          }
        }
        else {
          $time = date_format($result->date_start, 'g:i a');
          if ($result->date_start != $result->date_end) {
            if (date_format($result->date_start, 'Ymd') == date_format($result->date_end, 'Ymd')) {
              // various times within the same day
              $time .= ' - ' . date_format($result->date_end, 'g:i a');
            }
            else {
              // various times on different days
              $date_fmt = date_format($result->date_start, 'Y') == date_format($result->date_end, 'Y') ? 'F j' : 'F j, Y';
              $time = t('!start_time on !start_date, to !end_time on !end_date', array('!start_time' => date_format($result->date_start, 'g:i a'), '!start_date' => date_format($result->date_start, $date_fmt), '!end_time' => date_format($result->date_end, 'g:i a'), '!end_date' => date_format($result->date_end, $date_fmt)));
            }
          }
        }

        $div = '<div' . drupal_attributes(array('class' => $result->date_id)) . '><div class="view-field">';

        if ($vars['view']->date_info->granularity == 'day' && $vars['view']->mm_display_mode != 'upcoming') {
          $key = $result->calendar_all_day ? '': $time;
          $output[$key][] = $div . l($result->title, 'node/' . $result->id) . '</div></div>';
        }
        else {
          if ($row_time == 'ongoing') {
            $key = t('Ongoing');
          }
          else if ($vars['view']->mm_display_mode == 'upcoming') {
            $start = date_format($result->date_start, 'Ymd');
            if ($start == $today) {
              $key = t('Today');
            }
            else if ($start == $tomorrow) {
              $key = t('Tomorrow');
            }
            else if (!strncmp($start, $today, 4)) {
              $key = date_format($result->date_start, 'l, F jS');
            }
            else {
              $key = date_format($result->date_start, 'l, F jS, Y');
            }
          }
          else if ($vars['view']->date_info->granularity == 'year') {
            $key = date_format($result->date_start, 'F j, Y');
          }
          else {
            $key = date_format($result->date_start, 'l, F j, Y');
          }
          $output[$key][] = $div . $time . ': ' . l($result->title, 'node/' . $result->id) . '</div></div>';
        }
      }
    }
  }

  if (!$output) return '';

  $html = '<dl>';
  foreach ($output as $period => $events) {
    if (empty($period) && count($output) == 1) {
      $html .= '<dt>' . join('</dt><dt>', $events) . '</dt>';
    }
    else {
      if (empty($period)) $period = $all_day;
      $html .= '<dt>' . $period . '</dt>';
      $html .= '<dd>' . join('</dd><dd>', $events) . '</dd>';
    }
  }
  $html .= '</dl>';
  return $html;
}

function theme_calendar_weekday($vars) {
  $dayofmonth = date_format($vars['date'], 'j');
  $day_name = date_format($vars['date'], 'l');
  $date_text = date_format($vars['date'], 'Y-m-d');
  $url = date_pager_url($vars['view'], NULL, $date_text);
  return '<a rel="nofollow" href="' . check_plain($url) . '"><span class="calendar-weekday-name">' . $day_name . '</span> <span class="calendar-weekday-dayofmonth">' . check_plain($dayofmonth) . '</span></a>';
}

function theme_mm_schedule_add_link($vars) {
  $param = $vars['param'];
  $param['query']['mm_event_start'] = date_format($vars['start_date'], DATE_FORMAT_ISO);
  if (!empty($vars['increment'])) {
    $end_date = clone($vars['start_date']);
    date_modify($end_date, $vars['increment']);
    $param['query']['mm_event_end'] = date_format($end_date, DATE_FORMAT_ISO);
  }
  return '<div class="calendar-add">' . l($param['title'], $param['href'], $param) . '</div>';
}


/* Referenced by _mm_schedule_preprocess_date_views_pager() */
function _mm_static__mm_schedule_jump() {
  return <<<EOJ
Drupal.behaviors.MMSchedJump = {
  attach: function (context, settings) {
    \$('.view-header select', context)
    .once('mm-sched-jump', function() {
      \$(this).bind('change.mm-schedule', function() {
        window.location = \$(this).val();
      });
    });
  }
};
EOJ;
}
