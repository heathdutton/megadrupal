<?php

/**
 * @file
 * Reservations API
 */


/**
 * Validates the reservablility of a items selected
 *
 * @param $node
 *   The item node.
 */
function reservations_validate_reservations_selected_items($form, &$form_state) {
  //dsm('reservations_validate_reservations_selected_items');
  $node = (object) $form_state['values'];

  $langcode = $node->language;
  // ****
  // Build date objects we'll need for our different validations.
  // ****
  $start              = $node->field_reservations_date[$langcode][0]['value'];
  $end                = $node->field_reservations_date[$langcode][0]['value2'];
  
  // IS THIS RIGHT?  OR DOUBLE CONVERTING
  // IT'S USING THE FIELD VALUE
  $backtoback = variable_get('reservations_back_to_back_allowed', 0);
  if ($backtoback) {
    $end = date('Y-m-d H:i:s', strtotime($end) - 1);
  }
  
  if ($backtoback) {
    $start = date('Y-m-d H:i:s', strtotime($start) + 1);
  } 
  
  //dsm($start);
  //dsm($end);

  $messages = array();
  foreach ($node->reservations_reservation_items as $did => $item) {

    if (empty($item['reservations_item_nid'])) {
      continue;
    }
    // Bucket choice?
    if (! is_numeric($item['reservations_item_nid'])) {
      $item['type']           = $item['reservations_item_nid'];
      $item['reservations_item_nid'] = 0;
    }

    // Get the title of the item.
    // Also get the content type for new resource items.
    if (!isset($item['item_title']) or ! isset($item['type'])) {
      $new_item = node_load($item['reservations_item_nid']);
      if ($new_item) {
        $item['item_title'] = $new_item->title;
        $item['type']       = $new_item->type;
      }
      else {
        //TODO: should not be doing theming here.
        $content_settings   = reservations_load_item_settings($item['type']);
        $item['item_title'] = $content_settings->type_name;
      }
    }

    $messages[$did] = '';

    if (isset($item['type'])) {
      $type     = $item['type'];
      $item_nid = $item['reservations_item_nid'];
      $title    = $item['item_title'];


      // Is this content type active?
      $content_settings   = reservations_load_item_settings($type);
      if ($content_settings->reservations_active_status != RESERVATIONS_STATUS_ACTIVE) {
        $messages[$did] = '<div> ' . t("%name is not active.", array('%name' => $title)) . '</div>';
        form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
        continue;
      }

      // Does the user have access to manage reservations or this content type?
      if (!user_access('manage reservations') && !reservations_check_content_type_user_permissions($type)) {
        $messages[$did] = '<div> ' . t("You do not have permission to reserve %name.", array('%name' => $title)) . '</div>';
        form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
        continue;
      }

      // Did the user select too many of the same bucket item?
      if (reservations_type_setting($type) == 'bucket') {
        $selected_count[$type] = isset($selected_count[$type]) ? $selected_count[$type] + 1 : 1;
        if ( ! isset($inventory_count[$type])) {
          $inventory_count[$type]       = reservations_get_available_bucket_count($type);
        }
        
        if ($selected_count[$type] > $inventory_count[$type]) {
          $messages[$did] = '<div> ' . t("You've selected too many %name's.  We only have %amount in the current inventory.", array('%name' => $title, '%amount' => $inventory_count[$type])) . '</div>';
          form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
          continue;
        }
      }

      // Did the user select too many of the same item?
      if ($item_nid) {
        if (isset($selected_count[$item_nid])) {
          $inventory_count = 1;
          $messages[$did] = '<div> ' . t("You've selected too many %name's.  We only have %amount in the current inventory.", array('%name' => $title, '%amount' => $inventory_count)) . '</div>';
          form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
          continue;
        }
        else {
          $selected_count[$item_nid] = 1;
        }
      }

      // Is it available?

      // Are we checking an existing item?
      $vid = isset($item['did']) ? $node->vid : NULL;

      $count = reservations_is_item_reservable($item_nid, $type, $start, $end, $vid);
      if (!$count) {
        $messages[$did] = reservations_theme_conflict_grid($type, $title, $start, $end, $item_nid, $node->vid);
        form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
        continue;
      }

      // Is it overdue?
      $overdue_items_array = reservations_overdue_items($type, $start, $node->nid);
      
 	  
 	  if (reservations_type_setting($type) == 'bucket') {
         $items_overdue_currently = count($overdue_items_array);
         $available_bucket_items_count = $inventory_count[$type] - $items_overdue_currently;
         if ($available_bucket_items_count < 1) {
           $messages[$did] = '<div> ' . t("%name is not available because all items are currently overdue", array('%name' => $title)) . '</div>';
         
           foreach ($overdue_items_array as $reservations) {
             foreach (array_keys($reservations) as $nid) {
               $overdue = node_load($nid);
               // compare w/ overdue window
               $messages[$did] .= '<div> ' . l($overdue->title, 'node/' . $overdue->nid) . '</div>';
             }
           form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
           continue;
           }      
         }
       }else{
         if (!empty($overdue_items_array)) {
           $messages[$did] = '<div> ' . t("%name is not available because it is still checked out by:", array('%name' => $title)) . '</div>';
           foreach ($overdue_items_array as $reservations) {
             foreach (array_keys($reservations) as $nid) {
               $overdue = node_load($nid);
               $messages[$did] .= '<div> ' . l($overdue->title, 'node/' . $overdue->nid) . '</div>';
             }
            }
           form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
           continue;
          }
       }
 
       $count = reservations_is_item_reservable($item_nid, $type, $start, $end, $vid);
 
       if (!$count) {
         $messages[$did] = reservations_theme_conflict_grid($type, $title, $start, $end, $item_nid, $node->vid);
         //$messages[$did] = 'testing';
          form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
          continue;
        }
       

      // if (!empty($overdue_items_array)) {
//         $messages[$did] = '<div> ' . t("%name is not available because it is still checked out by:", array('%name' => $title)) . '</div>';
//         foreach ($overdue_items_array as $reservations) {
//           foreach (array_keys($reservations) as $nid) {
//             $overdue = node_load($nid);
//             $messages[$did] .= '<div> ' . l($overdue->title, 'node/' . $overdue->nid) . '</div>';
//           }
//         }
//         form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
//         continue;
//       }


      // Check item restrictions.  max hours, etc.
      $restrictions = reservations_check_content_type_restrictions($type, $start, $end);

      if (!empty($restrictions)) {
        foreach ($restrictions as $restriction) {
          $messages[$did] .= '<div>' . t($restriction, array('%name' => $title)) . '</div>';
        }
        form_set_error("reservations_reservation_items][$did][reservations_item_nid", $messages[$did]);
        continue;
      }

    }
  }
  return $messages;
}

