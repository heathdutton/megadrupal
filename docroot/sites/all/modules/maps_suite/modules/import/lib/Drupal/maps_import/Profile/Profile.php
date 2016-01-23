<?php

/**
 * @file
 * MaPS Import Profile class.
 *
 * A profile intends to define the connexion information
 * against MaPS System® and provide some helpful methods
 * to access to the MaPS System® fetched data.
 */

namespace Drupal\maps_import\Profile;

use Drupal\maps_import\Cache\Object\Converter as CacheConverter;
use Drupal\maps_import\Request\Request;
use Drupal\maps_import\Exception\WebServiceException;
use Drupal\maps_import\Mapping\Library\Library;

/**
 * maPS Import Profile class.
 */
class Profile {

  /**
   * MaPS System® default language ID
   */
  const MAPS_SUITE_MAPS_SYSTEM_LANGUAGE_DEFAULT_ID = 1;

  /**
   * Do not group the configuration results.
   */
  const KEY_NONE = 0x0000;

  /**
   * Use the "type" column to group the configuration results.
   */
  const KEY_TYPE = 0x0001;

  /**
   * Use the "id_language" column to group the configuration results.
   */
  const KEY_LANGUAGE = 0x0002;

  /**
   * Use the "id" column to group the configuration results.
   */
  const KEY_ID = 0x0004;

  /**
   * Use the "code" column to group the configuration results.
   */
  const KEY_CODE = 0x0008;

  /**
   * The last records will override existing one if the grouping keys are the same.
   */
  const KEY_OVERRIDE = 0x0010;

  /**
   * Fetch MaPS System data from the webservices.
   */
  const FETCH_WS = 'ws';

  /**
   * Fetch MaPS System data from files.
   */
  const FETCH_FILES = 'files';

  /**
   * Defines the available options for the maximum number of objects that may be
   * processed by an HTTP request to MaPS System® Web Services.
   */
  const MAX_OBJECTS_PER_REQUEST = '1|10|50|100|500|1000|5000';

  /**
   * Defines the available options for the maximum number of objects that may be
   * mapped in a single operation.
   */
  const MAX_OBJECTS_PER_MAPPING = '1|10|50|100|200|500';

  /**
   * Unpublished state.
   */
  const STATE_NOT_PUBLISHED = 0;

  /**
   * Published state.
   */
  const STATE_PUBLISHED = 1;

  /**
   * Media accessibility public.
   */
  const MEDIA_ACCESSIBILITY_PUBLIC = 'public';

  /**
   * Media accessibility private.
   */
  const MEDIA_ACCESSIBILITY_PRIVATE = 'private';

  /**
   * The machine name for a profile.
   *
   * @var string
   */
  protected $name = NULL;

  /**
   * The primary identifier for a profile.
   *
   * @var int
   */
  protected $pid = NULL;

  /**
   * The profile title, always treated as non-markup plain text.
   *
   * @var String
   */
  protected $title;

  /**
   * The fetch method.
   *
   * @var string
   */
  protected $fetch_method;

  /**
   * The security token used to authenticate with MaPS System® Web Services.
   *
   * @var String on 40 characters
   */
  protected $token;

  /**
   * The MaPS System® publication ID.
   *
   * @var int
   */
  protected $publication_id;

  /**
   * ID of the MaPS System® root object.
   *
   * @var int
   */
  protected $root_object_id;

  /**
   * ID of the MaPS System® preset group.
   *
   * @var int
   */
  protected $preset_group_id;

  /**
   * The MaPS System® Web Services URL.
   *
   * @var string
   */
  protected $url;

  /**
   * The relative path to the media inside the Drupal file system (public or private).
   *
   * @var string
   */
  protected $media_directory;

  /**
   * The media accessibility.
   *
   * @var string
   */
  protected $media_accessibility;

  /**
   * Maximum number of objects for each HTTP request.
   *
   * @var int
   */
  protected $max_objects_per_request;

  /**
   * Maximum number of objects to process per mapping operation.
   *
   * @var int
   */
  protected $max_objects_per_op;

