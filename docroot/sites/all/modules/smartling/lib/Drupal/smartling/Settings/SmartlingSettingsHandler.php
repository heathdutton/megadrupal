<?php

/**
 * @file
 * Smartling settings handler.
 */

namespace Drupal\smartling\Settings;

/**
 * Class SmartlingSettingsHandler.
 */
class SmartlingSettingsHandler {

  protected $callbackUrl;

  protected $overwriteApprovedLocales = FALSE;

  /**
   * Initialize.
   */
  public function __construct() {
    $this->callbackUrl = $this->getBaseUrl() . '/smartling/callback/' . $this->variableGet('cron_key', 'drupal');
  }

  /**
   * Wrapper for variable_get() function.
   *
   * @param string $variable_name
   *   Variable name.
   * @param mixed $default_value
   *   Variable default value.
   *
   * @return mixed
   *   Return variable value.
   */
  public function variableGet($variable_name, $default_value = NULL) {
    return variable_get($variable_name, $default_value);
  }

  /**
   * Wrapper for variable_set() function.
   *
   * @param string $variable_name
   *   Variable name.
   * @param mixed $value
   *   Variable value.
   */
  protected function variableSet($variable_name, $value) {
    variable_set($variable_name, $value);
  }

  /**
   * Wrapper for variable_del() function.
   *
   * @param string $variable_name
   *   Variable name.
   */
  protected function variableDel($variable_name) {
    variable_del($variable_name);
  }

  /**
   * Getter for global base_url variable.
   *
   * @return string
   *   BaseUrl.
   */
  public function getBaseUrl() {
    global $base_url;
    return $base_url;
  }

  /**
   * Get smartling field settings for specific entity type and specific bundle.
   *
   * @param string $entity_type
   *   Entity type for which we fetch the settings.
   *
   * @param string $bundle
   *   Bundle for which we fetch the settings.
   *
   * @return array
   */
  public function getFieldsSettingsByBundle($entity_type, $bundle) {
    if (empty($bundle)) {
      return array();
    }

    $field_settings = $this->getFieldsSettings($entity_type);

    if (is_array($field_settings) && isset($field_settings[$bundle])) {
      return $field_settings[$bundle];
    }
    return array();
  }

  /**
   * Get smartling field settings for specific entity type and specific bundle if it is set.
   *
   * @param string $entity_type
   *   Entity type for which we fetch the settings.
   *
   * @return array
   */
  public function getFieldsSettings($entity_type) {
    $fields = $this->getFieldsSettingsAll();

    if (is_array($fields) && isset($fields[$entity_type])) {
      return $fields[$entity_type];
    }
    return array();
  }

  /**
   * Fetches the field settings for all the available entity types.
   *
   * @return array | NULL
   */
  public function getFieldsSettingsAll() {
    return $this->variableGet('smartling_fields_settings');
  }

  /**
   * Set smartling fields settings for any entity type.
   *
   * @param string $entity_type
   *   Entity type for which we set the settings.
   *
   * @param array $fields_settings
   *   Smartling fields settings for entity.
   */
  public function setFieldsSettings($entity_type, array $fields_settings) {
    $settings = $this->getFieldsSettingsAll();

    if (!empty($fields_settings)) {
      $settings[$entity_type] = $fields_settings;

    }
    else {
      unset($settings[$entity_type]);
    }
    $this->variableSet('smartling_fields_settings', $settings);
  }

  /**
   * Resets all smartling fields settings to an empty array.
   *
   */
  public function resetAllFieldsSettings() {
    $settings = $this->getFieldsSettingsAll();

    foreach ($settings as $entity_type => $val) {
      $this->setFieldsSettings($entity_type, array());
    }
  }

  /**
   * Set smartling fields settings for node entity.
   *
   * @param array $node_fields_settings
   *   Smartling fields settings for node entity.
   */
  public function nodeSetFieldsSettings(array $node_fields_settings) {
    $this->setFieldsSettings('node', $node_fields_settings);
  }

