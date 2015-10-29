<?php
// $Id$

/**
 * @file
 * Coupon order total field handler
 */
class uc_coupon_handler_field_all_orders_total extends uc_order_handler_field_money_amount {
  /**
   * Add 'cid' field.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['cid'] = 'cid';
  }

  /**
   * Define option default values.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['statuses'] = array('default' => array());
    return $options;
  }

  /**
   * Add option for order status.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $statuses = array();
    foreach (uc_order_status_list() as $status) {
      $statuses[$status['id']] = $status['title'];
    }

    $form['statuses'] = array(
      '#title' => t('Order statuses'),
      '#type' => 'select',
      '#description' => t('Only orders with selected statuses will be counted.
          If you do not select any statuses, then all orders will be counted.')
          . '<br />' . t('Hold Ctrl + click to select multiple statuses.'),
      '#options' => $statuses,
      '#default_value' => $this->options['statuses'],
      '#multiple' => TRUE,
      '#size' => 8,
    );
  }

  /**
   * Here we add the aggregate field that will sum the orders.
   */
  function query() {
    $this->ensure_my_table();
    $uco = $this->query->ensure_table('uc_coupons_orders');
    $uo = $this->query->ensure_table('uc_orders');
    $this->add_additional_fields();
    if (empty($this->options['statuses'])) {
      // If no status specified, then we show all.
      $in_status = "1";
    }
    else {
      // An array of statuses was specified.
      $in_status =  "$uo.order_status IN('" . implode("', '", $this->options['statuses']) . "')";
    }

    $this->field_alias = $this->query->add_field(
      NULL,
      "CASE WHEN $in_status THEN $uo.order_total ELSE 0 END",
      $this->table_alias . '_' . $this->field . '_' . implode('_', $this->options['statuses']),
      array('function' => 'sum')
    );
  }

}

