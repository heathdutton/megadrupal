<?php

/**
 * @file
 * Provides metadata for the clickbank transaction entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function clickbank_ipn_entity_property_info() {
  $info = array();

  // Add meta-data about the basic clickbank_transaction properties.
  $properties = &$info['clickbank_transaction']['properties'];

  $properties['receipt'] = array(
    'type' => 'text',
    'label' => t('Receipt ID', array(), array('context' => 'a clickbank transaction')),
    'description' => t('The internal numeric ID of the transaction.'),
    'schema field' => 'receipt',
	'required' => TRUE,
  );
  $properties['version'] = array(
    'type' => 'text',
    'label' => t('Version', array(), array('context' => 'version of instant notification')),
    'description' => t('The version of the clickbank instant notification. See https://support.clickbank.com/entries/22803622-instant-notification-service'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'version',
  );
  $properties['created'] = array(
    'type' => 'date',
    'label' => t('Date created'),
    'description' => t('The date the clickbank transaction was created.'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'type' => 'date',
    'label' => t('Date changed'),
    'description' => t('The date the transaction was most recently updated.'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'changed',
  );
  $properties['type'] = array(
    'type' => 'text',
    'label' => t('Type'),
    'description' => t('The human readable name of the clickbank transaction type.'),
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'clickbank_transaction_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['uid'] = array(
    'type' => 'integer',
    'label' => t("Owner ID"),
    'description' => t("The unique ID of the transaction owner."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer clickbank_transaction entities',
    'clear' => array('owner'),
    'schema field' => 'uid',
  );
  $properties['role'] = array(
    'type' => 'text',
    'label' => t("Role"),
    'description' => t("Vendor or Affiliate (e.g., your role on the transaction event)"),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'role',
  );
  $properties['site'] = array(
    'type' => 'text',
    'label' => t("Site"),
    'description' => t("Vendor Nickname"),
    'setter callback' => 'entity_property_verbatim_set',
	'schema field' => 'site',
  );
  $properties['currency'] = array(
    'type' => 'text',
    'label' => t("Currency"),
    'description' => t('Currency of the Purchase [only populated for vendor]'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'currency',
  );
  $properties['affiliate'] = array(
    'type' => 'text',
    'label' => t("Affiliate"),
    'description' => t('Affiliate nickname'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'affiliate',
  );
  $properties['transactionType'] = array(
    'type' => 'text',
    'label' => t("transactionType"),
    'description' => t('Type of Transaction (SALE, JV_SALE, INSF, RFND, CGBK, BILL, JV_BILL, TEST_SALE, TEST_JV_SALE, TEST_BILL, TEST_JV_BILL, TEST_RFND)'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'transactionType',
  );
  $properties['transactionTime'] = array(
    'type' => 'text',
    'label' => t("transactionTime"),
    'description' => t('The Epoch time the transaction occurred'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'transactionTime',
  );
  $properties['transactionLanguage'] = array(
    'type' => 'text',
    'label' => t("transactionLanguage"),
    'description' => t('Language of the transaction'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'transactionlanguage',
  );
  $properties['totalProductAmount'] = array(
    'type' => 'text',
    'label' => t("totalProductAmount"),
    'description' => t('Total Amount of the Sale'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'totalProductAmount',
  );
  $properties['totalTaxAmount'] = array(
    'type' => 'text',
    'label' => t("totalTaxAmount"),
    'description' => t('Tax Applied to Sale'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'totalTaxAmount',
  );
  
  // Shipping Info
  
  $properties['totalShippingAmount'] = array(
    'type' => 'text',
    'label' => t("totalShippingAmount"),
    'description' => t('Shipping Amount Collected'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'totalShippingAmount',
  );
  $properties['shippingfirstName'] = array(
    'type' => 'text',
    'label' => t("shippingfirstName"),
    'description' => t('First Name - Shipping Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingfirstName',
  );
  $properties['shippinglastName'] = array(
    'type' => 'text',
    'label' => t("shippinglastName"),
    'description' => t('Last Name - Shipping Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippinglastName',
  );
  $properties['shippingfullName'] = array(
    'type' => 'text',
    'label' => t("shippingfullName"),
    'description' => t('Full Name - Shipping Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingfullName',
  );
  $properties['shippingphoneNumber'] = array(
    'type' => 'text',
    'label' => t("shippingphoneNumber"),
    'description' => t('Shipping Phone Number'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingphoneNumber',
  );
  $properties['shippingemail'] = array(
    'label' => t('Clickbank Shipping e-mail'),
    'description' => t('Email Address - Shipping Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'validation callback' => 'valid_email_address',
    'schema field' => 'shippingemail',
  );
  $properties['shippingaddress1'] = array(
    'type' => 'text',
    'label' => t("shippingaddress1"),
    'description' => t('Shipping Address - First Line'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingaddress1',
  );
  $properties['shippingaddress2'] = array(
    'type' => 'text',
    'label' => t("shippingaddress2"),
    'description' => t('Shipping Address - Second Line'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingaddress2',
  );
  $properties['shippingcity'] = array(
    'type' => 'text',
    'label' => t("shippingcity"),
    'description' => t('Shipping City'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingcity',
  );
  $properties['shippingcounty'] = array(
    'type' => 'text',
    'label' => t("shippingcounty"),
    'description' => t('Shipping County'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingcounty',
  );
  $properties['shippingstate'] = array(
    'type' => 'text',
    'label' => t("shippingstate"),
    'description' => t('Shipping State'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingstate',
  );
  $properties['shippingpostalCode'] = array(
    'type' => 'text',
    'label' => t("shippingpostalCode"),
    'description' => t('Shipping Postal Code'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingpostalCode',
  );
  $properties['shippingcountry'] = array(
    'type' => 'text',
    'label' => t("shippingcountry"),
    'description' => t('Shipping Country'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'shippingcountry',
  );
  
  // Billing Info
  
  $properties['billingfirstName'] = array(
    'type' => 'text',
    'label' => t("billingfirstName"),
    'description' => t('First Name - Billing Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingfirstName',
  );
  $properties['billinglastName'] = array(
    'type' => 'text',
    'label' => t("billinglastName"),
    'description' => t('Last Name - Billing Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billinglastName',
  );
  $properties['billingfullName'] = array(
    'type' => 'text',
    'label' => t("billingfullName"),
    'description' => t('Full Name - Billing Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingfullName',
  );
  $properties['billingphoneNumber'] = array(
    'type' => 'text',
    'label' => t("billingphoneNumber"),
    'description' => t('Shipping Phone Number'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingphoneNumber',
  );
  $properties['billingemail'] = array(
    'label' => t('Clickbank Billing e-mail'),
    'description' => t('Email Address - Billing Recipient'),
    'setter callback' => 'entity_property_verbatim_set',
    'validation callback' => 'valid_email_address',
    'schema field' => 'billingemail',
  );
  $properties['billingaddress1'] = array(
    'type' => 'text',
    'label' => t("billingaddress1"),
    'description' => t('Billing Address - First Line'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingaddress1',
  );
  $properties['billingaddress2'] = array(
    'type' => 'text',
    'label' => t("billingaddress2"),
    'description' => t('Billing Address - Second Line'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingaddress2',
  );
  $properties['billingcity'] = array(
    'type' => 'text',
    'label' => t("billingcity"),
    'description' => t('Billing City'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingcity',
  );
  $properties['billingcounty'] = array(
    'type' => 'text',
    'label' => t("billingcounty"),
    'description' => t('Billing County'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingcounty',
  );
  $properties['billingstate'] = array(
    'type' => 'text',
    'label' => t("billingstate"),
    'description' => t('Billing State'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingstate',
  );
  $properties['billingpostalCode'] = array(
    'type' => 'text',
    'label' => t("billingpostalCode"),
    'description' => t('Billing Postal Code'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingpostalCode',
  );
  $properties['billingcountry'] = array(
    'type' => 'text',
    'label' => t("billingcountry"),
    'description' => t('Billing Country'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'billingcountry',
  );
  
  // Line Items
  
  $properties['lineItemsitemNo'] = array(
    'type' => 'text',
    'label' => t("lineItemsitemNo"),
    'description' => t('Item Number / SKU code'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'lineItemsitemNo',
  );
  $properties['lineItemsproductTitle'] = array(
    'type' => 'text',
    'label' => t("lineItemsproductTitle"),
    'description' => t('Product Title'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'lineItemsproductTitle',
  );
  $properties['lineItemsdownloadUrl'] = array(
    'type' => 'text',
    'label' => t("lineItemsdownloadUrl"),
    'description' => t('Download URL [only populated for vendor]'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'lineItemsdownloadUrl',
  );
  $properties['lineItemsshippable'] = array(
    'type' => 'text',
    'label' => t("lineItemsshippable"),
    'description' => t('Shippable Product'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'lineItemsshippable',
  );
  $properties['lineItemsrecurring'] = array(
    'type' => 'text',
    'label' => t("lineItemsrecurring"),
    'description' => t('Recurring Product'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'lineItemsrecurring',
  );
  $properties['lineItemscustomerProductAmount'] = array(
    'type' => 'text',
    'label' => t("lineItemscustomerProductAmount"),
    'description' => t('Customer Amount Paid (USD)'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'lineItemscustomerProductAmount',
  );
  $properties['lineItemscustomerTaxAmount'] = array(
    'type' => 'text',
    'label' => t("lineItemscustomerTaxAmount"),
    'description' => t('Customer Tax Amount (USD)'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'lineItemscustomerTaxAmount',
  );

  return $info;
}