  /**
   * Get smartling fields settings array for node.
   *
   * @return array
   *   Return smartling fields settings array for node entity.
   */
  public function nodeGetFieldsSettings() {
    return $this->getFieldsSettings('node');
  }

  /**
   * Get smartling fields settings array for node by bundle.
   *
   * @param string $bundle
   *   Entity bundle.
   *
   * @return array
   *   Return smartling fields settings array.
   */
  public function nodeGetFieldsSettingsByBundle($bundle) {
    return $this->getFieldsSettingsByBundle('node', $bundle);
  }

  /**
   * Set smartling fields settings for comment entity.
   *
   * @param array $comment_fields_settings
   *   Smartling fields settings for comment entity.
   */
  public function commentSetFieldsSettings(array $comment_fields_settings) {
    $this->setFieldsSettings('comment', $comment_fields_settings);
  }

  /**
   * Get smartling fields settings array for comment.
   *
   * @return array
   *   Return smartling fields settings array for comment entity.
   */
  public function commentGetFieldsSettings() {
    return $this->getFieldsSettings('comment');
  }

  /**
   * Get smartling fields settings array for comment by bundle.
   *
   * @param string $bundle
   *   Entity bundle.
   *
   * @return array
   *   Return smartling fields settings array.
   */
  public function commentGetFieldsSettingsByBundle($bundle) {
    return $this->getFieldsSettingsByBundle('comment', $bundle);
  }

  /**
   * Set smartling fields settings for taxonomy_term entity.
   *
   * @param array $taxonomy_term_fields_settings
   *   Smartling fields settings for taxonomy_term entity.
   */
  public function taxonomyTermSetFieldsSettings(array $taxonomy_term_fields_settings) {
    $this->setFieldsSettings('taxonomy_term', $taxonomy_term_fields_settings);
  }

  /**
   * Get smartling fields settings array for taxonomy_term.
   *
   * @return array
   *   Return smartling fields settings array for taxonomy_term entity.
   */
  public function taxonomyTermGetFieldsSettings() {
    return $this->getFieldsSettings('taxonomy_term');
  }

  /**
   * Get smartling fields settings array for taxonomy_term by bundle.
   *
   * @param string $bundle
   *   Entity bundle.
   *
   * @return array
   *   Return smartling fields settings array.
   */
  public function taxonomyTermGetFieldsSettingsByBundle($bundle) {
    return $this->getFieldsSettingsByBundle('taxonomy_term', $bundle);
  }

  /**
   * Set smartling fields settings for user entity.
   *
   * @param array $user_fields_settings
   *   Smartling fields settings for user entity.
   */
  public function userSetFieldsSettings(array $user_fields_settings) {
    $this->setFieldsSettings('user', $user_fields_settings);
  }

  /**
   * Get smartling fields settings array for user.
   *
   * @return array
   *   Return smartling fields settings array for user entity.
   */
  public function userGetFieldsSettings() {
    return $this->getFieldsSettings('user');
  }

  /**
   * Get smartling fields settings array for user by bundle.
   *
   * @param string $bundle
   *   Entity bundle.
   *
   * @return array
   *   Return smartling fields settings array.
   */
  public function userGetFieldsSettingsByBundle($bundle) {
    return $this->getFieldsSettingsByBundle('user', $bundle);
  }


  /**
   * Add multiple fields to smartling settings.
   *
   * @param string $entity_type
   *   Entity type.
   * @param string $bundle
   *   Entity bundle.
   * @param array $field_names
   *   Field names.
   */
  public function addMultipleFieldsToSettings($entity_type, $bundle, array $field_names = array()) {
    $settings = $this->getFieldsSettings($entity_type);
    foreach ($field_names as $field_name) {
      $settings[$bundle][$field_name] = $field_name;
    }
    $this->setFieldsSettings($entity_type, $settings);
  }

