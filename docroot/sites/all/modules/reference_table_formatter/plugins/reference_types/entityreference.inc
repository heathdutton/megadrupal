<?php
/**
 * @file
 * A reference type plugin to describe field collections.
 */
$plugin = array(
  'handler' => array(
    'class' => 'reference_table_formatter_entityreference',
  )
);

/**
 * Class reference_table_formatter_field_collection
 */
class reference_table_formatter_entityreference extends reference_table_formatter_base_type {
  /**
   * {@inheritdoc}
   */
  public function get_bundles() {
    // If we are using the base selection, the bundles are easy to find.
    if ($this->field['settings']['handler'] == 'base' && isset($this->field['settings']['handler_settings']['target_bundles']) && count($this->field['settings']['handler_settings']['target_bundles']) > 0) {
      return array_keys($this->field['settings']['handler_settings']['target_bundles']);
    }
    else {
      // Otherwise use all bundles since it's not possible to say for certain
      // which bundles will appear for the field.
      $info = entity_get_info($this->entity_name());
      return array_keys($info['bundles']);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function entity_name() {
    return $this->field['settings']['target_type'];
  }

  /**
   * {@inheritdoc}
   */
  function renderable_properties() {
    $entity_name = $this->entity_name();
    $common_properties = array(
      'node' => array('title'),
      'user' => array('name'),
      'taxonomy_term' => array('name'),
    );
    return isset($common_properties[$entity_name]) ? $common_properties[$entity_name] : array();
  }

  /**
   * {@inheritdoc}
   */
  function load_entity_from_item($item) {
    $loaded_entities = entity_load($this->entity_name(), array($item['target_id']));
    return array_shift($loaded_entities);
  }

  /**
   * @param $info
   * @return string
   */
  function get_entity_property_label($info) {
    $bundles = $this->get_bundles();
    // It's a unique situation for nodes, to be able to override the label
    // for the "title" property.
    if (count($bundles) == 1 && $this->entity_name() == 'node') {
      $type = node_type_load($bundles[0]);
      return $type->title_label;
    }
    else {
      return parent::get_entity_property_label($info);
    }
  }
}
