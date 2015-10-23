<?php
/*
 * @file hotel_booking.theme.inc
 * Provides theme functions for hotel_booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */
/**
 * Default implementation of theme_hotel_booking_room_description().
 */
function theme_hotel_booking_room_cart_description($vars) {
  $teaser = $vars['teaser'];
  $data = $vars['data'];
  $content = '<div class="item-list">';
  if ($teaser && variable_get('hotel_booking_teaser_in_cart', FALSE)) {
    $content .= $teaser .'<br />';
  }
  $content .= theme('hotel_booking_night_list', array(
    'nights' => $data['nights'],
    'prices' => $data['prices'],
    'adults' => $data['adults'],
    'children' => $data['children']));
  return $content;
}

/**
 * Default implementation of theme_hotel_booking_quantity().
 */
function theme_hotel_booking_quantity($vars) {
  $data = isset($vars['data']) ? $vars['data'] : array();
  $output = 1;

  // The commented code that follows will set the quantity to the number of adults & children.
  // This will lead to confusion between the cart block & checkout pages. Quantities should default to one.
  // Otherwise people will get to the checkout page and see a different quantity and think it means they are booking
  // Multiple rooms, rather than multiple nights for multiple people in a single room.

  /*
  $adults = $data['adults'];
  $children = $data['children'];
  $output = '';
  if ($adults) {
    $output .= t('Adults').':&nbsp;'.$adults;
  }
  if ($children) {
    if ($output) $output .= '<br />';
    $output .= t('Children').':&nbsp;'.$children;
  } */

  return $output;
}


/**
 * Default implementation of theme_hotel_booking_night_list().
 *
 * html argument is for order pane product list which does a check plain
 */
function theme_hotel_booking_night_list($vars) {
  $nights = $vars['nights'];
  $price = $vars['prices'];
  $adults = $vars['adults'];
  $children = isset($vars['children']) ? $vars['children'] : 0;
  $html = isset($vars['html']) ? $vars['html'] : TRUE;
  $review = isset($vars['review']) ? $vars['review'] : FALSE;
  if ($adults) {
    $occupants = $adults .' Adult';
    if ($adults > 1) {
      $occupants .= 's';
    }
  }
  if ($children) {
    $occupants .= ', '. $children .' Child';
    if ($children > 1) {
      $occupants .= 'ren';
    }
  }
  if ($adults || $children) {
    $occupants .= '<br />';
  }
  if (count($nights)) {
    $i = 0;
    foreach ($nights as $night) {
      $date = strtotime($night);
      $nights_formatted[] = uc_date_format(date('m', $date), date('d', $date), date('Y', $date)) .': '. uc_currency_format($prices[$i]);
      $i++;
    }
    if ($review) {
      return count($nights) .' '. t('Night Stay:') .'<ul><li>'. implode('</li><li>', $nights_formatted) . '</li><ul>';
    }
    if ($html) {
      return $occupants . count($nights) .' '. t('Night Stay:') .'<ul class="product-description"><li>'. implode('</li><li>', $nights_formatted) .'</li></ul></div>';
    }
    else {
      return ' ('. implode(', ', $nights_formatted) .')';
    }
  }
  return;
}

/**
 * Default implementation of theme_hotel_booking_rates_list_form
*/
function theme_hotel_booking_rates_list_form($vars) {
  $form = $vars['form'];
  $header = array(
    t('Rate Name'),
    t('Actions'),
  );

  $rows = array();
  if (!isset($form['rates']) || !is_array($form['rates'])) {
    $form['rates'] = array();
  }
  foreach (element_children($form['rates']) as $key) {
    $rate = $form['rates'][$key]['#value'];
    $rows[] = array(
      l($rate->name, 'admin/store/hotel_booking/rates/'. $rate->hbrid),
      l(t('Edit'), 'admin/store/hotel_booking/rates/'. $rate->hbrid) .' | '.
      l(t('Delete'), 'admin/store/hotel_booking/rates/'. $rate->hbrid .'/delete'),
    );
  }
  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('No base rates exist, please add one.'), 'colspan' => 2));
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= l(t('Add Base Rate'), 'admin/store/hotel_booking/rates/add');
  return $output;
}