  /**
   * Add single field to smartling settings.
   *
   * @param string $entity_type
   *   Entity type.
   * @param string $bundle
   *   Entity bundle.
   * @param string $field_name
   *   Field name.
   */
  public function addSingleFieldToSettings($entity_type, $bundle, $field_name) {
    $this->addMultipleFieldsToSettings($entity_type, $bundle, array($field_name));
  }

  /**
   * Delete multiple fields from smartling settings.
   *
   * @param string $entity_type
   *   Entity type.
   * @param string $bundle
   *   Entity bundle.
   * @param array $field_names
   *   Field names.
   */
  public function deleteMultipleFieldsFromSettings($entity_type, $bundle, array $field_names = array()) {
    $settings = $this->getFieldsSettings($entity_type);

    foreach ($field_names as $field_name) {
      if (isset($settings[$bundle][$field_name])) {
        unset($settings[$bundle][$field_name]);

        if (count($settings[$bundle]) == 0) {
          unset($settings[$bundle]);
        }
      }
    }

    $this->setFieldsSettings($entity_type, $settings);
  }

  /**
   * Delete single field from smartling settings.
   *
   * @param string $entity_type
   *   Entity type.
   * @param string $bundle
   *   Entity bundle.
   * @param string $field_name
   *   Field name.
   */
  public function deleteSingleFieldFromSettings($entity_type, $bundle, $field_name) {
    $this->deleteMultipleFieldsFromSettings($entity_type, $bundle, array($field_name));
  }

  /**
   * Delete multiple bundles from smartling settings.
   *
   * @param string $entity_type
   *   Entity type.
   * @param array $bundles
   *   Entity bundles.
   */
  public function deleteMultipleBundleFromSettings($entity_type, array $bundles = array()) {
    $settings = $this->getFieldsSettings($entity_type);

    foreach ($bundles as $bundle) {
      if (isset($settings[$bundle])) {
        unset($settings[$bundle]);
      }
    }

    $this->setFieldsSettings($entity_type, $settings);
  }

  /**
   * Delete single bundle from smartling settings.
   *
   * @param string $entity_type
   *   Entity type.
   * @param string $bundle
   *   Entity bundle.
   */
  public function deleteSingleBundleFromSettings($entity_type, $bundle) {
    $this->deleteMultipleBundleFromSettings($entity_type, array($bundle));
  }


  /**
   * Get smartling API URL.
   *
   * @return string
   *   Return smartling API URL.
   */
  public function getApiUrl() {
    return $this->variableGet('smartling_api_url', SMARTLING_DEFAULT_API_URL);
  }

  /**
   * Set smartling API URL.
   *
   * @param string $api_url
   *   API url - https://capi.smartling.com/v1 by default.
   */
  public function setApiUrl($api_url) {
    if (empty($api_url)) {
      $api_url = SMARTLING_DEFAULT_API_URL;
    }
    $this->apiUrl = check_plain((string) $api_url);
    $this->variableSet('smartling_api_url', $this->apiUrl);
  }

  /**
   * Get callback url use.
   *
   * @return bool
   *   Return callback url use mode.
   */
  public function getCallbackUrlUse() {
    return $this->variableGet('smartling_callback_url_use', TRUE);
  }

  /**
   * Set callback url use.
   *
   * @param bool $use
   *   TRUE by default.
   */
  public function setCallbackUrlUse($use = TRUE) {
    $this->variableSet('smartling_callback_url_use', (bool) $use);
  }

  /**
   * Get auto authorize content.
   *
   * @return bool
   *   Return auto authorize content mode.
   */
  public function getAutoAuthorizeContent() {
    return $this->variableGet('smartling_auto_authorize_content', TRUE);
  }

  /**
   * Set auto authorize content.
   *
   * @param bool $auto
   *   TRUE by default.
   */
  public function setAutoAuthorizeContent($auto = TRUE) {
    $this->variableSet('smartling_auto_authorize_content', (bool) $auto);
  }

  /**
   * Get smartling log mode.
   *
   * @return int
   *   Return smarling log mode.
   */
  public function getLogMode() {
    return $this->variableGet('smartling_log', 1);
  }