/**
 * Validates the dates against hours of operation, max advance, closed dates
 *
 * @param $node
 *   The item node.
 */
function reservations_validate_reservations_reservation_date($form, &$form_state) {
  //dsm('reservations_validate_reservations_reservation_date');
  $node = (object) $form_state['values'];

  $langcode = $form_state['node']->language;
  $backtoback = variable_get('reservations_back_to_back_allowed', 0);
  // Build date objects we'll need for our different validations.
  $start              = $node->field_reservations_date[$langcode][0]['value'];
  $end                = $node->field_reservations_date[$langcode][0]['value2'];
  $start_object       = reservations_create_local_date_object($start, 'start', $backtoback);
  $end_object         = reservations_create_local_date_object($end, 'end', $backtoback);
  $start_day_of_week  = (int) date_format($start_object, 'w');
  $end_day_of_week    = (int) date_format($end_object, 'w');
  $start_month_day    = date_format($start_object, 'm-d');
  $end_month_day      = date_format($end_object, 'm-d');

  // Hours of operation restrictions, max days, and closed dates checks
  $hours_of_operation = reservations_load_hours_of_operation();
    
  if (empty($hours_of_operation)) {
    form_set_error('Reservations', 'You cannot make a Reservation until the Hours of Operation have been configured in the Reservation admin settings.');
    return;
  }
  
  $start_hours        = $hours_of_operation[$start_day_of_week];
  $end_hours          = $hours_of_operation[$end_day_of_week];
  $start_date         = date_format($start_object, 'm-d-Y');
  $max_days           = variable_get("reservations_max_days_advance_reservation", '0');
  
  // Users in role with Administer Reservations permssion or outside hours of operation skip these checks
  if (user_access('create reservations outside hours of operation')) {

    reservations_verbose_logging('SKIP Hours of Operation Check, Max Days Check, and Closed Dates Check because user has create reservations outside hours of operation permission');

    //check to see if warning should be displayed
    if (!empty($start) && !empty($end)) {
      if (strtotime(date('G:i', strtotime($start . ' UTC'))) < strtotime($start_hours['open']) || strtotime($start_hours['close']) < strtotime(date('G:i', strtotime($end . ' UTC')))) {
        drupal_set_message(t('You are making a Reservation outside the normal hours of operation. This may impact access to the items you are reserving.'), 'warning');
      }
    }
  }
  else {
    // Reservation start date cannot exceed the max advance
    reservations_verbose_logging('CHECKING Max Days');
    if ($max_days) {
      $max_date = new DateTime("+$max_days day");
      //$max_date = date('m-d-Y', mktime(0, 0, 0, date("m"), date("d")+$max_days, date("Y")));
      if ($start_object > $max_date) {
        form_set_error('field_reservations_date][0][value][date', t('You cannot make a Reservation more than %days days in advance. Start the Reservation before %date.', array('%days' => $max_days, '%date' => date_format($max_date, 'm-d-Y'))));
      }
    }

    // Can't start or end a reservation on days that are
    // closed dates.
    reservations_verbose_logging('CHECKING Closed Dates');
    if (in_array($start_month_day, $hours_of_operation['closed_days'])) {
      $name = date_format($start_object, 'F jS');
      form_set_error('field_reservations_date][0][value][date', t('Sorry, but we are closed on %day for a holiday or special event.', array('%day' => $name)));
    }
    if (in_array($end_month_day, $hours_of_operation['closed_days'])) {
      $name = date_format($end_object, 'F jS');
      form_set_error('field_reservations_date][0][value2][date', t('Sorry, but we are closed on %day for a holiday or special event.', array('%day' => $name)));
    }

    // Can't start or end a reservation on a day the facility
    // has no hours of operation, or outside hours of operation.
    reservations_verbose_logging('CHECKING Hours of Operation');
    $start_name = date_format($start_object, 'l');
    if (!$hours_of_operation[$start_day_of_week]) {
      form_set_error('field_reservations_date][0][value][date', t('Reservations cannot start on a %day.', array('%day' => $start_name)));
    }
    else {
      $start_time = date_format($start_object, 'H:i');
      if ($start_time < $start_hours['open']) {
        form_set_error('field_reservations_date][0][value][time', t('Reservations cannot start at a time before %start.',
          array('%start' => reservations_format_time($start_hours['open']))));
      }
      elseif ($start_time > $start_hours['close']) {
        form_set_error('field_reservations_date][0][value][time', t('Reservations cannot start at a time after %end.',
          array('%end' => reservations_format_time($start_hours['close']))));
      }
    }

    $end_name = date_format($end_object, 'l');
    if (!$hours_of_operation[$end_day_of_week]) {
      //check for push on end day
      if(user_access('push forward over closed days') && variable_get('reservations_' . strtolower($end_name) . '_push', 0)) {
        //check all equipment?
        
        
        //check the next 2 days
        $nextday = $end_object->add(new DateInterval('P1D'));
        $nextday_name = date_format($nextday, 'l');
        $nextday_number = ($end_day_of_week+1 == 8) ? 0 : ($end_day_of_week+1);
        
        if ($hours_of_operation[$nextday_number]) {
          
          $openinghour = $hours_of_operation[$nextday_number]['open'];
          $pushedduehour = strtotime($openinghour . ' + ' . variable_get('reservations_push_hours', '1') . ' hours'); 
          
          $openinghour_formatted = reservations_format_time($openinghour);
          $pushedduehour_formatted = reservations_format_time(date("H:i", $pushedduehour));
         
          form_set_error('field_reservations_date][0][value2][date', t('Reservations cannot end on %day.  The items you are reserving allow extending the normal maximum reservation hours by pushing the due date forward to %nextday and ending between %openhour and %pushedhour.', array('%day' => $end_name, '%nextday' => $nextday_name, '%openhour' => $openinghour_formatted, '%pushedhour' => $pushedduehour_formatted)));
        }
        
      } else {
        form_set_error('field_reservations_date][0][value2][date', t('Reservations cannot end on a %day.', array('%day' => $end_name)));
      }
    }
    else {
      $end_time = date_format($end_object, 'H:i');
      if ($end_time < $end_hours['open']) {
        form_set_error('field_reservations_date][0][value2][time', t('Reservations cannot end at a time before %start.',
          array('%start' => reservations_format_time($end_hours['open']))));
      }
      elseif ($end_time > $end_hours['close']) {

        form_set_error('field_reservations_date][0][value2][time', t('Reservations cannot end at a time after %end.',
          array('%end' => reservations_format_time($end_hours['close']))));
      }
    }
  } // Hours of operation restrictions, max days, and closed dates checks
}


