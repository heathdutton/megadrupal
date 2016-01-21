<?php

/**
 * @file
 * Contains CloudwordsI18nStringSource.
 */

class CloudwordsI18nStringSource implements CloudwordsSourceControllerInterface {

  protected $objectInfo = array();
  protected $stringInfo = array();
  protected $type;

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function __construct($type) {
    $this->type = $type;
    $this->objectInfo = i18n_object_info($type);
    $this->stringInfo = i18n_string_group_info($this->objectInfo['string translation']['textgroup']);
  }

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function typeLabel() {
    return $this->objectInfo['title'];
  }

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function textGroup() {
    return $this->objectInfo['string translation']['textgroup'];
  }

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function textGroupLabel() {
    return $this->stringInfo['title'];
  }

  /**
   * Loads the actual translation source.
   *
   * @param string $id
   *   The id of the translation source.
   *
   * Override this for custom loaders.
   */
  protected function loadObject($id) {
    if (empty($this->objectInfo['load callback']) && !empty($this->objectInfo['entity'])) {
      return entity_load_single($this->objectInfo['entity'], $id);
    }
    return $this->objectInfo['load callback']($id);
  }

  /**
   * Returns the i18n object wrapper for a translatable.
   *
   * @param CloudwordsTranslatable $translatable
   *   A Cloudwords translatable.
   *
   * @return i18n_object_wrapper
   *   An i18n_object_wrapper object.
   *
   * @see i18n_object
   */
  protected function getWrapper(CloudwordsTranslatable $translatable) {
    $object = $this->loadObject($translatable->objectid);
    return i18n_object($this->type, $object);
  }

  /**
   * Implements CloudwordsSourceControllerInterface::targetLabel().
   */
  public function targetLabel(CloudwordsTranslatable $translatable) {
    $wrapper = $this->getWrapper($translatable);
    $strings = $wrapper->get_strings();

    // Assume first string is label.
    $label = strip_tags(reset($strings)->string);
    // Trim string in order not to get limit of the label field
    // of "cloudwords_translatable" table.
    if (strlen($label) > 250) {
      $label = substr($label, 0, 250);
    }
    return $label;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::uri().
   */
  public function uri(CloudwordsTranslatable $translatable) {
    $wrapper = $this->getWrapper($translatable);
    if (method_exists($wrapper, 'get_translate_path') && ($edit_url = $wrapper->get_translate_path($translatable->language))) {
      return array(
        'path' => $edit_url,
      );
    }
  }

  /**
   * Implements CloudwordsSourceControllerInterface::data().
   */
  public function data(CloudwordsTranslatable $translatable) {
    $wrapper = $this->getWrapper($translatable);
    $strings = $wrapper->get_strings();

    $structure = array('#label' => 'i18n Strings: ' . $translatable->typeLabel());

    foreach ($strings as $string_id => $string) {
      $structure[$string_id] = array(
        '#label' => $string->title,
        '#text' => $string->string,
        '#translate' => TRUE,
      );
    }

    return $structure;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::save().
   */
  public function save(CloudwordsTranslatable $translatable) {
    foreach ($translatable->getData() as $i18n_string => $item) {
      if (isset($item['#translation']['#text'])) {
        i18n_string_translation_update($i18n_string, $item['#translation']['#text'], $translatable->language);
      }
    }
    return TRUE;
  }

}
