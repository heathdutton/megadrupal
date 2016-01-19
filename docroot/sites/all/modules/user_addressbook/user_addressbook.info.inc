<?php

/**
 * @file
 * Provides metadata for the user address entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function user_addressbook_entity_property_info() {
  $info = array();

  // Add meta-data about the basic user_address properties.
  $properties = &$info['user_address']['properties'];

  $properties['address_id'] = array(
    'label' => t('User address ID'),
    'description' => t('The internal numeric ID of the user address.'),
    'type' => 'integer',
    'schema field' => 'address_id',
  );
  $properties['name'] = array(
    'label' => t("Name"),
    'description' => t("The name of the user address."),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'name',
    'required' => TRUE,
  );
  $properties['uid'] = array(
    'label' => t('User ID'),
    'type' => 'integer',
    'description' => t("The unique ID of the user the user_address belongs to."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer user_address entities',
    'clear' => array('user'),
    'schema field' => 'uid',
  );
  $properties['user'] = array(
    'label' => t('User'),
    'type' => 'user',
    'description' => t("The user the user address belongs to."),
    'getter callback' => 'user_address_get_properties',
    'setter callback' => 'user_address_set_properties',
    'setter permission' => 'administer user_address entities',
    'required' => TRUE,
    'computed' => TRUE,
    'clear' => array('uid'),
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the user address was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer user_address entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the user address was last updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer user_address entities',
    'schema field' => 'changed',
  );

  return $info;
}