function reservations_validate_status($form, &$form_state) {
  $node = (object) $form_state['values'];
  $langcode = $form_state['node']->language;
  // Reservations with a checked out status.
  if ($node->reservations_reservation_status == RESERVATIONS_STATUS_CHECKED_OUT) {
    // Make sure all existing bucket reservations have an item assigned.
    if (empty($node->reservations_reservation_items)) {
      form_set_error('reservations_reservation_status', t('You can not finalize a reservation that has no reserved items.'));
    }
    else {
      foreach ($node->reservations_reservation_items as $did => $item) {
        // if there is a placeholder, but an real reservabled node hasn't been selected
        if (isset($item['reservations_placeholder_nid']) && $item['reservations_item_nid'] == "0") {
          form_set_error("reservations_reservation_items][$did][reservations_item_nid", t("The reservation for %title must have an item associated with it for finalized reservations.", array('%title' => $item['name'])));
        }
        // Can't add a bucket item and finalize at the same time.
        if (!is_numeric($item['reservations_item_nid']) and drupal_strlen($item['reservations_item_nid'])) {
          form_set_error("reservations_reservation_items][$did][reservations_item_nid", t("You cannot finalize a reservation while adding a bucket item."));
        }
      }
    }
  }
  // Prevent status changes on reservations that have past.
  $current_status = $node->reservations_original_reservation_status;
  if ($current_status && $current_status != $node->reservations_reservation_status && time() > strtotime($node->field_reservations_date[$langcode][0]['value2']) && !in_array((int) $node->reservations_reservation_status, array(RESERVATIONS_STATUS_CANCELLED, RESERVATIONS_STATUS_CHECKED_IN, RESERVATIONS_STATUS_DENIED))) {
    $statuses = reservations_record_status();
    form_set_error('reservations_reservation_status', t('You cannot change the status to %status for a reservation that has past.', array('%status' => $statuses[$node->reservations_reservation_status])));
  }
}

/**
 * Validates the state change of a reservable item.
 *
 * @param $node
 *   The item node.
 */
