<?php

/**
 * @file
 * Views style plugin for the agres_availability module.
 *
 * The style plugin is responsible for setting global values needed by
 * the row plugin. It then gathers up an array of view results (as restructured
 * by the row plugin) and organizes them into year/month/day/time
 * arrays that the theme can use to display the results in a agres_availability.
 */

/**
 * Default style plugin to render an iCal feed.
 */
class agres_availability_plugin_style extends calendar_plugin_style {
  //
//  var $date_info;
//  var $items;
//  var $curday;
  //
//  function init(&$view, &$display, $options = NULL) {
//    parent::init($view, $display, $options);
//    if (empty($view->date_info)) {
//      $this->date_info = new stdClass();
//    }
//    $this->date_info = &$this->view->date_info;
//  }

  /**
   * Add custom option definitions.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['agres_availability_type'] = array('default' => 'month');
//    $options['name_size'] = array('default' => 3);
//    $options['mini'] = array('default' => 0);
//    $options['with_weekno'] = array('default' => 0);
//    $options['multiday_theme'] = array('default' => 1);
//    $options['theme_style'] = array('default' => 1);
//    $options['max_items'] = array('default' => 0);
//    $options['max_items_behavior'] = array('default' => 'more');
//    $options['groupby_times'] = array('default' => 'hour');
//    $options['groupby_times_custom'] = array('default' => '');
//    $options['groupby_field'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['#title'] .= t('agres_availability period');
    $form['agres_availability_type'] = array(
      '#title' => t('agres_availability type'),
      '#type' => 'select',
      '#description' => t('Select the agres_availability time period for this display.'),
      '#default_value' => $this->options['agres_availability_type'],
      '#options' => agres_availability_display_types(),
    );
    $form['mini'] = array(
      '#title' => t('Display as mini agres_availability'),
      '#default_value' => $this->options['mini'],
      '#type' => 'radios',
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
        2 => t('Yes but only for current unit type'),
        3 => t('Yes but only for current category'), 
      ),
      '#description' => t('Display the mini style agres_availability, with no item details. Suitable for a agres_availability displayed in a block.'),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('month')),
    );
 
    $form['name_size'] = array(
      '#title' => t('agres_availability day of week names'),
      '#default_value' => $this->options['name_size'],
      '#type' => 'radios',
      '#options' => array(
        1 => t('First letter of name'),
        2 => t('First two letters of name'),
        3 => t('Abbreviated name'),
        99 => t('Full name'),
      ),
      '#description' => t('The way day of week names should be displayed in a agres_availability.'),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('month', 'week', 'year')),
    );
    $form['with_weekno'] = array(
      '#title' => t('Show week numbers'),
      '#default_value' => $this->options['with_weekno'],
      '#type' => 'radios',
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
      '#description' => t('Whether or not to show week numbers in the left column of agres_availability weeks and months.'),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('month')),
    );
    $form['max_items'] = array(
      '#title' => t('Maximum items'),
      '#type' => 'select',
      '#options' => array(
        0 => t('Unlimited'),
        3 => t('3 items'),
        5 => t('5 items'),
        10 => t('10 items'),
      ),
      '#default_value' => $agres_availability_type != 'day' ? $this->options['max_items'] : 0,
      '#description' => t('Maximum number of items to show in agres_availability cells, used to keep the agres_availability from expanding to a huge size when there are lots of items in one day.'),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('month')),
    );
    $form['max_items_behavior'] = array(
      '#title' => t('Too many items'),
      '#type' => 'select',
      '#options' => array(
        'more' => t("Show maximum, add 'more' link"),
        'hide' => t('Hide all, add link to day'),
      ),
      '#default_value' => $agres_availability_type != 'day' ? $this->options['max_items_behavior'] : 'more',
      '#description' => t('Behavior when there are more than the above number of items in a single day. When there more items than this limit, a link to the day view will be displayed.'),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('month')),
    );
    $form['groupby_times'] = array(
      '#title' => t('Time grouping'),
      '#type' => 'select',
      '#default_value' => $this->options['groupby_times'],
      '#description' => t("Group items together into time periods based on their start time."),
      '#options' => array(
        '' => t('None'),
        'hour' => t('Hour'),
        'half' => t('Half hour'),
        'custom' => t('Custom'),
      ),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('day', 'week')),
    );
    $form['groupby_times_custom'] = array(
      '#title' => t('Custom time grouping'),
      '#type' => 'textarea',
      '#default_value' => $this->options['groupby_times_custom'],
      '#description' => t("When choosing the 'custom' Time grouping option above, create custom time period groupings as a comma-separated list of 24-hour times in the format HH:MM:SS, like '00:00:00,08:00:00,18:00:00'. Be sure to start with '00:00:00'. All items after the last time will go in the final group."),
      '#dependency' => array('edit-style-options-groupby-times' => array('custom')),
    );
    $form['theme_style'] = array(
      '#title' => t('Overlapping time style'),
      '#default_value' => $this->options['theme_style'],
      '#type' => 'select',
      '#options' => array(
        0 => t('Do not display overlapping items'),
        1 => t('Display overlapping items'),
      ),
      '#description' => t('Select whether agres_availability items are displayed as overlapping items. Only works if some sort of time grouping is chosen!'),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('day', 'week')),
    );

    // Create a list of fields that are available for grouping,
    $field_options = array();
    $fields = $this->display->handler->get_option('fields');
    foreach ($fields as $field_name => $field) {
      $field_options[$field['table'] . '_' . $field['field']] = $field['field'];
    }
    $form['groupby_field'] = array(
      '#title' => t('Field grouping'),
      '#type' => in_array($agres_availability_type, array('day')) ? 'select' : 'value',
      '#default_value' => $this->options['groupby_field'],
      '#description' => t("Optionally group items into columns by a field value, for instance select the content type to show items for each content type in their own column, or use a location field to organize items into columns by location."),
      '#options' => array('' => '') + $field_options,
      '#dependency' => array('edit-style-options-agres_availability-type' => array('day')),
    );
    $form['multiday_theme'] = array(
      '#title' => t('Multi-day style'),
      '#default_value' => $this->options['multiday_theme'],
      '#type' => 'select',
      '#options' => array(0 => t('Display multi-day item as a single column')),
      '#description' => t('If selected, items which span multiple days will displayed as a multi-column row.  If not selected, items will be displayed as an individual column.'),
      '#dependency' => array('edit-style-options-agres_availability-type' => array('month', 'week')),
    );
  }

//  function options_validate(&$form, &$form_state) {
//    $values = $form_state['values']['style_options'];
//    if ($values['groupby_times'] == 'custom' && empty($values['groupby_times_custom'])) {
//      form_set_error('style_options][groupby_times_custom', t('Custom groupby times cannot be empty.'));
//    }
//    if (!empty($values['theme_style']) && empty($values['groupby_times'])) {
//      form_set_error('style_options][theme_style', t('You cannot use overlapping items unless there are groupby times selected.'));
//    }
//  }
  //
//  /**
//   * Helper function to find the date argument handler for this view.
//   */
//  function date_argument_handler() {
//    $i = 0;
//    foreach ($this->view->argument as $name => $handler) {
//      if (date_views_handler_is_date($handler, 'argument')) {
//        $this->date_info->date_arg_pos = $i;
//        return $handler;
//      }
//      $i++;
//    }
//    return FALSE;
//  }

