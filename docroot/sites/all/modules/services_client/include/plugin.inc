<?php

/**
 * @file
 * Base plugin definitions. All other plugins should be extended from this set of
 * plugins.
 */

/**
 * Basic configurable interface.
 */
interface ServicesClientConfigurableInterface {

  /**
   * Set plugin configuration.
   *
   * @param array $config
   *   Configuration options.
   */
  public function setConfiguration($config);

  /**
   * Retrieve existing plugin configuration.
   *
   * @return array
   *   Configuration options.
   */
  public function getConfiguration();

  /**
   * Build configuration form.
   *
   * @param array $form
   *   Form array.
   *
   * @param array $form_state
   *   Form API state array.
   */
  public function configForm(&$form, &$form_state);

  /**
   * Validate configuration form.
   */
  public function configFormValidate(&$form, &$form_state);

  /**
   * Submit configuration form and save config.
   */
  public function configFormSubmit(&$form, &$form_state);

}

/**
 * Base plugin class
 */
class ServicesClientPlugin implements ServicesClientConfigurableInterface {

  /**
   * Event definition
   */
  protected $event;

  /**
   * Plugin specific configuration
   *
   * @var array
   */
  protected $config;

  /**
   * Constructor.
   *
   * @param stdClass $event
   *   Event DB record.
   *
   * @param array $config
   *   Plugin configuration.
   */
  public function __construct($event, $config) {
    // Store configuration and connection definition
    $this->event = $event;
    $this->config = $config;
  }

  /**
   * Set configuration of plugin.
   *
   * @param array $config
   *   Configuration array.
   */
  public function setConfiguration($config) {
    $this->config = $config;
    return $this;
  }

  /**
   * Retrieve current plugin configuration.
   *
   * @return array
   */
  public function getConfiguration() {
    return $this->config;
  }

  /**
   * Configuration form options
   */
  public function configForm(&$form, &$form_state) {
    $form['markup'] = array(
      '#type' => 'item',
      '#markup' => t("This plugin doesn't provide any configuration options."),
    );
  }

  public function configFormValidate(&$form, &$form_state) {}

  public function configFormSubmit(&$form, &$form_state) {}

}

/**
 * Define logging level for services client events.
 */
class ServicesClientLogLevel {
  const NONE = 0;
  const ERROR = 16;
  const INFO = 32;
  const DEVEL = 64;

  public static function getLevels() {
    return array(
      self::NONE => 'none',
      self::ERROR => 'errors',
      self::INFO => 'info',
      self::DEVEL => 'development',
    );
  }
}

/**
 * Define error types.
 */
class ServicesClientErrorType {

  /**
   * Network error, like can't connect to other site.
   */
  const NETWORK = 1;
  const LOOP = 2;
  const REMOTE_LOGIC = 3;
  const REMOTE_SERVER = 4;
  const UNKNOWN = 5;

  /**
   * Get list of all error types keyed by error type contstant and value is
   * user readable name.
   *
   * @return array
   */
  public static function getTypes() {
    return array(
      self::NETWORK => 'network',
      self::LOOP => 'loop',
      self::REMOTE_LOGIC => 'remote logic',
      self::REMOTE_SERVER => 'remote server',
      self::UNKNOWN => 'unknown',
    );
  }

  /**
   * Retrieve error type user readable name.
   *
   * @param int $type
   *   Type contstant value.
   *
   * @return string
   *   Name of error
   */
  public static function getTypeName($type) {
    $types = self::getTypes();
    return isset($types[$type]) ? $types[$type] : NULL;
  }
}

/**
 * Event representation.
 */
class ServicesClientEvent {

  /**
   * Event local id.
   *
   * @var int
   */
  public $eid;

  /**
   * Name of connection that should be used.
   *
   * @var string
   */
  public $connection;

  /**
   * Event admin title.
   *
   * @var string
   */
  public $title;

  /**
   * Event machine name.
   *
   * @var string
   */
  public $name;

  /**
   * Entity type processed by event.
   *
   * @var string
   */
  public $entity_type;

  /**
   * Event name, can be 'save', 'delete'.
   *
   * @var string
   */
  public $event;

  /**
   * Handler plugin name.
   *
   * @var string
   */
  public $plugin;

  /**
   * Event configuration.
   *
   * @var array
   */
  public $config;

  /**
   * Initialized event handler plugin.
   *
   * @var EventHandler
   */
  protected $handler;

  /**
   * Retrieve event handler.
   *
   * @return EventHandler
   *   Initialized plugin.
   */
  public function getHandler() {
    if ($this->handler === NULL) {
      $reflection = new ReflectionClass($this->plugin);
      $this->handler = $reflection->newInstanceArgs(array($this, $this->config));
    }

    return $this->handler;
  }
}

/**
 * Class for handling event operation results.
 */
class ServicesClientEventResult {

  /**
   * Initialized event handler.
   *
   * @var EventHandler
   */
  protected $handler;

