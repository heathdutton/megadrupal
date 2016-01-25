<?php

/**
 * @file
 * MaPS Import request class.
 *
 * This class intends to retrieve data from MaPS System® web services and
 * store them into the database.
 */

namespace Drupal\maps_import\Request;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Exception\WebServiceException;

/**
 * This class is only a convenient xrapper around the getData() method.
 */
class Webservice extends Request {


  /**
   * Class constructor.
   *
   * @see self::getData().
   */
  public function __construct(Profile $profile, array $args, array $options) {
    $this->profile = $profile;
    $this->request_arguments = $args + array(
        'privateToken' => $profile->getToken(),
        'publicationId' => $profile->getPublicationId(),
        'rootObjectId' => $profile->getRootObjectId(),
        'templates' => $profile->getWebTemplate(),
      );

    // Check if we have to empty the webservice cache.
    $emptyCache = $profile->getOptionsItem('force_empty_cache');
    if ($emptyCache === FALSE || $emptyCache === 1) {
      $this->request_arguments['emptyCache'] = 1;
    }

    if (!array_key_exists('presetsGroup', $args) && $presetGroupId = $profile->getPresetGroupId()) {
      $this->request_arguments['presetsGroup'] = $presetGroupId;
    }

    $this->options = $options + array(
      'method' => 'POST',
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded; charset=utf-8'),
      'data' => drupal_http_build_query($this->request_arguments),
    );
  }

  /**
   * Get parsed data from MaPS System®.
   *
   * @param $profile
   *   The MaPS Import profile instance.
   * @param $args
   *   Extra arguments: an associative array of arguments to pass to the
   *   Web Service. Base arguments are already provided by the profile
   *   stored information.
   * @param $options
   *   (optional) An array of options to pass along to drupal_http_request().
   */
  public static function getData(Profile $profile, array $args = array(), array $options = array()) {
    try {
      $instance = new Webservice($profile, $args, $options);
      $instance->query();
      return $instance->parse();
    }
    catch(WebserviceException $e) {
      $e->addContext('$profile', $profile)->addContext('$args', $args);
      $e->watchdog();
      return FALSE;
    }
  }

  /**
   * Perform a request against the Web Service.
   */
  public function query() {
    $this->result = drupal_http_request($this->getProfile()->getUrl(), $this->options);

    if ($this->result->code != 200 || isset($this->result->error)) {
      throw new WebserviceException('Target URL returned an error: !error.', 0, array('!error' => highlight_string('<?php ' . var_export($this->result, TRUE), TRUE)));
    }
  }

}
