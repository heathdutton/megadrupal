<?php

/**
 * @file
 * Contains the metadata controller classes for the Shared Content entities.
 */

/**
 * Metadata controller for the sharedcontent index
 */
class SharedContentIndexMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = & $info[$this->type]['properties'];

    // Uuid
    $properties['uuid']['setter callback'] = 'entity_property_verbatim_set';

    // Parent uuid
    $properties['parent_uuid']['setter callback'] = 'entity_property_verbatim_set';

    // Origin - the bundle
    $properties['origin']['setter callback'] = 'entity_property_verbatim_set';
    $properties['origin']['label'] = t('Bundle');

    // Connection name
    $properties['connection_name']['setter callback'] = 'entity_property_verbatim_set';
    $properties['connection_name']['label'] = t('Site');
    $properties['connection_name']['options list'] = 'sharedcontent_get_connection_labels';

    // Entity ID
    $properties['entity_id']['setter callback'] = 'entity_property_verbatim_set';

    // Entity type
    $properties['entity_type']['setter callback'] = 'entity_property_verbatim_set';

    // Entity bundle
    $properties['entity_bundle']['setter callback'] = 'entity_property_verbatim_set';
    $properties['entity_bundle']['options list'] = 'sharedcontent_get_all_entity_bundle_labels';

    // Title
    $properties['title']['setter callback'] = 'entity_property_verbatim_set';

    // Language
    $properties['language']['setter callback'] = 'entity_property_verbatim_set';
    $properties['language']['options list'] = 'entity_metadata_language_list';

    // Translationset id
    $properties['translationset_id']['setter callback'] = 'entity_property_verbatim_set';
    $properties['translationset_id']['label'] = t('Translation set');

    // Keywords
    $properties['keywords']['setter callback'] = 'entity_property_verbatim_set';

    // Tags
    $properties['tags']['setter callback'] = 'entity_property_verbatim_set';

    // url
    $properties['url']['setter callback'] = 'entity_property_verbatim_set';

    // Status
    $properties['status']['setter callback'] = 'entity_property_verbatim_set';
    $properties['status']['options list'] = 'sharedcontent_get_index_status_labels';

    // Accessibility.
    $properties['accessibility']['setter callback'] = 'entity_property_verbatim_set';

    // Entity created
    $properties['entity_created']['setter callback'] = 'entity_property_verbatim_set';
    $properties['entity_created']['type'] = 'date';

    // Entity changed
    $properties['entity_changed']['setter callback'] = 'entity_property_verbatim_set';
    $properties['entity_changed']['type'] = 'date';

    // Created
    $properties['created']['setter callback'] = 'entity_property_verbatim_set';
    $properties['created']['type'] = 'date';

    // Changed
    $properties['changed']['setter callback'] = 'entity_property_verbatim_set';
    $properties['changed']['type'] = 'date';

    $properties['is_linkable'] = array(
      'label' => t('Is linkable'),
      'type' => 'boolean',
      'getter callback' => 'sharedcontent_is_linkable',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );

    // Add a calculated property for each flag associated with this entity.
    if (module_exists('flag')) {
      foreach (flag_get_flags('sharedcontent_index') as $key => $flag) {
        $flag_info = array(
          'label' => $flag->title,
          'type' => 'boolean',
          'getter callback' => 'sharedcontent_flag_value_get',
          'computed' => TRUE,
          // There might come the situation when we want to display this
          // attributes inside views. If this is the cas uncomment the
          // following line will allow to do so.
          // 'entity views field' => TRUE,
        );
        $properties[$key] = $flag_info;
      }
    }

    return $info;
  }

}

/**
 * Metadata controller for the sharedcontent assignment
 */
class SharedContentAssignmentMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = & $info[$this->type]['properties'];

    // id
    $properties['id']['setter callback'] = 'entity_property_verbatim_set';

    // Origin - the bundle
    $properties['origin']['setter callback'] = 'entity_property_verbatim_set';
    $properties['origin']['label'] = t('Bundle');

    // source_id
    $properties['source_id']['setter callback'] = 'entity_property_verbatim_set';
    $properties['source_id']['type'] = 'sharedcontent_index';

    // source
    $properties['source']['setter callback'] = 'entity_property_verbatim_set';

    // target_id
    $properties['target_id']['setter callback'] = 'entity_property_verbatim_set';
    $properties['target_id']['type'] = 'sharedcontent_index';

    // target
    $properties['target']['setter callback'] = 'entity_property_verbatim_set';

    // status
    $properties['status']['setter callback'] = 'entity_property_verbatim_set';
    $properties['status']['options list'] = 'sharedcontent_get_assignment_status_labels';

    // url
    $properties['url']['setter callback'] = 'entity_property_verbatim_set';

    // created
    $properties['created']['setter callback'] = 'entity_property_verbatim_set';
    $properties['created']['type'] = 'date';

    // changed
    $properties['changed']['setter callback'] = 'entity_property_verbatim_set';
    $properties['changed']['type'] = 'date';

    return $info;
  }

}
