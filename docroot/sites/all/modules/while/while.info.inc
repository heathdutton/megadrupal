<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class WhileMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['type'] = array(
      'type' => 'while_type',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer whiles',
      'required' => TRUE,
      'description' => while_get_string('type label'),
    ) + $properties['type'];

    $properties['label'] = array(
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'label' => t('Label'),
      'schema field' => 'label',
    );

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the @while.", array('@while' => while_get_string('entity label'))),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      'setter permission' => 'administer whiles',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the @while was created.", array('@while' => while_get_string('entity label'))),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer whiles',
      'schema field' => 'created',
    );

    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the @while was most recently updated.", array('@while' => while_get_string('entity label'))),
    );

    $properties['status'] = array(
      'label' => t("Status"),
      'type' => 'boolean',
      'schema field' => 'status',
      'description' => t("Whether the @while is active (true) or not (false).", array('@while' => while_get_string('entity label'))),
    );

    return $info;
  }
}