  /**
   * Inspect argument and view information to see which agres_availability
   * period we should show. The argument tells us what to use
   * if there is no value, the view args tell us what to use
   * if there are values.
   */
//  function granularity() {
//
//    if (!$handler = $this->date_argument_handler()) {
//      return 'month';
//    }
//    $default_granularity = !empty($handler) && !empty($handler->granularity) ? $handler->granularity : 'month';
//    $wildcard = !empty($handler) ? $handler->options['exception']['value'] : '';
//    $argument = $handler->argument;
  //
//    // TODO Anything else we need to do for 'all' arguments?
//    if ($argument == $wildcard) {
//      $this->view_granularity = $default_granularity;
//    }
//    elseif (!empty($argument)) {
//      module_load_include('inc', 'date_api', 'date_api_sql');
  //
//      $date_handler = new date_sql_handler();
//      $this->view_granularity = $date_handler->arg_granularity($argument);
//    }
//    else {
//      $this->view_granularity = $default_granularity;
//    }
//    return $this->view_granularity;
//  }

  function render() {
//      drupal_set_message('<pre>$this->row_plugin$this->row_plugin$this->row_plugin'.print_r($this->row_plugin).'</pre>');
    if (empty($this->row_plugin) || $this->row_plugin->definition['handler'] != 'agres_availability_plugin_row_node') {
      debug('agres_availability_plugin_style: Missing agres_availability row plugin');
      return;
    }
    if (!$argument = $this->date_argument_handler()) {
      debug('agres_availability_plugin_style: Missing date argument');
      return;
    }

    if (empty($argument->min_date)) {
      //debug('agres_availability_plugin_style: Empty date argument');
      return;
    }

    // Add information from the date argument to the view.
    $this->date_info->granularity = $this->granularity();
    $this->date_info->agres_availability_type = $this->options['agres_availability_type'];
    $this->date_info->date_arg = $argument->argument;
    $this->date_info->year = date_format($argument->min_date, 'Y');
    $this->date_info->month = date_format($argument->min_date, 'n');
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

    // Add agres_availability style information to the view.
    $this->date_info->agres_availability_popup = $this->display->handler->get_option('agres_availability_popup');
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
      $this->date_info->style_groupby_times = agres_availability_groupby_times($this->options['groupby_times']);
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

    // Identify the fields that need to be displayed on each item.
    $keys = array();
    foreach ($this->view->field as $key => $handler) {
      if (empty($handler->options['exclude'])) {
        $keys[] = $key;
      }
    }

    // Invoke the row plugin to massage each result row into agres_availability items.
    // Gather the row items into an array grouped by date and time.
    $items = array();
    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;

      foreach ($this->row_plugin->render($row) as $item) {
        $item->granularity = $this->date_info->granularity;
        $rendered_fields = array();
        $item_start = date_format($item->agres_availability_start_date, DATE_FORMAT_DATE);
        $item_end = date_format($item->agres_availability_end_date, DATE_FORMAT_DATE);
        $time_start = date_format($item->agres_availability_start_date, 'H:i:s');
        foreach ($keys as $field) {
          $rendered_fields[] = $this->get_field($row_index, $field);
        }
        $item->rendered_fields = $rendered_fields;
        $items[$item_start][$time_start][] = $item;
        $this->view->row_index++;
      }
    }

