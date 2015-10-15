<?php

/**
 * @file
 *  Additional theming functions for agreservations and booking agres_views
 */

/**
 * Implements hook_preprocess_date_views_pager().
 *
 * Creates a calendar_links array which is stored in the session and used in
 * calendar_menu_local_tasks_alter() to display the links as action items and
 * dynamic tabs.
 * The links can be altered or deleted using hook_calendar_links_alter().
 */
function agres_views_preprocess_date_views_pager(&$vars) {
//  $view = $vars['plugin']->view;
//  if ($view->plugin_name != 'agres_views_style') {
//    return;
//  }
//  $options = $view->style_options;
//
//  // If we're not on a view with a path (a page), no links are needed.
//  $current_path = !empty($view->display_handler->options['path']) ? $view->display_handler->options['path'] : '';
//  if (empty($current_path)) {
//    return;
//  }
//
//  // Find all the displays in this view that use the agres_views style and have a path and create links to each.
//  $agres_views_links[$current_path] = array();
//  $base = array('attributes' => array('rel' => 'nofollow'));
//  foreach ($view->display as $id => $display) {
//    // Check for !empty() in case the view is not fully formed or has displays that are marked to be deleted.
//    if (!empty($display->deleted) || empty($display->display_options['style_plugin'])) {
//      continue;
//    }
//    if ($display->display_options['style_plugin'] == 'agres_views_style' && !empty($display->display_options['path'])) {
//      $path = $display->display_options['path'];
//      $title = $display->display_title;
//      $type = !empty($display->display_options['style_options']['agres_views_type']) ? $display->display_options['style_options']['agres_views_type'] : 'month';
//
//      // Make sure the links to other agres_views displays use the right path
//      // for that display. Get rid of pager links when swapping between displays
//      // to force the base argument to be structured correctly for the type of
//      // display. This means you cannot use these links in a block or panel.
//      $href = str_replace($current_path, $path, date_pager_url($view, $type, NULL, TRUE, FALSE));
//      $part_path = drupal_substr($href, drupal_strlen(base_path()));
//
//      // Once we have a path for the links to other displays, add it to our
//      // links array.
//      $agres_views_links[$current_path]['tabs'][$type] = array(
//        'title' => $title,
//        'path' => $part_path,
//      );
//      $agres_views_links[$current_path]['types'][] = $type;
//    }
//  }
//
//  // If an 'Add new ... link is provided, add it here.
//  if (!empty($view->date_info->agres_views_date_link) && !empty($view->date_info->url)
//    && (user_access("administer nodes") || user_access('create ' . $view->date_info->agres_views_date_link . ' content'))) {
//
//    $name = node_type_get_name($view->date_info->agres_views_date_link);
//    $href = 'node/add/' . str_replace('_', '-', $view->date_info->agres_views_date_link);
//    $agres_views_links[$current_path]['actions'][] = array(
//      'title' => t('Add @name', array('@name' => $name)),
//      'path' => $href,
//    );
//  }
//
//  // Pass this through drupal_alter() so it can be adjusted in custom code or in the theme.
//  drupal_alter('agres_views_links', $agres_views_links);
//
//  // Add the value to the session so it can be used to create the tabs.
//  $_SESSION['agres_views_links'][$current_path] = $agres_views_links[$current_path];
}

/**
 * Preprocess an RSS feed
 */
function template_preprocess_agres_views_style(&$vars) {
  global $base_url;
  global $language;
}

/**
 * Display a month view.
 */
