<?php

/**
 * @file
 * Mood reaction class for Context integration.
 */

class context_mood_context_reaction_mood extends context_reaction {
  function options_form($context) {
    // Fetching saved values.
    $values = $this->fetch_from_context($context);
    $form = array();

    // Getting all entity types.
    $entities = entity_get_info();
    $entity_types = array();
    $view_modes = array();

    // Iterate all entity types and prepare an array with entity type
    // and their label.
    foreach ($entities as $entity => $entity_info) {
      $entity_types[$entity] = $entity_info['label'];
      foreach ($entity_info['view modes'] as $view_mode_name => $view_mode) {
        $view_modes[$entity][$view_mode_name] = $view_mode['label'];
      }
    }

    $form['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Entity type'),
      '#options' => $entity_types,
      '#default_value' => isset($values['entity_type']) ? $values['entity_type'] : '',
      '#empty_option' => t('Select entity type'),
    );

    // If entity type and entity id is available, we can display the entity
    // label as description for entity id field.
    if (isset($values['entity_type']) && isset($values['entity_id'])) {
      $entity_type = $values['entity_type'];
      $entity_id = $values['entity_id'];

      $entity = entity_load($entity_type, array($entity_id));
      $entity = reset($entity);
      $label = entity_label($entity_type, $entity);
      if ($label) {
        $entity_description = t('Title: %label', array('%label' => $label));
      }
    }

    $form['entity_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Entity id'),
      '#default_value' => isset($values['entity_id']) ? $values['entity_id'] : '',
      '#description' => isset($entity_description) ? $entity_description : '',
      '#states' => array(
        'visible' => array(
          ':input[name$="[mood][entity_type]"]' => array('!value' => ''),
        ),
      ),
      '#element_validate' => array('element_validate_integer_positive'),
    );

    // Provide a select list with view modes for every entity type.
    foreach ($view_modes as $entity_type => $entity_view_modes) {
      $form['view_mode'][$entity_type] = array(
        '#type' => 'select',
        '#title' => t('View mode'),
        '#options' => $entity_view_modes,
        '#default_value' => isset($values['entity_type']) ? $values['view_mode'][$values['entity_type']] : '',
        '#description' => t('The view mode which should be used.'),
        '#empty_option' => t('Select view mode'),
        '#states' => array(
          'visible' => array(
            ':input[name$="[mood][entity_type]"]' => array('value' => $entity_type),
          ),
        ),
      );
    }

    return $form;
  }

  function execute() {
    foreach ($this->get_contexts() as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        // Getting context settings.
        $settings = $context->reactions[$this->plugin];

        // Normalize the settings.
        $entity_type = $settings['entity_type'];
        $entity_id = $settings['entity_id'];
        $view_mode = $settings['view_mode'][$entity_type];

        // Load the entity.
        $entity = entity_load($entity_type, array($entity_id));
        // Getting the render array for the entity with the specific view mode.
        $entity = entity_view($entity_type, $entity, $view_mode);

        return $entity;
      }
    }
  }
}
