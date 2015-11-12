<?php
/**
 * @file
 * Integration with Mercado Pago SDK.
 */

/**
 * Define the configuration form of the payment Mercado Pago.
 */
function mercado_pago_payment_sdk_settings_form($settings = NULL) {
  $form = array();

  $form['client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Identificación del Client (CLIENT ID)'),
    '#description' => t('Número de cuenta. Averígualo en la configuración avanzada de MercadoPago.'),
    '#default_value' => (isset($settings['client_id'])) ? trim($settings['client_id']) : '',
    '#required' => TRUE,
  );

  $form['client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Clave Secreta del Client (CLIENT SECRET)'),
    '#description' => t('Averígualo en la configuración avanzada de MercadoPago.'),
    '#default_value' => (isset($settings['client_secret'])) ? trim($settings['client_secret']) : '',
    '#required' => TRUE,
  );

  $form['mp_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Opening methods'),
    '#description' => t('Choose how to open the payment process according to the type of window that you prefer for your website:'),
    '#options' => array(
      'modal' => t('Window modal'),
      'popup' => t('Window pop-up'),
      'blank' => t('New window or tab'),
      'redirect' => t('Same window'),
      'iframe' => t('Insert as an iframe'),
    ),
    '#default_value' => (isset($settings['mp_mode'])) ? $settings['mp_mode'] : 'modal',
  );

  $form['sandbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sandbox mode'),
    '#description' => t('If you want to test it, you can use check the Sandbox mode.'),
    '#default_value' => (isset($settings['sandbox'])) ? trim($settings['sandbox']) : FALSE,
    '#required' => FALSE,
  );

  $form['information'] = array(
    '#type' => 'textarea',
    '#title' => t('Información para el usuario'),
    '#description' => t('Información que se desplegará al momento de seleccionar Mercado Pago en las formas de pago durante el proceso de Checkout.'),
    '#default_value' => (isset($settings['information'])) ? trim($settings['information']) : '',
    '#required' => TRUE,
  );

  $form['methods'] = array(
    '#type' => 'fieldset',
    '#title' => t('Exclude Payment Methods'),
    '#description' => t('Configure which Payment Methods will be allowed on your site. But first select your country'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (($library = libraries_detect('mp_sdk')) && !empty($library['installed'])) {
    // The library is installed. Awesome!
    $form['methods']['country'] = array(
      '#type' => 'select',
      '#title' => t('Country Account'),
      '#description' => t('Choose the country where you account was created'),
      '#options' => mercado_pago_payment_allowed_countries(),
      '#default_value' => (isset($settings['methods']['country'])) ? $settings['methods']['country'] : '',
      '#ajax' => array(
        'callback' => 'mercado_pago_payment_sdk_form_ajax_callback',
        'wrapper' => 'excluded_payment_methods-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    $form['methods']['excluded_payment_methods'] = array(
      '#type' => 'checkboxes',
      '#title' => t("Payment methods to be excluded"),
      '#options' => array(),
      '#prefix' => '<div id="excluded_payment_methods-div">',
      '#suffix' => '</div>',
      '#description' => t('Payment methods depends on what country your account was created, if you change the country, save the module first and just after that select the Exclude Payment Methods.'),
    );

    if (!empty($settings['methods']['country'])) {
      $methods = mercado_pago_payment_methods_list($settings['methods']['country']);
      if (!empty($methods)) {
        $form['methods']['excluded_payment_methods']['#options'] = mercado_pago_payment_methods_list($settings['methods']['country']);
        $form['methods']['excluded_payment_methods']['#default_value'] = isset($settings['methods']['excluded_payment_methods']) ?
        $settings['methods']['excluded_payment_methods'] : '';
      }
      else {
        $form['methods']['excluded_payment_methods']['#description'] = '<p>' . t('These country does not have any allowable payment method') . '</p>' .
          $form['methods']['excluded_payment_methods']['#description'];
      }
    }
    else {
      $form['methods']['excluded_payment_methods']['#description'] = '<p>' . t('Please, select a country if you want to exlude a Payment Method.') . '</p>' .
        $form['methods']['excluded_payment_methods']['#description'];
    }
  }
  else {
    // This contains a detailed (localized) error message.
    $error_message = $library['error message'];
    $form['methods']['error_message'] = array(
      '#markup' => $error_message,
    );
    drupal_set_message(t('Something went wrong trying to load Mercado Pago SDK library. Message: !message', array('!message' => $error_message)), 'error');
  }
  return $form;
}

/**
 * Ajax callback for generating the options for disallowed payment methods.
 */
function mercado_pago_payment_sdk_form_ajax_callback(&$form, &$form_state) {
  return $form['parameter']['payment_method']['settings']['payment_method']['settings']['methods']['excluded_payment_methods'];
}

/**
 * Payment method callback: adds a message and CSS to the submission form.
 */
function mercado_pago_payment_sdk_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form['mp_information'] = array(
    '#markup' => $payment_method['settings']['information'],
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'mercado_pago_payment') . '/theme/mercado_pago_payment.css'),
    ),
  );

  return $form;
}

