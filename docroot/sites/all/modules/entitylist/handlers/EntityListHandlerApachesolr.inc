<?php

class EntityListHandlerApachesolr implements EntityListHandler {

  protected $config = array();

  protected $searchArguments = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => NULL, 'limit' => NULL);

  public function __construct(Array $config = array()) {
    $defaults = array(
      'env_id' => NULL,
    );
    $this->config = array_merge($defaults, $config);
  }

  protected function searchEnvironment() {
    return apachesolr_get_solr($this->config['env_id']);
  }

  protected function searchArguments() {
    if (empty($this->searchArguments)) {
      $arguments = $this->getArguments();
      $keywords = '';
      if (!empty($arguments)) {
        $keywords = reset($arguments)->data;
      }

      // Search keywords and filters.
      $params = array(
        'q' => $keywords,
        'fq' => isset($_GET['filters']) ? $_GET['filters'] : '',
      );

      // Sort parameter.
      $sort = isset($_GET['solrsort']) ? $_GET['solrsort'] : '';

      // If we are on a page with keyword arguments, use the base path.
      // Otherwise facet links will be based on the wrong base URL.
      // For example: 'search/site/%' becomes 'search/site'
      $menu_item = menu_get_item();
      $path = $menu_item['path'];
      if (strpos($path, '/%')) {
        $path = strstr($path, '/%', TRUE);
      }

      $this->searchArguments = array(
        'apachesolr',
        array_filter($params),
        $sort,
        $path,
        pager_find_page(),
        $this->searchEnvironment(),
      );
    }
    return $this->searchArguments;
  }

  public function entityType() {
    return 'node';
  }

  public function requiredArguments() {
    return array(
      'string' => array(
        'label' => 'Keywords',
        'required' => FALSE,
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
    try {
      $entity_type = $this->entityType();
      $results = call_user_func_array('apachesolr_search_run', $this->searchArguments());
      $ids = array();
      foreach ($results as $result) {
        $ids[] = $result[$entity_type]->entity_id;
      }
      if (!empty($ids)) {
        return entity_load($entity_type, $ids);
      }
    }
    catch (Exception $e) {
      watchdog_exception('entitylist', $e);
    }
    return array();
  }

  public function cacheTags() {
    return array();
  }

  public function adminPath() {
    if (!user_access('administer search')) {
      return FALSE;
    }
    return 'admin/config/search/apachesolr/settings/' . $this->config['env_id'] . '/edit';
  }

  public function configForm(&$form_state) {
    $form = array();
    $environments = apachesolr_load_all_environments();

    $options = array();
    foreach ($environments as $env_id => $environment) {
      $options[$env_id] = check_plain($environment['name']);
    }
    $form['env_id'] = array(
      '#type' => 'select',
      '#title' => t('Environment'),
      '#description' => t('Choose what environment the query should be made against.'),
      '#options' => $options,
      '#default_value' => array($this->config['env_id']),
    );

    return $form;
  }
}
