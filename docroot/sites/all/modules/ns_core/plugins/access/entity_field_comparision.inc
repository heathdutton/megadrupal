<?php

/**
 * @file
 * Plugin to provide access control/visibility
 */
$plugin = array(
  'title' => t('Entity: field comparison'),
  'description' => t('Control access by string match\'ed to taxonomy terms field value'),
  'callback' => 'ns_core_entity_field_comparison_ctools_access_check',
  'default' => array(
    'field' => array(),
    'operator' => '=',
    'value' => 0,
  ),
  'settings form' => 'ns_core_entity_field_comparison_ctools_access_settings',
  'summary' => 'ns_core_entity_field_comparison_ctools_access_summary',
//  'restrictions' => 'ns_core_entity_field_comparison_ctools_access_restrictions',
  'get child' => 'ns_core_entity_field_comparison_ctools_access_get_child',
  'get children' => 'ns_core_entity_field_comparison_ctools_access_get_children',
);

function ns_core_entity_field_comparison_ctools_access_get_child($plugin, $parent, $child) {
  $plugins = ns_core_entity_field_comparison_ctools_access_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function ns_core_entity_field_comparison_ctools_access_get_children($plugin, $parent) {
  $entities = entity_get_info();

  $plugins = array();
  foreach ($entities as $entity_type => $entity) {
    $plugin['title'] = t('@entity: Field', array('@entity' => $entity['label']));
    $plugin['keyword'] = $entity_type;
    $plugin['description'] = t('Control access by @entity entity field.', array('@entity' => $entity_type));
    $plugin['name'] = $parent . ':' . $entity_type;
    $plugin['required context'] = new ctools_context_required(t(ucfirst($entity_type)), $entity_type);
    $plugins[$parent . ':' . $entity_type] = $plugin;
  }

  return $plugins;
}

/**
 * Settings form!
 */
function ns_core_entity_field_comparison_ctools_access_settings($form, &$form_state, $conf) {
  $plugin = $form_state['plugin'];
  $entity_type = explode(':', $plugin['name']);
  $entity_type = $entity_type[1];
  $entity = entity_get_info($entity_type);
  foreach ($entity['bundles'] as $bundle_name => $bundle) {
    $fields = field_info_instances($entity_type, $bundle_name);
    foreach ($fields as $field_type => $field) {
      $options[$entity_type . ':' . $bundle_name . ':' . $field_type] = check_plain($bundle['label'] . ': ' . $field['label']);
    }
  }
  $form['settings']['field'] = array(
    '#title' => t('Field'),
    '#type' => 'select',
    '#options' => $options,
    '#description' => t('The field to check against'),
    '#default_value' => $conf['field'],
  );

  $form['settings']['operator'] = array(
    '#type' => 'radios',
    '#title' => t('Operator'),
    '#options' => array(
      '=' => t('Equal'),
    ),
    '#default_value' => $conf['operator'],
  );

  $form['settings']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value to compare field with'),
    '#default_value' => $conf['value'],
  );

  return $form;
}

function ns_core_entity_field_comparison_ctools_access_check($conf, $context) {
  $field = explode(':', $conf['field']);
  $bundle_name = $field[1];
  $field = $field[2];

  if ($context->restrictions['type'][0] == $bundle_name) {
    return ($context->data->{$field}['und'][0]['value'] == $conf['value']);
  }
  return FALSE;
}

function ns_core_entity_field_comparison_ctools_access_summary($conf, $context) {
  $entity = entity_get_info($context->type[2]);

  $names = array();
  foreach ($entity['bundles'] as $bundle_name => $bundle) {
    $fields = field_info_instances($context->type[2], $bundle_name);
    foreach ($fields as $field_type => $field) {
      if (($context->type[2] . ':' . $bundle_name . ':' . $field_type) == $conf['field']) {
        $names[] = check_plain($bundle['label'] . ': ' . $field['label']);
      }
    }
  }

  $operators = array(
    '=' => t('Equal to'),
  );

  return t('The field @identifier should be @operator "@value"', array('@identifier' => implode(', ', $names), '@operator' => $operators[$conf['operator']], '@value' => check_plain($conf['value'])));
}
