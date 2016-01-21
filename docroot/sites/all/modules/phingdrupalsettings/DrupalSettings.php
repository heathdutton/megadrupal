<?php

/**
 * @file
 * A Phing task to create a settings file.
 */

require_once "phing/Task.php";

class DrupalSettings extends Task {

  protected $user;
  protected $password;
  protected $host = 'localhost';
  protected $database;
  protected $port = '';
  protected $driver = 'mysql';
  protected $prefix = '';
  protected $propertyName;

  /**
   * Entry point for the task.
   * 
   * @throws BuildException
   */
  public function main() {
    if (empty($this->user) || empty($this->password) || empty($this->database) || empty($this->propertyName) || empty($this->host)) {
      throw new BuildException("You must specify a username, password, database and propertyName");
    }

    $settings = '$databases = ' . var_export($this->getSettings(), 1) . ';';
    $this->project->setProperty($this->propertyName, $settings);
  }

  /**
   * Get the Drupal settings array.
   * 
   * @return array
   *   The settings array.
   */
  protected function getSettings() {
    return array(
      'default' => array(
        'default' => array(
          'database' => $this->database,
          'username' => $this->user,
          'password' => $this->password,
          'host' => $this->host,
          'port' => $this->port,
          'driver' => $this->driver,
          'prefix' => $this->prefix,
        ),
      ),
    );
  }

  /**
   * Set the user.
   *
   * @param string $user
   *   The user name.
   */
  public function setUser($user) {
    $this->user = $user;
  }

  /**
   * Set the user password.
   *
   * @param string $password
   *   The user password.
   */
  public function setPassword($password) {
    $this->password = $password;
  }

  /**
   * Set the database server hostname.
   *
   * @param string $hostname
   *   The database server hostname.
   */
  public function setHost($hostname) {
    $this->host = $hostname;
  }

  /**
   * Set the database name.
   *
   * @param string $database
   *   The database name.
   */
  public function setDatabase($database) {
    $this->database = $database;
  }

  /**
   * Set the database server port.
   *
   * @param int $port
   *   The port number.
   */
  public function setPort($port) {
    $this->port = $port;
  }

  /**
   * Set the database driver.
   *
   * @param string $driver
   *   The database driver.
   */
  public function setDriver($driver) {
    $this->driver = $driver;
  }

  /**
   * Set the database table prefix.
   *
   * @param string $prefix
   *   The database table prefix.
   */
  public function setPrefix($prefix) {
    $this->prefix = $prefix;
  }

  /**
   * This is the property name the setting will be stored in.
   *
   * @param string $propertyName
   *   The property name.
   */
  public function setPropertyName($propertyName) {
    $this->propertyName = $propertyName;
  }

}