function template_preprocess_agres_views_month(&$vars) {
  // Add in all the $vars added by the main calendar preprocessor.
  $view = $vars['view'];
  $rows = $vars['rows'];

  if (sizeof($rows) > 1) {
    $day_names = array_shift($rows);
  }
  else {
    $day_names = $rows;
    $rows = array();
  }
  $month_rows = $rows;

  // Setup categories and unittypes
  $vars['unittypes'] = '';
  $countarg = count($view->args);

  if (module_exists('agres_categories')) {
    $countarg = count($view->args);
    if ($countarg > 4) {
      $view->args = array_slice($view->args, 0, 3);
    }
    $vars['currentcategory'] = '';
    if (isset($view->args[1]) && $view->args[1]) {
      $unittypes = _agres_categories_get_unittypenodes_of_category($view->args[1]);
      $vars['currentcategory'] = $view->args[1];
      $categories = _agres_categories_get_categories(TRUE);
      if (isset($view->args[2]) && $view->args[2]) {
        $units = _agres_categories_get_unitnodes_of_category($view->args[2], $view->args[1]);
        $vars['currentunittype'] = $view->args[2];
      }
      else {
        $unittypes = _agres_categories_get_unittypenodes_of_category($view->args[1]);
        $units = _agres_categories_get_unitnodes_of_category(NULL, $view->args[1]);
      }
    }
    else {

      $unittypes = _agres_categories_get_unittypenodes_of_category();
      $categories = _agres_categories_get_categories(TRUE);
      $units = _agreservations_get_unitnodes();
    }
  }
  else {
    if ($countarg > 3) {
      $view->args = array_slice($view->args, 0, 2);
    }
    if (isset($view->args[1])) {
      $units = _agreservations_get_unitnodes($view->args[1]);
      $vars['currentunittype'] = $view->args[1];
    }
    else {
      $units = _agreservations_get_unitnodes();
    }
    $unittypes = _agreservations_get_unittypenodes();
  }

  // Remove the args from agrescurrentpath:
  $toremovefrompath = implode("/", $view->args);
  $agrescurrentpath = get_current_path();
  $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);

  // Cut off trailing slash
  if (drupal_substr($agrescurrentpath, -1, 1) == "/") {
    $agrescurrentpath = drupal_substr($agrescurrentpath, 0, drupal_strlen($agrescurrentpath) - 1);
  }

  $vars['agrescurrentpath'] = $agrescurrentpath;

  // Step1: all in a single array
  $agdaysresitems = array();
  $agmonth_rows = array();
  $days = 0;
  foreach ($rows as $weekno => $row) {
    foreach ($row as $day => $data) {
      if (isset($data['data']['date']) & (!empty($data['data']['date']))) {
        $days += 1;
      }
      $agdaysresitems[] = $data;
    }
  }
  $agmonth_rows['rinfo'] = array();
  // Step 2: Organize in unit arrays
  foreach ($units as $unit) {
    $agmonth_rows['rinfo'][$unit->title] = array();

    foreach ($agdaysresitems as $day => $agdayresitem) {

      $data = $agdayresitem['data']['datebox'];
      $date = $agdayresitem['data']['date'];
      $agmonth_rows[$unit->title][$day]['datebox'] = $data;
      $agmonth_rows[$unit->title][$day]['date'] = $date;

      foreach ($agdayresitem['data']['items'] as $time => $hour) {

        // For each reservation object node in the hour
        foreach ($hour as $resobkey=>$resob) {
//          drupal_set_message('<pre>$rows$rows$rows '.print_r( $resob->nid,true).'</pre>');
          if (!isset($agmonth_rows[$unit->title][$day]['resitm'][$resob->nid])) {
            $agmonth_rows[$unit->title][$day]['resitm'][$resob->nid] = '0';
          }


          $continues = 0;
          $continuation = 0;

          $tmpunititems = _agreservations_get_unitn_for_reservation_node($resob->nid); //get units per reservation
          if (in_array($unit->nid, $tmpunititems)) {
            $temp_end = date_create($resob->row->field_field_agres_rdate[0]['raw']['value2']);
            $temp_granularity_end = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
            if ($temp_end > $temp_granularity_end) {
              $continues = 1;
            }
            else {
              $continues = 0;
            }
            $temp_start = date_create($resob->row->field_field_agres_rdate[0]['raw']['value']);
            $temp_granularity_start = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
            if ($temp_start < $temp_granularity_start) {
              $continuation = 1;
            }
            else {
              $continuation = 0;
            }
            $itemclass = 'itemwithoutorder';            
            if (!isset($resob->node->field_agres_orderid[$resob->node->language])||($resob->node->field_agres_orderid[$resob->node->language][0]['value'])==0) {
              // Treat as receptionist manual reservation defaultcolor orange
              $itemclass = 'itemwithoutorder';
            }
            else {
              // Treat as receptionist manual reservation defaultcolor blue
              $itemclass = 'itemwithorder';
            }
//            if (!isset($resob->node->field_agres_status[$resob->node->language]){
//              
//            }
            if ((!isset($agmonth_rows[$unit->title]['spaninfo'][$resob->nid]) || (empty($agmonth_rows[$unit->title]['spaninfo'][$resob->nid])))) {
              $theme = 'calendar_month_node';
              $variables = array(
                'node' => $resob,
                'view' => $view,
              );
              
              if (isset($resob->node->field_agres_status) && count($resob->node->field_agres_status) > 0) {
                foreach ($resob->node->field_agres_status as $k_rnfast => $rnfast) {
                  if ($rnfast[0]['value']==AGRES_STATUS_AVAILABILITY_BLOCK) {
                    $itemclass = 'agres_availability_unavailable';
//                    drupal_set_message('<pre>' . print_r($rnfast[0]['value'], true) . '</pre>');
                  }
                  
                  break;
                }
              }
//              $resob->node->field_agres_status
              
//              $itemclass = 
              $themeparams = array(
                'view' => $view,
                'rendered_fields' => $resob->rendered_fields,
                'item' => $resob,
                'continues' => $continues,
                'continuation' => $continuation,
                'itemclass' => $itemclass,
              );

              $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = '';
              $agmonth_rows[$unit->title]['spaninfo'][$resob->nid] = 1;
              $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = theme('agres_views_item', $themeparams);

              //only the first:
//            $agmonth_rows[$unit->title][$day]['data'] = array_slice($agmonth_rows[$unit->title][$day]['data'], 0, 1, TRUE);
            }
            else {
              $theme = 'calendar_month_node';
              $variables = array(
                'node' => $resob,
                'view' => $view,
              );
              $themeparams = array(
                'view' => $view,
                'rendered_fields' => $resob->rendered_fields,
                'item' => $resob,
                'continues' => $continues,
                'continuation' => $continuation,
                'itemclass' => $itemclass,
              );              
              $endpos = strpos($resob->row->field_field_agres_rdate[0]['raw']['value2'], $agmonth_rows[$unit->title][$day]['date'], 0);
              if (empty($agmonth_rows[$unit->title][$day]['data'][$resob->nid])) {
                if ($endpos === FALSE) {
                  $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = '***busy***';theme('agres_views_item', $themeparams);
                  $agmonth_rows[$unit->title]['spaninfo'][$resob->nid] += 1;
                }
              }
            }
          }
        }
      }

      // All Day
      if (isset($agdayresitem['data']['all_day'])) {
        foreach ($agdayresitem['data']['all_day'] as $resobkey=>$resob) {
//          drupal_set_message('<pre>$rows$rows$rows '.print_r( $resobkey,true).'</pre>');
          // For each reservation object node in the hour
          if (!isset($agmonth_rows[$unit->title][$day]['resitm'][$resob->nid])) {
            $agmonth_rows[$unit->title][$day]['resitm'][$resob->nid] = '0';
          }

          $tmpunititems = _agreservations_get_unitn_for_reservation_node($resob->nid); //get units per reservation
          if (in_array($unit->nid, $tmpunititems)) {
            $itemclass = 'itemwithoutorder';
            $continues = 0;
            $continuation = 1;
            $temp_end = date_create($resob->row->field_field_agres_rdate[0]['raw']['value2']);
            $temp_granularity_end = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
            if ($temp_end > $temp_granularity_end) {
              $continues = 1;
            }
            else {
              $continues = 0;
            }
            $temp_start = date_create($resob->row->field_field_agres_rdate[0]['raw']['value']);
            $temp_granularity_start = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
            if ($temp_start < $temp_granularity_start) {
              $continuation = 1;
            }
            else {
              $continuation = 0;
            }

            if (!isset($resob->node->field_agres_orderid[$resob->node->language])||($resob->node->field_agres_orderid[$resob->node->language][0]['value']==0)) {
              // Treat as receptionist manual reservation defaultcolor orange
              $itemclass = 'itemwithoutorder';
            }
            else {
              // Treat as receptionist manual reservation defaultcolor blue
              $itemclass = 'itemwithorder';
            }
            if ((!isset($agmonth_rows[$unit->title]['spaninfo'][$resob->nid]) || (empty($agmonth_rows[$unit->title]['spaninfo'][$resob->nid])))) {
              $theme = 'calendar_month_node';
              $variables = array(
                'node' => $resob,
                'view' => $view,
              );
              $themeparams = array(
                'view' => $view,
                'rendered_fields' => $resob->rendered_fields,
                'item' => $resob,
                'continues' => $continues,
                'continuation' => $continuation,
                'itemclass' => $itemclass,
              );
              $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = '';
              $agmonth_rows[$unit->title]['spaninfo'][$resob->nid] = 1;
              $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = theme('agres_views_item', $themeparams);
            }
            else {


              $endpos = strpos($resob->row->field_field_agres_rdate[0]['raw']['value2'], $agmonth_rows[$unit->title][$day]['date'], 0);
              if (empty($agmonth_rows[$unit->title][$day]['data'][$resob->nid])) {
                if ($endpos === FALSE) {
                  $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = '***busy***';
                  $agmonth_rows[$unit->title]['spaninfo'][$resob->nid] += 1;
                }
              }
            }
          }
        }
      }
    }
  }
    $agmonth_rows_copy = array();
