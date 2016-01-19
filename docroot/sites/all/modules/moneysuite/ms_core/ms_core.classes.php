<?php

/**
 * @file
 * Contains the MS Core classes.
 *
 * Original author: Leighton Whiting - Released under GENERAL PUBLIC LICENSE 
 * Current maintenance by multiple MoneySuite users.
 * Re: new initiative: https://www.drupal.org/node/2315653 
 */

/**
 * MsOrder class
 *
 * @author Leighton
 */
class MsOrder
{
  public $oid = 0;
  public $uid = 0;
  public $order_key = '';
  public $order_type = 'cart';
  public $status = 'checkout';
  public $gateway = '';
  public $amount = 0;
  public $total = 0;
  public $currency;
  public $title = '';
  public $order_number = '';
  public $recurring_schedule = array(
    'total_occurrences' => 0,
    'main_amount' => 0,
    'main_length' => 0,
    'main_unit' => '',
    'has_trial' => FALSE,
    'trial_amount' => 0,
    'trial_length' => 0,
    'trial_unit' => '',
  );
  public $first_name = '';
  public $last_name = '';
  public $email_address = '';
  public $shipping_address = array(
    'first_name' => '',
    'last_name' => '',
    'street' => '',
    'street2' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
    );
  public $billing_address = array(
    'street' => '',
    'street2' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
    );
  public $data = array(
    'skip_registration' => FALSE,
    'blocked_gateways' => array(),
  );
  public $products = array();
  public $payments = array();
  public $adjustments = array();
  public $history = array();
  public $created;
  public $modified;
  public $unique_key;

  /**
   * Constructor - set some additional defaults.
   */
  function __construct() {
    // Set the default variables.
    $this->created = time();
    $this->modified = time();
    $this->currency = variable_get('ms_core_default_currency', 'USD');
    $this->order_key = ms_core_generate_order_key($this);
    $this->unique_key = drupal_get_token(serialize($this));
  }

