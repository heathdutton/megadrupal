<?php
/**
 * @file
 * View Spider Calendar block events.
 */

/**
 * View block events.
 */
function spider_calendar_block_events_view() {
  require_once "spider_calendar_see_more.inc";
  if (isset($_GET['block_id'])) {
    $block_id = check_plain($_GET['block_id']);
    $theme_id = 12;
    $theme = db_query("SELECT * FROM {spider_calendar_block_theme_table} WHERE id=:id", array(':id' => $theme_id))->fetchObject();
    $title_color = '#' . $theme->event_title_color_popup;
    $title_size = $theme->event_title_fsize_popup;
    $title_font = $theme->event_title_family_popup;
    $title_style = $theme->event_title_fstyle_popup;
    $date_color = '#' . $theme->date_color_popup;
    $date_size = $theme->date_fsize_popup;
    $date_font = $theme->date_family_popup;
    $date_style = $theme->date_fstyle_popup;
    // $date_format = $theme->date_format_popup;
    $show_repeat = $theme->repeat_rate;
    // $date_format_array = explode('/', $date_format);
    // for ($i=0; $i < count($date_format_array); $i++) {
      // if ($date_format_array[$i] == 'w') {
        // $date_format_array[$i] = 'l';
      // }
      // if ($date_format_array[$i] == 'm') {
        // $date_format_array[$i] = 'F';
      // }
      // if ($date_format_array[$i] == 'y') {
        // $date_format_array[$i] = 'Y';
      // }
    // }
    if (isset($_GET['date'])) {
      $datee = $_GET['date'];
    }
    else {
      $datee = date("Y-m-d");
    }
    $cal_id = variable_get(('spider_calendar_block_select_calendars' . $block_id), -1);
    $activedate = explode('-', $datee);
    $activedatetimestamp = mktime(0, 0, 0, $activedate[1], $activedate[2], $activedate[0]);
    $activedatestr = date("l", $activedatetimestamp) . ', ' . date("d", $activedatetimestamp) . ' ' . date("F", $activedatetimestamp) . ', ' . date("Y", $activedatetimestamp);
    $all_calendar_files = spider_calendar_block_get_event_ids($cal_id, $theme_id, $activedate[0], $activedate[1]);
    $ev_ids = $all_calendar_files[0]['ev_ids'];
    $ev_id = explode('<br />', $ev_ids[$activedate[2]]);
    $date =  $datee;
    $day = substr($date, 8);
    $block_events_view = '
    <div id="block_events_view" >';
    foreach ($ev_id as $event_row) {
      $row = db_query("SELECT * FROM {spider_calendar_events_table} WHERE id=:id", array(':id' => (int)$event_row))->fetchObject();
      if ($row) {
        $block_events_view .= '
    <div id="dayevents" style="padding:0px 0px 0px 7px; line-height:30px; padding-top:0px;">';
        if ($date_style == "Bold" || $date_style == "Bold and Italic" ) {
          $date_font_weight = "font-weight:bold";
        }
        else {
          $date_font_weight = "font-weight:normal";
        }
        if ($date_style == "Italic" || $date_style == "Bold and Italic" ) {
          $date_font_style = "font-style:italic";
        }
        else {
          $date_font_style = "";
        }
        $block_events_view .= '
        <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . $activedatestr . '</div>';
        if ($title_style == "Bold" || $title_style == "Bold and Italic") {
          $font_weight = "font-weight:bold";
        }
        else {
          $font_weight = "font-weight:normal";
        }
        if ($title_style == "Italic" || $title_style == "Bold and Italic") {
          $font_style = "font-style:italic";
        }
        else {
          $font_style = "";
        }
        $weekdays = explode(',', $row->week);
        $date_format1 = 'd/m/y';
        if ($row->event_repeat == '1') {
          $repeat = '';
        }
        else {
          $repeat = $row->event_repeat;
        }
        if ($row->event_date_end && $row->event_date_end != '0000-00-00') {
          $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Date') . ':' . str_replace("d", drupal_substr($row->event_date, 8, 2), str_replace("m", drupal_substr($row->event_date, 5, 2), str_replace("y", drupal_substr($row->event_date, 0, 4), $date_format1))) . '&nbsp;-&nbsp;' . str_replace("d", drupal_substr($row->event_date_end, 8, 2), str_replace("m", drupal_substr($row->event_date_end, 5, 2), str_replace("y", drupal_substr($row->event_date_end, 0, 4), $date_format1))) . '&nbsp;' . $row->event_time . '</div>';
        }
        else {
          $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $font_weight . '; ' . $font_style . '">' . $row->event_time . '</div>';
        }
        if ($show_repeat == 1) {
          if ($row->repeat_method == 'daily') {
            $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Day') . '</div>';
          }
          if ($row->repeat_method == 'weekly') {
            $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Week(s) on') . ' : ';
            for ($i = 0; $i < count($weekdays); $i++) {
              if ($weekdays[$i] != '') {
                if ($i != count($weekdays) - 2) {
                  $block_events_view .= spider_calendar_week_convert($weekdays[$i]) . ',';
                }
                else {
                  $block_events_view .= spider_calendar_week_convert($weekdays[$i]);
                }
              }
            }
            $block_events_view .= '
      </div>';
          }
          if ($row->repeat_method == 'monthly' && $row->month_type == 1) {
            $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Month(s) on the') . ' ' . $row->month . '</div>';
          }
          if ($row->repeat_method == 'monthly' && $row->month_type == 2) {
            $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Month(s) on the') . ' ' . spider_calendar_week_number($row->month_list) . ' ' . spider_calendar_week_convert($row->month_week) . '</div>';
          }
          if ($row->repeat_method == 'yearly' && $row->month_type == 1) {
            $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Year(s) in') . ' ' . date('F', mktime(0, 0, 0, $row->years_month + 1, 0, 0)) . ' ' . t('on the') . ' ' . $row->month . '</div>';
          }
          if ($row->repeat_method == 'yearly' && $row->month_type == 2) {
            $block_events_view .= '
      <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Year(s) in') . ' ' . date('F', mktime(0, 0, 0, $row->years_month + 1, 0, 0)) . ' ' . t('on the') . ' ' . spider_calendar_week_number($row->month_list) . ' ' . spider_calendar_week_convert($row->month_week) . '</div>';
          }
        }
        $block_events_view .= '
       <div style="color:' . $title_color . '; font-size:' . $title_size . 'px; font-family:' . $title_font . '; ' . $font_weight . '; ' . $font_style . '">' . check_plain($row->title) . '</div>';
        $block_events_view .= '
         <div style="line-height:20px">' . $row->text . '</div><br />';
        $block_events_view .= '
      </div>';
      }
    }
    $block_events_view .= '
    </div>';
  }
  $form = array();
    $form['aaa'] = array(
      '#prefix' => $block_events_view,
    );
    return $form;
}

