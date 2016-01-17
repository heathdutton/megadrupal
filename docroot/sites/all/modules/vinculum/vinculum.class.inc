<?php
/**
 * @file
 * Class to store data about a received vinculum.
 */

class vinculum {
  // The node nid that the vinculum is recorded against.
  private $nid;

  // The remote URL.
  private $url;

  // The handler recording the vinculum (e.g. pingback, trackback, etc).
  private $handler;

  // The IP address of the remote site which registered the vinculum.
  private $origin_ip;

  // Timestamp when the vinculum was registered.
  private $timestamp;

  // Additional data supplied by the handler.
  private $data;

  // Boolean to show this record has already been validated.
  private $validated;

  /**
   * Constructor.
   */
  public function __construct($nid, $url, $handler) {
    // Validators.
    if (!(is_int($nid) && $nid > 0)) {
      throw new Exception('$nid should be a node nid, and must be an integer.');
    }
    if (!is_string($url)) {
      throw new Exception('$url should be a string with the URL of the remote page.');
    }
    if (!is_string($handler)) {
      throw new Exception('$handler must be a string.  Provide the name of the handler: e.g. pingback, trackback, etc');
    }

    $this->nid = $nid;
    $this->url = $url;
    $this->handler = $handler;

    // Set defaults for the optional params.
    $this->origin_ip = ip_address();
    $this->timestamp = time();
    $this->data = NULL;
  }

  /**
   * Setter for data, origin_ip and timestamp.
   */
  public function __set($key, $value) {
    switch ($key) {
      case 'data':
      case 'origin_ip':
        $this->$key = $value;
        break;

      case 'timestamp':
        if (!is_int($value)) {
          throw new Exception('Timestamp must be a unix timestamp (and the value must be an integer).');
        }
        $this->timestamp = $value;

      default:
        throw new Exception("{$key} is not supported. You can set data, origin_ip, and timestamp.");
    }
  }

  /**
   * Getters.
   */
  public function __get($key) {
    switch ($key) {
      case 'nid':
      case 'url':
      case 'handler':
      case 'data':
      case 'origin_ip':
      case 'timestamp':
      case 'validated':
        return $this->$key;

      default:
        throw new Exception("{$key} is not supported.");
    }
  }

  /**
   * Call if the vinculum has been successfully validated.
   */
  public function isValid() {
    $this->validated = TRUE;
  }
}
