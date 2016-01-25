<?php
/**
 * @file
 * Calendar view events see more popup.
 */

/**
 * See more.
 */
function spider_calendar_see_more() {
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_sliding_popup.js');
  drupal_add_css(drupal_get_path('module', 'spider_calendar') . '/css/spider_calendar_event_filedset.css');
  if (isset($_GET['cal_theme_id'])) {
    $theme_id = $_GET['cal_theme_id'];
    $theme = db_query("SELECT * FROM {spider_calendar_theme_table} WHERE id=:id", array(':id' => $theme_id))->fetchObject();
    $title_color = '#' . $theme->event_title_color_popup;
    $title_size = $theme->event_title_fsize_popup;
    $title_font = $theme->event_title_family_popup;
    $title_style = $theme->event_title_fstyle_popup;
    $date_color = '#' . $theme->date_color_popup;
    $date_size = $theme->date_fsize_popup;
    $date_font = $theme->date_family_popup;
    $date_style = $theme->date_fstyle_popup;
    $next_prev_event_bgcolor = '#' . $theme->arrow_bgcolor_popup;
    $next_prev_event_arrowcolor = '#' . $theme->arrow_color_popup;
    $show_event_bgcolor = '#' . $theme->popup_bgcolor;
    $popup_width = $theme->popup_width;
    $popup_height = $theme->popup_height;
    $date_format = $theme->date_format_popup;
    $show_repeat = $theme->repeat_rate;
    $date_format_array = explode('/', $date_format);
    for ($i=0; $i < count($date_format_array); $i++) {
      if ($date_format_array[$i] == 'w') {
        $date_format_array[$i] = 'l';
      }
      if ($date_format_array[$i] == 'm') {
        $date_format_array[$i] = 'F';
      }
      if ($date_format_array[$i] == 'y') {
        $date_format_array[$i] = 'Y';
      }
    }
    //$all_files = php_showevent();
    //$rows = $all_files[0]['rows'];
    //$option = $all_files[0]['option'];
    $nodeid = $_GET['nodeid'];
    if (isset($_GET['date'])) {
      $datee = $_GET['date'];
    }
    else {
      $datee = date("Y-m-d");
    }
    $activedate = explode('-', $datee); 
    $activedatetimestamp = mktime(0, 0, 0, $activedate[1], $activedate[2], $activedate[0]); 
    $activedatestr = date("l", $activedatetimestamp) . ', ' . date("d", $activedatetimestamp) . ' ' . date("F", $activedatetimestamp) . ', ' . date("Y", $activedatetimestamp);
    $date =  $datee;
    $day = substr($date, 8);
    if (isset($_GET['cal_id'])) {
      $calendar_id = $_GET['cal_id'];
    }
    else {
      $calendar_id = 0;
    }
    $ev_ids = $_GET['ev_ids'];
    $ev_id = explode(',', $ev_ids);
    $event_id = $_GET['eventID'];
    echo '<style>
            #sliding_popup .content { position:absolute; left:50%; top:70px; display:block;  height:100%; }
            #sliding_popup .see_more * { margin-left:300px; display:block;}
            #sliding_popup .close { overflow:hidden; display:block; position:absolute; right:10%; bottom:90%; width:32px; height:32px; text-indent:1000px; background-image:url("' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/close.png"); }
          </style>
          <div style="width:100%;height:120%;background-color:#000000;opacity:0.80;"></div>
          <div id="div1_content" class="content" style="z-index:1000000;">
           <div id="see_more" style="top:30px; background-color:' . $show_event_bgcolor . '; padding:15px; width:' . $popup_width . 'px; height:' . $popup_height . 'px; margin-left:-' . $popup_width/2 . 'px;">';
    if ($_GET['task'] == 'all_events') {
      
      foreach ($ev_id as $row) {
        $event_row = db_query("SELECT * FROM {spider_calendar_events_table} WHERE id=:id", array(':id' => (int)$row))->fetchObject();
        for ($i = 0; $i < count($ev_id); $i++) {
          if ($event_row->id == $ev_id[$i]) {
            echo '
              <div>
                <a style="font-size:' . $title_size . 'px; color:' . $title_color . '; line-height:30px" href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $theme_id, 'cal_id' => $calendar_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids, 'date' => $date, 'eventID' => $event_row->id), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\'>' . ($i + 1) . ' ' . check_plain($event_row->title) . '
                </a>
              </div>';
          }
        }
      }
    }
    elseif ($_GET['task'] == 'view_event') {
      $row = db_query("SELECT * FROM {spider_calendar_events_table} WHERE id=:id", array(':id' => $event_id))->fetchObject();
      echo '  <style>
                #previous , #next {
                  cursor:pointer;
                  height:' . ($popup_height - 6) . 'px;
                  width:5%;
                }
                .arrow {
                  color:' . $next_prev_event_arrowcolor . ';
                  font-size:50px;
                  text-decoration:none;
                }
              </style>';
      echo '  <table style="height:' . $popup_height . 'px; width:100%; background-color:' . $show_event_bgcolor . '; border-spacing:0; border-collapse:separate;"  align="center">
                <tr style="background-color:inherit;">
                  <td id="previous" onclick=\'spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $theme_id, 'cal_id' => $calendar_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids, 'date' => $date, 'eventID' => (int)$ev_id[array_search($event_id, $ev_id) - 1]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' style="border:none; ' . ((count($ev_id) == 1 || $event_id == $ev_id[0]) ? 'display:none;' : '') . '; text-align:center" onmouseover=\'document.getElementById("previous").style.backgroundColor="' . $next_prev_event_bgcolor . '"\' onmouseout=\'document.getElementById("previous").style.backgroundColor=""\'>
                    <span class="arrow">&lt;</span>
                  </td>
                  <td style="vertical-align:top; width:90%; border:none;">
                    <div id="dayevent" style="padding:0px 0px 0px 7px; line-height:30px; padding-top:0px;">';

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
      echo '
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
      if ($row->text != '') {
        if ($row->event_date_end && $row->event_date_end != '0000-00-00') {
          echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Date') . ':' . str_replace("d", drupal_substr($row->event_date, 8, 2), str_replace("m", drupal_substr($row->event_date, 5, 2), str_replace("y", drupal_substr($row->event_date, 0, 4), $date_format1))) . '&nbsp;-&nbsp;' . str_replace("d", drupal_substr($row->event_date_end, 8, 2), str_replace("m", drupal_substr($row->event_date_end, 5, 2), str_replace("y", drupal_substr($row->event_date_end, 0, 4), $date_format1))) . '&nbsp;' . $row->event_time . '</div>';
        }
        else {
          echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $font_weight . '; ' . $font_style . '">' . $row->event_time . '</div>';
        }
        if ($show_repeat == 1) {
          if ($row->repeat_method == 'daily') {
            echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Day') . '</div>';
          }
          if ($row->repeat_method == 'weekly') {
            echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Week(s) on') . ' : ';
            for ($i = 0; $i < count($weekdays); $i++) {
              if ($weekdays[$i] != '') {
                if ($i != count($weekdays) - 2) {
                  echo spider_calendar_week_convert($weekdays[$i]) . ',';
                }
                else {
                  echo spider_calendar_week_convert($weekdays[$i]);
                }
              }
            }
            echo '
                    </div>';
          }
          if ($row->repeat_method == 'monthly' && $row->month_type == 1) {
            echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Month(s) on the') . ' ' . $row->month . '</div>';
          }
          if ($row->repeat_method == 'monthly' && $row->month_type == 2) {
            echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Month(s) on the') . ' ' . spider_calendar_week_number($row->month_list) . ' ' . spider_calendar_week_convert($row->month_week) . '</div>';
          }
          if ($row->repeat_method == 'yearly' && $row->month_type == 1) {
            echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Year(s) in') . ' ' . date('F', mktime(0, 0, 0, $row->years_month + 1, 0, 0)) . ' ' . t('on the') . ' ' . $row->month . '</div>';
          }
          if ($row->repeat_method == 'yearly' && $row->month_type == 2) {
            echo '
                    <div style="color:' . $date_color . '; font-size:' . $date_size . 'px; font-family:' . $date_font . '; ' . $date_font_weight . '; ' . $date_font_style . '">' . t('Repeat Every') . ' ' . $repeat . ' ' . t('Year(s) in') . ' ' . date('F', mktime(0, 0, 0, $row->years_month + 1, 0, 0)) . ' ' . t('on the') . ' ' . spider_calendar_week_number($row->month_list) . ' ' . spider_calendar_week_convert($row->month_week) . '</div>';
          }
        }
        echo '
                    <div style="color:' . $title_color . '; font-size:' . $title_size . 'px; font-family:' . $title_font . '; ' . $font_weight . '; ' . $font_style . '">' . check_plain($row->title) . '</div>';
        echo '
                    <div style="line-height:20px">' . $row->text . '</div>';
      }
      else {
        echo '
                    <div style="color:' . $title_color . '; font-size:' . $title_size . 'px; font-family:' . $title_font . '; ' . $font_weight . '; ' . $font_style . '">' . check_plain($row->title) . '</div>';
        echo '
                    <h1 style="text-align:center"></h1>';
      }
      echo '
                  </div>
                  <div style="width:98%; text-align:right; ' . ((count($ev_id) == 1) ? 'display:none;' : '') . '">
                    <a style="color:' . $title_color . '; font-size:15px; font-family:' . $title_font . '; ' . $font_weight . '; ' . $font_style . '" href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $theme_id, 'cal_id' => $calendar_id, 'nodeid' => $nodeid, 'task' => 'all_events', 'ev_ids' => $ev_ids, 'date' => $date), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\'>' . t('Back to event list') . '
                    </a>                    
                  </div>
                </td>
                <td id="next" onclick=\'spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $theme_id, 'cal_id' => $calendar_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids, 'date' => $date, 'eventID' => (int)$ev_id[array_search($event_id, $ev_id) + 1]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' style="border:none; ' . ((count($ev_id) == 1 || $event_id == end($ev_id)) ? 'display:none;' : '') . '; text-align:center" onmouseover=\'document.getElementById("next").style.backgroundColor="' . $next_prev_event_bgcolor . '"\' onmouseout=\'document.getElementById("next").style.backgroundColor=""\'>
                  <span class="arrow">&gt;</span>
                </td>
              </tr>
            </table>';
    }
    echo '
          </div>
        </div>
        <a class="close" href="javascript:spider_calendar_destroypopup(1000);" style="z-index:1000001"></a>';
  }
}

/**
 * Convert week days.
 */
function spider_calendar_week_convert($x) {
  if ($x == 'Mon') {
    return t('Monday');
  }
  if ($x == 'Tue') {
    return t('Tuesday');
  }
  if ($x == 'Wed') {
    return t('Wednesday');
  }
  if ($x == 'Thu') {
    return t('Thursday');
  }
  if ($x == 'Fri') {
    return t('Friday');
  }
  if ($x == 'Sat') {
    return t('Saturday');
  }
  if ($x == 'Sun') {
    return t('Sunday');
  }
}

/**
 * Convert week days.
 */
function spider_calendar_week_number($x) {
  if ($x == 1) {
    return t('First');
  }
  if ($x == 8) {
    return t('Second');
  }
  if ($x == 15) {
    return t('Third');
  }
  if ($x == 22) {
    return t('Fourth');
  }
  if ($x == 'last') {
    return t('Last');
  }
}
