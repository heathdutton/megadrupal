<?php

/**
 * @file
 * Field API integration for the commerce_price_history module.
 */

/**
 * Implements hook_field_info().
 */
function commerce_price_history_field_info() {
  return array(
    'commerce_price_history' => array(
      'label' => t('Price history'),
      'description' => t('This field stores prices revisions.'),
      'settings' => array(),
      'instance_settings' => array('attached_field' => ''),
      'default_widget' => 'field_extrawidgets_hidden',
      'default_formatter' => 'commerce_price_history_chart',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 *
 * hook_field_is_emtpy() is where Drupal asks us if this field is empty.
 * Return TRUE if it does not contain data, FALSE if it does. This lets
 * the form API flag an error when required fields are empty.
 */
function commerce_price_history_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function commerce_price_history_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  $form = array();

  // Build an options array of product price fields.
  $options = array();
  $commerce_product_types = commerce_product_type_get_name();

  // Get the list of fields of type 'commerce_price'.
  $commerce_price_fields = _commerce_price_history_get_fields_by_type('commerce_price');

  $instances = field_info_instances('commerce_product');
  foreach ($instances as $bundle_name => $bundle) {
    foreach (array_intersect_key($bundle, $commerce_price_fields) as $field_name => $instance) {
      $options[$commerce_product_types[$bundle_name]]["$bundle_name:$field_name"] = $instance['label'];
    }
  }

  $form['attached_field'] = array(
    '#type' => 'select',
    '#title' => t('Attached Product Price field'),
    '#description' => t('A Product Price History field needs to be attached to a Product Price field.'),
    '#options' => $options,
    '#default_value' => !empty($settings['attached_field']) ? $settings['attached_field'] : '',
    '#required' => TRUE,
    '#disabled' => !empty($settings['attached_field']) ? TRUE : FALSE,
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function commerce_price_history_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {
  if ($form['#field']['type'] == 'commerce_price_history') {
    // Hide the cardinality setting on the field settings for
    // commerce_price_history fields.
    $form['field']['cardinality']['#default_value'] = 1;
    $form['field']['#access'] = FALSE;
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function commerce_price_history_field_formatter_info() {
  return array(
    'commerce_price_history_chart' => array(
      'label' => t('Product price history chart'),
      'field types' => array('commerce_price_history'),
      'settings' => array(
        'chart_interval' => '30',
        'chart_data_points' => '11',
        'chart_width' => '400',
        'chart_height' => '200',
        'chart_date_format' => 'M d',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function commerce_price_history_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element['chart_interval'] = array(
    '#type' => 'select',
    '#title' => t('Chart interval'),
    '#options' => array(
      '5' => '5',
      '10' => '10',
      '15' => '15',
      '30' => '30',
      '60' => '60',
      '90' => '90',
    ),
    '#default_value' => $settings['chart_interval'],
    '#description' => t('How many days to show in the price history chart.'),
    '#field_suffix' => t('days'),
  );
  $element['chart_data_points'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart data points'),
    '#default_value' => $settings['chart_data_points'],
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('How many data points to show in the chart. Can not be higher than <em>Chart interval</em>.'),
  );
  $element['chart_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart width'),
    '#default_value' => $settings['chart_width'],
    '#size' => 4,
    '#maxlength' => 4,
    '#field_suffix' => t('px'),
  );
  $element['chart_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart height'),
    '#default_value' => $settings['chart_height'],
    '#size' => 4,
    '#maxlength' => 4,
    '#field_suffix' => t('px'),
  );
  $element['chart_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart date format'),
    '#default_value' => $settings['chart_date_format'],
    '#size' => 10,
    '#description' => t("Define a php date format string like 'm d Y' (see <a href=\"@link\">http://php.net/date</a> for more details).", array('@link' => 'http://php.net/date')),
  );
  $element['#element_validate'][] = '_commerce_price_history_field_formatter_settings_form_validate';

  return $element;
}

/**
 * Element validate callback: ensure valid data is entered.
 */
function _commerce_price_history_field_formatter_settings_form_validate($element, &$form_state, $form) {
  // Validate the chart_data_points element.
  if (!is_numeric($element['chart_data_points']['#value']) || $element['chart_data_points']['#value'] <= 0) {
    form_set_error(implode('][', $element['chart_data_points']['#parents']), t('You must enter a positive numeric chart data points value.'));
  }
  if ($element['chart_data_points']['#value'] > $element['chart_interval']['#value']) {
    form_set_error(implode('][', $element['chart_data_points']['#parents']), t('Chart data points can not be higher than the chart interval (@chart_interval).', array('@chart_interval' => $element['chart_interval']['#value'])));
  }

  // Validate the chart_width element.
  if (!is_numeric($element['chart_width']['#value']) || $element['chart_width']['#value'] <= 0) {
    form_set_error(implode('][', $element['chart_width']['#parents']), t('You must enter a positive numeric chart width value.'));
  }

  // Validate the chart_height element.
  if (!is_numeric($element['chart_height']['#value']) || $element['chart_height']['#value'] <= 0) {
    form_set_error(implode('][', $element['chart_height']['#parents']), t('You must enter a positive numeric chart height value.'));
  }
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function commerce_price_history_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  if ($display['type'] == 'commerce_price_history_chart') {
    $summary[] = t('Chart interval: @chart_interval days', array('@chart_interval' => $settings['chart_interval']));
    $summary[] = t('Chart data points: @chart_data_points', array('@chart_data_points' => $settings['chart_data_points']));
    $summary[] = t('Chart width: @chart_width px', array('@chart_width' => $settings['chart_width']));
    $summary[] = t('Chart height: @chart_height px', array('@chart_height' => $settings['chart_height']));
    $summary[] = t('Chart date format: @chart_date_format', array('@chart_date_format' => $settings['chart_date_format']));
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function commerce_price_history_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // Populate the $items array to keep hook_field_formatter_view() happy.
  foreach ($entities as $entity_id => $entity) {
    $items[$entity_id][0] = '';
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function commerce_price_history_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $price_history = array();
  list($entity_id) = entity_extract_ids($entity_type, $entity);

  if (empty($instance['settings']['attached_field'])) {
    drupal_set_message(t('%label is not attached to any commerce_price field. Fix this by <a href="@url">configuring</a> the %label field.', array('%label' => $instance['label'], '@url' => url('admin/commerce/products/types/product/fields/' . $instance['field_name']))), 'warning');
  }
  else {
    $price_history = commerce_price_history_load($instance['settings']['attached_field'], $entity_id, $display['settings']['chart_interval']);

    // If we don't have any data for this field, get the current price and
    // save it as the first price history data.
    if (empty($price_history)) {
      $field = explode(':', $instance['settings']['attached_field']);
      $field_items = field_get_items($entity_type, $entity, $field[1]);

      if (!empty($field_items)) {
        $initial_price = reset($field_items);
        $initial_price_data = new stdClass();
        $initial_price_data->amount = $initial_price['amount'];
        $initial_price_data->currency_code = $initial_price['currency_code'];
        $initial_price_data->timestamp = REQUEST_TIME - ($display['settings']['chart_interval'] * 86400);
        $price_history[] = $initial_price_data;
      }
    }

    if (!empty($price_history)) {
      $chart_data = _commerce_price_history_generate_data($price_history, $display['settings']['chart_interval'], $display['settings']['chart_data_points']);

      $element[0] = array(
        '#theme' => 'commerce_price_history_chart',
        '#product_id' => $entity_id,
        '#chart_data' => $chart_data,
        '#chart_title' => $instance['label'],
        '#chart_date_format' => $display['settings']['chart_date_format'],
        '#chart_width' => $display['settings']['chart_width'],
        '#chart_height' => $display['settings']['chart_height'],
      );
    }
  }

  return $element;
}

/**
 * Implements hook_field_purge_field_instance().
 */
function commerce_price_history_field_purge_instance($instance) {
  // When a price history field is deleted, remove it's data from the
  // commerce_price_history table.
  if ($instance['entity_type'] == 'commerce_product' && isset($instance['settings']['attached_field'])) {
    $attached_field = $instance['settings']['attached_field'];
    db_delete('commerce_price_history')
      ->condition('attached_field', $attached_field)
      ->execute();
  }
}
