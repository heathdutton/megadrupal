<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class FacebookPublicationMetadataController extends EntityDefaultMetadataController {

  /**
   * Metadata controller information.
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['label'] = array(
      'label' => t('Label'),
      'description' => t('The Facebook publication label.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer facebook publications',
      'schema field' => 'label',
    );

    $properties['type'] = array(
      'type' => 'facebook_publication_type',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer facebook publications',
      'required' => TRUE,
      'description' => t('The Facebook publication type.'),
    ) + $properties['type'];

    unset($properties['uid']);

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the Facebook publication."),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      'setter permission' => 'administer facebook publications',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the Facebook publication was created."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer facebook publications',
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the Facebook publication was most recently updated."),
    );
    $properties['facebook_id'] = array(
      'label' => t("Facebook publication remote ID"),
      'type' => 'text',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer facebook publications',
      'schema field' => 'facebook_id',
      'description' => t("The ID returned by Facebook for the publication. This ID is needed to delete or edit the publication in Facebook."),
    );

    return $info;
  }
}
