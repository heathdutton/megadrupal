<?php
/*
 * @file hotel_booking.room_type.inc
 * Provides a hotel room type
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands (larowlan) leerowlands at rowlands-bcs dot com
 * @author Will Vincent (tcindie) tcindie at gmail dot com
 *
 */


/**
 * Implements hook_node_info().
 */
function hotel_booking_node_info() {
  return array(
    'hotel_room_type' => array(
      'name'           => t('Hotel Room Type'),
      'base'         => 'hotel_booking',
      'description'    => t('Describe a Hotel Room Type and specify its maximum occupancy.'),
      'help'           => t('Enter your room details and choose the rate settings'),
      'has_title'      => TRUE,
      'title_label'    => t('Room type name'),
      'has_body'       => TRUE,
      'body_label'     => t('Room type description'),
      'min_word_count' => 0,
      'locked'         => FALSE,
    )
  );
}

/**
 * Implements hook_node_access().
 * we're careful to return NULL here instead of FALSE so other node_access
 * modules can do their stuff
 */
function hotel_booking_node_access($node, $op, $account) {
  if (is_string($node)) {
    return NODE_ACCESS_IGNORE;
  }
  if ($node->type == 'hotel_room_type') {
    $allow = NULL;
    switch ($op) {
      case 'create':
        $allow = ((user_access('create hotel room types', $account)) ? TRUE : NULL);

      case 'update':
        $allow = ((user_access('edit all hotel room types', $account) ||
          ($node->uid == $account->uid && user_access('edit own hotel room type', $account))) ? TRUE : NULL);

      case 'delete':
        $allow = ((user_access('delete hotel room types', $account)) ? TRUE : NULL);
    }
    if ($allow === TRUE) {
      return NODE_ACCESS_ALLOW;
    }
    return NODE_ACCESS_IGNORE;
  }
  else {
    return NODE_ACCESS_IGNORE;
  }
}

/**
 * Form builder for node form
 */
