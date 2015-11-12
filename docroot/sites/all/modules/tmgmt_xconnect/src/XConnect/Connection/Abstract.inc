<?php
/**
 * @file
 * Abstract connection.
 */

/**
 * Abstract implementation of the ConnectionInterface.
 */
abstract class TmgmtXConnectConnectionAbstract implements TmgmtXConnectConnectionInterface {
  /**
   * The hostname to connect to.
   *
   * @var string
   */
  private $hostname;

  /**
   * The user to connect to the FTP server.
   *
   * @var string
   */
  private $username;

  /**
   * The password to connect to the FTP server.
   *
   * @var string
   */
  private $password;

  /**
   * The folder to where the requests need to be stored.
   *
   * @var string
   */
  private $pathRequest;

  /**
   * The folder from where to collect the results.
   *
   * @var string
   */
  private $pathReceive;

  /**
   * The folder where the processed files should be stored.
   *
   * @var string
   */
  private $pathProcessed;


  /**
   * Constructor.
   *
   * @param array $configuration
   *   The configuration for the connection.
   */
  public function __construct(array $configuration) {
    $this->setHostname(
      $this->extractValueFromConfiguration('host', $configuration)
    );
    $this->setUsername(
      $this->extractValueFromConfiguration('user', $configuration)
    );
    $this->setPassword(
      $this->extractValueFromConfiguration('pass', $configuration)
    );
    $this->setPathRequest(
      $this->extractValueFromConfiguration('folder_request', $configuration)
    );
    $this->setPathReceive(
      $this->extractValueFromConfiguration('folder_receive', $configuration)
    );
    $this->setPathProcessed(
      $this->extractValueFromConfiguration('folder_processed', $configuration)
    );
  }

  /**
   * Set the connection hostname.
   *
   * @param string $hostname
   *   The hostname.
   */
  protected function setHostname($hostname) {
    $hostname = preg_replace('#^ftp://#', '', $hostname);
    $hostname = trim($hostname, '/');
    $this->hostname = $hostname;
  }

  /**
   * Get the connection hostname.
   *
   * @return string
   *   The hostname.
   */
  public function getHostname() {
    return $this->hostname;
  }

  /**
   * Set the username to connect to the server.
   *
   * @param string $username
   *   The username.
   */
  protected function setUsername($username) {
    $this->username = $username;
  }

  /**
   * Get the username.
   *
   * @return string
   *   Username.
   */
  public function getUsername() {
    return $this->username;
  }

  /**
   * Set the password to connect to the server.
   *
   * @param string $password
   *   The password.
   */
  protected function setPassword($password) {
    $this->password = $password;
  }

  /**
   * Get the password to connect to the server.
   *
   * @return string
   *   The password.
   */
  public function getPassword() {
    return $this->password;
  }

  /**
   * Set the directory on the server where to upload the requests to.
   *
   * @param string $path
   *   The directory name like path/to/requests.
   */
  protected function setPathRequest($path) {
    $this->pathRequest = trim($path, '/');
  }

  /**
   * Get the directory on the server where to upload the requests to.
   *
   * @return string
   *   The directory name.
   */
  public function getPathRequest() {
    return $this->pathRequest;
  }

  /**
   * Set the directory on the server from where to collect the results.
   *
   * @param string $path
   *   The folder name like path/to/results.
   */
  protected function setPathReceive($path) {
    $this->pathReceive = trim($path, '/');
  }

  /**
   * Get the directory on the server from where to collect the results.
   *
   * @return string
   *   The directory path.
   */
  public function getPathReceive() {
    return $this->pathReceive;
  }

  /**
   * Set the directory on the server for the processed files.
   *
   * @param string $path
   *   The folder name like path/to/results.
   */
  protected function setPathProcessed($path) {
    $this->pathProcessed = trim($path, '/');
  }

  /**
   * Get the directory on the server for the processed files.
   *
   * @return string
   *   The directory path.
   */
  public function getPathProcessed() {
    return $this->pathProcessed;
  }

  /**
   * Helper to extract a variable from the given configuration.
   *
   * @param string $key
   *   The configuration key.
   * @param array $configuration
   *   The configuration array.
   * @param mixed $default
   *   The default value if the configuration is not set.
   *
   * @return mixed
   *   The configuration value.
   */
  protected function extractValueFromConfiguration($key, array $configuration, $default = NULL) {
    if (!isset($configuration[$key])) {
      return $default;
    }

    return $configuration[$key];
  }

  /**
   * Helper to create the remote request file path.
   *
   * @param string $file_name
   *   The name of the file to upload.
   *
   * @return string
   *   The relative path to upload the file to.
   */
  protected function filePathRequest($file_name) {
    return $this->getPathRequest() . '/' . $file_name;
  }

  /**
   * Helper to create the remote receive file path.
   *
   * @param string $file_name
   *   The name of the file in the receive directory.
   *
   * @return string
   *   The relative path of the file on the remote location.
   */
  protected function filePathReceive($file_name) {
    return $this->getPathReceive() . '/' . $file_name;
  }

  /**
   * Helper to create the local file path to download the receive file to.
   *
   * @param string $file_name
   *   The name of the file in the receive directory.
   * @param string $local_directory
   *   The local directory where the file should be stored.
   *
   * @return string
   *   The path of the file on the local filesystem.
   */
  protected function filePathReceiveLocal($file_name, $local_directory) {
    return rtrim($local_directory, '/') . '/' . $file_name;
  }

  /**
   * Helper to create the remote receive processed file path.
   *
   * @param string $file_name
   *   The name of the file to be stored in the processed directory.
   *
   * @return string
   *   The relative path to store the file on the remote location.
   */
  protected function filePathReceiveProcessed($file_name) {
    return $this->getPathProcessed() . '/' . $file_name;
  }

}
