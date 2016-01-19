<?php

/**
 * @file
 * Contains the DrupalHandler class, which is a glorified OO wrapper around
 * procedural Drupal functions that our code depends upon. This vastly
 * simplifies unit testing.
 */

namespace EntityXliff\Drupal\Utils;


class DrupalHandler {

  CONST WATCHDOG_WARNING = 4;
  CONST LANGUAGE_NONE = 'und';

  /**
   * Passes alterable variables to specific hook_TYPE_alter() implementations.
   * @param string $type
   * @param mixed $data
   * @param mixed $context1
   * @param mixed $context2
   * @param mixed $context3
   */
  public function alter($type, &$data, &$context1 = NULL, &$context2 = NULL, &$context3 = NULL) {
    drupal_alter($type, $data, $context1, $context2, $context3);
  }

  /**
   * Sets a value in a nested array with variable depth.
   * @param array $array
   * @param array $parents
   * @param mixed $value
   * @param bool $force
   * @see drupal_array_set_nested_value()
   */
  public function arraySetNestedValue(array &$array, array $parents, $value, $force = FALSE) {
    drupal_array_set_nested_value($array, $parents, $value, $force);
  }

  /**
   * Identifies the children of an element array, optionally sorted by weight.
   * @param array $elements
   * @param bool $sort
   * @return array
   * @see element_children()
   */
  public function elementChildren(&$elements, $sort = FALSE) {
    return element_children($elements, $sort);
  }

  /**
   * Get the entity info array of an entity type.
   * @param string $entityType
   * @return array
   * @see entity_get_info()
   */
  public function entityGetInfo($entityType = NULL) {
    return entity_get_info($entityType);
  }

  /**
   * Load entities from the database.
   * @param $entity_type
   * @param bool $ids
   * @param array $conditions
   * @param bool $reset
   * @return mixed
   * @see entity_load()
   */
  public function entityLoad($entity_type, $ids = FALSE, $conditions = array(), $reset = FALSE) {
    return entity_load($entity_type, $ids, $conditions, $reset);
  }

  /**
   * Returns a property wrapper for the given data.
   * @param string $type
   * @param mixed $data
   * @param array $info
   * @return \EntityMetadataWrapper
   * @see entity_metadata_wrapper()
   */
  public function entityMetadataWrapper($type, $data = NULL, array $info = array()) {
    return entity_metadata_wrapper($type, $data, $info);
  }

  /**
   * Returns a translation handler.
   * @param string $entityType
   * @param mixed $entity
   * @return \EntityTranslationHandlerInterface
   */
  public function entityTranslationGetHandler($entityType = NULL, $entity = NULL) {
    return entity_translation_get_handler($entityType, $entity);
  }

  /**
   * Determines whether the given entity type is translatable.
   * @param string $entityType
   * @param mixed  $entity
   * @param bool $skipHandler
   * @return bool
   */
  public function entityTranslationEnabled($entityType, $entity = NULL, $skipHandler = FALSE) {
    return entity_translation_enabled($entityType, $entity, $skipHandler);
  }

  /**
   * Returns entity xliff field handlers declared by installed Drupal modules.
   * @return array
   */
  public function entityXliffGetFieldHandlers() {
    return entity_xliff_get_field_handlers();
  }

  /**
   * Loads in includes provided on behalf of existing modules.
   */
  public function entityXliffLoadModuleIncs() {
    _entity_xliff_load_module_incs();
  }

  /**
   * Prepares an entity for translation.
   * @param string $entity_type
   * @param object $entity
   * @param string $langcode
   * @param object $source_entity
   * @param string $source_langcode
   */
  public function fieldAttachPrepareTranslation($entity_type, $entity, $langcode, $source_entity, $source_langcode) {
    field_attach_prepare_translation($entity_type, $entity, $langcode, $source_entity, $source_langcode);
  }

  /**
   * Returns the default language used on the site
   * @param string $property
   * @return mixed
   */
  public function languageDefault($property = NULL) {
    return language_default($property);
  }

  /**
   * Determines whether a given module exists.
   * @param string $module
   * @return bool
   * @see module_exists()
   */
  public function moduleExists($module) {
    return module_exists($module);
  }

  /**
   * Invokes a hook in all enabled modules that implement it.
   * @param string $hook
   * @return array
   */
  public function moduleInvokeAll($hook) {
    return call_user_func_array('module_invoke_all', func_get_args());
  }

  /**
   * Loads a node object from the database.
   * @param null $nid
   * @param null $vid
   * @param bool $reset
   * @see node_load()
   */
  public function nodeLoad($nid = NULL, $vid = NULL, $reset = FALSE) {
    return node_load($nid, $vid, $reset);
  }

  /**
   * Saves changes to a node or adds a new node.
   * @param object $node
   * @throws \Exception
   * @see node_save()
   */
  public function nodeSave($node) {
    node_save($node);
  }

  /**
   * Determines whether to save session data of the current request.
   * @param bool $status
   * @see drupal_save_session()
   */
  public function saveSession($status = NULL) {
    return drupal_save_session($status);
  }

  /**
   * Resets one or all centrally stored static variable(s).
   * @param string $name
   * @see drupal_static_reset()
   */
  public function staticReset($name = NULL) {
    drupal_static_reset($name);
  }

  /**
   * Gets all nodes in a given translation set.
   * @param int $tnid
   * @return array
   * @see translation_node_get_translations
   */
  public function translationNodeGetTranslations($tnid) {
    return translation_node_get_translations($tnid);
  }

  /**
   * Prepares a node for translation (content translation paradigm).
   * @param object $node
   */
  public function translationNodePrepare($node) {
    translation_node_prepare($node);
  }

  /**
   * Returns whether the given content type has support for translations.
   * @param string $type
   * @return bool
   */
  public function translationSupportedType($type) {
    return translation_supported_type($type);
  }

  /**
   * Loads a user object.
   * @param int $uid
   * @param bool $reset
   * @return mixed
   * @see user_load()
   */
  public function userLoad($uid, $reset = FALSE) {
    return user_load($uid, $reset);
  }
  /**
   * Logs a system message.
   * @param $type
   * @param $message
   * @param array $variables
   * @param null $severity
   * @param null $link
   * @see watchdog()
   */
  public function watchdog($type, $message, $variables = array(), $severity = NULL, $link = NULL) {
    watchdog($type, $message, $variables, $severity, $link);
  }

}
