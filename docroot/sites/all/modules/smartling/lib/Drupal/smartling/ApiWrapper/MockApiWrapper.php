<?php

/**
 * @file
 * Contains Drupal\smartling\ApiWrapper\MockApiWrapper.
 */

namespace Drupal\smartling\ApiWrapper;

use Drupal\smartling\ApiWrapperInterface;
use Drupal\smartling\Log\SmartlingLog;
use Drupal\smartling\Settings\SmartlingSettingsHandler;
use SmartlingAPI;

class MockApiWrapper implements ApiWrapperInterface {

  /**
   * @var SmartlingSettingsHandler
   */
  protected $settingsHandler;

  /**
   * @var SmartlingLog
   */
  protected $logger;

  /**
   * @var SmartlingAPI
   */
  protected $api;

  /**
   * @var array
   */
  protected $filesForDownload;

  protected $progresses;

  protected $filesForUpload;

  protected $connectionTests;

  /**
   * Initialize.
   *
   * @param SmartlingSettingsHandler $settings_handler
   * @param SmartlingLog $logger
   */
  public function __construct(SmartlingSettingsHandler $settings_handler, SmartlingLog $logger) {
    $this->settingsHandler = $settings_handler;
    $this->logger = $logger;

    $this->setApi(new SmartlingAPI($settings_handler->getApiUrl(), $settings_handler->getKey(), $settings_handler->getProjectId(), SMARTLING_PRODUCTION_MODE));
  }

  public function addExpectedFileForDownload($file_path) {
    $this->filesForDownload[] = $file_path;
  }

  /**
   * @param int|boolean $progress
   *   0..100 progress value or FALSE for error.
   */
  public function addExpectedProgress($progress) {
    $this->progresses[] = $progress;
  }

  public function addExpectedFileForUpload($file_path) {
    $this->filesForUpload[] = $file_path;
  }

  /**
   * @param boolean $isResponseSuccessful
   */
  public function addConnectionTestResponse($isResponseSuccessful) {
    $this->connectionTests[] = $isResponseSuccessful;
  }

  /**
   * {@inheritdoc}
   */
  public function setApi(SmartlingAPI $api) {
    $this->api = $api;
  }

  public function getLocaleList() {
    $response = '{"response":{"data":{"locales":[{"locale":"zh-CN","name":"Chinese (Simplified)","translated":"??"},{"locale":"nl","name":"Dutch (International)","translated":"Nederlands"},{"locale":"en-GB","name":"English (United Kingdom)","translated":"English (United Kingdom)"},{"locale":"fr-FR","name":"French (France)","translated":"Francais"},{"locale":"de-DE","name":"German (Germany)","translated":"Deutsch"},{"locale":"it-IT","name":"Italian (Italy)","translated":"Italiano"},{"locale":"ja-JP","name":"Japanese","translated":"???"},{"locale":"pl-PL","name":"Polish (Poland)","translated":"Polski"},{"locale":"es","name":"Spanish (International)","translated":"Espanol"},{"locale":"sv-SE","name":"Swedish","translated":"Svenska"},{"locale":"uk-UA","name":"Ukrainian","translated":"Українська"}]},"code":"SUCCESS","messages":[]}}';
    $response = json_decode($response);
    $locales = isset($response->response->data->locales) ? $response->response->data->locales : array();
    $result = array();
    foreach ($locales as $locale) {
      $result[$locale->locale] = "{$locale->name} ({$locale->translated})";
    }

    return $result;
  }

  /**
   * {@inheritdoc}
   */
  public function downloadFile($entity) {
    if (!empty($this->filesForDownload)) {
      $file_path = array_shift($this->filesForDownload);

      if (file_exists($file_path)) {
        return file_get_contents($file_path);
      }
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function getStatus($entity) {
    if (!empty($this->progresses)) {
      $progress = array_shift($this->progresses);

      if ($progress !== FALSE) {
        $entity_data = new \stdClass();
        $entity_data->progress = $progress;

        return array(
          'entity' => $entity,
          'entity_data' => $entity_data,
        );
      }
    }

    return NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function testConnection(array $locales) {
    $result = array();

    if (!empty($this->connectionTests)) {
      $connection_status = array_shift($this->connectionTests);

      if ($connection_status) {
        foreach ($locales as $locale) {
          $result[$locale] = $connection_status;
        }
      }
    }

    return $result;
  }

  /**
   * {@inheritdoc}
   */
  public function uploadFile($file_path, $file_name_unic, $file_type, array $locales) {
    if (!empty($this->filesForUpload)) {
      $file_path = array_shift($this->filesForUpload);

      if (file_exists($file_path)) {
        return SMARTLING_STATUS_EVENT_UPLOAD_TO_SERVICE;
      }
    }

    return SMARTLING_STATUS_EVENT_FAILED_UPLOAD;
  }

}