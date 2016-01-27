<?php

class EntityListHandlerEFQ implements EntityListHandler {

  protected $config = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => NULL, 'limit' => NULL);

  public function __construct(Array $config = array()) {
    $defaults = array(
      'class_name' => '',
    );
    $this->config = array_merge($defaults, $config);
  }

  public function entityType() {
    return call_user_func_array(array($this->config['class'], 'entityType'));
  }

  public function requiredArguments() {
    return call_user_func_array(array($this->config['class'], 'requiredArguments'));
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
    $class_name = $this->config['class_name'];
    // TODO: Check that $class_name implements EntityListEFQ.
    $query = new $class_name($this->config, $arguments);
    return $query->execute();
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
      '#description' => t('Enter the name of the EntitFieldQuery class that represents this list. The class has to be registered in the auto-loading registry and implement the <em>EntityListEFQ</em> interface.'),
      '#default_value' => $this->config['class_name'],
    );
    return $form;
  }
}

interface EntityListEFQ {

  public function __construct(Array $config, Array $arguments = array());

  static function entityType();

  static function requiredArguments();
}
