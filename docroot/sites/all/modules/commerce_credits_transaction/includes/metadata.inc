<?php
/**
 * @file
 * Commerce Credits Transaction Metadata Class
 */


/**
 * Entity Property Metadata class for Commerce Credits Transactions.
 */
class CommerceCreditsTransactionMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $properties = parent::entityPropertyInfo();

    $info = &$properties['commerce_credits_transaction']['properties'];

    $info['from_cid'] = array(
      'type' => 'commerce_credits',
      'label' => t('From Credit Entity'),
      'description' => t('The commerce credits entity the transaction originated from, if applicable.'),
      'schema field' => 'from_cid',
    );

    $info['from'] = array(
      'type' => 'entity',
      'label' => t('From Entity'),
      'description' => t('The entity referenced from the Commerce Credits from entity.'),
      'getter callback' => 'commmerce_credits_transaction_get_entity',
    );

    $info['from_entity_id']['label'] = t('From Entity Id');
    $info['from_entity_type']['label'] = t('From Entity Type');
    $info['from_entity_type']['type'] = 'token';
    $info['from_entity_type']['options list'] = 'commerce_credits_transaction_get_entity_types';
    $info['from_entity_type']['schema field'] = 'from_entity_type';

    $info['to_cid'] = array(
      'type' => 'commerce_credits',
      'label' => t('To Credit Entity'),
      'description' => t('The commerce credits entity the transaction was sent to, if applicable.'),
      'schema field' => 'to_cid',
    );

    $info['to'] = array(
      'type' => 'entity',
      'label' => t('To Entity'),
      'description' => t('The entity referenced from the Commerce Credits to entity.'),
      'getter callback' => 'commmerce_credits_transaction_get_entity',
    );

    $info['to_entity_id']['label'] = t('To Entity Id');

    $info['to_entity_type']['label'] = t('To Entity Type');
    $info['to_entity_type']['type'] = 'token';
    $info['to_entity_type']['options list'] = 'commerce_credits_transaction_get_entity_types';
    $info['to_entity_type']['schema field'] = 'to_entity_type';

    $info['credit_type'] = array(
      'type' => 'commerce_credits_group',
      'label' => t('Commerce Credits Group'),
      'description' => t('The Commerce Credits Group type.'),
      'options list' => 'commerce_credits_group_get_name',
      'required' => TRUE,
      'schema field' => 'credit_type',
    );
    
    $info['status'] = array(
      'type' => 'token',
      'label' => t('Status'),
      'description' => t('The status of the transaction. Completed transactions are locked.'),
      'options list' => 'commerce_credits_transaction_get_status_options',
      'required' => TRUE,
      'schema field' => 'status',
    );

    $info['transaction'] = array(
      'type' => 'token',
      'label' => t('Transaction'),
      'description' => t('The transaction direction such as in, out, or transfer.'),
      'options list' => 'commerce_credits_transaction_get_transaction_types',
      'required' => TRUE,
      'schema field' => 'transaction',
    );

    $info['transaction_date'] = array(
      'type' => 'date',
      'label' => t('Transaction Date'),
      'description' => t('The date the commerce credits transaction took place.'),
      'required' => TRUE,
      'schema field' => 'transaction_date',
    );

    return $properties;
  }
  
}
