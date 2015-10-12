<?php
/**
 * additional themingfunctions for agreservations and booking agres_availability
 */


/**
 * Format the time row headings in the week and day view.
 */
//function theme_agres_availability_time_row_heading($start_time, $next_start_time, $curday_date) {
//  static $format_hour, $format_ampm;
//  if (empty($format_hour)) {
////    $format = //variable_get('date_format_short', 'm/d/Y - H:i');
//    $format =  'm/d/Y - H:i';
////    $format_hour = str_replace(array('a', 'A'), '', date_limit_format($format, array('hour', 'minute')));
//    $format_hour = str_replace(array('a', 'A'), '', date_limit_format($format, array('hour', 'minute')));
//    $format_ampm = strstr($format, 'a') ? 'a' : (strstr($format, 'A') ? 'A' : '');
//  }
//  if ($start_time == '00:00:00' && $next_start_time == '23:59:59') {
//    $hour = t('All');
//  }
//  elseif ($start_time == '00:00:00') {
//    $date = date_create($curday_date .' '. $next_start_time);
//    $hour = t('< @time', array('@time' => date_format($date, $format_hour)));
//  }
//  else {
//    $date = date_create($curday_date .' '. $start_time);
//    $hour = date_format($date, $format_hour);
//  }
//  if (!empty($date)) {
//    $ampm = date_format($date, $format_ampm);
//  }
//  else {
//    $ampm = '';
//  }
//  return array('hour' => $hour, 'ampm' => $ampm);
//}

/**
 * Implements hook_preprocess_date_views_pager().
 *
 * Creates a calendar_links array which is stored in the session and used
 * in calendar_menu_local_tasks_alter() to display the links as action items and dynamic tabs.
 * The links can be altered or deleted using hook_calendar_links_alter().
 *
 */
//function agres_availability_preprocess_date_views_pager(&$vars) {
//  $view = $vars['plugin']->view;
//  if ($view->plugin_name != 'agres_availability_style') {
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
//  // Find all the displays in this view that use the agres_availability style and have a path and create links to each.
//  $agres_availability_links[$current_path] = array();
//  $base = array('attributes' => array('rel' => 'nofollow'));
//  foreach ($view->display as $id => $display) {
//    // Check for !empty() in case the view is not fully formed or has displays that are marked to be deleted.
//    if (!empty($display->deleted) || empty($display->display_options['style_plugin'])) {
//      continue;
//    }
//    if ($display->display_options['style_plugin'] == 'agres_availability_style' && !empty($display->display_options['path'])) {
//      $path = $display->display_options['path'];
//      $title = $display->display_title;
//      $type = !empty($display->display_options['style_options']['agres_availability_type']) ? $display->display_options['style_options']['agres_availability_type'] : 'month';
//
//      // Make sure the links to other agres_availability displays use the right path for that display.
//      // Get rid of pager links when swapping between displays to force the base argument
//      // to be structured correctly for the type of display. This means you can't use
//      // these links in a block or panel.
//      $href = str_replace($current_path, $path, date_pager_url($view, $type, NULL, TRUE, FALSE));
//      $part_path = str_replace(base_path(), '', $href);
//
//      // Once we have a path for the links to other displays, add it to our links array.
//      $agres_availability_links[$current_path]['tabs'][$type] = array(
//        'title' => $title,
//        'path' => $part_path,
//      );
//
//      $agres_availability_links[$current_path]['types'][] = $type;
//    }
//  }
//
//  // If an 'Add new ... link is provided, add it here.
//  if (!empty($view->date_info->agres_availability_date_link) && !empty($view->date_info->url)
//   && (user_access("administer nodes") || user_access('create ' . $view->date_info->agres_availability_date_link . ' content'))) {
//    $name = node_type_get_name($view->date_info->agres_availability_date_link);
//    $href = 'node/add/' . str_replace('_', '-', $view->date_info->agres_availability_date_link);
//    $agres_availability_links[$current_path]['actions'][] = array(
//      'title' => t('Add @name', array('@name' => $name)),
//      'path' => $href,
//    );
//  }
//
//  // Pass this through drupal_alter() so it can be adjusted in custom code or in the theme.
//  drupal_alter('agres_availability_links', $agres_availability_links);
//  // Add the value to the session so it can be used to create the tabs.
//  $_SESSION['agres_availability_links'][$current_path] = $agres_availability_links[$current_path];
//
//}

///**
// * Add agres_availability links above the pager.
// */
//function agres_availability_preprocess_date_views_pager(&$vars) {
//  $view = $vars['plugin']->view;
//  if ($view->plugin_name != 'agres_availability_style') {
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
//  // Find all the displays in this view that use the agres_availability style and have a path and create links to each.
//  $agres_availability_links = array();
//  $base = array('attributes' => array('rel' => 'nofollow'));
//  foreach($view->display as $id => $display) {
//    if ($display->display_options['style_plugin'] == 'agres_availability_style' && !empty($display->display_options['path'])) {
//      $path = $display->display_options['path'];
//      $title = $display->display_title;
//      // @TODO Why is this sometimes empty for a style that uses the default value?
//      $type = !empty($display->display_options['style_options']['agres_availability_type']) ? $display->display_options['style_options']['agres_availability_type'] : 'month';
//
//      // Make sure the links to other agres_availability displays use the right path for that display.
//      // Get rid of pager links when swapping between displays to force the base argument
//      // to be structured correctly for the type of display. This means you can't use
//      // these links in a block or panel.
//      $href = str_replace($current_path, $path, date_pager_url($view, $type, NULL, TRUE));
//
//      // Once we have a path for the links to other displays, add it to our links array.
//      $agres_availability_links['agres_availability agres_availability-'. $type] = array('title' => $title, 'href' => $href);
//    }
//  }
//
//  // If an 'Add new ... link is provided, add it here.
//  // the query will bring the user back here after adding the node.
//  if (!empty($view->date_info->agres_availability_date_link)
//  && (user_access("administer nodes") || user_access('create '. $view->date_info->agres_availability_date_link .' content'))) {
//    $name = node_type_get_name($view->date_info->agres_availability_date_link);
//    $href = 'node/add/' . str_replace('_', '-', $view->date_info->agres_availability_date_link);
//    $query = drupal_get_query_parameters(array('destination' => $view->date_info->url));
//    $agres_availability_links['agres_availability agres_availability-add'] = $base + array(
//      'title' => t('Add+'),
//      'href' => $href,
//      'query' => $query,
//      );
//  }
//
//  // Append the agres_availability links above the pager.
//  $links = array(
//    'links' => $agres_availability_links,
//    'attributes' => array('class' => array('agres_availability-links', 'inline')),
//  );
//  $vars['pager_prefix'] = theme('links', $links);
//}

/**
 * Preprocess an RSS feed
 */
function template_preprocess_agres_availability_style(&$vars) {
//    drupal_set_message('<pre>monthtemplate_preprocess_agres_availability_style '.print_r( $vars,true).'</pre>');
  global $base_url;
  global $language;

}

/**
 * Display a month view.
 */
