<?php
/**
 * @file
 * FTP connection.
 */

/**
 * A translation service connection that send the files over FTP.
 */
class TmgmtXConnectConnectionFTP extends TmgmtXConnectConnectionAbstract {
  /**
   * The FTP connection.
   */
  private $connection;


  /**
   * Make sure that we close open FTP connections.
   */
  public function __destruct() {
    if ($this->connection) {
      ftp_close($this->connection);
      $this->connection = NULL;
    }
  }

  /**
   * Connect and return the FTP connection resource.
   *
   * @return mixed
   *   A FTP stream resource on success or FALSE on error.
   *
   * @throws TmgmtXConnectConnectionException
   *   - When a connection to the given hostname is not possible.
   *   - when we can't login with the given credentials.
   */
  protected function connect() {
    if (!$this->connection) {
      // Connect to the host.
      $connection = @ftp_connect($this->getHostname());
      if (!$connection) {
        throw new TmgmtXConnectConnectionException(
          sprintf('Can\'t connect to host "%s"', $this->getHostname())
        );
      }

      // Login to host.
      $result = ftp_login($connection, $this->getUsername(), $this->getPassword());
      if (!$result) {
        throw new TmgmtXConnectConnectionException(
          sprintf('Can\t connect with user "%s"', $this->getUsername())
        );
      }

      // Set connection to passive mode.
      ftp_pasv($connection, TRUE);

      $this->connection = $connection;
    }

    return $this->connection;
  }

  /**
   * {@inheritdoc}
   */
  public function send(TmgmtXConnectRequestFile $file) {
    $connection = $this->connect();
    $result = ftp_put(
      $connection,
      $this->filePathRequest(basename($file->getPath())),
      $file->getPath(),
      FTP_BINARY
    );

    return $result;
  }

  /**
   * {@inheritdoc}
   */
  public function scan() {
    $connection = $this->connect();
    $files = ftp_nlist($connection, $this->getPathReceive());

    // We only want files, no directories.
    foreach ($files as $key => $file) {
      if (ftp_size($connection, $this->getPathReceive() . '/' . $file) < 0) {
        unset($files[$key]);
        continue;
      }
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
    $connection = $this->connect();
    $local_file = $this->filePathReceiveLocal($file_name, $local_directory);

    $result = ftp_get(
      $connection,
      $local_file,
      $this->filePathReceive($file_name),
      FTP_BINARY
    );

    if (!$result) {
      throw new TmgmtXConnectConnectionReceiveException(
        sprintf('File could not be downloaded: "%s"', $file_name)
      );
    }

    return new TmgmtXConnectResponse($local_file);
  }

  /**
   * {@inheritdoc}
   */
  public function processed($file_name) {
    $connection = $this->connect();
    $result = ftp_rename(
      $connection,
      $this->filePathReceive($file_name),
      $this->filePathReceiveProcessed($file_name)
    );

    return $result;
  }

}
