<?php
/**
 * @file
 * Social Content class.
 */

/**
 * TODO:
 *  Table names should be a property for ease of change
 *  Separate this class into smaller classes.
 */

abstract class SocialContent {

  // Internal settings array.
  protected $settings = array();
  // Custom language selector to let rows be imported into the current language.
  protected $currentLanguageKey = 'CURRENT';

  /**
   * Class constructor to instantiate a new object.
   *
   * @param array $instance_settings
   *   Settings for an instance.
   */
  public function __construct($instance_settings = array()) {

    $this->settings['global'] = array();
    $this->settings['instance'] = array();

    $this->loadGlobalSettings();
    if (isset($instance_settings['id'])) {
      $this->loadInstanceSettings($instance_settings['id']);
    }
  }

  /**
   * Allow for overridding of intsnace settings.
   *
   * @param array $instance_settings
   *   Settings to override the current ones with.
   */
  public function setInstanceSettings($instance_settings) {
    $this->settings['instance'] = $instance_settings;
  }

  /**
   * Load global settings.
   *
   * @return array
   *   The global settings loaded from storage
   */
  public function loadGlobalSettings() {

    $settings = variable_get($this->getFormRootElementKey(), array());
    $machine_name = $this->getMachineName();
    if (isset($settings[$machine_name])) {
      $settings = $settings[$machine_name];
    }
    $this->settings['global'] = $settings;
    return $settings;
  }

  /**
   * Delete global settings.
   */
  public function clearGlobals() {
    variable_del(self::getFormRootElementKey());
  }

  /**
   * Load instance settings for the current global.
   *
   * @param int $instance_id
   *   The instance id.
   *
   * @return array
   *   The instance settings loaded from storage.
   */
  public function loadInstanceSettings($instance_id) {

    $row = db_select('social_content_instances', 'social_content_instances')
      ->fields('social_content_instances', array())
      ->condition('id', $instance_id)
      ->execute()
      ->fetch();
    if ($row) {
      $settings = unserialize($row->settings);
      $settings['id'] = $row->id;
    }
    $settings['count'] = self::getImportCount($instance_id);
    $this->settings['instance'] = $settings;
    return $settings;
  }

  /**
   * Get an internal form of the gievn type.
   *
   * @param string $type
   *   The type of form to get.
   *
   * @return array
   *   A structured form array.
   */
  public function getForm($type) {

    $settings = array_merge($this->settings['global'], $this->settings['instance']);
    $fieldset_name = $this->getFormRootElementKey();
    $machine_name = $this->getMachineName();
    $form[$fieldset_name] = array(
      '#type' => 'fieldset',
      '#title' => $this->getLabel(),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );
    $form[$fieldset_name][$machine_name] = array();
    if ($type == 'global') {
      $form[$fieldset_name][$machine_name] = $this->globalSettingsForm();
    }
    if ($type == 'instance') {
      $form[$fieldset_name][$machine_name] = $this->instanceSettingsForm();
      if (!isset($settings['id'])) {
        $this->applyGlobalTemplate($form[$fieldset_name][$machine_name]);
      }
    }
    $form[$fieldset_name]['type'] = array(
      '#type' => 'hidden',
      '#value' => $type,
    );
    // Allow modules to attach their fields so they can be save later on.
    drupal_alter('social_content_settings', $type, $form[$fieldset_name][$machine_name], $settings);
    return $form;
  }

  /**
   * Save the form values of a form that was request with the getForm method.
   *
   * @param array $values
   *   The form values.
   */
  public function saveForm($values) {
    $type = $values[$this->getFormRootElementKey()]['type'];
    $settings = $values[$this->getFormRootElementKey()][$this->getMachineName()];
    if ($type == 'global') {
      $this->saveGlobalSettings($settings);
    }
    if ($type == 'instance') {
      $this->saveInstanceSettings($settings);
    }
  }

  /**
   * Apply global template to instance.
   */
  protected function applyGlobalTemplate(&$form, $settings = array()) {
    if (empty($settings)) {
      $settings = isset($this->settings['global']['instance_template']) ? $this->settings['global']['instance_template'] : array();
    }
    foreach (element_children($form) as $key) {
      if (!empty($settings[$key])) {
        if ($form[$key]['#type'] == 'fieldset') {
          $this->applyGlobalTemplate($form[$key], $settings[$key]);
        }
        else {
          $form[$key]['#default_value'] = $settings[$key];
        }
      }
    }
  }

