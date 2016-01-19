<?php

/**
 * @file
 * Business logic for invoices:
 * Main logic that allows site users to send out invoices, manage invoices and pay invoices
 *
 */

class InvoiceDB {

  private static $_invoice;

  /**
   * Get Invoice product by invoice number
   * @param $invoice_no
   * The invoice number
   *
   * @param $invoice_user
   * The user who created the invoice
   *
   * @return
   * An Invoice instance or FALSE if it doesn't exist
   */
  public static function GetInvoice($invoice_no) {
    //Preconditions
    $exists = self::CheckInvoiceExists($invoice_no);
    if ($exists == FALSE) {
      throw new InvalidArgumentException("Invoice must exist");
    }
    //End Preconditions

    self::$_invoice = new Invoice($invoice_no);
    self::$_invoice->load_invoice_product();

    return self::$_invoice;
  }

  /**
   * Get Invoice by invoice_id
   * @param $_invoice_id
   * The ID of the invoice product
   */
  public static function LoadInvoice($invoice_id) {
    //Preconditions
    $invoice = commerce_product_load($invoice_id);
    if ($invoice == FALSE) {
      throw new InvalidArgumentException("Invoice product must exist");
    }
    //End Preconditions

    $invoice_no = substr($invoice->title, 9, strpos($invoice->title, '(') - 9);
    self::$_invoice = new Invoice($invoice_no);
    self::$_invoice->load_invoice_product();

    return self::$_invoice;
  }

  /**
   * Check if the invoice is in the database
   *
   * @param $invoice_no
   * The invoice number
   *
   * @return
   * TRUE if it exists and FALSE if not
   */
  public static function CheckInvoiceExists($invoice_no) {
    //Preconditions
    if (empty($invoice_no))
      throw new InvalidArgumentException("Invoice number can't be empty");
    //End Preconditions

    $invoice = db_query("SELECT * FROM {commerce_product} WHERE type='invoice' AND sku='invoice_" . $invoice_no . "'")->fetchAssoc();

    if (!$invoice) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

  /**
   * Check if the invoice is already paid
   *
   * @param $invoice_no
   * The invoice number
   *
   * @return
   * TRUE if it's paid and FALSE if not
   */
  public static function CheckInvoiceAlreadyPaid($invoice_no) {
    //Preconditions
    if (empty($invoice_no))
      throw new InvalidArgumentException("Invoice number can't be empty");
    //End Preconditions

    $invoice = db_query("SELECT * FROM {commerce_product} WHERE type='invoice' AND sku='invoice_" . $invoice_no . "' AND status=0")->fetchAssoc();

    if (!$invoice) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

}

class Invoice {

  private $_invoice_no = '';
  private $_invoice_amount = 0.0;
  private $_invoice_payee = '';
  private $_invoice_product = null;

  function __construct($invoice_no) {
    $this->_invoice_no = $invoice_no;
  }

  public function get__invoice_no() {
    return $this->_invoice_no;
  }

  public function get__invoice_amount() {
    return $this->_invoice_amount;
  }

  public function get__invoice_payee() {
    return $this->_invoice_payee;
  }

  public function set__invoice_no($invoice_no) {
    //Preconditions
    if (empty($invoice_no))
      throw new InvalidArgumentException("Invoice No. can't be empty");
    //End Preconditions

    $this->_invoice_no = $invoice_no;
  }

  public function set__invoice_amount($invoice_amount) {
    //Preconditions
    if (!is_numeric($invoice_amount) || $invoice_amount <= 0)
      throw new InvalidArgumentException("Invoice amount must be a numeric value that is greater than 0");
    //End Preconditions

    $this->_invoice_amount = $invoice_amount;
  }

  public function get__invoice_product() {
    return $this->_invoice_product;
  }

  public function set__invoice_payee($invoice_payee) {
    //Preconditions
    if (empty($invoice_payee))
      throw new InvalidArgumentException("Invoice payee can't be empty");
    //End Preconditions

    $this->_invoice_payee = $invoice_payee;
  }

  /**
   * Add the Commerce invoice product for the invoice
   */
  public function load_invoice_product() {
    //Preconditions
    if (empty($this->_invoice_no))
      throw new InvalidArgumentException("Invoice number can't be empty");
    //End Preconditions

    $this->_invoice_product = commerce_product_load_by_sku("invoice_" . $this->_invoice_no);
    $this->set__invoice_amount($this->_invoice_product->commerce_price[LANGUAGE_NONE][0]['amount']);
    $this->set__invoice_payee(substr($this->_invoice_product->title, strpos($this->_invoice_product->title, '(') + 1, strpos($this->_invoice_product->title, ')') - strpos($this->_invoice_product->title, '(') - 1));
  }

  /**
   * Create a new Commerce product for the invoice
   */
  public function save_invoice($invoice_user) {
    //Create Commerce product of type invoice and set it's properties
    $this->_invoice_product = commerce_product_new('invoice');
    $this->_invoice_product->uid = $invoice_user->uid;
    $this->_invoice_product->sku = 'invoice_' . $this->_invoice_no;
    $this->_invoice_product->commerce_price['und'][0]['amount'] = $this->_invoice_amount * 100.0;
    $this->_invoice_product->commerce_price['und'][0]['currency_code'] = commerce_default_currency();
    $this->_invoice_product->title = 'Invoice #' . $this->_invoice_no . ' (' . $this->_invoice_payee . ')';

    //Save the invoice in the db
    commerce_product_save($this->_invoice_product);
  }

  /**
   * Mark invoice as paid and deactivate the invoice product
   */
  public function set_invoice_to_paid() {
    //Deactivate invoice to mark it as Paid
    $this->_invoice_product->status = 0;

    //Save the invoice in the database
    commerce_product_save($this->_invoice_product);
  }

}

class CommerceInvoiceOrder {

  private $_invoice;

  function __construct(Invoice $invoice) {
    $this->_invoice = $invoice;
  }

  public function Checkout($user) {
    //Create a Commerce Order and add the invoice product and redirect to checkout
    $order = ($user->uid) ? commerce_order_new($user->uid, 'checkout_checkout') : commerce_cart_order_new();
    $line_item = commerce_product_line_item_new($this->_invoice->get__invoice_product(), 1, $order->order_id);
    commerce_line_item_save($line_item);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order_wrapper->commerce_line_items[] = $line_item;
    $order->data['type'] = 'commerce_invoice';
    $order->data['invoice_no'] = $this->_invoice->get__invoice_no();

    commerce_order_save($order);

    drupal_goto('checkout/' . $order->order_id);
  }

}