<?php
/**
 * @file
 * Economic helper.
 *
 * A bridge between drupal commerce platform and the external
 * services hosted at e-conomic.
 */

/**
 * Economic helper.
 *
 * Active from the 1. september 2014 is that the domain of the webservice has
 * bin changed.
 */
class EconomicHelper {
  protected $wsdl = 'https://api.e-conomic.com/secure/api1/EconomicWebService.asmx?wsdl';
  protected $debug = TRUE;
  protected $languageId = 1;
  protected $client;
  protected $debitorHandle;
  protected $invoiceHandle;
  protected $templateId = 0;
  protected $account;
  protected $users;
  protected $pass;
  protected $lastLogId = 0;
  public $connected = FALSE;

  /**
   * Login to webservice.
   *
   * @param string $account
   *   An e-conomic account name.
   * @param string $user
   *   An e-conomic user account name.
   * @param string $pass
   *   An e-conomic user account password.
   */
  public function __construct($account = '', $user = '', $pass = '') {
    $this->account = $account;
    $this->user = $user;
    $this->pass = $pass;
    $this->connected = FALSE;
    $this->connect();
  }

  /**
   * Logout.
   */
  public function __destruct() {
    if (!empty($this->client) and is_object($this->client)) {
      $this->client->Disconnect();
    }
  }

  /**
   * Connects to e-conomic soap service.
   */
  public function connect() {
    try {
      $this->client = new SoapClient($this->wsdl, array(
        "trace" => $this->debug,
        "exceptions" => $this->debug,
      ));

      $this->client->Connect(array(
        'agreementNumber' => $this->account,
        'userName' => $this->user,
        'password' => $this->pass,
      ));

      $this->connected = TRUE;
    }
    catch (SoapFault $e) {
      $soap_fault_code = (!empty($e->faultcode) ? strtolower($e->faultcode) : FALSE);

      switch ($soap_fault_code) {
        case 'wsdl':
          $soap_falt_message = 'Error in webservice connection.';
        default:
          $soap_falt_message = 'Unknown error when trying to connect.';
          break;
      }

      watchdog('e-conomic', $soap_falt_message, array('@faultcode' => $e->faultcode), WATCHDOG_ALERT, $this->wsdl);
    }
  }

  /**
   * Locate debitor by id an return the handle.
   *
   * @param int $id
   *   An e-conomic debitor id.
   *
   * @return object
   *   An e-conomic debitor object if found, FALSE if not.
   */
  public function findDebitorById($id = 0) {
    $new_debtor_handle = $this->client->Debtor_FindByNumber(array('number' => $id));
    if (empty($new_debtor_handle)) {
      return FALSE;
    }
    return (!empty($new_debtor_handle->Debtor_FindByNumberResult->Number) ? $new_debtor_handle->Debtor_FindByNumberResult->Number : FALSE);
  }

  /**
   * Get name of the group baseed on number.
   *
   * @param int $number
   *   Debitor group id.
   *
   * @return string
   *   Debitor group name if found, empty string if not.
   */
  public function getDebitorGroupName($number = 0) {
    $group_info = $this->client->DebtorGroup_GetName(array('debtorGroupHandle' => array('Number' => $number)));
    if (!empty($group_info->DebtorGroup_GetNameResult)) {
      return $group_info->DebtorGroup_GetNameResult;
    }
    return '';
  }

  /**
   * Locate all created debitors on the account and returns array with values.
   *
   * @return array $debitors
   *   An array of e-conomic debitor records.
   */
  public function getDebitors() {
    $debitors = array();
    $tmp_debitors = $this->client->DebtorGroup_GetAll();
    if (!empty($tmp_debitors) and is_object($tmp_debitors)) {
      if (count($tmp_debitors->DebtorGroup_GetAllResult->DebtorGroupHandle) == 1) {
        $tmp_row = $tmp_debitors->DebtorGroup_GetAllResult->DebtorGroupHandle;
        $tmp_row->name = ((!empty($tmp_row->Number) and is_numeric($tmp_row->Number)) ? $this->getDebitorGroupName($tmp_row->Number) : '');
        $debitors[$tmp_row->Number] = (!empty($tmp_row->name) ? $tmp_row->name : $tmp_row->Number);
      }
      else {
        foreach ($tmp_debitors->DebtorGroup_GetAllResult->DebtorGroupHandle as $tmp_row) {
          if (!empty($tmp_row->Number)) {
            $tmp_row->name = ((!empty($tmp_row->Number) and is_numeric($tmp_row->Number)) ? $this->getDebitorGroupName($tmp_row->Number) : '');
            $debitors[$tmp_row->Number] = (!empty($tmp_row->name) ? $tmp_row->name : $tmp_row->Number);
          }
        }
      }
    }
    return $debitors;
  }

