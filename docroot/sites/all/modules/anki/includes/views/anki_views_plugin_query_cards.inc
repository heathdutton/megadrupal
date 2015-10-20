<?php
/**
 * @file
 * Contains anki_views_plugin_query_cards class.
 */

/**
 * Views plugin for querying cards from an Anki collection.
 */
class anki_views_plugin_query_cards extends views_plugin_query {
  /**
   * An array mapping table field names to aliases.
   */
  public $field_aliases = array();

  /**
   * The name of the collection we are querying.
   */
  public $collection = NULL;

  /**
   * Add a field to the query table, possibly with an alias. This will
   * automatically call ensure_table to make sure the required table
   * exists, *unless* $table is unset.
   *
   * @param $table
   *   The table this field is attached to. If NULL, it is assumed this will
   *   be a formula; otherwise, ensure_table is used to make sure the
   *   table exists.
   * @param $field
   *   The name of the field to add. This may be a real field or a formula.
   * @param $alias
   *   The alias to create. If not specified, the alias will be $table_$field
   *   unless $table is NULL. When adding formulae, it is recommended that an
   *   alias be used.
   * @param $params
   *   An array of parameters additional to the field that will control items
   *   such as aggregation functions and DISTINCT.
   *
   * @return $name
   *   The name that this field can be referred to as. Usually this is the alias.
   */
  public function add_field($table, $field, $alias = '', $params = array()) {
    /*
    if ($table !== $this->base_table) {
      throw new Exception("Cannot query fields from different tables with Anki.");
    }
    */
    $alias = $alias ? $alias : $field;
    $this->field_aliases[$field] = $alias;
    return $alias;
  }

  /**
   * Sets the collection we're going to be querying against.
   *
   * @param string $collection
   *   The collection name.
   */
  public function setCollection($collection) {
    if (empty($this->collection)) {
      $this->collection = $collection;
    }
    else {
      throw new Exception(t("Cannot set the collection multiple times."));
    }
  }

  /**
   * Generate a query and a countquery from all of the information supplied
   * to the object.
   *
   * @param $get_count
   *   Provide a countquery if this is true, otherwise provide a normal query.
   */
  function query($get_count = FALSE) {
    $query = array(
      // TODO: collection needs to get set by an argument!
      'collection' => $this->collection,
      'table' => $this->base_table,
      'anki_query' => array(
        'query' => '',
      ),
    );

    // If we want the count, then we don't include limit/offset and we don't
    // preload the cards.
    if (!$get_count) {
      $query['anki_query']['preload'] = TRUE;
      $query['anki_query']['limit'] = $this->limit;
      $query['anki_query']['offset'] = $this->offset;
    }

    return $query;
  }

  /**
   * Let modules modify the query just prior to finalizing it.
   *
   * @param view $view
   *   The view which is executed.
   */
  function alter(&$view) {
    drupal_alter('anki_views_query', $view, $this);
  }

  /**
   * Builds the necessary info to execute the query.
   *
   * @param view $view
   *   The view which is executed.
   */
  function build(&$view) {
    $view->init_pager($view);

    // Let the pager modify the query to add limits.
    $this->pager->query();

    $view->build_info['query'] = $this->query();
    $view->build_info['count_query'] = $this->query(TRUE);
  }

  /**
   * Internal function to execute the Anki query and return results.
   */
  protected function execute_anki_query($query) {
    // If $query['collection'] isn't set, we should return no items.
    if (empty($query['collection'])) {
      return array();
    }

    $collection = anki_server_collection($query['collection']);
    return $collection->findCards($query['anki_query']);
  }

  /**
   * Show an exception from executing a query.
   */
  protected function show_exception($e) {
    vpr('Exception in @human_name[@view_name]: @message', array('@human_name' => $view->human_name, '@view_name' => $view->name, '@message' => $e->getMessage()));
  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *  
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   *    
   * $view->result should contain an array of objects. The array must use a
   * numeric index starting at 0.
   *
   * @param view $view
   *   The view which is executed.
   */
  function execute(&$view) {
    $query = $view->build_info['query'];
    $count_query = $view->build_info['count_query'];

    $start = microtime(TRUE);

    try {
      $view->result = $this->execute_anki_query($query);
    }
    catch (AnkiServerError $e) {
      $view->result = array();
      $this->show_exception($e);
      return;
    }

    // Execute the count query for the pager.
    // NOTE: Most of this code adapted from the OData module.
    if ($this->pager->use_count_query() || !empty($view->get_total_rows)) {
      try {
        $this->pager->total_items = count($this->execute_anki_query($count_query));
      }
      catch (AnkiServerError $e) {
        $this->pager->total_items = count($view->result);
        $this->show_exception($e);
      }

      if (!empty($this->pager->options['offset'])) {
        $this->pager->total_items -= $this->pager->options['offset'];
      }
      $this->pager->update_page_info();

      $view->total_rows = $this->pager->total_items;
    }
    else {
      $view->total_rows = count($view->result);
    }

    $view->execute_time = microtime(TRUE) - $start;
  }
}
