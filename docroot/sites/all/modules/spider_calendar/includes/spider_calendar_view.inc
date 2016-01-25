<?php
/**
 * @file
 * This file contains all functions wich need for calendar view.
 */

/**
 * View calendar.
 */
function spider_calendar_content_view($cal_id, $cal_theme, $many_sp_calendar) {
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_sliding_popup.js');
  drupal_add_css(drupal_get_path('module', 'spider_calendar') . '/css/spider_calendar_event_filedset.css');
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar_show_cal.js');
  drupal_add_js(drupal_get_path('module', 'spider_calendar') . '/js/spider_calendar.js');
  $url = url('node/', array('absolute' => TRUE));
  $theme = db_query("SELECT * FROM {spider_calendar_theme_table} WHERE id=:id", array(':id' => $cal_theme))->fetchObject();
  $cal_width = $theme->theme_width;
  $weekstart = $theme->week_start_day;
  $border_color = '#' . $theme->border_color;
  $border_radius = $theme->border_radius;
  $border_width = $theme->border_width;
  $border_radius2 = $border_radius - $border_width;
  $bg_bottom = '#' . $theme->body_bgcolor;
  $cell_height = $theme->cell_height;
  $text_color_other_months = '#' . $theme->omd_fcolor;
  $bg_color_other_months = '#' . $theme->omd_bgcolor;
  $text_color_this_month_unevented = '#' . $theme->cell_text_color_wevents;
  $evented_color = '#' . $theme->cell_text_color_events;
  $evented_color_bg = '#' . $theme->cell_bgcolor_events;
  $event_title_color = '#' . $theme->event_title_color;
  $current_day_border_color = '#' . $theme->cur_day_cell_border_color;
  $cell_border_color = '#' . $theme->cell_border_color;
  $sun_days = '#' . $theme->sundays_tcolor;
  $sundays_bg_color = '#' . $theme->sundays_cell_bgcolor;
  $sundays_font_size = $theme->sundays_fcolor;
  $weekdays_font_size = $theme->days_fsize;
  $top_height = $theme->header_heidht;
  $bg_top = '#' . $theme->header_bgcolor;
  $year_font_size = $theme->year_font_size;
  $text_color_year = '#' . $theme->year_color;
  $color_arrow_year = '#' . $theme->year_arrow_color;
  $month_type = $theme->month_type;
  $text_color_month = '#' . $theme->cur_month_color;
  $month_font_size = $theme->cur_month_fsize;
  $color_arrow_month = '#' . $theme->month_arrow_color;
  $next_month_text_color = '#' . $theme->next_month_color;
  $next_month_font_size = $theme->next_month_fsize;
  $next_month_arrow_color = '#' . $theme->next_month_arrow_color;
  $prev_month_text_color = '#' . $theme->prev_month_color;
  $prev_month_font_size = $theme->prev_month_fsize;
  $prev_month_arrow_color = '#' . $theme->prev_month_arrow_color;
  $arrow_size = $theme->arrow_size;
  $arrow_size_hover = $arrow_size + 5;
  $color_week_days = '#' . $theme->weekdays_color;
  $week_days_cell_height = $theme->week_cell_height;
  $weekdays_bg_color = '#' . $theme->weekdays_bgcolor;
  $weekday_sunday_bg_color = '#' . $theme->sunday_bgcolor;
  $popup_width = $theme->popup_width;
  $popup_height = $theme->popup_height;
  $number_of_shown_evetns = $theme->displaied_events;
  $other_days_font_size = $theme->weekdays_fsize;
  $year = date("Y");
  $month =  date("n");
  $day = date("d");

  if (isset($_POST['date_year'])) {
    $year = check_plain($_POST['date_year']);
  }
  if (isset($_POST['date_month'])) {
    $month = check_plain($_POST['date_month']);
  }
  if ($month < 1) {
    --$year;
    $month = 12;
  }
  if ($month > 12) {
    ++$year;
    $month = 1;
  }
  $year_REFERER = $year;
  $month_REFERER = $month;
  $day_REFERER = 2222;//substr($date_REFERER,8,2);
  drupal_add_js(array(
    'spider_calendar' => array(
      'nodeid' => $nodeid,
      'url' => $url,
      'date' => $year,
      'month' => $month)),
    'setting');
  if ($cell_height == '') {
    $cell_height = 70;
  }
  if ($cal_width == '') {
    $cal_width = 700;
  }
  $cell_width = $cal_width / 7;
  $content = '
  <style type="text/css">
    #bigcalendar' . $many_sp_calendar . ' td,
    #bigcalendar' . $many_sp_calendar . ' tr,
    #spiderCalendarTitlesList td,
    #spiderCalendarTitlesList tr {
      border:none  !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .general_table {
      border-radius: ' . $border_radius . 'px !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .top_table {
      border-top-left-radius: ' . $border_radius2 . 'px !important;
      border-top-right-radius: ' . $border_radius2 . 'px !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .cala_arrow a:link,
    #bigcalendar .cala_arrow a:visited {
      text-decoration:none !important;
      background:none !important;
      font-size: ' . $arrow_size . 'px !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .cala_arrow {
      vertical-align:middle !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .cala_arrow a:hover {
      font-size: ' . $arrow_size_hover . 'px !important;
      text-decoration:none !important;
      background:none !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .cala_day a:link,
    #bigcalendar' . $many_sp_calendar . ' .cala_day a:visited {
      text-decoration:none !important;
      background:none !important;
      font-size:12px !important;
      color:red ;
    }
    #bigcalendar' . $many_sp_calendar . ' .cala_day a:hover {
      text-decoration:none !important;
      background:none !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .cala_day {
      border: 1px solid ' . $cell_border_color . ' !important;
      vertical-align: top !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .weekdays {
      vertical-align:middle !important;
      border: 1px solid ' . $cell_border_color . ' !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .week_days {
      font-size: ' . $weekdays_font_size . 'px !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .calyear_table {
      border-spacing:0 !important;
      width:100% !important;
    }
    .calyear_table table #bigcalendar' . $many_sp_calendar . ' .calmonth_table {	
      border-spacing:0 !important;
      width:100% !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .calbg,
    #bigcalendar .calbg td {
      background-color: ' . $bg_bottom . ' !important;
      text-align:center !important;
      width:14% !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .caltext_color_other_months {
      color: ' . $text_color_other_months . ' !important;
      border: 1px solid ' . $cell_border_color . ' !important;
      vertical-align: top !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .caltext_color_this_month_unevented {
      color: ' . $text_color_this_month_unevented . ' !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .calfont_year {    
      font-size:24px !important;
      font-weight:bold !important;
      color: ' . $text_color_year . ' !important;
    }
    .general_table table,
    .general_table td,
    .general_table tr {
      border:inherit !important;
      vertical-align:initial !important;
      border-collapse:inherit !important;
      margin:inherit !important;
      padding:inherit !important;
    }
    .general_table {
      border-collapse:inherit !important;
      margin:inherit !important;
    }
    .general_table p {
      margin:inherit !important;
      padding:inherit !important;
    }
    #bigcalendar' . $many_sp_calendar . ' .calsun_days {
      color: ' . $sun_days . ' !important;
      border: 1px solid ' . $cell_border_color . ' !important;
      vertical-align: top !important;
      text-align:left !important;   
      background-color: ' . $sundays_bg_color . ' !important;
    }
  </style>
  <div id="bigcalendar' . $many_sp_calendar . '">
    <div style="width:' . $cal_width . 'px !important;" >
      <table cellpadding="0" cellspacing="0" class="general_table" style="overflow:hidden !important; border-spacing:0 !important; width:' . $cal_width . 'px !important;border:' . $border_color . ' solid ' . $border_width . 'px !important; margin:0 !important; padding:0 !important;background-color:' . $bg_bottom . ' !important;">
        <tr style="background-color:inherit;">
          <td width="100%" style="padding:0 !important; margin:0 !important;">
            <table cellpadding="0" cellspacing="0" border="0" style="border-spacing:0 !important; font-size:12px !important; margin:0 !important; padding:0 !important;"  width="' . $cal_width . '">
              <tr style="height:40px !important; width:' . $cal_width . 'background-color:inherit;">';
  if ($month_type == 1) {
    $content .= '
                <td class="top_table" align="center" colspan="7" style="padding:0 !important; margin:0 !important; background-color:' . $bg_top . ' !important;height:20px !important; ">
                  <table cellpadding="0" cellspacing="0" border="0" align="center" class="calyear_table" style="margin:0 !important; padding:0 !important; text-align:center !important; width:' . (int) $cal_width . 'px !important; height:' . $top_height . 'px !important;">
                    <tr style="background-color:inherit;">
                      <td style="width:100% !important;vertical-align:bottom !important;padding-bottom:0px !important;">
                        <table style="width:100% !important;">
                          <tr style="background-color:inherit;">
                            <td class="cala_arrow" width="40%" style="text-align:right !important;margin:0px !important;padding:0px">';
  $content .= l(t('&#9668;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '1', 'style' => 'color:' . $color_arrow_year), 'html' => TRUE));
  $content .= '
                            <!--  <a style="color:' . $color_arrow_year . '"  href=\'javascript:showbigcalendar("bigcalendar' . $many_sp_calendar . '", "node/' . $nodeid . '/to")\'>&#9668;</a>-->
                            </td>
                            <td style="text-align:center !important; margin:0 !important; padding:0 !important;" width="20%">
                              <input name="year" type="hidden" readonly="" value="' . $year . '"/>
                                <span style="font-family:arial !important;font-size:' . $year_font_size . 'px !important;font-weight:bold !important;color:' . $text_color_year . ' !important;">' . $year . '</span>
                            </td>
                            <td style="margin:0 !important; padding:0 !important;text-align:left !important;" width="40%" class="cala_arrow">';
  $content .= l(t('&#9658;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '2', 'style' => 'color:' . $color_arrow_year), 'html' => TRUE));
  $content .= '
                            </td>
                          </tr>
                        </table>
                      </td>
                    </tr>
                    <tr style="background-color:inherit;">
                      <td style="width:100% !important;vertical-align:bottom !important; padding-bottom:5px !important;">
                        <table style="width:100% !important;line-height:150% !important;">
                          <tr style="padding-top:5px !important;background-color:inherit;">
                            <td class="cala_arrow" width="40%"  style="text-align:left !important;margin:0px !important;padding:0px !important;">
                              <table width="80%">
                                <tr style="background-color:inherit;">
                                  <td width="15%">';
  $content .= l(t('&#9668;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '3', 'style' => 'color:' . $prev_month_arrow_color), 'html' => TRUE));
  $content .= '
                                  </td>
                                  <td style="vertical-align:middle !important">
                                    <span style="font-family:arial !important; color:' . $prev_month_text_color . ' !important; font-size:' . $prev_month_font_size . 'px !important;">' . (($month - 1 < 1) ? spider_calendar_int_to_month_name(12) : spider_calendar_int_to_month_name($month - 1)) . '</span>
                                  </td>
                                </tr>
                              </table>
                            </td>
                          <td style="text-align:center !important; margin:0 !important; vertical-align:middle !important; padding:0px !important;" width="20%" >
                            <input type="hidden" name="month" readonly="" value="' . spider_calendar_int_to_month_name($month) . '"/>
                            <span  style="font-family:arial !important; color:' . $text_color_month . ' !important; font-size:' . $month_font_size . 'px !important;">' . spider_calendar_int_to_month_name($month) . '</span>
                          </td>
                          <td style="margin:0 !important; padding:0 !important;text-align:right" width="40%" class="cala_arrow">
                            <table width="100%">
                              <tr style="background-color:inherit;">
                                <td style="text-align:right; vertical-align:middle !important">
                                  <span style="font-family:arial !important; color:' . $next_month_text_color . ' !important; font-size:' . $next_month_font_size . 'px !important;">' . (($month + 1 > 12) ? spider_calendar_int_to_month_name(1) : spider_calendar_int_to_month_name($month + 1)) . '</span>&nbsp
															</td>
															<td width="10%" style="text-align:right">';
  $content .= l(t('&#9658;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '4', 'style' => 'color:' . $next_month_arrow_color), 'html' => TRUE));
  $content .= '
															</td>
														</tr>
													</table>
												</td>
											</tr>
                    </table>
                  </td>
                </tr>
              </table>
            </td>';
  }
  if ($month_type == 2) {
    $content .= '
            <td class="top_table" align="center" colspan="7" style="padding:0 !important; margin:0 !important; background-color:' . $bg_top . ' !important;height:20px !important;">
              <table cellpadding="0" cellspacing="0" border="0" align="center" class="calyear_table" style="margin:0 !important; padding:0 !important; text-align:center !important; width:' . $cal_width . 'px !important; height:' . $top_height . 'px !important;">
                <tr>
                  <td style="width:100% !important;vertical-align:bottom !important;padding-bottom:0px !important;">
                    <table style="width:100% !important;">
                      <tr>
                        <td class="cala_arrow" width="40%"  style="text-align:right !important;margin:0px !important;padding:0px">';
  $content .= l(t('&#9668;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '1', 'style' => 'color:' . $color_arrow_year), 'html' => TRUE));
  $content .= '
                        <!--  <a style="color:' . $color_arrow_year . '"  href=\'javascript:showbigcalendar("bigcalendar' . $many_sp_calendar . '", "?theme_id=' . $theme_id . '&calendar=' . $calendar_id . '&date=' . ($year - 1) . '-' . spider_calendar_add_0(spider_calendar_month_num($month)) . '&many_sp_calendar=' . $many_sp_calendar . '&cur_page_url=' . $path_sp_cal . '")\'>&#9668;</a>-->
                        </td>
                        <td style="text-align:center !important; margin:0 !important; padding:0 !important;" width="20%">
                          <input name="year" type="hidden" readonly="" value="' . $year . '"/>
                          <span style="font-family:arial !important;font-size:' . $year_font_size . 'px !important;font-weight:bold !important;color:' . $text_color_year . ' !important;">' . $year . '</span>
                        </td>
                        <td style="margin:0 !important; padding:0 !important;text-align:left" width="40%"  class="cala_arrow">';
  $content .= l(t('&#9658;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '2', 'style' => 'color:' . $color_arrow_year), 'html' => TRUE));
  $content .= '
                          <!--<a style="color:' . $color_arrow_year . '"  href=\'javascript:showbigcalendar("bigcalendar' . $many_sp_calendar . '", "?theme_id=' . $theme_id . '&calendar=' . $calendar_id . '&date=' . ($year + 1) . '-' . spider_calendar_add_0(spider_calendar_month_num($month)) . '&many_sp_calendar=' . $many_sp_calendar . '&cur_page_url=' . $path_sp_cal . '")\'>&#9658;</a>-->
                        </td>
                      </tr>
                    </table>
                  </td>
								</tr>
								<tr>
                  <td style="width:100% !important;vertical-align:top">
                    <table style="width:100% !important;line-height:150%">
                      <tr>
                        <td class="cala_arrow" width="40%"  style="text-align:right !important;margin:0px !important;padding:0px">';
  $content .= l(t('&#9668;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '3', 'style' => 'color:' . $color_arrow_month), 'html' => TRUE));
  $content .= '
                          <!--<a style="color:' . $color_arrow_month . '" href=\'javascript:showbigcalendar("bigcalendar' . $many_sp_calendar . '", "?theme_id=' . $theme_id . '&calendar=' . $calendar_id . '&date=' . ($year - 1) . '-12' . '&many_sp_calendar=' . $many_sp_calendar . '&cur_page_url=' . $path_sp_cal . '")\'>&#9668;</a>-->
                        </td>
                        <td style="text-align:center !important; margin:0 !important;" width="20%">
                          <input type="hidden" name="month" readonly="" value="' . $month . '"/>
                          <span style="font-family:arial !important; color:' . $text_color_month . ' !important; font-size:' . $month_font_size . 'px !important;">' . spider_calendar_int_to_month_name($month) . '</span>
                        </td>
                        <td style="margin:0 !important; padding:0 !important;text-align:left" width="40%"  class="cala_arrow">';
  $content .= l(t('&#9658;'), '', array('attributes' => array('class' => 'ahref', 'rel' => '4', 'style' => 'color:' . $color_arrow_month), 'html' => TRUE));
  $content .= '
                          <!--<a style="color:' . $color_arrow_month . '" href=\'javascript:showbigcalendar("bigcalendar' . $many_sp_calendar . '", "?theme_id=' . $theme_id . '&calendar=' . $calendar_id . '&date=' . ($year + 1) . '-01' . '&many_sp_calendar=' . $many_sp_calendar . '&cur_page_url=' . $path_sp_cal . '")\'>&#9658;</a>-->
                        </td>
                      </tr>
                    </table>
									</td>
                </tr>
              </table>
            </td>';
  }
  $content .= '
            <td colspan="7" style="margin:0 !important; padding:0 !important; background-color:' . $bg_top . ' !important;">
            </td>
          </tr>
          <tr align="center" height="' . $week_days_cell_height . '" style="background-color:' . $weekdays_bg_color . ' !important;">';
  if ($weekstart == "su") {
    $content .= '
          <td class="weekdays" style="width:' . $cell_width . 'px !important; color:' . $color_week_days . ' !important; margin:0 !important; padding:0 !important;background-color:' . $weekday_sunday_bg_color . '">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days">' . t('Sunday') . '</b>
            </div>
          </td>';
  }
  $content .= '
          <td class="weekdays" style="width:' . $cell_width . 'px !important; color:' . $color_week_days . ' !important; margin:0 !important; padding:0">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days"> '. t('Monday') . ' </b>
            </div>
          </td>
          <td class="weekdays" style="width:' . $cell_width . 'px !important; color:' . $color_week_days . ' !important; margin:0 !important; padding:0">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days"> '. t('Tuesday') . ' </b>
            </div>
          </td>
          <td class="weekdays" style="width:' . $cell_width . 'px !important; 	color:' . $color_week_days . ' !important; margin:0 !important; padding:0">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days"> '. t('Wednesday') . ' </b>
            </div>
          </td>
          <td class="weekdays" style="width:' . $cell_width . 'px !important; color:' . $color_week_days . ' !important; margin:0 !important; padding:0">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days"> '. t('Thursday') . ' </b>
            </div>
          </td>
          <td class="weekdays" style="width:' . $cell_width . 'px !important; color:' . $color_week_days . ' !important; margin:0 !important; padding:0">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days"> '. t('Friday') . ' </b>
            </div>
          </td>
          <td class="weekdays" style="width:' . $cell_width . 'px !important;	color:' . $color_week_days . ' !important; margin:0 !important; padding:0">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days"> '. t('Saturday') . ' </b>
            </div>
          </td>';
  if ($weekstart == "mo") {
    $content .= '
          <td class="weekdays" style="width:' . $cell_width . 'px !important; color:' . $color_week_days . ' !important; margin:0 !important; padding:0 !important;background-color:' . $weekday_sunday_bg_color . '">
            <div class="calbottom_border" style="text-align:center !important; width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b class="week_days"> '. t('Sunday') . ' </b>
            </div>
          </td>';
  }
  $content .= '
        </tr>';
  $month_first_weekday = date("N", mktime(0, 0, 0, $month, 1, $year));
  if ($weekstart == "su") {
    $month_first_weekday++;
    if ($month_first_weekday == 8) {
      $month_first_weekday = 1;
    }
  }
  $month_days = date("t", mktime(0, 0, 0, $month, 1, $year));
  $last_month_days = date("t", mktime(0, 0, 0, $month - 1, 1, $year));
  $weekday_i = $month_first_weekday;
  $last_month_days = $last_month_days - $weekday_i + 2;
  $percent = 1;
  $sum = $month_days - 8 + $month_first_weekday;
  if ($sum % 7 <> 0) {
    $percent = $percent + 1;
  }
  $sum = $sum - ($sum % 7);
  $percent = $percent + ($sum / 7);
  $percent = 107 / $percent;
  $all_calendar_files = spider_calendar_getdays($cal_id, $cal_theme, $year, $month);
  $array_days = $all_calendar_files[0]['array_days'];
  //$array_days1 = $all_calendar_files[0]['array_days1'];
  $title = $all_calendar_files[0]['title'];
  $ev_ids = $all_calendar_files[0]['ev_ids'];
  $content .= '
        <tr id="days" height="' . $cell_height . '" style="line-height:15px !important;background-color:inherit;">';
  for ($i = 1; $i < $weekday_i; $i++) {
    $content .= '
          <td class="caltext_color_other_months" style="background-color:' . $bg_color_other_months . '">
            <span style="font-size:' . $other_days_font_size . 'px !important;">' . $last_month_days . ' </span>
          </td>';
  $last_month_days = $last_month_days + 1;
  }
  for ($i = 1; $i <= $month_days; $i++) {
    if (isset($title[$i])) {
      $ev_title = explode('</p>', $title[$i]);
      array_pop($ev_title);
      $k = count($ev_title);
      $ev_id = explode('<br />', $ev_ids[$i]);
      array_pop($ev_id);
      $ev_ids_inline = implode(',', $ev_id);
    }
    $dayevent = '';
    if (($weekday_i % 7 == 0 && $weekstart == "mo") || ($weekday_i % 7 == 1 && $weekstart == "su")) {
      if (($i == $day_REFERER) && ($month == $month_REFERER) && ($year == $year_REFERER)) {
        $content .= '
          <td bgcolor="' . $bg_color_selected . '"  class="cala_day" style="padding:0 !important; margin:0 !important;line-height:15px !important;">
            <div class="calborder_day" style=" width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b style="color:' . $evented_color . '">' . $i . '</b>';
        $r = 0;
        for ($j = 0; $j < $k; $j++) {
          if ($r < $number_of_shown_evetns) {
            $content .= '
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '" style="background:none !important;color:' . $event_title_color . ' !important;">';
                $content .= '
                <b>' . $ev_title[$j] . '</b>
              </a>';
          }
          else {
            $content .= '<br />
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'all_events', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '" style="font-size:11px !important;background:none !important;color:' . $event_title_color . ' !important;text-align:center !important;">';
                $content .= '
                <b>' . t('See more...') . '</b>
              </a>';
            break;
          }
          $r++;
        }
        $content .= '
            </div>
          </td>';
      }
      elseif ($i == date('j') && $month == date('F') && $year == date('Y')) {
        if (in_array($i, $array_days)) {
          $content .= '
            <td class="cala_day" style="background-color:' . $evented_color_bg . ' !important;padding:0 !important; margin:0 !important;line-height:15px !important; border: 1px solid ' . $border_day . ' !important;">
              <b style="color:' . $evented_color . ' !important;font-size:' . $other_days_font_size . 'px">' . $i . '</b>';
          $r = 0;
          for ($j = 0; $j < $k; $j++) {
            if ($r < $number_of_shown_evetns) {
              $content .= '
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '"  rel="{handler: \'iframe\', size: {x: ' . $popup_width . ', y: ' . $popup_height . '}}" style="background:none !important;color:' . $event_title_color . ' !important;">';
                $content .= '
                <b>' . $ev_title[$j] . '</b>
              </a>';
            }
            else {
              $content .= '<br />
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'all_events', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '" style="font-size:11px !important;background:none !important;color:' . $event_title_color . ' !important;text-align:center !important;">';
                $content .= '
                <b>' . t('See more...') . '</b>
              </a>';
              break;
            }
            $r++;
          }
          $content .= '
            </td>';
        }
        else {
          $content .= '
            <td class="calsun_days" style="padding:0 !important; font-size:' . $sundays_font_size . 'px !important; margin:0 !important;line-height:15px !important; border: 1px solid ' . $border_day . ' !important;">
              <b>' . $i . '</b>
            </td>';
        }
      }
      else {
        if (in_array($i, $array_days)) {
          $content .= '
            <td class="cala_day" style="background-color:' . $evented_color_bg . ' !important;padding:0 !important; margin:0 !important;line-height:15px !important;">
              <b style="color:' . $evented_color . ' !important;font-size:' . $other_days_font_size . 'px !important;">' . $i . '</b>';
          $r = 0;
          for ($j = 0; $j < $k; $j++) {
            if ($r < $number_of_shown_evetns) {
              $content .= '
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '"  rel="{handler: \'iframe\', size: {x: ' . $popup_width . ', y: ' . $popup_height . '}}" style="background:none !important;color:' . $event_title_color . ' !important;">';
                $content .= '
                <b>' . $ev_title[$j] . '</b>
              </a>';
            }
            else {
              $content .= '<br />
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'all_events', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\'class="thickbox-previewbigcalendar' . $many_sp_calendar . '" style="font-size:11px !important;background:none !important;color:' . $event_title_color . ' !important;text-align:center !important;">';
                $content .= '
                <b>' . t('See more...') . '</b>
              </a>';
              break;
            }
            $r++;
          }
          $content .= '
            </td>';
        }
        else {
          $content .= '
            <td class="calsun_days" style="padding:0 !important; margin:0 !important;line-height:15px !important;font-size:' . $sundays_font_size . 'px !important;">
              <b>' . $i . '</b>
            </td>';
        }
      }
    }
    elseif ($i == $day_REFERER && $month == $month_REFERER && $year == $year_REFERER) {
      $content .= '
          <td bgcolor="' . $bg_color_selected . '" class="cala_day" style="padding:0 !important; margin:0 !important;line-height:15px !important;">
            <div class="calborder_day" style=" width:' . $cell_width . 'px !important; margin:0 !important; padding:0 !important;">
              <b style="color:' . $evented_color . ' !important;font-size:' . $other_days_font_size . 'px">' . $i . '</b>';
      $r = 0;
      for ($j = 0; $j < $k; $j++) {
        if ($r < $number_of_shown_evetns) {
          $content .= '
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '"  rel="{handler: \'iframe\', size: {x: ' . $popup_width . ', y: ' . $popup_height . '}}" style="background:none !important;color:' . $event_title_color . ' !important;">';
                $content .= '
                <b>' . $ev_title[$j] . '</b>
              </a>';
        }
        else {
          $content .= '<br />
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'all_events', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '" style="font-size:11px !important;background:none !important;color:' . $event_title_color . ' !important;text-align:center !important;">';
                $content .= '
                <b>' . t('See more...') . '</b>
              </a>';
          break;
        }
        $r++;
      }
      $content .= '
            </div>
          </td>';
    }
    else {
      if (($i == (int)date('j')) && (spider_calendar_int_to_month_name($month) == date('F')) && ($year == date('Y'))) {
        if (in_array ($i, $array_days)) {
          $content .= '
          <td class="cala_day" style="background-color:' . $evented_color_bg . ' !important;padding:0 !important; margin:0 !important;line-height:15px !important; border: 3px solid ' . $current_day_border_color . ' !important;">
            <b style="color:' . $evented_color . ' !important;font-size:' . $other_days_font_size . 'px">' . $i . '</b>';
          $r = 0;
          for ($j = 0; $j < $k; $j++) {
            if ($r < $number_of_shown_evetns) {
              $content .= '
            <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '"  rel="{handler: \'iframe\', size: {x: ' . $popup_width . ', y: ' . $popup_height . '}}" style="background:none !important;color:' . $event_title_color . ' !important;">';
              $content .= '
              <b>' . $ev_title[$j] . '</b>
            </a>';
            }
            else {
              $content .= '<br />
            <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'all_events', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\'class="thickbox-previewbigcalendar' . $many_sp_calendar . '"  rel="{handler: \'iframe\', size: {x: ' . $popup_width . ', y: ' . $popup_height . '}}" style="font-size:11px !important;background:none !important;color:' . $event_title_color . ' !important;text-align:center !important;">';
              $content .= '
              <b>' . t('See more...') . '</b>
            </a>';
              break;
            }
            $r++;
          }
          $content .= '
          </td>';
        }
        else {
          $content .= '
          <td style=" color:' . $text_color_this_month_unevented . ' !important;padding:0 !important; margin:0 !important; line-height:15px !important; border: 3px solid ' . $current_day_border_color . ' !important; vertical-align:top !important;">
            <b style="font-size:' . $other_days_font_size . 'px">' . $i . '</b>
          </td>';
        }
      }
      elseif (in_array($i, $array_days)) {
        $content .= '
          <td class="cala_day" style="background-color:' . $evented_color_bg . ' !important;padding:0 !important; margin:0 !important;line-height:15px !important;">
            <b style="color:' . $evented_color . ' !important;font-size:' . $other_days_font_size . 'px !important;">' . $i . '</b>';
        $r = 0;
        for ($j = 0; $j < $k; $j++) {
          if ($r < $number_of_shown_evetns) {
            $content .= '
            <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'view_event', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => TRUE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '" style="background:none !important;color:' . $event_title_color . ' !important;">';
              $content .= '
              <b>' . $ev_title[$j] . '</b>
            </a>';
          }
          else {
            $content .= '
            <p>
              <a href=\'javascript:spider_calendar_createpopup("' . url('see_more', array('query' => array('cal_theme_id' => $cal_theme, 'cal_id' => $cal_id, 'nodeid' => $nodeid, 'task' => 'all_events', 'ev_ids' => $ev_ids_inline, 'date' => $year . '-' . spider_calendar_add_0($month) . '-' . $i, 'eventID' => $ev_id[$j]), 'absolute' => FALSE)) . '", navigator.userAgent.indexOf("Opera") > -1?650:jQuery(window).height(), 1, "testpopup", 5);\' class="thickbox-previewbigcalendar' . $many_sp_calendar . '" style="font-size:11px !important;background:none !important;color:' . $event_title_color . ' !important;text-align:center !important;" >';
                $content .= '
                <b>' . t('See more...') . '</b>
              </a>
            </p>';
            break;
          }
          $r++;
        }
        $content .= '
          </td>';
      }
      else {
        $content .= '
          <td style=" color:' . $text_color_this_month_unevented . ' !important;padding:0 !important; margin:0 !important; line-height:15px !important;border: 1px solid ' . $cell_border_color . ' !important; vertical-align:top !important;">
            <b style="font-size:' . $other_days_font_size . 'px">' . $i . '</b>
          </td>';
      }
    }
    if ($weekday_i % 7 == 0 && $i <> $month_days) {
      $content .= '
        </tr>
        <tr height="' . $cell_height . '" style="!important;line-height:15px;background-color:inherit;">';
      $weekday_i = 0;
    }
    $weekday_i = $weekday_i + 1;
  }
  $weekday_i;
  $next_i = 1;
  if ($weekday_i != 1)
    for ($i = $weekday_i; $i <= 7; $i++) {
      if ($i != 7) {
        $content .= '
          <td class="caltext_color_other_months" style="background-color:' . $bg_color_other_months . ' !important;">
            <span style="font-size:' . $other_days_font_size . 'px !important;">' . $next_i . '</span>
          </td>';
      }
      else {
        $content .= '
          <td class="caltext_color_other_months" style="background-color:' . $bg_color_other_months . ' !important;">
            <span style="font-size:' . $other_days_font_size . 'px !important;">' . $next_i . '</span>
          </td>';
      }
      $next_i = $next_i + 1;
    }
    $content .= '
        </tr>
      </table>';
    $content .= '
      <input type="text" value="1" name="day" style="display:none" />
    </td>
  </tr>
</table>
</div>
</div>';
  return $content;
}

/**
 * Return month name.
 */
function spider_calendar_int_to_month_name($month_number) {
  $ar = array(
    "",
    t("January"),
    t("February"),
    t("March"),
    t("April"),
    t("May"),
    t("June"),
    t("July"),
    t("August"),
    t("September"),
    t("October"),
    t("November"),
    t("December"),
  );
  return $ar[$month_number];
}

/**
 * Add zero after month.
 */
function spider_calendar_add_0($month_num) {
  if ($month_num < 10) {
    return '0' . $month_num;
  }
  return $month_num;
}

/**
 * Return month number.
 */
function spider_calendar_month_num($month_name) {
  for ($month_num = 1; $month_num <= 12; $month_num++) {
    if (date("F", mktime(0, 0, 0, $month_num, 1, 0)) == $month_name) {
      return $month_num;
    }
  }
}

/**
 * Days difference.
 */
function spider_calendar_php_days_difference($begin_date, $end_date) {
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
function spider_calendar_php_get_next_date($begin_date, $repeat) {
  //explode the date by "-" and storing to array
  $date_parts1=explode("-", $begin_date);
  //gregoriantojd() Converts a Gregorian date to Julian Day Count
  $start_date=gregoriantojd($date_parts1[1], $date_parts1[2], $date_parts1[0]);
  return jdtogregorian($start_date+$repeat);
}

/**
 * Return event repeat days depend on repeat method.
 */
function spider_calendar_getdays($cal_id, $cal_theme, $year, $month) {
  $date = date("Y") . '-' . date("n") . '-' . date("d");
  $date = $year . '-' . spider_calendar_add_0($month);
  $year = substr($date, 0, 4);
  $month = substr($date, 5, 2);
  $show_time = db_query("SELECT time_in_cell FROM {spider_calendar_theme_table} WHERE id=:id", array(':id' => $cal_theme))->fetchField();
  $row = db_query("SELECT id FROM {spider_calendar_events_table} WHERE published=:published and (((event_date <= :event_date or event_date like :event_date) and  event_date_end >= :event_date_end) or (event_date_end=:event_date_end and event_date like :event_date)) and calendar=:calendar", array(':published' => 1, ':event_date' => substr($date, 0, 7) . '-01', ':event_date' => db_like(substr($date, 0, 7)) . '%', ':event_date_end' => substr($date, 0, 7) . '-01', ':event_date_end' => '', ':event_date' => db_like(substr($date, 0, 7)) . '%', ':calendar' => $cal_id))->fetchCol();
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
      $t = spider_calendar_php_days_difference($rows->event_date, $rows->event_date_end);
      for ($k = 1; $k <= $t / $repeat; $k++) {
        $next_date = spider_calendar_php_get_next_date($start_date, $repeat);
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
        $t = spider_calendar_php_days_difference($rows->event_date, $rows->event_date_end);
        $r = 0;
        for ($k = 1; $k < $t / $repeat; $k++) {
          $start_date_array[] = $start_date;
          $next_date = spider_calendar_php_get_next_date($start_date, $repeat * 7);
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