  /**
   * Get term name based on id.
   *
   * @param int $id
   *   Term ID.
   *
   * @return mixed
   *   Term name if found, FALSE if not.
   */
  public function getTermName($id = 0) {
    $term_info = $this->client->TermOfPayment_GetName(array('termOfPaymentHandle' => array('Id' => $id)));
    if (!empty($term_info->TermOfPayment_GetNameResult)) {
      return $term_info->TermOfPayment_GetNameResult;
    }
    return FALSE;
  }

  /**
   * Get all created terms from e-conomic.
   *
   * @return array $terms
   *   An array of e-conomic terms.
   */
  public function getTerms() {
    $terms = array();
    $tmp_terms = $this->client->TermOfPayment_GetAll();
    if (!empty($tmp_terms) and is_object($tmp_terms) and !empty($tmp_terms->TermOfPayment_GetAllResult->TermOfPaymentHandle)) {
      if (count($tmp_terms->TermOfPayment_GetAllResult->TermOfPaymentHandle) == 1) {
        $tmp_row = $tmp_terms->TermOfPayment_GetAllResult->TermOfPaymentHandle;
        $terms[$tmp_row->Id] = $this->getTermName($tmp_row->Id);
      }
      else {
        foreach ($tmp_terms->TermOfPayment_GetAllResult->TermOfPaymentHandle as $tmp_row) {
          $terms[$tmp_row->Id] = $this->getTermName($tmp_row->Id);
        }
      }
    }
    return $terms;
  }

  /**
   * Get group name based on number.
   *
   * @param int $number
   *   Unique product group identifier.
   *
   * @return mixed
   *   A product name, or FALSE if nothing was found.
   */
  public function getProductGroupName($number = 0) {
    $group_info = $this->client->ProductGroup_GetName(array('productGroupHandle' => array('Number' => $number)));
    if (!empty($group_info->ProductGroup_GetNameResult)) {
      return $group_info->ProductGroup_GetNameResult;
    }
    return FALSE;
  }

  /**
   * Get all product groups from e-conomic.
   *
   * @return array $products
   *   An array of product groups in e-conomic format.
   */
  public function getProductGroups() {
    $products = array();
    $tmp_products = $this->client->ProductGroup_GetAll();
    if (!empty($tmp_products) and is_object($tmp_products)) {
      if (count($tmp_products->ProductGroup_GetAllResult->ProductGroupHandle) == 1) {
        $tmp_row = $tmp_products->ProductGroup_GetAllResult->ProductGroupHandle;
        $group_info = $this->getProductGroupName($tmp_row->Number);
        $products[$tmp_row->Number] = $group_info;
      }
      else {
        foreach ($tmp_products->ProductGroup_GetAllResult->ProductGroupHandle as $tmp_row) {
          $group_info = $this->getProductGroupName($tmp_row->Number);
          $products[$tmp_row->Number] = $group_info;
        }
      }
    }
    return $products;
  }

  /**
   * Define invoice template id.
   *
   * @param int $id
   *   A new template id value.
   */
  public function setTemplate($id = 0) {
    $this->templateId = $id;
  }