  /**
   * The response format, which may be either:
   * - XML
   * - JSON
   *
   * @var string
   */
  protected $format;

  /**
   * Boolean indicating whether the profile is active or not.
   *
   * @var boolean
   */
  protected $enabled;

  /**
   * The weight of this profile
   *
   * @var int
   */
  protected $weight;

  /**
   * The template to use in the MaPS System® Webervices parameters.
   *
   * @var string
   */
  protected $web_template;

  /**
   * The correspondence between MaPS System® status and Drupal ones.
   *
   * @var array
   */
  protected $statuses;

  /**
   * The MaPS System® default language ID.
   *
   * @var int
   */
  private $defaultLanguageId;

  /**
   * The additional options.
   *
   * @var array
   */
  private $options = array();

  /**
   * The configuration file.
   *
   * @var string
   */
  private $configuration_file = '';

  /**
   * The objects file.
   *
   * @var string
   */
  private $objects_file = '';



  /**
   * Get all statuses for a given profile.
   *
   * @return array
   *   A list of MaPS System® statuses.
   */
  public function getStatuses() {
    return $this->getConfigurationTypes('status', 0);
  }

  /**
   * Get the published state from a MaPS System® status.
   *
   * @param $statusId
   *   The MaPS System® status ID.
   *
   * @return int
   *   The published state.
   */
  public function getPublishedState($statusId) {
    if (empty($this->statuses)) {
//      $this->statuses = maps_suite_reduce_array($this->getStatuses(), 'id');
      $this->statuses = $this->getStatusesVariableKey();
    }

    return $this->statuses[$statusId]['status'] == 1 ? self::STATE_PUBLISHED : self::STATE_NOT_PUBLISHED;
  }

  /**
   * Get the key that is used to store the statuses into Drupal
   * variables.
   *
   * @return string
   *   The Drupal variables related key.
   *
   * @see variable_get()
   */
  public function getStatusesVariableKey() {
    $statuses = variable_get("maps_import:statuses:$this->pid", array());
    uasort($statuses, 'drupal_sort_weight');
    return $statuses;
  }

  /**
   * Get all languages for a given profile.
   *
   * @return array
   *   A list of MaPS System® languages.
   */
  public function getMapsConfigurationLanguages() {
    return $this->getConfigurationTypes('language', $this->getLanguage());
  }

  /**
   * Get languages correspondence array between MaPS System® and Drupal.
   *
   * @param $filtered
   *   Whether this method should return all defined languages, even if not
   *   mapped, or only those that have a correspondence.
   *
   * @return array
   *   An array whose keys are MaPS System® language ID and values are
   *   the related Drupal langcode.
   */
  public function getLanguages($filtered = FALSE) {
    $languages = variable_get($this->getLanguagesVariableKey(), array());
    return $filtered ? array_filter($languages) : $languages;
  }

  /**
   * Get media types correspondence array between MaPS System® and Drupal.
   *
   * @return array
   *   An array whose keys are MaPS System® media types and values are
   *   the related Drupal file types.
   *
   * @todo Retrieve by language ?
   */
  public function getMediaTypes() {
    return $this->getConfigurationTypes('media_type', $this->getLanguage());
  }

  /**
   * Returns the MaPS System® language ID that match the given
   * Drupal langcode.
   *
   * @param $langcode
   *   The Drupal language code
   * @param strict
   *   Flag indicating that no default language should be returned
   *   if no MaPS System® language matches the given Drupal langcode.
   *
   * @return int
   *   The MaPS System® language ID that corresponds to the given
   *   langcode. If the $strict parameter is set to TRUE and there is,
   *   no language mapped <ith given langcode, FALSE is returned.
   */
  public function getLanguage($langcode = NULL, $strict = FALSE) {
    if (! isset($langcode)) {
      $langcode = $GLOBALS['language']->language;
    }

    $languages = $this->getLanguages();
    $languageId = array_search($langcode, $languages);

    if (!$strict && FALSE === $languageId) {
      // First try to map with Drupal default language.
      $default = language_default('language');

      if ($default != $langcode) {
        $languageId = array_search($default, $languages);
      }

      // If still nothing, use the default MaPS System® language.
      if (FALSE === $languageId) {
        $languageId = self::MAPS_SUITE_MAPS_SYSTEM_LANGUAGE_DEFAULT_ID;
      }
    }

    return $languageId;
  }