function template_preprocess_agres_availability_month(&$vars) {


  $view = $vars['view'];
  $rows = $vars['rows'];

  if (sizeof($rows) > 1) {
    $day_names = array_shift($rows);
  }
  else {
    $day_names = $rows;
    $rows = array();
  }
  foreach($rows as $kweek=>$week2check) {
    foreach($week2check as $kday=>$day2check) {
//      drupal_set_message('<pre>bis hier22' . print_r($day2check, true) . '</pre>');
      foreach($day2check['data']['items'] as $kitems=>$items2check) {
//        drupal_set_message('<pre>bis hier22' . print_r($items2check, true) . '</pre>');
        foreach($items2check as $key2delete => $item2check){
//          $x = new DateObject;          
          if ($item2check->date_end->format('Y-m-d')<$view->date_info->date_range[0]->format('Y-m-d')) {
//            drupal_set_message('<pre>bis hier22' . print_r($kitems, true) . '</pre>');
//            drupal_set_message('<pre>bis hier2332' . print_r($rows[$kweek][$kday]['data']['items'][$kitems][$key2delete], true) . '</pre>');
            unset($rows[$kweek][$kday]['data']['items'][$kitems][$key2delete]);
          }
//          drupal_set_message('<pre>bis hier22' . print_r($timetocheck, true) . '</pre>');
        }
      }
      
    }    
  }
//  foreach($rows as $kweek=>$week2check) {
//    foreach($week2check as $kday=>$day2check) {
//      drupal_set_message('<pre>bis hier22' . print_r($day2check, true) . '</pre>');
//    }
//  }
//  
  $vars['rows'] = $rows;
  $rows = $vars['rows'];
//  drupal_set_message('<pre>bis hier11' . print_r($view->date_info->date_range[0]->format('Y-m-d'), true) . '</pre>');
//  drupal_set_message('<pre>bis hier22' . print_r($rows, true) . '</pre>');
  $month_rows = $rows;
 
 
  /*      * **************setup categories and unittypes:******************************************
   *
   */
//  drupal_set_message('<pre>*or**'. print_r($view, TRUE) .'</pre>');
  $vars['unittypes'] = '';
  $countarg = count($view->args);

  if (module_exists('agres_categories')) {
    $countarg = count($view->args);
    if ($countarg > 4) {
      $view->args = array_slice($view->args, 0, 3);
    }
    $vars['currentcategory'] = '';
    if (isset($view->args[1]) && $view->args[1]) {
//         drupal_set_message('<pre>bis hier' . print_r($vars['unittypes'], true) . '</pre>');
      $unittypes = _agres_categories_get_unittypenodes_of_category($view->args[1]);
      $vars['currentcategory'] = $view->args[1];
      $categories = _agres_categories_get_categories(TRUE);
      if (isset($view->args[2]) && $view->args[2]) {
//        $units = _agreservations_get_unitnodes($view->args[2]);
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
//        drupal_set_message('<pre>bis else'.print_r($unittypes,true).'</pre>');
      $categories = _agres_categories_get_categories(TRUE);
      $units = _agreservations_get_unitnodes();
    }
  }
  else {
    if ($countarg > 3) {
      $view->args = array_slice($view->args, 0, 2);
    }
    if (isset($view->args[1])) {
//        drupal_set_message('<pre>*view->args[1]view->args[1]**'. print_r($vars['view']->name, TRUE) .'</pre>');
      $units = _agreservations_get_unitnodes($view->args[1]);
      $vars['currentunittype'] = $view->args[1];
    }
    else {
      $units = _agreservations_get_unitnodes();
//        drupal_set_message('<pre>*elseelseelse]**'. print_r($units, TRUE) .'</pre>');
    }
    $unittypes = _agreservations_get_unittypenodes();
  }
  //remove the args from agrescurrentpath:


  $toremovefrompath = implode("/", $view->args);
  $agrescurrentpath = get_current_path();
  $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
  //cut off trailing slash:


  if (substr($agrescurrentpath, -1, 1) == "/") {
    $agrescurrentpath = substr($agrescurrentpath, 0, strlen($agrescurrentpath) - 1);
  }
//    drupal_set_message('<pre>*orig**'. print_r($agrescurrentpath, TRUE) .'</pre>');


  $vars['agrescurrentpath'] = $agrescurrentpath;

//    drupal_set_message('<pre>*orig**'. print_r($rows, TRUE) .'</pre>');
  //step1: all in 1 array:
  $agdaysresitems = array();
  $agmonth_rows = array();
  foreach ($rows as $weekno => $row) {
    foreach ($row as $day => $data) {

      $agdaysresitems[] = $data;
    }
  }

  $agmonth_rows['rinfo'] = array();
  //step2 organize in unit arrays:
//     drupal_set_message('<pre>*orig**'. print_r($agdaysresitems, TRUE) .'</pre>');
  foreach ($units as $unit) {
    $agmonth_rows['rinfo'][$unit->title] = array();

    foreach ($agdaysresitems as $day => $agdayresitem) {
 //04.03.2012 remove items that do not belong into this months granularity:
      
      $data = $agdayresitem['data']['datebox'];
      $date = $agdayresitem['data']['date'];
      $agmonth_rows[$unit->title][$day]['datebox'] = $data;
      $agmonth_rows[$unit->title][$day]['date'] = $date;
//         drupal_set_message('<pre>*$day**'. print_r($date, TRUE) .'</pre>');

      foreach ($agdayresitem['data']['items'] as $time => $hour) {
//             drupal_set_message('<pre>*orig**'. print_r($time, TRUE) .'</pre>');
        //foreach reservation objet node in the hour DO:
        foreach ($hour as $resob) {
          if (!isset($agmonth_rows[$unit->title][$day]['resitm'][$resob->nid])) {
            $agmonth_rows[$unit->title][$day]['resitm'][$resob->nid] = '0';
          }


          $continues = 0;
          $continuation = 0;

          $tmpunititems = _agreservations_get_unitn_for_reservation_node($resob->nid); //get units per reservation
          if (in_array($unit->nid, $tmpunititems)) {
            $tmpEnd = date_create($resob->row->field_field_agres_rdate[0]['raw']['value2']);
            $tmpGranularityEnd = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
            if ($tmpEnd > $tmpGranularityEnd) {
              $continues = 1;

            }
            else {
              $continues = 0;
//                  drupal_set_message('<pre>*$continues = 0;**'. print_r(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME), TRUE) .'</pre>');
            }
            $tmpstart = date_create($resob->row->field_field_agres_rdate[0]['raw']['value']);
            $tmpGranularitystart = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
            if ($tmpstart < $tmpGranularitystart) {
              $continuation = 1;
            }
            else {
              $continuation = 0;
            }

            $itemclass = 'itemwithoutorder';
            if (!isset($resob->node->field_agres_orderid[$resob->node->language])) {
              $itemclass = 'itemwithoutorder'; //treat as receptionist manual reservation defaultcolor orange
            }
            else {
              $itemclass = 'itemwithorder'; //treat as receptionist manual reservation defaultcolor blue
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
              $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = theme('agres_availability_item', $themeparams); //theme($theme, $variables); //$resob->rendered['nid'];
              //only the first:
              $agmonth_rows[$unit->title][$day]['data'] = array_slice($agmonth_rows[$unit->title][$day]['data'], 0, 1, true);
            }
            else {
              $endpos = strpos($resob->row->field_field_agres_rdate[0]['raw']['value2'], $agmonth_rows[$unit->title][$day]['date'], 0); //strpos($resob->raw->field_data_field_agres_rdate_field_agres_rdate_value2, $agmonth_rows[$unit->title][$day]['date'], 0);
              if (empty($agmonth_rows[$unit->title][$day]['data'][$resob->nid])) {
                if ($endpos === false) {
                  $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = '***busy***';
                  $agmonth_rows[$unit->title]['spaninfo'][$resob->nid] += 1;
                }
                else {
//                          $agmonth_rows[$unit->title][$day]['data'][$resob->id] = '***busy***';
//                          $agmonth_rows[$unit->title]['spaninfo'][$resob->id] = +1;
                }
              }
              else {

              }
            }
          }
        }
      }
      /////////////////////////////ALL DAY///////////////////////////////////////////////
      if (isset($agdayresitem['data']['all_day'])) {
        foreach ($agdayresitem['data']['all_day'] as $resob) {
          //foreach reservation objet node in the hour DO:
          if (!isset($agmonth_rows[$unit->title][$day]['resitm'][$resob->nid])) {
            $agmonth_rows[$unit->title][$day]['resitm'][$resob->nid] = '0';
          }

          $tmpunititems = _agreservations_get_unitn_for_reservation_node($resob->nid); //get units per reservation
          if (in_array($unit->nid, $tmpunititems)) {
            $itemclass = 'itemwithoutorder';
            $continues = 0;
            $continuation = 1;
            $tmpEnd = date_create($resob->row->field_field_agres_rdate[0]['raw']['value2']);
            $tmpGranularityEnd = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
            if ($tmpEnd > $tmpGranularityEnd) {
              $continues = 1;

            }
            else {
              $continues = 0;
//                  drupal_set_message('<pre>*$continues = 0;**'. print_r(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME), TRUE) .'</pre>');
            }
            $tmpstart = date_create($resob->row->field_field_agres_rdate[0]['raw']['value']);
            $tmpGranularitystart = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
            if ($tmpstart < $tmpGranularitystart) {
              $continuation = 1;
            }
            else {
              $continuation = 0;
            }

            if (!isset($resob->node->field_agres_orderid[$resob->node->language])) {
              $itemclass = 'itemwithoutorder'; //treat as receptionist manual reservation defaultcolor orange
            }
            else {
              $itemclass = 'itemwithorder'; //treat as receptionist manual reservation defaultcolor blue
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
              $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = theme('agres_availability_item', $themeparams); //$resob->rendered['nid'];
            }
            else {

//                     drupal_set_message('<pre>'.print_r($resob,true).'<---->'.print_r(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME),true).'</pre>');
//                     drupal_set_message('<pre>day:'.$day.'itm:'.$resob->nid.'sonst '.print_r($resob,true).'</pre>');
              $endpos = strpos($resob->row->field_field_agres_rdate[0]['raw']['value2'], $agmonth_rows[$unit->title][$day]['date'], 0);
              if (empty($agmonth_rows[$unit->title][$day]['data'][$resob->nid])) {
                if ($endpos === false) {
                  $agmonth_rows[$unit->title][$day]['data'][$resob->nid] = '***busy***';
                  $agmonth_rows[$unit->title]['spaninfo'][$resob->nid] += 1;
                }
                else {
//                          $agmonth_rows[$unit->title][$day]['data'][$resob->id] = '***busy***';
//                          $agmonth_rows[$unit->title]['spaninfo'][$resob->id] = +1;
                }
              }
              else {

              }
            }
          }
        }
      }
      ///////////////////////////////////////////////////////////////////////////////////
    }
  }


  $daymonth = 0;
  $vars['unittypes'] = $unittypes;
  $vars['currentselectedmonth'] = $view->args[0]; // year.'-W'.$week;
  $vars['agmonth_rows'] = $agmonth_rows;
  $vars['units'] = $units;
  $vars['rows'] = $month_rows;
  $vars['day_names'] = $day_names;
  $vars['template_file'] = 'agres_availability-calendar-month-c';
  $vars['display_type'] = $view->date_info->granularity;
  $vars['min_date_formatted'] = date_format($view->date_info->min_date, DATE_FORMAT_DATETIME);
  $vars['max_date_formatted'] = date_format($view->date_info->max_date, DATE_FORMAT_DATETIME);
  if (module_exists('agres_categories')) {
    $vars['categories'] = $categories;
  }
//    $vars['template_file'] = 'agres_availability-calendar-month-c';
  $path = drupal_get_path('module', 'agres_availability');
  $vars['directory'] = $path . '/theme';
}

/**
 * Display a mini month view.
 */
function template_preprocess_agres_availability_mini(&$vars) {
     // Add in all the $vars added by the main agres_availability preprocessor.
     //$data['availability1'] = 2  ---> all free
     //$data['availability1'] = 1  ---> partly booked/partly free
     //$data['availability1'] = 99 ---> all booked
     //$data['availability1'] =    ---> no reservations that day
  
     template_preprocess_agres_availability_month($vars);
//drupal_set_message('<pre>dddddddddddddddtemplate_preprocess_agres_availability_mini  '.print_r($vars,true).'</pre>');
     if (isset( $vars['view']->args['agrcategory_nid'])) {
        $vars['agrcategory_nid'] = $vars['view']->args['agrcategory_nid']; 
     }
     if (isset( $vars['view']->args['agrunittype_nid'])) {
        $vars['agrunittype_nid'] = $vars['view']->args['agrunittype_nid']; 
     }
     if (isset( $vars['view']->args['agrunit_nid'])) {
        $vars['agrunit_nid'] = $vars['view']->args['agrunit_nid']; 
     }     
     $view = $vars['view'];
     if (empty($view->exposed_input['mini'])) {
       $vars['yearinfo'] = $view->args[0];
     } else {
       $vars['yearinfo'] = $vars['view']->exposed_input['mini'];
     }
     $rows = $vars['rows'];
  
  $vars['unittypes'] = '';
  $countarg = count($view->args);
  if (isset($vars['view']->args['agrcategory_nid'])) {
    $unitnodes = _agres_categories_get_unitnodes_of_category(NULL,$vars['view']->args['agrcategory_nid']);
  } else {
    if (isset($_REQUEST["cat"])) {
      $unitnodes = _agres_categories_get_unitnodes_of_category(NULL, $_REQUEST["cat"]);
    } else {
      $unitnodes = _agreservations_get_unitnodes();
    }
  }

  //remove the args from agrescurrentpath:
  $toremovefrompath = implode("/", $view->args);
  $agrescurrentpath = get_current_path();
  $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
  //cut off trailing slash:
  if (substr($agrescurrentpath, -1, 1) == "/") {
    $agrescurrentpath = substr($agrescurrentpath, 0, strlen($agrescurrentpath) - 1);
  }
  $vars['agrescurrentpath'] = $agrescurrentpath;
//     $unitnodes = _agreservations_get_unitnodes();
     $cntunit = count($unitnodes);
     $agdaysresitems = array();
     $agmonth_rows = array();

     foreach ($vars['rows'] as $weekno => $row) {
       foreach ($row as $day => $data) {
         if (count($data['data']['items']) == 0) {
           if ($cntunit > 0) {
             $data['availability1'] = 2;
           } else {
             $data['availability1'] = 99;
             //display as unavailable/blocked
             if (isset($vars['agrcategory_nid'])&&$vars['agrcategory_nid']>0) {
                $data['availability1'] = 88;
             }            
           }

           $agdaysresitems[$weekno][$data['id']] = $data;
         } else {
           if ($cntunit > 0) {
             $unodespday = array();
             foreach ($data['data']['items'] as $reservationsday) {
              
               ////date timezonehandling handling:
               foreach ($reservationsday as $reservation) {
                 $notadd = false;
                 if (isset($reservation->node->field_agres_rdate)) {
                   foreach ($reservation->node->field_agres_rdate as $country => $resdates) {
                     foreach ($resdates as $resdate) {
                       $dateTimezone1 = new DateTimeZone($resdate['timezone']);
                       $resenddate = new DateTime($resdate['value2'], $dateTimezone1);
                       $offset = $resenddate->getOffset();
                       $offset = $offset * 1;
                       $resenddate->modify($offset . 'seconds');
                       $datobj = new DateTime($data['data']['date'], $dateTimezone1);
                       if (strpos($resenddate->format('Y-m-d H:i:s'), $datobj->format('Y-m-d')) !== false) {
                         $notadd = true;
                       }
                     }
                   }
                   if ($notadd == false) {
                     foreach ($reservation->node->field_agres_ref_unit as $country => $unodenodes) {
                       foreach ($unodenodes as $unode) {
//drupal_set_message('<pre>dddddddddddddddtemplate_preprocess_agres_availability_mini  '.print_r($reservation,true).'</pre>');
                         $unodespday[] = $unode;
                       }
                     }
                   }
                 }
               }
             }
             if (($cntunit > count($unodespday))&&(count($unodespday)>0)) {
               $data['availability1'] = 1; //partly free
             } elseif ($cntunit <= count($unodespday)&&(!empty($unodespday))){
               $data['availability1'] = 99; //fully booked   
               //display as unavailable/blocked
               if (isset($vars['agrcategory_nid'])&&$vars['agrcategory_nid']>0) {
                  $data['availability1'] = 88;
               }                 
             } elseif(($cntunit > 0) &&(empty($unodespday))) {
                $data['availability1'] = 2;
             }
             $agdaysresitems[$weekno][$data['id']] = $data;
//               drupal_set_message('<pre>ddddddddddddddd'.print_r($unodespday,true).'</pre>');
           } else {
             $data['availability1'] = 99;
             //display as unavailable/blocked
             if (isset($vars['agrcategory_nid'])&&$vars['agrcategory_nid']>0) {
                $data['availability1'] = 88;
             }               
             $agdaysresitems[$weekno][$data['id']] = $data;
           }
         }
//         $agdaysresitems[$data['id']] =
       }
     }

     // Make sure that the agres_availability title links go to the month view,
     // not the year view (if this is embedded in a year display).
     $view->override_path = agres_availability_granularity_path($view, 'month');
     $vars['agdaysresitems'] = $agdaysresitems;
     $view->date_info->show_title = !empty($view->date_info->show_title) ? $view->date_info->show_title : FALSE;
     $vars['show_title'] = $view->date_info->show_title;
     $vars['view'] = $view;
   }
   
   /**
 * Display a mini month view.
 */
function template_preprocess_agres_availability_mini_unittype(&$vars) {
     // Add in all the $vars added by the main agres_availability preprocessor.
     //$data['availability1'] = 2  ---> all free
     //$data['availability1'] = 1  ---> partly booked/partly free
     //$data['availability1'] = 99 ---> all booked
     //$data['availability1'] =    ---> no reservations that day
     template_preprocess_agres_availability_month($vars);
//    drupal_set_message('<pre>dddddddddddddddtemplate_preprocess_agres_availability_mini_unittype '.print_r($vars,true).'</pre>');   
       
     $view = $vars['view'];
     if (empty($view->exposed_input['mini'])) {
       $vars['yearinfo'] = $view->args[0];
     } else {
       $vars['yearinfo'] = $vars['view']->exposed_input['mini'];
     }
     $rows = $vars['rows'];
  
  $vars['unittypes'] = '';
  $countarg = count($view->args);
  if (isset($_REQUEST["cat"])){
    $unitnodes = _agres_categories_get_unitnodes_of_category(NULL, $_REQUEST["cat"]);
  }
  else {
    $unitnodes = _agreservations_get_unitnodes();
  }
  //remove the args from agrescurrentpath:
  $toremovefrompath = implode("/", $view->args);
  $agrescurrentpath = get_current_path();
  $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
  //cut off trailing slash:
  if (substr($agrescurrentpath, -1, 1) == "/") {
    $agrescurrentpath = substr($agrescurrentpath, 0, strlen($agrescurrentpath) - 1);
  }
  $vars['agrescurrentpath'] = $agrescurrentpath;
//     $unitnodes = _agreservations_get_unitnodes();
     $cntunit = count($unitnodes);
     $agdaysresitems = array();
     $agmonth_rows = array();

     foreach ($vars['rows'] as $weekno => $row) {
       foreach ($row as $day => $data) {
         if (count($data['data']['items']) == 0) {
           if ($cntunit > 0) {
             $data['availability1'] = 2;
           } else {
             $data['availability1'] = 99;
           }

           $agdaysresitems[$weekno][$data['id']] = $data;
         } else {
           if ($cntunit > 0) {
             $unodespday = array();
             foreach ($data['data']['items'] as $reservationsday) {
               ////date timezonehandling handling:
               foreach ($reservationsday as $reservation) {
                 $notadd = false;
                 if (isset($reservation->node->field_agres_rdate)) {
                   foreach ($reservation->node->field_agres_rdate as $country => $resdates) {
                     foreach ($resdates as $resdate) {
                       $dateTimezone1 = new DateTimeZone($resdate['timezone']);
                       $resenddate = new DateTime($resdate['value2'], $dateTimezone1);
                       $offset = $resenddate->getOffset();
                       $offset = $offset * 1;
                       $resenddate->modify($offset . 'seconds');
                       $datobj = new DateTime($data['data']['date'], $dateTimezone1);
                       if (strpos($resenddate->format('Y-m-d H:i:s'), $datobj->format('Y-m-d')) !== false) {
                         $notadd = true;
                       }
                     }
                   }
                   if ($notadd == false) {
                     foreach ($reservation->node->field_agres_ref_unit as $country => $unodenodes) {
                       foreach ($unodenodes as $unode) {

                         $unodespday[] = $unode;
                       }
                     }
                   }
                 }
               }
             }
             if ($cntunit > count($unodespday)) {
               $data['availability1'] = 1; //partly free
             } else {
               $data['availability1'] = 99; //booked
             }
             $agdaysresitems[$weekno][$data['id']] = $data;
//               drupal_set_message('<pre>ddddddddddddddd'.print_r($unodespday,true).'</pre>');
           } else {
             $data['availability1'] = 99;
             $agdaysresitems[$weekno][$data['id']] = $data;
           }
         }
//         $agdaysresitems[$data['id']] =
       }
     }

     // Make sure that the agres_availability title links go to the month view,
     // not the year view (if this is embedded in a year display).
     $view->override_path = agres_availability_granularity_path($view, 'month');
     $vars['agdaysresitems'] = $agdaysresitems;
     $view->date_info->show_title = !empty($view->date_info->show_title) ? $view->date_info->show_title : FALSE;
     $vars['show_title'] = $view->date_info->show_title;
     $vars['view'] = $view;
   }
   
function template_preprocess_agres_availability_mini_category(&$vars) {
     // Add in all the $vars added by the main agres_availability preprocessor.
     //$data['availability1'] = 2  ---> all free
     //$data['availability1'] = 1  ---> partly booked/partly free
     //$data['availability1'] = 99 ---> all booked
     //$data['availability1'] =    ---> no reservations that day
     template_preprocess_agres_availability_month($vars);
//       drupal_set_message('<pre>dddddddddddddddmini_category '.print_r($vars,true).'</pre>');
       
     $view = $vars['view'];
     if (empty($view->exposed_input['mini'])) {
       $vars['yearinfo'] = $view->args[0];
     } else {
       $vars['yearinfo'] = $vars['view']->exposed_input['mini'];
     }
     $rows = $vars['rows'];
  
  $vars['unittypes'] = '';
  $countarg = count($view->args);
  if (isset($_REQUEST["cat"])){
    $unitnodes = _agres_categories_get_unitnodes_of_category(NULL, $_REQUEST["cat"]);
  }
  else {
    $unitnodes = _agreservations_get_unitnodes();
  }
  //remove the args from agrescurrentpath:
  $toremovefrompath = implode("/", $view->args);
  $agrescurrentpath = get_current_path();
  $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
  //cut off trailing slash:
  if (substr($agrescurrentpath, -1, 1) == "/") {
    $agrescurrentpath = substr($agrescurrentpath, 0, strlen($agrescurrentpath) - 1);
  }
  $vars['agrescurrentpath'] = $agrescurrentpath;
//     $unitnodes = _agreservations_get_unitnodes();
     $cntunit = count($unitnodes);
     $agdaysresitems = array();
     $agmonth_rows = array();

     foreach ($vars['rows'] as $weekno => $row) {
       foreach ($row as $day => $data) {
         if (count($data['data']['items']) == 0) {
           if ($cntunit > 0) {
             $data['availability1'] = 2;
           } else {
             $data['availability1'] = 99;
           }

           $agdaysresitems[$weekno][$data['id']] = $data;
         } else {
           if ($cntunit > 0) {
             $unodespday = array();
             foreach ($data['data']['items'] as $reservationsday) {
               ////date timezonehandling handling:
               foreach ($reservationsday as $reservation) {
                 $notadd = false;
                 if (isset($reservation->node->field_agres_rdate)) {
                   foreach ($reservation->node->field_agres_rdate as $country => $resdates) {
                     foreach ($resdates as $resdate) {
                       $dateTimezone1 = new DateTimeZone($resdate['timezone']);
                       $resenddate = new DateTime($resdate['value2'], $dateTimezone1);
                       $offset = $resenddate->getOffset();
                       $offset = $offset * 1;
                       $resenddate->modify($offset . 'seconds');
                       $datobj = new DateTime($data['data']['date'], $dateTimezone1);
                       if (strpos($resenddate->format('Y-m-d H:i:s'), $datobj->format('Y-m-d')) !== false) {
                         $notadd = true;
                       }
                     }
                   }
                   if ($notadd == false) {
                     foreach ($reservation->node->field_agres_ref_unit as $country => $unodenodes) {
                       foreach ($unodenodes as $unode) {

                         $unodespday[] = $unode;
                       }
                     }
                   }
                 }
               }
             }
             if ($cntunit > count($unodespday)) {
               $data['availability1'] = 1; //partly free
             } else {
               $data['availability1'] = 99; //booked
             }
             $agdaysresitems[$weekno][$data['id']] = $data;
//               drupal_set_message('<pre>ddddddddddddddd'.print_r($unodespday,true).'</pre>');
           } else {
             $data['availability1'] = 99;
             $agdaysresitems[$weekno][$data['id']] = $data;
           }
         }
//         $agdaysresitems[$data['id']] =
       }
     }

     // Make sure that the agres_availability title links go to the month view,
     // not the year view (if this is embedded in a year display).
     $view->override_path = agres_availability_granularity_path($view, 'month');
     $vars['agdaysresitems'] = $agdaysresitems;
     $view->date_info->show_title = !empty($view->date_info->show_title) ? $view->date_info->show_title : FALSE;
     $vars['show_title'] = $view->date_info->show_title;
     $vars['view'] = $view;
   }
/**
 * Display a year view.
 */
function template_preprocess_agres_availability_year(&$vars) {

  // Construct a agres_availability for each month, adjusting the $view passed
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
    $months[$month] = theme('agres_availability_mini', $variables);
  }
  $vars['months'] = $months;
  $vars['view']->date_info->hide_nav = FALSE;
  $vars['view']->date_info->granularity = 'year';
  $vars['mini'] = $view->date_info->mini;

}