//    $agmonth_rows_copy = $agmonth_rows;
    $agmonth_rows_multi = $agmonth_rows;
    $agmonth_rows_help = $agmonth_rows;    
    //initialize:
       foreach($agmonth_rows_multi as $kunitm => $unitrm) { 
                //<set content including spaninfo into extra dimension>:
                $agmonth_rows_copy[$kunitm][0]=$agmonth_rows_multi[$kunitm];
                  }
    foreach ($agmonth_rows as $kunit => $unitr) {
     
    if (!empty($unitr)) {
      foreach ($unitr as $kday => $dayr) {
        if (is_numeric($kday)) {
          if (isset($dayr['data'])) {
//            
            $i=0;
//            for ($i = 0; $i <= count($dayr['data']); $i++) {
            foreach ($dayr['data'] as $kdata => $daydata) {
              
              foreach($agmonth_rows_multi as $kunitmh => $unitrmh) { 
                if (!isset($agmonth_rows_copy[$kunitmh][$i]))
                  foreach ($unitrmh as $kdaymh => $dayrmh) {
                    if (isset($dayrmh['data'])) {
                      unset($agmonth_rows_multi[$kunitmh][$kdaymh]['data']);
                    }
                  }
              }
              foreach($agmonth_rows_multi as $kunitm => $unitrm) { 
                //<set content including spaninfo into extra dimension>:
                $agmonth_rows_copy[$kunitm][$i]=$agmonth_rows_multi[$kunitm];
                //</set content including spaninfo into extra dimension>
                foreach ($unitrm as $kdaym => $dayrm) {                   
                  if (is_numeric($kdaym)) {
                    if (isset($agmonth_rows_help[$kunitm][$kdaym]['data'])) { 
                      $arraykeys = array_keys($agmonth_rows_help[$kunitm][$kdaym]['data']);
                      foreach ($agmonth_rows_help[$kunitm][$kdaym]['data'] as $kdatam => $daydatam) {

                          if(!isset($agmonth_rows_copy[$kunitm][$i][$kdaym]['data'][$kdatam]))
                            if(isset($arraykeys[$i]))
                          $agmonth_rows_copy[$kunitm][$i][$kdaym]['data'][$arraykeys[$i]] = $agmonth_rows_help[$kunitm][$kdaym]['data'][$arraykeys[$i]];
       
                      }
                    }
               
                  }
                }
                
              }
              $i++;
//              drupal_set_message('<pre>foreachforeach '.print_r($kdata,true).'</pre>');
            }            
          }
        }

      }
    }
  }
  
//drupal_set_message('<pre>_____1111111111_______ '.print_r($agmonth_rows,true).'</pre>');
//    drupal_set_message('<pre>_____********_______ '.print_r($agmonth_rows_copy,true).'</pre>');
//   $agmonth_rows[$unit->title][$day]['data']

  $daymonth = 0;
  $vars['daynumber'] = $days;
  $vars['unittypes'] = $unittypes;
  $vars['currentselectedmonth'] = $view->args[0]; // year.'-W'.$week;
  $vars['agmonth_rows'] = $agmonth_rows;
  $vars['agmonth_rows_copy'] = $agmonth_rows_copy;
  $vars['units'] = $units;
  $vars['rows'] = $month_rows;
  $vars['day_names'] = $day_names;
  $vars['template_file'] = 'agres_views-calendar-month-c';
  $vars['display_type'] = $view->date_info->granularity;
  $vars['min_date_formatted'] = date_format($view->date_info->min_date, DATE_FORMAT_DATETIME);
  $vars['max_date_formatted'] = date_format($view->date_info->max_date, DATE_FORMAT_DATETIME);
  if (module_exists('agres_categories')) {
    $vars['categories'] = $categories;
  }
  $path = drupal_get_path('module', 'agres_views');
  $vars['directory'] = $path . '/theme';
}

/**
 * Display a mini month view.
 */
function template_preprocess_agres_views_mini(&$vars) {
  // Add in all the $vars added by the main agres_views preprocessor.
  template_preprocess_agres_views_month($vars);

  $view = $vars['view'];

  // Make sure that the agres_views title links go to the month view,
  // not the year view (if this is embedded in a year display).
  $view->override_path = agres_views_granularity_path($view, 'month');

  $view->date_info->show_title = !empty($view->date_info->show_title) ? $view->date_info->show_title : FALSE;
  $vars['show_title'] = $view->date_info->show_title;
  $vars['view'] = $view;
}

/**
 * Display a year view.
 */
function template_preprocess_agres_views_year(&$vars) {

  // Construct a agres_views for each month, adjusting the $view passed
  // to the theme so it will produce the right results.
  $view = clone($vars['view']);
  $year = date_format($view->date_info->min_date, 'Y');
  $view->date_info->style_with_weekno = FALSE;
  $rows = $vars['rows'];
  $months = array();
  foreach ($rows as $month => $month_rows) {
    $view->date_info->month = $month;
    $view->date_info->granularity = 'month';
    $view->date_info->mini = TRUE;
    $view->date_info->hide_nav = TRUE;
    $view->date_info->show_title = TRUE;
    $view->date_info->url = date_pager_url($view, NULL, date_pad($year, 4) . '-' . date_pad($month));
    $view->date_info->min_date = new DateObject($view->date_info->year . '-' . date_pad($month) . '-01 00:00:00', date_default_timezone());
    $view->date_info->max_date = clone($view->date_info->min_date);
    date_modify($view->date_info->max_date, '+1 month');
    date_modify($view->date_info->max_date, '-1 second');
    $variables = array(
      'view' => $view,
      'options' => $vars['options'],
      'rows' => $month_rows,
    );
    $months[$month] = theme('agres_views_mini', $variables);
  }
  $vars['months'] = $months;
  $vars['view']->date_info->hide_nav = FALSE;
  $vars['view']->date_info->granularity = 'year';
  $vars['mini'] = $view->date_info->mini;
}

