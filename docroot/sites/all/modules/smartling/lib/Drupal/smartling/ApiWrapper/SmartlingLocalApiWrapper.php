<?php

/**
 * @file
 * Smartling settings handler.
 */

namespace Drupal\smartling\ApiWrapper;

/**
 * Class SmartlingLocalApiWrapper.
 */
class SmartlingLocalApiWrapper extends SmartlingApiWrapper {

  /**
   * {@inheritdoc}
   */
  public function downloadFile($smartling_entity) {
    $file_name_unic = $smartling_entity->file_name;
    $file_path = drupal_realpath($this->settingsHandler->getDir($file_name_unic));

    $download_result = file_get_contents($file_path);

    $download_result = preg_replace_callback(
      '|>([^<^>]+)<|iU',
      function ($matches) {
        $val = strtoupper($matches[1]);
        $res = '';
        for ($i = 1; $i < strlen($val); ++$i) {
          $res .= $val[$i];
          if (preg_match('/[a-zA-Z]/', $val[$i])) {
            $res .= '~';
          }
        }
        return '>' . $res . '<';
      },
      $download_result
    );

    return $download_result;
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
  public function getStatus($smartling_entity) {
    $error_result = NULL;

    if ($smartling_entity === FALSE) {
      $this->logger->error('Smartling checks status for id - @rid is FAIL! Smartling entity not exist.', array('@rid' => $smartling_entity->rid), TRUE);
      return $error_result;
    }

    $this->logger->info('Smartling checks status for @entity_type id - @rid (@d_locale). approvedString = @as, completedString = @cs',
      array('@entity_type' => $smartling_entity->entity_type, '@rid' => $smartling_entity->rid, '@d_locale' => $smartling_entity->target_language, '@as' => 20, '@cs' => 10));

    // If true, file translated.

    $approved = 20;
    $completed = 10;
    $response_data = new \stdClass();
    $response_data->approvedStringCount = $approved;
    $response_data->completedStringCount = $completed;
    $progress = ($approved > 0) ? (int) (($completed / $approved) * 100) : 0;
    $smartling_entity->download = 0;
    $smartling_entity->progress = $progress;
    $smartling_entity->status = SMARTLING_STATUS_IN_TRANSLATE;

    return array(
      'entity_data' => $smartling_entity,
      'response_data' => $response_data,
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
        $result[$s_locale] = TRUE;
      }
    }

    return $result;
  }

  /**
   * {@inheritdoc}
   */
  public function uploadFile($file_path, $file_name_unic, $file_type, array $locales) {
    return SMARTLING_STATUS_EVENT_UPLOAD_TO_SERVICE;
  }
}
