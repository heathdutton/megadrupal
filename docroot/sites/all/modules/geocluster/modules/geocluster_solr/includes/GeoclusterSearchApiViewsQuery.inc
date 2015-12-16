<?php

/**
 * Search service class using Solr server.
 */
class GeoclusterSearchApiViewsQuery extends SearchApiViewsQuery {

  public function init($base_table, $base_field, $options) {
    parent::init($base_table, $base_field, $options);
  }

  public function build(&$view) {
    parent::build($view);

    if ($geocluster = geocluster_get_algorithm($view)) {
      $this->query->setOption("geocluster", $geocluster);
    }
  }

  /**
   * Helper function for adding results to a view in the format expected by the
   * view.
   */
  protected function addResults(array $results, $view) {
    if ($geocluster = geocluster_get_algorithm($view)) {
      $rows = array();
      $missing = array();
      $items = array();
      $ids = array();

      $handler = $geocluster->getFieldHandler();
      $this->fields[$handler->real_field] = TRUE;

      // First off, we try to gather as much field values as possible without
      // loading any items.
      foreach ($results as $id => $result) {
        $row = array();

        //$this->prepareEntityRow($row, $result, $id, $missing, $items, $ids);

        if (isset($result['geocluster_count'])) {
          $row = $result;
        }
        else {
          $this->prepareEntityRow($row, $result, $id, $missing, $items, $ids);
        }

        // Save the row values for adding them to the Views result afterwards.
        $rows[$id] = (object) $row;
      }

      // Load items of those rows which haven't got all field values, yet.
      if (!empty($ids)) {
        $items += $this->index->loadItems($ids);
        // $items now includes loaded items, and those already passed in the
        // search results.
        foreach ($items as $id => $item) {
          // Extract item properties.
          $wrapper = $this->index->entityWrapper($item, FALSE);
          $rows[$id]->_entity_properties += $this->extractFields($wrapper, $missing[$id]);
          $rows[$id]->entity = $item;


          $rows[$id]->geocluster_count = 1;
          $location = $wrapper->{$handler->real_field}->value();
          $rows[$id]->geocluster_lat = $location['lat'];
          $rows[$id]->geocluster_lon = $location['lon'];
          $rows[$id]->geocluster_ids = $id;
        }
      }

      // Finally, add all rows to the Views result set.
      $view->result = array_values($rows);
    }
  }

  protected function prepareEntityRow(&$row, $result, $id, &$missing, &$items, &$ids) {
    // Include the loaded item for this result row, if present, or the item
    // ID.
    if (!empty($result['entity'])) {
      $row['entity'] = $result['entity'];
    }
    else {
      $row['entity'] = $id;
    }

    $row['_entity_properties']['search_api_relevance'] = $result['score'];
    $row['_entity_properties']['search_api_excerpt'] = empty($result['excerpt']) ? '' : $result['excerpt'];

    // Gather any fields from the search results.
    if (!empty($result['fields'])) {
      $row['_entity_properties'] += $result['fields'];
    }

    // Check whether we need to extract any properties from the result item.
    $missing_fields = array_diff_key($this->fields, $row);
    if ($missing_fields) {
      $missing[$id] = $missing_fields;
      if (is_object($row['entity'])) {
        $items[$id] = $row['entity'];
      }
      else {
        $ids[] = $id;
      }
    }
  }

}
