<?php

/**
 * Creates nodequeues in code.
 *
 * @code
 * $my_nodequeue = CINC::init('Nodequeue')->set('format', 'my_date_format');
 * @endcode
 */
class CINC_Nodequeue extends CINC implements CINC_CRUD {

  public static $dependencies = array('nodequeue');
  public $config_machine_name = 'name';
  public $cinc_yaml_prefix = 'nodequeue';

  function __construct() {

    $this->config_filters['name'] = 'set_name';

    $this->config = array(
      'name' => '',
      'title' => '',
      'subqueue_title' => '',
      'size' => 10,
      'link' => '',
      'link_remove' => '',
      'owner' => 'nodequeue',
      'show_in_ui' => 1,
      'show_in_tab' => 1,
      'show_in_links' => 0,
      'reference' => 0,
      'reverse' => 0,
      'i18n' => 0,
      'subqueues' => 1,
      'types' => array(),
      'roles' => array(),
      'count' => 0,
    );

  }

  /**
   * Cleans name.
   */
  public function set_name(&$name) {
    $name = CINC::clean_machine_name($name);
    if (empty($this->config['title'])) {
      $this->set('title', CINC::display_name_from_machine_name($name));
    }
    return TRUE;
  }

  /**
   * Saves date format to the database.
   */
  public function create() {
    $queue = (object) $this->config;
    nodequeue_save($queue);
    if ($this->config['subqueues'] > 0) {
      nodequeue_add_subqueue($queue, $this->config['title']);
    }
    $this->set('qid', $queue->qid);
    return $this;
  }

  /**
   * Tests if node queue type exists.
   */
  public function exists() {
    $q = FALSE;
    if (!empty($this->config['qid'])) {
      $q = (array) nodequeue_load($this->config['qid']);
    }
    elseif (!empty($this->config['name'])) {
      $q = (array) nodequeue_load_queue_by_name($this->config['name']);
    }
    if ($q) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Loads nodequeue from the database.
   */
  public function read() {
    $q = array();
    if (!empty($this->config['qid'])) {
      $q = (array) nodequeue_load($this->config['qid']);
    }
    elseif (!empty($this->config['name'])) {
      $q = (array) nodequeue_load_queue_by_name($this->config['name']);
    }
    if ($q) {
      $this->config = $q;
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    $queues = nodequeue_load_queues(nodequeue_get_all_qids());

    foreach ($queues as $qid => $queue) {
      $list[$queue->name] = array(
        'display' => $queue->title,
      );
    }

    return $list;

  }


  /**
   * Saves date format to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes date format from the database.
   */
  public function delete() {
    if (empty($this->config['qid'])) {
      $this->read();
    }
    nodequeue_delete($this->config['qid']);
    return $this;
  }

}