/**
 * Display a day overlap view.
 */
function template_preprocess_agres_availability_day_overlap(&$vars) {

  template_preprocess_agres_availability_day($vars);
}

function template_preprocess_agres_availability_day(&$vars) {
  // Get rid of visual duplicates.
  $vars['view']->style_with_weekno = FALSE;
//       template_preprocess_calendar($vars);
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

//        $units = _agreservations_get_unitnodes($view->args[2]);
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
  //remove the args from agrescurrentpath:
  $toremovefrompath = implode("/", $view->args);
  $agrescurrentpath = get_current_path();
  $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
  //cut off trailing slash:
  if (substr($agrescurrentpath, -1, 1) == "/") {
    $agrescurrentpath = substr($agrescurrentpath, 0, strlen($agrescurrentpath) - 1);
  }
  $vars['agrescurrentpath'] = $agrescurrentpath;

  $columns = array();
  $grouped_items = array();
  $grouped_items = array();
  $spanning_items = array(); //test
  $r = 0;

//      drupal_set_message("<pre>gfcjcghgjgchj".print_r($rows,TRUE)."</pre>");
  foreach ($units as $unit) { //loop through all units
    foreach ($rows['all_day'] as $itmkey => $item) {

      $tmpunititems = _agreservations_get_unitn_for_reservation_node($item->nid); //get units of reservation
//           drupal_set_message("<pre>gfcjcghgjgchj".print_r($tmpunititems,TRUE)."</pre>");
      if (in_array($unit->nid, $tmpunititems)) { //if unit is in reserved units
        $column = t('Items');
        $theme = isset($item->calendar_node_theme) ? $item->calendar_node_theme : 'calendar_' . $view->date_info->granularity . '_node';
        //add the rendered items to the unit in the calendarmatrix
//        if (count($unit->actualitems[$column])==0) {
//            drupal_set_message("<pre>gfcjcghgjgchj".print_r($item->nid,TRUE)."</pre>");
//             $unit->actualitems['Items'][] = theme('agres_availability_item', array('view' => $view, 'rendered_fields' => $event[0]->rendered_fields, 'item' => $event[0])); //test// l(t("res:$item->nid"), "node/$item->nid", array('query' => drupal_get_destination()));
        $spanning_items[$column][] = theme($theme, $item, $view); //test
        //theme($theme, $item, $view);!!!"""§§§
        $item_count++;
      }
//      }
    }
  }

  $vars['rows']['all_day'] = $grouped_items;
  // Move timed items into the right columns and render them.
  $start_times = $view->date_info->style_groupby_times;
  $show_empty_times = FALSE;
  $end_start_time = '23:59:59';
  $grouped_items = array();
  $timeslots = $view->date_info->style_groupby_times;
//       drupal_set_message('<pre>'.print_r($timeslots,TRUE).'</pre>');
  array_push($timeslots, $end_start_time);
//  drupal_set_message('<pre>'.print_r($timeslots,TRUE).'</pre>');
  $timeslots_count = count($timeslots);
  //drupal_set_message("<pre>***timeslots_count**".print_r($timeslots_count,TRUE)."</pre>");
  //translate rows into better format
  $events = $rows['items'];
//       drupal_set_message("<pre>gfcjcghgjgchj".print_r($events,TRUE)."</pre>");
  foreach ($units as $unit) {
    $grouped_items[$unit->title] = array();
    foreach ($events as $elemkey => $event) {

      ////    if ($event->calendar_all_day) continue; // skip all day events.
      $group_key = isset($event->raw->{$grouping_field}) ? $event->raw->{$grouping_field} : t('Items'); // get initial value as formatted one already blanked
//
      $key = $group_key . '-' . 0;
      $columns["$group_key"] = array();
      if (!($columns["$group_key"])) { // init group on first encounter
        $columns["$group_key"][] = $group_key . '-0';
      }
//            settype($event->date_end,"DateTime");
//             drupal_set_message('<pre>$event  '.print_r($event,TRUE).'</pre>');
//             drupal_set_message('<pre>event g '.print_r($event,TRUE).'</pre>');
      //IF EVENTDATE < DAYCALENDARDATESTART:
      $tmpGranularityEnd = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
      if ($event[0]->date_start < $tmpGranularityEnd) {
        $event_start = '00:00:00';
      }
      else {
        $event_start = $event[0]->date_start->format('H:i:s');
      }
      $tmpGranularityEnd = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
      if ($event[0]->date_end > $tmpGranularityEnd) {
        $event_end = '23:59:59';
      }
      else {
        $event_end = $event[0]->date_end->format('H:i:s');
      }
//                       $tmpEnd = date_create($resob->row->field_field_agres_rdate[0]['raw']['value2']);
//                       $tmpGranularityEnd = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
//                       if($tmpEnd > $tmpGranularityEnd){
//                           $continues = 1;
//                       }else{
//                          $continues = 0;
//                       }
//
//           drupal_set_message("<pre>calendar_start_date  ".print_r($event_start,TRUE)."</pre>");
//           $event_end = $event[0]->date_end->format('H:i:s');
//            drupal_set_message('<pre>event g '.print_r($event_end,TRUE).'</pre>');
      $i = 0;
      $tmpunititems = _agreservations_get_unitn_for_reservation_node($event[0]->nid); //get units of reservation
      if (in_array($unit->nid, $tmpunititems)) {
        for ($slot = 0; $slot < $timeslots_count; $slot++) {
          if ($event_start >= $timeslots[$slot] &&
                       $event_end <= $timeslots[$slot + 1]) { // one slot event
//                 $theme = isset($event->calendar_node_theme) ? $event->calendar_node_theme : 'calendar_' . $view->date_info->granularity . '_node';
            // find free column to use
            $found = FALSE;
            foreach ($columns["$group_key"] as $ckey => $column) {
              $key = $group_key . '-' . $ckey;
              if (!($grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"])) {
                $found = TRUE;
                break;
              }
            }
            if (!$found) {
              $key = $group_key . '-' . (count($columns[$group_key]));
              $columns["$group_key"][] = $key; // add new column
            }

            $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = theme('agres_availability_item', array('view' => $view, 'rendered_fields' => $event[0]->rendered_fields, 'item' => $event[0])); // theme($theme, $event, $view);

          }
          elseif ($event_start >= $timeslots[$slot] &&
                       $event_start < $timeslots[$slot + 1] &&
                       $event_end > $timeslots[$i + 1]) { // first slot of multi slot event
            $theme = isset($event->calendar_node_theme) ? $event->calendar_node_theme : 'calendar_' . $view->date_info->granularity . '_node';
            // find free column to use
            $found = FALSE;
            foreach ($columns["$group_key"] as $ckey => $column) {
              $key = $group_key . '-' . $ckey;
              if (!($grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"])) {
                $found = TRUE;
                break;
              }
            }
            if (!$found) {
              $key = $group_key . '-' . (count($columns["$group_key"]));
              $columns["$group_key"][] = $key; // add new column
            }

            $span = $timeslots[$slot];
            $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = theme('agres_availability_item', array('view' => $view, 'rendered_fields' => $event[0]->rendered_fields, 'item' => $event[0])); // theme($theme, $event, $view);
            $grouped_items[$unit->title][$timeslots[$slot]]['span']["$key"] = 1;
          }
          elseif ($event_start < $timeslots[$slot] &&
                  $event_end > $timeslots[$slot]) { // middle of end slot of event
            $grouped_items[$unit->title][$span]['span']["$key"]++;
            $grouped_items[$unit->title][$timeslots[$slot]]['values']["$key"] = '***busy***';
          }
          else { // empty slot
            //$grouped_items[$timeslots[$slot]]['values']["$key"] = '';
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
      $heading = theme('agres_availability_time_row_heading', $variables);
//            drupal_set_message('<pre>dsfg'.print_r($heading,TRUE).'</pre>');
      $grouped_items[$unit->title][$start_time]['hour'] = $heading['hour'];
      $grouped_items[$unit->title][$start_time]['ampm'] = $heading['ampm'];
      $i++;
    }

    ksort($grouped_items[$unit->title]);
    ksort($grouped_items);
  }
//  drupal_set_message('<pre>dsfg'.print_r($grouped_items,TRUE).'</pre>');
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
      // also span all days events
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
//  $vars['first_column_width'] = 22;
  // add small javascript to extend datebox to whole td height
  drupal_add_js(
               "Drupal.behaviors.myEventBehavior = function(context){
      $('.agreservations-calendar td.agreservations-agenda-items div.agreservations div.view-item').each( function(){
        // Just a dirty fix. I have no idea how to properly calculate weight for this element
        var width = Math.max(($(this).parents('td').width())-5, $(this).width());
        $(this).width(width);
        });
    };",
               "inline");
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
function template_preprocess_agres_availability_day_old(&$vars) {
  //drupal_set_message('<pre>dayday '.print_r( $vars['rows'],true).'</pre>');
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
    drupal_add_css(drupal_get_path('module', 'agres_availability')  . '/css/agres_availability-overlap.css');
    if (empty($view->live_preview)) {
      drupal_add_js(drupal_get_path('module', 'agres_availability') . '/js/agres_availability_overlap.js');
    }
  }

  // If we're not grouping by time, move all items into the 'all day' array.
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

  // Move all_day items into the right columns and render them.
//  $grouped_items = array();
//  foreach ($rows['all_day'] as $item) {
//    if (isset($item->{$grouping_field})) {
//      $column = $item->{$grouping_field};
//      $item->{$grouping_field} = ''; // Remove the grouping field from the results.
//      if (!in_array($column, $columns)) {
//        $columns[] = $column;
//      }
//    }
//    else {
//      $column = t('Items');
//    }
//    $grouped_items[$column][] = theme('agres_availability_item', array('view' => $view, 'rendered_fields' => $item->rendered_fields, 'item' => $item));
//    $item_count++;
//  }
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
      $time = date_format($item->agres_availability_start_date, 'H:i:s');
      if (isset($item->{$grouping_field})) {
        $column = $item->{$grouping_field};
        $item->{$grouping_field} = ''; // Remove the grouping field from the results.
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
        $time_end = date_format($item->agres_availability_end_date, 'H:i:s');
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
    $heading = theme('agres_availability_time_row_heading', $variables);
    $grouped_items[$start_time]['hour'] = $heading['hour'];
    $grouped_items[$start_time]['ampm'] = $heading['ampm'];
    foreach ($grouped_items[$start_time]['values'] as $column => &$items) {
      foreach ($items as $index => $item) {
        if ($display_overlap) {
          $offset = round(date_format($item->agres_availability_start_date, 'i') / 15);
          $end = new DateObject('1970-01-01 ' . $item->time_end);
          $start = new DateObject('1970-01-01 ' . $item->time_start);
          $duration = $end->difference($start);
          $duration = round($duration / (15 * 60));
          $item->class = 'd_' . $duration . ' o_' . $offset . ' i_' . $item->indent . ' md_' . min($item->max_depth, 5);
        }
        $grouped_items[$start_time]['values'][$column][$index] = theme('agres_availability_item', array('view' => $view, 'rendered_fields' => $item->rendered_fields, 'item' => $item));
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

  $vars['agenda_hour_class'] = 'agres_availability-agenda-hour';
  $first_column_width = 10;

  if (empty($view->date_info->style_groupby_times)) {
    $vars['agenda_hour_class'] .= ' agres_availability-agenda-no-hours';
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
 * Display a week overlap view.
 */
//function template_preprocess_agres_availability_week_overlap(&$vars) {
//  template_preprocess_agres_availability_week($vars);
//}

/**
 * Display a week view.
 */
function template_preprocess_agres_availability_week(&$vars) {
//   Add in all the $vars added by the main calendar preprocessor.
  if ($vars['view']->name == 'agres_view' ) {
    $vars['view']->style_with_weekno = FALSE;
//       template_preprocess_calendar($vars);
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

//        $units = _agreservations_get_unitnodes($view->args[2]);
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
    //remove the args from agrescurrentpath:
    $toremovefrompath = implode("/", $view->args);
    $agrescurrentpath = get_current_path();
//       drupal_set_message('<pre>$agrescurrentpath'.print_r($agrescurrentpath,true).'</pre>');
    $agrescurrentpath = str_replace($toremovefrompath, "", $agrescurrentpath);
    //cut off trailing slash:
    if (substr($agrescurrentpath, -1, 1) == "/") {
      $agrescurrentpath = substr($agrescurrentpath, 0, strlen($agrescurrentpath) - 1);
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
    //initialize vars teststringggg:
//       drupal_set_message('<pre>***'. print_r($rows[0], TRUE) .'</pre>');
    $vars['rinfo'] = array();
    foreach ($rows[0] as $weekno => $row) {
//          drupal_set_message('<pre>weeeeek '.print_r(  $row,true).'</pre>');
      $vars['rows'][$weekno] = $row['data'];
      foreach ($row['data']['items'] as $ikey => $item) {
//            drupal_set_message('<pre>weeeeek '.print_r(  $item,true).'</pre>');
        ksort($item);
        foreach ($item as $skey => $singlereservation) {
//               $date = new DateObject($value, date_default_timezone(), DATE_ISO_FORMAT);
//               new DateOb
          $date = new DateObject($singlereservation->field['value'], $singlereservation->field['timezone_db'], NULL);
//               $date->setTimezone($singlereservation->field['timezone']);
          $date->setTimezone(timezone_open(date_default_timezone()));

          $tmpunititems = array();
          $tmpunititems = _agreservations_get_unitn_for_reservation_node($singlereservation->nid); //A.Gill: Räume je reservierung holen
          foreach ($units as $unit) {

            if (in_array($unit->nid, $tmpunititems)) { //A.Gill checks if unit is a booked item:
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
//                   $row['data']['all_day'][$unit->title] = $singlereservation;
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
//             drupal_set_message('<pre>***unit:dddd'. print_r($singlereservation, TRUE) .'</pre>');
        $tmpunititems = array();
        $tmpunititems = _agreservations_get_unitn_for_reservation_node($singlereservation->nid); //A.Gill: Räume je reservierung holen

        foreach ($units as $unit) {
          if (in_array($unit->nid, $tmpunititems)) { //
            $row['data']['all_day'][$unit->title] = $singlereservation;
          }
        }
      }
//        drupal_set_message('<pre>gdgdg***'. print_r($row['data']['all_day'], TRUE) .'</pre>');
      $columns[] = $weekno;
      foreach ($units as $unit) {
        //initialize $grouped_items:
        $grouped_items[$unit->title] = array();
        $start_times = $view->date_info->style_groupby_times;
        $start_time = array_shift($start_times);
        $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;
        ksort($row['data']['all_day']);
        foreach ($row['data']['all_day'] as   $key => $item) { //
//             $test = node_load($item->nid);
//             drupal_set_message('<pre>gdgdg***'. print_r($test, TRUE) .'</pre>');
          $tmpEnd = date_create($item->row->field_field_agres_rdate[0]['raw']['value2']);
          $tmpGranularityEnd = date_create(date_format($view->date_info->max_date, DATE_FORMAT_DATETIME));
          if ($tmpEnd > $tmpGranularityEnd) {
            $continues = 1;

          }
          else {
            $continues = 0;
          }
//                drupal_set_message('<pre>*$continues = 0;**'. print_r($item->row, TRUE) .'</pre>');
          $tmpstart = date_create($item->row->field_field_agres_rdate[0]['raw']['value']);
          $tmpGranularitystart = date_create(date_format($view->date_info->min_date, DATE_FORMAT_DATETIME));
          if ($tmpstart < $tmpGranularitystart) {
            $continuation = 1;
          }
          else {
            $continuation = 0;
          }
          $itemclass = 'itemwithoutorder';
          if (!isset($item->node->field_agres_orderid[$item->node->language])) {
            $itemclass = 'itemwithoutorder'; //treat as receptionist manual reservation defaultcolor orange
          }
          else {
            $itemclass = 'itemwithorder'; //treat as receptionist manual reservation defaultcolor blue
          }

          $themeparams = array(
            'view' => $view,
            'rendered_fields' => $item->rendered_fields,
            'item' => $item,
            'continues' => $continues,
            'continuation' => $continuation,
            'itemclass' => $itemclass,
          );
          $tmpunititems = _agreservations_get_unitn_for_reservation_node($item->nid); //get units per reservation
          if ($unit->title === $key) { //A.Gill unit is in booked item:    if (in_array($unit->nid, $tmpunititems)){//
            if ((!isset($vars['rinfo'][$unit->title][$item->nid]))) { //(!isset($vars['rows'][$weekno]['night'][$unit->title][$item->vid])){
              $vars['rows'][$weekno]['night'][$unit->title][$item->nid] = theme('agres_availability_item', $themeparams); // theme($theme, $variables);
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
            // slots if the option to show empty times was chosen.($show_empty_times) &&
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

//            agag   $theme = isset($item->calendar_node_theme) ? $item->calendar_node_theme : 'calendar_' . $view->date_info->granularity . '_node';
            $tmpunititems = array();
            $tmpunititems = _agreservations_get_unitn_for_reservation_node($item->nid); //get units per reservation
            if (in_array($unit->nid, $tmpunititems)) {
//             agag    $grouped_items[$unit->title][$start_time]['values'][$weekno][$item->nid] = theme($theme, $item, $view);
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
      // end foreach ($units as $unit) {
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
  else { //normal calendar
    $vars['template_file'] = 'agreservations-calendar-week-1';
//       template_preprocess_calendar($vars);
//       template_preprocess_calendar_week($vars);
    return;
  }
}

/**
 * Create the agres_availability date box.
 */
function template_preprocess_agres_availability_datebox(&$vars) {
  $date = $vars['date'];
  $view = $vars['view'];
  $vars['day'] = intval(substr($date, 8, 2));
  $force_view_url = !empty($view->date_info->block) ? TRUE : FALSE;
  $month_path = agres_availability_granularity_path($view, 'month');
  $year_path = agres_availability_granularity_path($view, 'year');
  $day_path = agres_availability_granularity_path($view, 'day');
  $vars['url'] = str_replace(array($month_path, $year_path), $day_path, date_pager_url($view, NULL, $date, $force_view_url));
  $vars['link'] = $vars['day'];//!empty($day_path) ? l($vars['day'], $vars['url']) : $vars['day'];
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
//  drupal_set_message('<pre>ifrekt. spaninfo '.print_r( $vars['day'],true).'</pre>');
}

/**
 * Format an agres_availability month node for display.
 */
function template_preprocess_agres_availability_month_multiple_node(&$vars) {
//    drupal_set_message('<pre>template_preprocess_agres_availability_month_multiple_node '.print_r( $vars['rows'],true).'</pre>');
  $view = $vars['view'];
  $curday = $vars['curday'];

  // get the year month and date
  $parts = explode('-', substr($curday, 0, 10));
  $year = $parts[0];
  $month = intval($parts[1]);
  $day = intval($parts[2]);

  // create the link to the day
  $vars['link'] = date_pager_url($view, NULL, date_pad($year, 4) . '-' . date_pad($month) . '-' . date_pad($day));
}

/**
 * Format an agres_availability week node for display.
 */
function template_preprocess_agres_availability_week_multiple_node(&$vars) {
  $view = $vars['view'];
  $curday = $vars['curday'];

  // get the year month and date
  $parts = explode('-', substr($curday, 0, 10));
  $year = $parts[0];
  $month = intval($parts[1]);
  $day = intval($parts[2]);

  // create the link to the day
  $vars['link'] = date_pager_url($view, NULL, date_pad($year, 4) . '-' . date_pad($month) . '-' . date_pad($day));
}

/**
 * Theme function for rendering views fields as a agres_availability 'item'.
 *
 * $vars['rendered_fields'] = An array of the rendered display of each field in the View.
 * $vars['item'] = The source data for this item.
 * $vars['view'] = The view that this item is displayed on.
 *
 * @TODO We need some options about how to combine rendered fields.
 * Fields rendered in multiday containers need to be inline.
 */
function template_preprocess_agres_availability_item(&$vars) {
//    $item->
  //drupal_set_message('<pre>template_preprocess_agres_availability_item '.print_r( $vars,true).'</pre>');
}

/**
 * Format the time row headings in the week and day view.
 */
function theme_agres_availability_time_row_heading($vars) {
  $start_time = $vars['start_time'];
  $next_start_time = $vars['next_start_time'];
  $curday_date = $vars['curday_date'];
  static $format_hour, $format_ampm;
  if (empty($format_hour)) {
//    $format = //variable_get('date_format_short', 'm/d/Y - H:i');
    $format =  'm/d/Y - H:i';
//    $format_hour = str_replace(array('a', 'A'), '', date_limit_format($format, array('hour', 'minute')));
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
function theme_agres_availability_stripe_legend() {
  if (empty($GLOBALS['agres_availability_stripes'])) {
    return '';
  }
  $header = array(
    array(
      'class' => 'agres_availability-legend',
      'data' => t('Item'),
    ),
    array(
      'class' => 'agres_availability-legend',
      'data' => t('Key'),
    ),
  );
  $rows = array();
  $output = '';
  foreach ((array) $GLOBALS['agres_availability_stripes'] as $label => $stripe) {
    if ($stripe) {
      $rows[] = array($label, '<div style="background-color:' . $stripe . ';color:' . $stripe . '" class="stripe" title="Key: ' . $label . '">&nbsp;</div>');
    }
  }
  if (!empty($rows)) {
    $variables = array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('mini', 'agres_availability-legend')),
    );
    $output .= theme('table', $variables);
    $output = '<div class="agres_availability legend">' . $output . '</div>';
  }
  return $output;
}

/**
 * Format item stripes
 */
function theme_agres_availability_stripe_stripe($vars) {
  $item = $vars['item'];
  if (empty($item->stripe) || (!count($item->stripe))) {
    return;
  }
  $output = '';
  if (is_array($item->stripe_label)) {
    foreach ($item->stripe_label as $k => $stripe_label) {
      if (!empty($item->stripe[$k]) && !empty($stripe_label)) {
        $GLOBALS['agres_availability_stripes'][$stripe_label] = $item->stripe[$k];
        $output .= '<div style="background-color:' . $item->stripe[$k] . ';color:' . $item->stripe[$k] . '" class="stripe" title="Key: ' . $item->stripe_label[$k] . '">&nbsp;</div>' . "\n";
      }
    }
  }
  return $output;
}

function template_preprocess_agres_availability_month_col(&$vars) {
//    $vars['agresss'][]=$vars['item'];
//        drupal_set_message('<pre>template_preprocess_agres_availability_month_col '.print_r( $vars,true).'</pre>');
//         $vars['item2']=$vars['item'];
}
/**
 * Format an empty day on a agres_availability
 *
 * @param day
 *   The day to display.
 */
function theme_agres_availability_empty_day(&$vars) {
//    drupal_set_message('<pre>weeeeek '.print_r($vars['view'],true).'</pre>');
  $curday = $vars['curday'];
//   $vars['agrcurdays'][]=$curday;
//  $vars['agrdays'][]=$curday;
  $view = $vars['view'];
//  drupal_set_message('<pre>weeeeek '.print_r($view->style_plugin->date_info->agres_availability_type,true).'</pre>');
  if ($view->style_plugin->date_info->agres_availability_type != 'day') {
    return '<div class="agres_availability-empty">&nbsp;</div>' . "\n";
  }
  else {
    return '<div class="agres_availability-dayview-empty">' . t('Empty day') . '</div>';
  }
}
