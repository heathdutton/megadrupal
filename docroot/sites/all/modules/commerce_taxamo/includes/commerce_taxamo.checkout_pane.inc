<?php

/**
 * @file
 * Functions used by Commerce Pane.
 */

/**
 * Callback function used by commerce pane.
 */
function commerce_taxamo_taxamo_validation_pane_settings_form($checkout_pane) {
  $form = array();

  return $form;
}

/**
 * Callback function used by commerce pane.
 */
function commerce_taxamo_taxamo_validation_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $form = array();

  if (empty($order->data['taxamo'])) {
    $order->data['taxamo'] = commerce_taxamo_reset_taxamo_data();
  }
  $taxamo_data = &$order->data['taxamo'];
  $taxamo_data['allow_vat_calculation'] = FALSE;
  $taxamo_data['force_country'] = FALSE;

  if ($taxamo = commerce_taxamo_get_taxamo_php_class('private')) {
    $locate_my_ip = commerce_taxamo_locate_my_ip();
    $taxamo_data['ip_country'] = $locate_my_ip['country_code'];
    $taxamo_data['billing_country'] = commerce_taxamo_get_billing_country($order);

    if (!empty($taxamo_data['billing_country']) && strtoupper($taxamo_data['billing_country']) == strtoupper($taxamo_data['ip_country'])) {
      // IP country and Billing country match... that's enough evidence to proceed.
      $taxamo_data['billing_country'] = strtoupper($taxamo_data['billing_country']);
      $taxamo_data['ip_country'] = strtoupper($taxamo_data['ip_country']);

      $form['commerce_taxamo_message'] = array(
        '#markup' => t("Your country has been validated to calculate your taxes."),
      );
      $taxamo_data['allow_vat_calculation'] = TRUE;
    }
    else {
      // IP country and Billing country do not match... lets check if 1 piece of evidence is enough.
      if (commerce_taxamo_get_dashboard_settings('allow_contradictory_evidence')) {
        // 1 piece of evidence is enough... let the customer continue with the checkout.
        $form['commerce_taxamo_message'] = array(
          '#markup' => t("The information needed to calculate your taxes has been validated."),
        );
        $taxamo_data['allow_vat_calculation'] = TRUE;
      }
      else {
        // 1 piece of evidence is not enough... lets check if Self Declaration is enabled.
        if (commerce_taxamo_get_dashboard_settings('allow_self_declaration')) {
          // STEPS:
          // 0: Initial step
          // 10: Country selected
          // 20: Mobile phone provided
          // 30: SMS token provided

          $current_step = 0;
          if (!empty($form_state['values']['taxamo_validation']['commerce_taxamo_self_declaration'])) {
            $current_step = 10;
          }
          if (!empty($form_state['values']['taxamo_validation']['commerce_taxamo_sms_number']) && commerce_taxamo_valid_mobile_number($form_state['values']['taxamo_validation']['commerce_taxamo_sms_number']) && !empty($taxamo_data['token_sent'])) {
            $current_step = 20;
          }

          $countries_dict = commerce_taxamo_countries();
          $countries = array();
          foreach ($countries_dict as $key => $country) {
            $countries[$key] = $country->name;
          }
          if ($current_step < 10) {
            $default_commerce_taxamo_self_declaration = 0;
            if (!empty($taxamo_data['self_declaration_country'])) {
              $default_commerce_taxamo_self_declaration = $taxamo_data['self_declaration_country'];
            }
            $form['commerce_taxamo_self_declaration'] = array(
              '#title' => t("Country of Residence"),
              '#type' => 'select',
              '#options' => $countries,
              '#default_value' => $default_commerce_taxamo_self_declaration,
              '#required' => TRUE,
            );
          }
          else {
            $self_declaration_value = $form_state['values']['taxamo_validation']['commerce_taxamo_self_declaration'];
            $form['commerce_taxamo_self_declaration'] = array(
              '#type' => 'value',
              '#value' => $self_declaration_value,
            );
            $form['commerce_taxamo_self_declaration_info'] = array(
              '#title' => t("Country of Residence"),
              '#type' => 'item',
              '#markup' => $countries[$self_declaration_value],
            );

            if (commerce_taxamo_get_dashboard_settings('allow_sms_verification')) {
              $calling_code = "(+" . $countries_dict[$self_declaration_value]->callingCode[0] . ") ";
              if ($current_step < 20) {
                $taxamo_data['token_sent'] = FALSE;
                $form['commerce_taxamo_sms_number'] = array(
                  '#title' => t("Mobile phone"),
                  '#type' => 'textfield',
                  '#field_prefix' => $calling_code,
                  '#required' => TRUE,
                  '#description' => t("A SMS message with a token will be sent to this mobile number. In the next step you will have to provide the SMS Token to continue."),
                );
              }
              else {
                $sms_number_value = $form_state['values']['taxamo_validation']['commerce_taxamo_sms_number'];
                $form['commerce_taxamo_sms_number'] = array(
                  '#type' => 'value',
                  '#value' => $sms_number_value,
                );
                $form['commerce_taxamo_sms_number_info'] = array(
                  '#title' => t("Mobile phone"),
                  '#type' => 'item',
                  '#markup' => $calling_code . $sms_number_value,
                );

                $form['commerce_taxamo_sms_token'] = array(
                  '#title' => t("SMS Token"),
                  '#type' => 'textfield',
                  '#required' => TRUE,
                  '#description' => t("Please provide the SMS Token that was sent to your mobile number @mobile_number in @country. If you want us to send you a new SMS token, please go back and start the validation process again.", array('@mobile_number' => $sms_number_value, '@country' => $countries[$self_declaration_value])),
                );
              }
            }
          }
        }
        else {
          // Self Declaration is not allowed.
          drupal_set_message(t("Your country can't be validated. Please go back a correct the information."), 'error');
        }
      }
    }

  }
  else {
    drupal_set_message(t("There is an issue loading Taxamo PHP library. Please try again later."), 'error');
  }

  return $form;
}

