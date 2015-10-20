<?php
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Style plugin for Expeditor INET export format.
 *
 * @ingroup views_style_plugins
 */
class commerce_socolissimo_export_expeditor_inet extends views_data_export_plugin_style_export {

  /**
   * Set options fields and default values.
   *
   * @return array
   *   An array of options information.
   */
  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

  /**
   * Options form mini callback.
   *
   * @param array $form
   *   Form array to add additional fields to.
   * @param array $form_state
   *   State of the form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  /**
   * Render the display in this style.
   */
  function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');

      return;
    }

    $rows = $this->render_body();

    return theme($this->theme_functions($this->definition['additional themes base']), array('rows' => $rows));
  }

  /**
   * Render the body of the file export.
   *
   * @return array|string
   *   The rows array.
   */
  function render_body() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('commerce_socolissimo_export_expeditor_inet: Missing row plugin');

      return;
    }
    if (_commerce_socolissimo_convert_from_utf8('test', "WINDOWS-1252") === FALSE) {
      vpr('commerce_socolissimo_export_expeditor_inet: Please install iconv, GNU recode or mbstring for PHP.');
      return;
    }

    // Get the view results.
    $records = $this->view->result;

    $order_ids = array();
    foreach ($records as $record) {
      // We always take the first element (order_id)
      $order_ids[] = reset($record);
    }

    $rows = array();
    if (!empty($order_ids)) {
      $rows = $this->generate_rows(commerce_order_load_multiple($order_ids));
    }

    return $rows;
  }

  /**
   * Generate rows for the style plugin.
   *
   * @param array $orders
   *   An array of fully loaded order.
   *
   * @return array
   *   A list of rows to be shown.
   */
  function generate_rows($orders) {
    $rows = array();
    $i = 1;
    foreach ($orders as $order) {
      $wrapper_order = entity_metadata_wrapper('commerce_order', $order);

      $shipping_line_item = _commerce_socolissimo_get_shipping_line_item($order);
      $shipping_code = '';
      if ($shipping_line_item) {
        $shipping_code = $shipping_line_item->data['service_details']['commerce_socolissimo_point']['typeDePoint'];
      }

      $socolissimo_api = variable_get('commerce_socolissimo_api', array());
      if (!empty($socolissimo_api['username'])) {
        $trader_company_name = $socolissimo_api['username'];
      }
      else {
        $error = t('So Colissimo API not configured. Go to !config_page to configure it.', array('!config_page' => l(t('So Colissimo configuration'), 'admin/commerce/config/socolissimo-flexibility')));
        drupal_set_message($error, 'error');
        $trader_company_name = '';
      }
      $shipping = $wrapper_order->commerce_customer_shipping->commerce_customer_address->value();
      $billing = $wrapper_order->commerce_customer_billing->commerce_customer_address->value();
      $weight = commerce_physical_order_weight($order, 'g');

      if (in_array($shipping_code, array('RDV', 'DOM', 'DOS'))) {
        $shipping_address = $shipping['thoroughfare'];
        $zip_code = $shipping['postal_code'];
        $locality = $shipping['locality'];
      }
      else {
        $shipping_address = $billing['thoroughfare'];
        $zip_code = $billing['postal_code'];
        $locality = $billing['locality'];
      }

      if (!empty($billing['first_name']) && !empty($billing['last_name'])) {
        $first_name = $billing['first_name'];
        $last_name = $billing['last_name'];
      }
      else {
        $first_name = $last_name = $billing['name_line'];
      }

      $phone_number = $shipping['phone_number'];

      // Record type.
      $rows[$i][0] = array(
        'value' => 'EXP',
        'position' => 0,
        'length' => 3,
      );

      // Shipping service code (DOM, DOS,RDV, BPR, CIT, ACP, A2P, MRL, CDI).
      $rows[$i][3] = array(
        'value' => $this->format_value($shipping_code, 4),
        'position' => 46,
        'length' => 4,
      );

      // User name line.
      $rows[$i][6] = array(
        'value' => $this->format_value($last_name, 35),
        'position' => 73,
        'length' => 35,
      );

      // Shipping address.
      $rows[$i][7] = array(
        'value' => $this->format_value($shipping_address, 35),
        'position' => 108,
        'length' => 35,
      );

      // Postal code.
      $rows[$i][8] = array(
        'value' => $this->format_value($zip_code, 35),
        'position' => 213,
        'length' => 9,
      );

      // Locality.
      $rows[$i][9] = array(
        'value' => $this->format_value($locality, 35),
        'position' => 222,
        'length' => 35,
      );

      if (isset($weight['weight'])) {
        // Weight.
        $rows[$i][15] = array(
          'value' => $this->format_value($weight['weight'], 15),
          'position' => 342,
          'length' => 15,
        );
      }
      // Customer mail address.
      $rows[$i][26] = array(
        'value' => $this->format_value($wrapper_order->mail->value(), 35),
        'position' => 444,
        'length' => 75,
      );

      // Customer firstname.
      $rows[$i][39] = array(
        'value' => $this->format_value($first_name, 29),
        'position' => 778,
        'length' => 29,
      );

      // Customer cellphone number.
      $rows[$i][41] = array(
        'value' => $this->format_value($phone_number, 10),
        'position' => 845,
        'length' => 10,
      );

      // Pick up code.
      if (in_array(
        $shipping_code, array(
          'BPR',
          'ACP',
          'CIT',
          'CDI',
          'A2P',
          'MRL',
        )
      )
      ) {
        $rows[$i][45] = array(
          'value' => $this->format_value($shipping_line_item->data['service_details']['commerce_socolissimo_point']['identifiant'], 6),
          'position' => 901,
          'length' => 6,
        );
      }

      // Compagny name.
      $rows[$i][47] = array(
        'value' => $this->format_value($trader_company_name, 38),
        'position' => 908,
        'length' => 38,
      );

      $i++;
    }

    return $rows;
  }

  /**
   * Add any HTTP headers that this style plugin wants to.
   */
  function add_http_headers() {
    $view = & $this->view;

    drupal_add_http_header('Cache-Control', 'max-age=60, must-revalidate');

    if (!empty($this->definition['export headers'])) {
      foreach ($this->definition['export headers'] as $name => $value) {
        drupal_add_http_header($name, $value);
      }
    }

    if (isset($this->options['filename']) && !empty($this->options['provide_file'])) {
      // General tokens.
      $tokens = array(
        '%view' => check_plain($view->name),
        '%display' => check_plain($view->current_display),
      );
      // Argument tokens.
      $count = 0;
      foreach ($view->display_handler->get_handlers('argument') as $handler) {
        $token = '%' . ++$count;
        $tokens[$token . '-title'] = check_plain($handler->title());
        $tokens[$token . '-value'] = isset($view->args[$count - 1]) ? check_plain($view->args[$count - 1]) : '';
      }

      // Effective exposed filters token.
      $exposed = array();
      foreach ($view->display_handler->get_handlers('filter') as $handler) {
        if (!$handler->options['exposed']) {
          continue;
        }
        if (!empty($view->exposed_input[$handler->options['expose']['identifier']]) && !is_array($view->exposed_input[$handler->options['expose']['identifier']])) {
          $exposed[] = check_plain($handler->options['expose']['identifier']) . '_' . check_plain($view->exposed_input[$handler->options['expose']['identifier']]);
        }
      }
      if (!empty($exposed)) {
        $tokens['%exposed'] = implode('-', $exposed);
      }
      else {
        $tokens['%exposed'] = 'default';
      }

      // Timestamp token.
      $time = REQUEST_TIME;
      $parts = array(
        'full' => 'Y-m-d\TH-i-s',
        'yy' => 'y',
        'yyyy' => 'Y',
        'mm' => 'm',
        'mmm' => 'M',
        'dd' => 'd',
        'ddd' => 'D',
        'hh' => 'H',
        'ii' => 'i',
        'ss' => 's',
      );
      foreach ($parts as $part => $format) {
        $tokens['%timestamp-' . $part] = format_date($time, 'custom', $format);
      }

      $filename = strtr($this->options['filename'], $tokens);

      if ($filename) {
        drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $filename . '"');
      }
    }
  }

  /**
   * Format a value.
   *
   * Depending of the value length, adds whitespaces to the passed value.
   *
   * @param string $value
   *   The value to format.
   * @param int $length
   *   The value length.
   *
   * @return string
   *   The value formatted.
   */
  function format_value($value, $length) {
    $value = trim($value);
    $value_length = strlen($value);

    if ($value_length != $length && $value_length < $length) {
      $value .= str_repeat(' ', ($length - $value_length));
    }

    return $value;
  }
}
