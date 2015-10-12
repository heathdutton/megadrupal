<?php

/**
 * @file
 * Provides metadata for the entities.
 */

/**
 * Implements hook_entity_property_info().
 */
function entity_email_entity_property_info() {
  $info = array();
  // Add meta-data about the entity_email_type properties.
  $properties = &$info['entity_email_type']['properties'];
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the entity_email_entity was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'created',
  );
  $properties['updated'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the entity_email_entity was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'updated',
  );
  $properties['label'] = array(
    'label' => t('Label'),
    'description' => t('The human-readable name of the email template.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'label',
  );
  $properties['name'] = array(
    'label' => t('Name'),
    'description' => t('The unified identifier for the email template.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'name',
  );
  $properties['tags'] = array(
    'label' => t('Tags'),
    'description' => t('The list of tags string associated with the entity email type.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'tags',
  );

  // Add meta-data about the basic entity_email properties.
  $properties = &$info['entity_email']['properties'];
  $properties['eid'] = array(
    'label' => t("Unique ID"),
    'type' => 'integer',
    'description' => t("The Drupal unique ID."),
    'schema field' => 'eid',
  );
  $properties['type'] = array(
    'label' => t("Type"),
    'type' => 'token',
    'description' => t("The type of this email."),
    'schema field' => 'type',
    'options list' => 'entity_email_type_options_list',
  );
  $properties['subject'] = array(
    'label' => t("Subject"),
    'type' => 'text',
    'description' => t("The email subject."),
    'schema field' => 'subject',
  );
  $properties['timestamp'] = array(
    'label' => t("Timestamp"),
    'type' => 'date',
    'description' => t("When the message email was recorded."),
    'schema field' => 'timestamp',
  );
  $properties['cuid'] = array(
    'label' => t("Creator"),
    'type' => 'user',
    'description' => t("The creator of the email instance."),
    'getter callback' => 'entity_metadata_node_get_properties',
    'setter callback' => 'entity_metadata_node_set_properties',
    'schema field' => 'cuid',
  );
  return $info;
}
