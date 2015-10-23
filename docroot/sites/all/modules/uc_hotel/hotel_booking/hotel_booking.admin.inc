<?php
/*
 * @file hotel_booking.admin.inc
 * Provides admin functions for hotel booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands (larowlan) leerowlands at rowlands-bcs dot com
 * @author Will Vincent (tcindie) tcindie at gmail dot com
 */

/**
 * Provides validation of amount
*/
function hotel_booking_admin_settings_validate($form, $form_state) {
  $values = $form_state['values'];
  $type = $values['hotel_booking_payment_type'];
  if ($type != 0) {
    $rate = $values['hotel_booking_payment_rate'];
    if (!$rate || !is_numeric($rate)) {
      form_set_error('hotel_booking_payment_rate', t('You must enter a numeric rate'));
    }
    if ($type == 2 && ($rate < 0 || $rate > 100)) {
      form_set_error('hotel_booking_payment_rate', t('You must enter a percentage between 0 and 100'));      
    }
  }
}

/**
 * Admin Settings page callback.
 */
function hotel_booking_admin_settings($form, &$form_state) {
  $form = array();
  global $base_url;

  $form['tabs'] = array(
    '#type' => 'vertical_tabs'
  );

  //display settings
  $form['tabs']['display'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#group'       => 'hotel_booking'
  );

  $form['tabs']['display']['hotel_booking_display_nodeview'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Calendars on node page'),
    '#description'   => t('Leave unchecked if you wish to control the position of the calendars using a node template (node-hotel_room_type.tpl.php). The calendars will be available as the $hotel_booking_calendars variable.'),
    '#default_value' => variable_get('hotel_booking_display_nodeview', TRUE),
  );

  $form['tabs']['display']['hotel_booking_display_monthcount'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Number of months to display'),
    '#default_value' => variable_get('hotel_booking_display_monthcount', 3),
    '#description'   => t("Calendars will show this number of months to all users except those with the 'edit hotel room availability' permissions who will always see a full year (from current month forward)."),
  );

  $form['tabs']['display']['hotel_booking_display_calprices'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display prices in availability calendars'),
    '#description'   => t('If unchecked, prices will not be displayed in availability calendars.'),
    '#default_value' => variable_get('hotel_booking_display_calprices', TRUE),
  );

  $form['tabs']['display']['hotel_booking_form_in_node_content'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display booking form on hotel room type node page?'),
    '#description'   => t('Leave unchecked if you wish to control the position of the form using a node template (node-hotel_room_type.tpl.php). The form will be available as the $hotel_booking_book_room_form variable.'),
    '#default_value' => variable_get('hotel_booking_form_in_node_content', 1),
  );

  $form['tabs']['display']['hotel_booking_display_image'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display images using ubercart image formatting?'),
    '#description'   => t('Leave unchecked if you wish to control the display of images using your theme.'),
    '#default_value' => variable_get('hotel_booking_display_image', 1),
  );

  $form['tabs']['display']['hotel_booking_book_room_button_text'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Text for node page booking form button'),
    '#default_value' => variable_get('hotel_booking_book_room_button_text', t('Book now')),
    '#description'   => t('Text to display on the button on the booking form on the hotel room type node page'),
  );

  //pricing options
  $form['tabs']['payment'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Payment options'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'       => 'hotel_booking'
  );
  $form['tabs']['payment']['hotel_booking_payment_type'] = array(
    '#type'          => 'radios',
    '#title'         => t('Payment type'),
    '#default_value' => variable_get('hotel_booking_payment_type', 0),
    '#description'   => t('Choose the payment type'),
    '#options'       => array(
      0 => t('Full payment on checkout'),
      1 => t('Fixed deposit fee on checkout'),
      2 => t('Percentage deposit on checkout')
    ),
    '#required'      => TRUE,
  );
  $form['tabs']['payment']['hotel_booking_payment_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Payment rate'),
    '#default_value' => variable_get('hotel_booking_payment_rate'),
    '#size'          => 60,
    '#description'   => t('Enter the payment amount, if using a deposit on checkout. If using a fixed fee, enter the fee. If using a percentage enter the amount (eg 20% = 20).'),
    '#maxlength'     => 10,
    '#required'      => FALSE,
  );

  //redirect options
  $form['tabs']['redirect'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Browser Redirect Options'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'       => 'hotel_booking'
  );

  $form['tabs']['redirect']['hotel_booking_redirect'] = array(
    '#type'          => 'radios',
    '#title'         => t('Where to redirect when a hotel room booking is added to the cart'),
    '#default_value' => variable_get('hotel_booking_redirect', 'cart'),
    '#options'       => array('cart' => t('Cart'), 'other' => t('Other')),
  );

  $form['tabs']['redirect']['hotel_booking_redirect_otherurl'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL to redirect to'),
    '#default_value' => variable_get('hotel_booking_redirect_otherurl', ''),
    '#description'   => t("If redirect destination of 'other' is selected above, enter the URL here, otherwise leave blank."),
    '#field_prefix'  => $base_url .'/',
  );

  //block options
  $form['tabs']['block'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Block Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#group'       => 'hotel_booking'
  );

  // Build list of room types
  $room_types[] = t('None');
  $results = db_query('SELECT hrt.nid, n.title
                      FROM {hotel_booking_room_types} AS hrt
                      INNER JOIN {node} AS n
                      ON n.nid = hrt.nid');
  foreach ($results as $result) {
    $room_types['node/'. $result->nid] = check_plain($result->title);
  }

  $form['tabs']['block']['hotel_booking_best_rate'] = array(
    '#type'          => 'select',
    '#title'         => t('Room with best rate'),
    '#default_value' => variable_get('hotel_booking_best_rate', '0'),
    '#options'       => $room_types,
    '#description'   => t("Select the room type with the best rate to display a link to it in the search block, if no room type is selected, the link will not be displayed."),
  );

  $form['tabs']['block']['hotel_booking_block_children'] = array(
    '#type'          => 'select',
    '#title'         => t('Children Selection'),
    '#default_value' => variable_get('hotel_booking_block_children', '1'),
    '#options'       => array(0 => 'No', 1 => 'Yes'),
    '#description'   => t('Select whether or not to display the children selection option in search block. Value will be 0 for searches if No is selected here.'),
  );

  $form['tabs']['block']['hotel_booking_block_smoking'] = array(
    '#type'          => 'select',
    '#title'         => t('Smoking Preference Selection'),
    '#default_value' => variable_get('hotel_booking_block_smoking', '1'),
    '#options'       => array(0 => 'No', 1 => 'Yes'),
    '#description'   => t('Select whether or not to display the smoking preference selection option in search block. Value will be No Preference if No is selected here.'),
  );

  //search options
  $form['tabs']['search'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Search Result Options'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#group'       => 'hotel_booking'
  );

  $form['tabs']['search']['hotel_booking_sort_order'] = array(
    '#type'        => 'radios',
    '#title'       => t('Sort Order'),
    '#options'     => array(
        'ASC' => t('Lowest price rooms first'),
        'DESC' => t('Highest price rooms first')
      ),
    '#default_value' => variable_get('hotel_booking_sort_order', 'ASC'),
    '#description' => t('The order to sort room search results.'),
  );

  $form['tabs']['search']['hotel_booking_calendar_in_search_results'] = array(
    '#type'        => 'checkbox',
    '#title'       => t('Calendar in search results'),
    '#default_value' => variable_get('hotel_booking_calendar_in_search_results', 1),
    '#description' => t('Show availability calendars in the search results.'),
  );

  $form['tabs']['search']['hotel_booking_no_search_results'] = array(
    '#type'        => 'textarea',
    '#title'       => t('Empty Search Result Message'),
    '#default_value' => variable_get('hotel_booking_no_search_results', '<p>Please select different dates and try your search again.</p>'),
    '#description' => t('This text will be displayed to users when a search returns no results. HTML is valid in this textarea.<br /><strong>Note:</strong> This will always be preceeded by <i>Your search returned 0 results.</i>'),
  );

  //expiry options
  $form['tabs']['availability'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Availability Settings'),
    '#description' => t('Select when you want available rooms to auto expire and other availability settings.'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#group'       => 'hotel_booking'
  );

  $form['tabs']['availability']['hotel_booking_expiration'] = array(
    '#type'        => 'radios',
    '#title'       => t('Close Availability'),
    '#options'     => array('yesterday' => t('Yesterday'), 'today' => t('Today'), 'tomorrow' => t('Tomorrow'), ),
    '#description' => t('Availability will be zeroed out by cron for dates <em>Equal To AND Older Than</em> this setting.'),
    '#default_value' => variable_get('hotel_booking_expiration', 'today'),
  );

  $form['tabs']['availability']['hotel_booking_auto_available'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Auto available interval'),
    '#description'   => t('Automatically make the default number of
                          rooms available this many months
                          in advance. Enter zero to set availability manually.'),
    '#default_value' => variable_get('hotel_booking_auto_available', 0),
  );


  //cart options
  $form['tabs']['cart'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Cart & Checkout settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#group'       => 'hotel_booking'
  );

  $form['tabs']['cart']['hotel_booking_teaser_in_cart'] = array(
    '#type'        => 'radios',
    '#title'       => t('Teaser in cart'),
    '#options'     => array(t('Yes'), t('No')),
    '#description' => t('Whether or not to include the teaser text in the cart. Applies to both Room Bookings and Upgrade/Addon Products'),
    '#default_value' => variable_get('hotel_booking_teaser_in_cart', 0),
  );

  $form['tabs']['cart']['hotel_booking_teaser_in_panes'] = array(
    '#type'        => 'radios',
    '#title'       => t('Teaser in cart, checkout, and order panes'),
    '#options'     => array(t('Yes'), t('No')),
    '#description' => t('Whether or not to include the teaser text in the Booking Details panes on cart, checkout, and order review pages.'),
    '#default_value' => variable_get('hotel_booking_teaser_in_panes', 0),
  );

  $form['tabs']['cart']['hotel_booking_reward_prompt'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Reward Membership Prompt'),
    '#default_value' => variable_get('hotel_booking_reward_prompt', ''),
    '#description'   => t('If you would like to prompt the user for a Reward/Membership number at checkout, enter the text to prompt them with here. Leave blank to disable the checkout field.'),
  );

  return system_settings_form($form);
}

/**
 * Form builder for hotel_booking rates
 * We use a form to build a table
 * so other modules can modify it with hook_form_alter
 * but theme it as a table
 * @see theme_hotel_booking_rates_list_form
*/
function hotel_booking_rates_list_form($form, &$form_state) {
  hotel_booking_inc('util');
  $rates = hotel_booking_get_rates();
  $form = array();
  foreach ($rates as $rate) {
    $form['rates'][$rate->hbrid] = array(
      '#value' => $rate,
      '#type' => 'value'
    );
  }

  return $form;
}

/**
 * Form builder for hotel_booking rate modifiers
 * We use a form to build a table
 * so other modules can modify it with hook_form_alter
 * but theme it as a table
 * @see theme_hotel_booking_rate_modifiers_list_form
*/
function hotel_booking_rate_modifiers_list_form($form, &$form_state) {
  hotel_booking_inc('util');
  $modifiers = hotel_booking_get_rate_modifiers();

  $form = array();
  foreach ($modifiers as $modifier) {
    $form['modifiers'][$modifier->hbrmid] = array(
      '#value' => $modifier,
      '#type' => 'value'
    );
  }

  return $form;
}

/**
 * Form builder for hotel_booking occupancy modifiers
 * We use a form to build a table
 * so other modules can modify it with hook_form_alter
 * but theme it as a table
 * @see theme_hotel_booking_occupancy_modifiers_list_form
*/
function hotel_booking_occupancy_modifiers_list_form($form, &$form_state) {
  hotel_booking_inc('util');
  $modifiers = hotel_booking_get_occupancy_modifiers();

  $form = array();
  foreach ($modifiers as $modifier) {
    $form['modifiers'][$modifier->hbomid] = array(
      '#value' => $modifier,
      '#type' => 'value'
    );
  }

  return $form;
}

/**
 * Hotel Booking Rate Modifiers form builder.
 * @param $form_state array std FAPI form state var
 * @param $modifier object modifer object
 * @see hotel_booking_rate_modifier_load
 */

function hotel_booking_rate_modifiers_form($form, &$form_state, $modifier = FALSE) {
  if (!$modifier) {
    drupal_set_title(t('New Rate Modifier'));
  }
  else {
    drupal_set_title(t('Rate Modifier #!modifier', array('!modifier' => $modifier->hbrmid)));
  }

  $form = array();

  $form['hbrmid'] = array(
    '#type'     => 'value',
    '#value'    => $modifier ? $modifier->hbrmid : NULL,
  );

  $form['method'] = array(
    '#type'          => 'radios',
    '#title'         => t('Method'),
    '#description'   => t('Which method will be used to apply this modifier, dollar value or percentage of base rate'),
    '#options'       => array('P' => 'Percentage', 'V' => 'Dollar Value'),
    '#default_value' => $modifier ? $modifier->method : 'P',
  );

  $form['rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Adjustment Amount'),
    '#required'      => TRUE,
    '#default_value' => $modifier ? number_format($modifier->rate, 2,
                                      variable_get('uc_currency_dec', '.'),
                                      variable_get('uc_currency_thou', ',')) : NULL,
  );

  if (!$modifier) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 14,
    );
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 14,
    );
    $form['delete_button'] = array(
      '#type' => 'submit',
      '#name' => 'delete',
      '#value' => t('Delete'),
      '#weight' => 15,
    );
  }
  return $form;
}