  /**
   * Global settings form.
   *
   * @return array
   *   Any global settings that will be included on all global forms.
   */
  protected function globalSettingsForm() {

    $form['instance_template'] = array(
      '#type' => 'fieldset',
      '#description' => t('Template to base new instances on.'),
      '#title' => t('Instance template'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $defaults = !empty($this->settings['global']['instance_template']) ? $this->settings['global']['instance_template'] : array();
    $form['instance_template'] += $this->instanceSettingsForm();
    foreach (element_children($form['instance_template']) as $key) {
      $form['instance_template'][$key]['#required'] = FALSE;
      if ($key == 'fields' && !empty($defaults[$key])) {
        foreach ($defaults[$key] as $field_name => $default_value) {
          $form['instance_template'][$key][$field_name]['#default_value'] = !empty($defaults[$key]) ? $defaults[$key][$field_name] : array();

          if (!$form['instance_template'][$key][$field_name]['#default_value']
             && $form['instance_template'][$key][$field_name]['#type'] == 'checkbox') {
            $form['instance_template'][$key]['#default_value'] = NULL;
          }
        }
      }
      else {
        $form['instance_template'][$key]['#default_value'] = !empty($defaults[$key]) ? $defaults[$key] : array();

        if (!$form['instance_template'][$key]['#default_value']
           && $form['instance_template'][$key]['#type'] == 'checkbox') {
          $form['instance_template'][$key]['#default_value'] = NULL;
        }
      }
    }

    return $form;
  }
  /**
   * Save global settings.
   *
   * @param array $settings
   *   The settings to save.
   */
  public function saveGlobalSettings($settings) {

    $storage = variable_get($this->getFormRootElementKey(), array());
    $storage[$this->getMachineName()] = $settings;
    variable_set($this->getFormRootElementKey(), $storage);
  }

  /**
   * Instance settings form.
   *
   * @return array
   *   Any instance settings that will be included on all
   *    instance forms for the current global.
   */
  protected function instanceSettingsForm() {

    $settings = array_merge($this->settings['global'], $this->settings['instance']);
    $form = array();
    if (!isset($settings['id'])) {
      $settings = $this->instanceSettingsFields();
    }

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('Friendly name, only seen by administrators.'),
      '#default_value' => isset($settings['title']) ? $settings['title'] : NULL,
      '#required' => TRUE,
    );

    // Content type selection.
    $types = node_type_get_types();
    $content_type_options = array();
    foreach ($types as $type) {
      $content_type_options[$type->type] = $type->name;
    }
    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type'),
      '#options' => $content_type_options,
      '#default_value' => isset($settings['content_type']) ? $settings['content_type'] : NULL,
      '#required' => TRUE,
    );

    // Fields mapping.
    $fields = field_info_fields();
    $fields_options = array('_NO_MAP_' => t('Do not map'), '_REMOVE_' => t('Remove'));
    foreach ($fields as $field_name => $field_info) {
      $fields_options[$field_name] = $field_name;
    }
    asort($fields_options);

    $form['fields'] = array(
      '#type' => 'fieldset',
      '#description' => t("Please make sure that the local field can accept the remote fields value i.e. don't map a text field to a file field."),
      '#title' => t('Fields'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    foreach ($this->getFieldMappings() as $key => $value) {
      if (empty($key) || in_array($value, array('created'))) {
        continue;
      }
      elseif (isset($settings['fields'][$key])) {
        $default = $settings['fields'][$key];
      }
      else {
        $default = $value;
      }
      $form['fields'][$key] = array(
        '#type' => 'select',
        '#title' => $key,
        '#options' => $fields_options,
        '#default_value' => $default,
      );
    }
    $form['fields']['extra'] = array(
      '#type' => 'textarea',
      '#title' => t('Extra field mappings'),
      '#description' => t('Map extra fields from the record. For example if the response returns an object for each record and the record has object->user->geo, you could could map the geo attribute to a field_user_geo by entering .user.geo|field_user_geo, separate fields by a new line.'),
      '#weight' => 999,
    );

    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Authored by'),
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#element_validate' => array('social_content_element_validate_user_name'),
      '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
    );
    if (isset($settings['author'])) {
      $author = user_load($settings['author']);
      $form['author']['#default_value'] = $author->name;
    }
    else {
      $admin_user = user_load(1);
      $form['author']['#default_value'] = $admin_user->name;
    }

    $form['limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Import limit'),
      '#size' => 10,
      '#description' => t('Set the maximum number of posts to import each time. Leave blank for no limit. Some type of instances may not support this.'),
      '#default_value' => isset($settings['limit']) ? $settings['limit'] : NULL,
      '#required' => FALSE,
      '#element_validate' => array('element_validate_number'),
    );

    $language_options = array(LANGUAGE_NONE => t('Language neutral'), $this->currentLanguageKey => t('Site language'));
    $languages = language_list();
    foreach ($languages as $langcode => $language) {
      $language_options[$langcode] = $language->language;
    }
    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => $language_options,
      '#description' => t('The language to import the content into.'),
      '#default_value' => isset($settings['language']) ? $settings['language'] : NULL,
      '#required' => FALSE,
    );
    if (module_exists('entity_translation')) {      
      $form['translate'] = array(
        '#type' => 'select',
        '#title' => t('Translate'),
        '#options' => $language_options,
        '#multiple' => TRUE,
        '#description' => t('Translate imported content to these languages.'),
        '#default_value' => isset($settings['translate']) ? $settings['translate'] : NULL,
        '#required' => FALSE,
      );
      unset($form['translate']['#options'][LANGUAGE_NONE]);
      unset($form['translate']['#options'][$this->currentLanguageKey]);
    }
    $form['auto_publish'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto publish'),
      '#default_value' => isset($settings['auto_publish']) ? $settings['auto_publish'] : NULL,
      '#required' => FALSE,
    );