function hotel_booking_form($node) {
  $type = node_type_get_type($node);
  hotel_booking_inc('util');
  // Set up some necessary variables for use in the forms
  $rate_types = hotel_booking_get_rates(TRUE);
  $rate_modifiers = hotel_booking_get_rate_modifiers(TRUE);
  $adult_occupancy_modifiers = hotel_booking_get_occupancy_modifiers(TRUE, 'A'); // filter to adult
  $child_occupancy_modifiers = hotel_booking_get_occupancy_modifiers(TRUE, 'C'); // filter to child
  $all_occupancy_modifiers = hotel_booking_get_occupancy_modifiers(TRUE, 'B'); // both

  $form['title'] = array(
    '#type'          => 'textfield',
    '#title'         => check_plain($type->title_label),
    '#required'      => TRUE,
    '#default_value' => $node->title,
    '#maxlength'     => 255,
  );

  // Because these nodes, while masquerading as products,
  // don't actually exist in the uc_products table,
  // we'll manually handle the model name/sku so the field isn't blank in sales reports.
  $form['model'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Room Code'),
    '#description'   => t('If you would like to specify a SKU, Model Number, or Room Code for this room type, do so here. It will display as the SKU on completed orders.'),
    '#default_value' => isset($node->model) ? $node->model : '',
    '#maxlength'     => 255,
  );


  $form['rate'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Rate settings'),
    '#description'   => t('Configure which base rate type this room type uses, and any modifiers to that base rate that apply to this room type.'),
    '#collapsible'   => FALSE,
    '#group'         => 'hotel_booking'
  );

  $form['rate']['hbrid'] = array(
    '#type'          => 'radios',
    '#title'         => t('Base Rate'),
    '#required'      => TRUE,
    '#options'       => $rate_types,
    '#default_value' => isset($node->hbrid) ? $node->hbrid : (count($rate_types) ? array_pop(array_keys($rate_types)) : NULL),
  );

  $form['rate']['hbrmid'] = array(
    '#type'          => 'radios',
    '#title'         => t('Modifier'),
    '#required'      => FALSE,
    '#options'       => $rate_modifiers,
    '#default_value' => isset($node->hbrmid) ? $node->hbrmid : array()
  );

  $form['rate']['occupancy'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Occupancy modifiers'),
    '#description'   => t('Occupancy modifiers that apply to this room type. Adult & Children modifiers apply only to adults and children, the Occupants modifiers apply to overall occupant count.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $form['rate']['occupancy']['adult_occupancy_modifiers'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Adults'),
    '#options'       => $adult_occupancy_modifiers,
    '#default_value' => isset($node->adult_occupancy_modifiers) ? $node->adult_occupancy_modifiers : array(),
  );

  $form['rate']['occupancy']['child_occupancy_modifiers'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Children'),
    '#options'       => $child_occupancy_modifiers,
    '#default_value' => isset($node->child_occupancy_modifiers) ? $node->child_occupancy_modifiers : array(),
  );

  $form['rate']['occupancy']['occupancy_modifiers'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Occupants'),
    '#options'       => $all_occupancy_modifiers,
    '#default_value' => isset($node->occupancy_modifiers) ? $node->occupancy_modifiers : array(),
  );

  $form['room'] = array(
    '#type'          => 'fieldset',
    '#title'         => t( 'Room type settings'),
    '#description'   => t('Configure basic settings for this room type.'),
    '#collapsible'   => FALSE,
    '#group'         => 'hotel_booking'
  );

  $form['room']['default_available'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default Number Available'),
    '#description'   => t('Set this to an integer value, for the default number of rooms of this type to make available per day. Rooms will only be made available after a month is edited within the node, however this setting will establish a default value so simply clicking the edit function on a calendar month, and then saving that month will make this number of rooms available for every day of that month.'),
    '#default_value' => isset($node->default_available) ? $node->default_available: 0,
  );

  $form['room']['minimum_occupancy'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default Minimum Occupancy Requirement'),
    '#description'   => t('Set this to an integer value, for the default minimum occupancy requirement of this room type per day. Rooms will be made available with this default occupancy requirement, but the value can be changed when you edit a calendar month within the node. (0 = No requirement)'),
    '#default_value' => isset($node->minimum_occupancy) ? $node->minimum_occupancy : 0,
  );

  $form['room']['capacity'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Maximum Occupancy'),
    '#required'      => TRUE,
    '#description'   => t('Set this to an integer value, for the maximum number of people that can stay in rooms of this type.  (ie: 4)'),
    '#default_value' => isset($node->capacity) ? $node->capacity : '',
  );

  $form['room']['smoking'] = array(
    '#type'          => 'radios',
    '#title'         => t('Smoking Room'),
    '#required'      => TRUE,
    '#options'       => array(
      1 => t('Smoking'),
      0 => t('Non-Smoking'),
    ),
    '#default_value' => isset($node->smoking) ? $node->smoking : 0,
  );

  //add vertical tabs support b/c it looks nicer
  if (function_exists('vertical_tabs_form_pre_render')) {
    $form['#pre_render'][] = 'vertical_tabs_form_pre_render';
  }

  return $form;
}

/**
 * Implements hook_validate().
 */
function hotel_booking_validate($node) {
  $type = node_type_get_type($node);
  if (!is_int((int)$node->capacity)) {
    form_set_error('capacity', t('Please enter the maximum occupancy of this @type as an integer.', array('@type' => $type->name)));
  }

  if (!is_int((int)$node->default_available)) {
    form_set_error('default_available', t('Please enter the default number of rooms to make available for this @type as an integer.', array('@type' => $type->name)));
  }

  if (!is_int((int)$node->minimum_occupancy)) {
    form_set_error('minimum_occupancy', t('Please enter the default minimum occupancy requirement of this @type as an integer.', array('@type' => $type->name)));
  }
  if ($node->minimum_occupancy > $node->capacity) {
    form_set_error('minimum_occupancy', t('You may not specify a minimum occupancy requirement greater than the maximum occupancy.'));
  }
}

/**
 * Implements hook_insert().
 */
function hotel_booking_insert($node) {
  drupal_write_record('hotel_booking_room_types', $node);

  hotel_booking_inc('util');
  // Process occupancy modifiers
  hotel_booking_set_room_from_rate($node->nid);
  hotel_booking_save_node_occupancy_modifiers($node);
  if ($node->is_new && variable_get('hotel_booking_auto_available', 0)) {
    //this is our first insert and we want rooms auto-available
    hotel_booking_auto_availability($node);
  }
}