/**
 * Hotel booking occupancy modifiers form builder
 * @param $form_state array standard FAPI form state var
 * @param $modifier object occupancy modifier
 * @see hotel_booking_occupancy_modifier_load
*/
function hotel_booking_occupancy_modifiers_form($form, &$form_state, $modifier = FALSE) {
  if (!$modifier) {
    drupal_set_title(t('New Occupancy Modifier'));
  }
  else {
    drupal_set_title(t('Occupancy Modifier #!modifier', array('!modifier' => $modifier->hbomid)));
  }

  $form = array();

  $form['hbomid'] = array(
    '#type'     => 'value',
    '#value'    => $modifier ? $modifier->hbomid : NULL,
  );

  $form['type'] = array(
    '#type'        => 'radios',
    '#title'       => t('Guest type'),
    '#description' => t('Select whether this modifier applies to adults, children, or overall number of occupants'),
    '#options'     => array(
      'A' => t('Adults'),
      'C' => t('Children'),
      'B' => t('Occupants')
    ),
    '#default_value' => $modifier ? $modifier->type : 'A',
  );

  $form['threshold'] = array(
    '#title'       => t('Threshold'),
    '#type'        => 'textfield',
    '#required'    => TRUE,
    '#default_value' => $modifier ? $modifier->threshold : 0,
    '#description' => t('Each guest over this threshold will be charged the additional adjustment amount per night.'),
  );

  $form['rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Adjustment Amount'),
    '#required'      => TRUE,
    '#field_prefix'  => variable_get('uc_currency_sign', '$'),
    '#default_value' => $modifier ? number_format($modifier->rate, 2,
                                      variable_get('uc_currency_dec', '.'),
                                      variable_get('uc_currency_thou', ',')) : NULL,
  );

  if (!$modifier) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 14,
    );
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 14,
    );
    $form['delete_button'] = array(
      '#type' => 'submit',
      '#name' => 'delete',
      '#value' => t('Delete'),
      '#weight' => 15,
    );
  }
  return $form;
}