    $form['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled on cron'),
      '#default_value' => isset($settings['enabled']) ? $settings['enabled'] : NULL,
      '#required' => FALSE,
    );

    $form['auto_delete'] = array(
      '#type' => 'textfield',
      '#title' => t('Auto delete'),
      '#size' => 10,
      '#description' => t('Automatically delete imported content posted after X days. Leave blank to keep it forever.'),
      '#default_value' => isset($settings['auto_delete']) ? $settings['auto_delete'] : NULL,
      '#required' => FALSE,
      '#field_suffix' => 'days',
      '#element_validate' => array('element_validate_number'),
    );

    $form['global'] = array(
      '#type' => 'hidden',
      '#value' => isset($settings['global']) ? $settings['global'] : NULL,
    );
    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => isset($settings['id']) ? $settings['id'] : NULL,
    );
    return $form;
  }
  /**
   * The default field keys which will be on all instance's forms.
   *
   * @return array
   *   field keys.
   */
  protected function instanceSettingsFields() {
    return array(
      'limit' => 50,
      'author' => 1,
      'auto_publish' => TRUE,
      'auto_delete' => NULL,
      'enabled' => FALSE,
      'language' => LANGUAGE_NONE,
      'content_type' => 'social_media',
      'global' => $this->getMachineName(),
      'id' => NULL,

    );
  }

  /**
   * Save instance settings.
   *
   * @param array $settings
   *   The settings to save.
   */
  public function saveInstanceSettings($settings) {
    if (isset($settings['fields'])) {
      $extra_fields = explode("\r\n", $settings['fields']['extra']);
      $extra_fields = array_filter($extra_fields);
      unset($settings['fields']['extra']);
      foreach ($extra_fields as $map) {
        list($key, $value) = explode('|', $map);
        $settings['fields'][$key] = $value;
      }

      // Remove indicated fields.
      foreach ($settings['fields'] as $key => $value) {
        if ($value == '_REMOVE_') {
          unset($settings['fields'][$key]);
        }
        elseif ($value == '_NO_MAP_') {
          $settings['fields'][$key] = NULL;
        }
      }
      unset($settings['fields']['extra']);
    }
    $settings['id'] = is_numeric($settings['id']) ? $settings['id'] : NULL;

    // Convert author field from username to uid.
    if (empty($settings['author'])) {
      $settings['author'] = 0;
    }
    else {
      $author = user_load_by_name($settings['author']);
      $settings['author'] = $author->uid;
    }

    $row = (object) $settings;
    $row->settings = serialize($settings);
    $update = !empty($row->id) ? 'id' : array();
    drupal_write_record('social_content_instances', $row, $update);
    return $row->id;
  }

  /**
   * Get all instances for the current global.
   *
   * @return array
   *   All the instances for the current global.
   */
  public function getInstances() {
    $instances = self::getAllInstances($this->getMachineName());
    return $instances;
  }

  /**
   * Whether the current instance is enabled for import.
   *
   * @return bool
   *   TRUE for enabled and FALSE if not.
   */
  public function isEnabled() {
    return (bool) $this->settings['instance']['enabled'];
  }
  /**
   * The machine name for the global.
   *
   * Implementing / Extending classes should override this method.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'social_content';
  }
  /**
   * The Label for this global.
   *
   * Implementing / Extending classes should override this method.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {
    return t('Social Content');
  }
  /**
   * The title to use for the current instance.
   *
   * @return string
   *   The title.
   */
  public function getInstanceTitle() {
    return $this->settings['instance']['title'];
  }

  /**
   * Delete the current instance as well as it's history if required.
   *
   * @param bool $delete_history
   *   Whether to also remove the instance's history.
   *
   * @return int
   *   The number of deleted entries from the history.
   */
  public function deleteInstance($delete_history = TRUE) {
    db_delete('social_content_instances')
      ->condition('id', $this->settings['instance']['id'])
      ->execute();

    $deleted = 0;
    if ($delete_history) {
      $deleted = db_delete('social_content_history')
        ->condition('instance', $this->settings['instance']['id'])
        ->execute();
    }

    return $deleted;
  }

  /**
   * Do the import.
   *
   * Will get all social content and create nodes from them.
   *
   * @param array $settings_override
   *   (Optional) Pass through settings in a key => value array to override
   *   instance and global settings.
   *
   * @return array
   *   Stats about the import process.
   */
  public function import($settings_override = array()) {
    $import_stats = array(
      'skipped' => 0,
      'imported' => 0,
      'processed' => 0,
    );
    $field_mappings = $this->getFieldMappings();

    // Override instance settings first, as these can be used elsewhere.
    $this->settings['instance'] = array_merge($this->settings['instance'], $settings_override);
    $settings = array_merge($this->settings['global'], $this->settings['instance']);

    $last_id = NULL;
    if (!isset($settings['skip_last_id']) || $settings['skip_last_id'] == FALSE) {
      $last_id = self::getLastImportedExternalID($settings['id']);
    }

    // Determine which language the imoprted content should be in.
    global $language;
    $langcode = $settings['language'] == $this->currentLanguageKey ? $language->language : $settings['language'];
    // The rows which to imort.
    $rows = $this->getRows($last_id);
    if ($rows) {
      // We know how many rows we will process for this import.
      $import_stats['processed'] = count($rows);
      foreach ($rows as $row) {
        // We are not importing a row if it is not an object
        // or doesn't have an id
        // or prepareRow returns FALSE.
        if (!is_object($row) && !isset($row->id) || $this->prepareRow($row) === FALSE) {
          $import_stats['skipped']++;
          continue;
        }

        // The path will be machine name plus row id.
        $path = $this->getMachineName() . '/' . $row->id;
        // Basic attributes of a node.
        $values = array(
          'type' => $settings['content_type'],
          'status' => $settings['auto_publish'],
          'promote' => 0,
          'language' => $langcode,
          'created' => isset($row->created) ? $row->created : time(),
          'path' => array('alias' => $path),
        );

        // If an author has not yet been set, default to uid 1 (this is for
        // legacy reasons, everything used to always be imported as uid 1).
        if (!isset($settings['author'])) {
          $values['uid'] = 1;
        }
        // If it's set but is empty, then it's the anonymous user.
        elseif (empty($settings['author'])) {
          $values['uid'] = 0;
        }
        else {
          $values['uid'] = $settings['author'];
        }

        // Using entity_metadata_wrapper as it's easier to set fields.
        $entity = entity_create('node', $values);
        $wrapper = entity_metadata_wrapper('node', $entity);

        // We only want to set properties in this array on the $wrapper
        // or else it will throw an exception.
        // Set the title.
        $property_info = $wrapper->getPropertyInfo();
        if (isset($property_info['title'])) {        
          if (empty($row->title)) {
            $wrapper->title->set($this->getLabel() . ' : ' . $row->id);
          }
          else {
            $wrapper->title->set($row->title);
            unset($row->title);
          }
        }

        // Attach the rest of the fields.
        $this->attachFields($field_mappings, $wrapper, $row);

        // Let other modules alter wrapper given full context.
        $context = array('settings' => $settings, 'row' => $row);
        drupal_alter('social_content_import', $wrapper, $context);

        // Finally save the node.
        try {
          $wrapper->save();
          if (module_exists('entity_translation') && !empty($settings['translate'])) {
            $this->translate($wrapper->value(), $langcode, array_filter($settings['translate']));
          }
        }
        catch(Exception $e) {
          watchdog('social_content', 'Exception caught: %message', array('%message' => $e->getMessage()));
          $import_stats['skipped']++;
          continue;
        }
        // Log the import of this row to history.
        $this->logHistory($wrapper, $row);
        // Increment the import counter.
        $import_stats['imported']++;
      }
    }
    return $import_stats;
  }

  /**
   * Attach declare fields onto wrapper.
   *
   * @param array $fields
   *   The fields to attach the wrapper to.
   * @param EntityMetadataWrapper $wrapper
   *   The wrapper to which to attach the wrappers to
   * @param object $row
   *   The row to which to map the fields from.
   */
  protected function attachFields($fields, $wrapper, $row) {
    $fields = array_filter($fields);
    $property_info = $wrapper->getPropertyInfo();
    foreach ($fields as $key => $field_name) {
      if (strpos($key, '.') === 0) {
        $value = self::getDynamicProperty($row, explode('.', $key));
      }
      elseif (isset($row->{$key})) {
        $value = $row->{$key};
      }
      else {
        continue;
      }
      if (isset($property_info[$field_name])) {
        if (isset($property_info[$field_name]['property info']['file']) && is_string($value)) {
          $file = $this->saveExternalFile($value, $field_name);
          $value = !empty($file) ? $file : NULL;
        }
        elseif ($property_info[$field_name]['type'] == 'text') {
          $value = self::validateText($value);
        }        
        elseif (in_array($property_info[$field_name]['type'], array('text_formatted', 'field_item_textsummary'))) {
          $value = self::validateText($value);
          $value = is_array($value) ? $value : array('value' => $value, 'format' => 'filtered_html');
        }
        elseif ($property_info[$field_name]['type'] == 'field_item_link') {
          $value = array('url' => $value);
        }
        if ($value) {
          $wrapper->{$field_name}->set($value);
        }
      }
    }
  }

  /**
   * Translate a node to a set given languages
   *
   * @param object $node
   *   The node to translate.
   * @param string $source_language
   *   The source language to translate from.
   * @param string $langcodes
   *   The languages to translate the node to.
   */
  public function translate($node, $source_language, $langcodes) {
    $instances = field_info_instances('node', $node->type);
    $handler = entity_translation_get_handler('node', $node);
    foreach ($langcodes as $langcode) {
      $values = array();
      foreach ($instances as $instance) {
        $field_name = $instance['field_name'];
        if (!empty($node->$field_name)){
          $values[$field_name][$langcode] = !empty($node->{$field_name}[$source_language]) ? $node->{$field_name}[$source_language]  : $node->{$field_name}[LANGUAGE_NONE];
        }
      }
      $translation = array(
        'translate' => 1,
        'status' => 1,
        'language' => $langcode,
        'source' => $source_language,
      ); 
      $handler->setTranslation($translation, $values);
      field_attach_update('node', $node);
    }
  }

  /**
   * Get the rows to import.
   *
   * @param mixed $last_id
   *   The id of the last import.
   */
  abstract public function getRows($last_id = NULL);

  /**
   * Get the source being used to get the rows i.e. account / hashtag.
   *
   * @return string
   *   The hashtag / account being used to fetch the rows.
   */
  abstract public function getSource();

  /**
   * Fields to save from the row.
   *
   * Get fields to save.
   * Implementing /Extending classes should provide
   * atleast the fields returned here.
   *
   * @return array
   *   List of fields to save.
   */
  public function fields() {
    return array(
      'id' => NULL,
      'created' => NULL,
    );
  }

  /**
   * Remote fields map to local fields.
   *
   * Get fields to import.
   *
   * @return array
   *   List of fields to save.
   */
  public function getFieldMappings() {
    if (isset($this->settings['instance']['fields'])) {
      return $this->settings['instance']['fields'];
    }
    else {
      return $this->fields();
    }
  }

  /**
   * Prepare the row for import.
   *
   * This is where you should structure your fields
   *  and make sure your row contains the important properties
   *  such as id and created.
   *
   * @param object $row
   *   The row to prepare.
   *
   * @return bool
   *   FALSE to skip this row else TRUE to import row.
   */
  public function prepareRow($row) {
    $exists = db_select('social_content_history', 'social_content_history')
      ->fields('social_content_history', array('internal_id'))
      ->condition('external_id', $row->id)
      ->condition('instance', $this->settings['instance']['id'])
      ->execute()
      ->fetchField();

    if ($exists) {
      watchdog('social_content', '%global : %id already exists', array('%global' => $this->getMachineName(), '%id' => $row->id));
    }

    return !$exists;
  }

  /**
   * Log import of row to history.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapper for the internal node.
   * @param object $row
   *   The row to which to log.
   */
  protected function logHistory($wrapper, $row) {
    $history = (object) array(
      'instance' => $this->settings['instance']['id'],
      'external_id' => $row->id,
      'internal_id' => $wrapper->getIdentifier(),
      'stamp' => $row->created,
    );
    drupal_write_record('social_content_history', $history);
  }

  /**
   * The root element to use in forms.
   *
   * All form elements (both for global and instance) will
   *  have this as the key for the root element.
   * @return string
   *   The root element key.
   */
  static public function getFormRootElementKey() {
    return 'social_content';
  }

  /**
   * Utility static function to get and save a remote file.
   *
   * @param string $url
   *   The url of the file to get.
   * @param string $field_name
   *   The field name which to file will be saved to
   * @param array $validators
   *   List of validators to run against the file before it is imported.
   *
   * @return mixed
   *   An array representing the saved image or FALSE if there was an error.
   */
  protected function saveExternalFile($url, $field_name, $validators = array()) {
    $result = self::httpRequest($url);
    if ($result->code != 200) {
      return FALSE;
    }

    // Find the image extension
    // Facebook has generic /graph/picture urls that redirect.
    // So look for a redirect url first in the response.
    if (isset($result->redirect_url)) {
      $path_info = pathinfo($result->redirect_url);
    }
    else {
      $path_info = pathinfo($url);
    }

    $filename = $path_info['basename'];
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'image') {
      $validators += array('file_validate_is_image' => array());
    }
    $field_uri_scheme = $field_info['settings']['uri_scheme'];

    $instance_info = field_info_instance('node', $field_name, $this->settings['instance']['content_type']);
    if ($instance_info && !empty($instance_info['settings']) && !empty($instance_info['settings']['file_directory'])) {
      $directory = $instance_info['settings']['file_directory'];
      $dir_path = $field_uri_scheme . '://' . $directory;
      if (file_prepare_directory($dir_path, FILE_CREATE_DIRECTORY)) {
        $filename = $directory . '/' . $filename;
      }
    }

    $file = NULL;
    try {
      $file = file_save_data($result->data, $field_uri_scheme . '://' . $filename, FILE_EXISTS_RENAME);
    } catch (Exception $e) {

      watchdog('social_content', 'Error saving file: %message', array(
        '%message' => $e->getMessage(),
      ));
      return FALSE;
    }

    if (!$file) {
      return FALSE;
    }

    if ($validators && file_validate($file, $validators)) {
      return FALSE;
    }

    // Add additional fields (required for adding to a file field).
    $file->status = 1;
    $file->display = 1;
    return (array) $file;
  }

  /**
   * Utility static function to get all instances.
   *
   * @param array $global
   *   The global to which the return instances should belong to.
   * @param bool $only_enabled
   *   Whether to only retrieve enabled instances.
   *
   * @return array
   *   Instances.
   */
  static public function getAllInstances($global = array(), $only_enabled = FALSE) {
    $query = db_select('social_content_instances', 'social_content_instances')
      ->fields('social_content_instances', array());

    if ($global) {
      $query->condition('global', $global);
    }
    if ($only_enabled) {
      $query->condition('enabled', 1);
    }
    $instances = $query->execute()->fetchAllAssoc('id');

    foreach ($instances as $id => &$instance) {
      $instance->settings = unserialize($instance->settings);
      $instance->settings['id'] = $id;
      $instance->settings['count'] = self::getImportCount($id);
    }
    return $instances;
  }

  /**
   * Utility static function to get a global object from instance id.
   *
   * @param array $classes
   *   The classes which the instance could belong to.
   * @param int $instance_id
   *   The id of the instance to which to create an object for
   *
   * @return array
   *   Instances.
   */
  static public function getObjectFromInstance($classes, $instance_id) {
    $global = db_select('social_content_instances', 'social_content_instances')
      ->fields('social_content_instances', array('global'))
      ->condition('id', $instance_id)
      ->execute()
      ->fetchField();
    if ($global) {
      return new $classes[$global](array('id' => $instance_id));
    }
    else {
      return NULL;
    }
  }

  /**
   * Utility static function to get a count number of imports for an instance.
   *
   * @param int $instance_id
   *   The id of the instance to which to get the count for.
   *
   * @return int
   *   Number of imports.
   */
  static public function getImportCount($instance_id) {
    $query = db_select('social_content_history', 'history');
    $query->addExpression('COUNT(*)');
    return $query->condition('instance', $instance_id)
      ->execute()
      ->fetchField();
  }

  /**
   * Utility static function to get last imported external id.
   *
   * @param array $instance_id
   *   The id of the instance to restrict the search to.
   *
   * @return mixed
   *   Last external id.
   */
  static public function getLastImportedExternalID($instance_id) {
    return db_select('social_content_history', 'history')
      ->fields('history', array('external_id'))
      ->condition('instance', $instance_id)
      ->orderBy('stamp', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchField();
  }

  /**
   * Utility static function to delete log history of an internal id(nid).
   *
   * @param array $internal_id
   *   internal id to restrict the search to.
   *
   * @return int
   *   Number of records to delete.
   */
  static public function deleteHistory($internal_id) {
    return db_delete('social_content_history')
      ->condition('internal_id', $internal_id)
      ->execute();
  }

  /**
   * Utility method make remote request.
   *
   * @param string $url
   *   The url to make the request to.
   * @param array $options
   *   The options for this request.
   */
  static protected function httpRequest($url, $options = array()) {
    return drupal_http_request($url, $options);
  }

  /**
   * Strip non utf8 characters that can sometimes come through.
   *
   * Helper function, used by modules implementing social content types.
   *
   * @param mixed $text
   *   Filtered text.
   *
   * @return string
   *   The stripped text.
   */
  protected static function validateText($text) {
    if (is_array($text)) {
      if (!isset($text['value'])) {
        return $text;
      }
      $raw = &$text['value'];
    }
    else {
      $raw = &$text;
    }

    // The following regular expression will replace 4-byte UTF-8 characters,
    // if this doesn't happen, then there will be sql insertion errors
    // see http://stackoverflow.com/questions/16496554/can-php-detect-4-byte-encoded-utf8-chars/16496799#16496799
    $raw = preg_replace('%(?:
              \xF0[\x90-\xBF][\x80-\xBF]{2}      # planes 1-3
            | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
            | \xF4[\x80-\x8F][\x80-\xBF]{2}      # plane 16
        )%xs', '', $raw);

    return $text;
  }

  /**
   * Get the value of a nested property, providing array of property names.
   *
   *
   * @param object $row
   *   The object to traverse into.
   * @param array $attributes
   *   properties to traverse.
   *
   * @return mixed
   *   FALSE on error or the property value.
   */
  protected function getDynamicProperty($row, $attributes) {
    // Remove only NULL values.
    $attributes = array_filter($attributes, 'strlen');
    $success = TRUE;
    $object = (array) $row;
    foreach ($attributes as $attr) {
      if (isset($object[$attr])) {
        // Convert objects to array else leave alone.
        $object = is_object($object[$attr]) ? (array) $object[$attr] : $object[$attr];
      }
      else {
        $success = FALSE;
        break;
      }
    }
    return $success ? $object : FALSE;
  }
}