    ksort($items);

    $rows = array();
    $this->curday = clone($this->date_info->min_date);
    $this->items = $items;

    // Retrieve the results array using a the right method for the granularity of the display.
    switch ($this->options['agres_availability_type']) {
      case 'year':
        $rows = array();
        $this->view->date_info->mini = TRUE;
        for ($i = 1; $i <= 12; $i++) {
          $rows[$i] = $this->agres_availability_build_mini_month();
        }
        $this->view->date_info->mini = FALSE;
        break;
      case 'month':
        $rows = !empty($this->date_info->mini) ? $this->agres_availability_build_mini_month() : $this->agres_availability_build_month();
        break;
      case 'day':
        $rows = $this->agres_availability_build_day();
        break;
      case 'week':
        $rows = $this->agres_availability_build_week();
        // Merge the day names in as the first row.
        $rows = array_merge(array(agres_availability_week_header($this->view)), $rows);
        break;
    }

    // Send the sorted rows to the right theme for this type of agres_availability.
    $this->definition['theme'] = 'agres_availability_' . $this->options['agres_availability_type'];

    // Adjust the theme to match the currently selected default.
    // Only the month view needs the special 'mini' class,
    // which is used to retrieve a different, more compact, theme.
    if ($this->options['agres_availability_type'] == 'month' && !empty($this->view->date_info->mini)) {
      $this->definition['theme'] = 'agres_availability_mini';
    }
 
    // If the overlap option was selected, choose the overlap version of the theme.
    elseif (in_array($this->options['agres_availability_type'], array('week', 'day')) && !empty($this->options['multiday_theme']) && !empty($this->options['theme_style'])) {
      $this->definition['theme'] .= '_overlap';
    }