/**
 * Hotel Booking Rates form.
 * @param $form_state array std FAPI form state var
 * @param $rate object rate object
 * @see hotel_booking_rate_load
 */

function hotel_booking_rates_form($form, &$form_state, $rate = FALSE) {
  if (!$rate) {
    drupal_set_title(t('New Base Rate'));
  }
  else {
    drupal_set_title($rate->name);
  }

  $sign_flag = variable_get('uc_sign_after_amount', FALSE);
  $currency_sign = variable_get('uc_currency_sign', '$');

  $form['hbrid'] = array(
    '#type'          => 'value',
    '#value'         => $rate ? $rate->hbrid : NULL,
  );

  $form['name'] = array(
    '#title'         => t('Base Rate Name'),
    '#type'          => 'textfield',
    '#weight'        => -50,
    '#required'      => TRUE,
    '#default_value' => $rate ? $rate->name : NULL,
  );

  $form['calendars'] = array(
    '#type' => 'vertical_tabs'
  );

  // Generate 12 months of calendars
  for ($i = 0; $i < 12; $i++) {
    $year = date("Y");
    $month = date("m") + $i;
    if ($month > 12) {
      $month -= 12;
      $year++;
    }

    $ref_date = mktime(0, 0, 0, $month, 1, $year);
    $title = t(
      '!month_name'. date("F", $ref_date) .' !year',
      array(
        '!month_name' => '',
        '!year' => date("Y", $ref_date)
      )
    );

    $form['calendars']['calendar_'. $i] = array(
      '#title'       => $title,
      '#type'        => 'fieldset',
      '#collapsible' => TRUE,
      '#weight'      => $i,
      '#collapsed'   => ($i > 0), //first is expanded
      '#group'       => 'hotel_booking'
    );

    $form['calendars']['calendar_'. $i]['rate_all_'. $i] = array(
      '#title'        => t('Set all !month_title rates to', array('!month_title' => $title)),
      '#type'         => 'textfield',
      '#size'         => 12,
      '#attributes'   => array('class' => array('uc_hotel_set_all uc_hotel_rate'. $i))
    );
    $form['calendars']['calendar_'. $i]['set_all_'. $i] = array(
      '#value'        => t('Apply'),
      '#type'         => 'button',
      '#attributes'   => array(
        'class'    => array('uc_hotel_set_all'),
        'rel'      => 'uc_hotel_rate'. $i
      )
    );
    $form['calendars']['calendar_'. $i]['rates'] = hotel_booking_rate_calendar($form_state, $month, $year, $rate, $i);

    $path = drupal_get_path('module', 'hotel_booking');
    drupal_add_js($path .'/js/hotel_booking.js');
  }

  if (!$rate) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 13,
    );
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 13,
    );
    $form['delete_button'] = array(
      '#type' => 'submit',
      '#name' => 'delete',
      '#value' => t('Delete'),
      '#weight' => 14,
    );
  }
  return $form;
}