/**
 * Implements hook_update().
 */
function hotel_booking_update($node) {
  if ($node->revision) {
    hotel_booking_insert($node);
  }
  else {
    drupal_write_record('hotel_booking_room_types', $node, array('vid'));
  }
  //update the from price
  hotel_booking_inc('util');
  hotel_booking_set_room_from_rate($node->nid);
  // Process occupancy modifiers
  hotel_booking_save_node_occupancy_modifiers($node);
}



/**
 * Implements hook_delete().
 */
function hotel_booking_delete(&$node) {
  db_delete('hotel_booking_room_types')->condition('nid', $node->nid)->execute();
  db_delete('hotel_booking_availability_calendars')->condition('nid', $node->nid)->execute();
  // Remove occupancy modifiers
  hotel_booking_delete_node_occupancy_modifiers($node->nid);
}



/**
 * Implements hook_load().
 */
function hotel_booking_load($nodes) {
  $vids = array();
  foreach ($nodes as $node) {
    $vids[] = $node->vid;
  }
  $data = db_query('SELECT * FROM {hotel_booking_room_types}
                                   WHERE vid IN(:vids)', array(':vids' => $vids))->fetchAll();
  foreach ($data as $row) {
    hotel_booking_load_node_occupancy_modifiers($nodes[$row->nid], $row);
    $next = db_query("SELECT MIN(calendar_dt) AS next
                                    FROM {hotel_booking_availability_calendars}
                                    WHERE nid = :nid
                                    AND available > 0", array(':nid' =>$row->nid))->fetch();
    $row->next_available = $next->next;
    if ($row->next_available) {
      $row->next_available_object = new DateObject($row->next_available, date_default_timezone(), DATE_FORMAT_DATETIME);
      $row->next_available_formatted = format_date($row->next_available_object->format('U'), 'short');
    }
    //this is for uc_cart errors
    $row->sell_price = $row->cost = $row->weight = $row->shippable = $row->weight_units = FALSE;
    foreach ($row as $key => $value) {
      $nodes[$row->nid]->$key = $value;
    }
  }
}






/**
 * Get list of selected occupancy modifiers
 * @param $node object the node object we are loading for
 * @param $data object, data object to set values in (returned via hook_load)
 */
function hotel_booking_load_node_occupancy_modifiers($node, &$data) {
  $fields = array(
    'A' => 'adult_occupancy_modifiers',
    'C' => 'child_occupancy_modifiers',
    'B' => 'occupancy_modifiers' //B is for both
  );

  $modifiers = db_query("SELECT hbrom.hbomid, hbom.type
                     FROM {hotel_booking_room_occupancy_modifiers} hbrom
                     INNER JOIN {hotel_booking_occupancy_modifiers} hbom
                     ON hbom.hbomid = hbrom.hbomid
                     WHERE hbrom.nid = :nid", array(':nid' => $node->nid),
                     array('fetch' => PDO::FETCH_OBJ));

  foreach ($modifiers as $modifier) {
    $field = $fields[$modifier->type];
    if (isset($data->{$field}) && is_array($data->{$field})) {
      $data->{$field}[] = $modifier->hbomid;
    }
    else {
      $data->{$field} = array($modifier->hbomid);
    }
  }
}

/**
 * Save selected occupancy modifiers to database
 * @param $node object node object
 */
function hotel_booking_save_node_occupancy_modifiers($node) {
  hotel_booking_delete_node_occupancy_modifiers($node->nid);
  $fields = array('adult_occupancy_modifiers',
                  'child_occupancy_modifiers',
                  'occupancy_modifiers');
  foreach ($fields as $field) {
    foreach ($node->{$field} as $hbomid => $value) {
      if ($value) {
        $record = array(
          'nid' => $node->nid,
          'hbomid' => $hbomid
        );
        drupal_write_record('hotel_booking_room_occupancy_modifiers', $record);
      }
    }
  }
}

/**
 * Delete all occupancy modifiers corresponding with this node from database
 * @param $nid int node id
 */
function hotel_booking_delete_node_occupancy_modifiers($nid) {
  db_delete('hotel_booking_room_occupancy_modifiers')->condition('nid', $nid)->execute();
}

/**
 * Implements hook_product_types().
 */
function hotel_booking_uc_product_types() {
  return array('hotel_room_type');
}

/**
 * Implements hook_view().
 * @see hotel_booking_preprocess_node - form for booking can also be added to template variables
 */
function hotel_booking_view($node, $view_mode) {
    if (module_exists('imagecache') && ($field = variable_get('uc_image_'. $node->type, ''))
        && isset($node->$field) && file_exists($node->{$field}[0]['filepath'])
        && module_exists('uc_product')
        && variable_get('hotel_booking_display_image', 1)) {
      $node->content['image'] = array(
        '#markup' => theme('uc_product_image', array('images' => $node->$field)),
      );
    }

    //add the 'book room' form
    if (variable_get('hotel_booking_form_in_node_content', 1) == 1 && $view_mode == 'full' && node_is_page($node)) {
      $form = drupal_get_form('hotel_booking_book_room_form_'. $node->nid, $node);
      $node->content['hotel_booking_book_room_form'] = array(
        '#markup' => render($form),
        '#weight' => 10
      );
    }

    if (variable_get('hotel_booking_display_nodeview', 1) && $view_mode == 'full' && node_is_page($node)) {
      $path = drupal_get_path('module', 'hotel_booking');
      hotel_booking_inc('calendars');
      $node->content['hotel_calendars'] = array(
        '#markup' => hotel_booking_calendars_node($node),
        '#weight' => 10,
      );
    }
    return $node;
}


/**
 * Implements hook_preprocess_node
*/
function hotel_booking_preprocess_node(&$vars) {
  $node = $vars['node'];
  if ($node->type == 'hotel_room_type' &&
      variable_get('hotel_booking_form_in_node_content', 1) == 0) {
    $form = drupal_get_form('hotel_booking_book_room_form_'. $node->nid, $node);
    $vars['hotel_booking_book_room_form'] = render($form);
  }
  if ($node->type == 'hotel_room_type' &&
      !variable_get('hotel_booking_display_nodeview', 1)) {
    hotel_booking_inc('calendars');
    $vars['hotel_booking_calendars'] = hotel_booking_calendars_node($node);
  }
}

/**
 *Implements hook_forms
 */
function hotel_booking_forms($form_id, $args) {
  $forms = array();
  if (isset($args[0]) && is_object($args[0]) && isset($args[0]->type) && isset($args[0]->nid)) {
    $node = $args[0];
    if ($node->type == 'hotel_room_type') {
      $forms['hotel_booking_book_room_form_'. $node->nid] = array('callback' => 'hotel_booking_book_room_form');
      $forms['hotel_booking_book_room_now_form_'. $node->nid] = array('callback' => 'hotel_booking_book_room_now_form');
    }
  }
  return $forms;
}

/**
 *Handler for hotel_booking_book_room_form - form includes number of occupants, date etc
 */
function hotel_booking_book_room_form($form, $form_state, $node) {
  $form = array();
  $form['#validate'][] = 'hotel_booking_book_room_form_validate';
  $form['#submit'][] = 'hotel_booking_book_room_form_submit';
  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $capacity = $node->capacity;
  $base_form = hotel_booking_search_form(array(), $form_state);
  $base_form = $base_form['search'];
  array_pop($base_form); //remove search button
  $form = array_merge($form, $base_form);
  //tidy up number of occupants
  foreach ($form['adults']['#options'] as $key => $value) {
    if ($key > $capacity) {
      //adults can't exceed capacity
      unset($form['adults']['#options'][$key]);
      unset($form['children']['#options'][$key]);
    }
    if ($key == $capacity) {
      //children can't exceed capacity -1 (as must be one adult)
      unset($form['children']['#options'][$key]);
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' =>  variable_get('hotel_booking_book_room_button_text', t('Book now')),
    '#id' => 'edit-submit-'. $node->nid,
    '#attributes' => array(
      'class' => array('node-book'),
    ),
  );
  return $form;
}

/**
 *Validate handler for book button
 */
function hotel_booking_book_room_form_validate($form, &$form_state) {
  $adults = $form_state['values']['adults'];
  $children = $form_state['values']['children'];
  $visitors = $adults + $children;
  $nid = $form_state['values']['nid'];
  $node = node_load($nid);
  //check visitors ok against capacity
  if ($node->capacity < $visitors) {
    form_set_error('adults', t('This room can only accommodate !capacity occupants',
                               array('!capacity' => $node->capacity)));
    form_set_error('children');
  }
}

/**
 *Submit handler for book now buttons
 */
function hotel_booking_book_room_form_submit($form, &$form_state) {
  hotel_booking_inc('util');
  $values = $form_state['values'];
  $check_in_date = $values['check_in'];
  if (is_array($check_in_date)) {
    $check_in = implode(' ', $check_in_date);
    $check_in_date = new DateObject($check_in, date_default_timezone(), DATE_FORMAT_ISO);
  }
  else {
    $check_in = $check_in_date;
    $check_in_date = new DateObject($check_in, date_default_timezone(), DATE_FORMAT_ISO);
  }
  $nights = $values['nights'];
  $adults = $values['adults'];
  $children = $values['children'];
  $node = node_load($form_state['values']['nid']);
  $smoking = $node->smoking;

  hotel_booking_save_defaults($form_state);

  $check_out = hotel_booking_calculate_checkout($check_in, $nights);

  $visitors = $adults + $children;


  $rooms = hotel_booking_get_available_rooms(
    $check_in,
    $check_out,
    $nights,
    $adults,
    $children,
    $smoking,
    array($node->nid)
  );

  $variables = array(
    'check_in' => $check_in,
    'check_out' => $check_out,
    'nights' => $nights,
    'adults' => $adults,
    'children' => $children,
    'smoking' => $smoking,
    'values' => $values
  );
  
  $room_only = TRUE;

  drupal_alter('hotel_booking_available_rooms', $rooms, $variables, $room_only);

  if (count($rooms) == 0) {
    //the room is not available
    drupal_set_message(t('The room is not available for this date'));
    drupal_goto(drupal_get_path_alias('node/'. $node->nid));
  }

  $results = hotel_booking_calculate_prices(
    $check_in,
    $check_out,
    $rooms,
    $adults,
    $children,
    $smoking,
    $values
  );

  $total = $results['totals'][$node->nid];
  $results = $results['results'];

  $prices = $nights = array();
  if (is_array($results[$node->nid]['prices'])
      && count($results[$node->nid]['prices']) > 0) {
      //nightly breakdown
    foreach ($results[$node->nid]['prices'] as $date => $price) {
      $nights[] = $date;
      $prices[]= $price;
    }
    $data = array(
      'module' => 'hotel_booking',
      'prices' => $prices,
      'nights' => $nights,
      'total' => $total,
      'adults' => $adults,
      'children' => $children,
      'shippable' => FALSE
    );

    $redirect_destination = variable_get('hotel_booking_redirect', 'cart');
    uc_cart_add_item($node->nid, 1, $data);
    $cart_item_id = db_query("SELECT MAX(cart_item_id)
                             FROM {uc_cart_products}
                             WHERE cart_id = :id", array(':id' => uc_cart_get_id(FALSE)))->fetch();

    if ($redirect_destination == 'cart') {
      drupal_goto($redirect_destination);
    }
    elseif ($redirect_destination == 'addon') {
      drupal_goto('booking_upgrades/'. $cart_item_id);
    }
    else {
      drupal_goto(variable_get('hotel_booking_redirect_otherurl', ''));
    }
  }
  else {
    //the room is not available
    drupal_set_message(t('The room is not available for this date'));
    drupal_goto(drupal_get_path_alias('node/'. $node->nid));
  }
}

/**
* Implements hook_field_extra_fields.
*/
function hotel_booking_field_extra_fields() {
  $extras['node']['hotel_room_type'] = array(
    'form' => array(
      'room' => array(
        'label' => t('Room settings form'),
        'description' => t('Room settings form.'),
        'weight' => 2,
      ),
      'rate' => array(
        'label' => t('Rate settings form'),
        'description' => t('Room rate settings form.'),
        'weight' => 1,
      )
    ),
    'display' => array(
      'book_form' => array(
        'label' => t('Book room form'),
        'description' => t('Form allowing user to book the room from the node page.'),
        'weight' => 11,
      ),
      'availability_calendars' => array(
        'label' => t('Availability calendars'),
        'description' => t('Availability calendars for the room.'),
        'weight' => 10,
      )
    )
  );

  return $extras;
}
