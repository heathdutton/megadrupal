<?php
/**
 * @file
 * NCIP Message object
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class NCIPMessage {
  const NCIP_SELF_SERVICE_MESSAGE = 0x0000;
  const NCIP_INITIATION_MESSAGE = 0x0001;
  const NCIP_RESPONSE_MESSAGE = 0x0002;

  const NCIP_NAMESPACE_URI = 'http://www.niso.org/ncip';
  const NCIP2_NAMESPACE_URI = 'http://www.niso.org/2008/ncip';

  // General information
  private $type;
  private $connection;
  private $version;
  private $dtd;
  private $xsd;
  private $namespace_prefix;
  private $namespace_uri;

  public $sent = FALSE;
  public $received = FALSE;

  // Message structure
  private $ext;
  private $service;
  private $header;
  public $message;

  /**
   * Load message from an XML string
   *
   * @param $connection
   *    Connection where message originated or should be directed
   * @param $xml
   *    XML string
   * @return
   *    NCIPMessage object
   */
  public static function from_xml($xml, $connection = NULL) {
    $obj = @simplexml_load_string((string) $xml);
    return $obj instanceof SimpleXMLElement
           ? NCIPMessage::from_simplexml($obj, $connection)
           : new NCIPMessage($connection, NULL, FALSE, NCIPMessage::NCIP_SELF_SERVICE_MESSAGE, NULL, NULL, NULL, NULL, "XML Parsing Failure. Empty Message Returned.");
  }

  /**
   * Load message from a DOM XML object
   *
   * @param $connection
   *    Connection where message originated or should be directed
   * @param $dom
   *    XML DOM object
   * @return
   *    NCIPMessage object
   */
  public static function from_dom(DOMDocument $dom, $connection = NULL) {
    $obj = simplexml_import_dom($dom);
    return $obj instanceof SimpleXMLElement
           ? NCIPMessage::from_simplexml($obj, $connection)
           : new NCIPMessage($connection, NULL, FALSE, NCIPMessage::NCIP_SELF_SERVICE_MESSAGE, NULL, NULL, NULL, NULL, "DOM Parsing Failure. Empty Message Returned.");
  }

  /**
   * Load message from a SimpleXML object
   *
   * @param $connection
   *    Connection where message originated or should be directed
   * @param $dom
   *    XML DOM object
   * @return
   *    NCIPMessage object
   */
  public static function from_simplexml(SimpleXMLElement $obj, $connection = NULL) {
    // attempt to find the correct ncip namespace if there is one
    foreach ($obj->getDocNamespaces() as $pf => $ns) {
      if ($ns == NCIPMessage::NCIP_NAMESPACE_URI || $ns == NCIPMessage::NCIP2_NAMESPACE_URI) {
        $namespace_prefix = $pf;
        $namespace_uri = $ns;
        break;
      }
      elseif (strpos($ns, '/ncip')) {
        $namespace_prefix = $pf;
        $namespace_uri = $ns;
      }
    }

    // reload with namespace, if applicable
    if ($namespace_prefix) {
      $obj = new SimpleXMLElement($obj->asXML(), NULL, FALSE, $namespace_uri);
    }

    $root = $obj->children($namespace_prefix ? $namespace_uri : NULL);
    $ext = $root->getName() == 'Ext' ? TRUE : FALSE;
    $service = $ext
      ? $root->children($namespace_prefix ? $namespace_uri : NULL)->getName()
      : $root->getName();
    $message = $obj;
    $type = NULL;

    $obj_attr = reset($obj->attributes($namespace_prefix ? $namespace_uri : NULL));
    if (preg_match('/ncip_v([\d_]+)\.(dtd|xsd)/', $obj_attr['version'], $matches)) {
      $version = (float) str_replace('_', '.', $matches[1]);
    }

    return new NCIPMessage($connection, $service, $ext, $type, $message, $version,
                           $namespace_prefix, $namespace_uri);
  }

  /**
   * NCIPMessage constructor
   *
   * @param $connection
   *    Connection where message should originate
   * @param $service
   *    NCIP service, which is the root element of the children of Ext element
   * @param $ext
   *    Whether or not the parent element is Ext
   * @param $type
   * @param $message
   *    Message array structure
   * @param $version
   * @param $namespace_prefix
   * @param $namespace_uri
   * @param $problem
   */
  public function __construct($connection, $service = NULL, $ext = FALSE,
      $type = NCIPMessage::NCIP_SELF_SERVICE_MESSAGE, $message = NULL,
      $version = NULL, $namespace_prefix = NULL, $namespace_uri = NULL,
      $problem = NULL) {

    $this->connection = $connection;
    $this->service = $service;
    $this->ext = $ext;
    $this->type = $type;
    $this->message = $message;
    $this->version = $version;
    $this->namespace_prefix = $namespace_prefix;
    $this->namespace_uri = $namespace_uri;
    if ($problem) {
      $this->set_problem($problem);
    }

    // Use connection or service name to determine type and version
    if ($this->connection instanceof NCIPConnection) {
      if (!$this->type) {
        // Determine type and header from NCIP connection
        switch ($connection->get_type()) {
          case NCIPConnection::NCIP_RESPONDING_CONNECTION:
            $this->type = NCIPMessage::NCIP_RESPONSE_MESSAGE;
            break;
          case NCIPConnection::NCIP_INITIATING_CONNECTION:
            $this->type = NCIPMessage::NCIP_INITIATION_MESSAGE;
            break;
          case NCIPConnection::NCIP_SELF_SERVICE_CONNECTION:
          default:
            $this->type = NCIPMessage::NCIP_SELF_SERVICE_MESSAGE;
            return; // no header information to add
            break;
        }
      }
      if (!$this->version) {
        $this->version = $this->connection->get_application()->get_version();
      }
    }
    elseif ($this->service) {
      if ((strrpos($this->service, 'Response') + 8) == strlen($this->service)) {
        $this->type = NCIPMessage::NCIP_RESPONSE_MESSAGE;
      }
      elseif (!$this->message) {
        $this->type = NCIPMessage::NCIP_INITIATION_MESSAGE;
      }
    }
    else {
      $this->version = 2;
    }

    // Determine DTD, XSD, and Namespace URI
    switch ($this->version) {
      case 1:
      case 1.01:
        $this->dtd = url("ncip/schemas/ncip_v" . number_format($this->version, 1, '_', '_') . ".dtd", array('absolute' => TRUE));
        $this->xsd = url("ncip/schemas/ncip_v" . number_format($this->version, 1, '_', '_') . ".xsd", array('absolute' => TRUE));
        $this->namespace_uri = null;
        // $this->namespace_uri = $this->namespace_uri ? $this->namespace_uri : NCIPMessage::NCIP_NAMESPACE_URI;
        break;
      case 2:
      case 2.01:
      default:
        $version_string = number_format($this->version, 1, '_', '_');
        // http://www.niso.org/schemas/ncip/v2_02/ncip_v2_02.xsd
        $this->xsd = url("http://www.niso.org/schemas/ncip/v" . $version_string . '/ncip_v' . $version_string . '.xsd', array('absolute' => TRUE));
        // $this->xsd = url("ncip/schemas/ncip_v" . number_format($this->version, 1, '_', '_') . ".xsd", array('absolute' => TRUE));
        $this->namespace_uri = $this->namespace_uri ? $this->namespace_uri : NCIPMessage::NCIP2_NAMESPACE_URI;
        break;
    }

    // Hack to inject namespace URI for version 2
    if (((int) $this->version == 2) && (!$this->namespace_prefix)) {
      $this->namespace_prefix = 'ns1';
    }

    // Create a new blank message
    if (!isset($this->message) || !($this->message instanceof SimpleXMLElement)) {
      $this->init_message();
      $this->init_message_headers();
    }

    // Check for and report problems
    if ($this->has_problem()) {
      xc_log_info('ncip problem', htmlspecialchars($this->get_problem()));
    }
  }

  /**
   * Creates a new message
   */
  private function init_message() {
    switch ($this->version) {
      case 1:
      case 1.01:
        // no namespace, but doctype required. use dtd
        $xml = '<!DOCTYPE NCIPMessage PUBLIC "-//NISO//NCIP DTD Version 1//EN" "http://www.niso.org/ncip/v1_0/imp1/dtd/ncip_v1_0.dtd">' . "\n" .
               "<NCIPMessage version=\"{$this->dtd}\"/>";
        break;
      case 2:
      case 2.01:
      default:
        // namespace possible, but no doctype. use xsd
        if ($this->namespace_prefix) {
          $ns = $this->namespace_prefix . ':';
          $xmlns = $this->namespace_uri ? "xmlns:$this->namespace_prefix=\"$this->namespace_uri\"" : "";
        }
        else {
          $ns = "";
          $xmlns = $this->namespace_uri ? "xmlns=\"$this->namespace_uri\"" : "";
        }
        $xml = "<{$ns}NCIPMessage {$xmlns} {$ns}version=\"{$this->xsd}\"/>";
        break;
    }

    try {
      $this->message = new SimpleXMLElement($xml, NULL, FALSE, $this->namespace_uri ? $this->namespace_uri : NULL);
    }
    catch (Exception $e) {
      xc_log_info('ncip parsing error', htmlspecialchars($this->message, TRUE));
    }
  }

  /**
   * Initializes message by adding header and version attribute information
   */
  private function init_message_headers() {
    $connection = $this->connection;
    if (!$connection) {
      return FALSE;
    }

    $application = $this->connection->get_application();
    if (!$application) {
      return FALSE;
    }

    // Gather data elements for header
    $from_agency_id = $application->get_from_agency_id();
    $from_agency_authentication = $application->get_from_agency_authentication();
    $from_system_id = $application->get_from_system_id();
    $from_system_authentication = $application->get_from_system_authentication();
    $to_system_id = $connection->get_to_system_id();
    $to_agency_id = $connection->get_to_agency_id();

    if ($from_agency_id['value'] || $from_system_id['value'] || $to_agency_id['value'] || $to_system_id['value']
         || $from_agency_authentication || $from_system_authentication) {

      // Determine version from connection
      switch ($this->version) {
        case 1:
        case 1.01:
          $unique = 'Unique';
          break;
        case 2:
        case 2.01:
        default:
          $unique = '';
          break;
      }

      // Determine header from NCIP connection
      switch ($this->type) {
        case NCIPMessage::NCIP_RESPONSE_MESSAGE:
          $this->header = 'ResponseHeader';
          break;
        case NCIPMessage::NCIP_INITIATION_MESSAGE:
          $this->header = 'InitiationHeader';
          break;
        case NCIPMessage::NCIP_SELF_SERVICE_MESSAGE:
        default:
          $this->header = NULL;
          return; // no header information to add
          break;
      }

      // Insert header element into message
      if ($this->service != 'LookupUser' && $this->service != 'LookupItemSet') {
        $this->insert_element($this->header);
      }

      if ($this->type == NCIPMessage::NCIP_INITIATION_MESSAGE) {
        $on_behalf_of_agency = $application->get_on_behalf_of_agency();
        $application_profile_type = $application->get_application_profile_type();
      }

      switch ($this->version) {
        case 1:
        case 1.01:
          $this->insert_from_agency_id($from_agency_id, $unique);
          $this->insert_to_agency_id($to_agency_id, $unique);
          $this->insert_from_system_id($from_system_id);
          $this->insert_to_system_id($to_system_id);
          break;
        case 2:
        case 2.01:
        default:
          if ($this->service != 'LookupUser' && $this->service != 'LookupItemSet') {
            $this->insert_from_system_id($from_system_id);
            $this->insert_from_agency_id($from_agency_id, $unique);
            $this->insert_to_system_id($to_system_id);
            $this->insert_to_agency_id($to_agency_id, $unique);
          }
          break;
      }

      // Insert FromAgencyAuthentication
      if (!empty($from_agency_authentication)) {
        $this->insert_string(
          array($this->header, 'FromAgencyAuthentication'),
          $from_agency_authentication);
      }

      // Insert FromSystemAuthentication
      if (!empty($from_system_authentication)) {
        $this->insert_string(
          array($this->header, 'FromSystemAuthentication'),
          $from_system_authentication);
      }

      // Insert OnBehalfOfAgency
      if (!empty($on_behalf_of_agency['scheme']) && !empty($on_behalf_of_agency['value'])) {
        $this->insert_scheme(
          array($this->header, 'OnBehalfOfAgency', $unique . 'AgencyId'),
          $on_behalf_of_agency['value'],
          $on_behalf_of_agency['scheme']
        );
      }

      // Insert ApplicationProfileType
      if (!empty($application_profile_type['scheme']) && !empty($application_profile_type['value'])) {
        $this->insert_scheme(
          array($this->header, 'ApplicationProfileType'),
          $to_system_id['value'],
          $to_system_id['scheme']
        );
      }
    }
  }

  // Insert FromAgencyId
  function insert_from_agency_id($from_agency_id, $unique) {
    if (!empty($from_agency_id['scheme']) && !empty($from_agency_id['value'])) {
      $this->insert_scheme(
        array($this->header, 'FromAgencyId', $unique . 'AgencyId'),
        $from_agency_id['value'],
        $from_agency_id['scheme']
      );
    }
  }

  // Insert ToAgencyId
  function insert_to_agency_id($to_agency_id, $unique) {
    if (!empty($to_agency_id['scheme']) && !empty($to_agency_id['value'])) {
      $this->insert_scheme(
        array($this->header, 'ToAgencyId', $unique . 'AgencyId'),
        $to_agency_id['value'],
        $to_agency_id['scheme']
      );
    }
  }

  // Insert FromSystemId
  function insert_from_system_id($from_system_id) {
    if (!empty($from_system_id['scheme']) && !empty($from_system_id['value'])) {
      $this->insert_scheme(
        array($this->header, 'FromSystemId'),
        $from_system_id['value'],
        $from_system_id['scheme']
      );
    }
  }

  // Insert ToSystemId
  function insert_to_system_id($to_system_id) {
    if (!empty($to_system_id['scheme']) && !empty($to_system_id['value'])) {
      $this->insert_scheme(
        array($this->header, 'ToSystemId'),
        $to_system_id['value'],
        $to_system_id['scheme']
      );
    }
  }

  /**
   * Check whether there is a problem with the NCIP message
   */
  public function has_problem() {
    return isset($this->message->{$this->service}->Problem) ? TRUE : FALSE;
  }

  public function set_problem($text, $element = NULL) {
    $text = (string) $text;

    switch ($this->version) {
      case 1:
      case 1.01:
        $element
          ? $element->ErrorMessage = $text
          : $this->message->{$this->service}->Problem->ErrorMessage = $text;
        break;
      case 2:
      case 2.01:
      default:
        if (isset($element)) {
          $element->ProblemDetail = $text;
        }
        elseif (isset($this->message->{$this->service}->Problem)) {
          $this->message->{$this->service}->Problem->ProblemDetail = $text;
        }
        break;
    }
  }

  public function get_problem($element = NULL) {
    if ($element || $this->message->{$this->service}->Problem) {
      switch ($this->version) {
        case 1:
        case 1.01:
          return $element
                  ? (string) $element->ErrorMessage
                  : (string) $this->message->{$this->service}->Problem->ErrorMessage;
          break;
        case 2:
        case 2.01:
        default:
          return $element
                  ? (string) $element->ProblemDetail . ': ' . (string) $element->ProblemValue
                  : (string) $this->message->{$this->service}->Problem->ProblemDetail;
          break;
      }
    }
  }

  /** Getters and setters **/
  public function set_type($type) {
    if (empty($type)) {
      $this->type = $type;
    }
  }

  public function set_xml_structure($xml_structure) {
    if (empty($this->xml_structure)) {
      $this->xml_structure = $xml_structure;
    }
  }

  public function get_xml_structure() {
    return $this->xml_structure;
  }

  public function get_version() {
    return $this->version;
  }

  public function get_service() {
    return $this->service;
  }

  public function has_ext() {
    return $this->ext;
  }

  /**
   * Insert a data element (and, optionally, its value) into the NCIP message
   *
   * This function will call one of the following functions, depending on the
   * type of element in at the last position in the $path:
   *  - insert_boolean($path, $boolean)
   *  - insert_datetime($path, $datetime)
   *  - insert_integer($path, $integer)
   *  - insret_non_negative_integer($path, $non_negative_integer)
   *  - insert_positive_integer($path, $positive_integer)
   *  - insert_string($path, $string)
   *  - insert_scheme($path, $scheme, $value)
   *
   * See each respective function for more information.
   *
   * @param $path
   *    An array representing the path to the element, wihtout including the
   *    service element for example:
   *    array('FirstChildUnderService', 'ChildOfFirstChild');
   *
   *    A good example is with the init_message() function which inserts
   *    elements for the header using this array to insert the FromAgencyId
   *    data element:
   *    array('InitiationHeader', 'FromAgencyId')
   *
   *    In some cases, there may be more than one element with the same node
   *    name at a certain level in the path. To distinglish between them, use
   *    the following format:
   *    array(
   *      array('name' => 'SecondRepeatingChild', 'delta' => 1),
   *      'ChildOfFirstRepeatingChild',
   *      array(
   *        'name' => 'FirstRepeatingChildOfChildOfSecondRepeatingChild',
   *        'delta' => 0
   *      )
   *    );
   *
   * @param $value
   *    The value to be placed in that element. If it is a scheme, this should
   *    be an array in the form of: array('scheme' => $scheme, 'value' => $value)
   * @return
   *    The element with value
   */
  public function insert($path, $value = NULL) {
    $path = is_array($path) ? $path : func_get_args();

    // Get data element from application
    $application = $this->connection->get_application();
    $data_elements = $application->get_data_elements();
    $data_element = $data_elements[key(end($path))];

    // Based on the type of NCIP element call the right function
    switch ($data_element['type']) {
      case NCIP_DATETIME:
        return $this->insert_datetime($path, $value);
        break;
      case NCIP_INTEGER:
        return $this->insert_integer($path, $value);
        break;
      case NCIP_NON_NEGATIVE_INTEGER:
        return $this->insert_non_negative_integer($path, $value);
        break;
      case NCIP_POSITIVE_INTEGER:
        return $this->insert_positive_integer($path, $value);
        break;
      case NCIP_STRING:
        return $this->insert_string($path, $value);
        break;
      case NCIP_OPEN_SCHEME:
      case NCIP_CLOSED_SCHEME:
        return $this->insert_scheme($path, $value['value'], $value['scheme']);
        break;
      case NCIP_COMPLEX_ELEMENT:
      case NCIP_EMPTY_ELEMENT:
      default:
        return $this->insert_element($path);
        break;
    }
  }

  /**
   * Create an heirarchial path to an element within the NCIP message.
   *
   * A depth level is created as the function iterates through each element
   * in the $path array.
   *
   * @param $path
   *    An array representing the ath to the element, for example:
   *    array('FirstChild', 'ChildOfFirstChild')
   * @return
   *    A reference to the last element in the path
   */
  public function insert_element($path, $value = NULL) {
    $path = is_array($path) ? $path : array($path);
    if ($this->ext) {
      $node = &$this->message->Ext->{$this->service};
    }
    else {
      $node = &$this->message->{$this->service};
    }

    // Create all elements, if necessary and insert value
    $last_key = end(array_keys($path));
    foreach ($path as $key => $element) {
      $delta = 0;
      $attributes = array();
      if (is_array($element)) {
        if (isset($element['delta'])) {
          $delta = (int) $element['delta'];
        }
        if (isset($element['attributes'])) {
          $attributes = (array) $element['attributes'];
        }
        $element = (string) $element['name'];
      }
      if ($delta && !isset($node->{$element}[$delta])) {
        $node->{$element}[$delta] = NULL;
      }
      if ($key == $last_key && isset($value)) {
        $node->{$element}[$delta] = $value;
      }
      foreach ($attributes as $n => $v) {
        $node->{$element}[$delta][$this->namespace_prefix ? "$this->namespace_prefix:$n" : $n] = $v;
      }
      $node = &$node->{$element}[$delta];
    }
  }

  /**
   * Insert a datetime value into an NCIP message element. Will parse a
   * timestamp from the variable, if possible, and convert it to ISO-8601
   * format.
   *
   * See insert($path, $value) for more information.
   */
  public function insert_datetime($path, $datetime) {
    // Format dateTime
    $timestamp = is_numeric($datetime) ? $datetime : @strtotime($datetime);
    $datetime = @date(DATE_ISO8601, $timestamp);
    $this->insert_element($path, $datetime);
  }

  /**
   * Insert an integer value into an NCIP message element. Will convert the
   * variable to int, if possible.
   *
   * See insert($path, $value) for more information.
   */
  public function insert_integer($path, $integer) {
    // Format integer
    $integer = (int) $integer;
    $this->insert_element($path, $integer);
  }

  /**
   * Insert a non-negative integer value into an NCIP message element. Will
   * convert the variable to int, if possible, and also convert to zero if
   * interger result is negative.
   *
   * See insert($path, $value) for more information.
   */
  public function insert_non_negative_integer($path, $non_negative_integer) {
    // Format non-negative integer
    $integer = (int) $non_negative_integer;
    $non_negative_integer = $integer < 0 ? 0 : $integer;

    $this->insert_element($path, $non_negative_integer);
  }

  /**
   * Insert a positive integer value into an NCIP message element. Will
   * convert the variable to int, if possible, and also convert to 1 if
   * interger result is not positive.
   *
   * See insert($path, $value) for more information.
   */
  public function insert_positive_integer($path, $positive_integer) {
    // Format positive integer
    $integer = (int) $non_negative_integer;
    $positive_integer = $integer < 1 ? 1 : $integer;

    $this->insert_element($path, $positive_integer);
  }

  /**
   * Insert a string integer value into an NCIP message element. Will
   * convert the variable to string, if possible.
   *
   * See insert($path, $value) for more information.
   */
  public function insert_string($path, $string) {
    // Format string
    $string = (string) $string;
    $this->insert_element($path, $string);
  }

  /**
   * Insert an open or closed scheme value into an NCIP message element.
   * Requires both $scheme and $path parameters.
   *
   * See insert($path, $value) for more information.
   *
   * @param $path (Array)
   *   The elemeent's path
   * @param $scheme (String)
   *   The scheme's URL
   * @param $value (String)
   *   A value
   * @return unknown_type
   */
  public function insert_scheme($path, $value, $scheme = null) {
    // Insert integer
    switch ($this->version) {
      case 1:
      case 1.01:
        if (!$scheme) {
          $scheme = url("ncip/schemes/unknownscheme.scm", array('absolute' => TRUE));
        }
        $this->insert_string(array_merge($path, array('Scheme')), $scheme);
        $this->insert_string(array_merge($path, array('Value')), $value);
        break;
      case 2:
      case 2.01:
      default:
        if ($scheme) {
          if (!is_array($element = array_pop($path))) {
            $element = array('name' => $element);
          }
          $element['attributes']['Scheme'] = $scheme;
          $path = array_merge($path, array($element));
        }
        $this->insert_string($path, $value);
        break;
    }
  }

  /**
   * Transforms NCIPMessage object into a valid NCIP XML string
   */
  public function to_xml() {
    return $this->message->asXML();
  }

  /**
   * Transforms NCIPMessage object into a valid NCIP DOMDocument object
   */
  public function to_dom() {
    return DOMDocument::loadXML($this->message->asXML());
  }
}
