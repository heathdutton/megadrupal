<?php

/**
 * Event handler plugin.
 */
abstract class EventHandler extends ServicesClientPlugin {

  /**
   * Holds connection to remote server.
   *
   * @var ServicesClientConnection
   */
  protected $connection;

  /**
   * Initialized plugin classes.
   *
   * @var array
   */
  protected $plugin_instances;

  /**
   * Processed drupal entity in event.
   *
   * @var stdClass
   */
  protected $entity;

  /**
   * Stores tags assigned to event.
   *
   * @var array
   */
  protected $tags = array();

  public function __construct($event, $config) {
    parent::__construct($event, $config);
    if (empty($this->config)) {
      $this->config = $this->getDefaultConfiguration();
    }
  }

  /**
   * Retrieve event definition.
   *
   * @return ServicesClientEvent
   *   Event object wrapped by current handler.
   */
  public function getEvent() {
    $this->event->config = $this->getConfiguration();
    return $this->event;
  }

  /**
   * Save current event configuration to database.
   *
   * @return bool
   *   Save result. @see ctools_export_crud_save
   */
  public function save() {
    $event = $this->getEvent();
    ctools_export_crud_save('services_client_connection_event', $event);
    return $event;
  }

  /**
   * Retrieve default event configuration.
   *
   * @return array
   *   Default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'condition' => array(),
      'uuid_version' => 'alpha1',
      'uuid_resource' => '',
      'resource' => '',
      'debug' => ServicesClientLogLevel::INFO,
      'auto_triggered' => TRUE,
    );
  }

  /**
   * Retrieves event connection to remote site.
   *
   * @return ServicesClientConnection
   *   Initialized services client connection client.
   */
  protected function getConnection() {
    if ($this->connection == NULL) {
      $this->connection = services_client_connection_get($this->event->connection);
    }

    return $this->connection;
  }

  /**
   * Retrieve remote connection id.
   *
   * @return string
   *   Remote connection id if exists.
   */
  protected function getConnectionId() {
    $connection = services_client_connection_load($this->event->connection);
    return isset($connection->services_client_id) ? $connection->services_client_id : NULL;
  }

  /**
   * Set new connection to remote site.
   *
   * @param ServicesClientConnection $connection
   *   Initialized SC Connection client.
   *
   * @return EventHandler
   */
  public function setConnection($connection) {
    $this->connection = $connection;
    return $this;
  }

  /**
   * Add configuration plugin.
   *
   * @param string $type
   *   Plugin type. I.e. 'condition'
   *
   * @param string $name
   *   Name of the plugin handler - class.
   */
  public function addPlugin($type, $name) {
    $uuid = $this->generateUuid();
    $this->config[$type][$uuid] = array('type' => $name, 'config' => array());
    return $uuid;
  }

  /**
   * Retrieve existing plugin.
   *
   * @param string $type
   *   Plugin type.
   *
   * @param string $uuid
   *   Plugin identifier.
   *
   * @return ServicesClientPlugin
   *   Plugin instance.
   */
  public function getPlugin($type, $uuid) {
    $plugin = isset($this->config[$type][$uuid]) ? $this->config[$type][$uuid] : NULL;
    if ($plugin) {
      return $this->getPluginInstance($plugin['type'], $plugin['config'], $uuid);
    }
    else {
      throw new Exception("Missing plugin {$type}:{$uuid}");
    }
  }

  /**
   * Update plugin configuration. This does is not saved to DB until save() method is called.
   *
   * @param string $type
   *   Plugin type.
   *
   * @param string $uuid
   *   Plugin identifier.
   *
   * @param array $config
   *   Plugin configuration.
   */
  public function setPluginConfig($type, $uuid, $config) {
    $this->config[$type][$uuid]['config'] = $config;
  }

  /**
   * Remove existing plugin from configuration. This does is not saved to DB until save() method is called.
   *
   * @param string $type
   *   Plugin type, i.e. 'mapping', 'condition'.
   *
   * @param string $uuid
   *   Plugin identifier.
   */
  public function removePlugin($type, $uuid) {
    unset($this->config[$type][$uuid]);
  }

  /**
   * Retrieve plugin instance by name and configuration.
   *
   * @param string $name
   *   Class name.
   *
   * @param array $config
   *   Plugin configuration array.
   *
   * @return object
   *   Plugin instance.
   */
  protected function getPluginInstance($name, $config, $uuid) {
    if (isset($this->plugin_instances[$uuid])) {
      return $this->plugin_instances[$uuid];
    }
    else {
      if (!class_exists($name)) {
        throw new Exception(t("Missing class @name when initializing plugin.", array('@name' => $name)));
      }
      $reflection = new ReflectionClass($name);
      $plugin = $reflection->newInstanceArgs(array($this->event, $config));
      $this->plugin_instances[$uuid] = $plugin;
      return $plugin;
    }
  }