  /**
   * Get a Drupal langcode from a MaPS System® language ID.
   *
   * @return string
   */
  public function getLangcode($languageId) {
    $languages = $this->getLanguages(TRUE);
    return isset($languages[$languageId]) ? $languages[$languageId] : NULL;
  }

  /**
   * Get the MaPS System® language ID that correspond to Drupal default language.
   */
  public function getDefaultLanguage() {
    if (!isset($this->defaultLanguageId)) {
      $this->defaultLanguageId = $this->getLanguage(language_default('language'));
    }

    return $this->defaultLanguageId;
  }

  /**
   * Get the key that is used to store the languages correspondence into
   * Drupal variables.
   *
   * @return string
   *   The Drupal variables related key.
   *
   * @see variable_get()
   */
  public function getLanguagesVariableKey() {
    return "maps_import:languages:$this->pid";
  }

  /**
   * Get the key that is used to store the media types correspondence into
   * Drupal variables.
   *
   * @return string
   *   The Drupal variables related key.
   *
   * @see variable_get()
   */
  public function getMediaTypesVariableKey() {
    return "maps_import:media_types:$this->pid";
  }

  /**
   * Get the configuration related to a specific type.
   *
   * @param $types
   *   Either the type as a string or an array of types to retrieve.
   * @param $languageId
   *   The MaPS System® language ID.
   *
   * @see self::getConfiguration()
   */
  public function getConfigurationTypes($types, $languageId = 0) {
    return $this->getConfiguration(array(array('type', $types), 'id_language' => $languageId), self::KEY_ID | self::KEY_OVERRIDE);
  }

  /**
   * Gets the profile related raw configuration.
   *
   * The returned value only contains raw data from the last request against
   * MaPS System® Web Services.
   *
   * @param $conditions
   *   An array of conditions to apply to this query.
   * @param $key
   *   The key(s) used to group the records. The possible values are given
   *   by the following constants:
   *   - KEY_NONE: no grouping.
   *   - KEY_TYPE: group by "type".
   *   - KEY_LANGUAGE: group by "id_language".
   *   - KEY_ID: group by "id".
   *   - KEY_CODE: group by "code".
   *   2 of the the "type", "language", "code" and "id" constants may be used
   *   together, the order above being used for priority.
   *   In such case, the records are grouped by the first key as first level,
   *   then by the other key.
   *   The constant KEY_OVERRIDE may be used with
   *   others to specify if the grouped records should with the same grouping
   *   keys should be overriden or put into an array of records.
   *
   * @return array
   *   The expected profile configuration.
   */
  public function getConfiguration(array $conditions = array(), $key = self::KEY_NONE) {
    $query = db_select('maps_import_configuration', NULL, array('fetch' => \PDO::FETCH_ASSOC))
      ->fields('maps_import_configuration', array('id', 'code', 'type', 'id_language', 'title', 'data'))
      ->condition('pid', $this->getPid())
      ->orderBy('type')
      ->orderBy('id_language')
      ->orderBy('id');

    if ($conditions) {
      foreach ($conditions as $column => $condition) {
        if (is_array($condition)) {
          call_user_func_array(array($query, 'condition'), $condition);
        }
        elseif (is_string($column)) {
          $query->condition($column, $condition);
        }
      }
    }

    $result = $query->execute();
    $args = array($result, (bool) ($key & self::KEY_OVERRIDE));
    $key &= ~self::KEY_OVERRIDE;

    if ($key == self::KEY_NONE) {
      $raws = $result->fetchAll();

      if (!empty($raws)) {
        foreach ($raws as &$raw) {
          $raw['data'] = unserialize($raw['data']);
        }
      }

      return $raws;
    }

    $bits = array(
      self::KEY_TYPE => 'type',
      self::KEY_LANGUAGE => 'id_language',
      self::KEY_ID => 'id',
      self::KEY_CODE => 'code',
    );

    foreach ($bits as $bit => $column) {
      if ($key & $bit) {
        $args[] = $column;
      }
    }

    return call_user_func_array(array($this, 'groupRecordsByKeys'), $args);
  }

