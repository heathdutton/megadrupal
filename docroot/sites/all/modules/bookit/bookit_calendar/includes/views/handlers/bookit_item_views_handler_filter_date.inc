<?php

/**
 * Filter by availability date.
 */
class bookit_item_views_handler_filter_date extends date_views_filter_handler_simple {

  function option_definition() {
    $options = parent::option_definition();
    $options['booking_group'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $options = array('' => t('- None -'));
    foreach (commerce_info_fields('entityreference') as $field_name => $field) {
      if ($field['settings']['target_type'] == 'bookit_item') {
        foreach ($field['bundles'] as $entity_type => $bundles) {
          $info = entity_get_info($entity_type);
          foreach ($bundles as $bundle) {
            $options[$info['label']][$entity_type . '_' . $bundle] = $info['bundles'][$bundle]['label'];
          }
        }
      }
    }

    $form['booking_group'] = array(
      '#title' => t('Booking Group'),
      '#description' => t('Select a booking group in order to use the stored booking dates from another view as default value in this view.'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->options['booking_group'],
      '#weight' => -499,
    );

    parent::options_form($form, $form_state);
  }

  function expose_options() {
    parent::expose_options();
    $this->options['expose']['remember'] = TRUE;
    $this->options['expose']['remember_roles'] = array(
      DRUPAL_ANONYMOUS_RID => DRUPAL_ANONYMOUS_RID,
      DRUPAL_AUTHENTICATED_RID => DRUPAL_AUTHENTICATED_RID,
    );
  }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['expose']['remember']['#access'] = FALSE;
    $form['expose']['remember_roles']['#access'] = FALSE;
  }

  function store_exposed_input($input, $status) {
    if (empty($this->options['exposed']) || empty($this->options['expose']['identifier'])) {
      return TRUE;
    }

    if (empty($this->options['booking_group'])) {
      return parent::store_exposed_input($input, $status);
    }

    $booking_group = $this->options['booking_group'];

    // FALSE means that we got a setting that means to recuse ourselves,
    // so we should erase whatever happened to be there.
    if (!$status && isset($_SESSION['booking_date'][$booking_group])) {
      unset($_SESSION['booking_date'][$booking_group]);
    }

    // Store the booking date.
    if ($status) {
      $_SESSION['booking_date'][$booking_group] = $input[$this->options['expose']['identifier']];
    }
  }

  function date_default_value($prefix, $options = NULL) {
    // Get booking group for this filter.
    $booking_group = $this->options['booking_group'];

    // Get default value from SESSION if exists.
    if (!empty($booking_group) and isset($_SESSION['booking_date'][$booking_group][$prefix])) {
      return $_SESSION['booking_date'][$booking_group][$prefix];
    }

    return parent::date_default_value($prefix, $options);
  }

  function operators() {
    return array(
      '=' => array(
        'title' => t('Is equal to'),
        'method' => 'op_simple',
        'short' => t('='),
        'values' => 1,
      ),
      'between' => array(
        'title' => t('Is between'),
        'method' => 'op_between',
        'short' => t('between'),
        'values' => 2,
      ),
    );
  }

  function op_simple($field) {
    if(empty($this->value['value'])) {
      return;
    }

    $subquery = bookit_calendar_not_bookable_query($this->value['value']);
    $this->query->add_where($this->options['group'], "$this->table_alias.item_id", $subquery, 'NOT IN');
  }

  function op_between($field) {
    if(empty($this->value['min']) or empty($this->value['max'])) {
      return;
    }

    $datetime = strtotime($this->value['min'] . ' 00:00:00');
    $end_date = strtotime($this->value['max'] . ' 00:00:00');

    // If datetimes are not in the correct order then throw error.
    if($datetime > $end_date) {
      return $this->query->add_where_expression($this->options['group'], '1 = 2');
    }

    $subquery = bookit_calendar_not_bookable_query($this->value['min'], $this->value['max']);
    $this->query->add_where($this->options['group'], "$this->table_alias.item_id", $subquery, 'NOT IN');
  }
}
