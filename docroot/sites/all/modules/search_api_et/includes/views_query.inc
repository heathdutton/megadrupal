<?php

/**
 * Views query class using a Search API index as the data source.
 */
class SearchApiEtViewsQuery extends SearchApiViewsQuery {

  /**
   * Helper function for adding results to a view in the format expected by the
   * view.
   *
   * Overrides SearchApiViewsQuery::addResults(), as it does not handle
   * multilingual search item IDs (like 'fr_13') properly.
   *
   * @see SearchApiViewsQuery::addResults()
   */
  protected function addResults(array $results, $view) {
    // Start with standard way of adding results to the view.
    parent::addResults($results, $view);

    // For multilingual indexes, update entity IDs in $view->result array
    // and remove language code from them, so that they contain real entity IDs
    // stored as integers.
    $controller = search_api_get_datasource_controller($this->index->item_type);
    if ($controller instanceof SearchApiEtDatasourceController) {
      foreach ($view->result as $delta => $result) {
        if (!is_object($result->entity) && is_scalar($result->entity) && strpos($result->entity, '_') !== FALSE) {
          list($language, $entity_id) = explode('_', $result->entity);
          $view->result[$delta]->entity = (int) $entity_id;
        }
      }
    }
  }

}
