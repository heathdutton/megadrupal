<?php

/**
 * @file
 *   Contains Entity API hooks.
 */

/**
 * Implements hook_entity_info().
 *
 * Defines the tables of this module's data model to Drupal core and Entity API
 * (from contrib):
 * - instagram_social_feeds is defined as entity.
 * - instagram_social_feed_photos is defined as an entity.
 *
 * Both Drupal core and Entity API keys and values are defined here.
 */
function instagram_social_feed_entity_info() {
  $return = array(
    'instagram_social_feeds' => array(
      'module' => 'instagram_social_feed',
      'label' => t('Instagram feed'),
      'plural label' => t('Instagram feeds'),
      'description' => t('An entity type used to store the definitions of an Instagram feed.'),
      'base table' => 'instagram_social_feeds',
      'revision table' => null,
      'entity keys' => array(
        'id' => 'id',
      ),
      'fieldable' => FALSE,
      'view modes' => array(
        'full' => array(
          'label' => t('Full content'),
          'custom settings' => FALSE,
        ),
      ),
      'controller class' => 'EntityAPIController',
      'entity class' => 'Entity',
      'label callback' => 'entity_class_label',
      'uri callback' => 'entity_class_uri',
      //'views controller class' => '',
      //'rules controller class' => '',
    ),
    'instagram_social_feed_photos' => array(
      'module' => 'instagram_social_feed',
      'label' => t('Instagram photo'),
      'plural label' => t('Instagram photos'),
      'description' => t('An entity type used to store Instagram photos.'),
      'base table' => 'instagram_social_feed_photos',
      'revision table' => null,
      'entity keys' => array(
        'id' => 'id',
      ),
      'fieldable' => FALSE,
      'view modes' => array(
        'full' => array(
          'label' => t('Full content'),
          'custom settings' => FALSE,
        ),
      ),
      'controller class' => 'EntityAPIController',
      'entity class' => 'Entity',
      'label callback' => 'entity_class_label',
      'uri callback' => 'entity_class_uri',
      //'views controller class' => '',
      //'rules controller class' => '',
    ),
  );
  return $return;
}

/**
 * Implements hook_entity_property_info_alter().
 *
 * Corrects the property definitions where necessary. Entity API has created a
 * set of properties based on the schema information. Here, we add more detailed
 * info where needed.
 *
 * @param array $info
 */
function instagram_social_feed_entity_property_info_alter(array &$info) {
  $properties = &$info['instagram_social_feeds']['properties'];
  // Define options list for feed_type.
  $properties['feed_type']['options_list'] = 'instagram_social_feed_get_feed_type_options';
  // We don't define a Drupal path to view this entity.
  unset($properties['url']);

  $properties = &$info['instagram_social_feed_photos']['properties'];
  // Feed_id is a reference to a instagram_social_feeds entity/record.
  $properties['feed_id']['type'] = 'instagram_social_feeds';
  // The integer created_time is a timestamp.
  $properties['created_time']['type'] = 'date';
  // The resolution and link properties are URIs:
  $properties['low_resolution']['type'] = 'uri';
  $properties['thumbnail']['type'] = 'uri';
  $properties['standard_resolution']['type'] = 'uri';
  $properties['caption'] = array(
    'type' => 'text',
    'label' => t('Caption'),
    'schema field' => 'caption',
    // Copied from EntityDefaultMetadataController::entityPropertyInfo().
    'description' =>  t('@entity "@property" property.', array('@entity' => t('Instagram photo'), '@property' => t('Caption'))),
  );
  $properties['instagram_link']['type'] = 'uri';
  // We don't define a Drupal path to view this photo.
  unset($properties['url']);
}