/**
 * Callback function used by commerce pane.
 */
function commerce_taxamo_taxamo_validation_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $taxamo_data = &$order->data['taxamo'];

  if ($taxamo_data['allow_vat_calculation']) {
    return TRUE;
  }

  if (commerce_taxamo_get_dashboard_settings('allow_self_declaration') && isset($form_state['values']['taxamo_validation']['commerce_taxamo_self_declaration'])) {
    if ($form_state['values']['taxamo_validation']['commerce_taxamo_self_declaration']) {
      $taxamo_data['self_declaration_country'] = strtoupper($form_state['values']['taxamo_validation']['commerce_taxamo_self_declaration']);
      if (in_array($taxamo_data['self_declaration_country'], array($taxamo_data['billing_country'], $taxamo_data['ip_country']))) {
        $taxamo_data['force_country'] = TRUE;
        $taxamo_data['force_country_key'] = 'self_declaration_country';
        return TRUE;
      }
      else {
        if (commerce_taxamo_get_dashboard_settings('allow_sms_verification')) {
          if (!empty($form_state['values']['taxamo_validation']['commerce_taxamo_sms_token'])) {
            $token_to_validate = $form_state['values']['taxamo_validation']['commerce_taxamo_sms_token'];
            if ($taxamo = commerce_taxamo_get_taxamo_php_class('public')) {
              try {
                $verify_sms_token = $taxamo->verifySMSToken($token_to_validate);
                if (!empty($verify_sms_token) && $verify_sms_token->country_code == $form_state['values']['taxamo_validation']['commerce_taxamo_self_declaration']) {
                  $taxamo_data['force_country'] = TRUE;
                  $taxamo_data['force_country_key'] = 'sms_token_country';
                  $taxamo_data['sms_token_country'] = $verify_sms_token->country_code;
                  return TRUE;
                }
                else {
                  drupal_set_message(t("The token was not validated. Please try again."), 'error');
                }
              } catch (Exception $e) {
                watchdog('commerce_taxamo', 'Error trying to verify sms token to @sms_token: @error', array('@sms_token' => $token_to_validate, '@error' => $e->getMessage()), WATCHDOG_ERROR);
              }
            }
          }
          elseif (!empty($form_state['values']['taxamo_validation']['commerce_taxamo_sms_number'])) {
            $sms_number = $form_state['values']['taxamo_validation']['commerce_taxamo_sms_number'];
            if (!commerce_taxamo_valid_mobile_number($sms_number)) {
              drupal_set_message(t("Please add a valid number. Only numbers are allowed."), 'error');
            }
            elseif (empty($taxamo_data['token_sent'])) {
              $token_sent = FALSE;
              if ($taxamo = commerce_taxamo_get_taxamo_php_class('public')) {
                try {
                  $create_sms_token_in = new CreateSMSTokenIn();
                  $create_sms_token_in->country_code = $form_state['values']['taxamo_validation']['commerce_taxamo_self_declaration'];
                  $create_sms_token_in->recipient = $sms_number;
                  $create_sms_token = $taxamo->createSMSToken($create_sms_token_in);
                  if (!empty($create_sms_token)) {
                    $token_sent = $create_sms_token->success;
                  }
                } catch (Exception $e) {
                  watchdog('commerce_taxamo', 'Error trying to send token to @sms_number: @error', array('@sms_number' => $sms_number, '@error' => $e->getMessage()), WATCHDOG_ERROR);
                }
              }

              if ($token_sent) {
                $taxamo_data['token_sent'] = TRUE;
              }
              else {
                drupal_set_message(t("There was a problem sending the token. Please check the information provided and try again."), 'error');
              }
            }
          }
        }
        else {
          drupal_set_message(t("Your country can not be validated."), 'error');
        }
      }
    }
  }

  return FALSE;
}

/**
 * Helper function to validate a mobile number. It actually only checks if it is
 * a positive integer.
 * @param  string $number The number to be validated.
 * @return boolean        Whether the the number is valid or not.
 */
function commerce_taxamo_valid_mobile_number($number) {
  if (is_numeric($number) && (int) $number == $number && $number > 0) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Callback function used by commerce pane.
 */
function commerce_taxamo_taxamo_validation_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {

}

/**
 * Callback function used by commerce pane.
 */
function commerce_taxamo_taxamo_validation_pane_review($form, $form_state, $checkout_pane, $order) {

}
