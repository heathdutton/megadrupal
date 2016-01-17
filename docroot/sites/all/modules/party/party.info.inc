<?php

/**
 * @file
 *
 * Contains additional entity information for the party entity.
 */

/**
 * Implements hook_entity_property_info_alter().
 *
 * Register our attached entities as entity properties so that other modules
 * can build items from it, e.g. Search API. Also adds boolean properties for
 * which entities are attached.
 */
function party_entity_property_info_alter(&$property_info) {
  $properties =& $property_info['party']['properties'];

  $properties['label']['description'] = t('The label for this party.');
  $properties['label']['setter callback'] = 'entity_property_verbatim_set';
  $properties['label']['party primary field'] = TRUE;
  $properties['email']['description'] = t("The party's primary email address.");
  $properties['email']['setter callback'] = 'entity_property_verbatim_set';
  $properties['email']['party primary field'] = TRUE;

  foreach (party_get_data_set_info() as $data_set_name => $set_info) {
    if (empty($property_info[$set_info['entity type']]['properties']['party'])) {
      $property = array(
        'label' => t('Party'),
        'description' => t('Party with this entity attached.'),
        'type' => 'party',
        'bundle' => 'party',
        'getter callback' => 'entity_property_verbatim_get',
        'schema field' => 'party_attaching_party',
      );
      $property_info[$set_info['entity type']]['properties']['party'] = $property;
    }

    $properties[$data_set_name] = array(
      'label' => t('@label (Attached Entities)', array('@label' => $set_info['label'])),
      'description' => t('The attached entities that are party of the @label data set.', array('@label' => $set_info['label'])),
      'type' => $set_info['entity type'],
      'bundle' => $set_info['entity bundle'],
      'computed' => TRUE,
      'getter callback' => 'party_property_dataset_get',
    );

    $properties['has_dataset_' . $data_set_name] = array(
      'label' => t('Party has @label (Attached Entities)', array('@label' => $set_info['label'])),
      'description' => t('Boolean showing that a party has the @label data set.', array('@label' => $set_info['label'])),
      'type' => 'boolean',
      'computed' => TRUE,
      'getter callback' => 'party_property_has_dataset_get',
      'entity views field' => TRUE,
    );
  }

  // If this has been called, we will need to re-build our caches.
  PartyPrimaryFields::clearCaches();
}
