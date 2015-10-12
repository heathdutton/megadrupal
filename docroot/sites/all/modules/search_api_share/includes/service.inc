<?php

/**
 * @file
 * Search service class using Solr server.
 */

class SearchApiShareSolrService extends SearchApiSolrService {

  /**
   * Implements SearchApiServiceInterface::deleteItems().
   *
   * This method has a custom, Solr-specific extension:
   *
   * If $ids is a string other than "all", it is treated as a Solr query. All
   * items matching that Solr query are then deleted. If $index is additionally
   * specified, then only those items also lying on that index will be deleted.
   *
   * It is up to the caller to ensure $ids is a valid query when the method is
   * called in this fashion.
   */
  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) {
    try {
      $this->connect();
      if ($index) {
        $index_id = $index->machine_name;
        $shared_index = variable_get('search_api_share_shared_index');
        if ($index_id == $shared_index) {
          $index_id = search_api_share_node_index_name($shared_index);
        }
        if (is_array($ids)) {
          $solr_ids = array();
          foreach ($ids as $id) {
            $solr_ids[] = $this->createId($index_id, $id);
          }
          $this->solr->deleteByMultipleIds($solr_ids);
        }
        elseif ($ids == 'all') {
          $this->solr->deleteByQuery("index_id:" . $index_id);
          // $q = $ids == 'all' ? '*:*' : $ids;
          // $this->solr->deleteByQuery($q);
        }
        else {
          $this->solr->deleteByQuery("index_id:" . $index_id . ' (' . $ids . ')');
        }
      }
      else {
        $q = $ids == 'all' ? '*:*' : $ids;
        $this->solr->deleteByQuery($q);
      }
      $this->scheduleCommit();
    }
    catch(SearchApiException $e) {
      watchdog_exception('search_api_solr', $e, '%type while deleting items from server @server: !message in %function (line %line of %file).', array('@server' => $this->server->name));
    }
  }
}

class SearchApiShareAcquiaSearchService extends SearchApiAcquiaSearchService {

  /**
   * Implements SearchApiServiceInterface::deleteItems().
   *
   * This method has a custom, Solr-specific extension:
   *
   * If $ids is a string other than "all", it is treated as a Solr query. All
   * items matching that Solr query are then deleted. If $index is additionally
   * specified, then only those items also lying on that index will be deleted.
   *
   * It is up to the caller to ensure $ids is a valid query when the method is
   * called in this fashion.
   */
  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) {
    try {
      $this->connect();
      if ($index) {
        $index_id = $index->machine_name;
        $shared_index = variable_get('search_api_share_shared_index');
        if ($index_id == $shared_index) {
          $index_id = search_api_share_node_index_name($shared_index);
        }
        if (is_array($ids)) {
          $solr_ids = array();
          foreach ($ids as $id) {
            $solr_ids[] = $this->createId($index_id, $id);
          }
          $this->solr->deleteByMultipleIds($solr_ids);
        }
        elseif ($ids == 'all') {
          $this->solr->deleteByQuery("index_id:" . $index_id);
        }
        else {
          $this->solr->deleteByQuery("index_id:" . $index_id . ' (' . $ids . ')');
        }
      }
      else {
        $q = $ids == 'all' ? '*:*' : $ids;
        $this->solr->deleteByQuery($q);
      }
      $this->scheduleCommit();
    }
    catch(SearchApiException $e) {
      watchdog_exception('search_api_solr', $e, '%type while deleting items from server @server: !message in %function (line %line of %file).', array('@server' => $this->server->name));
    }
  }
}
