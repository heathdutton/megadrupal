<?php

/**
 * Return an ief fapi array using inline entity form.
 * @param type $entity_collection
 * @param type $entity_types
 */
function entity_collection_ief_form($form, &$form_state, $entity_collection, $entity_type, $entity) {
  $controllers = entity_collection_ief_controllers();
  $controller = $form_state['controller'] = new $controllers[$entity_type]($entity_type, array());
  $form_state['entity_collection'] = $entity_collection;
  $form_state['entity_type'] = $entity_type;
  $form['new_entity'] = array();
  if (!is_object($entity)) {
    $form['new_entity']['#entity'] = inline_entity_form_create_entity($entity_type, $entity);
  }
  else {
    $form['new_entity']['#entity'] = $entity;
  }
  $form['new_entity'] = $controller->entityForm($form['new_entity'], $form_state);
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Validator callback.
 */
function entity_collection_ief_form_validate($form, &$form_state) {
  $form_state['controller']->entityFormValidate($form['new_entity'], $form_state);
}

/**
 * Submit handler for the IEF form.
 */
function entity_collection_ief_form_submit($form, &$form_state) {
  $entity_collection = $form_state['entity_collection'];
  $controller = $form_state['controller'];
  $entity = $form['new_entity']['#entity'];
  $entity_type = $form_state['entity_type'];
  $contexts = _entity_collection_get_contexts($entity_collection);
  $controller->entityFormSubmit($form['new_entity'], $form_state);
  $controller->save($entity, array('entity_collection', $entity_collection));
  $entity_id = entity_id($entity_type, $entity);
  $item = new EntityCollectionTreeNode($entity_type, $entity_id, $entity);
  entity_collection_append_item($entity_collection, $item, $contexts);
}

/**
 * Get URLs for all content types that can be created on the fly and inserted
 * in an entity collection.
 * @param EntityCollection $entity_collection
 * @return
 *   An options array with urls as keys and bundle labels as titles.
 */
function entity_collection_ief_url_options(EntityCollection $entity_collection) {
  $entity_info = entity_get_info();
  $options = array();
  $controllers = entity_collection_ief_controllers();
  foreach ($entity_collection->settings['allowed_bundles'] as $entity_type => $bundles) {
    if (isset($controllers[$entity_type])) {
      foreach ($bundles as $bundle) {
        if (entity_access('create', $entity_type)) {
          $url_options = array('query' => array());
          if (!empty($entity_collection->contexts)) {
            $url_options['query']['contexts'] = _entity_collection_serialize_contexts($entity_collection);
          }
          $options[url("entity_collection/add/{$entity_collection->name}/$entity_type/$bundle/ajax", $url_options)] = $entity_info[$entity_type]['bundles'][$bundle]['label'];
        }
      }
    }
  }
  return $options;
}
/**
 * Returns a FAPI array with elements that can be used to open a modal
 * where a new node can be created and inserted in the collection provided.
 * @param array $form
 *   The form to insert the fapi elements in.
 * @param EntityCollection $entity_collection
 *   The entity collection to add elements to.
 * @return
 *   The form with an index, create_new.
 */
function entity_collection_ief_create_form($form, EntityCollection $entity_collection) {
  ctools_include('modal');
  ctools_modal_add_js();
  $form['create_new'] = array(
    '#type' => 'fieldset',
    '#description' => t('Create new content and place it in this collection directly.'),
    '#title' => t('Create new content'),
  );
  $form['create_new']['container'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['create_new']['container']['url'] = array(
    '#type' => 'select',
    '#options' => entity_collection_ief_url_options($entity_collection),
    '#attributes' => array('class' => array('edit-create-url')),
  );
  $form['create_new']['container']['create'] = array(
    '#type' => 'button',
    '#attributes' => array('class' => array('ctools-use-modal')),
    '#value' => t('Create new'),
    '#submit' => 'test',
  );
  return $form;
}

/**
 * Menu callback for creating an entity and inserting it into an entity collection.
 * @param EntityCollection $entity_collection
 * @param $entity_type
 *   The entity type to create
 * @param $entity_bundle
 *   The bundle to use.
 * @param $js
 *   If we are going to use with JS or not. JSON will be returned if it's true.
 */
function entity_collection_ief_add(EntityCollection $entity_collection, $entity_type, $entity_bundle, $js) {
  if (!$js) {
    return drupal_get_form('entity_collection_ief_form', $entity_collection, $entity_type, $entity_bundle);
  }
  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'ajax' => TRUE,
    'title' => 'My title',
    'build_info' => array(
      'args' => array(
        $entity_collection,
        $entity_type,
        $entity_bundle,
      ),
    ),
  );
  $commands = ctools_modal_form_wrapper('entity_collection_ief_form', $form_state);
  if (!empty($form_state['executed'])) {
    $commands = array();
    $commands[] = ctools_modal_command_dismiss();
    $commands[] = ctools_ajax_command_reload();
  }
  print ajax_render($commands);
  exit();
}

/**
 * Get all available IEF controllers.
 * @return
 *   An array of controller class names keyed by entity type.
 */
function entity_collection_ief_controllers() {
  $entity_info = entity_get_info();
  $controllers = array();
  foreach ($entity_info as $name => $info) {
    if (isset($info['inline entity form']['controller'])) {
      $controllers[$name] = $info['inline entity form']['controller'];
    }
  }
  return $controllers;
}
