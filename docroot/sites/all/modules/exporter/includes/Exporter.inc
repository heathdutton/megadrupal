<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:13
 */


abstract class Exporter {

  const IDLE = 0;
  const INITIALIZATION = 1;
  const PROCESSING = 2;
  const FINISHING = 3;

  public $id;
  public $name;
  public $cron;
  public $description;
  public $last_start;
  public $last_finish;
  public $last_duration;
  public $period;
  public $limit;
  public $filename;
  public $use_cache;
  public $log_level;
  public $queue;
  public $cache;

  protected $file;
  protected $config;

  function __construct($id) {
    $this->id = $id;

    $this->info = $this->getInfo();
    $this->name = $this->info['name'];
    $this->description = $this->info['description'];

    // TODO: merge info and config.

    $this->config = new ExporterConfig($id);
    $this->last_start    = $this->config->get('last_start');
    $this->last_finish   = $this->config->get('last_finish');
    $this->last_duration = $this->config->get('last_duration');
    $this->limit         = $this->config->get('limit', 100);
    $this->period        = $this->config->get('period', 3600);
    $this->use_cron      = $this->config->get('use_cron', FALSE);
    $this->use_cache     = $this->config->get('use_cache', FALSE);
    $this->log_level     = $this->config->get('log_level', WATCHDOG_INFO);
    $this->filename      = $this->config->get('filename', $this->id . '_export.xml');

    $this->cache = new ExporterCache($this);
    $this->queue = $this->createQueue();
    $this->file = new ExporterFile($this->filename);
    $this->logger = new ExporterLogger($this->log_level);
  }

  /**
   * Return settings from the hook_exporter_info() array.
   *
   * @return array
   */
  function getInfo() {
    return exporter_get_info($this->id);
  }

  /**
   * Mark the current exporter as processed and render the header.
   */
  function exportStart() {
    try {
      $this->updateAttribute('last_start', REQUEST_TIME);
      $header = $this->renderHeader();
      $this->file->write($header, 'w');
    }
    catch (ExporterException $e) {
      $this->logger->logException($e);
    }
  }

  /**
   * Loop thorugh the items and render them.
   *
   * @param $offset
   * @param $limit
   */
  function exportItems($offset, $limit) {
    try {
      foreach ($this->getItemIds($offset, $limit) as $id) {
        $rendered_item = $this->use_cache ? $this->exportItemCached($id) : $this->exportItem($id);
        $this->file->write($rendered_item);
      }
    }
    catch (ExporterException $e) {
      $this->logger->logException($e);
    }
  }

  /**
   * Cached version of the renderItem() method.
   *
   * @param $item
   * @return string
   */
  function exportItemCached($id) {
    $rendered_item = $this->cache->get($id);
    if (!$rendered_item) {
      $item = $this->loadItem($id);
      $rendered_item = $this->renderItem($item);
      $this->cache->set($id, $rendered_item);
    }
    return $rendered_item;
  }

  /**
   * Cached version of the renderItem() method.
   *
   * @param $item
   * @return string
   */
  function exportItem($id) {
    $item = $this->loadItem($id);
    return $this->renderItem($item);
  }

  /**
   * Render footer, finish the export and move the file to the final destination.
   */
  function exportFinish() {
    try {
      $footer = $this->renderFooter();
      $this->file->write($footer);
      $this->file->finish();
      $this->updateAttribute('last_finish', REQUEST_TIME);
      $this->updateAttribute('last_duration', REQUEST_TIME - $this->last_start);
    }
    catch (ExporterException $e) {
      $this->logger->logException($e);
    }
  }

  /**
   * Returns true if file exists.
   *
   * @returns boolean
   */
  function fileExists() {
    return $this->file->exists();
  }

  /**
   * Returns path to the exported file.
   *
   * @return string
   */
  function getFilePath() {
    return $this->file->getFilepath();
  }

  /**
   * Load item object.
   *
   * @param $id
   * @return object
   */
  abstract function loadItem($id);

  /**
   * Return unique item key such as item ID.
   *
   * @param $item
   * @return string
   */
  abstract function getItemId($item);

  /**
   * Return total count of items to be exported.
   *
   * @return int
   */
  abstract function getCount();

  /**
   * Return items in the given offset and limit.
   *
   * @param $offset
   * @param $limit
   * @return array
   */
  abstract function getItemIds($offset, $limit);

  /**
   * Returns rendered header such as XML header and opening tag or header line in the CSV file.
   *
   * @return string
   */
  abstract function renderHeader();

  /**
   * Render item.
   *
   * @param $item
   * @return string
   */
  abstract function renderItem($item);

  /**
   * Render footer of the file such as XML document closing tag.
   *
   * @return mixed
   */
  abstract function renderFooter();

  /**
   * @return ExporterQueue
   */
  protected function createQueue() {
    $queue = DrupalQueue::get('exporter_' . $this->id);
    $queue->createQueue();
    return $queue;
  }

  protected function updateAttribute($attribute, $value) {
    $this->{$attribute} = $value;
    $this->config->set($attribute, $value);
  }

}
