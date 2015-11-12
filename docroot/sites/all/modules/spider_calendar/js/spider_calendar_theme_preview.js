function spider_calendar_get_theme_values() {
  cal_width = window.parent.document.getElementById('edit-theme-width').value;
  bg_top = '#' + window.parent.document.getElementById('edit-header-bgcolor').value;
  bg_bottom = '#' + window.parent.document.getElementById('edit-body-bgcolor').value;
  border_color = '#' + window.parent.document.getElementById('edit-border-color').value;
  text_color_year = '#' + window.parent.document.getElementById('edit-year-color').value;
  text_color_month = '#' + window.parent.document.getElementById('edit-cur-month-color').value;
  color_week_days = '#' + window.parent.document.getElementById('edit-weekdays-color').value;
  text_color_other_months = '#' + window.parent.document.getElementById('edit-omd-fcolor').value;
  text_color_this_month_unevented = '#' + window.parent.document.getElementById('edit-cell-text-color-wevents').value;
  evented_color = '#' + window.parent.document.getElementById('edit-cell-text-color-events').value;
  evented_color_bg = '#' + window.parent.document.getElementById('edit-cell-bgcolor-events').value;
  color_arrow_year = '#' + window.parent.document.getElementById('edit-year-arrow-color').value;
  color_arrow_month = '#' + window.parent.document.getElementById('edit-month-arrow-color').value;
  sun_days = '#' + window.parent.document.getElementById('edit-sundays-tcolor').value;
  event_title_color = '#' + window.parent.document.getElementById('edit-event-title-color').value;
  current_day_border_color = '#' + window.parent.document.getElementById('edit-cur-day-cell-border-color').value;
  cell_border_color = '#' + window.parent.document.getElementById('edit-cell-border-color').value;
  cell_height = window.parent.document.getElementById('edit-cell-height').value;
  popup_width = window.parent.document.getElementById('edit-popup-width').value;
  popup_height = window.parent.document.getElementById('edit-popup-height').value;
  number_of_shown_evetns = window.parent.document.getElementById('edit-displaied-events').value;
  sundays_font_size = window.parent.document.getElementById('edit-sundays-fcolor').value;
  other_days_font_size = window.parent.document.getElementById('edit-days-fsize').value;
  weekdays_font_size = window.parent.document.getElementById('edit-weekdays-fsize').value;
  border_width = window.parent.document.getElementById('edit-border-width').value;
  top_height = window.parent.document.getElementById('edit-header-heidht').value;
  bg_color_other_months = '#' + window.parent.document.getElementById('edit-omd-bgcolor').value;
  sundays_bg_color = '#' + window.parent.document.getElementById('edit-sundays-cell-bgcolor').value;
  weekdays_bg_color = '#' + window.parent.document.getElementById('edit-weekdays-bgcolor').value;
  weekstart = window.parent.document.getElementById('edit-week-start-day').value;
  weekday_sunday_bg_color = '#' + window.parent.document.getElementById('edit-sunday-bgcolor').value;
  border_radius = window.parent.document.getElementById('edit-border-radius').value;
  border_radius2 = border_radius - border_width;
  week_days_cell_height = window.parent.document.getElementById('edit-week-cell-height').value;
  year_font_size = window.parent.document.getElementById('edit-year-font-size').value;
  month_font_size = window.parent.document.getElementById('edit-cur-month-fsize').value;
  arrow_size = window.parent.document.getElementById('edit-arrow-size').value;
  arrow_size_hover = parseInt(arrow_size) + 5;
  next_month_text_color = '#' + window.parent.document.getElementById('edit-next-month-color').value;
  prev_month_text_color = '#' + window.parent.document.getElementById('edit-prev-month-color').value;
  next_month_arrow_color = '#' + window.parent.document.getElementById('edit-next-month-arrow-color').value;
  prev_month_arrow_color = '#' + window.parent.document.getElementById('edit-prev-month-arrow-color').value;
  next_month_font_size = window.parent.document.getElementById('edit-next-month-fsize').value;
  prev_month_font_size = window.parent.document.getElementById('edit-prev-month-fsize').value;
  month_type = window.parent.document.getElementById('edit-month-type').value;
  cell_width = cal_width / 7;
  if (cell_height == '') {
    cell_height = 70;
  }

  var head = document.getElementsByTagName('head')[0],
    style = document.createElement('style'),
    rules = document.createTextNode(

      '#bigcalendar .cala_arrow a:link, #bigcalendar .cala_arrow a:visited{text-decoration:none;background:none;font-size:' + arrow_size + 'px; }' +

        '#bigcalendar td,#bigcalendar tr,  #spiderCalendarTitlesList td,  #spiderCalendarTitlesList tr {border:none;}' +

        '#bigcalendar .general_table{border-radius: ' + border_radius + 'px;}' +

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
}

function spider_calendar_print_preview() {
  document.getElementById('bigcalendar').style.width=cal_width;
  document.getElementById('general_table').style.width=cal_width;
  document.getElementById('general_table').style.border=border_color+' solid '+border_width;
  document.getElementById('general_table').style.backgroundColor=bg_bottom;
  document.getElementById('header_table').style.width=cal_width;
  document.getElementById('top_table').style.backgroundColor=bg_top;
  document.getElementById('calyear_table').style.width=cal_width;
  document.getElementById('calyear_table').style.height=top_height;
  document.getElementById('cala_arrow_year_prev').style.color=color_arrow_year;
  document.getElementById('cala_arrow_year_next').style.color=color_arrow_year;
  document.getElementById('year_span').style.fontSize=year_font_size;
  document.getElementById('year_span').style.color=text_color_year;
  document.getElementById('cala_arrow_month_prev').style.color=prev_month_arrow_color;
  document.getElementById('cala_arrow_month_prev_span').style.color=prev_month_text_color;
  document.getElementById('cala_arrow_month_prev_span').style.fontSize=prev_month_font_size;  
  document.getElementById('cala_arrow_month_next_span').style.color=next_month_text_color;
  document.getElementById('cala_arrow_month_next_span').style.fontSize=next_month_font_size;  
  document.getElementById('current_month').style.fontSize=month_font_size;
  document.getElementById('current_month').style.color=text_color_month;  
  document.getElementById('cala_arrow_month_next').style.color=next_month_arrow_color;
  document.getElementById('week_days_tr').style.height=week_days_cell_height;
  document.getElementById('week_days_tr').style.backgroundColor=weekdays_bg_color;
  document.getElementById('top_td').style.backgroundColor=bg_top;  
  for (var i=1;i<=6;i++) {
    document.getElementById('weekdays'+i).style.width=cell_width;
    document.getElementById('weekdays'+i).style.color=color_week_days;
    document.getElementById('calbottom_border'+i).style.width=cell_width;
  }
  document.getElementById('weekdays_su').style.width=cell_width;
  document.getElementById('weekdays_su').style.color=color_week_days;
  document.getElementById('weekdays_su').style.backgroundColor=weekday_sunday_bg_color;
  document.getElementById('days').style.height=cell_height;
}