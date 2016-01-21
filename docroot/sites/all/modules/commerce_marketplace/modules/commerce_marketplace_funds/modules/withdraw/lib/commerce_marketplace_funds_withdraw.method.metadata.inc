<?php

class CommerceMarketplaceFundsWithdrawMethodMetadataController extends EntityDefaultMetadataController {
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    $properties['id'] = array(
      'label' => t('Withdraw Method ID'),
      'type' => 'integer',
      'description' => t("The unique ID of the Withdraw method."),
      'schema field' => 'id',
    );
    $properties['account'] = array(
      'label' => t("Associated funds account"),
      'type' => 'integer',
      'description' => t("The funds account associated with this withdraw method."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_account entities',
      'required' => TRUE,
      'schema field' => 'account_id',
    );
    $properties['type'] = array(
      'label' => t("Withdraw method type"),
      'type' => 'token',
      'description' => t("The type of the withdraw method."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cmp_funds_account entities',
      'required' => TRUE,
      'schema field' => 'type',
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