  /**
   * Set smartling log mode.
   *
   * @param int $log_mode
   *   1 if log mode ON. 1 by default.
   */
  public function setLogMode($log_mode = 1) {
    $this->variableSet('smartling_log', (int) $log_mode);
  }

  /**
   * Get log mode options.
   *
   * @return array
   *   Return log mode options.
   */
  public function getLogModeOptions() {
    return array(0 => 'OFF', 1 => 'ON');
  }

  /**
   * Get project id.
   *
   * @return string
   *   Return project id.
   */
  public function getProjectId() {
    return $this->variableGet('smartling_project_id', '');
  }

  /**
   * Set project id.
   *
   * @param string $project_id
   *   Smartling project id.
   */
  public function setProjectId($project_id) {
    if (empty($project_id)) {
      $this->variableDel('smartling_project_id');
    }
    else {
      $this->variableSet('smartling_project_id', (string) $project_id);
    }
  }

  /**
   * Get smartling account key.
   *
   * @return string
   *   Return smartling account key.
   */
  public function getKey() {
    return $this->variableGet('smartling_key', '');
  }

  /**
   * Set smartling account key.
   *
   * @param string $key
   *   Smartling account key.
   */
  public function setKey($key) {
    if (!empty($key)) {
      $this->variableSet('smartling_key', (string) $key);
    }
  }

  /**
   * Get active retrieval type.
   *
   * @return string
   *   Return smartling active retrieval type.
   */
  public function getRetrievalType() {
    return $this->variableGet('smartling_retrieval_type', 'published');
  }

  /**
   * Set retrieval type.
   *
   * @param string $retrieval_type
   *   Retrieval type.
   */
  public function setRetrievalType($retrieval_type) {
    $this->variableSet('smartling_retrieval_type', $retrieval_type);
  }

  /**
   * Get retrieval type options array.
   *
   * @return array
   *   Return retrieval type options array.
   */
  public function getRetrievalTypeOptions() {
    return array(
      'published' => 'published',
      'pending' => 'pending',
      'pseudo' => 'pseudo',
      'pseudo_local' => 'pseudo local',
    );
  }

  /**
   * Get target language options list.
   *
   * @return array
   *   Return target language options list.
   */
  public function getTargetLanguageOptionsList() {
    $target_language_options_list = array();
    $languages = language_list();
    $default_language = language_default();
    unset($languages[$default_language->language]);

    foreach ($languages as $langcode => $language) {
      //if ($language->enabled != '0') {
      $target_language_options_list[$langcode] = check_plain($language->name);
      //}
    }
    return $target_language_options_list;
  }

  /**
   * Get target locales array.
   *
   * @return array
   *   Return target locales array.
   */
  public function getTargetLocales() {
    return $this->variableGet('smartling_target_locales', array());
  }

  /**
   * Set target locales array.
   *
   * @param array $target_locales
   *   Target locales array.
   */
  public function setTargetLocales(array $target_locales) {
    $this->variableSet('smartling_target_locales', $target_locales);
  }

  /**
   * Make and set target locales.
   *
   * Array from $form_state['values']['target_locales'].
   *
   * @param array $target_locales
   *   Target locales array.
   */
  public function makeTargetLocales(array $target_locales) {
    foreach ($target_locales as $key => $lang) {
      // Must be === 0.
      if ($lang === 0) {
        unset($target_locales[$key]);
      }
    }
    if (!empty($target_locales)) {
      $this->setTargetLocales($target_locales);
    }
  }

  /**
   * Get locales convert array.
   *
   * @return array
   *   Return locales convert array.
   */
  public function getLocalesConvertArray() {
    return $this->variableGet('smartling_locales_convert_array', array());
  }

  /**
   * Set locales convert array.
   *
   * @param array $locales_convert_array
   *   Locales convert array.
   */
  public function setLocalesConvertArray(array $locales_convert_array) {
    $this->variableSet('smartling_locales_convert_array', $locales_convert_array);
  }

