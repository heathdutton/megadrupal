<?php

/**
 * @file
 * Smartling settings handler.
 */

namespace Drupal\smartling\ApiWrapper;

use Drupal\smartling\ApiWrapperInterface;
use Drupal\smartling\Log\SmartlingLog;
use Drupal\smartling\Settings\SmartlingSettingsHandler;
use SmartlingAPI;

/**
 * Class SmartlingApiWrapper.
 */
class SmartlingApiWrapper implements ApiWrapperInterface {

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
   * This function converts Drupal locale to Smartling locale.
   *
   * @param string $locale
   *   Locale string in some format: 'en' or 'en-US'.
   *
   * @return string|null
   *   Return locale or NULL.
   */
  protected function convertLocaleDrupalToSmartling($locale) {
    return smartling_convert_locale_drupal_to_smartling($locale);
  }

  /**
   * This function converts Smartling locale to Drupal locale.
   *
   * @param string $locale
   *   Locale string in some format: 'en' or 'en-US'.
   *
   * @return string|null
   *   Return locale or NULL.
   */
  protected function convertLocaleSmartlingToDrupal($locale) {
    return smartling_convert_locale_smartling_to_drupal($locale);
  }
  /**
   * Initialize.
   *
   * @param SmartlingSettingsHandler $settings_handler
   * @param SmartlingLog $logger
   */
  public function __construct(SmartlingSettingsHandler $settings_handler, SmartlingLog $logger) {
    $this->settingsHandler = $settings_handler;
    $this->logger = $logger;

    $this->setApi(new \SmartlingAPI($settings_handler->getApiUrl(), $settings_handler->getKey(), $settings_handler->getProjectId(), SMARTLING_PRODUCTION_MODE));
  }

  /**
   * {@inheritdoc}
   */
  public function setApi(\SmartlingAPI $api) {
    $this->api = $api;
  }

