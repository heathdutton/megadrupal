<?php
/**
 * @file
 * Rules integration for uc_element.
 */

/**
 * Implements hook_rules_action_info().
 */
function uc_element_rules_condition_info() {
  // Declares meta data for verifying a user's stored card.
  $conditions = array(
    'uc_element_condition_check_stored_card' => array(
      'label' => t('User has stored card'),
      'group' => t('User'),
      'base' => 'uc_element_check_stored_card_condition',
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
  );

  return $conditions;
}

/**
 * The action function for uc_element_rules_condition_info.
 */
function uc_element_check_stored_card_condition($user) {
  if (variable_get('uc_element_allow_stored_cards', FALSE) && isset($user->uid)) {
    // Gets the info about the stored card.
    $row = _uc_element_card_get($user->uid);
    // Check if the card exists.
    if (isset($row->token)) {
      // Check the expiration date.
      $expiration_date = strtotime($row->exp_year . '-' . str_pad($row->exp_month, 2, '0', STR_PAD_LEFT) . '-' . format_date(strtotime($row->exp_month . '/1/' . $row->exp_year), 'custom', 't'));
      // Can only show if the expiration date hasn't passed.
      if ($expiration_date < strtotime(format_date(time(), 'custom', 'Y-m-d'))) {
        // Connect to and delete account from Element.
        try {
          // Connect to the Element Express services gateway via SOAP.
          $soap_location = variable_get('uc_element_services_gateway_url', '');
          $connection_settings = array(
            'soap_version' => SOAP_1_1,
            'cache_wsdl' => WSDL_CACHE_NONE,
            'location' => $soap_location,
            'uri' => 'https://transaction.elementexpress.com',
            'trace' => TRUE,
          );
          $element_pass_soap_client = new SoapClient($soap_location, $connection_settings);
          // Delete old account from Element.
          $element_pass_soap_client->PaymentAccountDelete(_uc_element_delete_pass_account($row->token));
        }
        catch (Exception $e) {
          // Try the backup.
          try {
            $soap_location = variable_get('uc_element_services_gateway_url_backup', '');
            $connection_settings = array(
              'soap_version' => SOAP_1_1,
              'cache_wsdl' => WSDL_CACHE_NONE,
              'location' => $soap_location,
              'uri' => 'https://transaction.elementexpress.com',
              'trace' => TRUE,
            );
            $element_pass_soap_client = new SoapClient($soap_location, $connection_settings);
            // Delete old account from Element.
            $element_pass_soap_client->PaymentAccountDelete(_uc_element_delete_pass_account($row->token));
          }
          catch (Exception $e) {
          }
        }
        // Delete the expired card from the database.
        _uc_element_token_delete($user->uid);
      }
      // Check to make sure the account exists.
      else {
        try {
          // Connect to the Element Express services gateway via SOAP.
          $soap_location = variable_get('uc_element_services_gateway_url', '');
          $connection_settings = array(
            'soap_version' => SOAP_1_1,
            'cache_wsdl' => WSDL_CACHE_NONE,
            'location' => $soap_location,
            'uri' => 'https://transaction.elementexpress.com',
            'trace' => TRUE,
          );
          $element_pass_soap_client = new SoapClient($soap_location, $connection_settings);
          // Query account from Element.
          $results = $element_pass_soap_client->PaymentAccountQuery(_uc_element_query_pass_account($row->token));
        }
        catch (Exception $e) {
          drupal_set_message(t('We were unable to validate your stored credit card'), 'error');
        }
        if ($results->response->ExpressResponseCode == '0') {
          // This is valid, show the last 4 digits in the name.
          return TRUE;
        }
        else {
          // Delete the invalid card from the database.
          _uc_element_token_delete($user->uid);
        }
      }
    }
  }
  return FALSE;
}