/**
 * Hotel Booking Rates form submission
 *
 */
function hotel_booking_rates_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Save'):
    case t('Update'):
      //we are saving
      $record = $form_state['values'];
      $update = array();
      if ($record['hbrid']) {
        $update = array('hbrid');
      }
      $result = drupal_write_record('hotel_booking_rates', $record, $update);

      foreach ($record['rates'] as $date => $rate) {
        db_delete("hotel_booking_rate_calendars")
        ->condition('hbrid', $record['hbrid'])
        ->condition('calendar_dt', $date)
        ->execute();
        $rate_record = array(
          'hbrid' => $record['hbrid'],
          'calendar_dt' => $date,
          'rate' => $rate
        );
        drupal_write_record('hotel_booking_rate_calendars', $rate_record);
      }
      drupal_set_message(t('Saved your base rate'));
      hotel_booking_inc('util');
      hotel_booking_after_price_change($record['hbrid'], 'hbrid');
      $form_state['redirect'] = "admin/store/hotel_booking/rates";
      break;

    case t('Delete'):
      drupal_goto('admin/store/hotel_booking/rates/'. $form_state['values']['hbrid'] .'/delete');
      break;

    default:
      //this is the set all when user has no javascript
      for ($i = 0; $i < 12; $i++) {
        if (isset($form_state['values']['set_all_'. $i])) {
          if (isset($form_state['values']['rate_all_'. $i])) {
            $year = date("Y");
            $month = date("m") + $i;
            if ($month > 12) {
              $month -= 12;
              $year++;
            }
            $end_day = mktime(0, 0, 0, $month + 1, 0, $year); //last day of previous month
            $days_in_month = date('j', $end_day);

            $first_day = mktime(0, 0, 0, $month, 1, $year);

            while ($first_day <= $days_in_month) {
              $form_state['values']['rates'][date(DATE_FORMAT_DATE, $first_day)] = $form_state['values']['rate_all_'. $i];
              $first_day += 86400; //86400 sec = 1 day
            }
            $form_state['rebuild'] = TRUe;
          }
          else {
            form_set_error('calendars_'. $i .'][rate_all_'. $i, t('You must provide a rate to set all values to'));
          }
        }
      }

      break;
  }
}

