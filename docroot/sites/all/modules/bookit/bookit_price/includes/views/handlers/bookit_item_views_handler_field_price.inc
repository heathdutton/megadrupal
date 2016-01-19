<?php

class bookit_item_views_handler_field_price extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $field_type = field_info_field_types('commerce_price');
    $options['type'] = array(
      'default' => $field_type['default_formatter'],
    );

    $options['settings'] = array(
      'default' => array(),
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    module_load_include('inc', 'views', 'modules/field/views_handler_field_field');
    $formatters = _field_view_formatter_options('commerce_price');

    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Formatter'),
      '#options' => $formatters,
      '#default_value' => $this->options['type'],
      '#ajax' => array(
        'path' => views_ui_build_form_url($form_state),
      ),
      '#submit' => array('views_ui_config_item_form_submit_temporary'),
      '#executes_submit_callback' => TRUE,
    );

    // Get the currently selected formatter.
    $format = $this->options['type'];

    $formatter = field_info_formatter_types($format);

    // Get the settings form.
    $settings_form = array('#value' => array());
    $function = $formatter['module'] . '_field_formatter_settings_form';
    if (function_exists($function)) {
      $fake_field = array();
      $fake_instance = array(
        'display' => array(
          'default' => array(
            'type' => $format,
            'settings' => $this->options['settings'],
          ),
        ),
      );
      $settings_form = $function($fake_field, $fake_instance, 'default', $form, $form_state);
    }
    $form['settings'] = $settings_form;
  }

  function render($values) {
    if ($amount = $this->get_value($values)) {
      $formatter = field_info_formatter_types($this->options['type']);

      // Create price wrapper.
      $price = array(
        array(
          'amount' => $amount,
          'currency_code' => commerce_default_currency(),
          'data' => array(),
        ),
      );

      $function = $formatter['module'] . '_field_formatter_view';
      if (function_exists($function)) {
        $elements = $function(NULL, NULL, NULL, NULL, LANGUAGE_NONE, $price, array(
          'type' => $this->options['type'],
          'settings' => $this->options['settings'],
          'label' => 'hidden',
          'views_view' => $this->view,
          'views_field' => $this,
        ));

        return drupal_render($elements);
      }
    }

    return FALSE;
  }

  function query() {
    $this->ensure_my_table();

    if ($booking_date = bookit_calendar_date_by_view($this->view)) {
      // Build the join subquery.
      $subquery = db_select('bookit_price', 'p');
      $subquery->fields('p', array('item_id'));
      $subquery->addExpression('SUM(p.amount)', 'amount');
      $subquery->groupBy('p.item_id');

      if (isset($booking_date['min'])) {
        $subquery->condition('p.date', $booking_date['min'], '>=');
        $subquery->condition('p.date', $booking_date['max'], '<');
      }
      else {
        $subquery->condition('p.date', $booking_date['value']);
      }

      $join = new views_join();
      $join->definition = array(
        'type' => 'INNER',
        'table' => '',
        'table formula' => $subquery,
        'field' => 'item_id',
        'left_table' => $this->table_alias,
        'left_field' => 'item_id',
      );
      $join->construct();
      $join->adjusted = TRUE;
      $this->table_alias = $this->query->ensure_table('bookit_price', $this->relationship, $join);
      $this->real_field = 'amount';
      $this->options['group_type'] = 'min';
      return parent::query();
    }
  }
}
