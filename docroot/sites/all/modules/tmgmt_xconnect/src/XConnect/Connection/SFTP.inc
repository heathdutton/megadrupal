<?php
/**
 * @file
 * FTP connection.
 */

/**
 * A translation service connection that send the files over FTP.
 */
class TmgmtXConnectConnectionSFTP extends TmgmtXConnectConnectionAbstract {
  /**
   * The SSH connection.
   *
   * @var Net_SFTP
   */
  private $connection;


  /**
   * Make sure that we close open FTP connections.
   */
  public function __destruct() {
    if ($this->connection) {
      $this->connection->disconnect();
      $this->connection = NULL;
    }
  }

  /**
   * Connect and return the FTP connection resource.
   *
   * @return Net_SFTP
   *   The SFTP connection object.
   *
   * @throws TmgmtXConnectConnectionException
   *   - When a connection to the given hostname is not possible.
   *   - when we can't login with the given credentials.
   */
  protected function connect() {
    if (!$this->connection) {
      // Connect to hostname.
      $this->connection = new Net_SFTP($this->getHostname());
      if (!$this->connection) {
        throw new TmgmtXConnectConnectionException(
          sprintf('Can\'t connect to host "%s"', $this->getHostname())
        );
      }

      // Login to host.
      $result = $this->connection->login(
        $this->getUsername(),
        $this->getPassword()
      );
      if (!$result) {
        throw new TmgmtXConnectConnectionException(
          sprintf('Can\t connect with user "%s"', $this->getUsername())
        );
      }
    }

    return $this->connection;
  }

  /**
   * {@inheritdoc}
   */
  public function send(TmgmtXConnectRequestFile $file) {
    $result = $this->connect()->put(
      $this->filePathRequest(basename($file->getPath())),
      $file->getPath(),
      NET_SFTP_LOCAL_FILE
    );

    return $result;
  }

  /**
   * {@inheritdoc}
   */
  public function scan() {
    $files = array();

    $rawfiles = $this->connect()->rawlist($this->getPathReceive());
    if (empty($rawfiles)) {
      return $files;
    }

    // We only want files, no directories.
    foreach ($rawfiles as $rawfile) {
      if ($rawfile['type'] !== 1) {
        continue;
      }

      $files[] = $rawfile['filename'];
    }

    return $files;
  }

  /**
   * {@inheritdoc}
   *
   * @throws TmgmtXConnectConnectionReceiveException
   *   When the file can not be downloaded.
   */
  public function receive($file_name, $local_directory) {
    $local_file = $this->filePathReceiveLocal($file_name, $local_directory);

    $result = $this->connect()->get(
      $this->filePathReceive($file_name),
      $local_file
    );

    if (!$result) {
      throw new TmgmtXConnectConnectionReceiveException(
        sprintf('File could not be downloaded: "%s"', $file_name)
      );
    }

    new TmgmtXConnectResponse($local_file);
  }

  /**
   * {@inheritdoc}
   */
  public function processed($file_name) {
    $result = $this->connect()->rename(
      $this->filePathReceive($file_name),
      $this->filePathReceiveProcessed($file_name)
    );

    return $result;
  }

}