/**
 * Return block calendar events ids depend on day.
 */
function spider_calendar_block_get_event_ids($cal_id, $cal_theme, $year, $month) {
  $date = $year . '-' . $month;
  $year = substr($date, 0, 4);
  $month = substr($date, 5, 2);
  $theme = db_query("SELECT * FROM {spider_calendar_block_theme_table} WHERE id=:id", array(':id' => $cal_theme))->fetchObject();
  $show_time = $theme->time_in_cell;
  $row = db_query("SELECT id FROM {spider_calendar_events_table} WHERE published=:published and (((event_date <= :event_date or event_date like :event_date) and  event_date_end >= :event_date_end) or (event_date_end=:event_date_end and event_date like :event_date)) and calendar=:calendar", array(':published' => 1, ':event_date' => substr($date, 0, 7) . '-01', ':event_date' => db_like(substr($date, 0, 7)) . '%', ':event_date_end' => substr($date, 0, 7) . '-01', ':event_date_end' => '', ':event_date' => db_like(substr($date, 0, 7)) . '%', ':calendar' => $cal_id))->fetchCol();
  // $result = db_query("SELECT id FROM {spider_calendar_events_table} WHERE published=1 and (((event_date <= '" . substr($date, 0, 7) . "-01' or event_date like '" . substr($date, 0, 7) . "%') and  event_date_end >= '" . substr($date, 0, 7) . "-01') or (event_date_end='' and event_date like '" . substr($date, 0, 7) . "%')) and calendar='%d'", $cal_id);
  $s = count($row);
  $id_array = array();
  $array_days = array();
  $array_days1 = array();
  $title = array();
  $ev_ids = array();
  for ($i = 0; $i < $s; $i++) {
    $rows = db_query("SELECT * FROM {spider_calendar_events_table} WHERE id=:id", array(':id' => $row[$i]))->fetchObject();
    $date_month = (int)substr($rows->event_date, 5, 2);
    $date_end_month = (int)substr($rows->event_date_end, 5, 2);
    $date_day = (int)substr($rows->event_date, 8, 2);
    $date_end_day = (int)substr($rows->event_date_end, 8, 2);
    $date_year_month = (int)(substr($rows->event_date, 0, 4) . substr($rows->event_date, 5, 2));
    $date_end_year_month = (int)(substr($rows->event_date_end, 0, 4) . substr($rows->event_date_end, 5, 2));
    $date_start_year = (int)(substr($rows->event_date, 0, 4));
    $date_end_year = (int)(substr($rows->event_date_end, 0, 4));
    $year_month = (int)($year . $month);
    $repeat = $rows->event_repeat;
    if ($repeat == "") {
      $repeat = 1;
    }
    $start_date = $rows->event_date;
    $weekly = $rows->week;
    $weekly_array = explode(',', $weekly);
    $date_days = array();
    $weekdays_start = array();
    $weekdays = array();

    // No Repeat.
    if ($rows->repeat_method == 'no_repeat') {
      $date_days[] = $date_day;
    }
    // Repeat daily.
    if ($rows->repeat_method == 'daily') {
      $t = spider_calendar_php_days_difference_block($rows->event_date, $rows->event_date_end);
      for ($k = 1; $k <= $t / $repeat; $k++) {
        $next_date = spider_calendar_php_get_next_date_block($start_date, $repeat);
        $next_date_array = explode('/', $next_date);
        if ((int)$month == $date_month && (int)substr($date_year_month, 0, 4) == (int)$year) {
          $date_days[0] = $date_day;
        }
        if ((int)$month == $next_date_array[0] && (int)$year == $next_date_array[2]) {
          $date_days[] = $next_date_array[1];
        }
        $start_date = date("Y-m-d", mktime(0, 0, 0, $next_date_array[0], $next_date_array[1], $next_date_array[2]));
      }
    }
    // Repeat weekly.
    if ($rows->repeat_method == 'weekly') {
      for ($j = 0; $j <= 6; $j++) {
        if (in_array(date("D", mktime(0, 0, 0, $date_month, $date_day + $j, substr($rows->event_date, 0, 4))), $weekly_array)) {
          $weekdays_start[] = $date_day + $j;
        }
      }
      for ($p = 0; $p < count($weekly_array) - 1; $p++) {
        $start_date = substr($rows->event_date, 0, 8) . $weekdays_start[$p];
        $t = spider_calendar_php_days_difference_block($rows->event_date, $rows->event_date_end);
        $r = 0;
        for ($k = 1; $k < $t / $repeat; $k++) {
          $start_date_array[] = $start_date;
          $next_date = spider_calendar_php_get_next_date_block($start_date, $repeat * 7);
          $next_date_array = explode('/', $next_date);
          if ((int)$month == $date_month && (int)substr($date_year_month, 0, 4) == (int)$year)
            $date_days[0] = $weekdays_start[$p];
          if ((int)$month == $next_date_array[0] && (int)$year == $next_date_array[2]) {
            if ((int)$year > (int)substr($date_year_month, 0, 4)) {
              $weekdays[] = $next_date_array[1];
            }
            else {
              $weekdays[] = $next_date_array[1];
            }
          }
          $start_date = date("Y-m-d", mktime(0, 0, 0, $next_date_array[0], $next_date_array[1], $next_date_array[2]));
          if ($next_date_array[2] > (int)substr($rows->event_date_end, 0, 4))
            break;
        }
        $date_days = array_merge($weekdays, $date_days);
      }
      $repeat = $repeat * 7;
    }
    // Repeat Monthly.
    if ($rows->repeat_method == 'monthly') {
      $xxxxx = 13;
      $year_dif_count = (int)$year - (int)substr($rows->event_date, 0, 4);
      $mount_dif_count = 12 - (int)substr($rows->event_date, 5, 2) + (($year_dif_count - 1) * 12);
      if ($year_dif_count > 0)
        for ($my_serch_month = 1; $my_serch_month <= 12; $my_serch_month++) {
          if ((($mount_dif_count + $my_serch_month) % $rows->event_repeat) == 0) {
            $xxxxx = $my_serch_month;
            break;
          }
        }
      if ($xxxxx != 13) {
        if ($xxxxx < 10) {
          $xxxxx = '0' . $xxxxx;
        }
      }
      $month_days = date('t', mktime(0, 0, 0, $month, $date_day, $year));
      if ($date_month < (int)$month or (int)substr($date_year_month, 0, 4) < $year)
        $date_day = 1;
      if ($year > (int)substr($date_year_month, 0, 4))
        $date_year_month = $year . $xxxxx;
      $p = (int)substr($date_year_month, 4, 2);
      if ((int)substr($date_year_month, 0, 4) != (int)substr($date_end_year_month, 0, 4))
        $end = (int)substr($date_end_year_month, 4, 2) + 12;
      else
        $end = (int)substr($date_end_year_month, 4, 2);
      for ($k = 1; $k <= $end; $k++) {
        if ((int)$month == $p && $rows->month_type == 1) {
          $date_days[0] = $rows->month;
        }
        if ($p == (int)$month && $rows->month_type == 2) {
          if ($rows->month_list != 'last') {
            for ($j = (int)$rows->month_list; $j < (int)$rows->month_list + 7; $j++) {
              if (date("D", mktime(0, 0, 0, $month, $j, $year)) == $rows->month_week) {
                if ($j >= $date_day) {
                  $date_days[0] = $j;
                  
                }
              }
            }
          }
          else {
            for ($j = 1; $j <= $month_days; $j++) {
              if (date("D", mktime(0, 0, 0, $month, $j, $year)) == $rows->month_week) {
                if ($j >= $date_day) {
                  $date_days[0] = $j;
                }
              }
            }
          }
        }
        if ($year > (int)substr($date_year_month, 0, 4)) {
          $p = 1;
        }
        $p = $p + $repeat;
      }
      $repeat = 32;
    }
    // Repeat yearly.
    if ($rows->repeat_method == 'yearly') {
      $month_days = date('t', mktime(0, 0, 0, $month, $date_day, $year));
      $end = substr($date_end_year_month, 0, 4) - substr($date_year_month, 0, 4) + 1;
      if (substr($date_year_month, 0, 4) < $year) {
        $date_day = 1;
      }
      for ($k = 0; $k <= $end; $k += $repeat) {
        if ((int)$month == (int)$rows->years_month && $rows->month_type == 1 && $year == substr($date_year_month, 0, 4) + $k) {
          $date_days[0] = $rows->month;
        }
      }
      for ($k = 0; $k <= $end; $k += $repeat) {
        if ((int)$month == (int)$rows->years_month && $rows->month_type == 2 && $year == substr($date_year_month, 0, 4) + $k) {
          if ($rows->month_list != 'last') {
            for ($j = (int)$rows->month_list; $j < (int)$rows->month_list + 7; $j++) {
              if (date("D", mktime(0, 0, 0, $month, $j, $year)) == $rows->month_week) {
                $date_days[0] = $j;
              }
            }
          }
          else {
            for ($j = 1; $j <= $month_days; $j++) {
              if (date("D", mktime(0, 0, 0, $month, $j, $year)) == $rows->month_week) {
                $date_days[0] = $j;
              }
            }
          }
        }
      }
      $repeat = 32;
    }
    $used = array();
    foreach ($date_days as $date_day) {
      if ($date_month == $month && $date_start_year == $year && $date_end_year == 0) {
        if (in_array($date_day, $used)) {
          continue;
        }
        else {
          array_push($used, $date_day);
        }
        if (in_array($date_day, $array_days)) {
          $key = array_search($date_day, $array_days);
          $title_num[$date_day]++;
          if ($rows->text != "")
            $array_days1[$key] = $date_day;
          $c = $title_num[$date_day];
          $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
          if ($rows->event_time && $show_time != 0) {
            $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
          }
          else {
            $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
          }
          $title[$date_day] = $title[$date_day] . $list;
          $ev_ids[$date_day] = $ev_ids[$date_day] . $rows->id . '<br />';
        }
        else {
          $array_days[] = $date_day;
          $key = array_search($date_day, $array_days);
          if ($rows->text != "") {
            $array_days1[$key] = $date_day;
          }
          $title_num[$date_day] = 1;
          $c = 1;
          $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
          if ($rows->event_time && $show_time != 0) {
            $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
          }
          else {
            $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
          }
          $title[$date_day] = $list;
          $ev_ids[$date_day] = $rows->id . '<br />';
        }
      }
      if ($date_end_month > 0 &&  $date_year_month == $date_end_year_month && $date_end_year_month == $year_month) {
        for ($j = $date_day; $j <= $date_end_day; $j = $j + $repeat) {
          if (in_array($j, $used))
            continue;
          else
            array_push($used, $j);
          if (in_array($j, $array_days)) {
            $key = array_search($j, $array_days);
            $title_num[$j]++;
            if ($rows->text != "") {
              $array_days1[$key] = $j;
            }
            $c = $title_num[$j];
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $title[$j] . $list;
            $ev_ids[$j] = $ev_ids[$j] . $rows->id . '<br />';
          }
          else {
            $array_days[] = $j;
            $key = array_search($j, $array_days);
            if ($rows->text != "") {
              $array_days1[$key] = $j;
            }
            $title_num[$j] = 1;
            $c = 1;
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $list;
            $ev_ids[$j] = $rows->id . '<br />';
          }
        }
      }
      if ($date_end_month > 0 &&  $date_year_month < $date_end_year_month && $date_year_month == $year_month)
        for ($j = $date_day; $j <= 31; $j = $j + $repeat) {
          if (in_array($j, $used)) {
            continue;
          }
          else {
            array_push($used, $j);
          }
          if (in_array($j, $array_days)) {
            $key = array_search($j, $array_days);
            $title_num[$j]++;
            if ($rows->text != "")
              $array_days1[$key] = $j;
            $c = $title_num[$j];
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $title[$j] . $list;
            $ev_ids[$j] = $ev_ids[$j] . $rows->id . '<br />';
          }
          else {
            $array_days[] = $j;
            $key = array_search($j, $array_days);
            if ($rows->text != "") {
              $array_days1[$key] = $j;
            }
            $title_num[$j] = 1;
            $c = 1;
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $list;
            $ev_ids[$j] = $rows->id . '<br />';
          }
        }
      if ($date_end_month > 0 &&  $date_year_month < $date_end_year_month && $date_end_year_month == $year_month) {
        for ($j = $date_day; $j <= $date_end_day; $j = $j + $repeat) {
          if (in_array($j, $used)) {
            continue;
          }
          else {
            array_push($used, $j);
          }
          if (in_array($j, $array_days)) {
            $key = array_search($j, $array_days);
            $title_num[$j]++;
            if ($rows->text != "") {
              $array_days1[$key] = $j;
            }
            $c = $title_num[$j];
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $title[$j] . $list;
            $ev_ids[$j] = $ev_ids[$j] . $rows->id . '<br />';
          }
          else {
            $array_days[] = $j;
            $key = array_search($j, $array_days);
            if ($rows->text != "") {
              $array_days1[$key] = $j;
            }
            $title_num[$j] = 1;
            $c = 1;
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $list;
            $ev_ids[$j] = $rows->id . '<br />';
          }
        }
      }
      if ($date_end_month > 0 &&  $date_year_month < $date_end_year_month && $date_end_year_month > $year_month && $date_year_month < $year_month) {
        for ($j = $date_day; $j <= 31; $j = $j + $repeat) {
          if (in_array($j, $used)) {
            continue;
          }
          else {
            array_push($used, $j);
          }
          if (in_array($j, $array_days)) {
            $key = array_search($j, $array_days);
            $title_num[$j]++;
            if ($rows->text != "") {
              $array_days1[$key] = $j;
            }
            $c = $title_num[$j];
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $title[$j] . $list;
            $ev_ids[$j] = $ev_ids[$j] . $rows->id . '<br />';
          }
          else {
            $array_days[] = $j;
            $key = array_search($j, $array_days);
            if ($rows->text != "") {
              $array_days1[$key] = $j;
            }
            $title_num[$j] = 1;
            $c = 1;
            $list = '<p><b>' . $c . '.</b>&nbsp;&nbsp;';
            if ($rows->event_time && $show_time != 0) {
              $list .= '&nbsp;' . check_plain($rows->title) . '<br />(' . $rows->event_time . ')</p>';
            }
            else {
              $list .= '&nbsp;' . check_plain($rows->title) . '</p>';
            }
            $title[$j] = $list;
            $ev_ids[$j] = $rows->id . '<br />';
          }
        }
      }
    }
  }
  for ($i = 1; $i <= count($array_days) - 1; $i++) {
    if (isset($array_days[$i])) {
      if ($array_days[$i] > '00' && $array_days[$i] < '09' && substr($array_days[$i], 0, 1) == '0') {
        $array_days[$i] = substr($array_days[$i], 1, 1);
      }
    }
  }
  $all_calendar_files['array_days'] = $array_days;
  $all_calendar_files['title'] = $title;
  //$all_calendar_files['option'] = $option;
  $all_calendar_files['array_days1'] = $array_days1;
  //$all_calendar_files['calendar'] = $calendar;
  $all_calendar_files['ev_ids'] = $ev_ids;
  return array($all_calendar_files);
}

/**
 * Days difference.
 */
function spider_calendar_php_days_difference_block($begin_date, $end_date) {
  //explode the date by "-" and storing to array
  $date_parts1 = explode("-", $begin_date);
  $date_parts2 = explode("-", $end_date);
  //gregoriantojd() Converts a Gregorian date to Julian Day Count
  $start_date = gregoriantojd($date_parts1[1], $date_parts1[2], $date_parts1[0]);
  $end_date = gregoriantojd($date_parts2[1], $date_parts2[2], $date_parts2[0]);
  return $end_date-$start_date;
}

/**
 * Get next date.
 */
function spider_calendar_php_get_next_date_block($begin_date, $repeat) {
  //explode the date by "-" and storing to array
  $date_parts1=explode("-", $begin_date);
  //gregoriantojd() Converts a Gregorian date to Julian Day Count
  $start_date=gregoriantojd($date_parts1[1], $date_parts1[2], $date_parts1[0]);
  return jdtogregorian($start_date+$repeat);
}