  /**
   * Convert the current profile into an array.
   *
   * @return array
   */
  public function toArray() {
    $schema = drupal_get_schema('maps_import_profile');
    $data = array();

    foreach ($schema['fields'] as $field => $info) {
      $data[$field] = isset($this->{$field}) ? $this->{$field} : NULL;
    }

    return $data;
  }

  /**
   * Get the profile ID.
   *
   * @return int
   *   The profile ID.
   */
  public function getPid() {
    return $this->pid;
  }

  /**
   * Set the profile ID.
   */
  public function setPid($pid) {
    $this->pid = $pid;
  }

  /**
   * Get the profile machine name.
   *
   * @return string
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Set the profile machine name.
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Get the profile title.
   *
   * @return string
   *   The profile title.
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Set the profile title.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Get the fetch method.
   *
   * @return string
   *   The fetch method.
   */
  public function getFetchMethod() {
    return $this->fetch_method;
  }

  /**
   * Set the fetch method.
   *
   * @param string
   *   The fetch method.
   */
  public function setFetchMethod($fetch_method) {
    $this->fetch_method = $fetch_method;
  }

  /**
   * Get the profile security token.
   *
   * @return string
   *   The security token.
   */
  public function getToken() {
    return $this->token;
  }

  /**
   * Set the profile token.
   */
  public function setToken($token) {
    $this->token = $token;
  }

  /**
   * Get the profile publication ID.
   *
   * @return int
   *   The publication ID.
   */
  public function getPublicationId() {
    return $this->publication_id;
  }

  /**
   * Set the profile publication ID.
   */
  public function setPublicationId($publicationId) {
    $this->publication_id = $publicationId;
  }

  /**
   * Get the profile root object ID.
   *
   * @return int
   *   The root object ID.
   */
  public function getRootObjectId() {
    return $this->root_object_id;
  }

  /**
   * Set the profile root object ID.
   */
  public function setRootObjectId($rootObjectId) {
    $this->root_object_id = $rootObjectId;
  }

  /**
   * Get the profile preset group ID.
   *
   * @return int
   */
  public function getPresetGroupId() {
    return $this->preset_group_id;
  }

  /**
   * Set the profile preset group ID.
   */
  public function setPresetGroupId($presetGroupId) {
    $this->preset_group_id = $presetGroupId;
  }

  /**
   * Get the web template.
   *
   * @return string
   */
  public function getWebTemplate() {
    return $this->web_template;
  }

  /**
   * Set the web template.
   *
   * @var string
   *   The template export web.
   */
  public function setWebTemplate($web_template) {
    $this->web_template = $web_template;
  }

  /**
   * Get the MaPS System® Web Services url.
   *
   * @return string
   *   The MaPS System® Web Services url.
   */
  public function getUrl() {
    return $this->url;
  }

  /**
   * Set the MaPS System® Web Services url.
   */
  public function setUrl($url) {
    $this->url = $url;
  }

  /**
   * Get the path to media directory, relative to Drupal public
   * file directory.
   *
   * @return string
   *   The path to media directory.
   */
  public function getMediaDirectory() {
    return $this->media_directory;
  }

  /**
   * Set the path to media directory.
   */
  public function setMediaDirectory($mediaDirectory) {
    $this->media_directory = $mediaDirectory;
  }

  /**
   * Get the media accessibility.
   *
   * @return string
   *   The media accessibility.
   */
  public function getMediaAccessibility() {
    return $this->media_accessibility;
  }

