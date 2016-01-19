<?php

class CommerceMarketplaceFundsTransactionMetadataController extends EntityDefaultMetadataController {
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['transaction_id'] = array(
      'label' => t('Funds transaction ID'),
      'type' => 'integer',
      'description' => t("The unique ID of the funds transaction."),
      'schema field' => 'transaction_id',
    );
    $properties['type'] = array(
      'label' => t("Transaction type"),
      'type' => 'token',
      'description' => t("The type of the funds transaction."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_transaction entities',
      'options list' => 'commerce_marketplace_funds_transaction_types',
      'required' => TRUE,
      'schema field' => 'type',
    );
    $properties['originator_account'] = array(
      'label' => t("Originator account"),
      'type' => 'cmp_funds_account',
      'description' => t("The transaction originator account."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_transaction entities',
      'schema field' => 'from_account',
    );
    $properties['destination_account'] = array(
      'label' => t("Destination account"),
      'type' => 'cmp_funds_account',
      'description' => t("The transaction destination account."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_transaction entities',
      'required' => TRUE,
      'schema field' => 'to_account',
    );
    $properties['order'] = array(
      'label' => t("Order ID"),
      'type' => 'commerce_order',
      'description' => t("The Commerce Order of the transaction."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_transaction entities',
      'required' => TRUE,
      'schema field' => 'order_id',
    );
    $properties['status'] = array(
      'label' => t("Status"),
      'description' => t("The transaction status."),
      'options list' => 'commerce_marketplace_funds_transaction_statuses',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_transaction entities',
      'schema field' => 'status',
    );
    $properties['amount'] = array(
      'type' => 'commerce_price',
      'label' => t('Field: The transaction amount'),
      'getter callback' => 'entity_metadata_field_verbatim_get',
      'setter callback' => 'entity_metadata_field_verbatim_set',
      'property info' => commerce_price_field_data_property_info('amount'),
      'auto creation' => 'commerce_price_field_data_auto_creation',
      'field' => TRUE,
      'required' => TRUE,
    );
    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the funds transaction was created."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer funds_account entities',
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the funds transaction was most recently updated."),
    );

    return $info;
  }
}