/**
 * Default implementation of theme_hotel_booking_rate_modifiers_list_form
*/
function theme_hotel_booking_rate_modifiers_list_form($vars) {
  $form = $vars['form'];
  $header = array(
    t('Amount'),
    t('Actions'),
  );

  $rows = array();
  if (!isset($form['modifiers']) || !is_array($form['modifiers'])) {
    $form['modifiers'] = array();
  }
  foreach (element_children($form['modifiers']) as $key) {
    $modifier = $form['modifiers'][$key]['#value'];
    if ($modifier->method == 'P') {
      $amount = t('Base rate plus !percent%', array('!percent' => number_format($modifier->rate, 2, '.', ',')));
    }
    else {
      $amount = t('Base rate plus !rate', array(
            '!rate' => uc_currency_format($modifier->rate)));
    }
    $rows[] = array(
      $amount,
      l(t('Edit'), 'admin/store/hotel_booking/rate_modifiers/'. $modifier->hbrmid) .' | '.
      l(t('Delete'), 'admin/store/hotel_booking/rate_modifiers/'. $modifier->hbrmid .'/delete'),
    );
  }
  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('No rate modifiers exist, please add one.'), 'colspan' => 2));
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= l(t('Add rate modifier'), 'admin/store/hotel_booking/rate_modifiers/add');
  return $output;
}

/**
 * Default implementation of theme_hotel_booking_occupany_modifiers_list_form
*/
function theme_hotel_booking_occupancy_modifiers_list_form($vars) {
  $form = $vars['form'];
  $header = array(
    t('Threshold'),
    t('Amount'),
    t('Actions'),
  );

  $rows = array();
  $captions = array(
    'A' => array('1 adult', '@count adults'),
    'C' => array('1 child', '@count children'),
    'B' => array('1 occupant', '@count occupants')
  );
  if (!isset($form['modifiers']) || !is_array($form['modifiers'])) {
    $form['modifiers'] = array();
  }
  foreach (element_children($form['modifiers']) as $key) {
    $modifier = $form['modifiers'][$key]['#value'];
    list($singular, $plural) = $captions[$modifier->type];
    $criteria = format_plural($modifier->threshold, $singular, $plural);
    $rows[] = array(
      $criteria,
      uc_currency_format($modifier->rate),
      l(t('Edit'), 'admin/store/hotel_booking/occupancy_modifiers/'. $modifier->hbomid) .' | '.
      l(t('Delete'), 'admin/store/hotel_booking/occupancy_modifiers/'. $modifier->hbomid .'/delete'),
    );
  }
  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('No occupancy modifiers exist, please add one.'), 'colspan' => 3));
  }

  $output = '<p>'. t('Amounts are added to room cost, for each night, for each additional guest exceeding the threshold.') .'</p>';

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  $output .= l(t('Add Occupancy Modifier'), 'admin/store/hotel_booking/occupancy_modifiers/add');

  return $output;
}

/**
 * Default implementation of theme_hotel_booking_availability_form
*/
function theme_hotel_booking_availability_form($vars) {
  $form = $vars['form'];
  drupal_add_css(drupal_get_path('module', 'hotel_booking') .'/css/hotel_booking_calendars.css');
  $output = '';

  $header = array(
    array('data' => t('Sunday'), 'class' => 'calendar-day-name'),
    array('data' => t('Monday'), 'class' => 'calendar-day-name'),
    array('data' => t('Tuesday'), 'class' => 'calendar-day-name'),
    array('data' => t('Wednesday'), 'class' => 'calendar-day-name'),
    array('data' => t('Thursday'), 'class' => 'calendar-day-name'),
    array('data' => t('Friday'), 'class' => 'calendar-day-name'),
    array('data' => t('Saturday'), 'class' => 'calendar-day-name')
  );

  $rows = $row = array();
  $counter = 1;

  //start with our blanks
  $blank = $form['calendar']['#blank'];
  while ($blank > 0) {
    $row[] = '&nbsp;';
    $blank--;
    $counter++;
  }

  foreach (element_children($form['calendar']) as $day) {
    //render the cell
    $row[] = '<div class="calendar-day-number">'. $day .'</div>
              <div class="calendar-edit-wrap">'.
              render($form['calendar'][$day])
              .'</div>';
    $counter++;

    // Make sure we start a new row every week
    if ($counter > 7) {
      $counter = 1;
      $rows[] = $row;
      $row = array();
    }
  }

  // Finaly we finish out the calendar with some blanks if needed
  while ($counter > 1 && $counter <= 7) {
    $row[] = '&nbsp;';
    $counter++;
  }
  //add the final row
  if (count($row)) {
    $rows[] = $row;
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows,
                                  'attributes' => array('class' => array('hotel-booking-availability-edit'))));

  //render the rest of the form
  $output .= drupal_render_children($form);

  //add disclaimer
  $output .= '<p>'. t('Dates older than the configured expiry time, or without a configured base rate value will be reset to 0 when this form is submit.') .'</p>';
  return $output;
}

