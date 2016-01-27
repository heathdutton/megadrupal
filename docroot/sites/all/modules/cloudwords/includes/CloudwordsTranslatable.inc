<?php

/**
 * @file
 * Contains CloudwordsTranslatable.
 */

class CloudwordsTranslatable extends Entity {

  public $type, $textgroup, $ctid, $objectid,
  $language, $status, $translation_status, $label, $uid, $translated_document_id, $last_import;

  protected $defaultLabel;
  protected $translatableInfo;
  protected $data = array();
  protected $sourceController;
  protected $settings = array();

  /**
   * Overrides Entity::__construct().
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);
    $this->translatableInfo = cloudwords_translatable_info($this->type);
    $this->sourceController = cloudwords_get_source_controller($this->type);
  }

  /**
   * Overrides Entity::defaultUri().
   */
  protected function defaultUri() {
    if (!isset($this->defaultUri) && is_object($this->sourceController)) {
      $this->defaultUri = $this->sourceController->uri($this);
    }

    return $this->defaultUri;
  }

  public function editLink() {
    $uri = $this->uri();
    return l($this->label, $uri['path']);
  }

  public function textGroupLabel() {
    return $this->sourceController->textGroupLabel();
  }

  public function typeLabel() {
    return $this->sourceController->typeLabel();
  }

  public function targetLabel() {
    return $this->sourceController->targetLabel($this);
  }

  /**
   * Array of the data to be translated.
   *
   * The structure is similar to the form API in the way that it is a possibly
   * nested array with the following properties whose presence indicate that the
   * current element is a text that might need to be translated.
   *
   * - #text: The text to be translated.
   * - #label: (Optional) The label that might be shown to the translator.
   * - #comment: (Optional) A comment with additional information.
   * - #translate: (Optional) If set to FALSE the text will not be translated.
   * - #translation: The translated data. Set by the translator plugin.
   *
   * The key can be an alphanumeric string.
   * @param $key
   *   If present, only the subarray identified by key is returned.
   * @param $index
   *   Optional index of an attribute below $key.
   *
   * @return array
   *   A structured data array.
   */
  public function getData(array $key = array(), $index = null) {

    if (empty($this->data)) {
      $this->data = $this->sourceController->data($this);
    }

    if (empty($key)) {
      return $this->data;
    }
    if ($index) {
      $key = array_merge($key, array($index));
    }
    return drupal_array_get_nested_value($this->data, $key);
  }

  public function saveData(array $data) {
    $this->addTranslatedDataRecursive($data);
    $this->sourceController->save($this);
  }

  protected function addTranslatedDataRecursive($translation, array $key = array()) {
    if (isset($translation['#text'])) {
      $values = array(
        '#translation' => $translation,
      );
      $this->updateData($key, $values);
      return;
    }
    foreach (element_children($translation) as $item) {
      $this->addTranslatedDataRecursive($translation[$item], array_merge($key, array($item)));
    }
  }

  /**
   * Updates the values for a specific substructure in the data array.
   *
   * The values are either set or updated but never deleted.
   *
   * @param $key
   *   Key pointing to the item the values should be applied.
   *   The key can be either be an array containing the keys of a nested array
   *   hierarchy path or a string with '][' or '|' as delimiter.
   * @param $values
   *   Nested array of values to set.
   */
  public function updateData($key, $values = array()) {
    foreach ($values as $index => $value) {
      // In order to preserve existing values, we can not aplly the values array
      // at once. We need to apply each containing value on its own.
      // If $value is an array we need to advance the hierarchy level.
      if (is_array($value)) {
        $this->updateData(array_merge(cloudwords_ensure_keys_array($key), array($index)), $value);
      }
      // Apply the value.
      else {
        drupal_array_set_nested_value($this->data, array_merge(cloudwords_ensure_keys_array($key), array($index)), $value);
      }
    }
  }

  /**
   * Returns the user owning this profile.
   */
  public function user() {
    return user_load($this->uid);
  }

  /**
   * Sets a new user owning this profile.
   *
   * @param $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  public function translationStatusLabel() {
    $translation_statuses = cloudwords_exists_options_list();
    if (isset($translation_statuses[$this->translation_status])) {
      return $translation_statuses[$this->translation_status];
    }
  }

  public function queue() {
    if ($this->status == CLOUDWORDS_QUEUE_NOT_IN_QUEUE) {
      $this->status = CLOUDWORDS_QUEUE_QUEUED;
      $this->save();
    }
  }

  public function dequeue() {
    if ($this->status == CLOUDWORDS_QUEUE_QUEUED) {
      $this->status = CLOUDWORDS_QUEUE_NOT_IN_QUEUE;
      $this->save();
    }
  }

  public function cloudwordsLanguage() {
    module_load_include('inc', 'cloudwords', 'cloudwords.languages');
    $map = _cloudwords_map_drupal_cloudwords();
    return $map[$this->language];
  }

  public function setProjectTranslationStatus(CloudwordsProject $project, $status) {
    db_merge('cloudwords_content')
      ->key(array(
        'pid' => $project->getId(),
        'ctid' => $this->ctid,
      ))
      ->fields(array(
        'status' => $status,
      ))
      ->execute();
  }

  public function setSetting($key, $value) {
    $this->settings[$key] = $value;
  }

  public function getSetting($key) {
    if (isset($this->settings[$key])) {
      return $this->settings[$key];
    }
  }

  public function getSettings() {
    return $this->settings;
  }
}
