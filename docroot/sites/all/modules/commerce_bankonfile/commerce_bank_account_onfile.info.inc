<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_bank_account_onfile_entity_property_info() {
  $info = array();

  $properties = &$info['commerce_bank_account_file']['properties'];
  $properties['bank_account_id'] = array(
    'label' => t('Bank account ID'),
    'description' => t('The internal numeric ID of the bank account.'),
    'type' => 'integer',
    'schema field' => 'bank_account_id',
  );
  $properties['payment_method'] = array(
    'label' => t('Payment method'),
    'description' => t('The method_id of the payment method that stored the bank account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'payment_method',
  );
  $properties['instance_id'] = array(
    'label' => t('Instance ID'),
    'description' => t('The instance_id of the payment method that stored the bank account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'instance_id',
  );
  $properties['remote_id'] = array(
    'label' => t('Remote ID'),
    'description' => t('The id of the bank account at the payment gateway.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'remote_id',
  );
  $properties['bank_account_type'] = array(
    'label' => t('Bank account type'),
    'description' => t('The bank account type.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'commerce_bank_account_onfile_bank_account_types',
    'schema field' => 'bank_account_type',
  );
  $properties['aba_code'] = array(
    'label' => t('Routing'),
    'description' => t('The routing number (aba code).'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'aba_code',
  );
  $properties['bank_account_name'] = array(
    'label' => t('Bank account name'),
    'description' => t('The name on the bank account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'bank_account_name',
  );
  $properties['bank_account_number'] = array(
    'label' => t('Bank account number'),
    'description' => t('Truncated bank account number (last 4 digits).'),
    'type' => 'text',
    'getter callback' => 'commerce_bank_account_onfile_get_properties',
    'raw getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'bank_account_number',
  );
  $properties['bank_name'] = array(
    'label' => t('Bank name'),
    'description' => t('Name of the bank where account belong.'),
    'type' => 'text',
    'raw getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'bank_name',
  );
  // $properties['card_exp_month'] = array(
  //   'label' => t('Card expiration month'),
  //   'description' => t('Expiration month.'),
  //   'type' => 'integer',
  //   'setter callback' => 'entity_property_verbatim_set',
  //   'schema field' => 'card_exp_month',
  // );
  // $properties['card_exp_year'] = array(
  //   'label' => t('Card expiration year'),
  //   'description' => t('Expiration year.'),
  //   'type' => 'integer',
  //   'setter callback' => 'entity_property_verbatim_set',
  //   'schema field' => 'card_exp_year',
  // );
  // $properties['card_exp'] = array(
  //   'label' => t('Card expiration'),
  //   'description' => t('Expiration month and year.'),
  //   'type' => 'text',
  //   'getter callback' => 'commerce_cardonfile_get_properties',
  //   'computed' => TRUE,
  // );
  $properties['instance_default'] = array(
    'label' => t('Instance default'),
    'description' => t('Whether this is the default bank account for this payment method instance.'),
    'type' => 'boolean',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'instance_default',
  );
  $properties['status'] = array(
    'label' => t('Bank account Status'),
    'description' => t('The bank account status.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'commerce_bank_account_onfile_statuses',
    'schema field' => 'status',
  );
  $properties['created'] = array(
    'label' => t('Created'),
    'description' => t('The Unix timestamp when the bank account data was first stored.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Changed'),
    'description' => t('The Unix timestamp when the bank account data was last updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'changed',
  );
  $properties['user'] = array(
    'label' => t('User'),
    'description' => t('The bank account owner.'),
    'type' => 'user',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'uid',
  );

  return $info;
}