/**
 * Default implementation of theme_hotel_booking_rates_form_calendar
*/
function theme_hotel_booking_rates_form_calendar($vars) {
  $form = $vars['form'];
  drupal_add_css(drupal_get_path('module', 'hotel_booking') .'/css/hotel_booking_calendars.css');
  $output = '';

  $header = array(
    array('data' => t('Sunday'), 'class' => array('calendar-day-name')),
    array('data' => t('Monday'), 'class' => array('calendar-day-name')),
    array('data' => t('Tuesday'), 'class' => array('calendar-day-name')),
    array('data' => t('Wednesday'), 'class' => array('calendar-day-name')),
    array('data' => t('Thursday'), 'class' => array('calendar-day-name')),
    array('data' => t('Friday'), 'class' => array('calendar-day-name')),
    array('data' => t('Saturday'), 'class' => array('calendar-day-name'))
  );

  $rows = $row = array();
  $counter = 1;
  //start with our blanks
  $blank = (isset($form['#blank']) ? $form['#blank'] : 0);
  while ($blank > 0) {
    $row[] = '&nbsp;';
    $blank--;
    $counter++;
  }

  foreach (element_children($form) as $date) {
    //render the cell
    $row[] = '<div class="calendar-day-number">'. $form[$date]['#day'] .'</div>
              <div class="calendar-edit-wrap">'.
              render($form[$date])
              .'</div>';
    $counter++;

    // Make sure we start a new row every week
    if ($counter > 7) {
      $counter = 1;
      $rows[] = $row;
      $row = array();
    }
  }

  // Finaly we finish out the calendar with some blanks if needed
  while ($counter > 1 && $counter <= 7) {
    $row[] = '&nbsp;';
    $counter++;
  }
  //add the final row
  if (count($row)) {
    $rows[] = $row;
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows,
                                  'attributes' => array('class' => array('hotel-booking-rate-edit'))));

  //render the rest of the form
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Default implementation of theme_hotel_booking_calendars
 * @param $results
*/
function theme_hotel_booking_calendars($vars) {
  $node = $vars['node'];
  $results = $vars['results'];
  $output = '<div id="hotel-booking-calendars-outer">';

  foreach ($results as $year => $months) {
    $output .= theme('hotel_booking_calendars_months', array('node' => $node,
                                                             'year' => $year,
                                                             'months' => $months));
  }

  $output .= theme('hotel_booking_availability_key', array());
  $output .= "</div>";
  return $output;
}


/**
 * default implementation of theme_hotel_booking_calendars_months
 * @param $year int year of calendar
 * @param $months array of months
 * @see theme_hotel_calendars_multi_results for format of months
 * @ingroup themeable
*/
function theme_hotel_booking_calendars_months($vars) {
  $node = $vars['node'];
  $year = $vars['year'];
  $months = $vars['months'];
  $output = '';
  foreach ($months as $month => $days) {
    $output .= theme('hotel_booking_calendars_month', array('node' => $node,
                                                            'year' => $year,
                                                            'month' => $month,
                                                            'days' => $days));
  }
  return $output;
}

