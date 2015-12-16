<?php

/**
* @file
* Provides the settings form.
*/

/**
 * Form builder function for module settings.
 */
function fieldvariables_settings() {
  $form = array();

  $entities = entity_get_info();

  $form['information'] = array(
    '#markup' => t('Select bundles and view modes for which field variables should be loaded.'),
  );

  foreach ($entities as $entity => $entity_values) {
    if (!empty($entity_values['view modes']) && !empty($entity_values['bundles'])) {
      $form['entity_' . $entity] = array(
        '#type' => 'fieldset',
        '#title' => $entity_values['label'],
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['entity_' . $entity]['fieldvariables_entity_' . $entity] = array(
        '#type' => 'checkbox',
        '#title' => t('All @entity bundles', array('@entity' => $entity_values['label'])),
        '#attributes' => array('class' => array('all-bundles')),
        '#default_value' => variable_get('fieldvariables_entity_' . $entity),
      );

      // Loop bundles of this entity.
      foreach ($entity_values['bundles'] as $bundle => $bundle_values) {
        $form['entity_' . $entity]['entity_' . $entity . '_' . $bundle] = array(
          '#type' => 'fieldset',
          '#title' => $bundle_values['label'],
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
        );

        // Loop view modes for this entity.
        foreach ($entity_values['view modes'] as $view_mode => $view_mode_values) {
          $form['entity_' . $entity]['entity_' . $entity . '_' . $bundle]['fieldvariables_entity_' . $entity . '_' . $bundle . '_' . $view_mode] = array(
            '#type' => 'checkbox',
            '#title' => $view_mode_values['label'],
            '#default_value' => variable_get('fieldvariables_entity_' . $entity . '_' . $bundle . '_' . $view_mode),
          );
        }
      }
    }
  }

  return system_settings_form($form);
}
