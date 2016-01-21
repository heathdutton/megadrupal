<?php

/**
 * @file user_entities.admin.inc
 */
 
/**
 * User Entities Settings Form
 *
 * Allows for configuration of the user_entities module settings
 */
function user_entities_settings($form, &$form_state) {
  // Get a list of all of the entities available and allow the user to select which
  // entities can have relationships saved to user accounts
  $entities = entity_get_info();
  // We're not doing associations for taxonomy vocabularies or files
  unset($entities['taxonomy_vocabulary']);
  unset($entities['file']);
  // Take the list and place it into a series of checkboxes the user can check to
  // include that entity type in relationship association interface. If the entity
  // has bundles, then get the bundles so we can refine our selections down to specific
  // bundles. By default, all bundles should be selected when initially selected.
  $bundles = array();
  $entity_options = array();
  foreach ($entities as $entity_key=>$entity) {
    $entity_options[$entity_key] = $entity['label'];
    if (is_array($entity['bundles'])) {
      foreach ($entity['bundles'] as $bundle_key=>$bundle) {
        $bundles[$entity_key][$bundle_key] = array(
          'entity_key' => $entity_key,
          'entity_label' => $entity['label'],
          'bundle_key' => $bundle_key,
          'bundle_label' => $bundle['label'],
        );
      }
    }
  }

  $form['entity_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Entity Associations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['entity_options']['user_entities_entity_options'] = array(
    '#title' => t('Entity Options'),
    '#description' => t('The entities users should be allowed to form relationships with.'),
    '#type' => 'checkboxes',
    '#options' => $entity_options,
    '#default_value' => variable_get('user_entities_entity_options', array()),
  );
 
  // Iterate through our entities and create the bundle specific selectors.
  foreach ($entities as $entity_key => $entity) {
    $bundle_options = array();
    $form['entity_options'][$entity_key] = array(
      '#type' => 'fieldset',
      '#title' => $entity['label'] . ' Bundles',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#states' => array(
        'visible' => array(
          ':input[name="user_entities_entity_options['.$entity_key.']"]' => array('checked' => TRUE),
        ),
        'expanded' => array(
          ':input[name="user_entities_entity_options['.$entity_key.']"]' => array('checked' => TRUE),
        ),
      ),
    );
    foreach ($bundles[$entity_key] as $bundle_key=>$bundle) {
      $bundle_options[$bundle_key] = $bundle['bundle_label'];
    }
    $form['entity_options'][$entity_key]['user_entities_' . $entity_key . '_bundles'] = array(
      '#type' => 'checkboxes',
      '#title' => $entity['label'] . ' ' . t('Bundles'),
      '#options' => $bundle_options,
      '#default_value' => variable_get('user_entities_' . $entity_key . '_bundles', array()),
    );
  }
  
  $form['user_entities_enable_content_type_links'] = array(
    '#title' => t('Enable associate/disassociate links at the bottom of eneity display page'),
    '#type' => 'checkbox',
    '#description' => t('If you want a link to appear with the other entity specific links under the entities content, check this box. Otherwise you will have to use the module-provided block and configure its options on the blocks page.'),
    '#default_value' => variable_get('user_entities_enable_content_type_links', 0),
  );
  $form['user_entities_content_link_associate'] = array(
    '#type' => 'textfield',
    '#title' => 'Text For Associate Link',
    '#default_value' => variable_get('user_entities_content_link_associate', 'Associate'),
    '#size' => 50,
    '#required' => TRUE,
  );
  $form['user_entities_content_link_disassociate'] = array(
    '#type' => 'textfield',
    '#title' => 'Text For Associate Link',
    '#default_value' => variable_get('user_entities_content_link_disassociate', 'Disassociate'),
    '#size' => 50,
    '#required' => TRUE,
  );
  $form['user_entities_content_link_class'] = array(
    '#type' => 'textfield',
    '#title' => 'Class Name For Associate/Disassociate Link',
    '#size' => 30,
    '#default_value' => variable_get('user_entities_content_link_class', NULL),
  );
  
  return system_settings_form($form);
}