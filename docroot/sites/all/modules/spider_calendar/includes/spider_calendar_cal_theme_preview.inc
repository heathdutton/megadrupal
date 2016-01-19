<?php
/**
 * @file
 * Preview calendar theme.
 */

/**
 * Preview calendar theme.
 */
function spider_calendar_theme_preview() {
  echo "<script>
          cal_width = document.getElementById('edit-theme-width').value;
          bg_top = '#' + document.getElementById('edit-header-bgcolor').value;
          bg_bottom = '#' + document.getElementById('edit-body-bgcolor').value;
          border_color = '#' + document.getElementById('edit-border-color').value;
          text_color_year = '#' + document.getElementById('edit-year-color').value;
          text_color_month = '#' + document.getElementById('edit-cur-month-color').value;
          color_week_days = '#' + document.getElementById('edit-weekdays-color').value;
          text_color_other_months = '#' + document.getElementById('edit-omd-fcolor').value;
          text_color_this_month_unevented = '#' + document.getElementById('edit-cell-text-color-wevents').value;
          evented_color = '#' + document.getElementById('edit-cell-text-color-events').value;
          evented_color_bg = '#' + document.getElementById('edit-cell-bgcolor-events').value;
          color_arrow_year = '#' + document.getElementById('edit-year-arrow-color').value;
          color_arrow_month = '#' + document.getElementById('edit-month-arrow-color').value;
          sun_days = '#' + document.getElementById('edit-sundays-tcolor').value;
          event_title_color = '#' + document.getElementById('edit-event-title-color').value;
          current_day_border_color = '#' + document.getElementById('edit-cur-day-cell-border-color').value;
          cell_border_color = '#' + document.getElementById('edit-cell-border-color').value;
          cell_height = document.getElementById('edit-cell-height').value;
          popup_width = document.getElementById('edit-popup-width').value;
          popup_height = document.getElementById('edit-popup-height').value;
          number_of_shown_evetns = document.getElementById('edit-displaied-events').value;
          sundays_font_size = document.getElementById('edit-sundays-fcolor').value;
          other_days_font_size = document.getElementById('edit-days-fsize').value;
          weekdays_font_size = document.getElementById('edit-weekdays-fsize').value;
          border_width = document.getElementById('edit-border-width').value;
          top_height = document.getElementById('edit-header-heidht').value;
          bg_color_other_months = '#' + document.getElementById('edit-omd-bgcolor').value;
          sundays_bg_color = '#' + document.getElementById('edit-sundays-cell-bgcolor').value;
          weekdays_bg_color = '#' + document.getElementById('edit-weekdays-bgcolor').value;
          weekstart = document.getElementById('edit-week-start-day').value;
          weekday_sunday_bg_color = '#' + document.getElementById('edit-sunday-bgcolor').value;
          border_radius = document.getElementById('edit-border-radius').value;
          border_radius2 = border_radius - border_width;
          week_days_cell_height = document.getElementById('edit-week-cell-height').value;
          year_font_size = document.getElementById('edit-year-font-size').value;
          month_font_size = document.getElementById('edit-cur-month-fsize').value;
          arrow_size = document.getElementById('edit-arrow-size').value;
          arrow_size_hover = parseInt(arrow_size) + 5;
          next_month_text_color = '#' + document.getElementById('edit-next-month-color').value;
          prev_month_text_color = '#' + document.getElementById('edit-prev-month-color').value;
          next_month_arrow_color = '#' + document.getElementById('edit-next-month-arrow-color').value;
          prev_month_arrow_color = '#' + document.getElementById('edit-prev-month-arrow-color').value;
          next_month_font_size = document.getElementById('edit-next-month-fsize').value;
          prev_month_font_size = document.getElementById('edit-prev-month-fsize').value;
          month_type = document.getElementById('edit-month-type').value;
          cell_width = cal_width / 7;
          if (cell_height == '') {
            cell_height = 70;
          }

          var head = document.getElementsByTagName('head')[0],
            style = document.createElement('style'),
            rules = document.createTextNode(

              '#bigcalendar .cala_arrow a:link, #bigcalendar .cala_arrow a:visited{text-decoration:none;background:none;font-size:' + arrow_size + 'px; }' +

                '#bigcalendar td,#bigcalendar tr,  #spiderCalendarTitlesList td,  #spiderCalendarTitlesList tr {border:none;}' +

                '#bigcalendar .general_table_preview{border-radius: ' + border_radius + 'px;}' +

                '#bigcalendar .top_table {border-top-left-radius: ' + border_radius2 + 'px;border-top-right-radius: ' + border_radius2 + 'px;}' +

                '#bigcalendar .cala_arrow a:hover{font-size:' + arrow_size_hover + 'px;text-decoration:none;background:none;}' +

                '#bigcalendar .cala_day a:link, #bigcalendar .cala_day a:visited {text-decoration:none;background:none;font-size:12px;color:red;}' +

                '#bigcalendar .cala_day a:hover {text-decoration:none;background:none;}' +

                '#bigcalendar .cala_day {border:1px solid ' + cell_border_color + ';vertical-align:top;}' +

                '#bigcalendar .weekdays {border:1px solid ' + cell_border_color + '}' +

                '#bigcalendar .week_days {font-size:' + weekdays_font_size + 'px;font-family:arial}' +

                '#bigcalendar .calyear_table, .calmonth_table {border-spacing:0;width:100%; }' +

                '#bigcalendar .calbg, #bigcalendar .calbg td {text-align:center;	width:14%;}' +

                '#bigcalendar .caltext_color_other_months  {color:' + text_color_other_months + ';border:1px solid ' + cell_border_color + ';vertical-align:top;}' +

                '#bigcalendar .caltext_color_this_month_unevented {color:' + text_color_this_month_unevented + ';}' +

                '#bigcalendar .calfont_year {font-family:arial;font-size:24px;font-weight:bold;color:' + text_color_year + ';}' +

                '#bigcalendar .calsun_days {color:' + sun_days + ';border:1px solid ' + cell_border_color + ';vertical-align:top;text-align:left;background-color:' + sundays_bg_color + ';}'
            );

          style.type = 'text/css';
          if (style.styleSheet) {
            style.styleSheet.cssText = rules.nodeValue;
          }
          else {
            style.appendChild(rules);
          }
          head.appendChild(style);
       </script>";
  echo '<style>
          #sliding_popup .content { position:absolute; left:50%; top:70px; display:block;  height:100%; }
          #sliding_popup .bigcalendar * { margin-left:300px; display:block;}
          #sliding_popup .close { overflow:hidden; display:block; position:absolute; right:10%; bottom:90%; width:32px; height:32px; text-indent:1000px; background-image:url("' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/close.png"); }
        </style>
        <div style="width:100%;height:120%;background-color:#000000;opacity:0.80;"></div>
        <div id="div_content" class="content" style="z-index:1000000;">
          <script>
            document.getElementById("div_content").style.width=cal_width+"px";
            document.getElementById("div_content").style.marginLeft=(-cal_width/2)+"px";
          </script>
          <div id="bigcalendar" style="top:30px;">
            <table cellpadding="0" cellspacing="0" id="general_table_preview" class="general_table_preview" style="overflow:hidden;border-spacing:0; margin:0; padding:0;border-collapse:separate;">
              <tr>
                <td width="100%" style=" padding:0; margin:0">
                  <table cellpadding="0" id="header_table" cellspacing="0" border="0" style="border-spacing:0; font-size:12px; margin:0; padding:0;border-collapse:separate; overflow:hidden;">
                  <tr style="height:40px;">
                    <td id="top_table" class="top_table" align="center" colspan="7" style="padding:0; margin:0;height:20px; border:none;">
                      <table cellpadding="0" cellspacing="0" border="0" align="center" class="calyear_table" id="calyear_table" style="margin:0; padding:0; text-align:center;border:none;">
                        <tr>
                          <td style="width:100%;vertical-align:bottom;padding-bottom:0px;border:none;">
                            <table style="width:100%;border:none;margin:0px;">
                              <tr>
                                <td class="cala_arrow" width="40%" style="text-align:right;margin:0px;padding:0px">
                                  <a id="cala_arrow_year_prev" style="" href="">&#9668;</a>
                                </td>
                                <td style="text-align:center; margin:0; padding:0;" width="20%">
                                  <span id="year_span" style="font-family:arial;font-weight:bold;">2012</span>
                                </td>
                                <td style="margin:0;padding:0;text-align:left;border:none;" width="40%" class="cala_arrow">
                                  <a id="cala_arrow_year_next" style="" href="">&#9658;</a>
                                </td>
                              </tr>
                            </table>
                          </td>
                        </tr>
                        <tr>
                          <td style="width:100%;vertical-align:bottom; padding-bottom:5px;border:none;">
                            <table style="width:100%;line-height:150%;border:none;margin:0px;">
                              <tr>
                                <td class="cala_arrow" width="40%" style="text-align:left;margin:0px;padding:0px">
                                  <table width="80%" style="border:none;margin:0px;">
                                    <tr>
                                      <td width="15%">
                                        <a id="cala_arrow_month_prev" style="" href="">&#9668; </a>
                                      </td>
                                      <td style="border:none;">
                                        <span id="cala_arrow_month_prev_span" style="font-family:arial;">' . t('May') . '</span>
                                      </td>
                                    </tr>
                                  </table>
                                </td>
                                <td style="text-align:center; margin:0;" width="20%">
                                  <span id="current_month" style="font-family:arial;">' . t('June') . '</span>
                                </td>
                                <td style="margin:0; padding:0;text-align:right;border:none;" width="40%" class="cala_arrow">
                                  <table width="100%" style="border:none;margin:0px;">
                                    <tr>
                                      <td style="margin:0; padding:0;text-align:right">
                                        <span id="cala_arrow_month_next_span" style="font-family:arial;">' . t('July') . '</span>&nbsp;&nbsp;&nbsp;
                                      </td>
                                      <td width="10%" style="border:none;">
                                        <a id="cala_arrow_month_next" href="">&#9658;</a>
                                      </td>
                                    </tr>
                                  </table>
                                </td>
                              </tr>
                            </table>
                          </td>
                        </tr>
                      </table>
                    </td>
                    <td id="top_td" colspan="7" style="margin:0; padding:0;">
                    </td>
                  </tr>
                  <tr align="center" id="week_days_tr" style="">
                    <td id="weekdays1" class="weekdays" style="margin:0; padding:0">
                      <div id="calbottom_border1" class="calbottom_border" style="text-align:center; margin:0; padding:0;">
                        <b class="week_days">' . t('Monday') . '</b>
                      </div>
                    </td>
                    <td id="weekdays2" class="weekdays" style="margin:0; padding:0">
                      <div id="calbottom_border2" class="calbottom_border" style="text-align:center; margin:0; padding:0;">
                        <b class="week_days">' . t('Tuesday') . '</b>
                      </div>
                    </td>
                    <td id="weekdays3" class="weekdays" style="margin:0; padding:0">
                      <div id="calbottom_border3" class="calbottom_border" style="text-align:center; margin:0; padding:0;">
                        <b class="week_days">' . t('Wednesday') . '</b>
                      </div>
                    </td>
                    <td id="weekdays4" class="weekdays" style=" margin:0; padding:0">
                      <div id="calbottom_border4" class="calbottom_border" style="text-align:center;margin:0; padding:0;">
                        <b class="week_days">' . t('Thursday') . '</b>
                      </div>
                    </td>
                    <td id="weekdays5" class="weekdays" style="margin:0; padding:0">
                      <div id="calbottom_border5" class="calbottom_border" style="text-align:center;margin:0; padding:0;">
                        <b class="week_days">' . t('Friday') . '</b>
                      </div>
                    </td>
                    <td id="weekdays6" class="weekdays" style=" margin:0; padding:0">
                      <div id="calbottom_border6" class="calbottom_border" style="text-align:center;margin:0; padding:0;">
                        <b class="week_days">' . t('Saturday') . '</b>
                      </div>
                    </td>
                    <td id="weekdays_su" class="weekdays" style=" margin:0; padding:0;">
                      <div id="calbottom_border_su" class="calbottom_border" style="text-align:center;  margin:0; padding:0;">
                        <b class="week_days">' . t('Sunday') . '</b>
                      </div>
                    </td>
                  </tr>';
          
          $weekday_i = 6;
          $month_days = 30;
          $last_month_days = 31;
          $last_month_days = $last_month_days - $weekday_i + 2;
          $percent = 1;
          $weekstart = 'mo';
          $sum = $month_days - 8 + 6;
          if ($sum % 7 <> 0) {
            $percent = $percent + 1;
          }
          $sum = $sum - ($sum % 7);
          $percent = $percent + ($sum / 7);
          $percent = 107 / $percent;
          $array_days = array(11);
          $array_days1 = $array_days;
          $title = array(
            11 => '<br>
                   1. ' . t('Event1') . '<br>
                   2. ' . t('Event2') . '<br>
                   3. ' . t('Event3') . '<br>
                   4. ' . t('Event4'),
          );
          $ev_ids = array(
            11 => '97<br>
                   98<br>
                   99<br>
                   100');
          $day_REFERER = '';
          $month = t('June');
          $year = '2012';
          $number_of_shown_evetns = 2;
          echo '<tr  id="days" style="line-height:15px;">
                  <script>
                    document.getElementById("days").style.height=cell_height+"px";
                  </script>';
          for ($i = 1; $i < 6; $i++) {
            echo '<td id="td_last_month_days_' . $i . '" class="caltext_color_other_months">
                                            <script>
                                              document.getElementById("td_last_month_days_' . $i . '").style.border="1px solid " + cell_border_color;
                                              document.getElementById("td_last_month_days_' . $i . '").style.backgroundColor=bg_color_other_months;
                                            </script>
                                            <span id="span_last_month_days' . $i . '">' . $last_month_days . '
                                              <script>
                                                document.getElementById("span_last_month_days' . $i . '").style.fontSize=other_days_font_size+"px";
                                              </script>
                                            </span>
                                           </td>';
            $last_month_days = $last_month_days + 1;
          }
          for ($i = 1; $i <= $month_days; $i++) {
            if ($i == 11) {
              $ev_title = explode('</p>', $title[11]);
              $k = count($ev_title);
              $ev_id = explode('<br>', $ev_ids[$i]);
              array_pop($ev_id);
            }
            $dayevent = '';
            if (($weekday_i % 7 == 0 && $weekstart == "mo") || ($weekday_i % 7 == 1 && $weekstart == "su")) {
              if ($i == $day_REFERER && $month == $month_REFERER && $year == $year_REFERER ) {
                echo '<td id="td_bg_color_selected" class="cala_day" style="padding:0; margin:0;line-height:15px;">
                        <script>
                          document.getElementById("td_bg_color_selected").style.backgroundColor=bg_color_selected;
                        </script>
                        <div id="div_bg_color_selected" class="calborder_day" style="margin:0; padding:0;">
                          <script>
                            document.getElementById("div_bg_color_selected").style.width=cell_width+"px";
                          </script>
                          <b id="b_bg_color_selected">' . $i . '
                            <script>
                              document.getElementById("b_bg_color_selected").style.color=evented_color;
                            </script>
                          </b>';
                $r = 0;
                for ($j = 0; $j < $k; $j++) {
                  if ($r < $number_of_shown_evetns) {
                    echo '<a id="a_event_title" class="modal"  style="background:none;text-decoration:underline;" href="">
                            <script>
                              document.getElementById("a_event_title").style.color=event_title_color;
                            </script>
                            <b><br />1.   ' . t('Event1') . '<br />2.   ' . t('Event2') . '<br />3.   ' . t('Event3') . '<br /></b>
                          </a>';
                  }
                  $r++;
                }
                echo '</td>
                  </div>';
              }
              else
                if ($i == date('j') && $month == date('F') && $year == date('Y')) {
                  if (in_array($i, $array_days)) {
                    echo '<td id="td_evented_color_bg" class="cala_day" style="vertical-align:top;padding:0; margin:0;line-height:15px;">
                            <script>
                              document.getElementById("td_evented_color_bg").style.backgroundColor=evented_color_bg;
                              document.getElementById("td_evented_color_bg").style.border="1px solid "+border_day;
                            </script>
                            <b style="color:' . $evented_color . ';font-size:' . $other_days_font_size . 'px">' . $i . '</b>';
                    $r = 0;
                    for ($j = 0; $j < $k; $j++) {
                      if ($r < $number_of_shown_evetns) {
                        echo '<a id="a_popup_width" class="modal" style="background:none;text-decoration:underline;" href="">
                                <script>
                                  document.getElementById("a_popup_width").style.color=event_title_color;
                                  document.getElementById("a_popup_width").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                </script>
                                <b><br />1.   ' . t('Event1') . '<br />2.   ' . t('Event2') . '3.   ' . t('Event3') . '<br /></b>
                             </a>';
                        echo '<a id="a_popup_width_see_more" class="modal" style="background:none;text-align:center;text-decoration:underline;" href="">
                                <script>
                                  document.getElementById("a_popup_width_see_more").style.color=event_title_color;
                                  document.getElementById("a_popup_width_see_more").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                </script>
                                <b>' . t('See More') . '</b>
                              </a>';
                      }
                    }
                    echo '</td>';
                  }
                  else {
                    echo '<td class="calsun_days" id="calsun_days' . $i . '" style="vertical-align:top;padding:0;margin:0;line-height:15px;">
                            <script>
                              document.getElementById("calsun_days' . $i . '").style.fontSize=sundays_font_size+"px";
                              document.getElementById("calsun_days' . $i . '").style.border="1px solid "+cell_border_color;
                            </script>
                              <b>' . $i . '</b>
                          </td>';
                  }
                }
                else {
                  if (in_array($i, $array_days)) {
                    echo '<td id="td_evented_color_bg" class="cala_day" style="vertical-align:top;padding:0; margin:0;line-height:15px;">
                            <script>
                              document.getElementById("td_evented_color_bg").style.backgroundColor=evented_color_bg;
                              document.getElementById("td_evented_color_bg").style.border="1px solid "+border_day;
                            </script>
                            <b id="b_evented_color_bg">' . $i . '
                              <script>
                                document.getElementById("b_evented_color_bg").style.color=evented_color;
                                document.getElementById("b_evented_color_bg").style.fontSize=other_days_font_size+"px";
                              </script>
                            </b>';
                    $r = 0;
                    for ($j = 0; $j < $k; $j++) {
                      if ($r < $number_of_shown_evetns) {
                        echo '<a id="a_number_of_shown_evetns" class="modal"   style="background:none;text-decoration:underline;" href="">
                                <script>
                                  document.getElementById("a_number_of_shown_evetns").style.color=event_title_color;
                                  document.getElementById("a_number_of_shown_evetns").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                </script>
                                  <b><br />1.   ' . t('Event1') . '<br />2.   ' . t('Event2') . '<br />3.   ' . t('Event3') . '<br /></b>
                              </a>';
                        echo '<a id="a_number_of_shown_evetns_see_more" class="modal"  style="background:none;text-align:center;text-decoration:underline;" href="">
                                <script>
                                  document.getElementById("a_number_of_shown_evetns_see_more").style.color=event_title_color;
                                  document.getElementById("a_number_of_shown_evetns_see_more").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                </script>
                                <b>' . t('See More') . '</b>
                              </a>';
                      }
                    }
                    echo '</td>';
                  }
                  else {
                    echo '<td class="calsun_days" id="calsun_days' . $i . '" style="vertical-align:top;padding:0;margin:0;line-height:15px;">
                            <script>
                              document.getElementById("calsun_days' . $i . '").style.fontSize=sundays_font_size+"px";
                              document.getElementById("calsun_days' . $i . '").style.border="1px solid "+cell_border_color;
                            </script>
                            <b>' . $i . '</b>
                          </td>';
                  }
                }
              }
              else
                if ($i == $day_REFERER && $month == $month_REFERER && $year == $year_REFERER) {
                  echo '<td id="td_bg_color_selected" class="cala_day" style="padding:0; margin:0;line-height:15px;">
                          <script>
                            document.getElementById("td_bg_color_selected").style.backgroundColor=bg_color_selected;
                          </script>
                          <div id="div_cell_width" class="calborder_day" style="margin:0;padding:0;">
                            <script>
                              document.getElementById("div_cell_width").style.width=cell_width+"px";
                            </script>
                            <b id="b_cell_width">' . $i . '</b>
                            <script>
                              document.getElementById("div_cell_width").style.color=evented_color;
                              document.getElementById("div_cell_width").style.fontSize=other_days_font_size+"px";
                            </script>';
                  $r = 0;
                  for ($j = 0; $j < $k; $j++) {
                    if ($r < $number_of_shown_evetns) {
                      echo '<a id="a_number_of_shown_evetns_popup" class="modal" style="background:none;text-decoration:underline;" href="">
                              <script>
                                document.getElementById("a_number_of_shown_evetns_popup").style.color=event_title_color;
                                document.getElementById("a_number_of_shown_evetns_popup").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                              </script>
                              <b><br />1.   ' . t('Event1') . '<br />2.   ' . t('Event2') . '<br />3.   ' . t('Event3') . '<br /></b>
                            </a>';
                      echo '<a id="a_number_of_shown_evetns_popup_see_more" class=\'modal\' style="background:none;text-align:center;text-decoration:underline;" href="">
                              <script>
                                document.getElementById("a_number_of_shown_evetns_popup_see_more").style.color=event_title_color;
                                document.getElementById("a_number_of_shown_evetns_popup_see_more").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                              </script>
                              <b>' . t('See More') . '</b>
                            </a>';
                    }
                  }
                  echo '</td>
                    </div>';
                }
                else {
                  if ($i == 13 && $month == 'June' && $year=='2012') {
                    if (in_array($i, $array_days)) {
                      echo '<td id="td_evented_color_bg" class="cala_day" style="vertical-align:top;padding:0;margin:0;line-height:15px;">
                              <script>
                                document.getElementById("td_evented_color_bg").style.backgroundColor=evented_color_bg;
                                document.getElementById("td_evented_color_bg").style.border="3px solid "+current_day_border_color;
                              </script>
                              <b id="b_evented_color_bg">' . $i . '
                                <script>
                                  document.getElementById("b_evented_color_bg").style.color=evented_color;
                                  document.getElementById("b_evented_color_bg").style.fontSize=other_days_font_size+"px";
                                </script>
                              </b>';
                      $r = 0;
                      for ($j = 0; $j < $k; $j++) {
                        if ($r < $number_of_shown_evetns) {
                          echo '<a id="a_number_of_shown_evetns_popup_width" class="modal" style="background:none;text-decoration:underline;" href="">
                                  <script>
                                    document.getElementById("a_number_of_shown_evetns_popup_width").style.color=event_title_color;
                                    document.getElementById("a_number_of_shown_evetns_popup_width").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                  </script>
                                  <b><br />1.   ' . t('Event1') . '<br />2.   ' . t('Event2') . '<br />3.   ' . t('Event3') . '<br /></b>
                                </a>';
                          echo '<a id="a_number_of_shown_evetns_popup_width_see_more" class="modal" style="background:none;text-align:center;text-decoration:underline;" href="">
                                  <script>
                                    document.getElementById("a_number_of_shown_evetns_popup_width_see_more").style.color=event_title_color;
                                    document.getElementById("a_number_of_shown_evetns_popup_width_see_more").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                  </script>
                                  <b>' . t('See More') . '</b>
                                </a>';
                        }
                        else {
                          break;
                        }
                        $r++;
                      }
                      echo '</td>';
                    }
                    else {
                      echo '<td id="td_current_day_border_color" style="padding:0;margin:0;line-height:15px;vertical-align:top;">
                              <script>
                                document.getElementById("td_current_day_border_color").style.color=text_color_this_month_unevented;
                                document.getElementById("td_current_day_border_color").style.border="3px solid "+current_day_border_color;
                              </script>
                              <b id="b_current_day_border_color">' . $i . '</b>
                                <script>
                                  document.getElementById("b_current_day_border_color").style.fontSize=other_days_font_size+"px";
                                </script>
                            </td>';
                    }
                  }
                  else
                    if (in_array($i, $array_days)) {
                      echo '<td id="td_evented_color_bg" class="cala_day" style="vertical-align:top;padding:0; margin:0;line-height:15px;">
                              <script>
                                document.getElementById("td_evented_color_bg").style.backgroundColor=evented_color_bg;
                              </script>
                              <b id="b_evented_color_bg">' . $i . '
                                <script>
                                  document.getElementById("td_evented_color_bg").style.color=evented_color;
                                  document.getElementById("td_evented_color_bg").style.fontSize=other_days_font_size+"px";
                                </script>
                              </b>';
                      $r = 0;
                      for ($j = 0; $j < $k; $j++) {
                        if ($r < $number_of_shown_evetns) {
                          echo '<a id="a_number_shown_evetns" class="modal" style="background:none;text-decoration:underline;" href="">
                                  <script>
                                    document.getElementById("a_number_shown_evetns").style.color=event_title_color;
                                    document.getElementById("a_number_shown_evetns").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                  </script>
                                  <b><br />1.   ' . t('Event1') . '<br />2.  ' . t('Event2') . '<br />3.   ' . t('Event3') . '<br /></b>
                                </a>';
                          echo '<a id="a_number_shown_evetns_see_more" class="modal" style="background:none;text-align:center;text-decoration:underline;" href="">
                                  <script>
                                    document.getElementById("a_number_shown_evetns_see_more").style.color=event_title_color;
                                    document.getElementById("a_number_shown_evetns_see_more").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                                  </script>
                                  <b>' . t('See More') . '</b>
                                </a>';
                        }
                      }
                      echo '</td>';
                    }
                    else {
                      echo '<td id="td_cell_border_color' . $i . '" style="padding:0; margin:0; line-height:15px;vertical-align:top;">
                              <script>
                                document.getElementById("td_cell_border_color' . $i . '").style.color=text_color_this_month_unevented;
                                document.getElementById("td_cell_border_color' . $i . '").style.border="1px solid "+cell_border_color;
                              </script>
                              <b id="b_cell_border_color' . $i . '">' . $i . '
                                <script>
                                  document.getElementById("b_cell_border_color' . $i . '").style.fontSize=other_days_font_size+"px";
                                </script>
                              </b>
                            </td>';
                    }
                  }
                  if ($weekday_i % 7 == 0 && $i <> $month_days) {
                    echo '</tr><tr id="tr_cell_height' . $i . '" style="line-height:15px">
                            <script>
                              document.getElementById("tr_cell_height' . $i . '").style.height=cell_height+"px";
                            </script>';
                    $weekday_i = 0;
                  }
                  $weekday_i = $weekday_i + 1;
          }
          $weekday_i;
          $next_i = 1;
          if ($weekday_i != 1)
            for ($i = $weekday_i; $i <= 7; $i++) {
              if ($i != 7) {
                echo '<td id="td_bg_color_other_months" class="caltext_color_other_months">' . $next_i . '
                        <script>
                          document.getElementById("td_bg_color_other_months").style.backgroundColor=bg_color_other_months;
                        </script>
                      </td>';
              }
              else {
                echo '<td id="td_bg_color_other_months" class="caltext_color_other_months" style=\'background-color:"+bg_color_other_months+";\'  >' . $next_i . '
                        <script>
                          document.getElementById("td_bg_color_other_months").style.backgroundColor=bg_color_other_months;
                        </script>
                      </td>';
              }
              $next_i = $next_i + 1;
            }
          echo '</tr>
              </table>';
          echo '<input type="text" value="1" name="day" style="display:none"/>
              </td>
             </tr>
            </table>
           </div>';
           echo "<script>
                  document.getElementById('bigcalendar').style.width=cal_width+'px';
                  document.getElementById('general_table_preview').style.width=cal_width+'px';
                  document.getElementById('general_table_preview').style.border=border_color+' solid '+border_width+'px';
                  document.getElementById('general_table_preview').style.backgroundColor=bg_bottom;
                  document.getElementById('header_table').style.width=cal_width+'px';
                  document.getElementById('top_table').style.backgroundColor=bg_top;
                  document.getElementById('calyear_table').style.width=cal_width+'px';
                  document.getElementById('calyear_table').style.height=top_height+'px';
                  document.getElementById('cala_arrow_year_prev').style.color=color_arrow_year;
                  document.getElementById('cala_arrow_year_next').style.color=color_arrow_year;
                  document.getElementById('year_span').style.fontSize=year_font_size+'px';
                  document.getElementById('year_span').style.color=text_color_year;
                  document.getElementById('cala_arrow_month_prev').style.color=prev_month_arrow_color;
                  document.getElementById('cala_arrow_month_prev_span').style.color=prev_month_text_color;
                  document.getElementById('cala_arrow_month_prev_span').style.fontSize=prev_month_font_size+'px';  
                  document.getElementById('cala_arrow_month_next_span').style.color=next_month_text_color;
                  document.getElementById('cala_arrow_month_next_span').style.fontSize=next_month_font_size+'px';  
                  document.getElementById('current_month').style.fontSize=month_font_size+'px';
                  document.getElementById('current_month').style.color=text_color_month;  
                  document.getElementById('cala_arrow_month_next').style.color=next_month_arrow_color;
                  document.getElementById('week_days_tr').style.height=week_days_cell_height+'px';
                  document.getElementById('week_days_tr').style.backgroundColor=weekdays_bg_color;
                  document.getElementById('top_td').style.backgroundColor=bg_top;  
                  for (var i=1;i<=6;i++) {
                    document.getElementById('weekdays'+i).style.width=cell_width+'px';
                    document.getElementById('weekdays'+i).style.color=color_week_days;
                    document.getElementById('calbottom_border'+i).style.width=cell_width+'px';
                  }
                  document.getElementById('weekdays_su').style.width=cell_width+'px';
                  document.getElementById('weekdays_su').style.color=color_week_days;
                  document.getElementById('weekdays_su').style.backgroundColor=weekday_sunday_bg_color;
                  document.getElementById('days').style.height=cell_height+'px';
                </script>";
  echo '
        </div>
        <a class="close" href="javascript:spider_calendar_destroypopup(1000);" style="z-index:1000001">Close</a>';
}
