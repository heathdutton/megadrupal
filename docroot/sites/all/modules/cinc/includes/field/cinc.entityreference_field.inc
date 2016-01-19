<?php

/**
 * Creates entityreference that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('EntityReferenceField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_EntityReferenceField extends CINC_Field {

  public static $dependencies = array('field', 'entityreference');
  public static $field_types = array('entityreference');

  function __construct() {

    parent::__construct();

    $this->config_filters['cardinality'] = 'set_cardinality';

    $this
      ->set('type', 'entityreference')
      ->set('module', 'entityreference')
      ->set('label', t('Entity Reference'))
      ->set('description', t('This field reference another entity.'))
      ->set('settings', array(
          // Default to the core target entity type node.
          'target_type' => 'node',
          // The handler for this field.
          'handler' => 'base',
          // The handler settings.
          'handler_settings' => array(
            'target_bundles' => array(),
            'sort' => array('type' => 'none'),
            'behaviors' => array(
              'views-select-list' => array('status' => 0)
              ),
            ),
        ))
      ->set('instance_settings', array())
      ->set('default_widget', 'entityreference_autocomplete')
      ->set('default_formatter', 'entityreference_label')
      ->set('property_callbacks', array('entityreference_field_property_callback'));
  }

  public function set_cardinality(&$cardinality) {
    if ($cardinality == 0) {
      $cardinality = -1;
    }
    return TRUE;
  }

}
