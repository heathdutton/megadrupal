<?php

class EntityList {

  public $name = '';

  public $handler_plugin = NULL;

  public $handler_config = array();

  public $entities = array();

  protected $handler = NULL;

  protected $pager = FALSE;

  protected $pagerInitialized = FALSE;

  protected $built = FALSE;

  protected $executed = FALSE;

  /**
   * Load method that loads the configuration into the object itself.
   *
   * Since the CTools Export API is declarative by nature, we can't rely on
   * constructor injection and entitylist_list_create() as the only factory.
   */
  public function load() {
    $schema = drupal_get_schema('entitylist_lists');
    // Unserialize our serialized params.
    foreach ($schema['fields'] as $field => $info) {
      if (!empty($info['serialize']) && !is_array($this->{$field})) {
        $this->{$field} = (array) unserialize($this->{$field});
      }
    }
    $this->handler = new $this->handler_plugin($this->handler_config);
    return $this;
  }

  public function entityType() {
    return $this->handler->entityType();
  }

  public function requiredArguments() {
    return $this->handler->requiredArguments();
  }

  public function setArguments($arguments = array()) {
    $this->handler->setArguments($arguments);
    return $this;
  }

  public function getArguments() {
    return $this->handler->getArguments();
  }

  public function setParameters($parameters = array()) {
    $this->handler->setParameter($parameters);
    return $this;
  }

  public function getParameters() {
    return $this->handler->getParameters();
  }

  public function setRange($offset = NULL, $limit = NULL) {
    $this->handler->setRange((int)$offset, (int)$limit);
    return $this;
  }

  public function getRange() {
    return $this->handler->getRange();
  }

  public function setPager($limit = NULL, $element = NULL) {
    if (!isset($element)) {
      $element = PagerDefault::$maxElement++;
    }
    elseif ($element >= PagerDefault::$maxElement) {
      PagerDefault::$maxElement = $element + 1;
    }

    $this->pager = array(
      'limit' => (int)$limit,
      'element' => (int)$element,
    );
    return $this;
  }

  function getPager() {
    return $this->pager;
  }

  function initializePager() {
    if (!$this->pagerInitialized) {
      $pager = $this->getPager();
      if (!empty($pager['limit'])) {
        $count = $this->count();
        if ($count) {
          $page = pager_find_page($pager['element']);
          $pager['total'] = $count;
          $pager['offset'] = $page * $pager['limit'];
          pager_default_initialize($pager['total'], $pager['limit'], $pager['element']);
          $this->setRange($pager['offset'], $pager['limit']);
        }
        $this->pagerInitialized = TRUE;
      }
    }
  }

  public function buildQuery() {
    if (!$this->built) {
      $this->handler->buildQuery();
      $this->built = TRUE;
    }
    return $this;
  }

  public function count() {
    $this->buildQuery();
    return $this->handler->count();
  }

  public function execute() {
    if (!$this->executed) {
      $this->buildQuery();
      $this->initializePager();

      // Validate that we have all arguments that we need.
      // @todo We should probably find a unifom way of passing around arguments
      // so the handlers.
      $arguments = $this->getArguments();
      $required_count = 0;
      foreach ($this->requiredArguments() as $key => $info) {
        if ($info['required']) {
          $required_count++;
        }
      }
      if (count($arguments) < $required_count) {
        return array();
      }

      $entities = $this->handler->execute();
      $indexed_entities = array();
      if (!empty($entities)) {
        foreach ($entities as $entity) {
          $indexed_entities[] = $entity;
        }
      }
      $this->setEntities($indexed_entities);
    }
    return $this->getEntities();
  }

  public function setEntities($entities) {
    $this->entities = $entities;
    $this->executed = TRUE;
    return $this;
  }

  public function getEntities() {
    if (!$this->executed) {
      $this->execute();
    }
    return $this->entities;
  }

  public function entity($index) {
    $entities = $this->getEntities();
    if (isset($entities[$index])) {
      return $entities[$index];
    }
    else {
      return FALSE;
    }
  }

  public function cacheTags($execute = FALSE) {
    $tags = $this->handler->cacheTags();
    // Always add the list it self as a cache tag.
    // TODO: Add cache tags with the arguments for better granularity.
    $tags['entitylist'][] = $this->name;
    if ($execute) {
      $entities = $this->execute();
      $entity_type = $this->entityType();
      foreach ($entities as $id => $entity) {
        $tags[$entity_type][] = $id;
      }
    }
    return $tags;
  }

  function uri() {
    // We need to initialize the pager in order for the URI to reflect the range
    // given by the pager.
    $this->initializePager();
    $query = array_merge($this->getRange(), $this->getParameters());
    $query = array_merge($query, array('arguments' => $this->getArguments()));
    return 'entitylist/' . check_plain($this->name) . '?' . drupal_http_build_query($query);
  }

  public function adminPath() {
    return $this->handler->adminPath();
  }

  public function handlerConfigForm(&$form_state) {
    return $this->handler->configForm($form_state);
  }
}
