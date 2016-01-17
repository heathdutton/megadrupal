<?php

class SalsaEntityEntityReferenceSelectionHandler extends EntityReference_SelectionHandler_Generic {
  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new SalsaEntityEntityReferenceSelectionHandler($field, $instance);
  }

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    $entity_info = entity_get_info($field['settings']['target_type']);

    if (!empty($entity_info['entity keys']['bundle'])) {
      $bundles = array();
      foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
        $bundles[$bundle_name] = $bundle_info['label'];
      }

      $form['target_bundles'] = array(
        '#type' => 'select',
        '#title' => t('Target bundles'),
        '#options' => $bundles,
        '#default_value' => isset($field['settings']['handler_settings']['target_bundles']) ? $field['settings']['handler_settings']['target_bundles'] : array(),
        '#size' => 6,
        '#multiple' => TRUE,
        '#description' => t('The bundles of the entity type that can be referenced. Optional, leave empty for all bundles.')
      );
    }
    else {
      $form['target_bundles'] = array(
        '#type' => 'value',
        '#value' => array(),
      );
    }

    $form['sort']['type'] = array(
      '#type' => 'radios',
      '#title' => t('Sort by'),
      '#options' => array(
        'none' => t("Don't sort"),
        'property' => t('A property of the base table of the entity'),
      ),
      '#default_value' => isset($field['settings']['handler_settings']['sort']['type']) ? $field['settings']['handler_settings']['sort']['type'] : 'none',
    );



    $form['sort']['property'] = array(
      '#type' => 'select',
      '#title' => t('Sort property'),
      '#options' => drupal_map_assoc(array_keys(entity_get_all_property_info($field['settings']['target_type']))),
      '#default_value' => isset($field['settings']['handler_settings']['sort']['property']) ? $field['settings']['handler_settings']['sort']['property'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="field[settings][handler_settings][sort][type]"]' => array('value' => 'property'),
        ),
      ),
    );

    $form['sort']['direction'] = array(
      '#type' => 'select',
      '#title' => t('Sort direction'),
      '#options' => array(
        'ASC' => t('Ascending'),
        'DESC' => t('Descending'),
      ),
      '#default_value' => isset($field['settings']['handler_settings']['sort']['direction']) ? $field['settings']['handler_settings']['sort']['direction'] : 'ASC',
      '#states' => array(
        'invisible' => array(
          ':input[name="field[settings][handler_settings][sort][type]"]' => array('value' => 'none'),
        ),
      ),
    );

    return $form;
  }

  /**
   * Implements EntityReferenceHandler::getLabel().
   */
  public function getLabel($entity) {
    $entity_info = entity_get_info($this->field['settings']['target_type']);
    if (isset($entity_info['entity keys']['reference'])) {
      return $entity->{$entity_info['entity keys']['reference']};
    }
    return entity_label($this->field['settings']['target_type'], $entity);
  }
}
