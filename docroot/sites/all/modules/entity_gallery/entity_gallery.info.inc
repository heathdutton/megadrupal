<?php

/**
 * @file
 * Provides info about the entity gallery entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function entity_gallery_entity_property_info() {
  $info = array();
  // Add meta-data about the basic entity gallery properties.
  $properties = &$info['entity_gallery']['properties'];

  $properties['egid'] = array(
    'label' => t("Entity gallery ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the entity gallery."),
    'schema field' => 'egid',
  );
  $properties['vid'] = array(
    'label' => t("Revision ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the entity gallery's revision."),
    'schema field' => 'vid',
  );
  $properties['is_new'] = array(
    'label' => t("Is new"),
    'type' => 'boolean',
    'description' => t("Whether the entity gallery is new and not saved to the database yet."),
    'getter callback' => 'entity_gallery_entity_get_properties',
  );
  $properties['type'] = array(
    'label' => t("Gallery type"),
    'type' => 'token',
    'description' => t("The type of the entity gallery."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer entity galleries',
    'options list' => 'entity_gallery_type_get_names',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['title'] = array(
    'label' => t("Title"),
    'description' => t("The title of the entity gallery."),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'title',
    'required' => TRUE,
  );
  $properties['language'] = array(
    'label' => t("Language"),
    'type' => 'token',
    'description' => t("The language the entity gallery is written in."),
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'entity_metadata_language_list',
    'schema field' => 'language',
    'setter permission' => 'administer entity galleries',
  );
  $properties['url'] = array(
    'label' => t("URL"),
    'description' => t("The URL of the entity gallery."),
    'getter callback' => 'entity_metadata_entity_get_properties',
    'type' => 'uri',
    'computed' => TRUE,
  );
  $properties['edit_url'] = array(
    'label' => t("Edit URL"),
    'description' => t("The URL of the entity gallery's edit page."),
    'getter callback' => 'entity_gallery_entity_get_properties',
    'type' => 'uri',
    'computed' => TRUE,
  );
  $properties['status'] = array(
    'label' => t("Status"),
    'description' => t("Whether the entity gallery is published or unpublished."),
    // Although the status is expected to be boolean, its schema suggests
    // it is an integer, so we follow the schema definition.
    'type' => 'integer',
    'options list' => 'entity_metadata_status_options_list',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer entity galleries',
    'schema field' => 'status',
  );
  $properties['sticky'] = array(
    'label' => t("Sticky in lists"),
    'description' => t("Whether the entity gallery is displayed at the top of lists in which it appears."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer entity galleries',
    'schema field' => 'sticky',
    'type' => 'boolean',
  );
  $properties['created'] = array(
    'label' => t("Date created"),
    'type' => 'date',
    'description' => t("The date the entity gallery was posted."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer entity galleries',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t("Date changed"),
    'type' => 'date',
    'schema field' => 'changed',
    'description' => t("The date the entity gallery was most recently updated."),
  );
  $properties['author'] = array(
    'label' => t("Author"),
    'type' => 'user',
    'description' => t("The author of the entity gallery."),
    'getter callback' => 'entity_gallery_entity_get_properties',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer entity galleries',
    'required' => TRUE,
    'schema field' => 'uid',
  );
  $properties['source'] = array(
    'label' => t("Translation source entity gallery"),
    'type' => 'entity_gallery',
    'description' => t("The original-language version of this entity gallery, if one exists."),
    'getter callback' => 'entity_gallery_entity_get_properties',
  );
  $properties['log'] = array(
    'label' => t("Revision log message"),
    'type' => 'text',
    'description' => t("In case a new revision is to be saved, the log entry explaining the changes for this version."),
    'setter callback' => 'entity_property_verbatim_set',
    'access callback' => 'entity_gallery_entity_revision_access',
  );
  $properties['revision'] = array(
    'label' => t("Creates revision"),
    'type' => 'boolean',
    'description' => t("Whether saving this entity gallery creates a new revision."),
    'setter callback' => 'entity_property_verbatim_set',
    'access callback' => 'entity_gallery_entity_revision_access',
  );
  return $info;
}

/**
 * Implements hook_entity_property_info_alter().
 */
function entity_gallery_entity_property_info_alter(&$info) {
  // Move the description property to the entity gallery by default, as its
  // usually there this makes dealing with it more convenient.
  $info['entity_gallery']['properties']['entity_gallery_description'] = array(
    'type' => 'text_formatted',
    'label' => t('The main description text'),
    'getter callback' => 'entity_metadata_field_verbatim_get',
    'setter callback' => 'entity_metadata_field_verbatim_set',
    'property info' => entity_property_text_formatted_info(),
    'auto creation' => 'entity_property_create_array',
    'field' => TRUE,
  );
}