/**
 * Submit handler for rate modifiers
*/

function hotel_booking_rate_modifiers_form_submit($form, &$form_state) {
  $record = $form_state['values'];
  $update = array();
  if ($record['hbrmid']) {
    $update = array('hbrmid');
    $form_state['values']['is_new'] = FALSE;
  }
  $result = drupal_write_record('hotel_booking_rate_modifiers', $record, $update);
  if (!count($update)) {
    $form_state['values']['hbrmid'] = $record['hbrmid'];
    $form_state['values']['is_new'] = TRUE;
  }
  hotel_booking_inc('util');
  hotel_booking_after_price_change($record['hbrmid'], 'hbrmid');
  $form_state['redirect'] = "admin/store/hotel_booking/rate_modifiers";
}

/**
 * Submit handler for occupancy modifiers
*/

function hotel_booking_occupancy_modifiers_form_submit($form, &$form_state) {
  $record = $form_state['values'];
  $update = array();
  if ($record['hbomid']) {
    $update = array('hbomid');
  }
  $result = drupal_write_record('hotel_booking_occupancy_modifiers', $record, $update);
  $form_state['redirect'] = "admin/store/hotel_booking/occupancy_modifiers";
}


/**
 * Hotel Booking Rates form validation
 *
 */
function hotel_booking_rates_form_validate($form, &$form_state) {
  foreach ($form_state['values']['rates'] as $date => $rate) {
    if (!is_numeric($rate)) {
      form_set_error('rates]['. $date, t('You must enter a numeric rate'));
    }
  }
}


