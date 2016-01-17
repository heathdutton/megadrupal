<?php

/**
 * @file
 * Provides a CollectiveAccess specific FeedsFetcher.
 */

/**
 * Result of FeedsCollectiveAccessFetcher::fetch().
 */
class FeedsCollectiveAccessFetcherResult extends FeedsFetcherResult {
  protected $ca_instance_name;
  protected $fetch_settings;

  /**
   * Constructor.
   */
  public function __construct($ca_instance_name = NULL, $fetch_settings) {
    $this->ca_instance_name = $ca_instance_name;
    $this->fetch_settings = $fetch_settings;
    parent::__construct('');
  }

  /**
   * Overrides FeedsFetcherResult::getRaw();
   */
  public function getRaw() {
    $object_ids = array();
    $ca_instance = collectiveaccess_instance_load($this->ca_instance_name);
    $ca_object_builder = new CollectiveAccessObjectBuilder($ca_instance);

    // Get fetcher settings
    $fetch_mode = $this->fetch_settings['collectiveaccess_feeds_fetch_method'];
    $fetch_num = $this->fetch_settings['collectiveaccess_feeds_fetch_num'];
    $feeds_importer = $this->fetch_settings['collectiveaccess_feeds_feed_importer'];
    $fetch_all_total = $this->fetch_settings['collectiveaccess_feeds_fetch_all_total'];
    $fetch_collection_ids = $this->fetch_settings['collectiveaccess_feeds_fetch_collection_ids'];

    // First, check if there are items in the queue
    $items = collectiveaccess_feeds_get_queued_items($feeds_importer, $fetch_num);
    if ($items) {
      return $items;
    }
    // If not, processing queue is empty. Fetch new information from CollectiveAccess
    else {
      // Decide how to actually fetch the object ids that need to be imported
      switch ($fetch_mode) {
        case "all":
          $allitems = $this->getRawAll($fetch_all_total);
          break;
        case "lastchanged":
          $allitems = $this->getRawLastChanged($ca_object_builder);
          break;
        case "collection":
          $allitems = $this->getRawCollection($ca_object_builder, $fetch_collection_ids);
          break;
      }
      collectiveaccess_feeds_queue_items($feeds_importer, $allitems);
      // Allow modules to react on the queueing of items
      module_invoke_all('collectiveaccess_feeds_queue_add', $feeds_importer, $allitems, $fetch_mode);
      $items = collectiveaccess_feeds_get_queued_items($feeds_importer, $fetch_num);
    }
    return $items;
  }

  /**
   * Get a list of all objects up until $total
   * @TODO: find a better way to implement this
   */
  protected function getRawAll($total) {
    return range(1, $total);
  }

  /**
   * Get last changed object IDs
   */
  protected function getRawLastChanged($ca_object_builder) {
    $last_import_timestamp = variable_get('collectiveaccess_feeds_lastchanged_timestamp', 0);
    $object_ids = $ca_object_builder->getLastChangedObjectIDs($last_import_timestamp);
    variable_set('collectiveaccess_feeds_lastchanged_timestamp', time());
    return $object_ids;
  }

  /**
   * Get a list of object IDs that belong to given collection(s).
   */
  protected function getRawCollection($ca_object_builder, $collection_ids) {
    $ids = array();
    if (strpos($collection_ids, ',') !== FALSE) {
      $ids = explode(',', $collection_ids);
      $ids = array_map('trim', $ids);
    }
    else if (is_numeric($collection_ids)) {
      $ids = array($collection_ids);
    }

    return $ca_object_builder->getObjectIDsByCollection($ids);
  }

  public function get_ca_instance_name() {
    return $this->ca_instance_name;
  }
}

/**
 * Fetches data from CollectiveAccess webservice via CollectiveAccess module
 */
class FeedsCollectiveAccessFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    $collectiveaccess_instance = $source_config['collectiveaccess_instance'];

    $fetch_settings = array(
      "collectiveaccess_feeds_fetch_method" => $source_config['collectiveaccess_feeds_fetch_method'],
      "collectiveaccess_feeds_fetch_num" => $source_config['collectiveaccess_feeds_fetch_num'],
      "collectiveaccess_feeds_fetch_all_total" => $source_config['collectiveaccess_feeds_fetch_all_total'],
      "collectiveaccess_feeds_fetch_collection_ids" => $source_config['collectiveaccess_feeds_fetch_collection_ids'],
      "collectiveaccess_feeds_feed_importer" => $source->id,
    );

    if ($collectiveaccess_instance) {
      return new FeedsCollectiveAccessFetcherResult($collectiveaccess_instance, $fetch_settings);
    }
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'collectiveaccess_instance' => '',
      'collectiveaccess_feeds_fetch_method' => "lastchanged",
      'collectiveaccess_feeds_fetch_num' => 10,
      'collectiveaccess_feeds_fetch_all_total' => 5000,
      'collectiveaccess_feeds_fetch_collection_ids' => '',
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $instances = collectiveaccess_get_instance_list();
    $form = array();
    $form['collectiveaccess_instance'] = array(
      '#type' => 'select',
      '#title' => t('CollectiveAccess instance'),
      '#description' => t('Specify the CollectiveAccess instance to connect to for importing'),
      '#required' => TRUE,
      '#options' => $instances,
      '#default_value' => isset($this->config['collectiveaccess_instance']) ? $this->config['collectiveaccess_instance'] : NULL,
    );

    $form['collectiveaccess_feeds_fetch_method'] = array(
      '#type' => 'select',
      '#title' => t('Fetch method'),
      '#description' => t('Specify what method to use to fetch data from CollectiveAccess. For an initial import, set to "All". For keeping your installation in sync, set to "Last changed".'),
      '#required' => TRUE,
      '#options' => $this->getCollectiveAccessFetchMethods(),
      '#default_value' => isset($this->config['collectiveaccess_feeds_fetch_method']) ? $this->config['collectiveaccess_feeds_fetch_method'] : "lastchanged",
    );

    $form['collectiveaccess_feeds_fetch_num'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of items to fetch and process per import'),
      '#description' => t('Specify the number of items to fetch from the CollectiveAccess installation per feed import.'),
      '#default_value' => isset($this->config['collectiveaccess_feeds_fetch_num']) ? $this->config['collectiveaccess_feeds_fetch_num'] : 10,
    );

    // @TODO: hide the options via Javascript, based on selection made
    $form['collectiveaccess_feeds_fetch_all']['collectiveaccess_feeds_fetch_all_total'] = array(
      '#type' => 'textfield',
      '#title' => t('Fetch method "All": total number of items to fetch'),
      '#description' => t('Specify the total number of items to fetch from the CollectiveAccess installation. This should probably be equal to the number of objects in the database.'),
      '#default_value' => isset($this->config['collectiveaccess_feeds_fetch_all_total']) ? $this->config['collectiveaccess_feeds_fetch_all_total'] : 5000,
    );
    // @TODO: improve this!
    $form['collectiveaccess_feeds_fetch_collection']['collectiveaccess_feeds_fetch_collection_ids'] = array(
      '#type' => 'textfield',
      '#title' => t('Fetch method "Specified collection(s)": ID(s) of collection(s) to retrieve'),
      '#description' => t('Specify the CollectiveAccess ID(s) of the collections you wish to retrieve. Separate multiple IDs with a comma.'),
      '#default_value' => isset($this->config['collectiveaccess_feeds_fetch_collection_ids']) ? $this->config['collectiveaccess_feeds_fetch_collection_ids'] : '',
    );

    return $form;
  }

  protected function getCollectiveAccessFetchMethods() {
    return array("all" => t('All'), "lastchanged" => t('Last changed'), "collection" => t('Specified collection(s)'));
  }

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $config = $this->getConfig();
    $instances = collectiveaccess_get_instance_list();
    $form = array();

    $instance = isset($this->config['collectiveaccess_instance']) ? $this->config['collectiveaccess_instance'] : NULL;
    $fetch_method = isset($this->config['collectiveaccess_feeds_fetch_method']) ? $this->config['collectiveaccess_feeds_fetch_method'] : "lastchanged";
    $fetch_all_num = isset($this->config['collectiveaccess_feeds_fetch_num']) ? $this->config['collectiveaccess_feeds_fetch_num'] : 10;
    $fetch_all_total = isset($this->config['collectiveaccess_feeds_fetch_all_total']) ? $this->config['collectiveaccess_feeds_fetch_all_total'] : 5000;
    $fetch_collection_ids = isset($this->config['collectiveaccess_feeds_fetch_collection_ids']) ? $this->config['collectiveaccess_feeds_fetch_collection_ids'] : '';

    $form['collectiveaccess_instance'] = array(
      '#type' => 'value',
      '#value' => $instance,
    );

    $form['collectiveaccess_instance_markup'] = array(
      '#markup' => t('Import data from @collectiveaccess_instance', array('@collectiveaccess_instance' => $instance)),
    );

    // @TODO
    $form['collectiveaccess_representation_markup'] = array(
      '#markup' => NULL,
    );

    $form['collectiveaccess_feeds_fetch_method'] = array(
      '#type' => 'value',
      '#value' => $fetch_method,
    );

    $methods = $this->getCollectiveAccessFetchMethods();
    $method = $methods[$fetch_method];
    $form['collectiveaccess_feeds_fetch_method_markup'] = array(
      '#markup' => '<br />' . t("Using fetch method: @method", array("@method" => $method)),
    );

    $form['collectiveaccess_feeds_fetch_num'] = array(
      '#type' => 'value',
      '#value' => $fetch_all_num,
    );

    $form['collectiveaccess_feeds_fetch_all_total'] = array(
      '#type' => 'value',
      '#value' => $fetch_all_total,
    );

    $form['collectiveaccess_feeds_fetch_collection_ids'] = array(
      '#type' => 'value',
      '#value' => $fetch_collection_ids,
    );
    return $form;
  }
}