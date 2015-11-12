<?php

/**
 * @file
 * Contains administration form callbacks of commerce_reverse_auction.
 */

/**
 * Page callback of commerce_reverse_auction configuration form.
 */
function commerce_reverse_auction_admin_form($form, &$form_state) {
  $types = node_type_get_types();
  $options = array();
  $default_types = variable_get('commerce_reverse_auction_display_types', array());
  foreach ($types as $type => $obj) {
    $options[$type] = $obj->name;
  }

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('You can configure behavior of the commerce auction module in this page.') . '<br />' . t('Select Auction product display content types and adjust timeout settings.'),
  );

  $form['auction_displays'] = array(
    '#title' => t('Reverse auction display content types'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $options,
    '#default_value' => $default_types,
  );

  $form['decreasing_bid_amount'] = array(
    '#title' => t('Decreasing bids'),
    '#type' => 'checkbox',
    '#description' => t('Bid amounts should be decreasing'),
    '#default_value' => variable_get('commerce_reverse_auction_decreasing_amount', TRUE),
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
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Commerce Auction administration form validation callback.
 */
function commerce_reverse_auction_admin_form_validate($form, &$form_state) {
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
}

/**
 * Commerce Auction administration form submit callback.
 */
function commerce_reverse_auction_admin_form_submit($form, &$form_state) {
  if ($form_state['values']['extend']) {
    variable_set('commerce_reverse_auction_extend', TRUE);
  }
  else {
    variable_set('commerce_reverse_auction_extend', FALSE);
  }
  variable_set('commerce_reverse_auction_final_period', $form_state['values']['last_minutes']);
  variable_set('commerce_reverse_auction_extension_time', $form_state['values']['extention']);
  variable_set('commerce_reverse_auction_decreasing_amount', $form_state['values']['decreasing_bid_amount']);

  $product_field = field_info_field('field_product');
  if (!$product_field) {
    field_create_field(array(
      'field_name' => 'field_product',
      'type' => 'commerce_product_reference',
      'cardinality' => 1,
    ));
  }

  $auction_min_price = field_info_field('auction_min_price');
  if (!$auction_min_price) {
    field_create_field(array(
      'field_name' => 'auction_min_price',
      'type' => 'commerce_price',
      'cardinality' => 1,
    ));
  }
  $auction_max_price = field_info_field('auction_max_price');
  if (!$auction_max_price) {
    field_create_field(array(
      'field_name' => 'auction_max_price',
      'type' => 'commerce_price',
      'cardinality' => 1,
    ));
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
  $old_types = variable_get('commerce_reverse_auction_display_types', array());

  foreach ($form_state['values']['auction_displays'] as $type) {
    if (in_array($type, $old_types) === FALSE) {
      // This content type is new, add required fields.
      field_create_instance(array(
        'field_name' => 'auction_min_price',
        'entity_type' => 'node',
        'bundle' => $type,
        'label' => t('Minimum price'),
      ));
      field_create_instance(array(
        'field_name' => 'auction_max_price',
        'entity_type' => 'node',
        'bundle' => $type,
        'label' => t('Miximum price'),
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
  commerce_reverse_auction_remove_fields($old_types, $form_state['values']['auction_displays']);
  variable_set('commerce_reverse_auction_display_types',
                $form_state['values']['auction_displays']);
  drupal_set_message(t('Reverse auction settings are updated.'));
}

