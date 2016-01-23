<?php

/**
 * Node license type.
 *
 * This is a child license type of role_node, created for each node published
 * by the user.
 */
class CommerceLicenseNode extends CommerceLicenseBase  {

  /**
   * Implements EntityBundlePluginProvideFieldsInterface::fields().
   */
  static function fields() {
    $fields = parent::fields();
    $fields['cl_licensed_node']['field'] = array(
      'cardinality' => 1,
      'translatable' => FALSE,
      'settings' => array(
        'target_type' => 'node',
        'handler' => 'base',
      ),
      'module' => 'entityreference',
      'type' => 'entityreference',
    );
    $fields['cl_licensed_node']['instance'] = array(
      'label' => t('Licensed node'),
      'display' => array(
        'full' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 10,
        ),
        'line_item' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 10,
        ),
      ),
    );

    return $fields;
  }

}
