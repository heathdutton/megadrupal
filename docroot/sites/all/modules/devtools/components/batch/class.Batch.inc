<?php
namespace devtools\components;

/**
 * Simplifies batch operations.
 *
 * Use:
 *   $batch = new MyClass('table_name', 'flag_column_name');
 *   $batch->init();
 *    
 *  ... and see the process() method's comments blow.
 */
class Batch {
  protected $_table = '';
  protected $_column = '';
  protected $_limit = 10;

  protected $_total = NULL;
  protected $_lowest_id = NULL;
  protected $_operations = array();

  public function __construct($table, $column) {
    $this->_table = $table;
    $this->_column = $column;

    $this->_total     = db_query("SELECT COUNT(nid) FROM " . $this->_table . " WHERE " . $this->_column . " = 0")->fetchField();
    $this->_lowest_id = db_query("SELECT MIN(nid)   FROM " . $this->_table . " WHERE " . $this->_column . " = 0")->fetchField();
  
    $this->_operations[] = array(array($this, 'process'), array($this->_total, $this->_lowest_id));
  }

  /**
   * Sets up & initializes the batch operations.
   * @return [type] [description]
   */
  public function init() {
    if (!is_null($this->_lowest_id) && !is_null($this->_total) && $this->_operations) {
      if ($this->_total) {
        batch_set(array(
          'title' => t('Running batch operations...'),
          'operations' => $this->_operations,
          'finished' => array($this, 'finish'),
        ));
      }
      else {
        drupal_set_message(t('No batch data to process.'));
      }
    }
  }

  /**
   * Handles general iteration setup & returns iteration-specific data.
   * @param  [type] $total     [description]
   * @param  [type] $lowest_id [description]
   * @param  [type] $context   [description]
   * @return [type]            [description]
   */
  public function iterate($total, $lowest_id, &$context) {
    global $user;
    set_time_limit(0);

    if (empty($context['sandbox'])) {
      $context['sandbox']['count'] = 0;
      $context['sandbox']['last_id'] = 0;
    }

    // per-batch limits
    // keep low to avoid memory issues;
    $limit = $this->_limit;

    if ($limit > $total) {
      $limit = $total;
    }

    $min = $context['sandbox']['count'];
    $max = $min + $limit;

    if ($max > $total) {
      $max = $total;
    }

    $context['message'] = t('Processing @min - @max of @total', array(
      '@min' => $min,
      '@max' => $max,
      '@total' => $total
    ));

    if ($context['sandbox']['last_id'] == 0) {
      $context['sandbox']['last_id'] = $lowest_id;
    }

    return array($user, $min, $max, $limit);
  }

  /**
   * Batch iteration handler for processing data.
   * @param  [type] $total     [description]
   * @param  [type] $lowest_id [description]
   * @param  [type] $context   [description]
   * @return [type]            [description]
   */
  public function process($total, $lowest_id, &$context) {
    return;

    // this method should be extended in child classes as follows:

    // 1. get iteration data:
    list($user, $min, $max, $limit) = $this->iterate($total, $lowest_id, $context);

    // 2. query your content to pull in data you wish to process

    // use the $limit and/or $lowest_id variables to get a small set of 
    // non-processed results. You can get new items by restricting your query
    // to only pull in $limit number of records where unique_record_id > $lowest_id,
    // or just $limit results if you use a flag to mark each imported record
    // 
    // if not using a flag column, you'll want to override the constuctor
    $result = db_query($your_query); 
    if (!$result) {
      $context['finished'] = 1;
      return;
    }

    // 3. process the data
    foreach ($result as $row) {
      // if running multiple queries, use transactions
    
      // on success:
        // if you are using the $lowest_id in your query above, you should update
        // that value here, via:
          // $context['sandbox']['lowest_id'] = $row->unique_record_id
    
        // regardless of whether you use $lowest_id or not, increment the
        // number of records processed:
        $context['sandbox']['count']++;
    }

    // 4. update the batch progress
    $this->progress($total, $context);
  }

  /**
   * Calculates & updates progress.
   * @param  [type] $total   [description]
   * @param  [type] $context [description]
   * @return [type]          [description]
   */
  public function progress($total, &$context) {
    $context['finished'] = $context['sandbox']['count'] / $total;
  }

  /**
   * Wraps up the batch operations and notifies user.
   * @param  [type] $success    [description]
   * @param  [type] $results    [description]
   * @param  [type] $operations [description]
   * @return [type]             [description]
   */
  public function finish($success, $results, $operations) {
    if ($success) {
      drupal_set_message(t('Batch operations successfully completed.'));
    }
    else {
      drupal_set_message(t('An error has occurred during the batch process.'));
    }

    drupal_goto('admin');
  }
}