  /**
   * Entity id.
   *
   * @var int
   */
  protected $entity_id;

  /**
   * Event data object.
   *
   * @var stdClass
   */
  public $event;

  /**
   * Entity which was processed in event.
   *
   * @var stdClass
   */
  public $entity;

  /**
   * Entity type.
   *
   * @var string
   */
  public $entity_type;

  /**
   * Mapped object if exists.
   *
   * @var stdClass
   */
  public $object;

  /**
   * Request sent to remote site.
   *
   * @var ServicesClientConnectionHttpRequest
   */
  public $request;

  /**
   * Response retrieved from remote site.
   *
   * @var ServicesClientConnectionResponse
   */
  public $response;

  /**
   * If failed error message from exception.
   *
   * @var string
   */
  public $error_message;

  /**
   * If failed error code from exception.
   *
   * @var int
   */
  public $error_code;

  /**
   * Error type.
   *
   * @var ServicesClientErrorType const
   */
  public $error_type;

  /**
   * Retrieve entity id.
   *
   * @return int
   */
  public function getEntityId() {
    if (empty($this->entity_id)) {
      list($this->entity_id) = entity_extract_ids($this->entity_type, $this->entity);
    }

    return $this->entity_id;
  }

  /**
   * Retrieve event handler.
   *
   * @return EventHandler
   */
  public function getHandler() {
    // Lazy load handler.
    if (empty($this->handler)) {
      $this->handler = $this->event->getHandler();
    }

    return $this->handler;
  }

  /**
   * Set current handler.
   *
   * @param EventHandler $handler
   *   Event handler.
   */
  public function setHandler(EventHandler $handler) {
    $this->handler = $handler;
    return $this;
  }

  /**
   * Retry event.
   *
   * @return ServicesClientEventResult
   *   Result of new operation.
   */
  public function retry() {
    return $this->getHandler()->execute();
  }

  /**
   * Returns if event was successful.
   *
   * @return bool
   *   TRUE if successful.
   */
  public function success() {
    return empty($this->error_code) && empty($this->error_type);
  }
}

/**
 * Class for handling control data.
 */
class ServicesClientControl {

  protected $entity;

  protected $data;

  protected $client_id;

  protected $remote_id;

  /**
   * Constructor.
   *
   * @param stdClass $entity
   *   Drupal entity that is being processed.
   *
   * @param string $client_id
   *   Name of local services client
   *
   * @param string $remote_id
   *   Remote client id.
   */
  public function __construct($entity, $client_id, $remote_id) {
    $this->entity = $entity;
    $this->data = isset($entity->_services_client) ? $entity->_services_client : array();
    $this->client_id = $client_id;
    $this->remote_id = $remote_id;
  }

  /**
   * Determine if bypass queue flag is set.
   *
   * @return bool
   *   TRUE if bypassing queue is requested.
   */
  public function bypassQueue() {
    return (!empty($this->data['bypass_queue']) || !empty($this->data['v2']['bypass_queue']));
  }

  /**
   * Determine weather data should be queued.
   *
   * @return bool
   *   TRUE if event should be queued.
   */
  public function shouldQueue() {
    if (!$this->bypassQueue() && (!empty($this->data['origin']) || !empty($this->data['v2']['id']))) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Determine weather entity received by event is looping.
   *
   * @return boolean
   *   TRUE if entity already visited.
   */
  public function isLooping() {
    if (isset($this->data['visted']) && in_array($this->remote_id, $this->data['visted'])) {
      return TRUE;
    }

    if (isset($this->data['v2']['nodes']) && in_array($this->remote_id, $this->data['v2']['nodes'])) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Retrieve new controlling data which contains info about current node.
   *
   * @return array
   */
  public function getData() {
    $data = $this->data;

    // V1 data
    $data['origin'] = services_client_get_id();
    $data['visted'] = isset($data['visted']) ? $data['visted'] : array();
    if (empty($data['visted']) && !empty($this->data['v2']['nodes'])) {
      $data['visted'] = $this->data['v2']['nodes'];
    }
    if (!in_array($this->client_id, $data['visted'])) {
      $data['visted'][] = $this->client_id;
    }

    // V2 data
    $data['v2']['id'] = services_client_get_id();
    $data['v2']['source'] = drupal_is_cli() ? 'cli' : $_SERVER['HTTP_REFERER'];
    $data['v2']['nodes'] = isset($data['v2']['nodes']) ? $data['v2']['nodes'] : array();
    if (empty($data['v2']['nodes']) && !empty($this->data['visted'])) {
      $data['v2']['nodes'] = $this->data['visted'];
    }
    if (!in_array($this->client_id, $data['v2']['nodes'])) {
      $data['v2']['nodes'][] = $this->client_id;
    }

    return $data;
  }

  /**
   * Set control dat to new object that is sent to remote site.
   *
   * @param stdClass $object
   *   Mapped object.
   */
  public function setData($object) {
    $object->_services_client = $this->getData();
    return $this;
  }
}
