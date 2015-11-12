<?php
/**
 * @file
 * Preview block calendar theme.
 */

/**
 * Preview block calendar theme.
 */
function spider_calendar_block_theme_preview() {
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
          // popup_width = document.getElementById('edit-popup-width').value;
          // popup_height = document.getElementById('edit-popup-height').value;
          // number_of_shown_evetns = document.getElementById('edit-displaied-events').value;
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
          all_day_border_width = document.getElementById('edit-all-days-border').value;
          month_type = document.getElementById('edit-month-type').value;
          cell_width = cal_width / 7;
          if (cell_height == '') {
            cell_height = 70;
          }

          var head = document.getElementsByTagName('head')[0],
          style = document.createElement('style'),
          rules = document.createTextNode(
        
            '#bigcalendar .cala_arrow a:link, #bigcalendar .cala_arrow a:visited{text-decoration:none;background:none;font-size:'+arrow_size+'px; }'+

            '#bigcalendar td,#bigcalendar tr,  #spiderCalendarTitlesList td,  #spiderCalendarTitlesList tr {border:none;}'+

            '#bigcalendar .general_table_block_preview{border-radius: '+border_radius+'px;}'+

            '#bigcalendar .top_table {border-top-left-radius: '+border_radius2+'px;border-top-right-radius: '+border_radius2+'px;}'+

            '#bigcalendar .cala_arrow a:hover{font-size:'+arrow_size_hover+'px;text-decoration:none;background:none;}'+ 

            '#bigcalendar .cala_day a:link, #bigcalendar .cala_day a:visited {text-decoration:none;background:none;font-size:12px;color:red;}'+ 

            '#bigcalendar .cala_day a:hover {text-decoration:none;background:none;}'+

            '#bigcalendar .cala_day {border:'+all_day_border_width+'px solid '+cell_border_color+';vertical-align:middle;text-align:center}'+ 

            '#bigcalendar .weekdays {border:'+all_day_border_width+'px solid '+cell_border_color+'}'+

            '#bigcalendar .week_days {font-size:'+weekdays_font_size+'px;font-family:arial}'+

            '#bigcalendar .calyear_table, .calmonth_table {border-spacing:0;width:100%; }'+

            '#bigcalendar .calbg, #bigcalendar .calbg td {text-align:center;	width:14%;}'+

            '#bigcalendar .caltext_color_other_months  {color:'+text_color_other_months+';border:'+all_day_border_width+'px solid '+cell_border_color+';vertical-align:middle;text-align:center;}'+

            '#bigcalendar .caltext_color_this_month_unevented {color:'+text_color_this_month_unevented+';}'+

            '#bigcalendar .calfont_year {font-family:arial;font-size:24px;font-weight:bold;color:'+text_color_year+';}'+

            '#bigcalendar .calsun_days {color:'+sun_days+';border:'+all_day_border_width+'px solid '+cell_border_color+';vertical-align:middle;text-align:center;background-color:'+sundays_bg_color+';}'
          );

        style.type = 'text/css';
        if(style.styleSheet)
            style.styleSheet.cssText = rules.nodeValue;
        else style.appendChild(rules);
        head.appendChild(style);
      </script>";

  echo '<style>
          #sliding_popup .content { position:absolute; left:50%; top:170px; display:block;  height:100%; }
          #sliding_popup .bigcalendar * { margin-left:300px; display:block;}
          #sliding_popup .close { overflow:hidden; display:block; position:absolute; right:10%; bottom:90%; width:32px; height:32px; text-indent:1000px; background-image:url("' . base_path() . drupal_get_path('module', 'spider_calendar') . '/images/close.png"); }
        </style>
        <div style="width:100%;height:100%;background-color:#000000;opacity:0.80;"></div>
        <div id="div_content" class="content" style="z-index:1000000;height:60%;">
          <script>
            document.getElementById("div_content").style.width=cal_width+"px";
            document.getElementById("div_content").style.marginLeft=(-cal_width/2)+"px";
          </script>
          <div id="bigcalendar">
            <table cellpadding="0" cellspacing="0" id="general_table_block_preview"  class="general_table_block_preview"  style="overflow:hidden;border-spacing:0; margin:0; padding:0;border:none;">
              <tr>
                <td width="100%" style="padding:0; margin:0;padding:0px;">
                  <table cellpadding="0" id="header_table"  cellspacing="0" border="0" style="border-spacing:0; font-size:12px; margin:0; padding:0;border:none;">
                    <tr  style="height:40px;">
                      <td id="top_table" class="top_table" align="center" colspan="7" style="padding:0; margin:0;height:20px;padding:0px;">
                        <table cellpadding="0" cellspacing="0" border="0" align="center" class="calyear_table" id="calyear_table" style="margin:0; padding:0; text-align:center;border:none;">
                          <tr>
                            <td style="width:100%;vertical-align:bottom;padding-bottom:0px;padding:0px;">
                              <table style="width:100%;border:none;margin:0px;">
                                <tr>
                                  <td  class="cala_arrow" width="40%"  style="text-align:right;margin:0px;padding:0px">
                                    <a id="cala_arrow_year_prev" style=""  href="">&#9668;</a>
                                  </td>
                                  <td style="text-align:center; margin:0; padding:0;" width="20%">
                                    <span id="year_span" style="font-family:arial;font-weight:bold;">2012</span>
                                  </td>
                                  <td style="margin:0; padding:0;text-align:left" width="40%"  class="cala_arrow">
                                    <a id="cala_arrow_year_next" style=""  href="">&#9658;</a>
                                  </td>
                                </tr>
                              </table>
                            </td>
                          </tr>
                          <tr>
                            <td style="width:100%;vertical-align:bottom; padding-bottom:5px;padding:0px;">
                              <table style="width:100%;line-height:150%;border:none;margin:0px;">
                                <tr>
                                  <td class="cala_arrow" width="40%"  style="text-align:left;margin:0px;padding:0px">
                                    <table width="80%" style="border:none;margin:0px;">
                                      <tr>
                                        <td width="15%">
                                          <a id="cala_arrow_month_prev"  style="" href="" >&#9668; </a>
                                        </td>
                                        <td>
                                          <span id="cala_arrow_month_prev_span"  style="font-family:arial;">' . t('May') . '</span>
                                        </td>
                                      </tr>
                                    </table>
                                  </td>
                                  <td style="text-align:center; margin:0;" width="20%">
                                    <span id="current_month"  style="font-family:arial;">' . t('June') . '</span>
                                  </td>
                                  <td style="margin:0; padding:0;text-align:right" width="40%"  class="cala_arrow">
                                    <table width="100%" style="border:none;margin:0px;">
                                      <tr>
                                        <td style="margin:0; padding:0;text-align:right" >
                                          <span id="cala_arrow_month_next_span" style="font-family:arial;">' . t('July') . '</span>
                                        </td>
                                        <td width="10%">
                                          <a  id="cala_arrow_month_next" href="">&#9658;</a>
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
                          <b class="week_days">' . t('Mo') . '</b>
                        </div>
                      </td>
                      <td id="weekdays2" class="weekdays" style="margin:0; padding:0">
                        <div id="calbottom_border2" class="calbottom_border" style="text-align:center; margin:0; padding:0;">
                          <b class="week_days">' . t('Tu') . '</b>
                        </div>
                      </td>
                      <td id="weekdays3" class="weekdays" style="margin:0; padding:0">
                        <div id="calbottom_border3" class="calbottom_border" style="text-align:center; margin:0; padding:0;">
                          <b class="week_days">' . t('We') . '</b>
                        </div>
                      </td>
                      <td id="weekdays4" class="weekdays" style=" margin:0; padding:0">
                        <div id="calbottom_border4" class="calbottom_border" style="text-align:center;margin:0; padding:0;">
                          <b class="week_days">' . t('Th') . '</b>
                        </div>
                      </td>
                      <td id="weekdays5" class="weekdays" style="margin:0; padding:0">
                        <div id="calbottom_border5" class="calbottom_border" style="text-align:center;margin:0; padding:0;">
                          <b class="week_days">' . t('Fr') . '</b>
                        </div>
                      </td>
                      <td id="weekdays6" class="weekdays" style=" margin:0; padding:0">
                        <div id="calbottom_border6" class="calbottom_border" style="text-align:center;margin:0; padding:0;">
                          <b class="week_days">' . t('Sa') . '</b>
                        </div>
                      </td>
                      <td id="weekdays_su" class="weekdays" style=" margin:0; padding:0;">
                        <div id="calbottom_border_su" class="calbottom_border" style="text-align:center; margin:0; padding:0;">
                          <b class="week_days">' . t('Su') . '</b>
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
  $title = array(11 => '');
  $ev_ids = array(
    11 => '97<br />
           98<br />
           99<br />
           100');
  $day_REFERER = '';
  $month = t('June');
  $year = '2012';
  $number_of_shown_evetns = 2;
  echo '<tr id="days" style="line-height:15px;">';
  for ($i = 1; $i < 6; $i++) {
    echo '<td id="td_all_day_border_width' . $i . '" class="caltext_color_other_months" style="vertical-align:middle;text-align:center;padding:0px;">
            <script>
              document.getElementById("td_all_day_border_width' . $i . '").style.border=all_day_border_width+"px solid "+cell_border_color;
              document.getElementById("td_all_day_border_width' . $i . '").style.backgroundColor=bg_color_other_months;
            </script>
            <span id="span_all_day_border_width' . $i . '">' . $last_month_days . '
              <script>
                document.getElementById("span_all_day_border_width' . $i . '").style.fontSize=other_days_font_size+"px";
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
        echo '<td id="td_bg_color_selected_' . $i . '" class="cala_day" style="padding:0; margin:0;line-height:15px;">
                <script>
                  document.getElementById("td_bg_color_selected_' . $i . '").style.backgroundColor=bg_color_selected;
                </script>
                <div id="div_bg_color_selected_' . $i . '" class="calborder_day" style="margin:0; padding:0;">
                  <script>
                    document.getElementById("div_bg_color_selected_' . $i . '").style.width=cell_width+"px";
                  </script>
                  <b id="b_bg_color_selected_' . $i . '">
                    <script>
                      document.getElementById("b_bg_color_selected_' . $i . '").style.color=evented_color;
                    </script>
                    <a>' . $i . '</a>
                  </b>';
        $r = 0;
        for ($j = 0; $j < $k; $j++) {
          if ($r < $number_of_shown_evetns) {
            echo '<a id="a_event_title_color_' . $j . '" class="modal"  style="background:none;text-decoration:underline;" href="" ></b>
                    <script>
                      document.getElementById("a_event_title_color_' . $j . '").style.color=event_title_color;
                    </script>
                  </a>';
          }
          $r++;
        }
        echo '</td></div>';
      }
      else
        if ($i == date('j') and $month == date('F') and $year==date('Y')) {
          if (in_array($i, $array_days)) {
            echo '<td id="td_evented_color_bg_' . $i . '" class="cala_day" style="vertical-align:middle;text-align:center;padding:0; margin:0;line-height:15px;">
                    <script>
                      document.getElementById("td_evented_color_bg_' . $i . '").style.backgroundColor=evented_color_bg;
                      document.getElementById("td_evented_color_bg_' . $i . '").style.border="1px solid "+border_day;
                    </script>
                    <b id="b_evented_color_bg_' . $i . '">
                      <a>' . $i . '</a>
                      <script>
                        document.getElementById("b_evented_color_bg_' . $i . '").style.color=' . $evented_color . ';
                        document.getElementById("b_evented_color_bg_' . $i . '").style.fontSize=' . $other_days_font_size . '"px";
                      </script>
                    </b>';
            $r = 0;
            for ($j = 0; $j < $k; $j++) {
              if ($r < $number_of_shown_evetns) {
                echo '<a id="a_event_title_color_' . $j . '" class="modal" style="background:none;text-decoration:underline;" href="">
                        <script>
                          // document.getElementById("a_event_title_color_' . $j . '").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                          document.getElementById("a_event_title_color_' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
                      </a>';
                echo '<a id="aid_event_title_color_' . $j . '" class="modal" rel="" style="text-align:center;text-decoration:underline;" href=""> 
                        <script>
                          // document.getElementById("aid_event_title_color_' . $j . '").rel="{handler: \'iframe\', size: {x: '+popup_width+', y: '+popup_height+'}}";
                          document.getElementById("aid_event_title_color_' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
                      </a>';
              }
            }
            echo '</td>';
          }
          else {
            echo '<td class="calsun_days" id="calsun_days' . $i . '" style="vertical-align:middle;text-align:center;padding:0;margin:0;line-height:15px;">
                    <script>
                      document.getElementById("calsun_days' . $i . '").style.fontSize=sundays_font_size+"px";
                      document.getElementById("calsun_days' . $i . '").style.border=all_day_border_width+"px solid "+cell_border_color;
                    </script>
                    <b>' . $i . '</b>
                 </td>';
          }
        }
        else {
          if (in_array($i, $array_days)) {
            echo '<td id="td_evented_color_bg' . $i . '" class="cala_day" style="vertical-align:middle;text-align:center;padding:0; margin:0;line-height:15px;">
                    <script>
                      document.getElementById("td_evented_color_bg' . $i . '").style.backgroundColor=evented_color_bg;
                      document.getElementById("td_evented_color_bg' . $i . '").style.border="1px solid "+border_day;
                    </script>
                    <b id="b_evented_color_bg' . $i . '">
                      <script>
                        document.getElementById("b_evented_color_bg' . $i . '").style.color=evented_color;
                        document.getElementById("b_evented_color_bg' . $i . '").style.fontSize=other_days_font_size+"px";
                      </script>
                      <a>' . $i . '</a>
                    </b>';
            $r = 0;
            for ($j = 0; $j < $k; $j++) {
              if ($r < $number_of_shown_evetns) {
                echo '<a id="a_event_title_color' . $j . '" class="modal" style="background:none;text-decoration:underline;" href="">
                        <script>
                          // document.getElementById("a_event_title_color' . $j . '").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                          document.getElementById("a_event_title_color' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
                      </a>';
                echo '<a id="aid_event_title_color' . $j . '" class="modal" style="background:none;text-align:center;text-decoration:underline;" href=""> 
                        <script>
                          // document.getElementById("aid_event_title_color' . $j . '").rel="{handler: \'iframe\', size: {x: '+popup_width+', y: '+popup_height+'}}";
                          document.getElementById("aid_event_title_color' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
                      </a>';
              }
            }
            echo '</td>';
          }
          else {
            echo '<td class="calsun_days" id="calsun_days' . $i . '" style="vertical-align:middle;text-align:center;padding:0;margin:0;line-height:15px;">
                    <script>
                      document.getElementById("calsun_days' . $i . '").style.fontSize=sundays_font_size+"px";
                      document.getElementById("calsun_days' . $i . '").style.border=all_day_border_width+"px solid "+cell_border_color;
                    </script>
                    <b>' . $i . '</b>
                  </td>';
          }
        }
      }
      else
        if ($i == $day_REFERER && $month == $month_REFERER && $year == $year_REFERER) {
          echo '<td id="tdid_bg_color_selected' . $i . '" class="cala_day" style="padding:0; margin:0;line-height:15px;">
                  <script>
                    document.getElementById("tdid_bg_color_selected' . $i . '").style.backgroundColor=bg_color_selected;
                  </script>
                  <div id="divid_bg_color_selected' . $i . '" class="calborder_day" style="margin:0; padding:0;">
                    <script>
                      document.getElementById("divid_bg_color_selected' . $i . '").style.width=cell_width+"px";
                    </script>
                    <b id="bid_bg_color_selected' . $i . '">
                      <script>
                        document.getElementById("bid_bg_color_selected' . $i . '").style.color=evented_color;
                        document.getElementById("bid_bg_color_selected' . $i . '").style.fontSize=other_days_font_size+"px";
                      </script>
                      <a>' . $i . '</a>
                    </b>';
          $r = 0;
          for ($j = 0; $j < $k; $j++) {
            if ($r < $number_of_shown_evetns) {
              echo '<a id="a_popup_width' . $j . '" class="modal" style="background:none;text-decoration:underline;" href="">
                      <script>
                        // document.getElementById("a_popup_width' . $j . '").rel="{handler: \'iframe\', size: {x: '+popup_width+', y: '+popup_height+'}}";
                        document.getElementById("a_popup_width' . $j . '").style.color=event_title_color;
                      </script>
                      <b></b>
                    </a>';
              echo '<a id="aid_popup_width' . $j . '" class="modal" style="background:none;text-decoration:underline;" href=""> 
                      <script>
                        // document.getElementById("aid_popup_width' . $j . '").rel="{handler: \'iframe\', size: {x: '+popup_width+', y: '+popup_height+'}}";
                        document.getElementById("aid_popup_width' . $j . '").style.color=event_title_color;
                      </script>
                      <b></b>
                    </a>';
            }
          }
          echo '</td></div>';
        }
        else {
          if ($i == 13 && $month=='June' && $year=='2012') {
            if (in_array($i, $array_days)) {
              echo '<td id="td_evented_color_bgc_' . $i . '" class="cala_day" style="vertical-align:middle;text-align:center;padding:0; margin:0;line-height:15px;">
                      <script>
                        document.getElementById("td_evented_color_bgc_' . $i . '").style.backgroundColor=evented_color_bg;
                        document.getElementById("td_evented_color_bgc_' . $i . '").style.border="3px solid "+current_day_border_color;
                      </script>
                      <b id="b_evented_color_bgc_' . $i . '">
                        <script>
                          document.getElementById("b_evented_color_bgc_' . $i . '").style.color=evented_color;
                          document.getElementById("b_evented_color_bgc_' . $i . '").style.fontSize=other_days_font_size+"px";
                        </script>
                        <a>' . $i . '</a>
                      </b>';
            $r = 0;
            for ($j = 0; $j < $k; $j++) {
              if ($r < $number_of_shown_evetns) {
                echo '<a id="a_rel_' . $j . '" class="modal" style="background:none;text-decoration:underline;" href="">
                        <script>
                          // document.getElementById("a_rel_' . $j . '").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                          document.getElementById("a_rel_' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
                      </a>';
                echo '<a id="aid_rel_' . $j . '" class="modal" style="background:none;text-decoration:underline;" href=""> 
                        <script>
                          // document.getElementById("aid_rel_' . $j . '").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                          document.getElementById("aid_rel_' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
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
            echo '<td id="td_text_color_this_month_unevented" style="padding:0; margin:0; line-height:15px;vertical-align:middle;text-align:center;">
                    <script>
                      document.getElementById("td_text_color_this_month_unevented").style.color=text_color_this_month_unevented;
                      document.getElementById("td_text_color_this_month_unevented").style.border="3px solid "+current_day_border_color;
                    </script>
                    <b id="b_text_color_this_month_unevented">' . $i . '
                      <script>
                        document.getElementById("b_text_color_this_month_unevented").style.fontSize=other_days_font_size+"px";
                      </script>
                    </b>
                  </td>';
          }
        }
        else
          if (in_array($i, $array_days)) {
            echo '<td id="td_evented_color_bgcolor" class="cala_day" style="vertical-align:middle;text-align:center;padding:0; margin:0;line-height:15px;">
                    <script>
                      document.getElementById("td_evented_color_bgcolor").style.backgroundColor=evented_color_bg;
                    </script>
                    <b id="b_evented_color_bgcolor">
                      <script>
                        document.getElementById("b_evented_color_bgcolor").style.color=evented_color;
                        document.getElementById("b_evented_color_bgcolor").style.fontSize=other_days_font_size+"px";
                      </script>
                    <a>' . $i . '</a></b>';
            $r = 0;
            for ($j = 0; $j < $k; $j++) {
              if ($r < $number_of_shown_evetns) {
                echo '<a id="a_popup_height_' . $j . '" class="modal" style="background:none;text-decoration:underline;" href="">
                        <script>
                          // document.getElementById("a_popup_height_' . $j . '").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                          document.getElementById("a_popup_height_' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
                      </a>';
                echo '<a id="aid_popup_height_' . $j . '" class="modal" style="background:none;text-decoration:underline;" href=""> 
                        <script>
                          // document.getElementById("aid_popup_height_' . $j . '").rel="{handler: \'iframe\', size: {x: "+popup_width+", y: "+popup_height+"}}";
                          document.getElementById("aid_popup_height_' . $j . '").style.color=event_title_color;
                        </script>
                        <b></b>
                      </a>';
              }
            }
            echo '</td>';
          }
          else {
            echo '<td id="tdid_text_color_this_month_unevented_' . $i . '" style="padding:0; margin:0; line-height:15px;vertical-align:middle;text-align:center;text-align:center;">
                    <script>
                      document.getElementById("tdid_text_color_this_month_unevented_' . $i . '").style.border=all_day_border_width+"px solid "+cell_border_color;
                      document.getElementById("tdid_text_color_this_month_unevented_' . $i . '").style.color=text_color_this_month_unevented;
                    </script>
                    <b id="bid_text_color_this_month_unevented_' . $i . '">' . $i . '</b>
                      <script>
                        document.getElementById("bid_text_color_this_month_unevented_' . $i . '").style.fontSize=other_days_font_size+"px";
                        document.getElementById("bid_text_color_this_month_unevented_' . $i . '").style.color=text_color_this_month_unevented;
                      </script>
                  </td>';
          }
        }
              if ($weekday_i % 7 == 0 && $i <> $month_days) {
                echo '</tr><tr id="tr_cell_height_' . $i . '" style="line-height:15px">
                      <script>
                        document.getElementById("tr_cell_height_' . $i . '").style.height=cell_height+"px";
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
                  echo '<td id="tdid_bg_color_other_months' . $i . '" class="caltext_color_other_months">' . $next_i . '
                          <script>
                            document.getElementById("tdid_bg_color_other_months' . $i . '").style.backgroundColor=bg_color_other_months;
                          </script>
                        </td>';
                }
                else {
                  echo '<td id="tdid_bgc_other_months' . $i . '" class="caltext_color_other_months">' . $next_i . '
                          <script>
                            document.getElementById("tdid_bgc_other_months' . $i . '").style.backgroundColor=bg_color_other_months;
                          </script>
                        </td>';
                  echo '<td id="tdid_bc_other_months' . $i . '" class="caltext_color_other_months">' . $next_i . '
                          <script>
                            document.getElementById("tdid_bc_other_months' . $i . '").style.backgroundColor=bg_color_other_months;
                          </script>
                        </td>';
                }
                $next_i = $next_i + 1;
              }
              echo '</tr></table>';
              echo '<input type="text" value="1" name="day" style="display:none"/>
                </td>
              </tr>
            </table>
          </div>';
  echo "<script>
          document.getElementById('bigcalendar').style.width=cal_width+'px';
          document.getElementById('general_table_block_preview').style.width=cal_width+'px';
          document.getElementById('general_table_block_preview').style.border=border_color+' solid '+border_width+'px';
          document.getElementById('general_table_block_preview').style.backgroundColor=bg_bottom;
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
          for (var i = 1; i <= 6; i++) {
            document.getElementById('weekdays'+i).style.width=cell_width+'px';
            document.getElementById('weekdays'+i).style.color=color_week_days;
            document.getElementById('calbottom_border'+i).style.width=cell_width+'px';
          }
          document.getElementById('weekdays_su').style.width=cell_width+'px';
          document.getElementById('calbottom_border_su').style.width=cell_width+'px';
          document.getElementById('weekdays_su').style.color=color_week_days;
          document.getElementById('weekdays_su').style.backgroundColor=weekday_sunday_bg_color;
          document.getElementById('days').style.height=cell_height+'px';
        </script>";
  echo '
        </div>
        <a class="close" href="javascript:spider_calendar_destroypopup(1000);" style="z-index:1000001">Close</a>';
}

