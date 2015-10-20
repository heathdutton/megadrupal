<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class CommerceReturnMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['return_id'] = array(
      'type' => 'integer',
      'label' => t('Return ID', array(), array('context' => 'a drupal commerce return')),
      'description' => t('The internal numeric ID of the return.'),
      'schema field' => 'return_id',
    );
    $properties['return_number'] = array(
      'type' => 'text',
      'label' => t('Return number', array(), array('context' => 'a drupal commerce return')),
      'description' => t('The return number displayed to the customer.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'return_number',
    );
    $properties['status'] = array(
      'type' => 'text',
      'label' => t('Status'),
      'description' => t('The current status of the return.'),
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => 'commerce_return_status_options_list',
      'required' => TRUE,
      'schema field' => 'status',
    );
    $properties['state'] = array(
      'type' => 'token',
      'label' => t('State'),
      'description' => t('The state of the return derived from its status.'),
      'getter callback' => 'commerce_return_get_properties',
      'options list' => 'commerce_return_state_options_list',
      'computed' => TRUE,
    );
    $properties['created'] = array(
      'type' => 'date',
      'label' => t('Date created'),
      'description' => t('The date the return was created.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer commerce_return entities',
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'type' => 'date',
      'label' => t('Date changed'),
      'description' => t('The date the return was most recently updated.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer commerce_return entities',
      'schema field' => 'changed',
    );
    $properties['hostname'] = array(
      'type' => 'text',
      'label' => t('Host name'),
      'description' => t('The IP address that created this return.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer commerce_return entities',
      'schema field' => 'hostname',
    );
    $properties['type'] = array(
      'type' => 'text',
      'label' => t('Type'),
      'description' => t('The human readable name of the return type.'),
      'setter callback' => 'entity_property_verbatim_set',
      'options list' => 'commerce_return_type_options_list',
      'required' => TRUE,
      'schema field' => 'type',
    );
    $properties['uid'] = array(
      'type' => 'integer',
      'label' => t("Owner ID"),
      'description' => t("The unique ID of the return owner."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer commerce_return entities',
      'clear' => array('owner'),
      'schema field' => 'uid',
    );
    $properties['owner'] = array(
      'type' => 'user',
      'label' => t("Owner"),
      'description' => t("The owner of the return."),
      'getter callback' => 'commerce_return_get_properties',
      'setter callback' => 'commerce_return_set_properties',
      'setter permission' => 'administer commerce_return entities',
      'required' => TRUE,
      'computed' => TRUE,
      'clear' => array('uid'),
    );
    $properties['mail'] = array(
      'label' => t('Return e-mail'),
      'description' => t('The e-mail address associated with this return.'),
      'setter callback' => 'entity_property_verbatim_set',
      'validation callback' => 'valid_email_address',
      'schema field' => 'mail',
    );
    $properties['mail_username'] = array(
      'type' => 'text',
      'label' => t('Return e-mail prepared for username usage'),
      'description' => t('The e-mail address associated with this return with illegal characters for usernames replaced.'),
      'getter callback' => 'commerce_return_get_properties',
      'computed' => TRUE,
      'clear' => array('mail'),
    );

    return $info;
  }
}