/**
 * Validate rate modifier submission
*/
function hotel_booking_rate_modifiers_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['rate'])) {
    form_set_error('value', t('You must enter a numeric value.'));
  }
  if (isset($form_state['values']['delete'])) {
    drupal_goto('admin/store/hotel_booking/rate_modifiers/'. $form_state['values']['hbrmid'] .'/delete');
  }
}

/**
 * Validate occupancy modifier submission
*/
function hotel_booking_occupancy_modifiers_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['rate'])) {
    form_set_error('value', t('You must enter a numeric value.'));
  }
  if (isset($form_state['values']['delete'])) {
    drupal_goto('admin/store/hotel_booking/occupancy_modifiers/'. $form_state['values']['hbomid'] .'/delete');
  }
}

/**
 * Hotel Booking Rates deletion form.
 * @param $form_state array standard fapi var
 * @param $rate object see hotel_booking_rates table
 * @see hotel_booking_rate_load
 */
function hotel_booking_rates_delete_form($form, &$form_state, $rate = FALSE) {
  if (!$rate) {
    drupal_goto('admin/store/hotel_booking/rates');
  }

  drupal_set_title(t('Delete !name', array('!name' => $rate->name)));

  $form['hbrid'] = array(
    '#type' => 'value',
    '#value' => $rate->hbrid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $rate->name,
  );
  return confirm_form($form,
    t('Delete @name?', array('@name' => $rate->name)),
    'admin/store/hotel_booking/rates/'. $rate->hbrid,
    '',
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form to delete rate modifier
 * @param $form_state array standard fapi var
 * @param $modifer object see hotel_booking_rates table
 * @see hotel_booking_rate_load
 */

function hotel_booking_rate_modifiers_delete_form($form, &$form_state, $modifier = FALSE) {
  if (!$modifier) {
    drupal_goto('admin/store/hotel_booking/rate_modifiers');
  }

  drupal_set_title(t('Delete Rate Modifier #!name', array('!name' => $modifier->hbrmid)));

  $form['hbrmid'] = array(
    '#type' => 'value',
    '#value' => $modifier->hbrmid,
  );
  return confirm_form($form,
    t('Delete Modifier?'),
    'admin/store/hotel_booking/rate_modifiers/'. $modifier->hbrmid,
    t('Are you sure, this cannot be undone'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form to delete occupancy modifier
 * @param $form_state array standard fapi var
 * @param $modifer object see hotel_booking_rates table
 * @see hotel_booking_rate_load
 */

function hotel_booking_occupancy_modifiers_delete_form($form, &$form_state, $modifier = FALSE) {
  if (!$modifier) {
    drupal_goto('admin/store/hotel_booking/occupancy_modifiers');
  }

  drupal_set_title(t('Delete Occupancy Modifier #!name', array('!name' => $modifier->hbomid)));

  $form['hbomid'] = array(
    '#type' => 'value',
    '#value' => $modifier->hbomid,
  );
  return confirm_form($form,
    t('Delete Modifier?'),
    'admin/store/hotel_booking/occupancy_modifiers/'. $modifier->hbomid,
    t('Are you sure, this cannot be undone'),
    t('Delete'),
    t('Cancel')
  );
}


/**
 * Hotel Booking Rates deletion form submissions
 */
function hotel_booking_rates_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('hotel_booking_rates')->condition('hbrid', $form_state['values']['hbrid'])->execute();
    db_delete('hotel_booking_rate_calendars')->condition('hbrid', $form_state['values']['hbrid'])->execute();
    hotel_booking_inc('util');
    hotel_booking_after_price_change($form_state['values']['hbrid'], 'hbrid');
    drupal_set_message(t('Rate "@name" has been deleted', array('@name' => $form_state['values']['name'])));
  }

  $form_state['redirect'] = 'admin/store/hotel_booking/rates';
}

/**
 * Rate modifier delete submission handler
*/
function hotel_booking_rate_modifiers_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('hotel_booking_rate_modifiers')->condition('hbrmid', $form_state['values']['hbrmid'])->execute();
    hotel_booking_inc('util');
    hotel_booking_after_price_change($form_state['values']['hbrmid'], 'hbrmid');
    drupal_set_message(t('Deleted your modifier'));
  }

  $form_state['redirect'] = 'admin/store/hotel_booking/rate_modifiers';
}
/**
 * Occupancy modifier delete submission handler
*/

function hotel_booking_occupancy_modifiers_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('hotel_booking_occupancy_modifiers')->condition('hbomid', $form_state['values']['hbomid'])->execute();
    db_delete('hotel_booking_room_occupancy_modifiers')->condition('hbomid', $form_state['values']['hbomid'])->execute();
    drupal_set_message(t('Deleted your occupancy modifier'));
  }

  $form_state['redirect'] = 'admin/store/hotel_booking/occupancy_modifiers';
}