function reservations_validate_default_availability($element, &$form_state) {

  // Only perform the check if the item is set to an unavailable state.
  if (in_array((int) $element['#value'], array(RESERVATIONS_UNA_F, RESERVATIONS_UNA_S))) {

    $bad_reservations = reservations_incomplete_reservations_for_item_nid($form_state['values']['nid']);
    if (!empty($bad_reservations)) {
      $output = '<ul>';
      foreach ($bad_reservations as $node) {
        $output .= '<li>' . $node . '</li>';
      }
      $output .= '</ul>';
      form_set_error('reservations_default_availability', t('%title can not be set to an unavailable status until it is removed from the following reservations:', array('%title' => $form_state['values']['title'])) . $output);

    }
  }
}
/**
 * Validation for numeric textfields.
 */
function reservations_is_numeric_validate($form) {
  if ($form['#value'] && !is_numeric($form['#value'])) {
    form_set_error($form['#name'], t('%title must be a number.', array('%title' => $form['#title'])));
  }
}
/**
 * Custom validation function to protect reservations nodes from mass deletion.
 */
function reservations_node_admin_delete_validate($form, &$form_state) {

  // Look only for delete op.
  $operation = $form_state['values']['operation'];
  if ($operation != 'delete') {
    return;
  }
  // Get the checked nodes.
  $nids = array_filter($form_state['values']['nodes']);

  // Perform the check for each submitted node.
  foreach ($nids as $nid) {
    $node = node_load($nid);

    // Check to see if any of the nodes should not be deleted.
    if (!reservations_delete_item_validate($node)) {
      // If so, then unset the checked node so it will not be processed, and display a warning.
      // Note that the array element has to be completely removed here in order to prevent the
      // node from being deleted, due to the nature of the mass deletion callback.
      unset($form_state['values']['nodes'][$nid]);
      unset($nids[$nid]);
    }
  }

  // If we've unset all of the nodes that were checked, then don't continue with the form processing.
  if (!count($nids)) {
    drupal_set_message(t('No nodes selected.'), 'error');
    drupal_goto('admin/content/node');
  }
}


function reservations_validate_empty_reservation_items($form, &$form_state) {

  $node = (object) $form_state['values'];

  $choices = $node->reservations_reservation_items;

  $unselected = 0;

  foreach ($choices as $did => $item) {
    if (is_array($item)) {
      $value = $item['reservations_item_nid'];
    }
    else {
      $value = $item;
    }

    if (is_numeric($did) and !$value) {
      $value = $item['type'];
    }

    if (!$value) {
      $unselected++;
    }
  }

  if ($unselected == count($choices)) {
    $first = array_shift(array_keys($choices));
    if (isset($form_state['clicked_button']) && $form_state['clicked_button']['#value'] != 'Preview') {
      form_set_error("reservations_reservation_items][$first][reservations_item_nid", t("You cannot create a reservation without any items selected."));
    }
  }
}

/**
 * Submit handler for saving Reservations node type data.
 */
function reservations_node_type_save_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save content type')) {
    $settings = $form_state['values'];
    //$existing = db_result(db_query("SELECT type FROM {reservations_node_type} WHERE type = '%s'", $settings['type']));
    $existing = reservations_node_type_existing($settings['type']);
    if (!$existing) {
      $return = drupal_write_record('reservations_node_type', $settings);
    }
    else {
      $return = drupal_write_record('reservations_node_type', $settings, 'type');
    }
    cache_clear_all('reservations_' . $settings['type'] . '_data', 'cache');
    cache_clear_all('reservations_content_type_info', 'cache');
    // This hack is necessary because the node type form submit
    // automatically saves all remaining form items to {variable}
    // We're doing custom storage, so remove these.
    // Don't delete reservations_type_setting_

    $variables = array(
      'reservations_max_hours_per_reservation_',
      'reservations_allow_overnight_',
      'reservations_push_',
      'reservations_late_fee_per_hour_',
      'reservations_rate_per_hour_',
      'reservations_fee_free_hours_',
      'reservations_status_',
      'reservations_spare_items_',
      'reservations_min_cancel_hours_',
      'reservations_autocheckout_',
      'reservations_autocheckin_',
      'reservations_selfcheckout_',
    );
    foreach ($variables as $variable) {
      variable_del($variable . $settings['type']);
    }
  }

}

/**
 * Submit handler to add more choices to a reservation form. This handler is used when
 * javascript is not available. It makes changes to the form state and the
 * entire form is rebuilt during the page reload.
 */
function reservations_more_choices_submit($form, &$form_state) {
  // Set the form to rebuild and run submit handlers.
  node_form_submit_build_node($form, $form_state);

}

/**
 * Submit handler to date filter items on a reservation form.
 * It makes changes to the form state and the entire form is
 * rebuilt during the page reload.
 */
function reservations_date_filter($form, &$form_state) {
  // Set the form to rebuild and run submit handlers.
  node_form_submit_build_node($form, $form_state);
}

/**
 * writes additional info to log to aid in troubleshoot configuration
 */
function reservations_verbose_logging($string) {

  if (variable_get('reservations_verbose_logging', 0)) {
    watchdog('reservations', $string);
  }
}




