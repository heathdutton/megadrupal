<?php
/**
 * @file
 * SuiteCRM Drupal integration.
 */

namespace Drupal\clients_suitecrm\Clients\Connection;
use \Drupal\clients_suitecrm\RemoteEntity\Query;

/**
 *
 * Class SuiteCrm
 * @package Drupal\clients_suitecrm\Clients\Connection
 */
class SuiteCrm extends \clients_connection_base implements \ClientsConnectionAdminUIInterface, \ClientsRemoteEntityInterface {

  /**
   * Remote methods that don't need the session id.
   *
   * @var array
   *   List of remote methods that don't need a session id.
   */
  protected $methodsWithoutSession = array(
    'login',
    'get_server_info',
  );

  /**
   * Remote methods that can be cached.
   *
   * @var array
   *   Keyed by the remote method, value is the caching duration in sec.
   */
  protected $cacheableMethods = array(
    'get_available_modules' => 3600,
    'get_module_fields' => 3600,
    'login' => 600,
  );

  /**
   * The session of the logged in user.
   */
  protected $session = NULL;

  /**
   * Stores the last request.
   */
  protected $lastRequest = NULL;

  /**
   * Stores the last response.
   */
  protected $lastResponse = NULL;

  /**
   * Stores the last error object.
   */
  protected $lastError = NULL;

  /**
   * Stores if the last call was an error.
   */
  protected $isError = FALSE;


  /**
   * {@inheritdoc}
   */
  public function __construct(array $values = array(), $entity_type = NULL) {
    parent::__construct($values, $entity_type);

    // Set the session timeout, if available, as lifetime for login calls.
    if (!empty($values['configuration']['session_timeout'])) {
      $this->cacheableMethods['login'] = $values['configuration']['session_timeout'] * 60;
    }

    return $this;
  }

  /**
   * Declare an array of properties which should be treated as credentials.
   *
   * This lets the credentials storage plugin know which configuration
   * properties to take care of.
   *
   * @return array
   *   A flat array of property names.
   */
  public function credentialsProperties() {
    return array('username', 'password');
  }