  /**
   * Create e-conomic order.
   *
   * @param array $order_array
   *   An order array in a format suitable for e-conomic.
   *
   * @return int $economic_order_id
   *   An e-conomic order ID on successful creation, 0 otherwise.
   */
  public function createOrder($order_array = array()) {
    $error_count = 0;

    $debitor_name = (!empty($order_array['billing_contact_name']) ? $order_array['billing_contact_name'] : '');
    if (!empty($order_array['billing_organisation_name'])) {
      $debitor_name = $order_array['billing_organisation_name'];
    }

    $data = array(
      'DebtorHandle' => array('Number' => $order_array['old_debitor_handle']),
      'DebtorName' => $debitor_name,
      'Number' => (!empty($order_array['order_id']) ? $order_array['order_id'] : 0),
      'DebtorAddress' => (!empty($order_array['billing_address']) ? $order_array['billing_address'] : ''),
      'DebtorPostalCode' => (!empty($order_array['billing_postal_code']) ? $order_array['billing_postal_code'] : 0),
      'DebtorCity' => (!empty($order_array['billing_locality']) ? $order_array['billing_locality'] : ''),
      'DebtorCountry' => (!empty($order_array['billing_country']) ? $order_array['billing_country'] : ''),
      'DeliveryAddress' => (!empty($order_array['shipping_address']) ? $order_array['shipping_address'] : ''),
      'DeliveryPostalCode' => (!empty($order_array['shipping_postal_code']) ? $order_array['shipping_postal_code'] : 0),
      'DeliveryCity' => (!empty($order_array['shipping_locality']) ? $order_array['shipping_locality'] : ''),
      'DeliveryCountry' => (!empty($order_array['shipping_country']) ? $order_array['shipping_country'] : ''),
      'Date' => REQUEST_TIME,
      'DueDate' => 0,
      'Heading' => t('Order from webshop #@order_id', array(
        '@order_id' => $order_array['order_id'],
      )),
      // 'TextLine1' => 'Test 1',
      // 'TextLine2' => 'Test 2',
      'OtherReference' => (!empty($order_array['order_number']) ? $order_array['order_number'] : ''),
      'ExchangeRate' => 0,
      'IsVatIncluded' => TRUE,
      'IsArchived' => FALSE,
      'IsSent' => FALSE,
      'NetAmount' => (!empty($order_array['total_price']) ? $order_array['total_price'] : 0),
      'VatAmount' => (!empty($order_array['total_price']) ? ($order_array['total_price'] - ($order_array['total_price'] * 0.8)) : 0),
      'GrossAmount' => 0,
      'Margin' => 0,
      'MarginAsPercent' => 0,
      'TermOfPaymentHandle' => array('Id' => (!empty($order_array['debitor_payment_terms']) ? $order_array['debitor_payment_terms'] : 0)),
      'CurrencyHandle' => array('Code' => (!empty($order_array['currency_code']) ? $order_array['currency_code'] : 'DKK')),
      // 'OtherReference' => 0,
    );

    if (!empty($order_array['debitor_attention_handler']) and is_numeric($order_array['debitor_attention_handler'])) {
      $attention_handle = new stdClass();
      $attention_handle->Id = $order_array['debitor_attention_handler'];

      $data['AttentionHandle'] = $attention_handle;
    }

    $tmp_log_id = db_insert('economic_log')->fields(array(
      'order_id' => $order_array['order_id'],
      'request_data' => json_encode($data),
      'economic_debitor_refere' => (!empty($order_array['old_debitor_handle']) ? $order_array['old_debitor_handle'] : 0),
      'created' => REQUEST_TIME,
      'last_update' => REQUEST_TIME,
      'status' => 1,
    ))->execute();
    $this->lastLogId = $tmp_log_id;

    $data = array('data' => $data);
    $tmp_order = $this->client->Order_CreateFromData($data)->Order_CreateFromDataResult;
    $economic_order_id = (!empty($tmp_order->Id) ? $tmp_order->Id : FALSE);

    db_update('economic_log')->fields(array(
      'response_data' => json_encode($tmp_order),
      'last_update' => REQUEST_TIME,
      'economic_order_id' => $economic_order_id,
      'status' => 2,
    ))->condition('log_id', $this->lastLogId, '=')->execute();

    if (!empty($economic_order_id)) {
      if (!empty($order_array['products'])) {
        foreach ($order_array['products'] as $p_row) {
          if ($this->createOrderLine($economic_order_id, $p_row)) {
            /*
             * OrderLine created
             */
          }
          else {
            $error_count++;
          }
        }
      }
    }
    else {
      $error_count++;
    }

    return $economic_order_id;
  }