/**
 * Checks for reservation restrictions for a content type.
 *
 * These include maximum hours per reservation, and if the bucket/resource
 * is reservable overnight and/or is configured to push forward.
 *
 * @param $content_type
 *   The content type to be checked.
 * @param $start
 *   The start date of the reservation in DATETIME format and UTC timezone.
 * @param $end
 *   The end date of the reservation in DATETIME format and UTC timezone.
 *
 * @return
 *   An array of warning messages for any restrictions found.
 */
function reservations_check_content_type_restrictions($content_type, $start, $end) {
  if (!user_access('manage reservations')) {

    //TODO I don't like this.
    $type_settings = reservations_load_item_settings($content_type);
    $return = array();
    
    $backtoback = variable_get('reservations_back_to_back_allowed', 0);
    // Convert start/end dates to local time.
    // TODO clean this up.
    $start_object = reservations_create_local_date_object($start, 'start', $backtoback);
    $end_object = reservations_create_local_date_object($end, 'end', $backtoback);

    // We want these timestamps generated in UTC.
    $old_timezone = date_default_timezone_get();
    date_default_timezone_set('UTC');
    $start_timestamp = strtotime($start);
    $end_timestamp = strtotime($end);
    date_default_timezone_set($old_timezone);

    $reserved_hours    = ($end_timestamp - $start_timestamp) / (60 * 60);
    $start_day_of_week = date_format($start_object, 'w');
    $end_day_of_week   = date_format($end_object, 'w');

    // Make sure max hours aren't exceeded.
    if ($type_settings->reservations_max_hours_per_reservation && $reserved_hours > $type_settings->reservations_max_hours_per_reservation) {

      // Override max_hours_per_reservation if we can pash this past the closed days
      
      // Validate allow_push on the types
      if (user_access('push forward over closed days') || $type_settings->reservations_push) {
        $closed_days = array();
        // Do we allow pushing this reservation forward?
        if ($type_settings->reservations_push) {
          $i = 0;
          foreach (array('sunday', 'monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday') as $day) {
            if (variable_get('reservations_' . $day . '_push', 0)) {
              $closed_days[$i] = TRUE;
            }
            $i++;
          }
        }
        // Do we allow extending a reservtion over days we are closed?
        if (user_access('push forward over closed days')) {
          $hours_of_operation = reservations_load_hours_of_operation($content_type);
          for ($i = 1; $i <= 6; $i++) {
            if (empty($hours_of_operation[$i])) {
              $closed_days[$i] = TRUE;
            }
          }
        }

        // Only extend if the following day is closed/weekend.
        // TODO check that the end time is not the same day and within the hours of being open.
        if (!$closed_days[date('w', $start_timestamp + 86400)]) {
          $return[] = t('%name cannot be reserved for more than %hours hours.', array('%hours' => $type_settings->reservations_max_hours_per_reservation));
        }

        // Only extend the max time if the default max time falls on a weekend.
        if ($closed_days[date('w', $start_timestamp + $type_settings->reservations_max_hours_per_reservation * 60 * 60)]) {
          //Find the next day we are open.
          for ($i = 1; $i <= 6; $i++) {
            if (!$closed_days[date('w', $start_timestamp + $i * 86400)]) {

              // Does the end_day fall here?
              // TODO force time to be exactly when open.
              if ($end_day_of_week != date('w', $start_timestamp + $i * 86400)) {
                $return[] = t('%name cannot be reserved more then one day after a weekend.', array('%hours' => $type_settings->reservations_max_hours_per_reservation));
              }
              break;
            }
          }
        }
      }
      else {
        $return[] = t('%name cannot be reserved for more than %hours hours.', array('%hours' => $type_settings->reservations_max_hours_per_reservation));
      }
    }

    // Validate allow_overnight.
    if (!$type_settings->reservations_allow_overnight) {
      // Need the 48 hour check in case somebody starts and ends their
      // reservation on the same day.
      if ($start_day_of_week != $end_day_of_week || $reserved_hours > 48) {
        $return[] = t('%name cannot be reserved overnight.');
      }
    }
  }
  return (isset($return)) ? $return : NULL;
}
// reservations_check_content_type_restrictions

/**
 * Ensures the user has 'edit own [type] content' and 'delete own [type] content'
 * permissions, otherwise they are not allowed to reserve the content type.
 *
 * @return TRUE if the user has access to reserve the content type, FALSE
 *   otherwise.
 */
function reservations_check_content_type_user_permissions($type) {
  return user_access("edit own $type content") && user_access("delete own $type content");
}

function reservations_is_reservations_type($type) {
  return reservations_type_setting($type) != 'disabled';
}

function reservations_type_setting($type) {
  $return = variable_get('reservations_type_setting_' . $type, 'disabled');
  
  //double check this
  //if ($return == 'disabled') {  
    //$return = db_query("SELECT reservations_type_setting FROM {reservations_node_type} WHERE type = :type", array(':type' =>$type))->fetchField();
  //}

  return $return;
}

/**
 * Return a list of all reservations content types.
 *
 * @param $content_type_name
 *   If set, return information on just this type.
 *
 * Do some type checking and set up empty arrays for missing
 * info to avoid foreach errors elsewhere in the code.
 */
