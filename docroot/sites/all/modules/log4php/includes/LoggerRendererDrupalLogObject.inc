<?php

/**
 * Log4Php DrupalPlain Renderer
 */
class LoggerRendererDrupalLogObject implements LoggerRendererObject {

  /**
   * List of watchdog's log_entry array variables
   */
  public $type;
  public $message;
  public $variables;
  public $severity;
  public $link;
  public $user;
  public $request_uri;
  public $referer;
  public $ip;
  public $timestamp;

  // convenience property
  public $user_name;

  /**
   * @param LoggerRendererDrupalLogObject $drupalLogObject
   * @return string|void
   * @throws Exception
   */
  public function render($drupalLogObject) {
    throw new Exception('Do not use this class directly for logging');
  }

  /**
   * @param array $log_entry
   * @return LoggerRendererDrupalLogObject
   */
  public static function getInstanceFromLogEntry($log_entry) {
    $object = new static();
    $object->type = $log_entry['type'];
    $object->message = $log_entry['message'];
    $object->variables = $log_entry['variables'];
    $object->severity = $log_entry['severity'];
    $object->link = $log_entry['link'];
    $object->user = $log_entry['user'];
    $object->request_uri = $log_entry['request_uri'];
    $object->referer = $log_entry['referer'];
    $object->ip = $log_entry['ip'];
    $object->timestamp = $log_entry['timestamp'];

    $object->user_name = $log_entry['user']->uid > 0 ? $log_entry['user']->name : t('Anonymous');

    return $object;
  }
}
