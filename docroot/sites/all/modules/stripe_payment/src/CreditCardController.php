<?php

namespace Drupal\stripe_payment;

class CreditCardController extends \PaymentMethodController implements \Drupal\webform_paymethod_select\PaymentRecurrentController {
  public $controller_data_defaults = array(
    'private_key' => '',
    'public_key'  => '',
    'config' => array(
      'field_map' => array(),
    ),
  );

  public function __construct() {
    $this->title = t('Stripe Credit Card');
    $this->form = new CreditCardForm();

    $this->payment_configuration_form_elements_callback = 'payment_forms_method_form';
    $this->payment_method_configuration_form_elements_callback = '\Drupal\stripe_payment\configuration_form';
  }

  /**
   * {@inheritdoc}
   */
  function validate(\Payment $payment, \PaymentMethod $payment_method, $strict) {
    parent::validate($payment, $payment_method, $strict);

    if (!($library = libraries_detect('stripe-php')) || empty($library['installed'])) {
      throw new \PaymentValidationException(t('The stripe-php library could not be found.'));
    }
  }

  public function execute(\Payment $payment) {
    libraries_load('stripe-php');

    $context = &$payment->contextObj;
    $api_key = $payment->method->controller_data['private_key'];

    switch ($context->value('donation_interval')) {
      case 'm': $interval = 'month'; break;
      case 'y': $interval = 'year'; break;
      default:  $interval = NULL; break;
    }

    try {
      \Stripe::setApiKey($api_key);
      \Stripe::setApiVersion('2014-01-31');

      $customer = $this->createCustomer(
        $payment->method_data['stripe_payment_token'],
        $this->getName($context),
        $context->value('email')
      );

      $stripe  = NULL;
      $plan_id = NULL;
      if (!$interval) {
        $stripe = $this->createCharge($customer, $payment);
      } else {
        $plan_id = $this->createPlan($customer, $payment, $interval);
        $stripe  = $this->createSubscription($customer, $plan_id);
      }

      $payment->setStatus(new \PaymentStatusItem(PAYMENT_STATUS_SUCCESS));
      entity_save('payment', $payment);
      $params = array(
        'pid'       => $payment->pid,
        'stripe_id' => $stripe->id,
        'type'      => $stripe->object,
        'plan_id'   => $plan_id,
      );
      drupal_write_record('stripe_payment', $params);
    }
    catch(\Stripe_Error $e) {
      $payment->setStatus(new \PaymentStatusItem(PAYMENT_STATUS_FAILED));
      entity_save('payment', $payment);

      $message =
        '@method payment method encountered an error while contacting ' .
        'the stripe server. The status code "@status" and the error ' .
        'message "@message". (pid: @pid, pmid: @pmid)';
      $variables = array(
        '@status'   => $e->getHttpStatus(),
        '@message'  => $e->getMessage(),
        '@pid'      => $payment->pid,
        '@pmid'     => $payment->method->pmid,
        '@method'   => $payment->method->title_specific,
      );
      watchdog('stripe_payment', $message, $variables, WATCHDOG_ERROR);
    }
  }


  public function createCustomer($token, $description, $email) {
    return \Stripe_Customer::create(array(
        'card'        => $token,
        'description' => $description,
        'email'       => $email
      ));
  }

  public function getTotalAmount(\Payment $payment) {
    // convert amount to cents. Integer value.
    return (int) ($payment->totalAmount(0) * 100);
  }

  public function createCharge($customer, $payment) {
    return \Stripe_Charge::create(array(
        'customer' => $customer->id,
        'amount'   => $this->getTotalAmount($payment),
        'currency' => $payment->currency_code
      ));
  }

  public function createPlan($customer, $payment, $interval) {
    $amount = $this->getTotalAmount($payment);
    $currency = $payment->currency_code;
    $description = ($amount/100) . ' ' . $currency . ' / ' . $interval;

    $existing_id = db_select('stripe_payment_plans', 'p')
      ->fields('p', array('id'))
      ->condition('payment_interval', $interval)
      ->condition('amount', $amount)
      ->condition('currency', $currency)
      ->execute()
      ->fetchField();

    if ($existing_id) {
      return $existing_id;
    } else {
      $params = array(
        'id'       => $description,
        'amount'   => $amount,
        'payment_interval' => $interval,
        'name'     => 'donates ' . $description,
        'currency' => $currency,
      );
      drupal_write_record('stripe_payment_plans', $params);

      // This ugly hack is necessary because 'interval' is a reserved keyword
      // in mysql and drupal does not enclose the field names in '"'.
      $params['interval'] = $params['payment_interval'];
      unset($params['payment_interval']);
      unset($params['pid']);
      return \Stripe_Plan::create($params)->id;
    }
  }

