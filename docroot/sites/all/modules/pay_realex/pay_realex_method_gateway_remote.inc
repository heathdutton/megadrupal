<?php

/**
 * @file
 */

class pay_realex_method_gateway_remote extends pay_method_gateway {
  var $realex_merchid = '';
  var $realex_secret = '';
  var $realex_account = '';
  var $auto_settle_flag = 1;
  var $parser;
  var $record;
  var $current_field = '';
  var $field_type;
  var $ends_record;
  var $records;
  var $answers;
  var $cc_currency;
  var $realex_cctype_separate_subaccounts = FALSE;
  var $realex_cctype_accounts = array();


  function payment_types($filter = NULL) {
    $payment_types = parent::payment_types($filter);

    if ($filter) {
      foreach ($payment_types as $key => $label) {
        if (!$filter[$key]) {
          unset($payment_types[$key]);
        }
      }
    }

    return $payment_types;
  }

  function gateway_url() {
    $url = 'https://epage.payandshop.com/epage-remote.cgi';
    return $url;
  }

  function gateway_request() {
    // Set the transaction type based on requested activity.
    if (!$txntype = $this->realex_trxtype($this->activity->activity)) {
      drupal_set_message(t('Payment activity "@activity" is unsupported.  Not processing transaction.', array('@activity' => $this->activity->activity)), 'error');
      return FALSE;
    }

    $req = '';

    // Realex takes the value in cent.
    $amount = ($this->total() * 100);
    $currency = $this->cc_currency;
    $cardnumber = $this->cc_number;
    $cardname = $this->first_name . ' ' . $this->last_name;
    $cardtype = strtoupper($this->cc_type);
    $cardtype = ($cardtype == 'MASTERCARD' ? 'MC' : ($cardtype == 'SOLO' || $cardtype == 'MAESTRO' ? 'SWITCH' : $cardtype));

    $expdate = $this->cc_expiration();
    $cvn = $this->cc_ccv2;
    $issueno = $this->cc_issue_number;
    $auto_settle_flag = $this->auto_settle_flag;

    $merchantid = $this->realex_merchid;
    $secret = $this->realex_secret;
    $account = $this->realex_account;
    // Per card type subaccounts in use.
    if ($this->realex_cctype_separate_subaccounts) {
      $account = (!empty($this->realex_cctype_accounts[$this->cc_type]) ? $this->realex_cctype_accounts[$this->cc_type] : $account);
    }

    $url =  'https://epage.payandshop.com/epage-remote.cgi';

    // Creates timestamp that is needed to make up orderid.
    // Get the transaction created time, if not set fall back to now.
    if (!empty($this->activity->pxid)) {
      $transaction = pay_load_object('pay_transaction', $this->activity->pxid);
      $time = $transaction->created;
      $timestamp = strftime('%Y%m%d%H%M%S', $time);
      $orderid = $timestamp . '-' . $this->activity->pxid;
    }
    else {
      $time = REQUEST_TIME;
      $timestamp = strftime('%Y%m%d%H%M%S', $time);
      mt_srand((double) microtime() * 1000000);
      $orderid = $timestamp . '-' . mt_rand(1, 999);
    }

    // You can use any alphanumeric combination for the orderid.Although each transaction must have a unique orderid.
    $tmp = "$timestamp.$merchantid.$orderid.$amount.$currency.$cardnumber";
    $sha1hash = sha1($tmp);
    $tmp = "$sha1hash.$secret";
    $sha1hash = sha1($tmp);

    $ip_address = ip_address();

    // A number of variables are needed to generate the request xml that is send to Realex Payments.
    $req = "<request type='auth' timestamp='$timestamp'>
    <merchantid>$merchantid</merchantid>
    <account>$account</account>
    <orderid>$orderid</orderid>
    <amount currency='$currency'>$amount</amount>
    <card>
      <number>$cardnumber</number>
      <expdate>$expdate</expdate>
      <type>$cardtype</type>
      <chname>$cardname</chname>
      <issueno>$issueno</issueno>
    </card>";
    if ($cvn) {
      $req .= "<cvn>
        <number>$cvn</number>
        <presind>1</presind>
      </cvn>";
    }
    // No product ids for donation forms.
    // <prodid>$products</prodid>
    $req .= "<autosettle flag='$auto_settle_flag'/>
      <sha1hash>$sha1hash</sha1hash>
      <tssinfo>
        <address type=\"billing\">
          <country>ie</country>
        </address>
        <custipaddress>$ip_address</custipaddress>
      </tssinfo>
    </request>";

    return $req;
  }

