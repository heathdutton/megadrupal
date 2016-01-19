<?php

namespace Drupal\moip\Commerce\FormControllers;

class MoipCheckoutTransparenteForm {

  public static function getDefinition($payment_method, $pane_values, $checkout_pane, $order) {

    $form = array();

    unset($_SESSION['moip']);

    $sandbox = variable_get('moip_server');
    $phone_token = variable_get('moip_phone_token', '');
    $name_token = variable_get('moip_name_token', '');
    $birthday_token = variable_get('moip_birthday_token', '');
    $cpf_token = variable_get('moip_cpf_token', '');

    if (empty($phone_token)) {
      throw new \Drupal\moip\Exceptions\MissingConfigurationException(t('Order @order_id did not provide "phone number" while processing moip_ct_form', array('@order_id' => $order->order_id)));
    }

    $moip_payment_token = '';
    if (!empty($order->commerce_customer_billing)) {
      $moip_class = variable_get('moip_class', 'Drupal\moip\MoipDrupal');
      if (class_exists($moip_class)) {
        $moipDrupal = new $moip_class();
        $moipDrupal->setOrder($order);
      }
      else {
        throw new \Drupal\moip\Exceptions\MissingConfigurationException(t('Moip subclass specified was not found: ' . $moip_class));
      }
      $moip_answer = $moipDrupal->processOrder('moip_ct');
      $moip_payment_token = $moip_answer->__get('token');
      if (empty($moip_payment_token)) {
        throw new \Drupal\moip\Exceptions\MissingConfigurationException(t('Order @order_id did not provide "moip token" while processing moip_ct_form', array('@order_id' => $order->order_id)));
      }
    }
    else {
      global $user;
      $error_msg = print_r($user, TRUE)
        . print_r($order, TRUE)
        . print_r($pane_values, TRUE)
        . print_r($checkout_pane, TRUE);
      throw new \Drupal\moip\Exceptions\MissingConfigurationException(t(
        'Order @order_id did not provide "commerce_customer_billing" while processing moip_ct_form: <pre>@pre</pre>', array(
        '@order_id' => $order->order_id,
        '@pre' => $error_msg
        )
      ));
    }

    drupal_add_css(drupal_get_path('module', 'moip') . '/assets/stylesheets/moip.css', 'file');

    $moipjs_path = 'sites/all/libraries/moipjs/moip.min.js';
    if (!file_exists($moipjs_path)) {
      throw new \Drupal\moip\Exceptions\MissingConfigurationException(t('MoIP JS library was not found on "' . $moipjs_path . '". Download it in !link', array('!link' => 'https://github.com/moiplabs/moipjs')));
    }

    if ($sandbox) {
      $js_src = $moipDrupal->getEnvironmentURL() . '/transparente/MoipWidget-v2.js';
    }
    else {
      $js_src = $moipDrupal->getEnvironmentURL() . '/transparente/MoipWidget-v2.js';
    }
    drupal_add_js($js_src, 'external');

    $form['#attached']['js'][] = array(
      'data' => $moipjs_path,
      'type' => 'file',
    );
    if ($sandbox) {
      $form['#attached']['js'][] = array(
        'data' => drupal_get_path('module', 'moip') . '/assets/js/moip.js',
        'type' => 'file',
      );
    }
    else {
      $form['#attached']['js'][] = array(
        'data' => drupal_get_path('module', 'moip') . '/assets/js/moip.min.js',
        'type' => 'file',
      );
    }
    $form['moip_ct'] = array(
      '#type' => 'container'
    );
    $form['moip_ct']['payment_way'] = array(
      '#type' => 'radios',
      '#title' => t('MoIP - Choose your payment way'),
      '#options' => array(
        'creditcard' => '<div class="title">' . t('Credit Card') . '</div><div class="payment-icon payment-icon-creditcard"></div>',
        'bankbillet' => '<div class="title">' . t('Bank Billet') . '</div><div class="payment-icon payment-icon-bankbillet"></div>',
        'banktransfer' => '<div class="title">' . t('Bank Transfer') . '</div><div class="payment-icon payment-icon-banktransfer"></div>',
      ),
      '#required' => TRUE,
    );
    $form['moip_ct']['payment_way_option'] = array(
      '#attributes' => array('class' => 'moip-ct-paymentwayoption'),
      '#type' => 'hidden',
    );
    $form['moip_ct']['answer'] = array(
      '#attributes' => array('class' => 'moip-ct-answer'),
      '#type' => 'hidden',
    );
    $form['moip_ct']['id_unico'] = array(
      '#type' => 'hidden',
      '#value' => $moipDrupal->getUniqueId(),
    );
    $form['moip_ct']['js_form'] = array(
      '#type' => 'markup',
      '#markup' => theme('moip_ct_js_form', array(
        'moip_payment_token' => $moip_payment_token,
        'order_id' => $order->order_id,
        'moip_user_phone' => token_replace($phone_token, array('commerce-order' => commerce_order_load($order->order_number))),
        'moip_user_name' => token_replace($name_token, array('commerce-order' => commerce_order_load($order->order_number))),
        'moip_user_birthday' => token_replace($birthday_token, array('commerce-order' => commerce_order_load($order->order_number))),
        'moip_user_cpf' => token_replace($cpf_token, array('commerce-order' => commerce_order_load($order->order_number))),
      ))
    );

    return $form;
  }

