<?php

class Project_SelectionHandler_ProjectBehavior extends EntityReference_SelectionHandler_Generic {
  public $behaviorType = NULL;

  /**
   * Factory function: create a new instance of this handler for a given field.
   *
   * @param $field
   *   A field datastructure.
   * @return EntityReferenceHandler
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $target_entity_type = $field['settings']['target_type'];

    // Check if the entity type does exist and has a base table.
    $entity_info = entity_get_info($target_entity_type);
    if (empty($entity_info['base table']) || $target_entity_type != 'node') {
      return EntityReference_SelectionHandler_Broken::getInstance($field, $instance);
    }

    return new Project_SelectionHandler_ProjectBehavior($field, $instance, $entity_type, $entity);
  }

  /**
   * Generate a settings form for this handler.
   */
  public static function settingsForm($field, $instance) {
    $project_behaviors = project_get_behavior_info();
    $project_behavior_options = array();
    foreach ($project_behaviors as $module => $behavior_info) {
      $machine_name = $behavior_info['machine name'];
      $project_behavior_options[$machine_name] = $behavior_info['label'];
    }

    $form['behavior'] = array(
      '#type' => 'select',
      '#title' => t('Project behavior'),
      '#options' => $project_behavior_options,
      '#default_value' => !empty($field['settings']['handler_settings']['behavior']) ? $field['settings']['handler_settings']['behavior'] : 'none',
    );

    $form += parent::settingsForm($field, $instance);

    // We're overriding the bundles.
    unset($form['target_bundles']);

    return $form;
  }

  /**
   * Build an EntityFieldQuery to get referencable entities.
   * Almost the same as EntityReference_SelectionHandler_Generic::buildEntityFieldQuery,
   * but the bundles are dynamic.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->field['settings']['target_type']);
    $node_types = project_node_types_by_behavior($this->field['settings']['handler_settings']['behavior']);
    if (!empty($node_types)) {
      $query->entityCondition('bundle', $node_types, 'IN');
    }
    if (isset($match)) {
      $entity_info = entity_get_info($this->field['settings']['target_type']);
      if (isset($entity_info['entity keys']['label'])) {
        $query->propertyCondition($entity_info['entity keys']['label'], $match, $match_operator);
      }
    }

    // Add a generic entity access tag to the query.
    $query->addTag($this->field['settings']['target_type'] . '_access');
    $query->addTag('entityreference');
    $query->addMetaData('field', $this->field);
    $query->addMetaData('entityreference_selection_handler', $this);

    // Add the sort option.
    if (!empty($this->field['settings']['handler_settings']['sort'])) {
      $sort_settings = $this->field['settings']['handler_settings']['sort'];
      if ($sort_settings['type'] == 'property') {
        $query->propertyOrderBy($sort_settings['property'], $sort_settings['direction']);
      }
      elseif ($sort_settings['type'] == 'field') {
        list($field, $column) = explode(':', $sort_settings['field'], 2);
        $query->fieldOrderBy($field, $column, $sort_settings['direction']);
      }
    }

    return $query;
  }
}