  /**
   * Create debitor account.
   *
   * @param array $order_array
   *   An order array in e-conomic format.
   *
   * @return int
   *   A debtor ID if a debtor has been created successfully, 0 if not.
   *
   * @see economic_order_transfer()
   */
  public function createDebitor($order_array = array()) {
    $deb_obj = self::debitor('create', $order_array);

    $this->debtorHandle = $this->client->Debtor_CreateFromData(array(
      'data' => $deb_obj,
    ))->Debtor_CreateFromDataResult;

    return (!empty($this->debtorHandle->Number) ? $this->debtorHandle->Number : 0);
  }

  /**
   * Update debitor account by handle.
   *
   * @param array $order_array
   *   New order data to update debitor with.
   *
   * @return int
   *   Debitor handle id if the update was successful, 0 if not.
   */
  public function updateDebitor($order_array = array()) {
    $deb_obj = self::debitor('update', $order_array);

    $this->debtorHandle = $this->client->Debtor_UpdateFromData(array(
      'data' => $deb_obj,
    ))->Debtor_UpdateFromDataResult;

    return (!empty($this->debtorHandle->Number) ? $this->debtorHandle->Number : 0);
  }

  /**
   * Create contact based on uid.
   *
   * @param array $parent_debitor_handle
   *   The parent debitor id.
   * @param array $order_array
   *   The order array for details.
   *
   * @return int
   *   the id of the contact.
   */
  public function debitorContact($parent_debitor_handle = array(), $order_array = array()) {
    if (empty($parent_debitor_handle)) {
      return FALSE;
    }

    if (!empty($order_array['billing_uid']) and is_numeric($order_array['billing_uid'])) {
      $debitor_contact_old_handle = $this->client->DebtorContact_FindByExternalId(array(
        'externalId' => $order_array['billing_uid'],
      ));

      $debitor_contact_handle = FALSE;

      $debitor_contact_old_handle = 0;
      if (!empty($debitor_contact_old_handle->DebtorContact_FindByExternalIdResult[0]->Id)) {
        $debitor_contact_old_handle = $debitor_contact_old_handle->DebtorContact_FindByExternalIdResult[0]->Id;
      }
      elseif (!empty($debitor_contact_old_handle->DebtorContact_FindByExternalIdResult->Id)) {
        $debitor_contact_old_handle = $debitor_contact_old_handle->DebtorContact_FindByExternalIdResult->Id;
      }

      if (!empty($debitor_contact_old_handle)) {
        $debitor_contact_handle = new stdClass();
        $debitor_contact_handle->Id = $debitor_contact_old_handle->DebtorContact_FindByExternalIdResult->Id;
      }

      $debitor_handle = new stdClass();
      $debitor_handle->Number = $parent_debitor_handle;

      $debitor_contact_obj = new stdClass();
      $debitor_contact_obj->Number = $order_array['billing_uid'];
      $debitor_contact_obj->ExternalId = $order_array['billing_uid'];
      $debitor_contact_obj->DebtorHandle = $debitor_handle;
      $debitor_contact_obj->Name = (!empty($order_array['billing_contact_name']) ? $order_array['billing_contact_name'] : '');
      if (!empty($order_array['billing_phone_number'])) {
        $debitor_contact_obj->TelephoneNumber = $order_array['billing_phone_number'];
      }
      elseif (!empty($order_array['billing_phone'])) {
        $debitor_contact_obj->TelephoneNumber = $order_array['billing_phone'];
      }
      $debitor_contact_obj->Email = (!empty($order_array['billing_email']) ? $order_array['billing_email'] : '');

      if (!empty($debitor_contact_old_handle->DebtorContact_FindByExternalIdResult->Id)) {
        $debitor_contact_obj->Handle = $debitor_contact_handle;
        $debitor_contact_responce = $this->client->DebtorContact_UpdateFromData(array(
          'data' => $debitor_contact_obj,
        ));
      }
      else {
        $debitor_contact_obj->Id = $order_array['billing_uid'];
        $debitor_contact_obj->IsToReceiveEmailCopyOfOrder = FALSE;
        $debitor_contact_obj->IsToReceiveEmailCopyOfInvoice = FALSE;
        $debitor_contact_responce = $this->client->DebtorContact_CreateFromData(array(
          'data' => $debitor_contact_obj,
        ));
      }
    }

    if (!empty($debitor_contact_responce->DebtorContact_CreateFromDataResult->Id)) {
      return $debitor_contact_responce->DebtorContact_CreateFromDataResult->Id;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Define main object of a debitor to be created or updated.
   *
   * Debitors are created or returned from this function. If creating, pass
   * 'create' (or nothing) as the first parameter.
   *
   * @param string $type
   *   Determine what to do, update or creating debitor.
   * @param array $order_array
   *   Array of order details.
   *
   * @return object
   *   Debitor object in format suitable for passing to e-conomic.
   */
  protected function debitor($type = 'create', $order_array = array()) {
    $currency_handle = $this->client->Currency_FindByCode(array('code' => $order_array['currency_code']))->Currency_FindByCodeResult;
    $get_term = array('Id' => (!empty($order_array['debitor_payment_terms']) ? $order_array['debitor_payment_terms'] : 0));

    $debitor_handle = FALSE;
    if (!empty($order_array['old_debitor_handle'])) {
      $debitor_handle = new stdClass();
      $debitor_handle->Number = $order_array['old_debitor_handle'];
    }

    $deb_obj = new stdClass();
    $deb_obj->Handle = $debitor_handle;

    if (!empty($order_array['billing_organisation_name'])) {
      $deb_obj->Name = $order_array['billing_organisation_name'];
    }
    else {
      $deb_obj->Name = (!empty($order_array['billing_contact_name']) ? $order_array['billing_contact_name'] : t('Unknown'));
    }

    if ($type == 'create') {
      $deb_obj->Email = (!empty($order_array['billing_email']) ? $order_array['billing_email'] : '');
      $deb_obj->Id = (!empty($order_array['billing_uid']) ? $order_array['billing_uid'] : 0);

      if (!empty($order_array['billing_phone_number'])) {
        $deb_obj->TelephoneAndFaxNumber = $order_array['billing_phone_number'];
      }
      elseif (!empty($order_array['billing_phone'])) {
        $deb_obj->TelephoneAndFaxNumber = $order_array['billing_phone'];
      }
    }

    $deb_obj->IsAccessible = TRUE;
    $deb_obj->DebtorGroupHandle = array('Number' => (!empty($order_array['debitor_group_id']) ? $order_array['debitor_group_id'] : 0));
    $deb_obj->VatZone = (!empty($order_array['vat_zone']) ? $order_array['vat_zone'] : 'HomeCountry');

    $deb_obj->CurrencyHandle = $currency_handle;
    $deb_obj->TermOfPaymentHandle = $get_term;

    if (!empty($order_array['billing_ean'])) {
      $deb_obj->Ean = $order_array['billing_ean'];
    }

    if (!empty($order_array['billing_website'])) {
      $deb_obj->Website = $order_array['billing_website'];
    }

    if (!empty($order_array['billing_email'])) {
      $deb_obj->Email = $order_array['billing_email'];
    }

    $deb_obj->Address = (!empty($order_array['billing_address']) ? $order_array['billing_address'] : '');
    $deb_obj->PostalCode = (!empty($order_array['billing_postal_code']) ? $order_array['billing_postal_code'] : '');
    $deb_obj->City = (!empty($order_array['billing_locality']) ? $order_array['billing_locality'] : '');
    $deb_obj->Country = (!empty($order_array['billing_country']) ? $order_array['billing_country'] : 'DK');
    $deb_obj->CreditMaximum = (!empty($order_array['billing_credit_maximum']) ? $order_array['billing_credit_maximum'] : 0);
    $deb_obj->VatNumber = (!empty($order_array['vat_number']) ? $order_array['vat_number'] : '');
    $deb_obj->Number = (!empty($order_array['debitor_id']) ? $order_array['debitor_id'] : '');

    return $deb_obj;
  }

  /**
   * Create a new product.
   *
   * @param array $product
   *   A product array.
   *
   * @return bool
   *   TRUE if product has been saved in e-conomic, FALSE if not.
   */
  public function createProduct($product = array()) {
    if (!empty($product['sku'])) {
      $data = array(
        'Number' => $product['sku'],
        'Name' => (!empty($product['title']) ? trim($product['title']) : ''),
        'Description' => (!empty($product['description']) ? $product['description'] : ''),
        'Volume' => 1,
        'SalesPrice' => (!empty($product['price']) ? ($product['price'] * 0.8) : 0),
        'CostPrice' => (!empty($product['cost_price']) ? ($product['cost_price'] * 0.8) : 0),
        'RecommendedPrice' => (!empty($product['recommended_price']) ? ($product['recommended_price'] * 0.8) : 0),
        'ProductGroupHandle' => array('Number' => (!empty($product['group_id']) ? $product['group_id'] : 0)),
        'IsAccessible' => TRUE,
        'InStock' => TRUE,
        'Available' => TRUE,
      );
      $data = array('data' => $data);

      $product_handle = $this->client->Product_CreateFromData($data);
      if (!empty($product_handle->Product_CreateFromDataResult->Number)) {
        return $product_handle->Product_CreateFromDataResult->Number;
      }
    }

    return FALSE;
  }

  /**
   * Create order line.
   *
   * @param int $economic_order_id
   *   Unique e-conomic order ID.
   * @param array $product
   *   A product array.
   *
   * @return bool
   *   TRUE if order line was created, FALSE on failure.
   */
  protected function createOrderLine($economic_order_id, $product = array()) {
    /*
     * Product is located or created
     */
    $product_handle = $this->client->Product_FindByNumber(array('number' => $product['sku']));
    if (!empty($product_handle->Product_FindByNumberResult->Number)) {
      $product_handle = array('Number' => $product_handle->Product_FindByNumberResult->Number);
    }
    else {
      $new_number = 0;
      if ($new_number = $this->createProduct($product) and !empty($new_number)) {
        $product_handle = array('Number' => $new_number);
      }
    }

    /*
     * The order line is created based on located product.
     */
    if (!empty($product_handle)) {
      $data = array(
        'ProductHandle' => $product_handle,
        'Description' => (!empty($product['description']) ? $product['description'] : $product['title']),
        'Quantity' => (!empty($product['quantity']) ? $product['quantity'] : 1),
        'Id' => $product['line_item_id'],
        'Number' => $product['line_item_no'],
        'OrderHandle' => array('Id' => $economic_order_id),
        'TotalNetAmount' => (!empty($product['price']) ? (($product['price'] * 0.8) * $product['quantity']) : 0),
        'UnitNetPrice' => (!empty($product['price']) ? ($product['price'] * 0.8) : 0),
        'UnitCostPrice' => (!empty($product['cost_price']) ? ($product['cost_price'] * 0.8) : 0),
        'DiscountAsPercent' => (!empty($product['discount_percent']) ? $product['discount_percent'] : 0),
        'TotalMargin' => 0,
        'MarginAsPercent' => 0,
      );
      $data = array('data' => $data);

      $orderline_handle = $this->client->OrderLine_CreateFromData($data);
      if (!empty($orderline_handle->OrderLine_CreateFromDataResult->Number) and !empty($orderline_handle->OrderLine_CreateFromDataResult->Id)) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Converts an order to invoice.
   *
   * @param int $economic_order_id
   *   Unique e-conomic order ID.
   *
   * @return bool
   *   Id of the invoice if created, FALSE on failure.
   */
  public function upgradeOrderToInvoice($economic_order_id = 0) {
    $order_handle = $this->client->Order_GetNumber(array(
      'orderHandle' => array(
        'Id' => $economic_order_id,
      ),
    ));

    if (!empty($order_handle->Order_GetNumberResult)) {
      $order_invoice_result = $this->client->Order_UpgradeToInvoice(array(
        'orderHandle' => array(
          'Id' => $economic_order_id,
        ),
      ));

      if (!empty($order_invoice_result->Order_UpgradeToInvoiceResult->Id)) {
        $invoice_id = $order_invoice_result->Order_UpgradeToInvoiceResult->Id;

        db_update('economic_log')->fields(array(
          'last_update' => REQUEST_TIME,
          'economic_invoice_id' => $invoice_id,
          'status' => 3,
        ))->condition('economic_order_id', $economic_order_id, '=')->execute();

        return $order_invoice_result->Order_UpgradeToInvoiceResult->Id;
      }
    }

    return FALSE;
  }

  /**
   * Returns the last log made by this class if defined.
   *
   * @return bool
   *   Id if the log is created, FLASE if not.
   */
  public function getLastLog() {
    return (!empty($this->lastLogId) ? $this->lastLogId : FALSE);
  }
}
