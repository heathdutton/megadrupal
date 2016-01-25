<?php
/**
 * @file
 * Interface for translation service connection.
 */

/**
 * Interface to abstract the connection to the translation service.
 */
interface TmgmtXConnectConnectionInterface {
  /**
   * Send a request to the translation service.
   *
   * @param TmgmtXConnectRequestFile $file
   *   The file to send to the service.
   *
   * @return bool
   *   Success.
   */
  public function send(TmgmtXConnectRequestFile $file);

  /**
   * Scan a connection for translations ready to be picked up.
   *
   * @return array
   *   Array of files ready to be picked up.
   */
  public function scan();

  /**
   * Pick up a processed translation.
   *
   * @param string $file_name
   *   The file on the remote location.
   * @param string $local_directory
   *   The local directory to store the downloaded file.
   *
   * @return TmgmtXConnectResponse
   *   The response object containing the file.
   */
  public function receive($file_name, $local_directory);

  /**
   * Let the remote service know that we processed the translation.
   *
   * @param string $file_name
   *   The file that was processsed.
   *
   * @return bool
   *   Success.
   */
  public function processed($file_name);

}