/**
 * Base Rate Calendar
 * @param $form_state array std fapi form state array
 * @param $month int month of calendar
 * @param $year int year of calendar
 * @param $rate object rate object, prices are in $rate->rates['date']
 * @param $group int counter for groups (fieldsets) on form from 1 to 12 for 12 months
 *                  note that 1 does not equal jan, but equals current month
 */
function hotel_booking_rate_calendar($form_state, $month, $year, $rate, $group) {
  $sign_flag = variable_get('uc_sign_after_amount', FALSE);
  $currency_sign = variable_get('uc_currency_sign', '$');

  $first_day = mktime(0, 0, 0, $month, 1, $year);
  $break = mktime(0, 0, 0, $month + 1, 1, $year);

  // Find out what day of the week the first day of the month falls on
  $day_of_week = date('D', $first_day);
  $blank = date('w', $first_day);

  $end_day = mktime(0, 0, 0, $month + 1, 0, $year); //last day of previous month
  $days_in_month = date('j', $end_day);

  //store these for the theme function
  $form = array(
    '#theme' => 'hotel_booking_rates_form_calendar',
    '#tree' => TRUE,
    '#first_day' => $first_day,
    '#day_of_week' => $day_of_week,
    '#blank' => $blank,
    '#end_day' => $end_day,
    '#tree' => TRUE,
    '#days_in_month' => $days_in_month
  );

  $rows = $row = array();
  $counter = new DateObject(date(DATE_FORMAT_ISO, $first_day), date_default_timezone(), DATE_FORMAT_ISO);

  while ((int)$counter->format('U') < $break) {
    $ref_day = $counter->format(DATE_FORMAT_DATE);
    $form[$ref_day] = array(
      '#type'          => 'textfield',
      '#attributes'    => array(
        'class' => array('uc_hotel_rate'. $group)
      ),
      '#day'           => $counter->format('j'),
      '#field_prefix'  => !$sign_flag ? $currency_sign : FALSE,
      '#field_suffix'  => $sign_flag ? $currency_sign : FALSE,
      '#default_value' => (isset($form_state['rates']) && isset($form_state['rates'][$ref_day])) ?
                          $form_state['rates'][$ref_day] :
                          (isset($rate) && is_object($rate) && is_array($rate->rates) && isset($rate->rates[$ref_day]) ?
                            $rate->rates[$ref_day] :
                          0),
      '#size'          => 10,
      '#maxlength'     => 7,
      '#required'      => TRUE,
    );
    $counter->modify('+1 day');
  }

  return $form;
}
