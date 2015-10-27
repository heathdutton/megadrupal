<?php
/**
 * @file
 * Drupal RestMini Service Example module.
 */


/**
 * An instance represents an Example Thingie, whereas the static methods implement REST operations.
 */
class RestMiniServiceExampleThingie {

  /**
   * @var integer
   */
  public $id;

  /**
   * Unique.
   *
   * @see restmini_service_example_schema()
   *
   * @var string
   */
  public $name;

  /**
   * @var string
   */
  public $color = 'transparent';

  /**
   * @var integer
   */
  public $created;

  /**
   * @var integer
   */
  public $changed;


  /**
   * @param array|NULL $properties
   */
  public function __construct($properties = NULL) {
    if ($properties) {
      foreach ($properties as $key => $val) {
        $this->{$key} = $val;
      }
    }
  }


  /**
   * Lists exposed - from outside settable - properties; values indicating whether a value for the property is required.
   *
   * @var array
   */
  protected static $exposedProperties = array(
    'name' => TRUE,
    'color' => FALSE
  );

  /**
   * RESTmini service endpoint.
   *
   * @see RestMiniService::router()
   * @see restmini_service_respond()
   * @see restmini_service_error()
   *
   * @param mixed $anyNumberOfArgs,...
   *
   * @return mixed
   *   Won't return anything, but eventually exit, in cases where the response is handled
   *   by restmini_service_respond() or restmini_service_error(), instead RestMiniService::router().
   */
  public static function endpoint($anyNumberOfArgs = NULL) {
    $method = $_SERVER['REQUEST_METHOD'];
    try {
      switch ($method) {

        case 'GET':
        case 'HEAD':
          // Retrieve operation requires one path arg.
          if (($id = $anyNumberOfArgs)) {
            // We could handle response ourselves:
            //restmini_service_respond(static::rerieve($id), 200);
            // Or simply let RESTmini's router handle things for us (it knows that GET|HEAD + payload means 200):
            return static::rerieve($id);
          }
          // Index operation.
          else {
            // We could handle response ourselves:
            //restmini_service_respond(static::index(), 200);
            // Or simply let RESTmini's router handle things for us (it knows that GET|HEAD + payload means 200):
            return static::index();
          }
          break;

        case 'POST':
          // Lets's do automatic validation, according to the parameters defined in restmini_service_example_restmini_service().

          // Import and validate path|get|post arguments.
          $validationFailures = array();
          // Get all values in shallow unified array, and do validate all parameters (continue on failure).
          $arguments = KKRestMiniService::importArguments($validationFailures, TRUE, TRUE);

          if ($validationFailures) {
            // Tell requestor that it was a 'Bad Request', and be a pall and tell which argument(s) failed validation.
            restmini_service_respond(NULL, 400, join(' ', $validationFailures));
            exit;
          }

          // We could handle response ourselves:
          //restmini_service_respond(static::create($properties), 201);
          // Or simply let RESTmini's router handle things for us (it knows that POST + payload means 201):
          return (int)static::create($arguments);
          break;

        case 'PUT':
          // Import and validate path|get|post arguments.
          $validationFailures = array();
          // Don't get the values in a shallow unified array, but do validate all parameters (continue on failure).
          $arguments = KKRestMiniService::importArguments($validationFailures, FALSE, TRUE);

          if ($validationFailures) {
            // $validationFailures is always a unified array (no path/get/post sub arrays).
            restmini_service_respond(NULL, 400, join(' ', $validationFailures));
            exit;
          }

          // update() throws 404 exception if thingie by the ID doesn't exit.
          // The service router() will pick up that exception, and send a 404 Not Found.
          static::update($arguments['path']['id'], $arguments['post']);

          // Our PUT has nothing to report to the requestor, except that things worked fine, so we send a 204 No Content.
          //restmini_service_respond(NULL, 204);
          // Well, RESTmini's router knows how to handle things for us (that PUT + no payload means 204):
          return NULL;
          break;

        case 'DELETE':
          // Do validation slightly different, less automated.
          $pattern = RestMiniService::validationPattern('integer_positive', array(
            'except_value' => '*'
          ));

          $path_args = func_get_args();

          if (!RestMiniService::validate($pattern, $path_args, 0)) {
            $validationFailures = RestMiniService::validationFailures();
            RestMiniService::respond(NULL, 400, join(' ', $validationFailures));
          }

          // delete throws 404 exception if thingie by the ID doesn't exit.
          static::delete($path_args[0]);

          // DELETE has nothing to report to the requestor, except that things worked fine,
          // so we let RESTmini's router send a 204 No Content.
          return NULL;
          break;

        default:
          // Method Not Allowed.
          // RestMiniService::router() will actually handle this for us
          // - because router() only supports GET|HEAD|POST|PUT|DELETE, and checks if current method is one of these -
          // so this is just to examplify.
          restmini_service_error(NULL, 405);
      }
    }
    catch (PDOException $xc) {
      // A database error is not something we want to be specific about, but we'll certainly log the error.
      restmini_service_error($xc, 500);
    }
    catch (Exception $xc) {
      // Some of our operations may throw exceptions for Bad Request and Not Found.
      $code = $xc->getCode();
      switch ($code) {
        case 400: // Bad Request.
          // We pass error message to response message, so that requestor may figure out what's wrong.
          // Of course, we cannot send an exception message unless we're pretty sure that it doesn't reveal any secrets;
          // and we are that, in this case.
          restmini_service_respond(NULL, 400, $xc->getMessage());
          break;
        case 404: // Not Found.
          restmini_service_respond(NULL, 404);
          break;
        default:
          // Something went seriously wrong.
          restmini_service_error($xc, 500);
      }
    }
  }


