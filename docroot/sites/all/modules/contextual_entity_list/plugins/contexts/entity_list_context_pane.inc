<?php
/**
 * @file
 * ctools context type plugin.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Entity Type"),
  'description' => t('Entity Type as contextual entity.'),
  'context' => 'contextual_entity_list_entity_list_create_context_pane',
  'context name' => 'contextual_entity_list_entity_list_context_pane',
  'edit form' => 'contextual_entity_list_entity_list_context_pane_settings_form',
  'keyword' => 'contextual-entity',
  'convert list' => 'contextual_entity_list_entity_list_context_pane_convert_list',
  'convert' => 'contextual_entity_list_entity_list_context_pane_convert',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter some data to represent this Contextual entity'),
  ),
);

/**
 * Context callback.
 *
 * @param boolean $empty
 *   If true, just return an empty context.
 * @param object $data
 *   return the configuration provide by context setting form.
 * @param boolean $conf
 *   TRUE if the $data is coming from admin configuration.
 *
 * @return object
 *   A render object for a page containing a list of content.
 */
function contextual_entity_list_entity_list_create_context_pane($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('contextual_entity_list_entity_list_context_pane');
  $context->plugin = 'contextual_entity_list_entity_list_context_pane';

  if ($empty) {
    return $context;
  }

  if ($conf) {
    if (!empty($data)) {
      $context->data = new stdClass();
      $context->data->contextdata = $data;
      return $context;
    }
  }
}

/**
 * Context configuration for entity type.
 */
function contextual_entity_list_entity_list_context_pane_settings_form($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $conf = $form_state['conf'];
  if (isset($conf) && !empty($conf)) {
    $bundleoptions = _contextual_entity_list_entity_options($conf['entity-type']);
  }
  $form['entity-type'] = array(
    '#title' => t('Entity type'),
    '#type' => 'select',
    '#options' => _contextual_entity_list_entity_type_list(),
    '#default_value' => $conf['entity-type'],
    '#description' => t('Select the Entity type for this form.'),
    '#ajax' => array(
      'path' => 'bundles',
    ),
  );
  $form['bundle'] = array(
    '#title' => t('Bundle type'),
    '#type' => 'select',
    '#validated' => 'true',
    '#options' => isset($bundleoptions) ? $bundleoptions : array(),
    '#default_value' => $conf['bundle'],
    '#description' => t('Select the bundle for this form.'),
  );
  return $form;
}

/**
 * Context-Configuration form submit.
 */
function contextual_entity_list_entity_list_context_pane_settings_form_submit($form, $form_state) {
  $form_state['conf']['entity-type'] = $form_state['values']['entity-type'];
  $form_state['conf']['bundle'] = $form_state['values']['bundle'];
}

/**
 * Listing the available entity -type.
 */
function _contextual_entity_list_entity_type_list() {
  $entity_types_list = array();
  foreach (entity_get_info() as $entitykey => $entityvalue) {
    $entity_types_list[$entitykey] = $entityvalue['label'];
  }
  return $entity_types_list;
}

/**
 * Available nodes for a Node-type bundle.
 */
function _contextual_entity_list_entity_node_list($entity_type, $context_bundle) {

  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', $entity_type)
    ->entityCondition('bundle', $context_bundle)
    ->propertyCondition('status', 1)
    ->execute();
  return $entities['node'];
}

/**
 * Provide a list of sub-keywords.
 *
 * This is used to provide keywords from the context for use in a content type,
 * pane, etc.
 */
function contextual_entity_list_entity_list_context_pane_convert_list() {

  return array(
    'nid' => t('Node Id'),
    'nodetype' => t('Node type'),
  );
}


/**
 * Convert the context into string.
 */
function contextual_entity_list_entity_list_context_pane_convert($context, $type) {
  switch ($type) {
    case 'nid':
      return $context->data->nid;

    case 'nodetype':
      return $context->data->nodetype;
  }
}
