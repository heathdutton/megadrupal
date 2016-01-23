<?php

/**
 * @file
 * Contains the metadata controller classes for the Translation Management Tool
 * entities.
 */


/**
 * Metadata controller for the remote source entity.
 */
class TMGMTRemoteSourceMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    // Make the created and changed property appear as date.
    $properties['changed']['type'] = $properties['created']['type'] = 'date';

    // Use the defined entity label callback instead of the custom label directly.
    $properties['label']['getter callback'] = 'entity_class_label';

    // Add the options list for the available languages.
    $properties['target_language']['options list'] = $properties['source_language']['options list'] = 'entity_metadata_language_list';

    // Add the options list for the defined state constants.
    $properties['state']['options list'] = 'tmgmt_server_remote_source_states';

    // Link the author property to the corresponding user entity.
    $properties['owner'] = array(
      'label' => t('Owner'),
      'type' => 'user',
      'description' => t('The client that the remote source belongs to.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer tmgmt',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    return $info;
  }

}

/**
 * Metadata controller for the client entity.
 */
class TMGMTClientMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();

    $properties = &$info[$this->type]['properties'];

    // Make the created and changed property appear as date.
    $properties['changed']['type'] = $properties['created']['type'] = 'date';

    // Use the defined entity label callback instead of the custom label directly.
    $properties['label']['getter callback'] = 'entity_class_label';

    // Link the author property to the corresponding user entity.
    $properties['owner'] = array(
      'label' => t('Owner'),
      'type' => 'user',
      'description' => t('The owner of the client entity.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer remote clients',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    return $info;
  }

}
