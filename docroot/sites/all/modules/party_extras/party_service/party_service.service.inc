<?php
/**
 * @file
 * The main page callback for managing requests.
 */

/**
 * Main page callabck for party_service requests.
 *
 * @param Entity $service
 *   The service being accessed (some sites may expose more than one.).
 * @param Party $party (optional)
 *   The party being acted on.
 */
function party_service_callback($service, $party = NULL) {
  // Make sure the party didn't fail to load.
  if ($party === FALSE) {
    return MENU_NOT_FOUND;
  }

  // Get the operations.
  $op = $_SERVER['REQUEST_METHOD'];
  $return_type = $_SERVER['HTTP_ACCEPT'];
  //$api_key = $_SERVER['HTTP_X_API_KEY'];
  $content = file_get_contents('php://input');

  // Get the API Key.
  $api_key = $_SERVER['HTTP_X_API_KEY'];
  $key = entity_load_single('party_service_key', $api_key);

  // Prepare a context array.
  $context = array(
    'op' => $op,
    'return_format' => $return_type,
    'apikey' => $key,
  );

  $handler = PartyServiceHandlerBase::create($service, $context);

  // Check that we support the method.
  if (!$handler->checkMethod($op)) {
    return PARTY_SERVICE_METHOD_NOT_ALLOWED;
  }

  // Carry out the request.
  if (empty($party)) {
    switch ($op) {
      case 'GET':
        return $handler->getAll();
        break;
      case 'POST':
        return $handler->post($content);
        break;
      case 'OPTIONS':
        return $handler->options();
        break;
    }
  }
  else {
    switch ($op) {
      case 'GET':
        return $handler->get($party);
        break;
      case 'PUT':
        return $handler->put($party, $content);
        break;
      case 'PATCH':
        return $handler->patch($party, $content);
        break;
      case 'DELETE':
        return $handler->delete($party);
        break;
    }
  }
}

interface PartyServiceHandlerInterface {

  /**
   * Check that the service accepts the method.
   *
   * @param string $method
   *   The HTTP Method used.
   *
   * @return boolean
   *   True if the method is supported. False otherwise.
   */
  public function checkMethod($method);

  /**
   * Get a list of all parties involved in this service.
   */
  public function getAll();

  /**
   * Save a new party.
   *
   * @param $content
   *   The content of the request.
   */
  public function post($content);

  /**
   * Retrieve options about the service.
   */
  public function options();

  /**
   * Get a particular party in the right format.
   *
   * @param Party $party
   *   The party to return.
   */
  public function get($party);

  /**
   * Override a party with a new version.
   *
   * @param Party $party
   * @param $content
   */
  public function put($party, $content);

  /**
   * Override specific values.
   *
   * @param Party $party
   * @param $content
   */
  public function patch($party, $content);

  /**
   * Delete a party
   *
   * @param Party $party
   */
  public function delete($party);
}

class PartyServiceHandlerBase implements PartyServiceHandlerInterface {

  /**
   * The service config entity.
   */
  protected $service = NULL;

  /**
   * Get a new handler instance.
   *
   * @param Entity $service
   *   The service configuation entity.
   * @param array $context
   *   And array of contextual info.
   */
  public static function create($service, $context = array()) {
    return new PartyServiceHandlerBase($service, $context);
  }

  /**
   * Construct a new handler.
   */
  public function __construct($service, $context = array()) {
    $this->service = $service;
    $this->context = $context;
  }

  /**
   * {@inheritdoc}
   */
  public function checkMethod($method) {
    $supported_methods = array('POST');
    return in_array($method, $supported_methods);
  }

  /**
   * {@inheritdoc}
   */
  public function getAll() {}

  /**
   * {@inheritdoc}
   */
  public function get($party) {}

  /**
   * {@inheritdoc}
   */
  public function options() {}

  /**
   * {@inheritdoc}
   */
  public function post($content) {
    $input = json_decode($content);
    if (!empty($input->uuid) && party_uuid_load($input->uuid)) {
      return array(
        '#status' => 403,
        '#content' => 'A record with that UUID already exists',
      );
    }

    $party = entity_create('party', array());
    PartyServiceFormatterBase::createFromEntity($this->service)
      ->convertRecordToParty($input, $party);

    $party->save();
    foreach ($party->data_set_controllers as $controller) {
      $controller->save(TRUE);
    }

    // Save the party to the map.
    db_merge('party_service_map')
      ->key(array('party' => $party->uuid))
      ->fields(array(
        'apikey' => $this->context['apikey']->id,
      ))
      ->execute();

    $this->invoke('post', $party, $input);

    $output = array(
      '#status' => 201,
      '#headers' => array(
        'Location' => 'path/to/resource',
      ),
      '#content' => 'Success',
    );
    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function put($party, $content) {}

  /**
   * {@inheritdoc}
   */
  public function patch($party, $content) {}

  /**
   * {@inheritdoc}
   */
  public function delete($party) {}

  /**
   * Invoke hooks.
   *
   * @param $hook
   * @param Party $party
   * @param $input
   */
  protected function invoke($hook, $party, $input = FALSE) {
    $hook = 'party_service_'.$hook;
    if (module_exists('rules')) {
      if ($input) {
        rules_invoke_all($hook, $this->service, $party, $input);
      }
      else {
        rules_invoke_all($hook, $this->service, $party);
      }
    }
    else {
      if ($input) {
        module_invoke_all($hook, $this->service, $party, $input);
      }
      else {
        module_invoke_all($hook, $this->service, $party);
      }
    }
  }

}
