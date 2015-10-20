<?php

class EntityListHandlerSQL implements EntityListHandler {

  protected $config = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => NULL, 'limit' => NULL);

  protected $subhandler = NULL;

  protected $query = NULL;

  public function __construct(Array $config = array()) {
    $defaults = array(
      'class_name' => '',
    );
    $this->config = array_merge($defaults, $config);
    $class_name = $this->config['class_name'];
    $this->subhandler = new $class_name($this->config);
  }

  public function entityType() {
    return $this->subhandler->entityType();
  }

  public function requiredArguments() {
    return $this->subhandler->requiredArguments();
  }

  public function setArguments(Array $arguments = array()) {
    $this->arguments = $arguments;
    return $this;
  }

  public function getArguments() {
    return $this->arguments;
  }

  public function setParameters(Array $parameters = array()) {
    $this->parameters = $parameters;
    return $this;
  }

  public function getParameters() {
    return $this->parameters;
  }

  public function setRange($offset = NULL, $limit = NULL) {
    $this->range = array(
      'offset' => $offset,
      'limit' => $limit,
    );
    return $this;
  }

  public function getRange() {
    return $this->range;
  }

  public function buildQuery() {
    if (!$this->query) {
      $arguments = $this->getArguments();
      $this->query = $this->subhandler->buildQuery($arguments);
    }
  }

  public function count() {
    return FALSE;
  }

  public function execute() {
    // Set the range.
    $range = $this->getRange();
    if (!is_null($range['offset']) && !is_null($range['limit'])) {
      $this->query->range($range['offset'], $range['limit']);
    }
    // Execute the query.
    $result = $this->query->execute();

    // Fetch some important entity info.
    $entity_info = entity_get_info($this->subhandler->entityType());
    $id_key = $entity_info['entity keys']['id'];
    // Build the result array of entities.
    $entities = array();
    foreach ($result as $row) {
      $entity = entity_load($this->subhandler->entityType(), array($row->{$id_key}));
      $entities[] = reset($entity);
    }
    return $entities;
  }

  public function cacheTags() {
    return array();
  }

  public function adminPath() {
    return FALSE;
  }

  public function configForm(&$form_state) {
    $form['class_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of query class'),
      '#description' => t('Enter the name of the query class that represents this list. The class has to be registered in the auto-loading registry and implement the <em>EntityListSQL</em> interface.'),
      '#default_value' => $this->config['class_name'],
      '#required' => TRUE,
    );
    return $form;
  }
}

interface EntityListSQL {

  public function __construct(Array $config);

  public function buildQuery(Array $arguments = array());

  static function entityType();

  static function requiredArguments();
}