  public function getLocaleList() {
    $response = $this->api->getLocaleList();
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
  public function getAuthorizedLocaleList($smartling_entity) {
    $file_name_unic = $smartling_entity->file_name;
    $authorized_locales = $this->api->getAuthorizedLocales($file_name_unic);
    // @todo add logging.

    if (isset($authorized_locales->response->code)) {
      $authorized_locales = json_decode($authorized_locales);

      $code = '';
      $messages = array();
      if (isset($authorized_locales->response)) {
        $code = isset($authorized_locales->response->code) ? $authorized_locales->response->code : array();
        $messages = isset($authorized_locales->response->messages) ? $authorized_locales->response->messages : array();
      }


      $this->logger->error('Authorized locales:<br/>
      Project Id: @project_id <br/>
      Action: download <br/>
      URI: @file_uri <br/>
      Error: response code -> @code and message -> @message',
        array(
          '@project_id' => $this->settingsHandler->getProjectId(),
          '@file_uri' => $file_name_unic,
          '@code' => $code,
          '@message' => implode(' || ', $messages),
        ), TRUE);
      return FALSE;
    }
    $data = json_decode($authorized_locales);

    if (empty($data->response->data->locales)) {
      $this->logger->error('Authorized locales:<br/>
      Project Id: @project_id <br/>
      Action: download <br/>
      URI: @file_uri <br/>
      Error: response code -> @code and message -> @message',
        array(
          '@project_id' => $this->settingsHandler->getProjectId(),
          '@file_uri' => $file_name_unic,
          '@code' => 500,
          '@message' => implode(' || ', 'JSON is invalid'),
        ), TRUE);
      return FALSE;
    }

    return $data->response->data->locales;
  }

  /**
   * {@inheritdoc}
   */
  public function downloadFile($smartling_entity) {
    $smartling_entity_type = $smartling_entity->entity_type;
    $d_locale = $smartling_entity->target_language;
    $file_name_unic = $smartling_entity->file_name;

    $retrieval_type = $this->settingsHandler->variableGet('smartling_retrieval_type', 'published');
    $download_param = array(
      'retrievalType' => $retrieval_type,
    );

    $this->logger->info("Smartling queue start download '@file_name' file and update fields for @entity_type id - @rid, locale - @locale.",
      array(
        '@file_name' => $file_name_unic,
        '@entity_type' => $smartling_entity_type,
        '@rid' => $smartling_entity->rid,
        '@locale' => $smartling_entity->target_language
      ));

    $s_locale = $this->convertLocaleDrupalToSmartling($d_locale);
    // Try to download file.
    $download_result = $this->api->downloadFile($file_name_unic, $s_locale, $download_param);

    if (isset($download_result->response->code)) {
      $download_result = json_decode($download_result);

      $code = '';
      $messages = array();
      if (isset($download_result->response)) {
        $code = isset($download_result->response->code) ? $download_result->response->code : array();
        $messages = isset($download_result->response->messages) ? $download_result->response->messages : array();
      }


      $this->logger->error('smartling_queue_download_translated_item_process try to download file:<br/>
      Project Id: @project_id <br/>
      Action: download <br/>
      URI: @file_uri <br/>
      Drupal Locale: @d_locale <br/>
      Smartling Locale: @s_locale <br/>
      Error: response code -> @code and message -> @message',
        array(
          '@project_id' => $this->settingsHandler->getProjectId(),
          '@file_uri' => $file_name_unic,
          '@d_locale' => $d_locale,
          '@s_locale' => $s_locale,
          '@code' => $code,
          '@message' => implode(' || ', $messages),
        ), TRUE);

      return FALSE;
    }

    return $download_result;
  }


  /**
   * {@inheritdoc}
   */
  public function getStatus($smartling_entity) {
    $error_result = NULL;

    if ($smartling_entity === FALSE) {
      $this->logger->error('Smartling checks status for id - @rid is FAIL! Smartling entity not exist.', array('@rid' => $smartling_entity->rid), TRUE);
      return $error_result;
    }

    $file_name_unic = $smartling_entity->file_name;

    $s_locale = $this->convertLocaleDrupalToSmartling($smartling_entity->target_language);
    // Try to retrieve file status.
    $json = $this->api->getStatus($file_name_unic, $s_locale);
    $status_result = json_decode($json);

    if ($status_result === NULL) {
      $this->logger->error('File status commend: downloaded json is broken. JSON: @json', array('@json' => $json), TRUE);
      return $error_result;
    }

    // This is a get status.
    if (($this->api->getCodeStatus() != 'SUCCESS') || !isset($status_result->response->data)) {
      $code = '';
      $messages = array();
      if (isset($status_result->response)) {
        $code = isset($status_result->response->code) ? $status_result->response->code : array();
        $messages = isset($status_result->response->messages) ? $status_result->response->messages : array();
      }


      $this->logger->error('Smartling checks status for @entity_type id - @rid: <br/>
      Project Id: @project_id <br/>
      Action: status <br/>
      URI: @file_uri <br/>
      Drupal Locale: @d_locale <br/>
      Smartling Locale: @s_locale <br/>
      Error: response code -> @code and message -> @message', array(
        '@entity_type' => $smartling_entity->entity_type,
        '@rid' => $smartling_entity->rid,
        '@project_id' => $this->settingsHandler->getProjectId(),
        '@file_uri' => $file_name_unic,
        '@d_locale' => $smartling_entity->target_language,
        '@s_locale' => $s_locale,
        '@code' => $code,
        '@message' => implode(' || ', $messages),
      ), TRUE);
      return $error_result;
    }

    $this->logger->info('Smartling checks status for @entity_type id - @rid (@d_locale). approvedString = @as, completedString = @cs',
      array(
        '@entity_type' => $smartling_entity->entity_type,
        '@rid' => $smartling_entity->rid,
        '@d_locale' => $smartling_entity->target_language,
        '@as' => $status_result->response->data->approvedStringCount,
        '@cs' => $status_result->response->data->completedStringCount
      ));

    // If true, file translated.
    $response_data = $status_result->response->data;
    $approved = $response_data->approvedStringCount;
    $completed = $response_data->completedStringCount;
    $progress = ($approved > 0) ? (int) (($completed / $approved) * 100) : 0;
    $smartling_entity->download = 0;
    $smartling_entity->progress = $progress;
    $smartling_entity->status = SMARTLING_STATUS_IN_TRANSLATE;

    return array(
      'entity_data' => $smartling_entity,
      'response_data' => $status_result->response->data,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function testConnection(array $locales) {
    $result = array();

    foreach ($locales as $key => $locale) {
      if ($locale !== 0 && $locale == $key) {
        $s_locale = $this->convertLocaleDrupalToSmartling($locale);
        // Init api object.
        $server_response = $this->api->getList($s_locale, array('limit' => 1));

        if ($this->api->getCodeStatus() == 'SUCCESS') {
          $result[$s_locale] = TRUE;
        }
        else {
          $this->logger->warning('Connection test for project: @project_id and locale: @locale FAILED and returned the following result: @server_response.',
            array(
              '@project_id' => $this->settingsHandler->getProjectId(),
              '@locale' => $key,
              '@server_response' => $server_response
            ));
        }
      }
    }

    return $result;
  }

  /**
   * {@inheritdoc}
   */
  // TODO : Replace $file_type with enum class
  public function uploadFile($file_path, $file_name_unic, $file_type, array $locales) {
    $locales_to_approve = array();
    foreach ($locales as $locale) {
      $locales_to_approve[] = $this->convertLocaleDrupalToSmartling($locale);
    }

    $version = system_get_info('module', 'smartling');
    $version = $version['version'];
    $upload_params = new \FileUploadParameterBuilder();
    $upload_params->setFileUri($file_name_unic)
      ->setFileType($file_type)
      ->setApproved(0)
      ->setOverwriteApprovedLocales(0)
      ->setClientLibId("{\"client\":\"drupal-connector\",\"version\":\"$version\"}");

    if ($this->settingsHandler->getAutoAuthorizeContent()) {
      $upload_params->setLocalesToApprove($locales_to_approve);
    }
    if ($this->settingsHandler->getCallbackUrlUse()) {
      $upload_params->setCallbackUrl($this->settingsHandler->getCallbackUrl());
    }
    if ($this->settingsHandler->getOverwriteApprovedLocales()) {
      $upload_params->setOverwriteApprovedLocales(1);
    }
    $upload_params = $upload_params->buildParameters();

    $upload_result = $this->api->uploadFile($file_path, $upload_params);
    $upload_result = json_decode($upload_result);

    if ($this->api->getCodeStatus() == 'SUCCESS') {

      $this->logger->info('Smartling uploaded @file_name for locales: @locales',
        array(
          '@file_name' => $file_name_unic,
          '@locales' => implode('; ', $locales),
          'entity_link' => l(t('View file'), $file_path)
        ));

      return SMARTLING_STATUS_EVENT_UPLOAD_TO_SERVICE;
    }
    elseif (is_object($upload_result)) {
      foreach ($upload_params as $param_name => $value) {
        $upload_params[$param_name] = $param_name . ' => ' . $value;
      }

      $code = '';
      $messages = array();
      if (isset($upload_result->response)) {
        $code = isset($upload_result->response->code) ? $upload_result->response->code : array();
        $messages = isset($upload_result->response->messages) ? $upload_result->response->messages : array();
      }

      $this->logger->error('Smartling failed to upload xml file: <br/>
          Project Id: @project_id <br/>
          Action: upload <br/>
          URI: @file_uri <br/>
          Drupal Locale: @d_locale <br/>
          Smartling Locale: @s_locale <br/>
          Error: response code -> @code and message -> @message
          Upload params: @upload_params',
        array(
          '@project_id' => $this->settingsHandler->getProjectId(),
          '@file_uri' => $file_path,
          '@d_locale' => implode('; ', $locales),
          '@s_locale' => implode('; ', $locales_to_approve),
          '@code' => $code,
          '@message' => implode(' || ', $messages),
          '@upload_params' => implode(' | ', $upload_params),
        ), TRUE);
    }

    return SMARTLING_STATUS_EVENT_FAILED_UPLOAD;
  }

  /*
   * Uploads context file to Smartling and writes some logs
   *
   * @param array $data
   * @return int
   */
  public function uploadContext($data) {
    $data['action'] = 'upload';
    $upload_result = $this->api->uploadContext($data);

    if ($this->api->getCodeStatus() !== 'SUCCESS') {
      //error handling code
      $this->logger->error('Smartling failed to upload context for module @angular_module with message: @message', array(
        '@angular_module' => $data['url'],
        '@message' => $upload_result
      ), TRUE);
      return -1;
    }

    $upload_result = json_decode($upload_result);
    $requestId = $upload_result->response->data->requestId;

    $data = array(
      'requestId' => $requestId,
      'action' => 'getStats'
    );

    $upload_result = $this->api->getContextStats($data);

    if ($this->api->getCodeStatus() !== 'SUCCESS') {
      //error handling code
      $this->logger->error('Smartling uploaded the context, but failed to get context statistics for request: @requestId  with message: @message', array(
        '@requestId' => $requestId,
        '@message' => $upload_result
      ), TRUE);
      return -1;
    }

    $upload_result = json_decode($upload_result);
    $updatedStringsCount = $upload_result->response->data->updatedStringsCount;

    return $updatedStringsCount;
  }

  function deleteFile($file_name) {
    $this->api->deleteFile($file_name);

    if ($this->api->getCodeStatus() !== 'SUCCESS') {
      //error handling code
      $this->logger->error('Smartling failed to delete file: @file_name', array('@file_name' => $file_name), TRUE);
      return -1;
    }

    return 1;
  }

  /**
   * {@inheritdoc}
   */
  public function getStatusAllLocales($file_name) {
    $error_result = NULL;

    if (empty($file_name)) {
      //@todo: improve log message.
      $this->logger->error('Smartling checks status for file - @file_name failed! Because it was empty.', array('@file_name' => $file_name), TRUE);
      return $error_result;
    }

    // Try to retrieve file status.
    $json = $this->api->getStatusAllLocales($file_name);
    $status_result = json_decode($json);

    if ($status_result === NULL) {
      $this->logger->error('File status command: downloaded json is broken. JSON: @json', array('@json' => $json), TRUE);
      return $error_result;
    }

    // This is a get status.
    if (($this->api->getCodeStatus() != 'SUCCESS') || !isset($status_result->response->data)) {
      $code = '';
      $messages = array();
      if (isset($status_result->response)) {
        $code = isset($status_result->response->code) ? $status_result->response->code : array();
        $messages = isset($status_result->response->messages) ? $status_result->response->messages : array();
      }

      $this->logger->error('Smartling checks status for file name: @file_uri <br/>
      Project Id: @project_id <br/>
      Action: status <br/>
      Error: response code -> @code and message -> @message', array(
        '@project_id' => $this->settingsHandler->getProjectId(),
        '@file_uri' => $file_name,
        '@code' => $code,
        '@message' => implode(' || ', $messages),
      ), TRUE);
      return $error_result;
    }

    $this->logger->info('Smartling checks status for file: @file_name. totalString = @as',
      array(
        '@file_name' => $file_name,
        '@as' => $status_result->response->data->totalStringCount,
      ));

    $result = array();
    $items = $status_result->response->data->items;
    foreach ($items as $item) {
      $locale = $this->convertLocaleSmartlingToDrupal($item->localeId);
      if (!empty($locale)) {
        $result[$locale] = (array) $item;
      }
    }

    return $result;
  }

  public function getLastModified($file_name, $locale = '', $params = array()) {
    //return $this->api->getLastModified($fileUri, $locale, $params);

    $error_result = NULL;

    if (empty($file_name)) {
      //@todo: improve log message.
      $this->logger->error('Smartling check of last updated date for file - @file_name failed.', array('@file_name' => $file_name), TRUE);
      return $error_result;
    }

    // Try to retrieve file status.
    $json = $this->api->getLastModified($file_name, $locale, $params);
    $status_result = json_decode($json);

    if ($status_result === NULL) {
      $this->logger->error('File status command: downloaded json is broken. JSON: @json', array('@json' => $json), TRUE);
      return $error_result;
    }

    // This is a get status.
    if (($this->api->getCodeStatus() != 'SUCCESS') || !isset($status_result->response->data)) {
      $code = '';
      $messages = array();
      if (isset($status_result->response)) {
        $code = isset($status_result->response->code) ? $status_result->response->code : array();
        $messages = isset($status_result->response->messages) ? $status_result->response->messages : array();
      }

      $this->logger->error('Smartling check last updated dates for file name: @file_uri <br/>
      Project Id: @project_id <br/>
      Action: status <br/>
      Error: response code -> @code and message -> @message', array(
        '@project_id' => $this->settingsHandler->getProjectId(),
        '@file_uri' => $file_name,
        '@code' => $code,
        '@message' => implode(' || ', $messages),
      ), TRUE);
      return $error_result;
    }

    $result = array();
    $items = $status_result->response->data->items;
    foreach ($items as $item) {
      $locale = $this->convertLocaleSmartlingToDrupal($item->locale);
      if (!empty($locale)) {
        $result[$locale] = strtotime($item->lastModified);
      }
    }

    return $result;
  }
}
