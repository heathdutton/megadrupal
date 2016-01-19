<?php
class RestWSSearchApiResource implements RestWSQueryResourceControllerInterface {
  protected $indexName, $index, $query, $entity_type, $results;

  public function __construct($name, $info) {
    $this->indexName = $name;
    $this->index = search_api_index_load($name);
    $this->entity_type = $this->index->item_type;
  }

  /**
   * Implements the propertyInfo method
   * 
   * @see RestWSResourceControllerInterface::propertyInfo()
   * 
   * @return array
   */
  public function propertyInfo() {
    $fields = array();
    
    // Add the query field for full text searching
    $fields['query'] = array('type' => '');
    $fields['f'] = array('type' => '');
    
    return $fields;
  }

  /**
   * Implements the wrapper method
   * 
   * @see RestWSResourceControllerInterface::wrapper()
   * 
   * @return EntityDrupalWrapper
   */
  public function wrapper($id) {
    return entity_metadata_wrapper($this->entity_type, $id);
  }

  /**
   * Implements the read method
   * 
   * @see RestWSResourceControllerInterface::read()
   * 
   * @return Entity
   */
  public function read($id) {
    return $this->wrapper($id)->value();
  }

  /**
   * Implements the create method
   * 
   * Throws a 405 error since this controller can't create
   * 
   * @see RestWSResourceControllerInterface::create()
   */
  public function create(array $values) {
    throw new RestWSException('Method not allowed', 405);
  }

  /**
   * Implements the update method
   * 
   * Throws a 405 error since this controller can't update
   * 
   * @see RestWSResourceControllerInterface::update()
   */
  public function update($id, array $values) {
    throw new RestWSException('Method not allowed', 405);
  }

  /**
   * Implements the delete method
   * 
   * Throws a 405 error since this controller can't delete
   * 
   * @see RestWSResourceControllerInterface::delete()
   */
  public function delete($id) {
    throw new RestWSException('Method not allowed', 405);
  }
  
  /**
   * Build the query from the provided filters
   * 
   * @param array $filters
   * 
   * @return SearchApiQueryInterface
   */
  protected function buildQuery($filters) {
    if (NULL == $this->query) {
      $this->query = $this->index->query(array(
        'search_api_spellcheck' => TRUE,
      ));
      
      if (TRUE === isset($filters['query'])) {
        $this->query->keys($filters['query']);
        unset($filters['query']);
      }
    }
    
    return $this->query;
  }

  /**
   * Implements RestWSQueryResourceControllerInterface::query().
   */
  public function query($filters = array(), $meta_controls = array()) {
    $limit = variable_get('restws_query_max_limit', 100);
    $offset = 0;
    
    $rest_controls = restws_meta_controls();
    foreach ($meta_controls as $control_name => $value) {
      switch ($control_name) {
        case $rest_controls['sort']:
          $direction = isset($meta_controls[$rest_controls['direction']]) ? $meta_controls[$rest_controls['direction']] : 'ASC';
          $this->query->sort($value, $direction);
          break;

        case $rest_controls['limit']:
          $limit = $this->limit($value);
          break;

        case $rest_controls['page']:
          $offset = $value > 0 ? $value : $offset;
          break;
      }
    }
    
    // Calculate the offset.
    $offset *= $limit;
    
    $this->buildQuery($filters);
    
    $results = $this->query
    ->range($offset, $limit)
    ->execute();
    
    $entities = array_keys($results['results']);
    
    return $entities;
  }


  /**
   * Implements RestWSQueryResourceControllerInterface::count().
   */
  public function count($filters = array()) {
    $this->buildQuery($filters);
    $results = $this->query->range(0, 1)->execute();
    
    return $results['result count'];
  }

  /**
   * Implements RestWSQueryResourceControllerInterface::limit().
   */
  public function limit($client_limit = NULL) {
    $limit = variable_get('restws_query_max_limit', 100);
    // Only allow user provided limits smaller than the system hard limit.
    if (!empty($client_limit) && $client_limit < $limit) {
      return $client_limit;
    }
    else {
      return $limit;
    }
  }

  /**
   * Implements access method
   * 
   * @see RestWSResourceControllerInterface::access()
   * 
   * @return boolean
   */
  public function access($op, $id) {
    return entity_access($op, $this->entity_type, isset($id) ? $this->wrapper($id)->value() : NULL);
  }

  /**
   * Implements resource method
   * 
   * @see RestWSResourceControllerInterface::resource()
   * 
   * @return string
   */
  public function resource() {
    return $this->indexName;
  }
}