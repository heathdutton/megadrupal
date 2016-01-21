<?php

/**
 * @file
 * Settings Plugin Factor
 */

namespace Drupal\menu_bean;
use Drupal\menu_bean\Filter\SettingsFilterInterface;
use Drupal\menu_bean\Form\SettingsFormInterface;
use Drupal\menu_bean\Setting\SettingInterface;

/**
 * Class SettingsFactor
 * @singleton
 */
class SettingsFactory {
  /**
   * Get the Settings Form Class
   *
   * @param $key
   * @param $info
   * @throws MenuBeanException
   * @return SettingInterface
   */
  public static function getSettingInstance($key, $info) {
    return self::getInstance($key, menu_bean_get_settings_class($info), 'Drupal\menu_bean\Form\SettingInterface');
  }


  /**
   * @param $key
   * @param $class
   * @param $interface
   * @return SettingsFilterInterface|SettingInterface
   * @throws MenuBeanException
   */
  protected static function getInstance($key, $class, $interface) {
    static $instances = array();

    if (!isset($instances[$key])) {
      $ref_class = new \ReflectionClass($class);

      if (in_array($interface, $ref_class->getInterfaceNames())) {
        throw new MenuBeanException("$interface does not match " . $class);
      }

      $instances[$key] = new $class();
    }

    return $instances[$key];
  }
}