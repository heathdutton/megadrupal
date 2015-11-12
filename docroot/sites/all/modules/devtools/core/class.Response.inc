<?php
namespace devtools;

class Response extends \devtools\Base  {
  protected $_response = '';
  protected $_parsed_response = array();
  protected $_handler = FALSE;
  protected $_errors = array();

  /**
   * Constructor
   */
  public function __construct(&$handler, $response) {
    $this->_handler = $handler;
    $this->_response = $response;

    // check for errors
    if (curl_errno($handler)) {
      $this->_errors += explode("\n", curl_error($handler));
    }

    curl_close($handler);
  }

  /*--------------------------------------------------------------------------
      Getters & setters
  --------------------------------------------------------------------------*/  

  // errors
  public function get_errors() {
    return $this->_errors;
  }

  /*--------------------------------------------------------------------------
      Response processing
  --------------------------------------------------------------------------*/

  /**
   * Parses response data.
   * @return [type] [description]
   */
  public function parse() {
    if ($this->_errors) {
      return FALSE;
    }

    parse_str($this->_response, $this->_parsed_response);

    return $this->_parsed_response;
  }

  /**
   * Returns the raw response.
   * @return [type] [description]
   */
  public function raw() {
    return $this->_response;
  }

  /**
   * Displays error messages.
   * @return [type] [description]
   */
  function display_errors() {
    foreach ($this->get_errors() as $error) {
      drupal_set_message(t($error), 'error');
    }
  }
}