function reservations_content_types($type_name = NULL) {
  // handle type name with either an underscore or a dash
  $type_name = !empty($type_name) ? str_replace('-', '_', $type_name) : NULL;

  $info = _reservations_content_type_info();
  if (!isset($type_name)) {
    return $info;
  }
  else {
    return isset($info[$type_name]) ? $info[$type_name] : array();
  }
}

/**
 * Return the name of a type code.
 *
 * @param string|int $code
 *  if int, will return translated name of the code.
 *  if NULL, returns array of codes as keys, and translated strings as value
 *
 * @return string|int
 */
function reservations_item_status($code = NULL) {
  $statuses = array(
    RESERVATIONS_AVA_F => t('Available'),
    RESERVATIONS_UNA_F => t('Unavailable'),
    RESERVATIONS_AVA_T => t('Template Only'),
    RESERVATIONS_UNA_S => t('No Longer in Inventory'),
  );
  if (isset($code)) {
    return $statuses[$code];
  }
  else {
    return $statuses;
  }
}


function reservations_item_reservation_status($code = NULL) {
  // Item status for reservations.
  $statuses = array(
    RESERVATIONS_ITEM_STATUS_AVAILABLE => t('Available'),
    RESERVATIONS_ITEM_STATUS_CHECKED_OUT => t('Checked Out'),
  );
  if (isset($code)) {
    return $statuses[$code];
  }
  else {
    return $statuses;
  }
}
/**
 * Return the name of a status code.
 *
 * @param string|int $code
 *  if int, will return translated name of the code.
 *  if NULL, returns array of codes as keys, and translated strings as value
 *
 * @return string|int
 */
function reservations_record_status($code = NULL) {
  $types = array(
    RESERVATIONS_STATUS_UNCONFIRMED => t('Unconfirmed'),
    RESERVATIONS_STATUS_PENDING => t('Confirmed'),
    RESERVATIONS_STATUS_CHECKED_OUT => t('Checked out'),
    RESERVATIONS_STATUS_CHECKED_IN => t('Checked in'),
    RESERVATIONS_STATUS_CANCELLED => t('Cancelled'),
    RESERVATIONS_STATUS_DENIED => t('Denied'),
    RESERVATIONS_STATUS_NO_SHOW => t('No Show'),
  );
  if (isset($code)) {
    return $types[$code];
  }
  else {
    return $types;
  }
}


/**
 * Pulls items available to assign to a bucket for a reservation.
 *
 * @param $node
 *   The reservation node.
 * @param $bucket_type
 *   The bucket type.
 *
 * @return
 *   An array of available items, in select options format.
 */
function reservations_get_available_bucket_items($node, $bucket_type) {

  $langcode = $node->language;
  $date_info = $node->field_reservations_date[$langcode][0];
  $start     = $date_info['value'];
  $end       = $date_info['value2'];

  $options = reservations_get_reservable_items($bucket_type, $start, $end, $node->nid);

  return $options;
}

function reservations_get_suggested_bucket_item($content_type, $start, $end, $items = array()) {
  $total_items_array = reservations_reserved_bucket_items($content_type, $start, $end );

  foreach ($total_items_array as $item_nid => $node) {
    if (empty($total_items_array[$item_nid]) && !in_array($item_nid, $items)) {
      return $item_nid;
    }

  }

  return 0;
}

/**
 * Builds an array representing the hours of operation for the facility.
 *
 * @return
 *   An associative array with the following key/value pairs:
 *     [php_day_of_week_number_as_in_date_function] => An associative
 *       array with the following key/values pairs:
 *         'open'  => Opening time (military).
 *         'close' => Closing time (military).
 *     'closed_days' => An array of closed dates in mm-dd format.
 */
function reservations_load_hours_of_operation($content_type = '', $display = NULL) {
  $hours = variable_get('reservations_hours_operation', '');
  $adminhours = explode('-', variable_get('reservations_hours_admin', '07:00-22:00'));
  
  if ($display) {

    foreach($hours as $key => $value) {
	  if(!isset($hours[$key]['open'])) {
	    $hours[$key]['open'] =  $adminhours[0]; 
	  }
	  if(!isset($hours[$key]['close'])) {
	    $hours[$key]['close'] =  $adminhours[1]; 
	  }   
    }
  }
  return $hours;
}

function reservations_hours_str_to_array($str) {

  if (drupal_strlen($str) == 11) {
    $parts = explode('-', ($str));
    if (count($parts) == 2) {
      return array(
        'open' => $parts[0],
        'close' => $parts[1],
      );
    }
  }

  return FALSE;
}
// reservations_hours_str_to_array

/**
 * Creates a date object based on the site's local timezone.
 *
 * @param $datetime
 *   A date in DATETIME format, UTC timezone.
 *
 * @return
 *   A php date object in the site's timezone.
 */