  /**
   * Set the media accessibility.
   *
   * @param string $media_accessibility
   *   The media accessibility.
   */
  public function setMediaAccessibility($media_accessibility) {
    $this->media_accessibility = $media_accessibility;
  }

  /**
   * Get the maximum number of objects that may be retrieved
   * per request.
   *
   * @return int
   *   The maximum number of objects per request.
   */
  public function getMaxObjectsPerRequest() {
    return $this->max_objects_per_request;
  }

  /**
   * Set the maximum number of objects that may be retrieved
   * per request.
   */
  public function setMaxObjectsPerRequest($maxObjectsPerRequest) {
    $this->max_objects_per_request = $maxObjectsPerRequest;
  }

  /**
   * Get the maximum number of objects that may be mapped by
   * a single operation.
   *
   * @return int
   *   The maximum number of objects per mapping.
   */
  public function getMaxObjectsPerOp() {
    return $this->max_objects_per_op;
  }

  /**
   * Set the maximum number of objects that may be mapped by
   * a single operation.
   */
  public function setMaxObjectsPerOp($maxObjectsPerOp) {
    $this->max_objects_per_op = $maxObjectsPerOp;
  }

  /**
   * Get the response format.
   *
   * @return string
   *   Either JSON or XML.
   */
  public function getFormat() {
    return $this->format;
  }

  /**
   * Set the response format.
   */
  public function setFormat($format) {
    $this->format = $format;
  }

  /**
   * Get the profile status.
   *
   * @return boolean
   *   The profile status.
   */
  public function isEnabled() {
    return (bool) $this->enabled;
  }

  /**
   * Set the profile status.
   */
  public function setEnabled($enabled) {
    $this->enabled = $enabled;
  }

  /**
   * Get the profile weight.
   *
   * @return int
   *   The profile weight.
   */
  public function getWeight() {
    return $this->weight;
  }

  /**
   * Set the profile weight.
   *
   * @param int
   *   The profile weight.
   */
  public function setWeight($weight) {
    $this->weight = $weight;
  }

  /**
   * Get the configuration file name.
   *
   * @return string
   *   The configuration file name.
   */
  public function getConfigurationFile() {
    return $this->configuration_file;
  }

  /**
   * Set the configuration file name.
   *
   * @param string
   *   The configuration file name.
   */
  public function setConfigurationFile($configuration_file) {
    $this->configuration_file = $configuration_file;
  }

  /**
   * Get the objects file name.
   *
   * @return string
   *   The objects file name.
   */
  public function getObjectsFile() {
    return $this->objects_file;
  }

  /**
   * Set the objects file name.
   *
   * @param string
   *   The objects file name.
   */
  public function setObjectsFile($objects_file) {
    $this->objects_file = $objects_file;
  }

  /**
   * Get the profile options.
   *
   * @return array
   *   The profile options.
   */
  public function getOptions() {
    return $this->options;
  }
  
  /**
   * Get the specific values of the options.
   * 
   * @param $index
   *   The index to search.
   * 
   * @return array
   *   The specific options.
   */
  public function getOptionsItem($index) {
    return isset($this->options[$index]) ? $this->options[$index] : FALSE;
  }

  /**
   * Set the profile options.
   *
   * @param array
   *   The profile options.
   */
  public function setOptions(array $options = array()) {
    $this->options = $options;
  }

  /**
   * Add a new option to the profile options.
   *
   * @param string
   *   The option name.
   * @param string
   *   The option value.
   */
  public function addOption($name, $value) {
    $this->options[$name] = $value;
  }

