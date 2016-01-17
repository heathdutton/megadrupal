<?php

class EntityListHandlerSearchApi implements EntityListHandler {

  protected $config = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => 0, 'limit' => NULL);

  protected $query = NULL;

  protected $results = NULL;

  protected $entities = array();

  public function __construct(Array $config = array()) {
    $defaults = array(
      'id' => NULL,
      'sort' => array(
        'field' => '',
        'order' => 'DESC',
      ),
    );
    $this->config = array_merge($defaults, $config);
  }

  public function entityType() {
    // TODO: Load the index and inspect entity type settings.
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
    // This handler doesn't deal with filter options because it's too complex
    // and Facet API is doing a good job providing that context through GET
    // parameters. We will fetch them after the query is executed instead.
    // However, we allow keywords and sort, because that's easy.
    $options = array();
    $arguments = $this->getArguments();

    // We only have one argument, so the first one is the keywords.
    $keys = reset($arguments);
    // Sort config.
    if (!empty($this->config['sort']['field'])) {
      // At this point, we only support sorting on one field at the time. We'd
      // have to implement some AJAX magic in the config form to support
      // multiple fields.
      $options['sort'] = array(
        $this->config['sort']['field'] => $this->config['sort']['order'],
      );
    }
    // Build the base query.
    $this->query = search_api_query($this->config['id'], $options)->keys($keys);
  }

  public function count() {
    // Execute the full query and fetch the count information.
    // Also make the result available so that we don't need to execute in
    // self::execute() aswell.
    $this->results = $this->query->execute();
    if (isset($this->results['result count'])) {
      return $this->results['result count'];
    }
    return 0;
  }

  public function execute() {
    // The query might already have been executed in self::count().
    if ($this->results === NULL) {
      $this->results = $this->query->execute();
    }
    // Fetch the searcher, it's adapter and it's processor. We do this to fetch
    // all parameters that were injected through GET, that we didn't care of
    // in self::buildQuery().
    $searchers = facetapi_get_active_searchers();
    $searcher = reset($searchers);
    $adapter = facetapi_adapter_load($searcher);
    $processor = $adapter->getUrlProcessor();
    $this->setParameters($processor->getParams());

    $ids = array();
    // Range conditions are not done on the query itself since search queries
    // are expected to return information about the total number of results
    // etc. That's why the full query is executed. So offset and limit are
    // done on output here. However, we only load the entites we need.
    $i = 0;
    $ii = 0;
    $range = $this->getRange();
    foreach ($this->results['results'] as $id => $result) {
      if (entitylist_count($i, $ii, $range)) {
        $ids[] = $result['id'];
      }
    }
    return entity_load($this->entityType(), $ids);
  }

  public function cacheTags() {
    return array();
  }

  public function adminPath() {
    if (!user_access('administer search_api')) {
      return FALSE;
    }
    return 'admin/config/search/search_api/index/' . $this->config['id'] . '/edit';
  }

  public function configForm(&$form_state) {
    $form = array();
    $indices = search_api_index_load_multiple(FALSE);

    $options = array();
    foreach ($indices as $id => $index) {
      $options[$id] = check_plain($index->name);
    }
    $form['id'] = array(
      '#type' => 'select',
      '#title' => t('Index'),
      '#description' => t('Choose what index the query should be made against.'),
      '#options' => $options,
      '#default_value' => array($this->config['id']),
      '#required' => TRUE,
    );

    $form['sort'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Sorting'),
    );

    // At this point, we only support sorting on one field at the time. We'd
    // have to implement some AJAX magic here to support multiple fields.
    $options = array('' => t('Search score'));
    if (!empty($this->config['id']) && !empty($indices[$this->config['id']])) {
      foreach ($indices[$this->config['id']]->options['fields'] as $field_name => $info) {
        $options[$field_name] = $field_name;
      }
    }
    $form['sort']['field'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
      '#options' => $options,
      '#description' => t('Configure what field to sort on. Choosing none will sort on the default <em>score</em> field. Sorting on multiple fields is not supported at the moment. <em>Configuring (and saving) the Index setting first might give you more available fields.</em>'),
      '#default_value' => $this->config['sort']['field'],
    );
    $form['sort']['order'] = array(
      '#type' => 'select',
      '#title' => t('Order'),
      '#options' => array(
        'ASC' => t('Ascending'),
        'DESC' => t('Descending'),
      ),
      '#default_value' => $this->config['sort']['order'],
    );
    return $form;
  }
}