function reservations_create_local_date_object($datetime, $time = NULL, $backtoback = NULL) {
  //dsm($time);
  
  if ($backtoback && $time == 'start') {
    $datetime = date('Y-m-d H:i:s', strtotime($datetime) + 1);
    //dsm('adujsting start');
  } 
  
  if ($backtoback && $time == 'end') {
    $datetime = date('Y-m-d H:i:s', strtotime($datetime) - 1);
    //dsm('adjusting end');
  } 

  $date_object = date_create($datetime, timezone_open('UTC'));
  $timezone = variable_get('date_default_timezone', 'UTC');
  date_timezone_set($date_object, timezone_open($timezone));

  return $date_object;
}

/**
 * Sort by vid
 *
 * @param $a
 *   The first object.
 * @param $b
 *   The second object
 *
 * @return
 *   0, 1, or -1 indicating which object has a higher VID
 */
function reservations_by_vid() {

  if ($a->vid == $b->vid) {

    return 0;

  }
  return ($a->vid > $b->vid) ? -1 : 1;
}
// reservations_by_vid

/**
 * Calculates the short hour/minute time format based on the site settings.
 */
function reservations_time_format() {
  static $time_only_format = NULL;
  if (empty($time_only_format)) {
    $short_date_format = variable_get('date_format_short', 'm/d/Y - H:i');
    $time_only_format = date_limit_format($short_date_format, array('hour', 'minute'));
  }
  return $time_only_format;
}

/**
 * Formats a time value into the site's preferred format.
 *
 * @param object $hours_minutes
 *   A string of the form 'H:MM' or 'HH:MM'
 *
 * @return
 *   A string in 12- or 24-hour format with no leading zero.
 */
function reservations_format_time($hours_minutes) {
  $return = date(reservations_time_format(), strtotime($hours_minutes));
  if ($return[0] == '0') {
    return drupal_substr($return, 1);
  }
  return $return;
}



/**
 * Callback function for updating Reservation status from VBO.
 */
function reservations_operations_update($nodes) {
  foreach ($nodes as $nid) {
    reservations_confirm_reservation($nid);
  }
}

/**
 * Callback function for updating Reservation status.
 */
function reservations_confirm_reservation($nid) {
  $node = node_load($nid);

  //only update if Reservations Status is Unconfirmed
  if ($node->reservations_reservation_status == RESERVATIONS_STATUS_UNCONFIRMED) {
    $node->reservations_reseravation_status = RESERVATIONS_STATUS_PENDING;
    node_save($node);
    return TRUE;
  }

}



// Loads the current settings for reservable item nodes.
/* If you just want the content type settings just pass only node->type.
 */
function reservations_load_item_settings($object) {

  // Allow either a node object or type name to be passed in.
  if (is_string($object)) {
    $type = $object;
  }
  else {
    $node = (array) $object;
    $type = $node['type'];
  }

  // Initialize here in case there is no nid.
  $item_settings = array();

  // Settings from the content type edit page.
  $content_settings = reservations_content_types($type);

  if (isset($node['nid'])) {
    // Settings common to all reservations item nodes.
    // resource or bucket.
    $reservations_type = isset($content_settings['reservations_type_setting']) ? $content_settings['reservations_type_setting'] : '';

    $vid = $node['vid'];

    $item_settings = reservations_reservation_item_node_settings($vid);
    switch ($reservations_type) {
      case 'bucket':
        if ($item_settings['reservations_sub_type'] == RESERVATIONS_SUB_TYPE_RESERVATION) {
          unset($item_settings['reservations_default_availability']);
          unset($item_settings['reservations_item_status']);
          $item_settings += reservations_bucket_node_settings($vid);
        }
        break;

      case 'resource':
        if ($item_settings) {
          $item_settings += reservations_resource_node_settings($vid);
        }
        else {
          $item_settings = reservations_resource_node_settings($vid);
        }
        break;
    }
  }
  if ($item_settings && $content_settings) {
    return (object) ($item_settings + $content_settings);
  }
  else {
    if ($content_settings) {
      return (object) ($content_settings);
    }
    else {
      return (object) ($item_settings);
    }
  }
}

/**
 * Adds items to reservation on creation/update.
 *
 * @param $node
 *   The reservation node.
 */
