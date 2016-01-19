<?php
/**
 * @file
 * Implementation adapted from https://www.drupal.org/project/psr3_watchdog
 */

use Psr\Log\LogLevel;

/**
 * Logs into drupals watchdog.
 */
class YellowCubeWatchdogLogger extends \Psr\Log\AbstractLogger {

    static $levelMap = array(
      LogLevel::EMERGENCY => WATCHDOG_EMERGENCY,
      LogLevel::ALERT => WATCHDOG_ALERT,
      LogLevel::CRITICAL => WATCHDOG_CRITICAL,
      LogLevel::ERROR => WATCHDOG_ERROR,
      LogLevel::WARNING => WATCHDOG_WARNING,
      LogLevel::NOTICE => WATCHDOG_NOTICE,
      LogLevel::INFO => WATCHDOG_INFO,
      LogLevel::DEBUG => WATCHDOG_DEBUG,
    );

    protected $type;

    /**
     * @param string $moduleName Type that should be used for calling watchdog().
     */
    public function __construct($moduleName = 'yellowcube') {
        $this->type = $moduleName;
    }

    /**
     * @inheritdoc
     */
    public function log($level, $message, array $context = array()) {
        $variables = array();
        foreach ($context as $key => $value) {
            if (strpos($message, '{' . $key . '}') !== FALSE) {
                $variables['@' . $key] = $value;
                $message = str_replace('{' . $key . '}', '@' . $key, $message);
            }
        }

        watchdog($this->type, $message, $variables + $context, self::$levelMap[$level]);
    }
}
