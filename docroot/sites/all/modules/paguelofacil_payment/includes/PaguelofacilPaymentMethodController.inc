<?php

/**
 * PagueloFacil Payment controller
 */
class PaguelofacilPaymentMethodController extends PaymentMethodController {

  /**
  * The production checkout URL.
  */
  const URL_CHECKOUT_PRODUCTION = 'https://secure.paguelofacil.com/LinkDeamon.cfm';

  public $payment_method_configuration_form_elements_callback = 'paguelofacil_payment_payment_method_configuration_form_elements';

  public $controller_data_defaults = array(
    'cclw' => ''
  );

  public function __construct() {
    $this->title = t('PagueloFacil Payment Button');
  }

 
  /**
   * Form build callback: implements
   * PaymentMethodController::payment_method_configuration_form_elements_callback.
   */
  public function payment_method_configuration_form_elements(array $form, array &$form_state) {
    $payment_method = $form_state['payment_method'];
    $controller = $payment_method->controller;
    $controller_data = $payment_method->controller_data + $controller->controller_data_defaults;

    $elements['authentication'] = array(
      '#title' => t('Authentication'),
      '#type' => 'fieldset',
    );
    $elements['authentication']['cclw'] = array(
      '#default_value' => $controller_data['cclw'],
      '#title' => t('Key (CCLW)'),
      '#required' => TRUE,
      '#type' => 'textfield',
    );

    return $elements;
  }

  /**
   * Implements PaymentMethodController::execute().
   */
  function execute(Payment $payment) {

    $description = t('Payment to @site_name', array('@site_name' => variable_get('site_name')));
    $values = array(
      'CCLW' => $payment->method->controller_data['cclw'],
      'CMTN' => $payment->totalAmount(TRUE),
      'CDSC' => $description,
      'PID' => $payment->pid,
    );

    $options = array('query' => $values);

    drupal_goto(self::URL_CHECKOUT_PRODUCTION, $options);
  }
}
