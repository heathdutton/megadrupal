<?php

class EntityListHandlerEntityReference implements EntityListHandler {

  protected $config = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => NULL, 'limit' => NULL);

  public function __construct(Array $config = array()) {
    $defaults = array(
      'entity_info' => '',
      'field_name' => '',
    );
    $this->config = array_merge($defaults, $config);
  }

  public function entityType() {
    // TODO: Fetch field settings.
    return 'node';
  }

  public function requiredArguments() {
    // TODO: Entity type should be from config form.
    return array(
      'entity:node' => array(
        'label' => 'Node',
        'required' => TRUE,
      ),
    );
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

  }

  public function count() {
    return FALSE;
  }

  public function execute() {
    $arguments = $this->getArguments();
    $entity = $arguments[0]->data;
    $entity_type = $this->entityType();
    $entity_ids = array();
    // TODO: We need to support configurable languages.
    if (isset($entity->{$this->config['field_name']}[LANGUAGE_NONE])) {
      foreach ($entity->{$this->config['field_name']}[LANGUAGE_NONE] as $field) {
        $entity_ids[] = $field['target_id'];
      }
    }
    if ($entity_ids) {
      return entity_load($entity_type, $entity_ids);
    }
  }

  public function cacheTags() {
    $arguments = $this->getArguments();
    $entity = $arguments[0]->data;
    // TODO: Make this more dynamic when the argument is configurable.
    return array($this->entityType() => array($entity->nid));
  }

  public function adminPath() {
    $arguments = $this->getArguments();
    $entity = $arguments[0]->data;

    // TODO: This should follow the entity type configuration.
    if (!node_access('update', $entity)) {
      return FALSE;
    }

    $entity_type = $this->entityType();
    $entity_info = entity_get_info($entity_type);
    $uri = $entity_info['uri callback']($entity);
    // TODO: Find a more solid and generic way of solving this.
    return $uri['path'] . '/edit';
  }

  public function configForm(&$form_state) {
    $form = array();

    $fields = field_info_fields();
    $options = array();
    foreach ($fields as $field_name => $field) {
      if ($field['type'] == 'entityreference') {
        $options[$field_name] = $field_name;
      }
    }
    $form['field_name'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
      '#options' => $options,
      '#default_value' => $this->config['field_name'],
      '#description' => t('Select the field that the list should be assembled from.'),
    );
    return $form;
  }
}
