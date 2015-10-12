<?php

/**
 * @file
 * Access plugin to check whether the entity has children.
 */

$plugin = array(
  'title' => t("Hierarchy: entity has children"),
  'description' => t("Control access by the entity children presence in hierarchy."),
  'callback' => 'hierarchy_hierarchy_has_children_access_check',
  'settings form' => 'hierarchy_hierarchy_has_children_access_settings',
  'summary' => 'hierarchy_hierarchy_has_children_access_summary',
  'default' => array('field' => ''),
  'get child' => 'hierarchy_hierarchy_has_children_get_child',
  'get children' => 'hierarchy_hierarchy_has_children_get_children',
);

function hierarchy_hierarchy_has_children_get_child($plugin, $parent, $child) {
  $plugins = hierarchy_hierarchy_has_children_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function hierarchy_hierarchy_has_children_get_children($plugin, $parent) {
  $plugins = array();
  $entity_types = array();

  foreach (field_info_field_map() as $field_name => $field_info) {
    if ($field_info['type'] === 'hierarchy_position') {
      foreach (array_keys($field_info['bundles']) as $entity_type) {
        $entity_types[$entity_type] = $entity_type;
      }
    }
  }

  foreach ($entity_types as $entity_type) {
    $entity = entity_get_info($entity_type);
    $plugin['title'] = t('Hierarchy: @entity has children', array('@entity' => $entity['label']));
    $plugin['keyword'] = $entity_type;
    $plugin['name'] = $parent . ':' . $entity_type;
    $plugin['required context'] = new ctools_context_required(t(ucfirst($entity_type)), $entity_type);
    $plugins[$parent . ':' . $entity_type] = $plugin;
  }

  return $plugins;
}

/**
 * Check for access.
 */
function hierarchy_hierarchy_has_children_access_check($conf, $context, $plugin) {
  list(, $entity_type) = explode(':', $plugin['name']);

  if ((empty($entity_type)) || (empty($context->data)) || (empty($conf['field']))) {
    return FALSE;
  }

  $entity = $context->data;
  $field = $conf['field'];

  $info = entity_get_info($entity_type);
  $entity_id = isset($entity->{$info['entity keys']['id']}) ? $entity->{$info['entity keys']['id']} : NULL;

  $query = new EntityFieldQuery();
  $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
  $query->fieldCondition($field, 'parent_type', $entity_type);
  $query->fieldCondition($field, 'parent_id', $entity_id);
  $query->range(0, 1);

  $entities = $query->execute();

  return !empty($entities);
}

/**
 * Settings form.
 */
function hierarchy_hierarchy_has_children_access_settings($form, &$form_state, $conf) {
  $fields = array();

  foreach (field_info_field_map() as $field_name => $field_info) {
    if ($field_info['type'] === 'hierarchy_position') {
      $fields[$field_name] = $field_name;
    }
  }

  $form['settings']['field'] = array(
    '#title' => t('Hierarchy position field'),
    '#type' => 'select',
    '#options' => $fields,
    '#default_value' => $conf['field'],
  );

  return $form;
}

/**
 * Provide a summary description.
 */
function hierarchy_hierarchy_has_children_access_summary($conf, $context, $plugin) {
  if (empty($conf['field'])) {
    return FALSE;
  }

  return t("Entity has children in hierarchy using field @field", array('@field' => $conf['field']));
}
