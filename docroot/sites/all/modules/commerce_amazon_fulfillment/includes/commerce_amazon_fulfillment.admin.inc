<?php


/**
 * Administrative form for commerce amazon fulfillment
 *
 * @return mixed
 */

function commerce_amazon_fulfillment_admin() {
  // Initiate form array.
  $form = array();

  // Description of form for users.
  $form['description'] = array(
    '#markup' => t('Change settings here for how your drupal commerce setup will work with amazon fulfillment.
    For more information on the significance of all of these settings, please visit !website.',
      array(
        '!website' => l(t("amazon's documentation"), "http://docs.developer.amazonservices.com/en_US/fba_outbound/FBAOutbound_CreateFulfillmentOrder.html", array(
          'attributes' => array('target' => '_blank')
          ,)
        ),
      )
    ),
  );

  // Fieldset for amazon credentials.
  $form['amazon_credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Amazon Credentials'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Conditional collapsing of amazon_credentials fieldset.
  if(variable_get('amazon_fulfillment_dai')){
    $form['amazon_credentials']['#collapsed'] = TRUE;
  }


  // Developer Account Idenitifier field.
  $form['amazon_credentials']['aws-dai'] = array(
    '#title' => t('Developer Account Identifier'),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_dai', ''),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 60,
  );

  $form['amazon_credentials']['aws-keyid'] = array(
    '#title' => t('AWS Access Key ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_keyid', ''),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 60,
  );

  $form['amazon_credentials']['aws-secretkey'] = array(
    '#title' => t('Secret Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_secretkey', ''),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 60,
  );

  $form['amazon_credentials']['aws-region'] = array(
    '#title' => t('Amazon Fulfillment Region.'),
    '#description' => t("Specifies region which determines marketplace endpoint to us."),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_MWS_endpoint', 'na-us'),
    '#type' => 'select',
    '#options' => array(
      'na-ca' => t('North America - Canada'),
      'na-us' => t('North America - United States'),
      'eu-de' => t('Europe - DE'),
      'eu-es' => t('Europe - ES'),
      'eu-fr' => t('Europe - ES'),
      'eu-in' => t('Europe - FR'),
      'eu-it' => t('Europe - IN'),
      'eu-uk' => t('Europe - UK'),
      'fe-jp' => t('FE - JP'),
      'cn-cn' => t('CN - CN'),
    ),
  );

  $form['amazon_credentials']['aws-merchantid'] = array(
    '#title' => t('Merchant/Seller ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_merchantid', ''),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 60,
  );

  $form['amazon_credentials']['aws-marketplaceid'] = array(
    '#title' => t('Marketplace ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_marketplaceid', ''),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 60,
  );


  // Fieldset for fufillment options.
  $form['fufillment_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Amazon Fulfillment Options'),
    '#weight' => 8,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Fulfillment options: Displayable order date time
  $form['fufillment_options']['FulfillmentAction'] = array(
    '#title' => t('Amazon Fulfillment Action.'),
    '#description' => t("Specifies whether the fulfillment order should ship now or have an order hold put on it."),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_FulfillmentAction', 'Ship'),
    '#type' => 'select',
    '#options' => array(
      'Ship' => t('Ship'),
      'Hold' => t('Hold'),
    ),
  );

  $form['fufillment_options']['DisplayableOrderIdPrefix'] = array(
    '#title' => t('Displayable Order ID Prefix'),
    '#description' => t("If you wish to prefix the order ID displayed on amazon shipping with something (EX: <b>Drupal Store</b> Order #123."),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_DisplayableOrderIdPrefix', ''),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 60,
  );

  $form['fufillment_options']['displayableComment'] = array(
    '#title' => t('Displayable Comment'),
    '#description' => t("Amazon MWS requires a displayable comment be attached to all orders. Please provide one below."),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_displayable_comment', ''),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 60,
  );

  $form['fufillment_options']['ShippingSpeedCategory'] = array(
    '#title' => t('Shipping Speed Category'),
    '#description' => t("The shipping method for your fulfillment order."),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_ShippingSpeedCategory', 'Standard'),
    '#type' => 'select',
    '#options' => array(
      'Standard' => t('Standard'),
      'Expedited' => t('Expedited'),
      'Priority' => t('Priority'),
      'ScheduledDelivery' => t('Scheduled Delivery - JAPAN ONLY!'),
    ),
  );

  $form['fufillment_options']['FulfillmentPolicy'] = array(
    '#title' => t('Amazon Fulfillment Policy'),
    '#description' => t("Indicates how unfulfillable items in a fulfillment order should be handled.
    <br/><br/>
    <b>FillOrKill</b> - If an item in a fulfillment order is determined to be
    unfulfillable before any shipment in the order moves to the Pending status
    (the process of picking units from inventory has begun), then the entire
    order is considered unfulfillable. However, if an item in a fulfillment order
    is determined to be unfulfillable after a shipment in the order moves to the
    Pending status, Amazon cancels as much of the fulfillment order as possible.
    See the FulfillmentShipment datatype for shipment status definitions.
    <br/><br/>
    <b>FillAll</b> - All fulfillable items in the fulfillment order are shipped.
    The fulfillment order remains in a processing state until all items are either
    shipped by Amazon or cancelled by the seller.
    <br/><br/>
    <b>FillAllAvailable</b> - All fulfillable items in the fulfillment order are
    shipped. All unfulfillable items in the order are cancelled by Amazon.
    "),
    '#required' => TRUE,
    '#default_value' => variable_get('amazon_fulfillment_FulfillmentPolicy', 'FillOrKill'),
    '#type' => 'select',
    '#options' => array(
      'FillOrKill' => t('Fill or Kill.'),
      'FillAll' => t('Fill All'),
      'FillAllAvailable' => t('Fill all available'),
    ),
  );

  $form['#validate'][] = 'commerce_amazon_fulfillment_admin_validate';
  $form['#submit'][] = 'commerce_amazon_fulfillment_admin_submit';

  return system_settings_form($form);
}

/**
 * Admin form validation function.
 *
 * Currently this is just a placeholder for later form validation that may
 * occur.
 *
 * @param array $form
 *   Form object.
 *
 * @param array $form_state
 *   Form state object
 */
function commerce_amazon_fulfillment_admin_validate($form, &$form_state) {
  // Validation can occur in here.
}

/**
 * Admin form submission handler.
 *
 * @param object $form
 *   Form object.
 *
 * @param array $form_state
 *   Form state object
 */

function commerce_amazon_fulfillment_admin_submit($form, &$form_state){
  // Set credentials.
  variable_set('amazon_fulfillment_dai', $form_state['input']['aws-dai']);
  variable_set('amazon_fulfillment_keyid', $form_state['input']['aws-keyid']);
  variable_set('amazon_fulfillment_secretkey', $form_state['input']['aws-secretkey']);
  variable_set('amazon_fulfillment_MWS_endpoint', $form_state['input']['aws-region']);
  variable_set('amazon_fulfillment_merchantid', $form_state['input']['aws-merchantid']);
  variable_set('amazon_fulfillment_marketplaceid', $form_state['input']['aws-marketplaceid']);

  // Determine AWS service URL.
  // TODO: Update everything below for correct endpoint after testing wraps up.
  switch ($form_state['input']['aws-region']) {
    // North America.
    case 'na-us': case 'na-ca':
      $endpoint_url = "https://mws.amazonservices.com/FulfillmentOutboundShipment/2010-10-01";
      $service_url = 'https://mws.amazonservices.com';
      break;

    // Europe.
    case 'eu-de': case 'eu-es': case 'eu-fr': case 'eu-in': case 'eu-it': case 'eu-uk':
      $endpoint_url = "https://mws-eu.amazonservices.com/FulfillmentOutboundShipment/2010-10-01";
      $service_url = 'https://mws-eu.amazonservices.com';
      break;

    // Japan.
    case 'fe-jp':
      $endpoint_url = "https://mws.amazonservices.jp/FulfillmentOutboundShipment/2010-10-01";
      $service_url = 'https://mws.amazonservices.jp';
      break;

    // China.
    case 'cn-cn':
      $endpoint_url = "https://mws.amazonservices.com.cn/FulfillmentOutboundShipment/2010-10-01";
      $service_url = 'https://mws.amazonservices.com.cn';
      break;
  }
  variable_set('amazon_fulfillment_marketplace_endpoint', $endpoint_url);
  variable_set('amazon_fulfillment_marketplace_url', $service_url);


  // Set order related options.
  variable_set('amazon_fulfillment_FulfillmentAction', $form_state['input']['FulfillmentAction']);
  variable_set('amazon_fulfillment_displayable_comment', $form_state['input']['displayableComment']);
  variable_set('amazon_fulfillment_DisplayableOrderIdPrefix', $form_state['input']['DisplayableOrderIdPrefix']);
  variable_set('amazon_fulfillment_ShippingSpeedCategory', $form_state['input']['ShippingSpeedCategory']);
  variable_set('amazon_fulfillment_FulfillmentPolicy', $form_state['input']['FulfillmentPolicy']);

}
