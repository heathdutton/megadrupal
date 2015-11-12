<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_cardonfile_entity_property_info() {
  $info = array();

  $properties = &$info['commerce_cardonfile']['properties'];
  $properties['card_id'] = array(
    'label' => t('Card ID'),
    'description' => t('The internal numeric ID of the card.'),
    'type' => 'integer',
    'schema field' => 'card_id',
  );
  $properties['payment_method'] = array(
    'label' => t('Payment method'),
    'description' => t('The method_id of the payment method that stored the card.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'payment_method',
  );
  $properties['instance_id'] = array(
    'label' => t('Instance ID'),
    'description' => t('The instance_id of the payment method that stored the card.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'instance_id',
  );
  $properties['remote_id'] = array(
    'label' => t('Remote ID'),
    'description' => t('The id of the card at the payment gateway.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'remote_id',
  );
  $properties['card_type'] = array(
    'label' => t('Card type'),
    'description' => t('The card type.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'commerce_cardonfile_credit_card_types',
    'schema field' => 'card_type',
  );
  $properties['card_name'] = array(
    'label' => t('Card name'),
    'description' => t('The name on the card.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'card_name',
  );
  $properties['card_number'] = array(
    'label' => t('Card number'),
    'description' => t('Truncated card number (last 4 digits).'),
    'type' => 'text',
    'getter callback' => 'commerce_cardonfile_get_properties',
    'raw getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'card_number',
  );
  $properties['card_exp_month'] = array(
    'label' => t('Card expiration month'),
    'description' => t('Expiration month.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'card_exp_month',
  );
  $properties['card_exp_year'] = array(
    'label' => t('Card expiration year'),
    'description' => t('Expiration year.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'card_exp_year',
  );
  $properties['card_exp'] = array(
    'label' => t('Card expiration'),
    'description' => t('Expiration month and year.'),
    'type' => 'text',
    'getter callback' => 'commerce_cardonfile_get_properties',
    'computed' => TRUE,
  );
  $properties['instance_default'] = array(
    'label' => t('Instance default'),
    'description' => t('Whether this is the default card for this payment method instance.'),
    'type' => 'boolean',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'instance_default',
  );
  $properties['status'] = array(
    'label' => t('Card Status'),
    'description' => t('The card status.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'commerce_cardonfile_statuses',
    'schema field' => 'status',
  );
  $properties['created'] = array(
    'label' => t('Created'),
    'description' => t('The Unix timestamp when the card data was first stored.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Changed'),
    'description' => t('The Unix timestamp when the card data was last updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'changed',
  );
  $properties['user'] = array(
    'label' => t('User'),
    'description' => t('The card owner.'),
    'type' => 'user',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'uid',
  );

  return $info;
}
