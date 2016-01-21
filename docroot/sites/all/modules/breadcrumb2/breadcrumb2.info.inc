<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class Breadcrumb2MetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['path'] = array(
      'type' => 'text',
      'label' => t('path'),
      'description' => t('The internal path where breadcrumb should be shown.'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer breadcrumbs',
      'schema field' => 'path',
	  'required' => TRUE,
    );

    return $info;
  }
}