  /**
   * Configuration form.
   */
  public function configForm(&$form, &$form_state) {
    // Show conditions
    $conditions = array();
    foreach ($this->config['condition'] as $uuid => $plugin) {
      try {
        $handler = $this->getPluginInstance($plugin['type'], $plugin['config'], $uuid);
        $conditions[] = array($handler->getSummary(), implode(' | ', array(
          l(t('Edit'), $this->getUrl('plugin/condition/' . $uuid . '/edit')),
          l(t('Remove'), $this->getUrl('plugin/condition/' . $uuid . '/remove'), array('query' => array('token' => drupal_get_token($uuid))))
        )));
      }
      catch (Exception $e) {
        watchdog_exception('services_client', $e);
      }
    }

    $form['condition']['title'] = array(
      '#markup' => '<h2>' . t('Conditions') . '</h2>',
    );

    $form['condition']['table'] = array(
      '#theme' => 'table',
      '#header' => array(t('Name'), t('Actions')),
      '#rows' => $conditions,
      '#empty' => t('There are no conditions added'),
    );

    $form['condition']['add_condition'] = array(
      '#theme_wrappers' => array('container'),
      '#markup' => l('+ ' . t('Add condition'), $this->getUrl('add_plugin/condition')),
    );

    $form['remote'] = array(
      '#type' => 'fieldset',
      '#title' => t('Remote configuration'),
      '#tree' => FALSE,
      '#weight' => 100,
    );

    $form['remote']['uuid_version'] = array(
      '#type' => 'select',
      '#title' => t('UUID version'),
      '#description' => t('UUID version on remote site.'),
      '#options' => array(
        'alpha1' => t('7.0 alpha-1'),
        'alpha3+' => t('7.0 alpha-3 and higher')
      ),
      '#default_value' => $this->config['uuid_version'],
    );

    $form['remote']['uuid_resource'] = array(
      '#type' => 'textfield',
      '#title' => t('UUID resource'),
      '#description' => t('Name of the uuid resource that should be searched.'),
      '#default_value' => $this->config['uuid_resource'],
    );

    $form['remote']['resource'] = array(
      '#type' => 'textfield',
      '#title' => t('Remote resource name'),
      '#description' => t("Provide remote resource name i.e. 'node_raw'."),
      '#default_value' => $this->config['resource'],
    );

    $form['event'] = array(
      '#type' => 'fieldset',
      '#title' => t('Event configuration'),
      '#tree' => FALSE,
      '#weight' => 200,
    );

    $form['event']['auto_triggered'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically triggered'),
      '#description' => t('This event will be triggered automatically when event happens in Drupal. I.e. on node_save call.'),
      '#default_value' => $this->config['auto_triggered']
    );

    $form['event']['queue'] = array(
      '#type' => 'checkbox',
      '#title' => t('Queue execution'),
      '#description' => t('Queue execution of this event in all cases.'),
      '#default_value' => isset($this->config['queue']) ? $this->config['queue'] : NULL,
      '#states' => array(
        'visible' => array(
          ':input[name="auto_triggered"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['event']['debug'] = array(
      '#type' => 'select',
      '#title' => t('Logging level'),
      '#description' => t('Select level of debugging information sent to log.'),
      '#options' => ServicesClientLogLevel::getLevels(),
      '#default_value' => $this->config['debug']
    );
  }

  /**
   * Submit handler; Store plugin configuration.
   */
  public function configFormSubmit(&$form, &$form_state) {
    $this->config['uuid_version'] = $form_state['values']['uuid_version'];
    $this->config['uuid_resource'] = $form_state['values']['uuid_resource'];
    $this->config['resource'] = $form_state['values']['resource'];
    $this->config['debug'] = $form_state['values']['debug'];
    $this->config['auto_triggered'] = $form_state['values']['auto_triggered'];
    $this->config['queue'] = $form_state['values']['queue'];
  }

  /**
   * Execute syncing of entity.
   *
   * @return result
   */
  abstract public function execute();

  /**
   * Determine wheather entity is matching event conditions.
   *
   * @return boolean
   *   TRUE if etnity matches all conditions.
   */
  public function isMatching() {
    // If no entity is provided return FALSE
    $entity = $this->getEntity();
    if (empty($entity)) {
      return FALSE;
    }

    // Go through each plugin and check if entity matches conditions.
    foreach ($this->config['condition'] as $uuid => $plugin) {
      try {
        $handler = $this->getPluginInstance($plugin['type'], $plugin['config'], $uuid);
        if (!$handler->match($this->getEntity())) {
          return FALSE;
        }
      }
      catch (Exception $e) {
        watchdog_exception('services_client', $e);
      }
    }

    // All conditions were matched.
    return TRUE;
  }

  /**
   * Set entity for current event.
   *
   * @param stdClass $entity
   *   Drupal entity object like node or user.
   *
   * @return EntityHandler
   *   Reference to itself.
   */
  public function setEntity($entity) {
    $this->entity = $entity;
    return $this;
  }

  /**
   * Retrieve current event entity.
   *
   * @return stdClass
   *   Drupal entity.
   */
  public function getEntity() {
    return $this->entity;
  }

  /**
   * Retrieve entity ID.
   *
   * @return int
   *   Retrieve local entity id.
   */
  public function getEntityId() {
    if ($this->getEntity()) {
      list($entity_id) = entity_extract_ids($this->event->entity_type, $this->getEntity());
      return $entity_id;
    }

    return NULL;
  }

  /**
   * Get path prefixed with event specific URL.
   *
   * @param string $path
   *   Path that should be appended to URL, i.e.
   *
   * @return string
   *   Full path prefixed with event URL.
   */
  public function getUrl($path = '') {
    return $this->getBaseUrl() . (!empty($path) ? '/' . $path : '');
  }

  /**
   * Retrieve UI base url for event.
   *
   * @return string
   *   Event specific URL.
   */
  public function getBaseUrl() {
    return 'admin/structure/services_client/list/' . $this->event->name;
  }

  /**
   * Retrieve instance of object initialized with object cache.
   *
   * @return self
   */
  public function objectCached() {
    // If object is not edit locked put it to object cache.
    if (!$this->getEditLock()) {
      $class = get_class($this);
      $item = $this->getObjectCacheOrCache();
      return new $this($item, $item->config);
    }
    return $this;
  }

  /**
   * Retrieve object cached version of event.
   *
   * @return stdClass
   *   Cached version if exists, otherwise event.
   */
  public function getObjectCache() {
    ctools_include('object-cache');
    return ctools_object_cache_get('ctui_services_client', $this->event->name . '::configure');
  }

  /**
   * Retrieve current object from cache. If not currently in object cache adds object to
   * object cache.
   *
   * @return stdClass
   *   Event object.
   */
  public function getObjectCacheOrCache() {
    $event = $this->getObjectCache();

    if (empty($event)) {
      $this->setObjectCache();
      return $this->getEvent();
    }

    return $event;
  }

  /**
   * Store current object state to object cache.
   */
  public function setObjectCache() {
    $event = $this->getEvent();
    ctools_object_cache_set('ctui_services_client', $event->name . '::configure', $event);
  }

  /**
   * Clear current object cache.
   */
  public function clearObjectCache() {
    ctools_object_cache_clear('ctui_services_client', $this->event->name . '::configure');
  }

  /**
   * Determine weather object has been changed by editing configuration and not
   * which isn't stored in permanent storage.
   *
   * @return bool
   *   TRUE if is changed.
   */
  public function isChanged() {
    $item = $this->getEvent();
    return (md5(json_encode($item)) !== md5(json_encode(services_client_get_event($item->name)->getEvent())));
  }

  /**
   * Retrieve edit lock if exists (other user is editing same event).
   *
   * @return stdClass
   *   UID and timestamp of user which is editing.
   */
  public function getEditLock() {
    ctools_include('object-cache');
    return ctools_object_cache_test('ctui_services_client', $this->event->name . '::configure');
  }

  /**
   * Break any edit lock for current event.
   */
  public function breakEditLock() {
    ctools_object_cache_clear_all('ctui_services_client', $this->event->name . '::configure');
  }

  /**
   * Generates a UUID v4 using PHP code.
   *
   * Based on code from @see http://php.net/uniqid#65879 , but corrected.
   */
  protected function generateUuid() {
    // The field names refer to RFC 4122 section 4.1.2.
    return sprintf('%04x%04x-%04x-4%03x-%04x-%04x%04x%04x',
      // 32 bits for "time_low".
      mt_rand(0, 65535), mt_rand(0, 65535),
      // 16 bits for "time_mid".
      mt_rand(0, 65535),
      // 12 bits after the 0100 of (version) 4 for "time_hi_and_version".
      mt_rand(0, 4095),
      bindec(substr_replace(sprintf('%016b', mt_rand(0, 65535)), '10', 0, 2)),
      // 8 bits, the last two of which (positions 6 and 7) are 01, for "clk_seq_hi_res"
      // (hence, the 2nd hex digit after the 3rd hyphen can only be 1, 5, 9 or d)
      // 8 bits for "clk_seq_low" 48 bits for "node".
      mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535)
    );
  }

  /**
   * Retrieve printed version of any variable. Should be used
   * for logging.
   *
   * @param mixed $object
   *   Data that should be converted.
   *
   * @return string
   *   Printable version of data.
   */
  protected function debugObject($object) {
    return print_r($object, TRUE);
  }

  /**
   * Retrieve controll data for current entity.
   *
   * @return ServicesClientControl
   */
  protected function getControlData() {
    if ($this->getEntity()) {
      $remote_id = $this->getConnectionId();
      return new ServicesClientControl($this->getEntity(), services_client_get_id(), $remote_id);
    }

    return NULL;
  }

  /**
   * Retrieve remote entity ID.
   *
   * @return mixed|NULL
   *   If no remote entity exists returns NULL
   */
  public function getRemoteEntityId() {
    // For entities that don't provide UUID don't search for remote match.
    if (!isset($this->getEntity()->uuid)) {
      $this->log(ServicesClientLogLevel::ERROR, "MISSING ENTITY UUID; entity : <pre>@entity</pre>", array(
        '@entity' => $this->debugObject($this->getEntity()),
      ));
      return NULL;
    }

    // Default result.
    $result = $this->getRemoteIdByUUID($this->getEntity()->uuid, $this->config['uuid_resource'], $this->config['uuid_resource']);

    // Log remote id.
    if (!empty($result)) {
      $this->log(ServicesClientLogLevel::DEVEL, "FOUND ID; local uuid : @local_uuid, remote id : @remote_id", array(
        '@local_uuid' => $this->getEntity()->uuid,
        '@remote_id' => $result,
      ));
    }
    else {
      $this->log(ServicesClientLogLevel::DEVEL, "NOT FOUND ID; local uuid : @local_uuid", array(
        '@local_uuid' => $this->getEntity()->uuid,
        '@remote_id' => $result,
      ));
    }

    return $result;
  }

  /**
   * Load remote ID by UUID based on remote UUID configuration.
   *
   * @param string $uuid
   *   UUID identifier.
   *
   * @param string $a1_resource
   *   UUID module Alpha 1 version resource name. ('user', 'node')
   *
   * @param string $a3_resource
   *   UUID module Alpha 3+ version resource name. ('user', 'node')
   *
   * @param string $a3_attribute
   *   Alpha 3+ attribute that should be searched in retrieved object.
   *
   * @return int
   *   Remote ID if found, otherwise NULL.
   */
  protected function getRemoteIdByUUID($uuid, $a1_resource, $a3_resource, $a3_attribute = 'uuid') {
    // By UUID version decide what type of remote id is requested.
    if ($this->config['uuid_version'] == 'alpha1') {
      $result = $this->getConnection()->get('uuid', $a1_resource, array('uuid' => $uuid));
      // Make sure we're getting scalar value
      if (is_array($result) && count($result) == 1 && isset($result[0])) {
        $result = $result[0];
      }
    }
    else {
      try {
        $result = $this->getConnection()->get($a3_resource, $uuid);
        if (!empty($result) && isset($result[$a3_attribute])) {
          $result = $result[$a3_attribute];
        }
        else {
          $result = NULL;
        }
      }
      catch (ServicesClientConnectionResponseException $e) {
        // Only log error different than 404, which means that entity doens't exists
        // on remote server.
        if ($e->getErrorCode() != 404) {
          $e->log();
        }
        $result = NULL;
      }
    }

    return $result;
  }

  /**
   * Allow plugin to react on before sync event.
   *
   * @param stdClass $object
   *   Mapped object that will be sent to remote site.
   */
  protected function beforeSync($object) {}

  /**
   * Allow plugin to react on before sync event.
   *
   * @param stdClass $object
   *   Mapped object that will be sent to remote site.
   */
  protected function afterSync($object, ServicesClientEventResult $result) {}

  /**
   * Make actual request to remote site to sync object.
   *
   * @param stdClass $object
   *   Mapped object that will be sent to remote site.
   *
   * @return stdClass
   *   Response from ServicesClientConnection
   */
  abstract protected function doSync($object);

  /**
   * Log messages to Drupal watchdog.
   *
   * @param int $level
   *   Log level. @see ServicesClientLogLevel
   *
   * @param string $message
   *   Message that should be logged.
   *
   * @param array $data
   *   Watchdog data param.
   *
   * @param int $severity
   *   Watchdog severity param.
   */
  protected function log($level, $message, $data, $severity = WATCHDOG_NOTICE) {
    if ($level && $level <= $this->config['debug']) {
      watchdog('services_client', $message, $data, $severity);
    }
  }

  /**
   * Log error result from services client operation.
   *
   * @param ServicesClientEventResult $result
   *   Result of services client operation.
   */
  protected function logErrorResult($result) {
    // Don't log loops as this is special error type.
    if (!$result->success() && $result->error_type != ServicesClientErrorType::LOOP) {
      $this->log(ServicesClientLogLevel::ERROR, "ERROR: error_message : @error_message, error_type : @error_type, error_code : @error_code, entity_type : @entity_type, entity_id : @entity_id, uuid : @uuid", array(
        '@error_message' => $result->error_message,
        '@error_type' => !empty($result->error_type) ? ServicesClientErrorType::getTypeName($result->error_type) : ServicesClientErrorType::getTypeName(ServicesClientErrorType::UNKNOWN),
        '@error_code' => !empty($result->error_code) ? $result->error_code : '',
        '@entity_type' => $this->event->entity_type,
        '@entity_id' => $this->getEntityId(),
        '@uuid' => isset($this->getEntity()->uuid) ? $this->getEntity()->uuid : NULL,
      ), WATCHDOG_ERROR);
    }
  }

  /**
   * Determine weather event should be fired automatically on drupal object action like node_save or node_delete.
   *
   * @return boolean
   *   TRUE if should be triggered.
   */
  public function isAutoTriggered() {
    return !empty($this->config['auto_triggered']);
  }

  /**
   * Return TRUE if this entity shouldn't be send automatically to all connections.
   *
   * @return bool
   *   TRUE if shoudln't be auto synced.
   */
  public function skipAutosync() {
    if (!empty($this->getEntity()->_services_client_exclude)) {
      return TRUE;
    }

    foreach (module_implements('services_client_skip_autosync') as $module) {
      $function = $module . '_services_client_skip_autosync';
      $result = call_user_func_array($function, array($this, $this->getEntity(), $this->event->entity_type));
      if ($result === TRUE) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Enqueue item if should be queue.
   *
   * @param bool $force
   *   Force enqueueing.
   *
   * @return bool
   *   TRUE if queued.
   */
  public function enqueue($force = FALSE) {
    if ($this->getEntity()) {
      if ($force || !empty($this->config['queue']) || $this->getControlData()->shouldQueue()) {
        $this->log(ServicesClientLogLevel::INFO, "QUEUE; entity_type : @entity_type, entity_id : @entity_id, uuid : @uuid", array(
          '@entity_type' => $this->event->entity_type,
          '@entity_id' => $this->getEntityId(),
          '@uuid' => isset($this->getEntity()->uuid) ? $this->getEntity()->uuid : NULL,
        ));
        return $this->enqueueEntity();
      }
    }

    return FALSE;
  }

  /**
   * Put current entity to queue.
   *
   * @return bool
   *   TRUE if item was created.
   */
  protected function enqueueEntity() {
    $data = array(
      'version' => 2,
      'entity' => $this->getEntity(),
      'event' => $this->getEvent()->name,
    );

    // Get queue
    $queue = DrupalQueue::get('services_client_sync', TRUE);

    // Enqueue data
    return $queue->createItem($data);
  }

  /**
   * Add event tag.
   *
   * @param string $tag
   *   Tag that should be added to event.
   *
   * @return EventHandler
   */
  public function addTag($tag) {
    if (!$this->hasTag($tag)) {
      $this->tags[] = $tag;
    }
    return $this;
  }

  /**
   * Check if event has specific tag.
   *
   * @param string $tag
   *   Tag name.
   *
   * @return bool
   *   TRUE if event has tag.
   */
  public function hasTag($tag) {
    return in_array($tag, $this->tags);
  }

  /**
   * Remove tag from event.
   *
   * @param string $tag
   *   Tag name.
   */
  public function removeTag($tag) {
    if ($this->hasTag($tag)) {
      $this->tags = array_diff($this->tags, array($tag));
    }
  }
}

/**
 * General entity save handler.
 */
class EntitySaveHandler extends EventHandler {

  public function getDefaultConfiguration() {
    $config = parent::getDefaultConfiguration();
    $config['mapping'] = array();
    return $config;
  }

  public function configForm(&$form, &$form_state) {
    // Show conditions
    $mapping = array();
    foreach ($this->config['mapping'] as $uuid => $plugin) {
      try {
        $handler = $this->getPluginInstance($plugin['type'], $plugin['config'], $uuid);
        $mapping[] = array($handler->getReaderSummary(), $handler->getFormatterSummary(), implode(' | ', array(
          l(t('Edit'), $this->getUrl('plugin/mapping/' . $uuid . '/edit')),
          l(t('Remove'), $this->getUrl('plugin/mapping/' . $uuid . '/remove'), array('query' => array('token' => drupal_get_token($uuid))))
        )));
      } catch (Exception $e) {
        watchdog_exception('services_client', $e);
      }
    }

    $form['mapping']['title'] = array(
      '#markup' => '<h2>' . t('Mapping') . '</h2>',
    );

    $form['mapping']['table'] = array(
      '#theme' => 'table',
      '#header' => array(t('Source (local)'), t('Destination (remote)'), t('Actions')),
      '#rows' => $mapping,
      '#empty' => t('There are no mappings added'),
    );

    $form['mapping']['add_mapping'] = array(
      '#theme_wrappers' => array('container'),
      '#markup' => l('+ ' . t('Add mapping'), $this->getUrl('add_plugin/mapping')),
    );

    parent::configForm($form, $form_state);
  }

  /**
   * Retrieve object that should be send to remote site.
   *
   * @return stdClass
   *   Mapped object.
   */
  protected function getMappedObject() {
    $entity = $this->getEntity();
    $mapped = new stdClass;

    foreach ($this->config['mapping'] as $uuid => $plugin) {
      try {
        $handler = $this->getPluginInstance($plugin['type'], $plugin['config'], $uuid);
        $source = $handler->getReader()->read($entity);
        if (!empty($source)) {
          $formatted = $handler->getFormatter()->format($source);
          if (!empty($formatted['key'])) {
            // Merge fields
            if (isset($mapped->{$formatted['key']}) && is_array($mapped->{$formatted['key']})) {
              $mapped->{$formatted['key']} = array_replace_recursive($mapped->{$formatted['key']}, $formatted['value']);
            }
            else {
              $mapped->{$formatted['key']} = $formatted['value'];
            }
          }
        }
      } catch (Exception $e) {
        watchdog_exception('services_client', $e);
      }
    }

    // Automatically map UUIDs
    if (isset($entity->uuid) && !isset($mapped->uuid)) {
      $mapped->uuid = $entity->uuid;
    }

    $this->log(ServicesClientLogLevel::DEVEL, "MAPPED DATA; source : <pre>@source</pre>, mapped: <pre>@mapped</pre>", array(
      '@source' => $this->debugObject($entity),
      '@mapped' => $this->debugObject($mapped),
    ));

    return $mapped;
  }

  /**
   * Execute event and send event to remove endpoint.
   *
   * @return ServicesClientEventResult
   */
  public function execute() {
    // Load mapped object.
    $object = $this->getMappedObject();

    // Set services client control data.
    $control = $this->getControlData();
    $control->setData($object);

    $this->beforeSync($object);

    // Allow other modules to alter mapping.
    drupal_alter('services_client_mapped_object', $this, $object);

    // Create action result.
    $result = new ServicesClientEventResult();
    $result->setHandler($this);
    $result->event = $this->getEvent();
    $result->object = $object;
    $result->entity = $this->getEntity();
    $result->entity_type = $result->event->entity_type;

    // Allow other modules to react on before request.
    module_invoke_all('services_client_before_request', $this, $object);

    if ($control->isLooping()) {
      $this->log(ServicesClientLogLevel::ERROR, "LOOP; entity_type: @type, entity_id: @id, event: @event", array(
        '@type' => $this->event->entity_type,
        '@id' => $this->getEntityId(),
        '@event' => $this->event->name,
      ), WATCHDOG_ERROR);
      // Mark error as loop
      $result->error_type = ServicesClientErrorType::LOOP;
    }
    else {
      $this->log(ServicesClientLogLevel::INFO, "SENDING; connection : @connection, event : @event, entity_type : @entity_type, entity_id : @entity_id, uuid : @uuid, object : <pre>@object</pre>", array(
        '@event' => $this->event->name,
        '@connection' => $this->getConnectionId(),
        '@entity_type' => $this->event->entity_type,
        '@entity_id' => $this->getEntityId(),
        '@uuid' => isset($this->getEntity()->uuid) ? $this->getEntity()->uuid : NULL,
        '@object' => $this->debugObject($object)
      ));
      try {
        $result->response = $this->doSync($object);
        $result->request = $this->getConnection()->getRequest();
      }
      catch (ServicesClientConnectionResponseException $e) {
        $e->log();

        $result->error_message = $e->getServicesMessage();
        $result->error_code = $e->getErrorCode();
        $result->request = $e->request;
        $result->response = $e->response;

        // Determien what error type, by default we assume remote server failed.
        $error_type = ServicesClientErrorType::REMOTE_SERVER;

        // Logic errors that came from remote site, like can't login
        if ($e->getErrorCode() >= 400 && $e->getErrorCode() < 500) {
          $error_type = ServicesClientErrorType::REMOTE_LOGIC;
        }
        // For network errors cURL sets error codes bellow 100; like DNS lookup error.
        elseif ($e->getErrorCode() < 100) {
          $error_type = ServicesClientErrorType::NETWORK;
        }

        // Set error type
        $result->error_type = $error_type;
      }
      catch (Exception $e) {
        $result->error_message = $e->getMessage();
        $result->error_type = ServicesClientErrorType::UNKNOWN;
      }
    }

    $this->logErrorResult($result);

    $this->afterSync($object, $result);

    $this->log(ServicesClientLogLevel::DEVEL, "RESULT; <pre>@result</pre>", array('@result' => print_r($result, TRUE)));

    // Allow other modules to react on after request.
    module_invoke_all('services_client_after_request', $this, $object, $result);

    return $result;
  }

  /**
   * Execute sync action.
   */
  protected function doSync($object) {
    $object = (array) $object;
    $id = $this->getRemoteEntityId();
    if (empty($id)) {
      $this->getConnection()->create($this->config['resource'], $object);
      return $this->getConnection()->getResponse();
    }
    else {
      $this->getConnection()->update($this->config['resource'], $id, $object);
      return $this->getConnection()->getResponse();
    }
  }
}

/**
 * General entity delete handler.
 */
class EntityDeleteHandler extends EventHandler {

  /**
   * Execute event and send event to remove endpoint.
   *
   * @return ServicesClientEventResult
   */
  public function execute() {
    // Create action result.
    $result = new ServicesClientEventResult();
    $result->setHandler($this);
    $result->event = $this->getEvent();
    $result->object = NULL;
    $result->entity = $this->getEntity();
    $result->entity_type = $result->event->entity_type;

    // Allow other modules to react on before request.
    module_invoke_all('services_client_before_request', $this, NULL);

    $this->log(ServicesClientLogLevel::INFO, "DELETING; connection : @connection, event : @event, entity_type : @entity_type, entity_id : @entity_id, uuid : @uuid", array(
      '@event' => $this->event->name,
      '@connection' => $this->getConnectionId(),
      '@entity_type' => $this->event->entity_type,
      '@entity_id' => $this->getEntityId(),
      '@uuid' => $this->getEntity()->uuid,
    ));

    try {
      $result->response = $this->doSync(NULL);
      $result->request = $this->getConnection()->getRequest();
    }
    catch (ServicesClientConnectionResponseException $e) {
      $e->log();

      $result->error_message = $e->getServicesMessage();
      $result->error_code = $e->getErrorCode();
      $result->request = $e->request;

      // Determien what error type, by default we assume remote server failed.
      $error_type = ServicesClientErrorType::REMOTE_SERVER;

      // Logic errors that came from remote site, like can't login
      if ($e->getErrorCode() >= 400 && $e->getErrorCode() < 500) {
        $error_type = ServicesClientErrorType::REMOTE_LOGIC;
      }
      // For network errors cURL sets error codes bellow 100; like DNS lookup error.
      elseif ($e->getErrorCode() < 100) {
        $error_type = ServicesClientErrorType::NETWORK;
      }

      // Set error type
      $result->error_type = $error_type;
    }
    catch (Exception $e) {
      $result->error_message = $e->getMessage();
      $result->error_type = ServicesClientErrorType::UNKNOWN;
    }

    $this->logErrorResult($result);

    $this->afterSync(NULL, $result);

    // Allow other modules to react on after request.
    module_invoke_all('services_client_after_request', $this, NULL, $result);

    return $result;
  }

  /**
   * Execute sync action.
   */
  protected function doSync($object) {
    $id = $this->getRemoteEntityId();
    if (!empty($id)) {
      $this->getConnection()->delete($this->config['resource'], $id);
      return $this->getConnection()->getResponse();
    }
  }
}

/**
 * Adds extra logic for saving remote nodes.
 */
class NodeSaveHandler extends EntitySaveHandler {

  /**
   * Load default plugin configuration.
   */
  public function getDefaultConfiguration() {
    $conf = parent::getDefaultConfiguration();
    $conf += array(
      'node_type' => '',
      'node_author' => 'load_remote',
      'node_author_value' => 0,
      'node_author_load_remote' => 0,
    );
    return $conf;
  }

  /**
   * Configuration form.
   */
  public function configForm(&$form, &$form_state) {
    parent::configForm($form, $form_state);

    $form['node'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node configuration'),
      '#tree' => FALSE,
    );

    $form['node']['node_type'] = array(
      '#type' => 'textfield',
      '#title' => t('Node type'),
      '#description' => t('Override node type which will be sent to remote site'),
      '#default_value' => $this->config['node_type'],
    );

    $form['node']['node_author'] = array(
      '#type' => 'select',
      '#title' => t('Node author'),
      '#description' => t('How to set remote node author'),
      '#default_value' => $this->config['node_author'],
      '#options' => array(
        'value' => t('Hardcode value for each node'),
        'load_remote' => t('Map current user to remote site'),
        'no_value' => t("Don't send any author value"),
      ),
    );

    $form['node']['node_author_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Hardcoded author value'),
      '#description' => t('Enter remote site UID'),
      '#default_value' => $this->config['node_author_value'],
      '#states' => array(
        'visible' => array(
          ':input[name="node_author"]' => array('value' => 'value')
        ),
      ),
    );

    $form['node']['node_author_load_remote'] = array(
      '#type' => 'textfield',
      '#title' => t('Default author value'),
      '#description' => t('Enter remote site UID which will be used if mapping to remote uid will fail.'),
      '#default_value' => $this->config['node_author_load_remote'],
      '#states' => array(
        'visible' => array(
          ':input[name="node_author"]' => array('value' => 'load_remote')
        ),
      ),
    );
  }

  /**
   * Save configuration.
   */
  public function configFormSubmit(&$form, &$form_state) {
    parent::configFormSubmit($form, $form_state);
    $this->config['node_type'] = $form_state['values']['node_type'];
    $this->config['node_author'] = $form_state['values']['node_author'];
    $this->config['node_author_value'] = $form_state['values']['node_author_value'];
    $this->config['node_author_load_remote'] = $form_state['values']['node_author_load_remote'];
  }

  /**
   * Before sync event.
   */
  protected function beforeSync($object) {
    if (empty($object->type) && !empty($this->config['node_type'])) {
      $object->type = $this->config['node_type'];
    }

    // Load uid from remote source
    if ($this->config['node_author'] == 'load_remote') {
      try {
        if (!empty($this->getEntity()->uid)) {
          $remote_uid = $this->getRemoteUserId($this->getEntity()->uid);
          $object->uid = $remote_uid !== NULL ? $remote_uid : $this->config['node_author_load_remote'];
        }
      }
      catch (ServicesClientConnectionResponseException $e) {
        $e->log();
        $object->uid = $this->config['node_author_load_remote'];
      }
    }
    elseif ($this->config['node_author'] == 'value' && !empty($this->config['node_author_value'])) {
      $object->uid = $this->config['node_author_value'];
    }
  }

  /**
   * Retrieve remote user ID.
   *
   * @return mixed|NULL
   *   If no remote entity exists returns NULL
   */
  public function getRemoteUserId($uid) {
    $account = user_load($uid);
    $result = $this->getRemoteIdByUUID($account->uuid, 'user', 'user', 'uid');

    // Log remote id.
    if (!empty($result)) {
      $this->log(ServicesClientLogLevel::DEVEL, "FOUND AUTHOR ID; local uuid : @local_uuid, remote id : @remote_id", array(
        '@local_uuid' => $account->uuid,
        '@remote_id' => $result,
      ));
    }
    else {
      $this->log(ServicesClientLogLevel::DEVEL, "NOT AUTHOR FOUND ID; local uuid : @local_uuid", array(
        '@local_uuid' => $account->uuid,
        '@remote_id' => $result,
      ));
    }

    return $result;
  }
}

class UserSaveHandler extends EntitySaveHandler {

  public function getDefaultConfiguration() {
    $config = parent::getDefaultConfiguration();
    $config += array(
      'user_map_roles' => 0,
      'user_sync_by_name' => 0,
      'user_map_roles_map' => array(),
    );
    return $config;
  }

  /**
   * Config form.
   */
  public function configForm(&$form, &$form_state) {
    parent::configForm($form, $form_state);
    $form['user_config'] = array(
      '#prefix' => '<div id="user-config-wrapper">',
      '#suffix' => '</div>',
      '#type' => 'fieldset',
      '#title' => t('User configuration'),
      '#tree' => FALSE,
    );

    $form['user_config']['user_sync_by_name'] = array(
      '#type' => 'checkbox',
      '#title' => t('Sync by names'),
      '#description' => t('Check if users should be synced by names rather by UUID.'),
      '#default_value' => $this->config['user_sync_by_name'],
    );

    $form['user_config']['user_map_roles'] = array(
      '#type' => 'checkbox',
      '#title' => t('Map user roles'),
      '#description' => t('Check if user roles should be mapped from local site to remote'),
      '#default_value' => $this->config['user_map_roles'],
    );

    $form['user_config']['user_map_roles_widget'] = array(
      '#type' => 'fieldset',
      '#title' => t('Roles mapping'),
      '#tree' => TRUE,
    );

    // Calculate how many rows
    $rows = !empty($form_state['user_roles_count']) ? $form_state['user_roles_count'] : (count($this->config['user_map_roles_map']) + 1);
    if (empty($form_state['user_roles_count'])) {
      $form_state['user_roles_count'] = $rows;
    }

    $form['user_config']['user_map_roles_widget']['#theme'] = 'services_client_mapping_rows';

    $local_roles = array('' => '< ' . t('none') . ' >') + user_roles(TRUE);
    $remote_roles = $this->getRemoteRoles();
    for ($i = 0; $i < $rows; $i++) {
      $form['user_config']['user_map_roles_widget'][$i]['local'] = array(
        '#type' => 'select',
        '#title' => t('Local'),
        '#default_value' => isset($this->config['user_map_roles_map'][$i]['local']) ? $this->config['user_map_roles_map'][$i]['local'] : '',
        '#options' => $local_roles,
      );

      if (empty($remote_roles)) {
        $form['user_config']['user_map_roles_widget'][$i]['remote'] = array(
          '#type' => 'textfield',
          '#title' => t('Remote'),
          '#default_value' => isset($this->config['user_map_roles_map'][$i]['remote']) ? $this->config['user_map_roles_map'][$i]['remote'] : '',
        );
      }
      else {
        $form['user_config']['user_map_roles_widget'][$i]['remote'] = array(
          '#type' => 'select',
          '#title' => t('Remote'),
          '#default_value' => isset($this->config['user_map_roles_map'][$i]['remote']) ? $this->config['user_map_roles_map'][$i]['remote'] : '',
          '#options' => array('' => '< ' . t('none') . ' >') + drupal_map_assoc(array_values($remote_roles)),
        );
      }

      $form['user_config']['user_map_roles_widget']['add_row'] = array(
        '#type' => 'submit',
        '#value' => t('Add row'),
        '#submit' => array('services_client_plugin_mapping_role_add_row'),
        '#ajax' => array(
          'callback' => 'services_client_plugin_mapping_role_ajax',
          'wrapper' => 'user-config-wrapper'
        ),
        '#weight' => 100,
      );
    }

    $form['user_config']['user_map_roles_remote'] = array(
      '#markup' => '<p>' . t('Available roles') . '</p><pre>' . check_plain($this->debugObject($remote_roles)) . '</pre>',
    );
    $form['user_config']['user_map_roles_remote_refresh'] = array(
      '#type' => 'submit',
      '#value' => t('Refresh roles'),
      '#submit' => array('services_client_plugin_mapping_roles_refresh'),
      '#ajax' => array(
        'callback' => 'services_client_plugin_mapping_role_ajax',
        'wrapper' => 'user-config-wrapper'
      ),
      '#weight' => 100,
    );
  }

  /**
   * Override get remote id. This allows to sync users by name.
   */
  public function getRemoteEntityId() {
    if (!empty($this->config['user_sync_by_name'])) {
      $entity = $this->getEntity();
      $name = isset($entity->original->name) ? $entity->original->name : $entity->name;
      $result = $this->getConnection()->index('user', 'uid,name', array('name' => $name));
      if (!empty($result[0]['uid'])) {
        return $result[0]['uid'];
      }
    }
    else {
      return parent::getRemoteEntityId();
    }
  }

  /**
   * Form submit.
   */
  public function configFormSubmit(&$form, &$form_state) {
    parent::configFormSubmit($form, $form_state);
    $this->config['user_map_roles'] = $form_state['values']['user_map_roles'];
    $this->config['user_sync_by_name'] = $form_state['values']['user_sync_by_name'];

    // Store role mapping
    $this->config['user_map_roles_map'] = array();
    $i = 0;
    foreach (array_keys($form_state['values']['user_map_roles_widget']) as $key) {
      if ($key !== 'add_row') {
        $local = $form_state['values']['user_map_roles_widget'][$key]['local'];
        $remote = $form_state['values']['user_map_roles_widget'][$key]['remote'];
        if (!empty($local) && !empty($remote)) {
          $this->config['user_map_roles_map'][$i++] = array('local' => $local, 'remote' => $remote);
        }
      }
    }
  }

  /**
   * Add role mapping.
   */
  public function beforeSync($object) {
    if (!empty($this->config['user_map_roles'])) {
      $account = $this->getEntity();

      // Build map of role to format [local_role_id] => [remote_role_name]
      $map = array();
      foreach ($this->config['user_map_roles_map'] as $id => $row) {
        $map[$row['local']] = $row['remote'];
      }
      // Load remote roles in format [remote_role_name] => [remote_role_id]
      $remote_roles = array_flip($this->getRemoteRoles());

      // Build list of roles that will be attached to object
      $roles = array();
      // First we need to make sure that we have roles to map and also remote_roles
      // list isn't empty.
      if (!empty($map) && !empty($remote_roles)) {
        foreach ($map as $local_rid => $remote_role) {
          // Foreach local role, check if role is attached to local user account
          // and if we have remote role.
          if (isset($account->roles[$local_rid]) && isset($remote_roles[$remote_role])) {
            $roles[$remote_roles[$remote_role]] = $remote_role;
          }
        }
      }

      // If some mapping was created, attach roles.
      if (!empty($roles)) {
        $object->roles = $roles;
      }
    }
  }

  /**
   * Get list of remote roles.
   *
   * @return array
   *   Remote connection roles.
   */
  protected function getRemoteRoles() {
    $cid = 'services_client:remote_roles:' . $this->event->connection;
    $roles = array();
    if (($cache = cache_get($cid))) {
      $roles = $cache->data;
    }
    else {
      try {
        $roles = $this->getConnection()->action('user', 'list_roles');
        cache_set($cid, $roles, 'cache', time() + 60 * 60);
      }
      catch (ServicesClientConnectionResponseException $e) {
        $e->log();
        $roles = array();
      }
    }
    return $roles;
  }

  /**
   * Force refreshing list of remote roles.
   */
  public function refreshRemoteRoles() {
    cache_clear_all('services_client:remote_roles:' . $this->event->connection, 'cache');
    return $this->getRemoteRoles();
  }
}