/**
 * Display a day overlap view.
 */
function template_preprocess_agres_views_day_overlap(&$vars) {

  template_preprocess_agres_views_day($vars);
}

function template_preprocess_agres_views_day(&$vars) {
  // Get rid of visual duplicates.
  $vars['view']->style_with_weekno = FALSE;
  $view = $vars['view'];
  $rows = $vars['rows'];
  $show_empty_times = $view->date_info->style_show_empty_times;
  $grouping_field = $view->date_info->style_groupby_field;
  $countarg = count($view->args);
  if (module_exists('agres_categories')) {
    $countarg = count($view->args);
    if ($countarg > 4) {
      $view->args = array_slice($view->args, 0, 3);
    }
    if (isset($view->args[1]) && $view->args[1]) {
      $unittypes = _agres_categories_get_unittypenodes_of_category($view->args[1]);
      $vars['currentcategory'] = $view->args[1];
      $categories = _agres_categories_get_categories(TRUE);
      if (isset($view->args[2]) && $view->args[2]) {
        $units = _agres_categories_get_unitnodes_of_category($view->args[2], $view->args[1]);
        $vars['currentunittype'] = $view->args[2];
      }
      else {
        $unittypes = _agres_categories_get_unittypenodes_of_category($view->args[1]);
        $units = _agres_categories_get_unitnodes_of_category(NULL, $view->args[1]);
      }
    }
    else {
      $unittypes = _agres_categories_get_unittypenodes_of_category();
      $categories = _agres_categories_get_categories(TRUE);
      $units = _agreservations_get_unitnodes();
    }
  }
  else {
    if ($countarg > 3) {
      $view->args = array_slice($view->args, 0, 2);
    }
    if ($view->args[1]) {
      $units = _agreservations_get_unitnodes($view->args[1]);
      $vars['currentunittype'] = $view->args[1];
    }
    else {
      $units = _agreservations_get_unitnodes();
    }
    $unittypes = _agreservations_get_unittypenodes();
  }
  // Remove the args from agrescurrentpath
  $toremovefrompath = implode("/", $view->args);
  $agrescurrentpath = get_current_path();
  $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
  // Cut off trailing slash
  if (drupal_substr($agrescurrentpath, -1, 1) == "/") {
    $agrescurrentpath = drupal_substr($agrescurrentpath, 0, drupal_strlen($agrescurrentpath) - 1);
  }
  $vars['agrescurrentpath'] = $agrescurrentpath;

  $columns = array();
  $grouped_items = array();
  $grouped_items = array();
  $spanning_items = array(); //test
  $r = 0;

  foreach ($units as $unit) { //loop through all units
    foreach ($rows['all_day'] as $itmkey => $item) {
      // Get units of reservation
      $tmpunititems = _agreservations_get_unitn_for_reservation_node($item->nid);
      // If unit is in reserved units
      if (in_array($unit->nid, $tmpunititems)) {
        $column = t('Items');
        $theme = isset($item->calendar_node_theme) ? $item->calendar_node_theme : 'calendar_' . $view->date_info->granularity . '_node';
        // Add the rendered items to the unit in the calendarmatrix
        $spanning_items[$column][] = theme($theme, $item, $view); //test
        $item_count++;
      }
    }
  }

  $vars['rows']['all_day'] = $grouped_items;
  // Move timed items into the right columns and render them.
  $start_times = $view->date_info->style_groupby_times;
  $show_empty_times = FALSE;
  $end_start_time = '23:59:59';
  $grouped_items = array();
  $timeslots = $view->date_info->style_groupby_times;
  array_push($timeslots, $end_start_time);
  $timeslots_count = count($timeslots);

  // Translate rows into better format
  $events = $rows['items'];
  foreach ($units as $unit) {
    $grouped_items[$unit->title] = array();
    foreach ($events as $elemkey => $event) {
       foreach(array_keys($event) as $subevent){ //!
      // Get initial value as formatted one already blanked
      $group_key = isset($event->raw->{$grouping_field}) ? $event->raw->{$grouping_field} : t('Items');

      $key = $group_key . '-' . 0;
      $columns["$group_key"] = array();
      // Init group on first encounter
      if (!($columns["$group_key"])) {
        $columns["$group_key"][] = $group_key . '-0';
      }

      // If EVENTDATE < DAYCALENDARDATESTART:
      $temp_granularity_end = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
      if ($event[$subevent]->date_start < $temp_granularity_end) {
        $event_start = '00:00:00';
      }
      else {
        $event_start = $event[$subevent]->date_start->format('H:i:s');
      }

      $temp_granularity_end = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
      if ($event[0]->date_end > $temp_granularity_end) {
        $event_end = '23:59:59';
      }
      else {
        $event_end = $event[$subevent]->date_end->format('H:i:s');
      }

      $i = 0;
      // Get units of reservation
      $tmpunititems = _agreservations_get_unitn_for_reservation_node($event[$subevent]->nid);

      if (in_array($unit->nid, $tmpunititems)) {
        for ($slot = 0; $slot < $timeslots_count; $slot++) {
          // One slot event
          if ($event_start >= $timeslots[$slot] &&
            $event_end <= $timeslots[$slot + 1]) {
            $grouped_items[$unit->title][$timeslots[$slot]]['values'] = array();
            // Find free column to use
            $found = FALSE;
            foreach ($columns["$group_key"] as $ckey => $column) {
              $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = '';
              $key = $group_key . '-' . $ckey;
              if (!($grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"])) {
                $found = TRUE;
                break;
              }
            }
            if (!$found) {
              $key = $group_key . '-' . (count($columns[$group_key]));
              // Add new column
              $columns["$group_key"][] = $key;
            }

            $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = theme('agres_views_item', array('view' => $view, 'rendered_fields' => $event[$subevent]->rendered_fields, 'item' => $event[$subevent])); // theme($theme, $event, $view);
          }
          // First slot of multi slot event
          elseif ($event_start >= $timeslots[$slot] && $event_start < $timeslots[$slot + 1] && $event_end > $timeslots[$i + 1]) {
            $grouped_items[$unit->title][$timeslots[$slot]]['values'] = array();
            $theme = isset($event->calendar_node_theme) ? $event->calendar_node_theme : 'calendar_' . $view->date_info->granularity . '_node';
            // Find free column to use
            $found = FALSE;
            foreach ($columns["$group_key"] as $ckey => $column) {
              $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = '';
              $key = $group_key . '-' . $ckey;
              if (!($grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"])) {
                $found = TRUE;
                break;
              }
            }
            if (!$found) {
              $key = $group_key . '-' . (count($columns["$group_key"]));
              // Add new column
              $columns["$group_key"][] = $key;
            }

            $span = $timeslots[$slot];
            $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = theme('agres_views_item', array('view' => $view, 'rendered_fields' => $event[$subevent]->rendered_fields, 'item' => $event[$subevent])); // theme($theme, $event, $view);
            $grouped_items[$unit->title][$timeslots[$slot]]['span']["$key"] = 1;
          }
          elseif ($event_start < $timeslots[$slot] &&
            $event_end > $timeslots[$slot]) { // middle of end slot of event
            $grouped_items[$unit->title][$span]['span']["$key"]++;
            $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = '***busy***';
          }
          // Empty slot
          else {

          }
        }
      }
    }
  }
    // Do the headers last, once we know what the actual values are.
    $i = 0;
    $start_times = array_keys($grouped_items[$unit->title]);
    $start_times = $view->date_info->style_groupby_times;
    foreach ($start_times as $start_time) {

      $next_start_time = array_key_exists($i + 1, $start_times) ? $start_times[$i + 1] : '23:59:59';
      $variables = array(
        'start_time' => $start_time,
        'next_start_time' => $next_start_time,
        'curday_date' => $rows['date'],
      );
      $heading = theme('agres_views_time_row_heading', $variables);
      $grouped_items[$unit->title][$start_time]['hour'] = $heading['hour'];
      $grouped_items[$unit->title][$start_time]['ampm'] = $heading['ampm'];
      $i++;
    }

    ksort($grouped_items[$unit->title]);
    ksort($grouped_items);
  }
  $vars['rows']['items'] = $grouped_items;
  if (empty($columns)) {
    $header_columns[t('Items')]['title'] = t('Items');
    $flat_columns[t('Items')] = '';
  }
  else {
    $header_columns = array();
    foreach ($columns as $group_key => $group_column) {
      $header_columns[$group_key]['title'] = $group_key;
      $header_columns[$group_key]['span'] = count($group_column);
      // Also span all days events
      $vars['rows']['all_day'][$group_key]['span'] = count($group_column);
      foreach ($group_column as $column) {
        $flat_columns[] = $column;
      }
    }
  }

  $vars['header_columns'] = $header_columns;
  $vars['columns'] = $flat_columns;
  if (count($flat_columns)) {
    $vars['column_width'] = round(90 / count($flat_columns));
  }
  else {
    $vars['column_width'] = 90;
  }
  $vars['currentselectedday'] = $view->args[0]; // year.'-W'.$week;
  $vars['units'] = $units;
  $vars['unittypes'] = $unittypes;
  $vars['calendarname'] = $view->name;

  // Add small javascript to extend datebox to whole td height
  drupal_add_js(
    "Drupal.behaviors.myEventBehavior = function(context){
      $('.agreservations-calendar td.agreservations-agenda-items div.agreservations div.view-item').each( function(){
        // Just a dirty fix. I have no idea how to properly calculate weight for this element
        var width = Math.max(($(this).parents('td').width())-5, $(this).width());
        $(this).width(width);
        });
    };", "inline");

  $modulepath = drupal_get_path('module', 'agreservations');
  $vars['template_file'] = 'agreservations-calendar-day-2';
  if (module_exists('agres_categories')) {
    $vars['template_file'] = 'agreservations-calendar-day-c';
    $vars['categories'] = $categories;
  }
  return;
}

/**
 * Display a day view.
 */
function template_preprocess_agres_views_day_old(&$vars) {
  $vars['view']->style_with_weekno = FALSE;
  $view = $vars['view'];
  $rows = $vars['rows'];

  $item_count = 0;
  $by_hour_count = 0;
  $grouping_field = $view->date_info->style_groupby_field;
  $display_overlap = !empty($view->date_info->style_theme_style) && !empty($view->date_info->style_groupby_times);

  // Add optional css
  if ($display_overlap) {
    $overlapped_items = array();
    drupal_add_css(drupal_get_path('module', 'agres_views') . '/css/agres_views-overlap.css');
    if (empty($view->live_preview)) {
      drupal_add_js(drupal_get_path('module', 'agres_views') . '/js/agres_views_overlap.js');
    }
  }

  // If we are not grouping by time, move all items into the 'all day' array.
  if (empty($view->date_info->style_groupby_times)) {
    // Items are already grouped into times, so we need to process each time-group.
    foreach ($rows['items'] as $time => $items) {
      foreach ($items as $item) {
        $rows['all_day'][] = $item;
      }
    }
    $rows['items'] = array();
  }

  $columns = array();

  $vars['rows']['all_day'] = $grouped_items;

  // Moved timed items into the right columns and render them.
  $start_times = $view->date_info->style_groupby_times;
  $show_empty_times = $view->date_info->style_show_empty_times;
  $end_start_time = '23:59:59';
  $start_time = array_shift($start_times);
  $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;

  $grouped_items = array();
  foreach ($rows['items'] as $items) {
    foreach ($items as $item) {
      $time = date_format($item->agres_views_start_date, 'H:i:s');
      if (isset($item->{$grouping_field})) {
        $column = $item->{$grouping_field};
        // Remove the grouping field from the results.
        $item->{$grouping_field} = '';
        if (!in_array($column, $columns)) {
          $columns[] = $column;
        }
      }
      else {
        $column = t('Items');
      }
      // Find the next time slot and fill it. Populate the skipped
      // slots if the option to show empty times was chosen.
      while ($time >= $next_start_time && $time < $end_start_time) {
        if ((!empty($show_empty_times) || $display_overlap) && !array_key_exists($start_time, $grouped_items)) {
          $grouped_items[$start_time]['values'] = array();
        }
        $start_time = $next_start_time;
        $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;
      }
      $grouped_items[$start_time]['values'][$column][] = $item;
      if ($display_overlap) {
        $time_end = date_format($item->agres_views_end_date, 'H:i:s');
        $item->time_start = $time;
        $item->time_end = $time_end;
        _calc_indents($overlapped_items, $time, $time_end, $item);
      }
      $item_count++;
      $by_hour_count++;
    }
  }

  // Finish out the day's time values if we want to see empty times.
  if (!empty($show_empty_times) || $display_overlap) {
    while ($start_time < $end_start_time && (!empty($start_time) || $display_overlap)) {
      if (empty($start_time)) {
        $start_times = $view->date_info->style_groupby_times;
        $start_time = array_shift($start_times);
        $next_start_time = array_shift($start_times);
      }
      if (!array_key_exists($start_time, $grouped_items)) {
        $grouped_items[$start_time]['values'] = array();
      }
      $start_time = $next_start_time;
      $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;
    }
  }

  // Do the headers last, once we know what the actual values are.
  $i = 0;
  $start_times = array_keys($grouped_items);
  foreach ($start_times as $start_time) {
    $next_start_time = array_key_exists($i + 1, $start_times) ? $start_times[$i + 1] : '23:59:59';
    $variables = array(
      'start_time' => $start_time,
      'next_start_time' => $next_start_time,
      'curday_date' => $rows['date'],
    );
    $heading = theme('agres_views_time_row_heading', $variables);
    $grouped_items[$start_time]['hour'] = $heading['hour'];
    $grouped_items[$start_time]['ampm'] = $heading['ampm'];
    foreach ($grouped_items[$start_time]['values'] as $column => &$items) {
      foreach ($items as $index => $item) {
        if ($display_overlap) {
          $offset = round(date_format($item->agres_views_start_date, 'i') / 15);
          $end = new DateObject('1970-01-01 ' . $item->time_end);
          $start = new DateObject('1970-01-01 ' . $item->time_start);
          $duration = $end->difference($start);
          $duration = round($duration / (15 * 60));
          $item->class = 'd_' . $duration . ' o_' . $offset . ' i_' . $item->indent . ' md_' . min($item->max_depth, 5);
        }
        $grouped_items[$start_time]['values'][$column][$index] = theme('agres_views_item', array('view' => $view, 'rendered_fields' => $item->rendered_fields, 'item' => $item));
      }
    }
    $i++;
  }
  ksort($grouped_items);
  $vars['rows']['items'] = $grouped_items;

  if (empty($columns)) {
    $columns = array(t('Items'));
  }
  $vars['columns'] = $columns;

  $vars['agenda_hour_class'] = 'agres_views-agenda-hour';
  $first_column_width = 10;

  if (empty($view->date_info->style_groupby_times)) {
    $vars['agenda_hour_class'] .= ' agres_views-agenda-no-hours';
    $first_column_width = 1;
  }

  $vars['first_column_width'] = $first_column_width;
  if (count($columns)) {
    $vars['column_width'] = round((100 - $first_column_width) / count($columns));
  }
  else {
    $vars['column_width'] = (100 - $first_column_width);
  }
  $vars['item_count'] = $item_count;
  $vars['by_hour_count'] = $by_hour_count;
  $vars['start_times'] = $view->date_info->style_groupby_times;
  return;
}

/**
 * Display a week view.
 */
function template_preprocess_agres_views_week(&$vars) {
  // Add in all the $vars added by the main calendar preprocessor.
  if ($vars['view']->name == 'agres_view') {
    $vars['view']->style_with_weekno = FALSE;
    $view = $vars['view'];
    $rows = $vars['rows'];
    $agrrows = $vars['rows'];
    $item_count = 0;
    $by_hour_count = 0;
    if (sizeof($rows) > 1) {
      $day_names = array_shift($rows);
    }
    else {
      $day_names = $rows;
      $rows = array();
    }
    $countarg = count($view->args);
    if (module_exists('agres_categories')) {
      $countarg = count($view->args);
      if ($countarg > 4) {
        $view->args = array_slice($view->args, 0, 3);
      }
      $vars['currentcategory'] = '';
      if (isset($view->args[1]) && $view->args[1]) {
        $categories = _agres_categories_get_categories(TRUE);
        $unittypes = _agres_categories_get_unittypenodes_of_category($view->args[1]);
        $vars['currentcategory'] = $view->args[1];

        if (isset($view->args[2]) && $view->args[2]) {
          $units = _agres_categories_get_unitnodes_of_category($view->args[2], $view->args[1]);
          $vars['currentunittype'] = $view->args[2];
        }
        else {
          $unittypes = _agres_categories_get_unittypenodes_of_category($view->args[1]);
          $units = _agres_categories_get_unitnodes_of_category(NULL, $view->args[1]);
        }
      }
      else {
        $unittypes = _agres_categories_get_unittypenodes_of_category();
        $categories = _agres_categories_get_categories(TRUE);
        $units = _agreservations_get_unitnodes();
      }
    }
    else {
      if ($countarg > 3) {
        $view->args = array_slice($view->args, 0, 2);
      }
      if (isset($view->args[1])) {
        if ($view->args[1]) {
          $units = _agreservations_get_unitnodes($view->args[1]);
          $vars['currentunittype'] = $view->args[1];
        }
        else {
          $units = _agreservations_get_unitnodes();
        }
      }
      else {
        $units = _agreservations_get_unitnodes();
      }
      $unittypes = _agreservations_get_unittypenodes();
    }
    // Remove the args from agrescurrentpath:
    $toremovefrompath = implode("/", $view->args);
    $agrescurrentpath = get_current_path();
    $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
    // Cut off trailing slash:
    if (drupal_substr($agrescurrentpath, -1, 1) == "/") {
      $agrescurrentpath = drupal_substr($agrescurrentpath, 0, drupal_strlen($agrescurrentpath) - 1);
    }
    $vars['agrescurrentpath'] = $agrescurrentpath;
    // Moved timed items into the right columns and render them.
    $show_empty_times = $view->date_info->style_show_empty_times;
    $end_start_time = '23:59:59';
    $grouped_items = array();
    $vars['rows'] = $rows[0];
    $lastweekno = '';
    $icount = 1;
    $ff = 0;
    // Initialize vars teststring
    $vars['rinfo'] = array();
    foreach ($rows[0] as $weekno => $row) {
      $vars['rows'][$weekno] = $row['data'];
      foreach ($row['data']['items'] as $ikey => $item) {
        ksort($item);
        foreach ($item as $skey => $singlereservation) {

          $date = new DateObject($singlereservation->field['value'], $singlereservation->field['timezone_db'], NULL);
          $date->setTimezone(timezone_open(date_default_timezone()));

          $tmpunititems = array();
          // Rooms per reservation pick
          $tmpunititems = _agreservations_get_unitn_for_reservation_node($singlereservation->nid);
          foreach ($units as $unit) {

            // Checks if unit is a booked item:
            if (in_array($unit->nid, $tmpunititems)) {
              $itemdate1 = new DateObject($singlereservation->field['value'], $singlereservation->field['timezone_db'], NULL);
              $itemdate1->setTimezone(timezone_open(date_default_timezone()));
              $itemdate2 = new DateObject($singlereservation->field['value2'], $singlereservation->field['timezone_db'], NULL);
              $itemdate2->setTimezone(timezone_open(date_default_timezone()));
              $pos = FALSE;

              if (isset($itemdate1)) {
                $pos = strpos($itemdate1->format("Y-m-d"), $row['data']['date'], 0);
              }
              $pos2 = FALSE;
              if (isset($itemdate2)) {
                $pos2 = strpos($itemdate2->format("Y-m-d"), $row['data']['date'], 0);
              }

              if (( ($pos2 === FALSE)) || (($pos !== FALSE) && ($pos2 !== FALSE))) {
                $row['data']['all_day'][$unit->title] = $singlereservation;
              }
            }
          }
        }
      }
      reset($row['data']['all_day']);
      ksort($row['data']['all_day']);


      $rowcopy = $row['data']['all_day'];
      $row['data']['all_day'] = array();
      foreach ($rowcopy as $ikey => $singlereservation) {
        $tmpunititems = array();
        // Rooms per reservation pick
        $tmpunititems = _agreservations_get_unitn_for_reservation_node($singlereservation->nid);

        foreach ($units as $unit) {
          if (in_array($unit->nid, $tmpunititems)) {
            $row['data']['all_day'][$unit->title] = $singlereservation;
          }
        }
      }
      $columns[] = $weekno;
      foreach ($units as $unit) {
        // Initialize $grouped_items
        $grouped_items[$unit->title] = array();
        $start_times = $view->date_info->style_groupby_times;
        $start_time = array_shift($start_times);
        $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;
        ksort($row['data']['all_day']);
        foreach ($row['data']['all_day'] as $key => $item) {
          $temp_end = date_create($item->row->field_field_agres_rdate[0]['raw']['value2']);
          $temp_granularity_end = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
          if ($temp_end > $temp_granularity_end) {
            $continues = 1;
          }
          else {
            $continues = 0;
          }
          $temp_start = date_create($item->row->field_field_agres_rdate[0]['raw']['value']);
          $temp_granularity_start = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
          if ($temp_start < $temp_granularity_start) {
            $continuation = 1;
          }
          else {
            $continuation = 0;
          }          
          $itemclass = 'itemwithoutorder';
          if (!isset($item->node->field_agres_orderid[$item->node->language])||($item->node->field_agres_orderid[$item->node->language][0]['value']==0)) {
            // Treat as receptionist manual reservation defaultcolor orange
            $itemclass = 'itemwithoutorder';
          }
          else {
            // Treat as receptionist manual reservation defaultcolor blue
            $itemclass = 'itemwithorder';
          }

          $themeparams = array(
            'view' => $view,
            'rendered_fields' => $item->rendered_fields,
            'item' => $item,
            'continues' => $continues,
            'continuation' => $continuation,
            'itemclass' => $itemclass,
          );
          // Units per reservation
          $tmpunititems = _agreservations_get_unitn_for_reservation_node($item->nid);
          // Unit is in booked item
          if ($unit->title === $key) {
            if ((!isset($vars['rinfo'][$unit->title][$item->nid]))) {
              $vars['rows'][$weekno]['night'][$unit->title][$item->nid] = theme('agres_views_item', $themeparams);
              $vars['rinfo'][$unit->title][$item->nid] = '1';
              $vars['spaninfo'][$unit->title][$item->nid] = 1;
            }
            else {
              $vars['rows'][$weekno]['night'][$unit->title][$item->nid] = '***busy***';
              $vars['spaninfo'][$unit->title][$item->nid]++;
            }
          }
          $item_count++;
        }

        foreach ($row['data']['items'] as $time => $items) {
          foreach ($items as $item) {

            // Find the next time slot and fill it. Populate the skipped
            // slots if the option to show empty times was chosen.
            while ($time >= $next_start_time && $time < $end_start_time) {
              if (is_array($grouped_items[$unit->title])) {
                if (!array_key_exists($start_time, $grouped_items[$unit->title])) {
                  $grouped_items[$unit->title][$start_time]['values'][$weekno] = array();
                }
              }
              else {
                $grouped_items[$unit->title][$start_time]['values'][$weekno] = array();
              }
              $start_time = $next_start_time;
              $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;
            }

            $tmpunititems = array();
            // Units per reservation
            $tmpunititems = _agreservations_get_unitn_for_reservation_node($item->nid);
            if (in_array($unit->nid, $tmpunititems)) {

            }
            $item_count++;
            $by_hour_count++;
          }
        }
        // Finish out the day's time values if we want to see empty times.
        if ($show_empty_times) {
          while ($start_time < $end_start_time) {
            if (is_array($grouped_items)) {
              if (is_array($grouped_items[$unit->title])) {
                if (!array_key_exists($start_time, $grouped_items[$unit->title])) {
                  $grouped_items[$unit->title][$start_time]['values'][$weekno] = array();
                }
              }
              else {
                $grouped_items[$unit->title][$start_time]['values'][$weekno] = array();
              }
            }
            $start_time = $next_start_time;
            $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;
          }
        }
      }
    }
    ksort($grouped_items);
    $i = 0;

    $vars['items'] = $grouped_items;
    $vars['day_names'] = $day_names;
    $vars['columns'] = $columns;
    $vars['start_times'] = $view->date_info->style_groupby_times;

    $vars['agenda_hour_class'] = 'calendar-agenda-hour';
    $first_column_width = 10;

    if (empty($view->date_info->style_groupby_times)) {
      $vars['agenda_hour_class'] .= ' calendar-agenda-no-hours';
      $first_column_width = 1;
    }
    $vars['item_count'] = $item_count;
    $vars['by_hour_count'] = $by_hour_count;
    $vars['units'] = $units;
    $vars['unittypes'] = $unittypes;
    $vars['calendarname'] = $view->name;
    $vars['currentselectedweek'] = $view->args[0]; // year.'-W'.$week;
    $vars['template_file'] = 'agreservations-calendar-week-2';
    if (module_exists('agres_categories')) {
      $vars['template_file'] = 'agreservations-calendar-week-c';
      $vars['categories'] = $categories;
    }
    return;
  }
  // Normal calendar
  else {
    $vars['template_file'] = 'agreservations-calendar-week-1';
    return;
  }
}

/**
 * Create the agres_views date box.
 */
function template_preprocess_agres_views_datebox(&$vars) {
  $date = $vars['date'];
  $view = $vars['view'];
  $vars['day'] = intval(drupal_substr($date, 8, 2));
  $force_view_url = !empty($view->date_info->block) ? TRUE : FALSE;
  $month_path = agres_views_granularity_path($view, 'month');
  $week_path = agres_views_granularity_path($view, 'week');
  $year_path = agres_views_granularity_path($view, 'year');
  $day_path = agres_views_granularity_path($view, 'day');
  $xtempurl = date_pager_url($view, NULL, $date, $force_view_url);
  $vars['url'] = str_replace(array($month_path, $year_path, $week_path), $day_path, $xtempurl);
  $vars['link'] = !empty($day_path) ? l($vars['day'], $vars['url']) : $vars['day'];
//   drupal_set_message('<pre>cssssssutemplate_preprocess_agres_views_datebox '.print_r(agres_views_granularity_path($view, 'week'),true).'</pre>');
  $vars['granularity'] = $view->date_info->granularity;
  $vars['mini'] = !empty($view->date_info->mini);
  if ($vars['mini']) {
    if (!empty($vars['selected'])) {
      $vars['class'] = 'mini-day-on';
    }
    else {
      $vars['class'] = 'mini-day-off';
    }
  }
  else {
    $vars['class'] = 'day';
  }
}

/**
 * Format an agres_views month node for display.
 */
function template_preprocess_agres_views_month_multiple_node(&$vars) {
  $view = $vars['view'];
  $curday = $vars['curday'];

  // Get the year month and date
  $parts = explode('-', drupal_substr($curday, 0, 10));
  $year = $parts[0];
  $month = intval($parts[1]);
  $day = intval($parts[2]);

  // Create the link to the day
  $vars['link'] = date_pager_url($view, NULL, date_pad($year, 4) . '-' . date_pad($month) . '-' . date_pad($day));
}

/**
 * Format an agres_views week node for display.
 */
function template_preprocess_agres_views_week_multiple_node(&$vars) {
  $view = $vars['view'];
  $curday = $vars['curday'];

  // Get the year month and date
  $parts = explode('-', drupal_substr($curday, 0, 10));
  $year = $parts[0];
  $month = intval($parts[1]);
  $day = intval($parts[2]);

  // Create the link to the day
  $vars['link'] = date_pager_url($view, NULL, date_pad($year, 4) . '-' . date_pad($month) . '-' . date_pad($day));
}

/**
 * Theme function for rendering views fields as a agres_views 'item'.
 *
 * $vars['rendered_fields'] = An array of the rendered display of each field in the View.
 * $vars['item'] = The source data for this item.
 * $vars['view'] = The view that this item is displayed on.
 *
 * @todo We need some options about how to combine rendered fields. Fields
 *  rendered in multiday containers need to be inline.
 */
function template_preprocess_agres_views_item(&$vars) {

}

/**
 * Format the time row headings in the week and day view.
 */
function theme_agres_views_time_row_heading($vars) {
  $start_time = $vars['start_time'];
  $next_start_time = $vars['next_start_time'];
  $curday_date = $vars['curday_date'];
  static $format_hour, $format_ampm;
  if (empty($format_hour)) {
    $format = 'm/d/Y - H:i';
    $format_hour = str_replace(array('a', 'A'), '', date_limit_format($format, array('hour', 'minute')));
    $format_ampm = strstr($format, 'a') ? 'a' : (strstr($format, 'A') ? 'A' : '');
  }
  if ($start_time == '00:00:00' && $next_start_time == '23:59:59') {
    $hour = t('All');
  }
  elseif ($start_time == '00:00:00') {
    $date = date_create($curday_date . ' ' . $next_start_time);
    $hour = t('< @time', array('@time' => date_format($date, $format_hour)));
  }
  else {
    $date = date_create($curday_date . ' ' . $start_time);
    $hour = date_format($date, $format_hour);
  }
  if (!empty($date)) {
    $ampm = date_format($date, $format_ampm);
  }
  else {
    $ampm = '';
  }
  return array('hour' => $hour, 'ampm' => $ampm);
}

/**
 * Format a node stripe legend
 */
function theme_agres_views_stripe_legend() {
  if (empty($GLOBALS['agres_views_stripes'])) {
    return '';
  }
  $header = array(
    array(
      'class' => 'agres_views-legend',
      'data' => t('Item'),
    ),
    array(
      'class' => 'agres_views-legend',
      'data' => t('Key'),
    ),
  );
  $rows = array();
  $output = '';
  foreach ((array) $GLOBALS['agres_views_stripes'] as $label => $stripe) {
    if ($stripe) {
      $rows[] = array($label, '<div style="background-color:' . $stripe . ';color:' . $stripe . '" class="stripe" title="Key: ' . $label . '">&nbsp;</div>');
    }
  }
  if (!empty($rows)) {
    $variables = array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('mini', 'agres_views-legend')),
    );
    $output .= theme('table', $variables);
    $output = '<div class="agres_views legend">' . $output . '</div>';
  }
  return $output;
}

