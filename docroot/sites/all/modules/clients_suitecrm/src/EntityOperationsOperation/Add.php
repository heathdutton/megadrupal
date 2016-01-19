<?php
/**
 * @file
 * Generic entity form for suiteCRM entities.
 */

namespace Drupal\clients_suitecrm\EntityOperationsOperation;

/**
 * Op handler for editing a SuiteCRM entity.
 */
class Add extends Edit {

  public $access_verb = 'create';

  /**
   * Returns strings for the operations.
   */
  public function operationStrings() {
    return array(
      'tab title' => 'Add',
      'label' => 'Add',
      'description' => 'Create a new entity, with a bundle selection page if required.',
      // Note page title is set by setTitle().
    ) + parent::operationStrings();
  }

  /**
   * Properties for the menu item specific to this handler.
   */
  public function menu_item($operation_path, $operation_definition, $loader_position) {
    $entity_info = entity_get_info($this->entityType);

    $form_id = $this->getFormID($this->entityType, $operation_path);

    $item = array(
      // Special path which doesn't have an entity loader.
      'path' => $entity_info['operations ui']['path'] . '/add',
      // We don't want a tab.
      'type' => MENU_CALLBACK,
      // Although we have EntityOperationsOperationForm in our ancestry, we use
      // the page callback as our output is not always a form.
      'page callback' => 'entity_operations_operation_page_callback',
      // Change the callback arguments to pass NULL for the entity parameter.
      'page arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        // The path component after 'add', which should be the bundle name.
        // This is passed into the form and access callbacks as the $entity.
        $loader_position + 1,
      ),
      'access arguments' => array(
        $this->entityType,
        get_class($this),
        $operation_path,
        $loader_position + 1,
      ),
    );

    // Add in what the parent class has.
    $item += parent::menu_item($operation_path, $operation_definition, $loader_position);
    return $item;
  }

  /**
   * Helper to set the page title.
   *
   * @see build() for the parameters.
   */
  public function setTitle($entity_type, $entity, $params = array()) {
    if (empty($entity)) {
      module_load_include('inc', 'entity', 'includes/entity.ui');
      drupal_set_title(entity_ui_get_action_title('add', $entity_type));
    }
    else {
      // We're on /add/bundle.
      // build() will call entity_ui_get_bundle_add_form() which takes care of
      // setting the title.
    }
  }

  /**
   * Build the operation render array.
   *
   * @param string $entity_type
   *   The entity type.
   * @param object $entity
   *   The entity. Pass NULL if there is no entity in context.
   * @param array $params
   *   (optional) An array of additional parameters. It is up to subclasses that
   *   require this to document what they expect.
   *
   * @return array
   *   Output suitable for a page callback.
   */
  public function build($entity_type, $entity, $params = array()) {
    // There are several different cases for two different results.
    $entity_info = entity_get_info($this->entityType);

    // The $entity parameter is in fact the bundle name.
    $bundle_name = $entity;

    if (count($entity_info['bundles']) == 1) {
      // Case 1: we are on /add, and the entity type has only one bundle (note
      // that even if hook_entity_info() doesn't define any bundles,
      // entity_get_info() defines one with the same name as the entity).
      // Just show the add form.
      $output = 'form';

      $values = array();

      // Create a new entity, setting the bundle key, if we need it.
      // Must use empty(), as entity_get_info() defaults this to ''.
      if (!empty($entity_info['entity keys']['bundle'])) {
        // Set a variable to avoid a strict warning with reset().
        $bundle_names = array_keys($entity_info['bundles']);
        $bundle_name = reset($bundle_names);

        $bundle_key = $entity_info['entity keys']['bundle'];
        $values[$bundle_key] = $bundle_name;
      }

      $entity = entity_create($entity_type, $values);
    }
    elseif (empty($bundle_name)) {
      // Case 2: we are on /add, and we don't know which bundle to use. Show a
      // list of links.
      $output = 'links';
    }
    else {
      // Case 3: we are on /add/BUNDLE. Show the add form.
      $output = 'form';

      // Create an entity of the right bundle.
      $values = array();
      $bundle_key = $entity_info['entity keys']['bundle'];
      $values[$bundle_key] = $bundle_name;

      $entity = entity_create($entity_type, $values);
    }

    // Now return the output, depending on what's required.
    switch ($output) {
      case 'links':
        return $this->bundleSelectionList($entity_type);

      case 'form':
        // Return the form.
        // This will:
        // - look up the form ID with our hook_forms().
        // - use the base form builder entity_operations_operation_form().
        // - go back into this handler's form().
        // - call entityForm(), which calls the parent method
        //   EntityOperationsOperationEditGeneric::entityForm().
        $form_id = $this->getFormID($entity_type, $this->path);
        return drupal_get_form($form_id, $entity_type, get_class($this), $this->path, $entity);
    }
  }

  /**
   * Form builder for this operation.
   */
  public function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Kill the page title.
    unset($form_state['entity_operation_form_elements']['page title']);

    // Kill the cancel link.
    unset($form_state['entity_operation_form_elements']['cancel link']);

    return $this->entityForm($form, $form_state, $entity_type, $entity, $operation_path, 'add');
  }
}
