<?php

/**
 * @file
 * Wrapper against Profile class for CTools exportable.
 */

namespace Drupal\maps_import\Plugins\CTools\ExportUI;

use Drupal\maps_import\Cache\Object\Converter as CacheConverter;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Plugins\CTools\ExportUI\ConverterExportable;
use Drupal\maps_import\Profile\Profile as MapsImportProfile;

/**
 * Profile Exportable class.
 */
class ProfileExportable extends Exportable {

  /**
   * The profile object.
   *
   * @var Profile
   */
  protected $profile;

  /**
   * The list of converters.
   *
   * @var array
   */
  protected $converters = array();

  /**
   * Class constructor.
   */
  public function __construct(MapsImportProfile $profile = NULL, $type = EXPORT_IN_DATABASE) {
    parent::__construct($profile, $type);
    $this->disabled = is_null($profile) ? TRUE : !$profile->isEnabled();
  }

  /**
   * Magic method __set_state().
   */
  public static function __set_state(array $array) {
    $default = array(
      'disabled' => FALSE,
      'api_version' => NULL,
      'languages' => array(),
      'statuses' => array(),
      'media_types' => array(),
    );

    $array += $default + array(
      'profile' => NULL,
      'type' => EXPORT_IN_DATABASE,
    );

    if (is_numeric($array['profile'])) {
      if (!$array['profile'] = CacheProfile::getInstance()->loadSingle($array['profile'])) {
        $array['profile'] = NULL;
      }
    }
    elseif (is_string($array['profile'])) {
      if (!$array['profile'] = CacheProfile::getInstance()->loadSingle($array['profile'], 'name')) {
        $array['profile'] = NULL;
      }
    }
    elseif (is_object($array['profile']) && !$array['profile'] instanceof MapsImportProfile) {
      $array['profile'] = NULL;
    }

    $keys = array_keys($default);
    $class = get_called_class();
    $instance = new $class($array['profile'], $array['type']);

    if (!is_object($array['profile'])) {
      $schema = ctools_export_get_schema('maps_import_profile');
      $keys = array_merge($keys, array_keys($schema['fields']));
    }

    foreach ($keys as $key) {
      if (isset($array[$key])) {
        $instance->{$key} = $array[$key];
      }
    }

    if (isset($array['converters'])) {
      foreach ($array['converters'] as $converterExportable) {
        if ($converterExportable instanceof ConverterExportable) {

          // We absolutely need to pass the profile object by reference, for
          // the "save" operation where the profile is saved to the database.
          // The PID is only available there and is set to the root object.
          $converterExportable->setProfile($instance);

          $instance->addConverter($converterExportable);
        }
      }
    }

    return $instance;
  }

  /**
   * @inheritdoc
   *
   * @return MapsImportProfile
   */
  public function getWrappedObject() {
    return $this->profile;
  }

  /**
   * @inheritdoc
   *
   * @param MapsImportProfile
   */
  public function setWrappedObject($object = NULL) {
    if (!isset($object)) {
      $object = new MapsImportProfile();
    }

    $this->profile = $object;
  }

  /**
   * Get the profile object.
   *
   * This is a alias of the getWrappedObject() method, but its
   * name is really more explicit.
   *
   * @return Profile
   */
  public function getProfile() {
    return $this->profile;
  }

  /**
   * Get the converters.
   *
   * @return array
   */
  public function getConverters() {
    return $this->converters;
  }

  /**
   * Get the converter with the given name, if any.
   *
   * @param string name
   *   The converter name.
   *
   * @return ConverterExportable
   */
  public function getConverter($name) {
    if (isset($this->converters[$name])) {
      return $this->converters[$name];
    }
  }

  /**
   * Add a converter to the list.
   *
   * @param ConverterExportable $converter
   *   The converter object to add.
   */
  public function addConverter(ConverterExportable $converter) {
    $this->converters[$converter->getName()] = $converter;
  }

  /**
   * Remove a converter from the list.
   *
   * @param mixed $name
   *   Either the converter name or the converter object.
   */
  public function removeConverter($converter) {
    if (is_object($converter)) {
      if ($converter instanceof ConverterInterface) {
        $name = $converter->getName();
      }
    }
    elseif (is_string($converter)) {
      $name = $converter;
    }

    if (isset($name) && isset($this->converters[$name])) {
      unset($this->converters[$name]);
    }
  }

  /**
   * (@inheritdoc)
   */
  public function export($prefix = '') {
    $schema = ctools_export_get_schema('maps_import_profile');
    $output = array();

    $output[] = get_class($this) . '::__set_state(array(';

    if ($schema['export']['can disable']) {
      $output[] = "  'disabled' => FALSE, /* Edit this to true to make a default profile disabled initially */";
    }

    if (!empty($schema['export']['api']['current_version'])) {
      $output[] = "  'api_version' => " . ctools_var_export($schema['export']['api']['current_version'], '  ') . ',';
    }

    // Export languages, statuses and media types.
    foreach (array('languages', 'statuses', 'media_types') as $key) {
      $output[] = "  '$key' => " . ctools_var_export(variable_get('maps_import:' . $key . ':' . $this->getPid(), array()), '    ') . ',';
    }

    // Profile fields.
    $output = array_merge($output, $this->exportFromSchema('maps_import_profile', '  '));

    // Export converters.
    $output[] = "  'converters' => array(";

    foreach (CacheConverter::getInstance()->load(array($this->getPid()), 'pid') as $converter) {
      $converterExportable = new ConverterExportable($this, $converter);

      if ($converterExportable->isChildConverter() && !$parent = $converter->getParent()) {
        watchdog('Maps Suite', 'Profile export: Missing parent converter for the converter "@name".', array('@name' => $converter->getName()));
        continue;
      }

      $output = array_merge($output, explode("\n", '    ' . $converterExportable->export('    ') . ','));
    }

    $output[] = "  ),";
    $output[] = '))';

    return implode("\n$prefix", $output);
  }

}
