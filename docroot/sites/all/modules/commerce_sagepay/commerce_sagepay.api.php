<php

/**
* @file
* This file contains no working PHP code; it exists to provide additional
* documentation for doxygen as well as to document hooks in the standard
* Drupal manner.
*/

/**
* Allows modules to alter the transaction data before it is encrypted and sent.
*
* Before the payment gateway redirection, all of the transaction data
* required by the SagePay V3.0 protocol is collection into an array.
* The array is then encrypted before being send to the gateway.
* This hook allows you to modify the query before it goes to SagePay.
*
* An example use of this is to popupate the telephone numbers for the customer
* profiles. As these are not standard fields, we don't know what they have
* been called in any particular Commerce implementation. Therefore the hook
* allows you to create a custom module to populate these additional values based
* on your site configuration.
*
* @param $query
*   An array of transaction parameters as defined in the SagePay V3.0 protocol.
* @param $order
*   The order from which the transaction was generated.
*/
function hook_sagepay_order_data_alter(&$query, $order) {
// No example.
}

/**
* Allows the server url the module will attempt to connect to for transaction
* processing to be altered.
* An example of this would be when trying to use a token rather than direct payment.
**/
function hook_sagepay_url_alter(&$url, $pane_values){
// No example
}

/**
* Allows modules to alter the Basket XML before it is added to the SagePay
* transaction.
*
* Some data in the basket XML definition is not standard in Drupal Commerce,
* so if you want to use these parameters you will need to define custom fields.
* Since we don't know what you might call these, this hook allows you to map
* your custom fields into the basket XML.
*
* Examples of parameters you may want to add this way:
* AgentId
* recipientPhone
*/
function hook_commerce_sagepay_basket_xml($xmlDoc) {
// No example.
}