  public static function submit($payment_method, $pane_form, $pane_values, $order, $charge) {

    try {
      $payment_way = $pane_values['moip_ct']['payment_way'];
      $payment_way_option = $pane_values['moip_ct']['payment_way_option'];
      $moip_ct_answer = json_decode($pane_values['moip_ct']['answer']);

      if (variable_get('moip_debug', FALSE)) {
        watchdog('moipdbg_ct_answer', t('@payment_way_option: <pre>@pre</pre>', array(
          '@payment_way_option' => $payment_way_option,
          '@pre' => print_r($moip_ct_answer, TRUE)
        )));
      }

      if (empty($payment_way) || empty($moip_ct_answer)) {
        global $user;
        $error_msg = print_r($user, TRUE)
          . print_r($order, TRUE)
          . print_r($pane_values, TRUE)
          . print_r($moip_ct_answer, TRUE);
        throw new \ErrorException('moip_ct_submit - empty_data: <pre>' . $error_msg . '</pre>');
      }

      switch ($payment_way) {

        case 'creditcard':
          if (empty($payment_way_option)) {
            global $user;
            $error_msg = print_r($user, TRUE)
              . print_r($order, TRUE)
              . print_r($pane_values, TRUE)
              . print_r($moip_ct_answer, TRUE);
            throw new \ErrorException('moip_ct_submit - empty_payment_way_option: <pre>' . $error_msg . '</pre>');
          }
          $moip_data = array(
            'id_transacao' => $pane_values['moip_ct']['id_unico'],
            'valor' => str_replace('.', '', $moip_ct_answer->TotalPago),
            'cod_moip' => $moip_ct_answer->CodigoMoIP,
            'forma_pagamento' => 3,
            'tipo_pagamento' => 'CartaoDeCredito',
            'parcelas' => 1,
            'email_consumidor' => $order->mail
          );
          switch ($moip_ct_answer->Status) {
            case 'Iniciado':
              $moip_data['status_pagamento'] = MOIP_STATUS_INITIALIZED;
              break;
            case 'EmAnalise':
              $moip_data['status_pagamento'] = MOIP_STATUS_PENDING;
              break;
            case 'Autorizado':
              $moip_data['status_pagamento'] = MOIP_STATUS_AUTHORIZED;
              break;
            case 'Cancelado':
              $moip_data['status_pagamento'] = MOIP_STATUS_CANCELED;
              break;
          }
          $MoipNasp = new \Drupal\moip\Entity\MoipNasp($moip_data);
          $MoipNasp->processMoipData();
          break;

        case 'bankbillet':
          commerce_order_status_update(
            $order, COMMERCE_PAYMENT_STATUS_PENDING, FALSE, TRUE
            , t('Order was processed by moip_ct with bankbillet and is waiting for response from MoIP')
          );
          break;

        case 'banktransfer':
          if (empty($payment_way_option)) {
            global $user;
            $error_msg = print_r($user, TRUE)
              . print_r($order, TRUE)
              . print_r($pane_values, TRUE)
              . print_r($moip_ct_answer, TRUE);
            throw new \ErrorException('moip_ct_submit - empty_payment_way_option: <pre>' . $error_msg . '</pre>');
          }
          commerce_order_status_update(
            $order, COMMERCE_PAYMENT_STATUS_PENDING, FALSE, TRUE
            , t('Order was processed by moip_ct with banktransfer and is waiting for response from Moip')
          );
          break;
      }

      $order->data['moip_ct_payment_method'] = $payment_way;
      $order->data['moip_ct_payment_method_option'] = $payment_way_option;
      commerce_order_save($order);
      drupal_goto('checkout/' . $order->order_id . '/complete');
    } catch (\ErrorException $e) {
      watchdog_exception('moip_error', $e);
      drupal_set_message(t('There was an error with MoIP. Please try again later.'), 'error');
    } catch (\Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }

}
