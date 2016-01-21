<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:12
 */

class ExporterFile {

  protected $filename;

  function __construct($filename) {
    $this->filename = $filename;
    // TODO: do we need to check directories?
    // Prepare & check temp dir
    //$this->temp_directory = file_directory_temp();
    //$this->checkDirectory($this->temp_directory);
    // Prepare & check file dir
    //$this->directory = file_default_scheme() . '://';
    //$this->checkDirectory($this->directory);
  }

  function getFilename() {
    return $this->filename;
  }

  function getFilepath() {
    return file_stream_wrapper_uri_normalize(file_default_scheme() . '://' . $this->filename);
  }

  function getTempFilepath() {
    return file_stream_wrapper_uri_normalize('temporary://' . $this->filename);
  }

  function exists() {
    return file_exists($this->getFilepath());
  }

  /**
   * Safe write function.
   *
   * @see http://php.net/manual/en/function.flock.php#84824
   *
   * @param $data
   * @param string $mode
   * @throws ExporterException
   */
  function write($data, $mode = 'a') {
    $uri = $this->getTempFilepath();
    $handle = fopen($uri, $mode);

    $retries = 0;
    $max_retries = 100;

    if (!$handle) {
      throw new ExporterException("Can't open handle %file.", array('%file' => $uri));
    }

    // Keep trying to get a lock as long as possible
    while (!flock($handle, LOCK_EX)) {
      // Sleep in microseconds.
      usleep(rand(1, 1000));
      ++$retries;

      // Couldn't get the lock, give up.
      if ($retries >= $max_retries) {
        throw new ExporterException("Can't write to locked %file.", array('%file' => $uri));
      }
    }

    // We can write to the file now.
    exporter_log(WATCHDOG_DEBUG, 'Writing to @uri', array('@uri' => $uri));
    if (fwrite($handle, $data) === FALSE) {
      throw new ExporterException("Can't write to %file.", array('%file' => $uri));
    }

    // Force to flush the data, release the lock and close the file.
    fflush($handle);
    flock($handle, LOCK_UN);
    fclose($handle);
  }

  function finish() {
    $source = $this->getTempFilepath();
    $destination = $this->getFilepath();
    exporter_log(WATCHDOG_DEBUG, 'Moving file from %source to %destionation.', array('%source' => $source, '%destination' => $destination));
    if (!file_unmanaged_move($source, $destination, FILE_EXISTS_REPLACE)) {
      throw new ExporterException('File move from  %source to %destionation failed.', array('%source' => $source, '%destination' => $destination));
    }
  }

  protected function checkDirectory($dir) {
    if (!file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
      throw new ExporterException('Directory %dir not found or is not writable', array('%dir' => $dir));
    }
  }

}