  /**
   * Loads an order from an oid.
   *
   * @param int $oid
   *   The order id of the order to load.
   */
  function load($oid) {
    // Load the order from the database.
    $query = db_select('ms_orders')
        ->fields('ms_orders')
        ->condition('oid', $oid)
        ->execute();

    if (!$row = $query->fetchObject()) {
      return FALSE;
    }

    // Set the values.
    $this->oid = $row->oid;
    $this->uid = $row->uid;
    $this->order_type = $row->order_type;
    $this->status = $row->status;
    $this->gateway = $row->gateway;
    $this->amount = round($row->amount, 2);
    $this->total = $row->total;
    $this->currency = $row->currency;
    $this->first_name = $row->first_name;
    $this->last_name = $row->last_name;
    $this->email_address = $row->email_address;
    $this->created = $row->created;
    $this->modified = $row->modified;
    $this->unique_key = $row->unique_key;
    $this->order_key = $row->order_key;

    // Get the Products for the Order.
    $this->products = ms_core_get_order_products($row->oid);

    // Get the Adjustments for the Order.
    $this->adjustments = ms_core_get_order_adjustments($this);

    // Get the Payments for the Order.
    $this->payments = ms_core_get_order_payments($row->oid);

    // Get the History for the Order.
    $this->history = ms_core_get_order_history($row->oid);

    $this->recurring_schedule = unserialize($row->recurring_schedule);

    // Set the defaults.
    if (!isset($this->recurring_schedule['trial_length'])) {$this->recurring_schedule['trial_length'] = '';}
    if (!isset($this->recurring_schedule['trial_unit'])) {$this->recurring_schedule['trial_unit'] = '';}
    if (!isset($this->recurring_schedule['trial_amount'])) {$this->recurring_schedule['trial_amount'] = '';}
    if (!isset($this->recurring_schedule['has_trial'])) {$this->recurring_schedule['has_trial'] = FALSE;}
    if (!isset($this->recurring_schedule['main_length'])) {$this->recurring_schedule['main_length'] = '';}
    if (!isset($this->recurring_schedule['main_unit'])) {$this->recurring_schedule['main_unit'] = '';}
    if (!isset($this->recurring_schedule['main_amount'])) {$this->recurring_schedule['main_amount'] = '';}

    // Apply adjustments to the recurring schedule main amount.
    if ($this->recurring_schedule['trial_length'] AND $this->recurring_schedule['trial_unit']) {
      $this->recurring_schedule['trial_amount'] = round(ms_core_get_product_adjusted_price($this->recurring_schedule['trial_amount'], $this->adjustments, 'all'), 2);
    }
    else {
      $this->recurring_schedule['main_amount'] = round(ms_core_get_product_adjusted_price($this->recurring_schedule['main_amount'], $this->adjustments, 'recurring'), 2);
    }

    $this->shipping_address = unserialize($row->shipping_address);
    $this->billing_address = unserialize($row->billing_address);

    // Set the defaults.
    if (!isset($this->billing_address['street'])) {$this->billing_address['street'] = '';}
    if (!isset($this->billing_address['city'])) {$this->billing_address['city'] = '';}
    if (!isset($this->billing_address['state'])) {$this->billing_address['state'] = '';}
    if (!isset($this->billing_address['zip'])) {$this->billing_address['zip'] = '';}
    if (!isset($this->billing_address['country'])) {$this->billing_address['country'] = '';}
    if (!isset($this->billing_address['phone'])) {$this->billing_address['phone'] = '';}

    if (!isset($this->shipping_address['first_name'])) {$this->shipping_address['first_name'] = '';}
    if (!isset($this->shipping_address['last_name'])) {$this->shipping_address['last_name'] = '';}
    if (!isset($this->shipping_address['street'])) {$this->shipping_address['street'] = '';}
    if (!isset($this->shipping_address['street2'])) {$this->shipping_address['street2'] = '';}
    if (!isset($this->shipping_address['city'])) {$this->shipping_address['city'] = '';}
    if (!isset($this->shipping_address['state'])) {$this->shipping_address['state'] = '';}
    if (!isset($this->shipping_address['zip'])) {$this->shipping_address['zip'] = '';}
    if (!isset($this->shipping_address['country'])) {$this->shipping_address['country'] = '';}
    if (!isset($this->shipping_address['phone'])) {$this->shipping_address['phone'] = '';}

    $this->data = unserialize($row->data);

    $this->title = ms_core_get_order_title($this, 127);
    $this->order_number = ms_core_order_number($this);

    // Calculate the totals.
    $this->calculate_total();
    return TRUE;
  }

  /**
   * Saves the MsOrder object to the database.
   *
   * @return
   *   Returns TRUE if the operation is successful, FALSE otherwise.
   */
  function save() {
    // Calculate the totals.
    $this->calculate_total();

    if ($this->oid > 0) {
      // Update into the database.
      if (drupal_write_record('ms_orders', $this, 'oid')) {
        return TRUE;
      }
      return FALSE;
    }
    else {
      // Insert into Database.
      if (drupal_write_record('ms_orders', $this)) {
        return TRUE;
      }
      return FALSE;
    }
  }