  function gateway_response($response = NULL) {

    if (is_null($response)) {
      drupal_set_message(t('We were unable to process your credit card payment.  Please verify your card details and try again. If the problem persists, contact us to complete your order.'), 'error');
      return FALSE;
    }

    // Tidy it up.
    $response = preg_replace('/\s+/', ' ', $response);
    $response = preg_replace('/[\n\r]/', '', $response);

    // Parse the response xml.
    $this->PayRealexRemote($response);
    $result = $this->record;

    if ($result['result'][0] != '00') {
      watchdog('payment', "Error processing payment: Realex gateway returned '@err'", array('@err' => $result['message']));
      drupal_set_message(t('We were unable to process your credit card payment; reason provided by bank: %message. If the problem persists, contact us to complete your order.', array('%message' => $result['message'])), 'error');
      return FALSE;
    }

    $orderid = $result['orderid'];

    $merchantid = $this->realex_merchid;
    $secret = $this->realex_secret;

    // Below is the code for creating the digital signature using the md5 algorithm.
    // This digital siganture should correspond to the
    // one Realex Payments POSTs back to this script and can therefore be used to verify the message Realex sends back.
    $timestamp = $result['timestamp'];
    $resultcode = $result['result'][0];
    $message = $result['message'];
    $pasref = $result['pasref'];
    $authcode = $result['authcode'];
    $tmp = "$timestamp.$merchantid.$orderid.$resultcode.$message.$pasref.$authcode";
    $sha1hash = sha1($tmp);
    $tmp = "$sha1hash.$secret";
    $sha1hash = sha1($tmp);

    // Save the transaction ID for tracking and/or future operations.
    $this->activity->identifier = $result['orderid'];
    $this->activity->data = array(
      'timestamp' => $result['timestamp'],
      'response_string' => $result['message'],
      'result_code' => $result['result'][0],
      'approval_code' => $result['authcode'],
      'caav_response' => $result['cvnresult'],
      'pasref' => $result['pasref'],
      'batchid' => $result['batchid'],
      'transaction_total' => $this->total(),
    );
    if (!empty($result['avspostcoderesponse'])) {
      $this->activity->data['avs_response'] = $result['avspostcoderesponse'];
    }

    $success = FALSE;
    if ($result['result'][0] == '00') {
      // Check to see if hashes match or not
      if ($sha1hash != $result['sha1hash']) {
        watchdog('pay_realex', 'Hash match failed for order @order_id.', array('@order_id' => $result['order_id']));
        drupal_set_message(t("hashes don't match - response not authenticated!"), 'error');
      }
      else {
        $success = TRUE;
        $this->activity->result = TRUE;
        drupal_set_message(t('Your payment has been successfully processed.'));
      }
    }

    return $success;
  }

  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);
    $group = $this->handler();

    $form[$group]['realex_remote']['#type'] = 'fieldset';
    $form[$group]['realex_remote']['#collapsible'] = FALSE;
    $form[$group]['realex_remote']['#title'] = t('Realex Remote settings');
    $form[$group]['realex_remote']['#group'] = $group;

    $form[$group]['realex_remote']['realex_merchid'] = array(
      '#type' => 'textfield',
      '#title' => t('Realex Merchant ID'),
      '#default_value' => $this->realex_merchid,
      '#required' => TRUE,
      '#parents' => array($group, 'realex_merchid'),
    );
    $form[$group]['realex_remote']['realex_account'] = array(
      '#type' => 'textfield',
      '#title' => t('Realex Account'),
      '#default_value' => $this->realex_account,
      '#required' => TRUE,
      '#parents' => array($group, 'realex_account'),
    );
    $form[$group]['realex_remote']['realex_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Shared Secret'),
      '#default_value' => $this->realex_secret,
      '#required' => TRUE,
      '#parents' => array($group, 'realex_secret'),
    );
    $form[$group]['realex_remote']['auto_settle_flag'] = array(
      '#type' => 'select',
      '#title' => t('Auto settle flag'),
      '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
      '#default_value' => $this->auto_settle_flag,
      '#required' => TRUE,
      '#parents' => array($group, 'auto_settle_flag'),
    );
    $form[$group]['realex_remote']['realex_cctype_separate_subaccounts'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use separate accounts for different card types'),
      '#description' => t('In some configurations, Realex requires the use of different accounts for some card types.'),
      '#default_value' => $this->realex_cctype_separate_subaccounts,
      '#required' => FALSE,
      '#parents' => array($group, 'realex_cctype_separate_subaccounts'),
      '#attributes' => array('onchange' => array('(function ($) { $("fieldset.realex-cctype-subaccounts").toggleClass("collapsed"); })(jQuery)')),
    );
    $form[$group]['realex_remote']['realex_cctype_accounts']['#type'] = 'fieldset';
    $form[$group]['realex_remote']['realex_cctype_accounts']['#collapsible'] = TRUE;
    $form[$group]['realex_remote']['realex_cctype_accounts']['#collapsed'] = ($this->realex_cctype_separate_subaccounts ? FALSE : TRUE);
    $form[$group]['realex_remote']['realex_cctype_accounts']['#title'] = t('Realex Card Type Account Settings');
    $form[$group]['realex_remote']['realex_cctype_accounts']['#group'] = $group;
    $form[$group]['realex_remote']['realex_cctype_accounts']['#attributes'] = array('class' => array('realex-cctype-subaccounts'));
    $form[$group]['realex_remote']['realex_cctype_accounts']['markup']['#value'] = '<p>' . t('Only used if "Use separate accounts for different card types" is checked and the default settings configured above will be used if the following fields are left empty.') . '</p>';
    $cctypes = $this->payment_types();
    foreach ($cctypes as $cctype => $label) {
      $form[$group]['realex_remote']['realex_cctype_accounts'][$cctype] = array(
        '#type' => 'textfield',
        '#title' => t('@cctype account', array('@cctype' => $label)),
        '#default_value' => (!empty($this->realex_cctype_accounts[$cctype]) ? $this->realex_cctype_accounts[$cctype] : 0),
        '#required' => FALSE,
        '#parents' => array($group, 'realex_cctype_accounts', $cctype),
      );
    }
  }

  function form(&$form, &$form_state) {
    parent::form($form, $form_state);
    $group = $this->pay_form->handler();
    $form[$group]['pay_method'][$this->pmid]['cc_currency'] = array(
      '#type' => 'textfield',
      '#title' => t('Currency'),
      '#access' => TRUE,
      '#required' => TRUE,
    );
  }

  function realex_trxtype($activity) {
    $trxtypes = array(
      'complete' => 'Sale',
    );
    return $trxtypes[$activity];
  }

  function cc_expiration() {
    if ($this->cc_exp_month && $this->cc_exp_year) {
      $exp = str_pad( (int) $this->cc_exp_month, 2, '0', STR_PAD_LEFT);
      $exp .= str_pad( (int) $this->cc_exp_year, 2, '0', STR_PAD_LEFT);
      return $exp;
    }
  }

  function set_cc_currency($value) {
    $this->cc_currency = $value;
  }

  function PayRealexRemote($response) {
    // Create and initialise XML parser
    $this->parser = xml_parser_create();
    xml_set_object($this->parser, $this);
    xml_set_element_handler($this->parser, 'startElement', 'endElement');
    xml_set_character_data_handler($this->parser, 'cDataHandler');

    // 1 = single field, 2 = array field, 3 = record container
    $this->field_type = array(
      'response' => 1,
      'orderid' => 1,
      'authcode' => 1,
      'result' => 2,
      'message' => 1,
      'pasref' => 1,
      'batchid' => 1,
      'md5hash' => 1,
      'sha1hash' => 1,
      'cvnresult' => 1,
    );

    xml_parse($this->parser, $response);
    xml_parser_free($this->parser);
  }

  /* The "startElement()" function is called when an open element tag is found.
   It creates a variable on the fly contructed of all the parent elements
   joined together with an underscore. So the following xml:

   <response><something>Owen</something></response>

   would create two variables:

   $RESPONSE and $RESPONSE_SOMETHING

   */
  function startElement($parser, $element, &$attrs) {
    $element = strtolower($element);
    if (isset($this->field_type[$element]) && $this->field_type[$element] != 0) {
      $this->current_field = $element;
    }
    else {
      $this->current_field = '';
    }
    if ($element == 'response' && $attrs['TIMESTAMP']) {
      $this->record['timestamp'] = $attrs['TIMESTAMP'];
    }
  }

  function endElement($parser, $element) {
    $element = strtolower($element);
    $this->current_field = '';
  }

  /* The "cDataHandler()" function is called when the parser encounters any text that's
   not an element. Simply places the text found in the variable that
   was last created. So using the XML example above the text "Owen"
   would be placed in the variable $RESPONSE_SOMETHING
   */

  function cDataHandler($parser, $text) {
    if (empty($this->current_field)) return;
    if ($this->field_type[$this->current_field] == 2) {
      $this->record[$this->current_field][] = $text;
    }
    elseif ($this->field_type[$this->current_field] === 1) {
      if (empty($this->record[$this->current_field])) {
        $this->record[$this->current_field] = $text;
      }
      else {
        $this->record[$this->current_field] .= $text;
      }
    }
  }
}

