<?php
/**
 * @file
 * Contains Page manager search Entity controller class.
 */

class PageManagerSearchEntityController extends DrupalDefaultEntityController {

  /**
   * Constructor: sets basic variables.
   */
  public function __construct($entityType) {
    $this->entityType = $entityType;
    // $this->entityInfo = entity_get_info($entityType);
    $this->entityCache = array();
    $this->hookLoadArguments = array();
    // $this->idKey = $this->entityInfo['entity keys']['id'];
    $this->revisionKey = FALSE;

    // Check if the entity type supports static caching of loaded entities.
    $this->cache = !empty($this->entityInfo['static cache']);
  }

  /**
   * Implements DrupalEntityControllerInterface::load().
   */
  public function load($ids = array(), $conditions = array()) {
    $entities = array();

    $conditions += array(
      'length' => 9,
    );

    // Get all IDs if no IDs are provided.
    if (empty($ids)) {
      $ids = array_keys(page_manager_search_get_pages_by_dec(NULL, $conditions['length']));
    }

    module_load_include('inc', 'page_manager', 'plugins/tasks/page');
    foreach ($ids as $id) {
      $subtask = page_manager_search_get_pages_by_dec($id, $conditions['length']);

      $entity = new stdClass();
      $entity->pid = $id;
      $entity->title = isset($subtask->menu['title']) ? $subtask->menu['title'] : $subtask->admin_title;
      $entity->content = page_manager_page_execute($subtask->name);
      if (is_array($entity->content)) {
        $entity->content = drupal_render($entity->content);
      }
      $entity->path = $subtask->path;

      $entities[$id] = $entity;
    }

    return $entities;
  }

}
