<?php

/**
 * @file
 */

class mm_schedule_calendar_plugin_style_list extends calendar_plugin_style {
  function render() {
    if (empty($this->row_plugin) || !$this->has_calendar_row_plugin()) {
      throw new Exception('mm_schedule_calendar_plugin_style_list: The calendar row plugin is required when using the Date List style, but it is missing.');
    }
    if (!$argument = $this->date_argument_handler()) {
      throw new Exception('mm_schedule_calendar_plugin_style_list: A date argument is required when using the Date List style, but it is missing or is not using the default date.');
    }

    // There are date arguments that have not been added by Date Views.
    // They will be missing the information we would need to render the field.
    if (empty($argument->min_date)) {
      return;
    }

    // Add information from the date argument to the view.
    $this->date_info->granularity = $this->granularity();
    $this->date_info->calendar_type = $this->options['calendar_type'];
    $this->date_info->date_arg = $argument->argument;
    $this->date_info->year = date_format($argument->min_date, 'Y');
    $this->date_info->month = date_format($argument->min_date, 'n');;
    $this->date_info->day = date_format($argument->min_date, 'j');
    $this->date_info->week = date_week(date_format($argument->min_date, DATE_FORMAT_DATE));
    $this->date_info->date_range = $argument->date_range;
    $this->date_info->min_date = $argument->min_date;
    $this->date_info->max_date = $argument->max_date;
    $this->date_info->limit = $argument->limit;
    $this->date_info->url = $this->view->get_url();
    $this->date_info->min_date_date = date_format($this->date_info->min_date, DATE_FORMAT_DATE);
    $this->date_info->max_date_date = date_format($this->date_info->max_date, DATE_FORMAT_DATE);
    $this->date_info->forbid = isset($argument->forbid) ? $argument->forbid : FALSE;

    // Add calendar style information to the view.
    $this->date_info->calendar_popup = $this->display->handler->get_option('calendar_popup');
    $this->date_info->style_name_size = $this->options['name_size'];
    $this->date_info->mini = $this->options['mini'];
    $this->date_info->style_with_weekno = $this->options['with_weekno'];
    $this->date_info->style_multiday_theme = $this->options['multiday_theme'];
    $this->date_info->style_theme_style = $this->options['theme_style'];
    $this->date_info->style_max_items = $this->options['max_items'];
    $this->date_info->style_max_items_behavior = $this->options['max_items_behavior'];
    if (!empty($this->options['groupby_times_custom'])) {
      $this->date_info->style_groupby_times = explode(',', $this->options['groupby_times_custom']);
    }
    else {
      $this->date_info->style_groupby_times = calendar_groupby_times($this->options['groupby_times']);
    }
    $this->date_info->style_groupby_field = $this->options['groupby_field'];

    // TODO make this an option setting.
    $this->date_info->style_show_empty_times = !empty($this->options['groupby_times_custom']) ? TRUE : FALSE;

    // Set up parameters for the current view that can be used by the row plugin.
    $display_timezone = date_timezone_get($this->date_info->min_date);
    $this->date_info->display_timezone = $display_timezone;
    $this->date_info->display_timezone_name = timezone_name_get($display_timezone);
    $date = clone($this->date_info->min_date);
    date_timezone_set($date, $display_timezone);
    $this->date_info->min_zone_string = date_format($date, DATE_FORMAT_DATE);
    $date = clone($this->date_info->max_date);
    date_timezone_set($date, $display_timezone);
    $this->date_info->max_zone_string = date_format($date, DATE_FORMAT_DATE);

    $this->render_fields($this->view->result);
    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $rows = $this->row_plugin->render($row);
      foreach ($rows as $item) {
        $item->granularity = $this->date_info->granularity;
        if ($item->ongoing) {
          $item_start = '0000-00-00';   // Make it sort to the top.
          $time_start = 'ongoing';
        }
        else {
          $item_start = date_format($item->calendar_start_date, DATE_FORMAT_DATE);
          $time_start = date_format($item->calendar_start_date, 'H:i:s');
        }
        $item->rendered_fields = $this->rendered_fields[$row_index];
        $items[$item_start][$time_start][] = $item;
      }
    }

    ksort($items);

    return theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $items
      )
    );
  }
}
