<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extends the defaults metadata controller by setting property types.
 */
class HierarchyMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['type'] = array(
      'label' => t('Type'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'create hierarchies',
      'required' => TRUE,
      'description' => t('The hierarchy type.'),
      'type' => 'token',
      'options list' => 'hierarchy_type_get_names',
    ) + $properties['type'];

    // Unset the uid property, as it is available via the user.
    unset($properties['uid']);

    $properties['user'] = array(
      'label' => t('User'),
      'type' => 'user',
      'description' => t('The user that created the hierarchy.'),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      'setter permission' => 'create hierarchies',
      'schema field' => 'uid',
      'required' => TRUE,
    );

    $properties['timestamp'] = array(
      'label' => t('Creation time'),
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'create hierarchies',
      'description' => t('The time when the hierarchy was created.'),
    ) + $properties['timestamp'];

    return $info;
  }

}

/**
 * Extends the defaults metadata controller by setting property types.
 */
class HierarchyRelationMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['field_name'] = array(
      'getter callback' => 'entity_property_verbatim_get',
      'description' => t('The name of the relation field on the host entity.'),
      'type' => 'token',
    ) + $properties['field_name'];

    unset($properties['field_name']['setter callback']);

    $properties['weight'] = array(
      'label' => t('Weight'),
      'type' => 'integer',
      'description' => t('The weight of item among its siblings.'),
      'getter callback' => 'entity_property_verbatim_get',
    ) + $properties['weight'];

    unset($properties['weight']['setter callback']);

    $properties['host_entity'] = array(
      'label' => t('Host entity'),
      'type' => 'entity',
      'description' => t('The entity containing the entity position field.'),
      'getter callback' => 'hierarchy_relation_get_host_entity',
    );

    return $info;
  }

}