    $output = theme($this->theme_functions(),
      array(
      'view' => $this->view,
      'options' => $this->options,
      'rows' => $rows,
    ));
    unset($this->view->row_index);
    return $output;
  }

  function agres_availability_build_month() {
    $curday = $this->curday;
    $items  = $this->items;
    $view = $this->view;
    $month = date_format($curday, 'n');
    date_modify($curday, '-' . strval(date_format($curday, 'j') -1) . ' days');

    $rows = array();
    do {
      $rows = array_merge($rows, $this->agres_availability_build_week(TRUE));
      $curday_date = date_format($curday, DATE_FORMAT_DATE);
      $curday_month = date_format($curday, 'n');
//    drupal_set_message('<pre>$curday_month '.print_r( $curday_month,true).'</pre>');
//    drupal_set_message('<pre>*agres_availability_build_month**'. print_r($curday_date, TRUE) .'</pre>');
    } while ($curday_month == $month && $curday_date <= $view->date_info->max_date_date);
    // Merge the day names in as the first row.
    $rows = array_merge(array(agres_availability_week_header($this->view)), $rows);

    return $rows;
  }



  /**
   * Build one week row.
   */
  function agres_availability_build_week($check_month = FALSE) {
    $curday_date = date_format($this->curday, DATE_FORMAT_DATE);
    $weekdays = agres_availability_untranslated_days($this->items, $this->view);
//     drupal_set_message('<pre>$curday_month '.print_r( $this->items,true).'</pre>');
    $month = date_format($this->curday, 'n');
    $first_day = variable_get('date_first_day', 0);
    $week = date_week($curday_date);
    $today = date_format(date_now(date_default_timezone()), DATE_FORMAT_DATE);
    // Set up buckets
    $total_rows = 0;
    $multiday_buckets = array(array(), array(), array(), array(), array(), array(), array());
    $singleday_buckets = array(array(), array(), array(), array(), array(), array(), array());

    // move backwards to the first day of the week
    $day_wday = date_format($this->curday, 'w');
    date_modify($this->curday, '-' . strval((7 + $day_wday - $first_day) % 7) . ' days');


    for ($i = 0; $i < 7; $i++) {
      $curday_date = date_format($this->curday, DATE_FORMAT_DATE);
      $class = strtolower($weekdays[$i] .
         ($this->view->style_plugin->date_info->mini ? ' mini' : ''));
//   if ($check_month && ($curday_date < $view->date_info->min_date_date || $curday_date > $view->date_info->max_date_date || date_format($curday, 'n') != $month)) {
      if ($check_month && ($curday_date < $this->date_info->min_date_date || $curday_date > $this->date_info->max_date_date || date_format($this->curday, 'n') != $month)) {

        $class = strtolower($weekdays[$i]) . ' empty';
        $singleday_buckets[$i][][] = array(
          'entry' => theme('agres_availability_empty_day', array(
            'curday' => $curday_date,
            'view' => $this->view,
          )),
          'item' => NULL,
        );
        //from good old
        $class .= ' empty';
        $content = array(
          'date' => '',
          'datebox' => '',
          'empty' => theme('agres_availability_empty_day', array(
            'curday' => $curday_date,
            'view' => $this->view,
          )),
          'link' => '',
          'all_day' => array(),
          'items' => array(),
        );
      }
      else {

        $content = $this->agres_availability_build_day();
        $class .= ($curday_date == $today ? ' today' : '') .
        ($curday_date < $today ? ' past' : '') .
        ($curday_date > $today ? ' future' : '') .
        (empty($items[$curday_date]) ? ' has-no-events' : ' has-events');
      }
      $rows[$week][] = array(
        'data' => $content,
        'class' => $class,
        'id' => $this->view->name . '-' . $curday_date,
      );
      date_modify($this->curday, '+1 day');

    }
    return $rows;
  }

  /**
   * Build the contents of a single day for the $rows results.
   */
  function agres_availability_build_week_day($wday, &$multiday_buckets, &$singleday_buckets) {
    $curday_date = date_format($this->curday, DATE_FORMAT_DATE);
    $max_events = !empty($this->date_info->style_max_items) ? $this->date_info->style_max_items : 0;
    $hide = !empty($this->date_info->style_max_items_behavior) ? ($this->date_info->style_max_items_behavior == 'hide') : FALSE;
    $multiday_theme = !empty($this->date_info->style_multiday_theme) && $this->date_info->style_multiday_theme == '1';
    $first_day = variable_get('date_first_day', 0);
    $cur_cnt = 0;
    $total_cnt = 0;
    $ids = array();

    // If we are hiding, count before processing further
    if ($max_events != agres_availability_SHOW_ALL) {
      foreach ($this->items as $date => $day) {
        if ($date == $curday_date) {
          foreach ($day as $time => $hour) {
            foreach ($hour as $key => $item) {
              $total_cnt++;
              $ids[] = $item->date_id;
            }
          }
        }
      }
    }

    // If we haven't already exceeded the max or we'll showing all, then process the items
    if ($max_events == agres_availability_SHOW_ALL || !$hide || $total_cnt <= $max_events) {
      // Count currently filled items
      foreach ($multiday_buckets[$wday] as $bucket) {
        if (!$bucket['avail']) {
          $cur_cnt++;
        }
      }
      foreach ($this->items as $date => $day) {
        if ($date == $curday_date) {
          $count = 0;
          ksort($day);
          foreach ($day as $time => $hour) {
            foreach ($hour as $key => $item) {
              $count++;
              // Can we add an item?
              if ($max_events == agres_availability_SHOW_ALL || $cur_cnt <= $max_events) {
                $all_day = $item->agres_availability_start_date == $item->agres_availability_end_date;

                // Parse out date part
                $start_ydate = date_format($item->date_start, DATE_FORMAT_DATE);
                $end_ydate = date_format($item->date_end, DATE_FORMAT_DATE);
                $cur_ydate = date_format($this->curday, DATE_FORMAT_DATE);

                $is_multi_day = ($start_ydate < $cur_ydate || $end_ydate > $cur_ydate);

                // Does this event span multi-days?
                if ($multiday_theme && ($is_multi_day || $all_day)) {
                  // If this the first day of the week, or is the start date of the multi-day event,
                  // then record this item, otherwise skip over
                  $day_no = date_format($this->curday, 'd');
                  if ($wday == 0 || $start_ydate == $cur_ydate || ($this->date_info->granularity == 'month' && $day_no == 1) || ($all_day && !$is_multi_day)) {
                    $cur_cnt++;
                    // Calculate the colspan for this event

                    // If the last day of this event exceeds the end of the current month or week,
                    // truncate the remaining days
                    $diff =  $this->curday->difference($this->date_info->max_date, 'days');
                    $remaining_days = ($this->date_info->granularity == 'month') ? min(6 - $wday, $diff) : $diff - 1;
                    // The bucket_cnt defines the colspan.  colspan = bucket_cnt + 1
                    $days = $this->curday->difference($item->date_end, 'days');
                    $bucket_cnt = max(0, min($days, $remaining_days));

                    // See if there is an available slot to add an event.  This will allow
                    // an event to precede a row filled up by a previous day event
                    $avail = FALSE;
                    $bucket_index = count($multiday_buckets[$wday]);
                    for ($i = 0; $i < $bucket_index; $i++) {
                      if ($multiday_buckets[$wday][$i]['avail']) {
                        $bucket_index = $i;
                        break;
                      }
                    }

                    // Add continuation attributes
                    $item->continuation =  ($item->date_start < $this->curday);
                    $item->continues = ( $days > $bucket_cnt );

                    // Assign the item to the available bucket
                    $multiday_buckets[$wday][$bucket_index] = array(
                      'colspan' => $bucket_cnt + 1,
                      'rowspan' => 1,
                      'filled' => TRUE,
                      'avail' => FALSE,
                      'all_day' => $all_day,
                      'item' => $item,
                      'wday' => $wday,
                      'entry' => theme('agres_availability_item', array('view' => $this->view, 'rendered_fields' => $item->rendered_fields, 'item' => $item)),
                    );

                    // Block out empty buckets for the next days in this event for this week
                    for ($i = 0; $i < $bucket_cnt; $i++) {
                      $bucket = &$multiday_buckets[$i + $wday + 1];
                      $bucket_row_count = count($bucket);
                      $row_diff = $bucket_index - $bucket_row_count;

                      // Fill up the preceding buckets - these are available for future
                      // events
                      for ( $j = 0; $j < $row_diff; $j++) {
                        $bucket[($bucket_row_count + $j)] = array(
                          'entry' => '&nbsp;',
                          'colspan' => 1,
                          'rowspan' => 1,
                          'filled' => TRUE,
                          'avail' => TRUE,
                          'wday' => $wday,
                          'item' => NULL,
                        );
                      }
                      $bucket[$bucket_index] = array(
                        'filled' => FALSE,
                        'avail' => FALSE,
                      );
                    }
                  }
                }
                else {
                  $cur_cnt++;
                  // Assign to single day bucket
                  $singleday_buckets[$wday][$time][] = array(
                    'entry' => theme('agres_availability_item', array('view' => $this->view, 'rendered_fields' => $item->rendered_fields, 'item' => $item)),
                    'item' => $item,
                    'colspan' => 1,
                    'rowspan' => 1,
                    'filled' => TRUE,
                    'avail' => FALSE,
                    'wday' => $wday,
                  );
                }
              }
              else {
                break; // exceeded count
              }
            }
          }
        }
      }
    }

    // Add a more link if necessary
    if ($max_events != agres_availability_SHOW_ALL && $total_cnt > 0 && $cur_cnt < $total_cnt) {
      $singleday_buckets[$wday][][] = array(
        'entry' => theme('agres_availability_' . $this->date_info->agres_availability_type . '_multiple_node', array(
          'curday' => $curday_date,
          'count' => $total_cnt,
          'view' => $this->view,
          'ids' => $ids,
        )),
        'more_link' => TRUE,
        'item' => 'NULL',
      );
    }
//     drupal_set_message('<pre>weeeeeekdayyyy '.print_r( $multiday_buckets,true).'</pre>');
  }

  /**
   * Build the contents of a single day for the $rows results.
   */
  function agres_availability_build_day() {
    $curday_date = date_format($this->curday, DATE_FORMAT_DATE);
    $selected = FALSE;
    $max_events = !empty($this->date_info->style_max_items) ? $this->date_info->style_max_items : 0;
    $types = array();
    $inner = array();
    $all_day = array();
    $empty = '';
    $link = '';
    $count = 0;
//    drupal_set_message('<pre>dayday '.print_r( $this->items,true).'</pre>');
    foreach ($this->items as $date => $day) {
      if ($date == $curday_date) {
        $count = 0;
        $selected = TRUE;
        ksort($day);
        foreach ($day as $time => $hour) {
          foreach ($hour as $key => $item) {
            $count++;
            if (isset($item->type)) {
              $types[$item->type] = $item;
            }
            //empty($this->date_info->mini) &&
            if ( ($max_events == agres_availability_SHOW_ALL || $count <= $max_events || ($count > 0 && $max_events == agres_availability_HIDE_ALL))) {
              $key = date_format($item->date_start, 'H:i:s');
              $inner[$key][] = $item;
//                drupal_set_message('<pre>dayday '.print_r( $item,true).'</pre>');
//              if ($item->agres_availability_all_day) {
//                $all_day[] = $item;
//              }
//              else {
//                $key = date_format($item->date_start, 'H:i:s');
//                $inner[$key][] = $item;
//              }
            }
          }
        }
      }
    }
    ksort($inner);

    if (empty($inner) && empty($all_day)) {
      $empty = theme('agres_availability_empty_day', array('curday' => $curday_date, 'view' => $this->view));
    }
    // We have hidden events on this day, use the theme('agres_availability_multiple_') to show a link.
    if ($max_events != agres_availability_SHOW_ALL && $count > 0 && $count > $max_events && $this->date_info->agres_availability_type != 'day' && !$this->date_info->mini) {
      if ($this->date_info->style_max_items_behavior == 'hide' || $max_events == agres_availability_HIDE_ALL) {
        $all_day = array();
        $inner = array();
      }
      $link = theme('agres_availability_' . $this->date_info->agres_availability_type . '_multiple_node', array(
        'curday' => $curday_date,
        'count' => $count,
        'view' => $this->view,
        'types' => $types,
      ));

    }

    $content = array(
      'date' => $curday_date,
      'datebox' => theme('agres_availability_datebox', array(
        'date' => $curday_date,
        'view' => $this->view,
        'items' => $this->items,
        'selected' => $selected,
      )),
      'empty' => $empty,
      'link' => $link,
      'all_day' => $all_day,
      'items' => $inner,
    );

    return $content;

  }

  /**
   * Build one mini month.
   */
  function agres_availability_build_mini_month() {
    $curday = $this->curday;
    $items  = $this->items;
    $view = $this->view;
    $month = date_format($curday, 'n');
    date_modify($curday, '-' . strval(date_format($curday, 'j') -1) . ' days');

    $rows = array();
    do {
      $rows = array_merge($rows, $this->agres_availability_build_week(TRUE));
      $curday_date = date_format($curday, DATE_FORMAT_DATE);
      $curday_month = date_format($curday, 'n');

//    drupal_set_message('<pre>*agres_availability_build_month**'. print_r($curday_date, TRUE) .'</pre>');
    } while ($curday_month == $month && $curday_date <= $view->date_info->max_date_date);
    // Merge the day names in as the first row.
    $rows = array_merge(array(agres_availability_week_header($this->view)), $rows);
//  drupal_set_message('<pre>$curday_month '.print_r( $rows,true).'</pre>');
    return $rows;
  }

  /**
   * Build one week row.
   */
  function agres_availability_build_mini_week($check_month = TRUE) {
    $curday_date = date_format($this->curday, DATE_FORMAT_DATE);
    $weekdays = agres_availability_untranslated_days($this->items, $this->view);
    $today = date_format(date_now(date_default_timezone()), DATE_FORMAT_DATE);
    $month = date_format($this->curday, 'n');
    $week = date_week($curday_date);
    $first_day = variable_get('date_first_day', 0);
    // move backwards to the first day of the week
    $day_wday = date_format($this->curday, 'w');
    date_modify($this->curday, '-' . strval((7 + $day_wday - $first_day) % 7) . ' days');
    $curday_date = date_format($this->curday, DATE_FORMAT_DATE);
//
    for ($i = 0; $i < 7; $i++) {
      $curday_date = date_format($this->curday, DATE_FORMAT_DATE);
      $class = strtolower($weekdays[$i] . ' mini');
      if ($check_month && ($curday_date < $this->date_info->min_date_date || $curday_date > $this->date_info->max_date_date || date_format($this->curday, 'n') != $month)) {
        $class .= ' empty';
        $variables = array(
          'curday' => $curday_date,
          'view' => $this->view,
        );
//        drupal_set_message('<pre>agres_availability_build_mini_week ' . print_r($this->items, true) . '</pre>');
        $content = array(
          'date' => '',
          'datebox' => '',
          'empty' => theme('agres_availability_empty_day', $variables),
          'link' => '',
          'all_day' => array(),
          'items' => array(),
        );
        $content = array(
          'date' => '',
          'datebox' => '',
          'empty' => theme('agres_availability_empty_day', array(
            'curday' => $curday_date,
            'view' => $this->view,
          )),
          'link' => '',
          'all_day' => array(),
          'items' => array(),
        );
      }
      else {
        $content = $this->agres_availability_build_day();
        $class .= ($curday_date == $today ? ' today' : '') .
          ($curday_date < $today ? ' past' : '') .
          ($curday_date > $today ? ' future' : '') .
          (empty($this->items[$curday_date]) ? ' has-no-events' : ' has-events');
      }
      $rows[$week][] = array(
        'data' => $content,
        'class' => $class,
        'id' => $this->view->name . '-' . $curday_date,
      );
      date_modify($this->curday, '+1 day');
    }
    return $rows;
  }

}