function reservations_add_reservation_items($node) {

  $member_total = 0;
  $comreservationsal_total = 0;

  $langcode = $node->language;
  $hours = round(strtotime($node->field_reservations_date[$langcode][0]['value2']) - strtotime($node->field_reservations_date[$langcode][0]['value'])) / 3600;

  $exempt_items = array();
  // Update existing items or add new ones.
  if (isset($node->reservations_reservation_items)) {
    foreach ($node->reservations_reservation_items as $did => $item) {

      if (empty($item['reservations_item_nid']) and ! isset($item['type'])) {
        continue;
      }

      // If we are copying a reservation.  I.e. via node_repeat
      // Also copy over the placeholder nodes.
      if (isset($item['reservations_placeholder_nid']) and $node->is_new) {
        $placeholder_node = node_load($item['reservations_placeholder_nid']);
        if ($placeholder_node) {
          $item['did'] = NULL;
          $placeholder_node->nid = NULL;
          $placeholder_node->vid = NULL;
          $placeholder_node = node_submit($placeholder_node);
          node_save($placeholder_node);
          $item['reservations_placeholder_nid'] = $placeholder_node->nid;
        }
      }
      // Create a placeholder node if we don't have one yet.
      if (! isset($item['reservations_placeholder_nid']) or empty($item['reservations_placeholder_nid'])) {
        // Resource.
        if (is_numeric($item['reservations_item_nid'])) {
          $item_node        = node_load($item['reservations_item_nid']);
          $item['type']        = $item_node->type;
          $item['item_title']       = $item_node->title;
          $settings = $item_node;
        }
        // Bucket.
        elseif ($item['reservations_item_nid']) {

          $item['type']        = $item['reservations_item_nid'];
          $settings = reservations_load_item_settings($item['type']);
          $item['name']  = $settings->type_name;
          if ($settings->reservations_auto_assign_bucket_item) {
            $date_info = $node->field_reservations_date[$langcode][0];
            $start     = $date_info['value'];
            $end       = $date_info['value2'];

            $item['reservations_item_nid'] = reservations_get_suggested_bucket_item($item['type'], $start, $end, $exempt_items);
            $exempt_items[] = $item['reservations_item_nid'];
            $item_node        = node_load($item['reservations_item_nid']);
            $item['item_title'] = $item_node->title;
            $settings = $item_node;
          }
        }
        else {
          break;
        }
        $title = isset($item['item_title']) ? $item['item_title']    : $item['name'];

        // Build the item's placeholder node.
        $reservation = new stdClass();
        $reservation->type = $item['type'];
        $reservation->name = $node->name;
        $reservation->uid = $node->uid;
        $reservation->title = "$title " . t('(Reservation)');
        $reservation->body = '';
        $reservation->status = 0;
        $reservation->promote = 0;
        $reservation->sticky = 0;

        // Reservations specific data.
        $reservation->reservations_default_availability = RESERVATIONS_AVA_F;
        $reservation->reservations_sub_type = RESERVATIONS_SUB_TYPE_RESERVATION;

        // Use the item specific accounting data if an item is assigned,
        // otherwise fall back to the content type defaults.
        // TODO move to nodeapi insert and update ops.

        $rate = (isset($settings->reservations_rate_per_hour)) ? $settings->reservations_rate_per_hour : 0;
        $comreservationsal_total = $comreservationsal_total + ($hours * $rate);
        $reservation->reservations_late_fee_per_hour = (isset($settings->reservations_late_fee_per_hour)) ? $settings->reservations_late_fee_per_hour : 0;
        $reservation->reservations_rate_per_hour = (isset($settings->rate_per_hour)) ? $settings->rate_per_hour : 0;
        $reservation->reservations_fee_free_hours = (isset($settings->fee_free_hours)) ? $settings->fee_free_hours : 0;
        $reservation->reservations_rate_per_hour = (isset($settings->reservations_rate_per_hour)) ? $settings->reservations_rate_per_hour : 0;
        $reservation->reservations_autocheckin  = (isset($settings->reservations_autocheckin))  ? $settings->reservations_autocheckin  : 0;
        $reservation->reservations_autocheckout = (isset($settings->reservations_autocheckout)) ? $settings->reservations_autocheckout : 0;
        $reservation = node_submit($reservation);
        node_save($reservation);

        $item['reservations_placeholder_nid'] = $reservation->nid;
      }

      // Update the state of all items with associations.
      switch ((int) $node->reservations_reservation_status) {
        case RESERVATIONS_STATUS_UNCONFIRMED:
        case RESERVATIONS_STATUS_PENDING:
          $item_status = RESERVATIONS_ITEM_STATUS_RESERVED;
          break;

        case RESERVATIONS_STATUS_CHECKED_OUT:
          $item_status = RESERVATIONS_ITEM_STATUS_CHECKED_OUT;
          break;

        case RESERVATIONS_STATUS_CHECKED_IN:
          $item_status = RESERVATIONS_ITEM_STATUS_CHECKED_IN;
          break;

        case RESERVATIONS_STATUS_CANCELLED:
          $item_status = RESERVATIONS_ITEM_STATUS_CANCELED;
          break;
        case RESERVATIONS_STATUS_DENIED:
          $item_status = RESERVATIONS_ITEM_STATUS_AVAILABLE;
          break;
      }

      // If we have an item assigned.  Set status to reserved.
      $item['reservations_item_status'] = $item_status;

      if ($node->revision or is_string($did) or $node->is_new) {
        unset($item['did']);  //striped so revisions get new did
        $item['nid'] = $node->nid;
        $item['vid'] = $node->vid;
        drupal_write_record('reservations_reservation_detail', $item);
      }
      else {
        $item['did'] = $did;
        drupal_write_record('reservations_reservation_detail', $item, 'did');
      }

      $item_status = $item_status == RESERVATIONS_ITEM_STATUS_CHECKED_OUT ? RESERVATIONS_ITEM_STATUS_CHECKED_OUT : RESERVATIONS_ITEM_STATUS_AVAILABLE;
      if (is_numeric($item['reservations_item_nid']) and $item['reservations_item_nid'] > 0) {
        $update = array();
        $update['nid'] = $item['reservations_item_nid'];
        $update['reservations_item_status'] = $item_status;
        drupal_write_record('reservations_reservation_item_node', $update, 'nid');
      }
    }
  }
}