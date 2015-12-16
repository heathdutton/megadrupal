<?php

/**
 * @file
 * MaPS Import request class.
 *
 * This class intends to retrieve data from MaPS System® files and
 * store them into the database.
 */

namespace Drupal\maps_import\Request;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Exception\RequestException;

/**
 * This class is only a convenient wrapper around the getData() method.
 */
class Files extends Request {

  /**
   * Class constructor.
   *
   * @see self::getData().
   */
  public function __construct(Profile $profile, array $args = array(), array $options = array()) {
    $this->profile = $profile;
    $this->request_arguments = $args;
    $this->options = $options;
  }

  /**
   * Get parsed data from MaPS System®.
   *
   * @param $profile
   *   The MaPS Import profile instance.
   * @param $args
   * @param $options
   */
  public static function getData(Profile $profile, array $args = array(), array $options = array()) {
    try {
      $instance = new Files($profile, $args, $options);
      $instance->query();
      return $instance->parse();
    }
    catch(RequestException $e) {
      $e->addContext('$profile', $profile)->addContext('$args', $args);
      $e->watchdog();
      return FALSE;
    }
  }

  /**
   * Perform a request against the Web Service.
   */
  public function query() {
    $this->result = new \stdClass();
    $this->result->data = '';

    // Check if the file exists.
    if (isset($this->request_arguments['noData']) && $this->request_arguments['noData']) {
      $this->result->data = $this->queryNoData();
      return;
    }

    if (isset($this->request_arguments['config']) && $this->request_arguments['config']) {
      $file = $this->getFilePath($this->profile->getConfigurationFile());
      $this->result->data = file_get_contents($file);
    }
    else {
      $this->result->data = $this->queryObjects();

      if (empty($this->result->data)) {
        throw new RequestException('Empty data.');
      }
    }
  }

  private function queryObjects() {
    $file = $this->getFilePath($this->profile->getObjectsFile());

    $xml = new \XMLReader();
    $xml->open($file);

    $start = 0;
    $i = 0;

    $result = array(
      'objects' => '',
      'links' => '',
    );
    while ($xml->read()) {
      if ($i === (int) $this->request_arguments['maxResults']) {
        break;
      }

      if ($xml->nodeType != \XMLReader::ELEMENT) {
        continue;
      }

      if ($xml->name === 'object') {
        if ($start < $this->request_arguments['startIndex']) {
          $start++;
          continue;
        }

        $result['objects'] .= $xml->readOuterXml();
        $i++;
        $xml->next();
      }
      // @todo only process links once per fetch operation...
      elseif ($xml->name === 'link') {
        $result['links'] .= $xml->readOuterXml();
        $xml->next();
      }
    }

    $xml->close();
    return '<main><objects>' . $result['objects'] . '</objects><links>' . $result['links'] . '</links></main>';
  }

  /**
   * Retrieve the noData informations.
   */
  private function queryNoData() {
    $xml = new \XMLReader();

    $file = $this->getFilePath($this->profile->getObjectsFile());
    $xml->open($file);
    while ($xml->read()) {
      if ($xml->nodeType == \XMLReader::ELEMENT && $xml->name === 'info') {
        $noData = $xml->readOuterXml();
      }
    }
    $xml->close();

    // Calculate the number of element in the xml.
    if (isset($noData) && isset($this->request_arguments['startIndex']) && isset($this->request_arguments['maxResults'])) {
      $count = 0;
      $xml->XML($this->queryObjects());
      while ($xml->read()) {
        if ($xml->nodeType != \XMLReader::ELEMENT) {
          continue;
        }

        if ($xml->name === 'object') {
          $count++;
        }
      }
      $xml->close();

      $noData = preg_replace('/<in_file_objects>(.*)<\/in_file_objects>/uis', "<in_file_objects>$count</in_file_objects>", $noData);
    }

    return isset($noData) ? $noData : 0;
  }

  /**
   * Get the file path.
   *
   * @param string
   */
  private function getFilePath($filename) {
    if (!file_exists(drupal_realpath($this->getProfile()->getMediaAccessibility() . '://') . '/maps_suite/' . $filename)) {
      throw new RequestException(t('Objects file %filename does not exists.', array('%filename' => drupal_realpath($this->getProfile()->getMediaAccessibility() . '://') . '/maps_suite/' . $filename)));
    }

    return drupal_realpath($this->getProfile()->getMediaAccessibility() . '://') . '/maps_suite/' . $filename;
  }

}
