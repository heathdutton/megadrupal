<?php

/**
 * @file
 *  Base class that implements the EntityExtRatingSourceInterface interface
 *  All other plugins should actually extend this class.
 */
abstract class EntityExtRatingSource implements EntityExtRatingSourceInterface {

  // Stores the configuration of the rating source.
  protected $config;

  /**
   * Creates an instance of a plugin if it does not exist.
   *
   * @param string $class
   *   A string identifying the plugin class name.
   *
   * @return object
   *   An object of type $class.
   */
  public static function instance($class) {
    if (empty($class)) {
      throw new Exception(t('Empty source class.'));
    }
    static $instances = array();
    if (!isset($instances[$class])) {
      $instances[$class] = new $class();
    }
    return $instances[$class];
  }

  protected function __construct() {
    // Every time a plugin is instantiated, make sure we also load its
    // configuration from the database.
    $this->load();
  }

  /**
   * Configuration form.
   */
  public function configForm(&$form_state) {
    $form = array();
    return $form;
  }

  /**
   * Validation handler for the configuration form.
   */
  public function configFormValidate(&$values) {
  }

  /**
   * Submit handler for the configuration form.
   */
  public function configFormSubmit(&$values) {
    $this->setConfig($values);
    $this->save();
    drupal_set_message(t('Your changes have been saved.'));
  }

  /**
   * Setter for the config attribute.
   */
  public function setConfig($config) {
    $this->config = $config;
  }

  /**
   * Getter for the $config attribute
   */
  public function getConfig() {
    return $this->config;
  }

  /**
   * Saves the configuration.
   */
  public function save() {
    $save = new stdClass();
    $save->name = $this->sourceName();
    $save->config = $this->getConfig();
    if (db_query_range("SELECT 1 FROM {entity_ext_rating_sources} WHERE name = :name", 0, 1, array(':name' => $save->name))->fetchField()) {
      drupal_write_record('entity_ext_rating_sources', $save, 'name');
    }
    else {
      drupal_write_record('entity_ext_rating_sources', $save);
    }
  }

  /**
   * Loads the configuration.
   *
   * @return bool
   *   TRUE if the configuration was loaded, FALSE otherwhise.
   */
  public function load() {
    $config_data = db_select('entity_ext_rating_sources', 'sources')
            ->fields('sources')
            ->condition('name', $this->sourceName())
            ->execute()->fetchAssoc();
    if (is_array($config_data)) {
      $this->config = unserialize($config_data['config']);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Saves a rating value from a source.
   *
   * @param string rating_id
   *   The id of the rating.
   *
   * @param int entity_id
   *   The id of the entity.
   *
   * @param string entity_type
   *   The type of the entity.
   *
   * @param float $rating
   *   The value of the rating.
   */
  public function saveRating($rating_id, $entity_id, $entity_type, $rating) {
    $save = new stdClass();
    $save->rating_id = $rating_id;
    $save->entity_id = $entity_id;
    $save->entity_type = $entity_type;
    $save->rating = $rating;
    $save->source = $this->sourceName();
    // First, delete the old entry if it exists.
    db_delete('entity_ext_rating_values')
      ->condition('rating_id', $rating_id)
      ->condition('entity_id', $entity_id)
      ->condition('entity_type', $entity_type)
      ->condition('source', $save->source)
      ->execute();
    // Then, just write the record.
    drupal_write_record('entity_ext_rating_values', $save);
  }

  /**
   * The function will actuall call the external source API to get the rating
   * and then it will save it.
   *
   * @param array $params
   *   If the plugin cannot handle more than one URL at a time, then the $params
   *   array will have only one element. Each element is an array with the keys
   *   and values:
   *     - rating_id = the rating id.
   *     - entity_id = the entity id.
   *     - entity_type = the entity type.
   *     - params = the params array (usually contains the URL).
   */
  public function saveExternalRating($params) {
    // The params array is used by reference by the getExternalRating() method.
    $rating = $this->getExternalRating($params);
    // Save the rating, but only if the result is not FALSE.
    if ($rating !== FALSE) {
      foreach ($params as $key => $value) {
        // Adjust first the rating.
        $rating_value = $this->adjustRating($value['rating_value']);
        $this->saveRating($value['rating_id'], $value['entity_id'], $value['entity_type'], $rating_value);
      }
    }
    else {
      ob_start();
      echo '<pre>';
      print_r($params);
      echo '</pre>';
      $output = ob_get_clean();
      watchdog('entity_ext_rating', 'Source @source failed to load the ratings. Parameters: !parameters',
      array('@source' => $this->sourceName(), '!parameters' => $output), WATCHDOG_WARNING);
    }
  }

  /**
   * Adjusts the rating that comes from a source.
   *
   * For example, it is usefull when there are more plugins active and you want
   * to use a weighted arithmetic average.
   *
   * @param float $rating
   *   The value of the rating.
   *
   * @return float
   *   The new value of the rating, after the adjustment has been applied.
   */
  public function adjustRating($rating) {
    // By default, it just returns the rating.
    // @todo: implement a customizable adjustment.
    // Useless multiplcation with 1, but the '1' can actually be a variable.
    return $rating * 1;
  }
}