  /**
   * Group records by specified keys.
   *
   * @see self::getConfiguration()
   */
  protected function groupRecordsByKeys(\DatabaseStatementInterface $result, $override, $key_1, $key_2 = NULL) {
    $records = array();

    if ($result->rowCount()) {
      foreach ($result as $record) {
        if (! isset($keys_num)) {
          $keys = array_filter(array($key_1, $key_2));
          $keys_num = count($keys);

          // Ensures the specified keys exist in the result set.
          if ($keys_num !== count(array_intersect_key(array_flip($keys), $record))) {
            break;
          }

          $is_object = is_object($record);
        }

        $record = (array) $record;

        if ($keys_num == 2) {
          if (!isset($records[$record[$key_1]][$record[$key_2]])) {
            $records[$record[$key_1]][$record[$key_2]] = $override ? array() : NULL;
          }

          $current = &$records[$record[$key_1]][$record[$key_2]];
        }
        else {
          if (!isset($records[$record[$key_1]])) {
            $records[$record[$key_1]] = $override ? array() : NULL;
          }

          $current = &$records[$record[$key_1]];
        }

        if ($override) {
          $current = $is_object ? (object) $record : $record;
        }
        else {
          $current[] = $is_object ? (object) $record : $record;
        }
      }
    }

    return $records;
  }

  /**
   * Get an array of options for the maximum allowed objects per request
   * or per mapping operation.
   *
   * @param $type
   *   Either 'request' or 'mapping'.
   * @param $formatted
   *   A flag indicating whether the options should be formatted to be
   *   used as selct input options.
   * @param $allow_maintenance_mode
   *   A flag indicating whether the options that are only available in
   *   maintenance mode should be returned or not.
   */
  public static function getMaxObjectOptions($type, $formatted = TRUE, $allow_maintenance_mode = TRUE) {
    static $options = array('request' => self::MAX_OBJECTS_PER_REQUEST, 'mapping' => self::MAX_OBJECTS_PER_MAPPING);

    if (!isset($options[$type])) {
      throw new WebServiceException('The option %type is not allowed and does not correspond to any number of object limit.', 0, array('%type' => $type));
    }

    if (!is_array($options[$type])) {
      $options[$type] = explode('|', $options[$type]);
    }

    $allowed = $allow_maintenance_mode ? $options[$type] : array_slice($options[$type], 1);
    return !$formatted ? $allowed : array_map(function ($number) { return number_format($number, 0, ',', ' '); }, drupal_map_assoc($allowed));
  }

  /**
   * Delete the profile from database.
   *
   * @param array $options
   *   An associative array containing options:
   *   - converter delete: (optional) Flag indicating if the converters that rely on this
   *     profile should be removed. Default to TRUE.
   *   - converter delete mode: (optional) Either 'unlink' or 'delete'. For more details
   *     on these values, see ConverterInterface::delete(). The 'reassign' mode is not
   *     available, so if the converted entities need to be reassigned, the converters
   *     have to be deleted one after another with the necessary options for reassignement
   *     before removing the profile. Default to 'unlink'.
   *   - library delete mode: A value indicating how the library related entities should be processed.
   *     Default to 'unlink'.
   *     - delete terms: All the taxonomy terms that have a correspondence with a library are
   *       deleted.
   *     - delete vocabularies: The all vocabularies that are mapped with a library are totally
   *       removed, including terms.
   *     - unlink: The taxonomy terms are kept, but no more synchronized with MaPS Suite.
   */
  public function delete(array $options = array()) {
    $options += array(
      'converter delete' => TRUE,
      'converter delete mode' => 'unlink',
      'library delete mode' => 'unlink',
      );

    if ($options['converter delete']) {
      foreach (CacheConverter::getInstance()->load(array($this->getPid()), 'pid') as $converter) {
        $converter->delete(array('mode' => $options['converter delete mode']));
      }
    }

    switch ($options['library delete mode']) {
      case 'delete terms':
        Library::deleteTerms($this);
        break;
      case 'delete vocabularies':
        Library::deleteVocabularies($this);
        break;
      case 'unlink':
      default:
        // Nothing to do, the library tables are deleted below.
        break;
    }

    $to_delete_tables = array(
      'maps_import_configuration',
      'maps_import_libraries',
      'maps_import_library_index',
      'maps_import_library_map',
      'maps_import_medias',
      'maps_import_objects',
      'maps_import_object_media',
      'maps_import_profile',
    );

    foreach ($to_delete_tables as $table) {
      db_delete($table)->condition('pid', $this->getPid())->execute();
    }
  }

}