  /**
   * Make locales convert array.
   *
   * @param array $values
   *   Drupal form values array.
   */
  public function makeLocalesConvertArray(array $values) {
    $locales_convert_array = $values['target_locales'];
    foreach ($values['target_locales'] as $key => $lang) {
      // Must be === 0 .
      if ($lang === 0) {
        unset($locales_convert_array[$key]);
      }
      else {
        if (!empty($values['target_locales_text_key_' . $key])) {
          $locales_convert_array[$key] = check_plain($values['target_locales_text_key_' . $key]);
        }
      }
    }
    $this->setLocalesConvertArray($locales_convert_array);
  }

  /**
   * Get Smartling dir path.
   *
   * @param string $file_name
   *   File name.
   *
   * @return string
   *   Return smartling dir path.
   */
  public function getDir($file_name = '') {
    $smartling_dir = (variable_get('file_private_path', FALSE)) ? ('private://smartling') : ('public://smartling');
    $smartling_dir .= (empty($file_name)) ? '' : '/' . $file_name;
    return (string) $smartling_dir;
  }

  /**
   * Set Smartling callback url.
   *
   * @param string $url
   *   Callback url.
   */
  public function setCallbackUrl($url) {
    $this->callbackUrl = $url;
  }

  /**
   * Get Smartling callback url.
   *
   * @return string
   *   Return smartling callback url.
   */
  public function getCallbackUrl() {
    return $this->callbackUrl;
  }

  /**
   * Set async mode.
   *
   * @param int $async_mode
   *   State of the asynchronous mode.
   */
  public function setAsyncMode($async_mode) {
    $this->variableSet('smartling_async_mode', (int) $async_mode);
  }

  /**
   * Get async mode.
   *
   * @return int
   */
  public function getAsyncMode() {
    return $this->variableGet('smartling_async_mode', 1);
  }

  /**
   * Set last cron run time.
   *
   * @param int $check_status_cron_last
   */
  public function setCheckStatusCronLast($check_status_cron_last) {
    $this->variableSet('smartling_check_status_cron_last', (int) $check_status_cron_last);
  }

  /**
   * Get last cron run time.
   *
   * @return int
   */
  public function getCheckStatusCronLast() {
    return $this->variableGet('smartling_check_status_cron_last', 0);
  }

  public function setConvertEntitiesBeforeTranslation($convert_entities_before_translation) {
    $this->variableSet('smartling_convert_entities_before_translation', (int) $convert_entities_before_translation);
  }

  public function getConvertEntitiesBeforeTranslation() {
    return $this->variableGet('smartling_convert_entities_before_translation', 1);
  }

  public function setUITranslationsMergeMode($ui_translations_merge_mode) {
    $this->variableSet('smartling_ui_translations_merge_mode', (int) $ui_translations_merge_mode);
  }

  public function getUITranslationsMergeMode() {
    return $this->variableGet('smartling_ui_translations_merge_mode', LOCALE_IMPORT_OVERWRITE);
  }

  public function setCustomRegexpPlaceholder($custom_regexp_placeholder) {
    $this->variableSet('smartling_custom_regexp_placeholder', $custom_regexp_placeholder);
  }

  public function getCustomRegexpPlaceholder() {
    return $this->variableGet('smartling_custom_regexp_placeholder', '(@|%|!)[\w-]+');
  }

  public function setAutoResubmitFlag($auto_resubmit_on_save) {
    $this->variableSet('smartling_auto_resubmit_on_save', $auto_resubmit_on_save);
  }

  public function getAutoResubmitFlag() {
    return $this->variableGet('smartling_auto_resubmit_on_save', TRUE);
  }

  public function setOverwriteApprovedLocales($overwriteApprovedLocales) {
    $this->overwriteApprovedLocales = $overwriteApprovedLocales;
  }

  public function getOverwriteApprovedLocales() {
    return $this->overwriteApprovedLocales;
  }

}
