<?php
/**
 * The Connection class
 */
namespace Ming\Core;

use Mongo;
use MongoDB;
use Exception;

class Connection {

  /**
   * The Mongo connection
   *
   * Because this is a public object, we can call all the usual Mongo methods on
   * it, for example Connection::connection->listDBs
   *
   * @var Mongo
   */
  public $connection;

  /**
   * The Db
   *
   * @var MongoDB
   */
  public $db;

  /**
   * Constructor
   *
   * @param $settings
   */
  public function __construct($settings) {
    $this->connect($settings);
    return $this;
  }

  /**
   * Connect to connection
   *
   * @todo Add Replica Set and Socket support
   * @todo Should we support wildcard/random persistent connections?
   *
   * @param $settings
   *  An array of database settings
   * @param array $options
   *  Other options to use for the connection (e.g. Replica Sets et al)
   *
   * @return \Ming\Core\Connection A PHP Mongo class
   */
  public function connect($settings, $options = array()) {

    // Options
    $default_options = array("persist" => "ming_" . $settings['connection_key']);
    $options = $default_options + $options;

    // Default to localhost if not provided
    if (!isset($settings['mongo_host'])) {
      $settings['mongo_host'] = 'localhost';
    }

    // Build a connection string
    $c_string = '';
    if (isset($settings['mongo_user']) && !empty($settings['mongo_user'])) {
      $c_string = "${settings['mongo_user']}:${$settings['mongo_pass']}@";
    }
    $c_string .= $c_string . $settings['mongo_host'];
    if (isset($settings['mongo_port'])) {
      $c_string .= $c_string . ':' . $settings['mongo_port'];
    }

    $server = 'mongodb://' . $c_string;

    // Fire!
    try {

      $this->connection = new Mongo($server, $options);
    }
    catch (Exception $e) {

      drupal_set_message('Could not connect to the Mongo database ' . $settings['mongo_host'] . '. Please check your connection settings.', 'error');
    }

    return $this;
  }

  /**
   * Use a database from the selected connection
   *
   * @param $db_name
   *
   * @return \MongoDB
   */
  public function useDB($db_name) {
    if (!isset($this->connection) || empty($this->connection)) {
      ming_error('Attempt to set database on an empty connection. Is MongoDB running?', 'warning');
      return FALSE;
    }

    try {
      $this->db = $this->connection->selectDB($db_name);
    }
    catch(Exception $e) {
      ming_error('Could not select database', 'error');
    }
    return $this->db;
  }
}