/**
 * Implementation of CALLBACK_redirect_form().
 */
function mercado_pago_payment_sdk_redirect_form($form, &$form_state, $order, $payment_method) {
  $url_return = url('checkout/' . $order->order_id . '/payment/return/' .
    $order->data['payment_redirect_key'], array('absolute' => TRUE));
  $url_back = url('checkout/' . $order->order_id . '/payment/back/' .
    $order->data['payment_redirect_key'], array('absolute' => TRUE));

  // Adds js for automatically clicking the Pay button and for redirecting
  // the site after closing the modal window.
  drupal_add_js(
    "function mercado_pago_onreturn(json) {
      if (json.collection_status=='approved' ||
          json.collection_status=='pending' ||
          json.collection_status=='in_process'){    
        document.location.href='$url_return';
      } else {
        document.location.href='$url_back';
      } 
    }
    function fireEvent(obj,evt){
        var fireOnThis = obj;
        if( document.createEvent ) {
           var evObj = document.createEvent('MouseEvents');
           evObj.initEvent( evt, true, false );
           fireOnThis.dispatchEvent( evObj );
        } else if( document.createEventObject ) {
          var evObj = document.createEventObject();
          fireOnThis.fireEvent( 'on' + evt, evObj );
        }
     }
    fireEvent(document.getElementById(\"btnPagar\"), 'click')
  ", array('type' => 'inline', 'scope' => 'footer'));

  // Getting currency code
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $currency_code = $wrapper->commerce_order_total->currency_code->value();

  // Getting unity price.
  $amount = $wrapper->commerce_order_total->amount->value();

  // If provided, get the cart name and cart surname.
  $cart_name = '';
  $cart_surname = '';
  if(!empty($order->commerce_customer_billing) && !empty($order_wrapper->commerce_customer_billing->commerce_customer_address)){
    $billing_address = $wrapper->commerce_customer_billing->commerce_customer_address->value();
    if (empty($billing_address['first_name'])) {
      $name_parts = explode(' ', $billing_address['name_line']);
      $billing_address['first_name'] = array_shift($name_parts);
      $billing_address['last_name'] = implode(' ', $name_parts);
    }
    $cart_name = $billing_address['first_name'];
    $cart_surname = $billing_address['last_name'];
  }

  // Setting preference for MP.
  $preference = array(
    'items' => array(
      array(
        'title' => t('Order @order_number at @store',
          array(
            '@order_number' => $order->order_number,
            '@store' => variable_get('site_name', url('<front>', array('absolute' => TRUE))),
          )
        ),
        'quantity' => 1,
        'currency_id' => $currency_code,
        'unit_price' => round(commerce_currency_amount_to_decimal($amount, $currency_code), 2),
        'id' => $order->order_number,
        'category_id' => 'others',
      ),
    ),
    'payer' => array(
      'name' => $cart_name,
      'surname' => $cart_surname,
      'email' => $order->mail,
    ),
    'back_urls' => array(
      'success' => $url_return,
      'failure' => $url_back,
      'pending' => $url_return,
    ),
    // TODO. Learn how to use this notification URL, if enable I get an IPN notification, but I dont know how to use it.
    // 'notification_url' => url('mercado_pago_payment_sdk/ipn/' . $payment_method['instance_id'], 
    //   array('absolute' => TRUE)),
    'external_reference' => $order->order_id . '-' . REQUEST_TIME,
  );

  // Exclude payment methods.
  $settings = $payment_method['settings'];
  if (isset($settings['methods']['excluded_payment_methods'])) {
    $methods = array();
    foreach ($settings['methods']['excluded_payment_methods'] as $method) {
      $methods[] = array('id' => $method);
    }
    $preference['payment_methods'] = array('excluded_payment_methods' => $methods);
  }

  // If commerce shipping is enable append shipping information.
  if (module_exists('commerce_shipping')) {
    if (!empty($wrapper->commerce_customer_shipping->commerce_customer_address)) {
      $shipping_address = $wrapper->commerce_customer_shipping->commerce_customer_address->value();

      // Ensure there's a name_line.
      if (empty($shipping_address['name_line'])) {
        $shipping_address['name_line'] = $shipping_address['first_name'] . ' ' .
          $shipping_address['last_name'];
      }

      // From the oficial MercadoPago modules, the shipping street is as follows.
      $streetname = substr($shipping_address['thoroughfare'], 0, 100) . ' - ' .
        substr($shipping_address['premise'], 0, 100) . ' - ' .
        substr($shipping_address['locality'], 0, 40) . ' - ' .
        substr($shipping_address['administrative_area'], 0, 40);

      $preference['shipments'] = array(
        "receiver_address" => array(
          "floor" => "-",
          "zip_code" => substr($shipping_address['postal_code'], 0, 20),
          "street_name" => $streetname,
          "street_number" => "-",
          "apartment" => "-",
          "street_number" => "-",
        ),
      );

      // Change payer address to the shipping address.
      $preference['payer']['address'] = array(
        "floor" => "-",
        "zip_code" => substr($shipping_address['postal_code'], 0, 20),
        "street_name" => $streetname,
        "apartment" => "-",
        "street_number" => "-",
      );
    }
  }

  // Loading MercadoPago SDK.
  libraries_load('mp_sdk');
  $mp = new MP($settings["client_id"], $settings["client_secret"]);

  // Creating the preferences.
  try{
    $preference_result = $mp->create_preference($preference);
  }catch (Exception $e) {
    // In case of comunication error, notify the user and create a back button.
    watchdog('mercado_pago_sdk', 'Comuncation with MercadoAPI fail. <br /> Message: <pre>!message</pre>',
      array('!message' => $e->getMessage()), WATCHDOG_ERROR);
    drupal_set_message(t('Comunication with the API of Mercado fail. Please, contact the administrator.'), "error");
    $form['mp_error'] = array(
      '#markup' => '<div class="commerce-mercado-pago-cancel">' .
        l(t('Cancel payment and go back'), $url_back) . '</div>'
      );
    return $form;
  }

  // If sandbox is enable switch to sandbox_init_point
  $url = $preference_result['response']['init_point'];
  if ($settings['sandbox']) {
    $url = $preference_result['response']['sandbox_init_point'];
  }

  // If not set mp-mode set it to modal.
  if (!isset($settings['mp_mode'])) {
    $settings['mp_mode'] = 'modal';
  }

  // Determine the form mode.
  switch ($settings['mp_mode']) {
    // For IFRAME, create an iframe.
    case 'iframe':
      $form["mp_iframe"] = array(
        '#markup' => implode('',
          array(
            '<iframe src="',
            $url,
            '" name="MP-Checkout" width="740" height="600" frameborder="0"></iframe>',
            mercado_pago_payment_get_script(),
          )
        ),
      );

      // Add a link to cancel payment process.
      $form['mp_iframe']['#suffix'] = '<div class="commerce-mercado-pago-cancel">' .
        l(t('Cancel payment and go back'), $url_back) . '</div>';
      break;

    // For redirect, redirect to the init_point.
    case 'redirect':
      drupal_goto($url);
      break;

    // For any value else, create a button with different mp-mode.
    default:
      $form["mp_link_button"] = array(
        '#markup' => implode('',
          array(
            '<a href="',
            $url,
            '" id="btnPagar" name="MP-payButton" mp-mode="',
            $settings['mp_mode'],
            '" class="lightblue-M-Ov-ArOn" onreturn="mercado_pago_onreturn">',
            t('Pay'),
            '</a>',
            '<script type="text/javascript" src="https://www.mercadopago.com/org-img/jsapi/mptools/buttons/render.js"></script>',
          )
        ),
      );
  }

  return $form;
}

/**
 * Processes an incoming IPN.
 */
function _mercado_pago_payment_sdk_validate_ipn($payment_method = NULL) {
  drupal_add_http_header('Content-Type', 'text/plain');
  $ipn = $_GET;

  if (empty($ipn)) {
    watchdog('mercado_pago_sdk', 'IPN URL accessed with no POST data submitted.', array(), WATCHDOG_WARNING);
    drupal_add_http_header('Status', '401 ERROR');
    echo "Error:174";
    exit;
  }

  if (!isset($ipn["topic"]) || $ipn["topic"] != "payment") {
    watchdog('mercado_pago_sdk', 'IPN URL accessed with no "topic" or not set to "payment" on POST data submitted.',
      array(), WATCHDOG_WARNING);
    drupal_add_http_header('Status', '401 ERROR');
    echo "Error:182";
    exit;
  }

  if (!isset($ipn["id"])) {
    watchdog('mercado_pago_payment_sdk', 'IPN URL accessed with no id on POST data submitted.',
      array(), WATCHDOG_WARNING);
    drupal_add_http_header('Status', '401 ERROR');
    echo "Error:190";
    exit;
  }

  // If no payment_method, load default instance.
  if (empty($payment_method)) {
    $payment_method = commerce_payment_method_instance_load(
      "mercado_pago_payment_sdk|commerce_payment_mercado_pago_payment_sdk");
  }

  $settings = $payment_method['settings'];

  libraries_load('mp_sdk');
  $mp = new MP($settings["client_id"], $settings["client_secret"]);

  // Check if ipn is in sandbox mode.
  if (isset($ipn['sandbox']) && $ipn['sandbox'] == 'true') {
    watchdog('mercado_pago_sdk', 'IPN sandbox mode', array(), WATCHDOG_NOTICE);
    $mp->sandbox_mode(TRUE);
  }
  else {
    $mp->sandbox_mode(FALSE);
  }

  try {
    $payment_info = $mp->get_payment_info('' . $ipn["id"]);
  }
  catch (Exception $e) {
    watchdog('mercado_pago_sdk', 'An error occurred when processing payment with id = %id.<br /> Message: <pre>!message</pre>',
      array('%id' => $ipn["id"], '!message' => $e->getMessage()), WATCHDOG_ERROR);
    drupal_add_http_header('Status', '401 ERROR');
    echo 'Caught exception: ',  $e->getMessage(), "\n";
    exit;
  }

  if ($payment_info['status'] != '200') {
    watchdog('mercado_pago_sdk', 'An error occurred when processing payment with id = %id.',
      array('%id' => $ipn["id"]), WATCHDOG_ERROR);
    drupal_add_http_header('Status', '401 ERROR');
    echo "Error:199";
    exit;
  }

  list($order_id) = explode('-', $payment_info['response']['collection']['external_reference']);

  if (!$order_id || empty($order_id)) {
    watchdog('mercado_pago_sdk', 'La información enviada no pudo ser procesada, el parametro "external_reference" es no valido. <pre>%posted_info</pre>',
      array('%posted_info' => print_r($payment_info, TRUE)), WATCHDOG_INFO);
    drupal_add_http_header('Status', '401 ERROR');
    echo "Error:208";
    exit;
  }

  $order = commerce_order_load($order_id);
  if ($order == False) {
    watchdog('mercado_pago_sdk', 'Error when trying to load order #%order_id referred in PayPal IPN notification (id = %id).<br /> Order #%order_id do not exist. <br />IPN info: <pre>%message</pre>',
      array('%order_id' => $order_id, '%id' => $ipn["id"], '%message' => print_r($payment_info, TRUE)), WATCHDOG_ERROR);
    drupal_add_http_header('Status', '401 ERROR');
    echo 'Caught exception: Order do not exist', "\n";
    exit;
  }

  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $currency_code = $wrapper->commerce_order_total->currency_code->value();

  $transaction = NULL;
  $transactions = commerce_payment_transaction_load_multiple(array(), array('remote_id' => $ipn["id"]));
  if (!empty($transactions)) {
    $transaction = current($transactions);
  }

  if ($transaction == NULL) {
    $transaction = commerce_payment_transaction_new('mercado_pago_payment_sdk', $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
  }

  // Verify that the order and IPN notificacion have the same currency code, or else, use IPNs
  $mp_currency_code = $payment_info['response']['collection']['currency_id'];
  if ($currency_code != $mp_currency_code) {
    watchdog('mercado_pago_sdk', t('The currency code is different between MP(%mp_code) and your order(%order_code). Using %mp_code .'), 
      array('%mp_code'=>$mp_currency_code, '%order_code'=>$currency_code), WATCHDOG_NOTICE);
    $currency_code = $mp_currency_code;
  }

  $transaction->remote_id = $ipn["id"];
  $transaction->amount = commerce_currency_decimal_to_amount($payment_info['response']['collection']['transaction_amount'], $currency_code);
  $transaction->currency_code = $currency_code;
  $transaction->payload[REQUEST_TIME] = $payment_info;

  switch ($payment_info['response']['collection']['status']) {
    case 'approved':
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = 'Pago acreditado';
      break;

    case 'cancelled':
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'Mercadopago cancelado';
      break;

    case 'in_mediation':
      $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
      $transaction->message = 'El pago entró en disputa';
      break;

    case 'in_process':
      $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
      $transaction->message = 'Su Mercadopago está pendiente de acreditación';
      break;

    case 'pending':
      $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
      $transaction->message = 'Mercadopago pendiente, aún no se realiza deposito';
      break;

    case 'refunded':
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'Pago reintegrado a la contraparte';
      break;

    case 'rejected':
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'Mercadopago rechazado';
      break;

    case 'uninitialized':
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'Se abandonó el pago en el medio del proceso';
      break;

    default:
      // No action.
      break;

  }

  commerce_payment_transaction_save($transaction);
  // $posted_info['transaction_id'] = $transaction->transaction_id;
  watchdog('mercado_pago_sdk', 'El IPN de MercadoLibre fue procesado para la orden @order_number con transacción ID @txn_id.',
    array(
      '@txn_id' => $transaction->transaction_id,
      '@order_number' => $order->order_number,
    ),
    WATCHDOG_INFO);
  drupal_add_http_header('Status', '200 OK');
  echo "Success";

  commerce_payment_redirect_pane_next_page($order);
}

/**
 * Return the javascript required by MercadoPago.
 */
function mercado_pago_payment_get_script() {
  return '
  <script type="text/javascript">
      (function(){function $MPBR_load(){window.$MPBR_loaded !== true && (function(){var s = document.createElement("script");s.type = "text/javascript";s.async = true;
      s.src = ("https:"==document.location.protocol?"https://www.mercadopago.com/org-img/jsapi/mptools/buttons/":"http://mp-tools.mlstatic.com/buttons/")+"render.js";
      var x = document.getElementsByTagName(\'script\')[0];x.parentNode.insertBefore(s, x);window.$MPBR_loaded = true;})();}
      window.$MPBR_loaded !== true ? (window.attachEvent ? window.attachEvent(\'onload\', $MPBR_load) : window.addEventListener(\'load\', $MPBR_load, false)) : null;})();
  </script> ';

}
