<?php

class CommerceMarketplaceFundsAccountMetadataController extends EntityDefaultMetadataController {
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    $properties['account_id'] = array(
      'label' => t('Funds account ID'),
      'type' => 'integer',
      'description' => t("The unique ID of the funds account."),
      'schema field' => 'account_id',
    );
    $properties['owner_type'] = array(
      'label' => t("Owner type"),
      'type' => 'token',
      'description' => t("Entity type of the funds account owner."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_account entities',
      'options list' => 'commerce_marketplace_funds_account_types',
      'required' => TRUE,
      'schema field' => 'owner_type',
    );
    $properties['owner'] = array(
      'label' => t("Funds account owner"),
      'type' => 'entity',
      'description' => t("The owner of the funds account."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_account entities',
      'required' => TRUE,
      'schema field' => 'owner_id',
    );
    $properties['balance'] = array(
      'type' => 'commerce_price',
      'label' => t('Field: The funds account balance'),
      'getter callback' => 'entity_metadata_field_verbatim_get',
      'setter callback' => 'entity_metadata_field_verbatim_set',
      'property info' => commerce_price_field_data_property_info('balance'),
      'auto creation' => 'commerce_price_field_data_auto_creation',
      'field' => TRUE,
      'required' => TRUE,
    );
    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the funds account was created."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_account entities',
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the funds account was most recently updated."),
    );
    return $info;
  }
}