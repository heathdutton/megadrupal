<?php

/**
 * @file
 * Defines a base ad plugin.
 */

namespace Drupal\google_dfp;

/**
 * A base ad plugin.
 */

abstract class PluginBase {

  /**
   * The plugin id.
   *
   * @var string
   */
  protected $id;

  /**
   * The plugin title.
   *
   * @var string
   */
  protected $title;

  /**
   * The plugin configuration.
   *
   * @var array
   */
  protected $configuration = array();

  /**
   * Default configuration.
   *
   * @var array
   */
  protected $defaultConfiguration = array(
    'weight' => 0,
  );

  /**
   * Constructs the ad plugin object.
   *
   * @param string $id
   *   The plugin id.
   * @param array $configuration
   *   (optional) The plugin configuration.
   */
  public function __construct($id, array $configuration = array()) {
    $this->id = $id;
    $this->setConfiguration($configuration);
  }

  /**
   * {@inheritdoc}
   */
  public static function createInstance($id, array $configuration) {
    return new static($id, $configuration);
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(array $configuration = array()) {
    if (empty($configuration)) {
      $this->configuration = $this->defaultConfiguration;
    }
    else {
      $this->configuration = $configuration;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration($key = NULL) {
    if ($key) {
      if (isset($this->configuration[$key])) {
        return $this->configuration[$key];
      }
      return isset($this->defaultConfiguration[$key]) ? $this->defaultConfiguration[$key] : FALSE;
    }
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function getId() {
    return $this->id;
  }

  /**
   * {@inheritdoc}
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsFormSubmit(&$form, &$form_state) {
    // Empty definition by default.
  }

  /**
   * Wraps menu_get_object().
   *
   * @return object
   *   The active node object.
   */
  protected function getActiveNode() {
    return menu_get_object('node');
  }

  /**
   * Wraps field_get_items().
   *
   * @param string $entity_type
   *   The entity type.
   * @param object $entity
   *   The entity.
   * @param string $field_name
   *   The field name
   *
   * @return array
   *   Array of field items.
   */
  protected function getFieldItems($entity_type, $entity, $field_name) {
    return field_get_items($entity_type, $entity, $field_name);
  }

  /**
   * Wraps taxonomy_term_load|taxonomy_term_load_multiple.
   *
   * @param array|int $ids
   *   Either a single id or array of ids.
   *
   * @return array[object]|object
   *   If a single id is passed, a single term, else an array of term objects.
   */
  protected function loadTerms($ids) {
    if (is_array($ids)) {
      return taxonomy_term_load_multiple($ids);
    }
    return taxonomy_term_load($ids);
  }

  /**
   * Wraps taxonomy_term_parents_all().
   *
   * @param \StdClass $term
   *   A term object
   *
   * @return array
   *   Array of term names.
   */
  protected function getParents($term) {
    return taxonomy_get_parents_all($term->tid);
  }

}
