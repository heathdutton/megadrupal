<?php

/**
 * @file
 * Aggregator Datasource Engine classes.
 */

/**
 * Aggregator feed datasource engine.
 */
class DatasourceEngineAggregatorFeed extends DatasourceEngineAbstract {
  
  protected $items = array();
  
  protected $feed = NULL;
  
  public function initImport() {
    $this->feed = db_query('SELECT * FROM {aggregator_feed} WHERE fid = :fid', array(
          ':fid' => $this->input['settings']['fid'],
        ))->fetch(PDO::FETCH_ASSOC);
    $ts = 0;
    if ($this->feed['datasource_timeframe'] > 0) {
      $ts = time() - $this->feed['datasource_timeframe']*3600*24;
    }
    $this->items = db_query('SELECT * FROM {aggregator_item} WHERE fid = :fid AND timestamp > :ts ORDER BY timestamp', array(
          ':fid' => $this->input['settings']['fid'],
          ':ts' => $ts,
        ))->fetchAll(PDO::FETCH_ASSOC);
    return parent::initImport();
  }
  
  protected function getRecord() {
    if (! list(, $item) = each($this->items) ) {
      return FALSE;
    }
    $item['udid_iid'] = array('source' => 'datasource_aggregator_feed_iid', 'id' => $item['iid'], 'branch' => 'main', 'version' => $item['timestamp']);
    $item['udid_guid'] = array('source' => 'datasource_aggregator_feed_guid', 'id' => $item['guid'], 'branch' => 'main', 'version' => $item['timestamp']);
    return $item;
  }
}