  /**
   * @throws PDOException
   *   Propagated.
   *
   * @return array
   */
  public static function index() {
    $list = array();
    if (($instances = db_select('restmini_service_example_thingie', 'tbl', array('fetch' => PDO::FETCH_ASSOC))
      ->fields('tbl') // ~ all fields.
      ->execute()->fetchAll())
    ) {
      foreach ($instances as $properties) {
        $list[] = new static($properties);
      }
    }
    return $list;
  }

  /**
   * @throws Exception
   *   On no instance found by that $id.
   * @throws PDOException
   *   Propagated.
   *
   * @see RestMiniServiceExampleThingie::index()
   *
   * @param integer $id
   *
   * @return RestMiniServiceExampleThingie
   */
  public static function rerieve($id = NULL) {
    if (($properties = db_select('restmini_service_example_thingie', 'tbl')
      ->fields('tbl') // ~ all fields.
      ->condition('id', $id)
      ->execute()->fetchAssoc())
    ) {
      return new static($properties);
    }
    // Use HTTP status code as error code.
    throw new Exception('Not Found', 404);
  }

  /**
   * @throws Exception
   *   On missing required property.
   *   On duplicate key (name).
   * @throws PDOException
   *   Propagated.
   *
   * @param array $properties
   *
   * @return integer
   *   ID of the new instance.
   */
  public static function create(array $properties) {
    $o = new static();
    try {
      $exposedProperties =& static::$exposedProperties;
      $missing = array();
      foreach ($exposedProperties as $key => $required) {
        if (array_key_exists($key, $properties)) {
          $o->{$key} = $properties[$key];
        }
        elseif ($required) {
          $missing[] = $key;
        }
      }
      if ($missing) {
        throw new Exception('Missing required properties[' . join(', ', $missing) . '].', 400);
      }
      $o->created = $o->changed = REQUEST_TIME;

      // db_insert...execute() returns last insert ID.
      return db_insert('restmini_service_example_thingie')
        ->fields((array) $o)
        ->execute();
    }
    catch (PDOException $xc) {
      // Fair chance that it's a duplicate key error, though 23000 may also (MySQL) mean null error.
      if ((int)$xc->getCode() == 23000) {
        // Duplicate key.
        throw new Exception('Example thingie name[' . $o->name . '] already exists.', 400);
      }
      else {
        throw $xc;
      }
    }
  }

  /**
   * Only allows updating of exposed properties.
   * 
   * @see RestMiniServiceExampleThingie::$exposedProperties
   *
   * @throws Exception
   *   On no instance found by that $id.
   *   On duplicate key (name).
   * @throws PDOException
   *   Propagated.
   *
   * @param integer $id
   * @param array $properties
   */
  public static function update($id, array $properties) {
    try {
      $exposedKeys = array_keys(static::$exposedProperties);
      $fields = array(
        'changed' => REQUEST_TIME,
      );
      foreach ($exposedKeys as $key) {
        if (array_key_exists($key, $properties)) {
          $fields[$key] = $properties[$key];
        }
      }

      // db_update...execute() returns number of updates.
      $count = db_update('restmini_service_example_thingie')
        ->fields($fields)
        ->condition('id', $id)
        ->execute();

      if (!$count) {
        // Use HTTP status code as error code.
        throw new Exception('Not Found', 404);
      }
    }
    catch (PDOException $xc) {
      // Fair chance that it's a duplicate key error, though 23000 may also (MySQL) mean null error.
      if ((int)$xc->getCode() == 23000 && isset($fields['name'])) {
        // Duplicate key.
        throw new Exception('Example thingie name[' . $fields['name'] . '] already exists.');
      }
      else {
        throw $xc;
      }
    }
  }

  /**
   * @throws Exception
   *   On no instance found by that $id.
   * @throws PDOException
   *   Propagated.
   *
   * @param integer|string $id
   *   '*' means delete all instances.
   */
  public static function delete($id) {
    // db_delete...execute() returns number of deletes.
    $query = db_delete('restmini_service_example_thingie');
    if ($id !== '*') {
      $query->condition('id', $id);
    }
    $count = $query->execute();

    if (!$count) {
      // Use HTTP status code as error code.
      throw new Exception('Not Found', 404);
    }
  }

}