  /**
   * Extra form elements specific to a class's edit form.
   *
   * @see clients_connection_form()
   * @see clients_connection_form_submit()
   */
  public function connectionSettingsFormAlter(&$form, &$form_state) {
    // Change the endpoint element.
    $form['endpoint']['#title'] = t('SuiteCRM Service endpoint');
    $form['endpoint']['#description'] = t('SuiteCRM Service endpoint URL');

    $form['configuration']['application_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of the application'),
      '#size' => 50,
      '#maxlength' => 50,
      '#description' => t('The name of the application connecting to SuiteCRM'),
      '#required' => TRUE,
    );
    $form['credentials']['username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#size' => 50,
      '#maxlength' => 100,
      '#description' => t('The account username for connecting to SuiteCRM'),
      '#required' => TRUE,
    );
    $form['credentials']['password'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#size' => 50,
      '#maxlength' => 100,
      '#description' => t('The account password for connecting to SuiteCRM'),
    );
    $form['configuration']['read_only'] = array(
      '#type' => 'checkbox',
      '#title' => t('Read-only'),
      '#description' => t('Marks this connection read-only. This requires associated Resources to support this feature.'),
    );
    $form['configuration']['session_timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Session Timeout'),
      '#field_suffix' => t('Minutes'),
      '#description' => t('Session timeout for the session in the SuiteCRM. Tendentially its better to set a slightly lower one than defined in the CRM.'),
      '#default_value' => $this->config('session_timeout'),
    );
    $form['configuration']['timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Execution Timeout'),
      '#field_suffix' => t('Seconds'),
      '#description' => t('The execution timeout to respect. The module dynamically increases this value when the payload is bigger than 500kb.'),
      '#default_value' => $this->config('timeout'),
    );
    // The connect timeout is only supported by chr and only in a patched
    // version. For details see https://www.drupal.org/node/2613362
    if (module_exists('chr')) {
      $form['configuration']['connect_timeout'] = array(
        '#type' => 'textfield',
        '#title' => t('Connection Timeout'),
        '#field_suffix' => t('Seconds'),
        '#description' => t('The connect timeout to respect. Currently just has an effect if this patch is applied: https://www.drupal.org/node/2613362'),
        '#default_value' => $this->config('connect_timeout'),
      );
    }
  }

  /**
   * Load a remote entity.
   *
   * The resource calling this should take care of process the data returned by
   * this into a Drupal entity.
   *
   * @param string $entity_type
   *   The entity type to load.
   * @param string $remote_id
   *   The (remote) ID of the entity.
   *
   * @return Entity
   *   The remote entity data.
   */
  public function remote_entity_load($entity_type, $remote_id) {
    $query = $this->getRemoteEntityQuery('select');
    $query->base($entity_type);
    $query->entityCondition('entity_id', $remote_id);
    $result = $query->execute();

    // There's only one. Same pattern as entity_load_single().
    return current($result);
  }

  /**
   * Load multiple remote entities.
   *
   * The resource calling this should take care of process the data returned by
   * this into Drupal entities.
   *
   * @param string $entity_type
   *   The entity type to load.
   * @param array $remote_ids
   *   An array of the (remote) entity IDs.
   *
   * @return array
   *   The remote entity data. This is a numeric array in the same order as the
   *   given array of remote IDs.
   */
  public function remote_entity_load_multiple($entity_type, $remote_ids) {
    $query = $this->getRemoteEntityQuery('select');
    $query->base($entity_type);
    $query->entityCondition('entity_id', $remote_ids, 'IN');
    $result = $query->execute();

    return $result;
  }

  /**
   * Save a remote entity.
   *
   * @param string $entity_type
   *   The entity type to save.
   * @param Entity $entity
   *   The entity to save.
   *
   * @return string|NULL
   *   If the entity is being created remotely, the new remote GUID.
   */
  public function remote_entity_save($entity_type, $entity, $remote_properties = array()) {
    // Determine whether this is an insert or an update. An entity not yet
    // saved remotely will have no remote ID property.
    $action = empty($entity->remote_id) ? 'insert' : 'update';

    $query = $this->getRemoteEntityQuery($action);
    $query->setEntity($entity_type, $entity);

    if ($action == 'insert') {
      return $query->execute();
    }
    else {
      $query->addFields($remote_properties);
      return $query->execute();
    }
  }

  /**
   * Save multiple entities remotely.
   *
   * @param string $entity_type
   *   The entity type to save.
   * @param array $entities
   *   An array of entities to save in bulk, keyed by the entity ID.
   * @param array $remote_properties
   *   (Optional) An array of properties to save. Values should be names of
   *   properties which are keys in the entity info 'property map' array. Only
   *   applies when updating rather than inserting.
   *
   * @return array
   *   An array of the remote GUIDs created by the operation, keyed by the
   *   entity ID. Thus an entity which is updated rather than inserted will not
   *   be present in the array.
   */
  public function remote_entity_save_multiple($entity_type, $entities, $remote_properties = array()) {
    $query = $this->getRemoteEntityQuery('bulk_save');
    $query->setEntities($entity_type, $entities);
    $query->addFields($remote_properties);

    // Execute the query. The guids array is keyed by entity ID.
    return $query->execute();
  }

  /**
   * Provide a map of remote property types to Drupal types.
   *
   * Roughly analogous to _entity_metadata_convert_schema_type().
   *
   * @see hook_entity_property_info()
   *
   * @return array
   *   An array whose keys are remote property types as used as types for fields
   *   in hook_remote_entity_query_table_info(), and whose values are types
   *   recognized by the Entity Metadata API.
   */
  public function entity_property_type_map() {
    // @TODO: Add generic types for related entities.
    return array(
      'id' => 'text',
      'datetime' => 'text',
      'date' => 'text',
      'bool' => 'boolean',
      'name' => 'text',
      'phone' => 'text',
      'email' => 'text',
      // 'url' => 'uri',
      'url' => 'text',
      'varchar' => 'text',
      'enum' => 'text',
      'assigned_user_name' => 'text',
      'relate' => 'entity',
    );
  }

  /**
   * Provide setter and getter callbacks for specific remote property types.
   *
   * @return array
   *   An array keyed by remote property type, where each value is an array
   *   defining the callbacks for remote properties of this type. Keys in this
   *   must be 'getter callback' and 'setter callback'. The raw setter and
   *   getter callbacks may also be included.
   *
   * @see RemoteEntityAPIDefaultMetadataController::entityPropertyInfo()
   */
  public function entity_property_type_callbacks() {
    return array(
      'datetime' => array(
        // Add callbacks for raw values, which can just be the basic ones.
        'raw getter callback' => 'remote_entity_entity_mapped_property_get',
        'raw setter callback' => 'remote_entity_entity_mapped_property_set',
        'getter callback' => 'entity_property_verbatim_get',
        'setter callback' => 'entity_property_verbatim_set',
      ),
      'date' => array(
        // Add callbacks for raw values, which can just be the basic ones.
        'raw getter callback' => 'remote_entity_entity_mapped_property_get',
        'raw setter callback' => 'remote_entity_entity_mapped_property_set',
        'getter callback' => 'entity_property_verbatim_get',
        'setter callback' => 'entity_property_verbatim_set',
      ),
    );
  }

  /**
   * Get a new RemoteEntityQuery object appropriate for the connection.
   *
   * @param string $query_type
   *   (optional) The type of the query. Defaults to 'select'.
   *
   * @return RemoteEntityQuery
   *   A remote query object of the type appropriate to the query type.
   */
  public function getRemoteEntityQuery($query_type = 'select') {
    switch ($query_type) {
      case 'select':
        return new Query\SuiteCrmRestSelectQuery($this);

      case 'insert':
        return new Query\SuiteCrmRestInsertQuery($this);

      case 'update':
        return new Query\SuiteCrmRestUpdateQuery($this);

      case 'bulk_save':
        return new Query\SuiteCrmRestBulkSaveQuery($this);
    }
  }

  /**
   * Calls a remote method.
   *
   * @see \Drupal\clients_suitecrm\Clients\Connection\SuiteCrm::callMethodArray()
   *
   * @param string $name
   *   The method to call on the service.
   * @param array $arguments
   *   The parameter to pass on.
   *
   * @return mixed
   *   The response of the service.
   */
  public function __call($name, $arguments) {
    return $this->callMethodArray($name, $arguments);
  }

  /**
   * Call a remote method.
   *
   * @param string $method
   *   The name of the remote method to call.
   * @param array $method_params
   *   An array of parameters to passed to the remote method.
   *
   * @return mixed
   *   Whatever is returned from the remote site.
   */
  public function callMethodArray($method, $method_params = array()) {
    $this->isError = FALSE;
    $correlation_id = uniqid('clients_suitecrm-');

    // Automatically set the session on method that require a session id.
    if (!in_array($method, $this->methodsWithoutSession)) {
      // Prepend the session ID as it is expected to be the first parameter.
      $method_params = array_merge(array('session' => $this->getSessionId()), $method_params);
    }

    // Check if we can cache this.
    $cacheable = isset($this->cacheableMethods[$method]);
    if ($cacheable) {
      // Hash the params - but sort them before to get maximum hit rate. Copy
      // the array to ensure the order in the actual array isn't changed.
      $cid_params = $method_params;
      ksort($cid_params);
      $param_cid = md5(serialize($cid_params));
      unset($cid_params);
      $cid = 'suitecrm:' . $this->name . ':' . $method . ':' . $param_cid;
      if (($cache = cache_get($cid, 'cache_suitecrm'))) {
        return $cache->data;
      }
    }

    $this->debug(format_string('@crid Send rest_data: !rest_data', array(
      '@crid' => $correlation_id,
      '!rest_data' => print_r($method_params, TRUE),
    )));

    $post = array(
      'method'        => $method,
      'input_type'    => 'JSON',
      'response_type' => 'JSON',
      'rest_data'     => json_encode($method_params),
    );
    $options = array(
      'method' => 'POST',
      'data' => http_build_query($post),
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
        'X-CID' => $correlation_id,
      ),
    );
    $payload_bytes = drupal_strlen($options['data']);
    $options['timeout'] = $this->getExecutionTimeout($payload_bytes);
    // Only some modules like chr support this option.
    $options['connect_timeout'] = $this->config('connect_timeout');
    if ($payload_bytes > 1024) {
      // As we don't know if drupal_http_request() might uses cURL and because
      // cURL obeys the RFCs as it should. Meaning that for a HTTP/1.1 backend
      // if the POST size is above 1024 bytes cURL sends a
      // 'Expect: 100-continue' header. The server acknowledges and sends back
      // the '100' status code. cuRL then sends the request body. This is proper
      // behaviour. Nginx supports this header. This allows to work around
      // servers that do not support that header.
      // @link http://pilif.github.io/2007/02/the-return-of-except-100-continue/
      $options['headers']['Expect'] = '';
    }

    // Forward xdebug cookie if set.
    if (!empty($_COOKIE['XDEBUG_SESSION'])) {
      $options['headers']['Cookies'] = 'XDEBUG_SESSION=' . rawurlencode($_COOKIE['XDEBUG_SESSION']) . ';';
    }

    $this->debug(format_string('@crid Send request to @endpoint: !request', array(
      '@crid' => $correlation_id,
      '@endpoint' => $this->endpoint,
      '!request' => print_r($options, TRUE),
    )));

    $result = drupal_http_request($this->endpoint, $options);

    $this->debug(format_string('Result: !result', array(
      '@crid' => $correlation_id,
      '!result' => print_r($result, TRUE),
    )));

    // Check if this is a http error.
    if (!empty($result->error)) {
      $this->lastError = $result->code . ':' . $result->error;
      $this->isError = TRUE;
    }

    // @TODO Is this an error?
    if (empty($result->data)) {
      return NULL;
    }
    $response = json_decode($result->data);

    // Check if this is an suiteCRM error.
    $arr_response = (array) $response;
    $error_keys = array('name', 'number', 'description');
    if (count($arr_response) == 3 && array_keys($arr_response) == $error_keys) {
      $this->lastError = $response;
      $this->isError = TRUE;
    }

    // Cache if cacheable and no error.
    if ($cacheable && !$this->isError) {
      cache_set($cid, $response, 'cache_suitecrm', REQUEST_TIME + (int) $this->cacheableMethods[$method]);
    }

    return $response;
  }

  /**
   * Returns the calculated connection timeout.
   *
   * If a payload size is given the timeout will be calculated based on the size
   * and the given standard timeout.
   * Algorithm is per 500kb one time the default connection timeout.
   * That way large request won't run into a timeout while short requests fail
   * after an usable duration.
   * This lacks but currently is the best compromise.
   *
   * @param int $payload_size
   *   Size of the request in bytes.
   *
   * @return int
   *   The connection timeout to use.
   */
  public function getExecutionTimeout($payload_size = NULL) {
    $timeout = $this->config('timeout');
    if ($payload_size) {
      $timeout = ceil($payload_size / 500000) * $timeout;
    }
    return $timeout;
  }

  /**
   * Returns the last raw response.
   */
  public function getLastResponse() {
    return $this->lastResponse;
  }

  /**
   * Returns the information to the last request.
   */
  public function getLastRequest() {
    return $this->lastRequest;
  }

  /**
   * Returns the last error.
   *
   * @return array
   *   Returns information to the last error
   */
  public function getLastError() {
    return $this->lastError;
  }

  /**
   * Returns TRUE if the last call returned an error.
   *
   * @return bool
   *   TRUE if the last call returned an error.
   */
  public function isError() {
    return $this->isError;
  }

  /**
   * Wrapper around the configuration property to ensure sane defaults.
   *
   * If no item is given the full configuration array is returned. If the
   * given item isn't found in the configuration NULL is returned.
   *
   * @param string $item
   *   The specific configuration item to return.
   *
   * @return mixed
   *   The configuration array.
   */
  public function config($item) {
    if (!is_array($this->configuration)) {
      $this->configuration = array();
    }
    if (empty($this->configuration['username'])) {
      $this->credentialsLoad();
    }
    $config = $this->configuration + array(
      'session_timeout' => 10,
      'timeout' => 15,
      'connect_timeout' => 3,
      'application_name' => 'Drupal',
    );

    if (isset($item)) {
      if (isset($config[$item])) {
        return $config[$item];
      }
      return NULL;
    }
    return $config;
  }

  /**
   * Connect and log in to the remote service.
   */
  public function connect() {
    $cid = 'suitecrm:' . $this->name;
    $cache = cache_get($cid, 'cache_suitecrm');
    if (empty($cache->data)) {
      $this->login();
      cache_set($cid, $this->getSessionId(), 'cache_suitecrm', REQUEST_TIME + ($this->config('session_timeout') * 60));
    }
    else {
      $this->session = $cache->data;
    }
  }

  /**
   * Try to authenticate with SuiteCRM.
   *
   * @return bool
   *   State of the login.
   */
  public function login() {
    if (empty($this->session)) {
      $login_parameters = func_get_args();
      // If no arguments are given use configured values. That way this method
      // is transparent.
      if (empty($login_parameters)) {
        $login_parameters = array(
          'user_auth' => array(
            'user_name' => $this->config('username'),
            'password' => md5($this->config('password')),
            'version' => '1',
          ),
          'application_name' => $this->config('application_name'),
        );
      }
      $result = $this->callMethodArray('login', $login_parameters);
      if (empty($result->id)) {
        return FALSE;
      }
      $this->session = $result->id;
    }
    return TRUE;
  }

  /**
   * Explicitly disconnect.
   *
   * Deletes the cached session id.
   */
  public function disconnect() {
    cache_clear_all('suitecrm:' . $this->name, 'cache_suitecrm', TRUE);
  }

  /**
   * Returns the session id.
   *
   * @return string|NULL
   *   The session id.
   */
  public function getSessionId() {
    // Ensures login was called. Only executed if session is empty.
    $this->login();
    return $this->session;
  }

}