/**
 * default implementation of theme_hotel_calendars_month
 * @param $year int year
 * @param $month int month
 * @param $days array of day data
 * @see hotel_booking_calendars_node for format of days data
 * @ingroup themeable
*/
function theme_hotel_booking_calendars_month($vars) {
  $node = $vars['node'];
  $year = $vars['year'];
  $month = $vars['month'];
  $days = $vars['days'];
  static $count;
  // Generate the first day of the month
  $first_day = mktime(0, 0, 0, $month, 1, $year);
  $title = t(date('F', $first_day), array(), array('context' => 'month_name'));

  // Find out what day of the week the first day of the month falls on
  $day_of_week = date('D', $first_day);
  $blank = date('w', $first_day);

  $end_day = mktime(0, 0, 0, $month + 1, 0, $year); //last day of previous month
  $days_in_month = date('j', $end_day);

  $show_prices = variable_get('hotel_booking_display_calprices', TRUE);

  drupal_add_css(drupal_get_path('module', 'hotel_booking') .'/css/hotel_booking_calendars.css');
  $class = "hotel-booking-calendar-". ($count + 1);
  if (($count + 1) % 4 == 0 && !$show_prices) {
    //add 'fourth' class to every fourth for themers
    $class .= ' hotel-booking-calendar-fourth';
  }
  if (($count + 1) % 2 == 0 && $show_prices) {
    //add 'second' class to every second for themers when showing prices
    $class .= ' hotel-booking-calendar-second';
  }


  $count++;
  $output = '<div class="hotel-booking-calendar-outer '. $class .' hotel-booking-calendar-with'.
              ($show_prices ? '' : 'out')
              .'-prices"><div class="hotel-booking-calendar-month">'.
              $title .' '. $year;
  if (($menu_item = menu_get_item('node/'. $node->nid .'/availability/'. $year)) &&
      $menu_item['access']) {
    /*we do  it like this b/c other modules might want to
     override our access function using hook_menu_alter -
    doing this means we always check the menu_item so if
    it has been overriden, we get the overriden access callback*/
    $output .= ' '. l(t('edit'), 'node/'. $node->nid .'/availability/'. $year .'/'. $month);
  }
  $output .= '</div>';

  $header = array(
    array('data' => t('Sun'), 'class' => array('calendar-day-name')),
    array('data' => t('Mon'), 'class' => array('calendar-day-name')),
    array('data' => t('Tue'), 'class' => array('calendar-day-name')),
    array('data' => t('Wed'), 'class' => array('calendar-day-name')),
    array('data' => t('Thu'), 'class' => array('calendar-day-name')),
    array('data' => t('Fri'), 'class' => array('calendar-day-name')),
    array('data' => t('Sat'), 'class' => array('calendar-day-name'))
  );

  $rows = $row = array();
  $counter = 1;

  //start with our blanks
  while ($blank > 0) {
    $row[] = '&nbsp;';
    $blank--;
    $counter++;
  }

  // Set the first day of the month to 1
  $day_num = 1;

  // Count up the days, until the end of the month*/
  for ($i = 1; $i <= $days_in_month; $i ++) {
    $context = array(
      'subject' => array(
        'node' => $node
      ),
      'revision' => 'altered'
    );
    if (!isset($days[$i])) {
      continue;
    }
    $data = (array) $days[$i];
    //render the cell
    $data['day'] = $i;
    $data['id'] = 'hotel-booking-cell-'. $node->nid .'-'. $year . $month . $i;

    if ($data['available'] > 0) {
      if ($data['minimum_occupancy'] > 1 ||
          $data['minimum_stay'] > 1 ||
          $data['no_check_in'] ||
          $data['no_check_out']) {
        $data['class'] = 'hotel-booking-state-restricted';
        $data['restrictions'] = theme('hotel_booking_calendar_cell_tip', array('day' => $data));
        if (module_exists('beautytips')) {
          $options[$data['id']] = array(
            'cssSelect' => '#'. $data['id'],
            'text' => $data['restrictions'],
          );
          beautytips_add_beautytips($options);
        }
      }
      else {
        $data['class'] = 'hotel-booking-state-available';
      }
    }
    else {
      $data['class'] = 'hotel-booking-state-unavailable';
    }
    if ($show_prices) {
      $data['rate'] = $data['rate'];
      $data['formatted_price'] = uc_currency_format($data['rate']);
      $data['price'] = theme('hotel_booking_calendar_cell_price', array('day' => $data));
    }
    $row[] = '<div class="calendar-view-wrap">'.
              theme('hotel_booking_calendar_cell', array('day' => $data))
              .'</div>';
    $counter++;

    // Make sure we start a new row every week
    if ($counter > 7) {
      $counter = 1;
      $rows[] = $row;
      $row = array();
    }
  }// Finaly we finish out the calendar with some blanks if needed
  while ($counter > 1 && $counter <= 7) {
    $row[] = '&nbsp;';
    $counter++;
  }
  //add the final row
  if (count($row)) {
    $rows[] = $row;
  }

  return $output .
          theme('table', array('header' => $header, 'rows' => $rows,
                               'attributes' => array('class' => array('hotel-booking-calendar-view'))))
          .'</div>';
}