  /**
   * Calculates and sets the correct total based on the products.
   */
  function calculate_total() {
    // Set the price and total again.
    $this->amount = 0.00;
    $this->total = ms_core_get_order_total($this);

    switch ($this->order_type) {
      case 'multi_recurring':
      case 'recurring':
        foreach ($this->products as $product) {
          $this->recurring_schedule = $product->recurring_schedule;
          if ($product->recurring_schedule['has_trial']) {
            // Calculate the amount that should be paid for the initial payment
            // from the trial_amount.
            $this->recurring_schedule['trial_amount'] = round(ms_core_get_product_adjusted_price($product->recurring_schedule['trial_amount'], $this->adjustments, 'all'), 2);
          }
          else {
            // Set the trial length to the same as the main length.
            $this->recurring_schedule['trial_length'] = $this->recurring_schedule['main_length'];
            $this->recurring_schedule['trial_unit'] = $this->recurring_schedule['main_unit'];
            // Calculate the amount that should be paid for the initial payment
            // from the main_amount.
            $this->recurring_schedule['trial_amount'] = round(ms_core_get_product_adjusted_price($product->recurring_schedule['main_amount'], $this->adjustments, 'all'), 2);
          }
          $this->recurring_schedule['main_amount'] = round(ms_core_get_product_adjusted_price($product->recurring_schedule['main_amount'], $this->adjustments, 'recurring'), 2);

          // If the initial payment is the same as the regular payment, don't do
          // a trial.
          if ($this->recurring_schedule['main_amount'] == round($this->recurring_schedule['trial_amount'], 3)) {
            $this->recurring_schedule['has_trial'] = FALSE;
          }
          else {
            $this->recurring_schedule['has_trial'] = TRUE;
          }

          // Set the order amount to be the initial payment amount.
          $this->amount += round($this->recurring_schedule['trial_amount'], 2);
        }
        break;

      case 'cart':
        // Just set the amount to what it should be for the initial payment
        // (with all adjustments).
        $this->amount = round(ms_core_get_final_price($this), 2);
        break;
    }
  }
}

/**
 * MsPayment class.
 *
 * @author Leighton
 */
class MsPayment
{
  public $pid = 0;
  public $oid = 0;
  public $gateway = '';
  public $type = '';
  public $amount = 0;
  public $currency = '';

  public $transaction = '';
  public $recurring_id = '';
  public $data = array();

  public $shipping_address = array(
    'street' => '',
    'street2' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
  );
  public $billing_address = array(
    'street' => '',
    'street2' => '',
    'city' => '',
    'state' => '',
    'zip' => '',
    'country' => '',
    'phone' => '',
  );

  public $first_name = '';
  public $last_name = '';

  /**
   * Constructor.
   */
  function __construct() {

  }

  /**
   * Loads a payment from the database.
   *
   * @param int $pid
   *   The id of the payment to load
   */
  function load($pid) {
    // Load the order from the database.
    $query = db_select('ms_payments')
        ->fields('ms_payments')
        ->condition('pid', $pid)
        ->execute();

    if (!$row = $query->fetchObject()) {
      return FALSE;
    }

    // Set the values.
    $this->pid = $row->pid;
    $this->oid = $row->oid;
    $this->type = $row->type;
    $this->transaction = $row->transaction;
    $this->recurring_id = $row->recurring_id;
    $this->gateway = $row->gateway;
    $this->amount = round($row->amount, 2);
    $this->currency = $row->currency;
    $this->created = $row->created;
    $this->modified = $row->modified;

    $this->data = unserialize($row->data);

    return TRUE;
  }

  /**
   * Saves the object to the database.
   *
   * @return
   *   Returns TRUE if the operation is successful, FALSE otherwise.
   */
  function save() {
    // Ensure that the amount field is actually a decimal value.
    if (!$this->amount) {
      $this->amount = 0;
    }
    if (!empty($this->pid)) {
      // Update into the database.
      return drupal_write_record('ms_payments', $this, 'pid');
    }
    else {
      $object = (object) array();
      foreach ($this as $field => $value) {
        $object->$field = $value;
      }
      unset($object->pid);
      // Insert into Database.
      if (drupal_write_record('ms_payments', $object)) {
        $this->pid = $object->pid;
        return TRUE;
      }
      return FALSE;
    }
  }
}

/**
 * MsProduct class.
 *
 * @author Leighton
 */
class MsProduct
{
  public $order_product_id = 0;
  public $oid = 0;
  public $type = 'cart';
  public $name = '';
  public $module = '';
  public $qty = 1;
  public $amount = '';
  public $id = '';
  public $owner = 0;
  public $edit_path = '';
  public $purchase_path = '';

  public $data = array();
  public $recurring_schedule = array(
    'total_occurrences' => 0,
    'main_amount' => 0,
    'main_length' => 0,
    'main_unit' => '',
    'has_trial' => FALSE,
    'trial_amount' => 0,
    'trial_length' => 0,
    'trial_unit' => '',
  );