  public function createSubscription($customer, $plan_id) {
    return $customer->subscriptions->create(array('plan' => $plan_id));
  }

  public function getName($context) {
    return trim(
      $context->value('title') . ' ' .
      $context->value('first_name') . ' ' .
      $context->value('last_name')
    );
  }

  /**
   * Helper for entity_load().
   */
  public static function load($entities) {
    $pmids = array();
    foreach ($entities as $method) {
      if ($method->controller instanceof CreditCardController) {
        $pmids[] = $method->pmid;
      }
    }
    if ($pmids) {
      $query = db_select('stripe_payment_payment_method_controller', 'controller')
        ->fields('controller')
        ->condition('pmid', $pmids);
      $result = $query->execute();
      while ($data = $result->fetchAssoc()) {
        $method = $entities[$data['pmid']];
        unset($data['pmid']);
        $data['config'] = unserialize($data['config']);
        $method->controller_data = (array) $data;
        $method->controller_data += $method->controller->controller_data_defaults;
      }
    }
  }

  /**
   * Helper for entity_insert().
   */
  public function insert($method) {
    $method->controller_data += $this->controller_data_defaults;
    $data = $method->controller_data;
    $data['pmid'] = $method->pmid;
    $data['config'] = serialize($data['config']);

    $query = db_insert('stripe_payment_payment_method_controller');
    $query->fields($data);
    $query->execute();
  }

  /**
   * Helper for entity_update().
   */
  public function update($method) {
    $data = $method->controller_data;
    $data['config'] = serialize($data['config']);
    $query = db_update('stripe_payment_payment_method_controller');
    $query->fields($data);
    $query->condition('pmid', $method->pmid);
    $query->execute();
  }

  /**
   * Helper for entity_delete().
   */
  public function delete($method) {
    db_delete('stripe_payment_payment_method_controller')
      ->condition('pmid', $method->pmid)
      ->execute();
  }
}

function configuration_form(array $form, array &$form_state) {
  $cd = drupal_array_merge_deep(array(
    'private_key' => '',
    'public_key' => '',
    'config' => array('field_map' => array()),
  ), $form_state['payment_method']->controller_data);

  $library = libraries_detect('stripe-php');
  if (empty($library['installed'])) {
    drupal_set_message($library['error message'], 'error', FALSE);
  }

  $form['private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key'),
    '#description' => t('Available from Your Account / Settings / API keys on stripe.com'),
    '#required' => true,
    '#default_value' => $cd['private_key'],
  );

  $form['public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Public key'),
    '#description' => t('Available from Your Account / Settings / API keys on stripe.com'),
    '#required' => true,
    '#default_value' => $cd['public_key'],
  );

  $form['config']['field_map'] = array(
    '#type' => 'fieldset',
    '#title' => t('Personal data mapping'),
    '#description' => t('This setting allows you to map data from the payment context to stripe fields. If data is found for one of the mapped fields it will be transferred to stripe. Use a comma to separate multiple field keys.'),
  );

  $map = $cd['config']['field_map'];
  foreach (CreditCardForm::extraDataFields() as $name => $field) {
    $default = implode(', ', isset($map[$name]) ? $map[$name] : array());
    $form['config']['field_map'][$name] = array(
      '#type' => 'textfield',
      '#title' => $field['#title'],
      '#default_value' => $default,
    );
  }

  return $form;
}

function configuration_form_validate(array $element, array &$form_state) {
  $cd = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  foreach ($cd['config']['field_map'] as $k => &$v) {
    $v = array_filter(array_map('trim', explode(',', $v)));
  }

  $library = libraries_detect('stripe-php');
  if (empty($library['installed'])) {
    drupal_set_message($library['error message'], 'error', FALSE);
  }

  if (substr($cd['private_key'], 0, 3) != 'sk_') {
    form_error($element['private_key'], t('Please enter a valid private key (starting with sk_).'));
  }
  else {
    libraries_load('stripe-php');
    try {
      \Stripe_Account::retrieve($cd['private_key']);
    }
    catch(\Stripe_Error $e) {
      $values = array(
        '@status'   => $e->getHttpStatus(),
        '@message'  => $e->getMessage(),
      );
      $msg = t('Unable to contact stripe using this set of keys: HTTP @status: @message.', $values);
      form_error($element['private_key'], $msg);
    }
  }
  if (substr($cd['public_key'], 0, 3) != 'pk_') {
    form_error($element['public_key'], t('Please enter a valid public key (starting with pk_).'));
  }

  $form_state['payment_method']->controller_data = $cd;
}
