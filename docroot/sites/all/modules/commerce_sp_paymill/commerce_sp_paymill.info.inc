<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class CommerceSPPaymillSubMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    unset($properties['uid']);
    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the subscription."),
      'getter callback' => 'entity_property_getter_method',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    unset($properties['order_id']);
    $properties['order'] = array(
      'label' => t("Commerce order"),
      'type' => 'commerce_order',
      'description' => t("The commmerce order associated to this subscription."),
      'getter callback' => 'entity_property_getter_method',
      'required' => TRUE,
      'schema field' => 'order_id',
    );

    $properties['subscription_id'] = array(
      'label' => t("Paymill subscription id"),
      'description' => t("The Paymill subscription id."),
      'schema field' => 'subscription_id',
    );
    $properties['status'] = array(
      'label' => t("Status"),
      'type' => 'boolean',
      'description' => t("The status of the subscription."),
      'schema field' => 'status',
    );
    $properties['payment_count'] = array(
      'label' => t("Payment count"),
      'type' => 'integer',
      'description' => t("The number of payment that have been made for this subscriptions."),
      'schema field' => 'payment_count',
    );
    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the subscription was created."),
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the subscription was most recently updated."),
    );

    return $info;
  }
}
