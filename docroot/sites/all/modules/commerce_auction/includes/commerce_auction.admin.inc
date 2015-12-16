<?php

/**
 * @file
 * Contains commerce_auction admin form callback functions.
 */

/**
 * Commerce auction administration form callback.
 */
function commerce_auction_admin_form($form, &$form_state) {
  $types = node_type_get_types();
  $options = array();
  $default_types = variable_get('commerce_auction_display_types', array());
  foreach ($types as $type => $obj) {
    $options[$type] = $obj->name;
  }

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('You can configure behavior of the commerce auction module in this page.') . '<br />' . t('Select Auction product display content types and adjust timeout settings.'),
  );

  $form['auction_displays'] = array(
    '#title' => t('Auction display content types'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $options,
    '#default_value' => $default_types,
  );

  $form['auction_timeout_extend'] = array(
    '#type' => 'fieldset',
    '#title' => t('Auction Timeout settings'),
    '#description' => t('All times are measured in minutes'),
  );
  $form['auction_timeout_extend']['extend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Extend auction time if there is a new bid in the last few minutes'),
    '#default_value' => variable_get('commerce_auction_extend', TRUE),
  );
  $form['auction_timeout_extend']['last_minutes'] = array(
    '#type' => 'textfield',
    '#title' => t('Extend if a new bid is placed in'),
    '#default_value' => variable_get('commerce_auction_final_period', 15),
    '#states' => array(
      'visible' => array(
        ':input[name="extend"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['auction_timeout_extend']['extention'] = array(
    '#type' => 'textfield',
    '#title' => t('Extend the auction time for'),
    '#default_value' => variable_get('commerce_auction_extension_time', 30),
    '#states' => array(
      'visible' => array(
        ':input[name="extend"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['bid_limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bid Limits'),
    '#description' => t('These values may be overriden on a per-product basis when editing an auctioned node product.'),
  );
  $currency = commerce_currency_load(commerce_default_currency());
  $form['bid_limits']['bid_increment'] = array(
    '#type' => 'textfield',
    '#title' => t('Bid increment'),
    '#field_prefix' => $currency['symbol'],
    '#size' => 10,
    '#default_value' => variable_get('commerce_auction_bid_increment', 0.25),
    '#description' => t('Bids must be a multiple of this increment to be accepted. If your site is using a currency which does not have subunits in circulation (JPY, KRW), this should be an integer.'),
  );
  $form['bid_limits']['min_bid_inc'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum bid increase'),
    '#field_prefix' => $currency['symbol'],
    '#size' => 10,
    '#default_value' => variable_get('commerce_auction_min_bid_inc', 0.50),
    '#description' => t('New bids must be at least this much higher than the current high bid to be accepted. This value should be a multiple of the Bid increment value.'),
  );
  $form['bid_limits']['max_bid_inc'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum bid increase'),
    '#field_prefix' => $currency['symbol'],
    '#size' => 10,
    '#default_value' => variable_get('commerce_auction_max_bid_inc', 1000.00),
    '#description' => t('New bids which are this much higher than the current high bid will not be accepted. Setting a maximum bid increase can make it more difficult for bidders to place a ridiculously high bid, either accidentally or otherwise. Enter zero or leave blank to not enforce this limit. In the case that both the Maximum bid increase and Maximum bid increase percentage values are set, both limits will be enforced.'),
  );
  $form['bid_limits']['max_bid_inc_percent'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum bid increase percentage'),
    '#field_suffix' => '%',
    '#size' => 3,
    '#default_value' => variable_get('commerce_auction_max_bid_inc_percent', 100),
    '#description' => t("Bids which increase the high bid value by this percentage will not be accepted. For example, if this value is set to 50% and an item’s current high bid is ¤1000, bids larger than ¤1500 will not be accepted (as ¤500 is 50% of ¤1000). Setting a maximum bid increase percentage can make it more difficult for bidders to place a ridiculously high bid, either accidentally or otherwise. Enter zero or leave blank to not enforce this limit. In the case that both the Maximum bid increase and Maximum bid increase percentage values are set, both limits will be enforced."),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Commerce Auction administration form validation callback.
 */
function commerce_auction_admin_form_validate($form, &$form_state) {
  if ($form_state['values']['extend']) {
    if (!is_numeric($form_state['values']['last_minutes']) ||
        $form_state['values']['last_minutes'] <= 0 ||
        (int) $form_state['values']['last_minutes'] != $form_state['values']['last_minutes']) {
      form_set_error('last_minutes', t('Enter a positive integer value'));
    }
    if (!is_numeric($form_state['values']['extention']) ||
        $form_state['values']['extention'] <= 0 ||
        (int) $form_state['values']['extention'] != $form_state['values']['extention']) {
      form_set_error('extention', t('Enter a positive integer value'));
    }
  }
  if (!is_numeric($form_state['values']['bid_increment']) || $form_state['values']['bid_increment'] < 0) {
    form_set_error('bid_increment', t('Please enter a positive numerical value for Bid increment value.'));
  }
  if (!is_numeric($form_state['values']['min_bid_inc']) || $form_state['values']['min_bid_inc'] < 0) {
    form_set_error('min_bid_inc', t('Please enter a positive numerical value for Minimum bid increment value.'));
  }
  if (!is_numeric($form_state['values']['max_bid_inc']) || $form_state['values']['max_bid_inc'] < 0) {
    form_set_error('max_bid_inc', t('Please enter a positive numerical value for Maximum bid increment value.'));
  }
  if (!is_numeric($form_state['values']['max_bid_inc']) && $form_state['values']['max_bid_inc'] < $form_state['values']['min_bid_inc']) {
    form_set_error('max_bid_inc', t('Maximum bid increment value should be more than Minimum bid increment.'));
  }
  if (!is_numeric($form_state['values']['max_bid_inc_percent']) || $form_state['values']['max_bid_inc_percent'] < 0) {
    form_set_error('max_bid_inc_percent', t('Please enter a positive numerical value for Maximum bid increment percentage.'));
  }
}

/**
 * Commerce Auction administration form submit callback.
 */
function commerce_auction_admin_form_submit($form, &$form_state) {
  if ($form_state['values']['extend']) {
    variable_set('commerce_auction_extend', TRUE);
  }
  else {
    variable_set('commerce_auction_extend', FALSE);
  }
  variable_set('commerce_auction_final_period', $form_state['values']['last_minutes']);
  variable_set('commerce_auction_extension_time', $form_state['values']['extention']);

  variable_set('commerce_auction_bid_increment', $form_state['values']['bid_increment']);
  variable_set('commerce_auction_min_bid_inc', $form_state['values']['min_bid_inc']);
  variable_set('commerce_auction_max_bid_inc', $form_state['values']['max_bid_inc']);
  variable_set('commerce_auction_max_bid_inc_percent', $form_state['values']['max_bid_inc_percent']);

  $old_types = variable_get('commerce_auction_display_types', array());
  $auction_bid_ref_field = field_info_field('auction_bid_refs');
  if (!$auction_bid_ref_field) {
    field_create_field(array(
      'field_name' => 'auction_bid_refs',
      'type' => 'entityreference',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'settings' => array(
        'target_type' => 'commerce_auction_bid',
      ),
    ));
  }
  $auction_highest_bid = field_info_field('auction_highest_bid');
  if (!$auction_highest_bid) {
    field_create_field(array(
      'field_name' => 'auction_highest_bid',
      'type' => 'commerce_price',
      'cardinality' => 1,
    ));
  }

  $product_field = field_info_field('field_product');
  if (!$product_field) {
    field_create_field(array(
      'field_name' => 'field_product',
      'type' => 'commerce_product_reference',
      'cardinality' => 1,
    ));
  }
  
  $auction_starting_price = field_info_field('auction_starting_price');
  if (!$auction_starting_price) {
    field_create_field(array(
      'field_name' => 'auction_starting_price',
      'type' => 'commerce_price',
      'cardinality' => 1,
    ));
  }

  $field_names = array(
    'field_bid_increment',
    'field_min_bid_inc',
    'field_max_bid_inc',
    'field_max_bid_inc_percent',
  );
  foreach ($field_names as $field_name) {
    $field = field_info_field($field_name);
    if (!$field) {
      field_create_field(array(
        'field_name' => $field_name,
        'type' => 'number_decimal',
        'cardinality' => 1,
      ));
    }
  }

  $timeout_field = field_info_field('auction_timeout');
  if (!$timeout_field) {
    field_create_field(array(
      'field_name' => 'auction_timeout',
      'type' => 'datestamp',
      'cardinality' => 1,
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
      ),
    ));
  }

  if (!$form_state['values']['auction_displays']) {
    $form_state['values']['auction_displays'] = array();
  }

  $currency = commerce_currency_load(commerce_default_currency());
  foreach ($form_state['values']['auction_displays'] as $type) {
    if (in_array($type, $old_types) === FALSE) {
      // This content type is new, add required fields.
      field_create_instance(array(
        'field_name' => 'auction_bid_refs',
        'entity_type' => 'node',
        'bundle' => $type,
        'label' => t('Bids'),
      ));
      field_create_instance(array(
        'field_name' => 'auction_highest_bid',
        'entity_type' => 'node',
        'bundle' => $type,
        'label' => t('Highest Bid'),
        'widget' => array(
          'type' => 'readonly_widget',
        ),
      ));
      field_create_instance(array(
        'field_name' => 'auction_starting_price',
        'entity_type' => 'node',
        'bundle' => $type,
        'label' => t('Starting price'),
        'widget' => array(
          'type' => 'readonly_widget',
        ),
      ));
      field_create_instance(array(
        'field_name' => 'auction_timeout',
        'entity_type' => 'node',
        'bundle' => $type,
        'label' => t('Auction Timeout'),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'jstimer',
            'settings' => array(
              'dir' => 'down',
              'format_txt' => '',
            ),
            'type' => 'jst_timer',
          ), 
        ),
        'settings' => array(
          'default_value' => 'strtotime',
          'default_value_code' => '+5 days',
        ),
        'widget' => array(
          'settings' => array(
            'increment' => '15',
            'input_format' => 'm/d/Y - H:i:s',
            'input_format_custom' => '',
            'label_position' => 'above',
            'text_parts' => array(),
            'year_range' => '-0:+3',
          ),
          'type' => 'date_popup',
        ),
      ));
      field_create_instance(
        array(
          'field_name' => 'field_bid_increment',
          'entity_type' => 'node',
          'bundle' => $type,
          'label' => t('Bid increment'),
          'required' => TRUE,
          'default_value' => array(array('value' => 0)),
          'description' => t('(@value) Bids must be a multiple of this increment to be accepted. If your site is using a currency which does not have subunits in circulation (JPY, KRW), this should be an integer. Set to -1 to disable for this auction. Set to 0 to use global auction settings.', array('@value' => $currency['symbol'] . variable_get('commerce_auction_bid_increment', 0.25))),
          'settings' => array(
            'min' => -1,
            'prefix' => $currency['symbol'],
          ),
          'display' => array(
            'default' => array(
              'type' => 'hidden',
            ),
            'teaser' => array(
              'type' => 'hidden',
            ),
          ),
        )
      );
      field_create_instance(
        array(
          'field_name' => 'field_min_bid_inc',
          'entity_type' => 'node',
          'bundle' => $type,
          'required' => TRUE,
          'default_value' => array(array('value' => 0)),
          'label' => t('Minimum bid increase'),
          'description' => t('(@value) New bids must be at least this much higher than the current high bid to be accepted. This value should be a multiple of the Bid increment value. Set to -1 to disable for this auction. Set to 0 to use global auction settings.', array('@value' => $currency['symbol'] . variable_get('commerce_auction_min_bid_inc', 0.50))),
          'settings' => array(
            'min' => -1,
            'prefix' => $currency['symbol'],
          ),
          'display' => array(
            'default' => array(
              'type' => 'hidden',
            ),
            'teaser' => array(
              'type' => 'hidden',
            ),
          ),
        )
      );
      field_create_instance(
        array(
          'field_name' => 'field_max_bid_inc',
          'entity_type' => 'node',
          'bundle' => $type,
          'required' => TRUE,
          'default_value' => array(array('value' => 0)),
          'label' => t('Maximum bid increase'),
          'description' => t('(@value) New bids which are this much higher than the current high bid will not be accepted. Setting a maximum bid increase can make it more difficult for bidders to place a ridiculously high bid, either accidentally or otherwise. In the case that both the Maximum bid increase and Maximum bid increase percentage values are set, both limits will be enforced. Set to -1 to disable for this auction. Set to 0 to use global auction settings.', array('@value' => $currency['symbol'] . variable_get('commerce_auction_max_bid_inc', 1000))),
          'settings' => array(
            'min' => -1,
            'prefix' => $currency['symbol'],
          ),
          'display' => array(
            'default' => array(
              'type' => 'hidden',
            ),
            'teaser' => array(
              'type' => 'hidden',
            ),
          ),
        )
      );
      field_create_instance(
        array(
          'field_name' => 'field_max_bid_inc_percent',
          'entity_type' => 'node',
          'bundle' => $type,
          'required' => TRUE,
          'default_value' => array(array('value' => 0)),
          'label' => t('Maximum bid increase percentage'),
          'description' => t('(@value) Bids which increase the high bid value by this percentage will not be accepted. For example, if this value is set to 50% and an item’s current high bid is ¤1000, bids larger than ¤1500 will not be accepted (as ¤500 is 50% of ¤1000). Setting a maximum bid increase percentage can make it more difficult for bidders to place a ridiculously high bid, either accidentally or otherwise. Enter zero or leave blank to not enforce this limit. In the case that both the Maximum bid increase and Maximum bid increase percentage values are set, both limits will be enforced. Set to -1 to disable for this auction. Set to 0 to use global auction settings.', array('@value' => variable_get('commerce_auction_max_bid_inc_percent', 100))),
          'settings' => array(
            'min' => -1,
            'suffix' => '%',
          ),
          'display' => array(
            'default' => array(
              'type' => 'hidden',
            ),
            'teaser' => array(
              'type' => 'hidden',
            ),
          ),
        )
      );
      $instance = field_info_instance('node', 'field_product', $type);
      if (!$instance) {
        field_create_instance(array(
          'field_name' => 'field_product',
          'entity_type' => 'node',
          'bundle' => $type,
          'label' => t('Product'),
        ));
      }
    }
  }

  commerce_auction_remove_fields($old_types, $form_state['values']['auction_displays']);
  variable_set('commerce_auction_display_types',
                $form_state['values']['auction_displays']);
  drupal_set_message(t('Auction settings are updated.'));
}