/**
 * Format item stripes
 */
function theme_agres_views_stripe_stripe($vars) {
  $item = $vars['item'];
  if (empty($item->stripe) || (!count($item->stripe))) {
    return;
  }
  $output = '';
  if (is_array($item->stripe_label)) {
    foreach ($item->stripe_label as $k => $stripe_label) {
      if (!empty($item->stripe[$k]) && !empty($stripe_label)) {
        $GLOBALS['agres_views_stripes'][$stripe_label] = $item->stripe[$k];
        $output .= '<div style="background-color:' . $item->stripe[$k] . ';color:' . $item->stripe[$k] . '" class="stripe" title="Key: ' . $item->stripe_label[$k] . '">&nbsp;</div>' . "\n";
      }
    }
  }
  return $output;
}

/**
 *
 * @todo Documentation
 */
function template_preprocess_agres_views_month_col(&$vars) {

}

/**
 * Format an empty day on a agres_views
 *
 * @param day
 *   The day to display.
 */
function theme_agres_views_empty_day(&$vars) {
  $curday = $vars['curday'];
  $view = $vars['view'];
  if ($view->style_plugin->date_info->agres_views_type != 'day') {
    return '<div class="agres_views-empty">&nbsp;</div>' . "\n";
  }
  return '<div class="agres_views-dayview-empty">' . t('Empty day') . '</div>';
}