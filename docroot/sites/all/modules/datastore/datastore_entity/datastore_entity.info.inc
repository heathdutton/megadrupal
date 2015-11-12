<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class DatastoreEntityMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['did'] += array(
      'validation callback' => 'entity_metadata_validate_integer_positive',
    );

    $properties['domain'] = array(
      'setter callback' => 'entity_metadata_verbatim_set',
      'setter permission' => 'administer datastores',
      'required' => TRUE,
      'description' => t('The dataset domain.'),
    ) + $properties['domain'];

    $properties['category'] = array(
      'label' => t("Category"),
      'description' => t("..."),
      'setter callback' => 'entity_metadata_verbatim_set',
      'setter permission' => 'administer datastores',
    ) + $properties['category'];

    $properties['reference'] = array(
      'label' => t("Reference"),
      'description' => t("..."),
      'setter callback' => 'entity_metadata_verbatim_set',
      'setter permission' => 'administer datastores',
    ) + $properties['reference'];

    return $info;
  }
}