  /**
   * Constructor.
   */
  function __construct() {

  }

  /**
   * Loads the product from the database.
   *
   * @param $id
   *   The id of the product to load.
   */
  function load($id) {
    // Load the order from the database.
    $query = db_select('ms_order_products')
        ->fields('ms_order_products')
        ->condition('order_product_id', $id)
        ->execute();

    if (!$row = $query->fetchObject()) {
      return FALSE;
    }

    // Set the values.
    $this->order_product_id = $row->order_product_id;
    $this->oid = $row->oid;
    $this->type = $row->type;
    $this->id = $row->id;
    $this->name = $row->name;
    $this->module = $row->module;
    $this->amount = round($row->amount, 2);
    $this->qty = $row->qty;

    $this->recurring_schedule = unserialize($row->recurring_schedule);

    // Set the defaults.
    if (!isset($this->recurring_schedule['trial_length'])) {$this->recurring_schedule['trial_length'] = 0;}
    if (!isset($this->recurring_schedule['trial_unit'])) {$this->recurring_schedule['trial_unit'] = '';}
    if (!isset($this->recurring_schedule['trial_amount'])) {$this->recurring_schedule['trial_amount'] = 0;}
    if (!isset($this->recurring_schedule['main_length'])) {$this->recurring_schedule['main_length'] = 0;}
    if (!isset($this->recurring_schedule['main_unit'])) {$this->recurring_schedule['main_unit'] = '';}
    if (!isset($this->recurring_schedule['main_amount'])) {$this->recurring_schedule['main_amount'] = 0;}

    $this->recurring_schedule['main_amount'] *= $this->qty;
    $this->recurring_schedule['trial_amount'] *= $this->qty;

    $this->data = unserialize($row->data);

    return TRUE;
  }

  /**
   * Saves the object to the database.
   *
   * @return
   *   Returns TRUE if the operation is successful, FALSE otherwise.
   */
  function save() {
    $this->name = substr($this->name, 0, 127);
    // Ensure that the amount field is actually a decimal value.
    if (!$this->amount) {
      $this->amount = 0;
    }
    if ($this->order_product_id > 0) {
      // Update into the database.
      return drupal_write_record('ms_order_products', $this, 'order_product_id');
    }
    else {
      // Insert into Database.
      if (drupal_write_record('ms_order_products', $this)) {
        return TRUE;
      }
      return FALSE;
    }
  }

  /**
   * Initializes a product object with variables.
   *
   * This is mostly used for loading cart products into the class.
   *
   * @param $product
   *   The $product object from the database.
   */
  function initialize($product) {
    foreach ($product as $key => $value) {
      $this->$key = $value;
    }
    $this->name = substr($this->name, 0, 127);
    $this->data = unserialize($this->data);
    $this->recurring_schedule = unserialize($this->recurring_schedule);

    $this->recurring_schedule['main_amount'] *= $this->qty;
    $this->recurring_schedule['trial_amount'] *= $this->qty;
  }
}

/**
 * MsAdjustment class.
 *
 * @author Leighton
 */
class MsAdjustment
{
  // Property Declaration.
  public $id = '';
  public $product_id = NULL;
  public $optional = FALSE;
  public $active = TRUE;
  public $display = '';
  public $type = 'fixed';
  public $scope = 'recurring';
  public $value = 0;
  public $weight = 0;
  public $data = array();

  /**
   * Constructor.
   *
   * @param $adjustment
   *   (Optional) The $adjustment object from the database to use for
   *   initializing values.
   */
  function __construct($adjustment = NULL) {
    if (isset($adjustment)) {
      $this->initialize($adjustment);
    }
  }

  /**
   * Initializes an adjustment object with variables.
   *
   * This is mostly used for loading cart adjustments into the class.
   *
   * @param $adjustment
   *   The $adjustment object from the database.
   */
  function initialize($adjustment) {
    foreach ($adjustment as $key => $value) {
      $this->$key = $value;
    }
    if ($this->data AND !is_array($this->data)) {
      $this->data = unserialize($this->data);
    }
  }
}
