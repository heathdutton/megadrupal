<?php

/**
 * @file
 * Pin It Interfaces and Classes
 */

define('PINIT_TABLE', 'pinit');
define('PINIT_PERM_ADMIN', 'administer pin it button');
define('PINIT_PERM_EDIT', 'edit pin it button settings');
define('PINIT_VAR_CONTENT_TYPES_ENABLED', 'pinit_types_enabled');
define('PINIT_VAR_CONTENT_TYPE_', 'pinit_type_');
define('PINIT_VAR_SOURCE', 'pinit_source');
define('PINIT_VAR_DEFAULT_PINCOUNT', 'pinit_default_pincount');
define('PINIT_VAR_DEFAULT_DESCRIPTION', 'pinit_default_description');
define('PINIT_VAR_DEFAULT_IMG', 'pinit_default_img');

class PinIt {

  const PINCOUNT = 'pincount';
  const DESCRIPTION = 'description';
  const IMAGE = 'image';
  const URL = 'url';

  /** Singleton instance. */
  protected static $instance = NULL;

  protected $settingsObj = NULL;
  protected $dataObj = NULL;

  /**
   * Constructor
   */
  protected function __construct() {
    $this->settingsObj = new PIDrupalSettings();
    $this->dataObj = new PIDrupalData();
  }

  /**
   * Get Singleton instance.
   */
  public static function instance() {
    if (empty(self::$instance)) {
      self::$instance = new PinIt();
    }
    return self::$instance;
  }

  /**
   * Get default values for all pins (including optional ones).
   *
   * @param object $node
   *   A node object. If provided then defaults will be tailored to this node.
   * @param boolean $full_view
   *   Whether the node is being viewed on its own in full view.
   */
  protected function getPinDefaults($node = NULL, $full_view = FALSE) {
    // Defaults.
    static $defaults = array();
    if (empty($defaults)) {
      $defaults = array(
        self::PINCOUNT => '',
        self::DESCRIPTION => '',
        self::IMAGE => '',
        self::URL => '',
      );
    }

    $ret = $defaults;

    if (!empty($node)) {
      // If node given then override defaults.
      $source = $this->settingsObj->get(PINIT_VAR_SOURCE, 'title');

      // The description.
      $body = PinItDrupalLayer::getNodeBody($node);
      if ($source == 'body') {
        $ret[self::DESCRIPTION] = !empty($body) ? mb_substr(strip_tags($body), 0, 500) : $node->title;
      }
      else {
        $ret[self::DESCRIPTION] = $node->title;
      }

      $ret[self::PINCOUNT] = $this->settingsObj->get(PINIT_VAR_DEFAULT_PINCOUNT, '');
      $ret[self::IMAGE] = $this->settingsObj->get(PINIT_VAR_DEFAULT_IMG, '');

      $images = $this->harvestImagesFromNode($node);
      if (!empty($images)) {
        $i = array_shift($images);
        $ret[self::IMAGE] = $i['url'];
      }

      $ret[self::URL] = url(drupal_get_path_alias('node/' . $node->nid), array('absolute' => TRUE));
    }

    return $ret;
  }

  /**
   * Get whether meta tags are enabled for the given content type.
   * @return boolean
   *   TRUE if so; FALSE otherwise.
   */
  public function pinsAreEnabledForContentType($type) {
    $content_types = $this->settingsObj->get(PINIT_VAR_CONTENT_TYPES_ENABLED, array());
    $content_types = array_filter($content_types);
    return empty($content_types) || !empty($content_types[$type]);
  }

  /**
   * Delete pin data for the given node.
   */
  public function deleteNodedata($nid) {
    $this->dataObj->deletePins($nid);
  }

  /**
   * Save pin data for the given node.
   *
   * @param int $nid
   *   The node id
   * @param array $data
   *   key-value pairs
   */
  public function saveNodeData($nid, $data) {
    $ret = $this->dataObj->loadPins($nid);

    if (FALSE === $ret) {
      $row = new stdClass();
      $row->nid = $nid;
    }
    else {
      $row = $ret;
    }

    // Collapse data tree into 1-D array.
    $collapsed_data = new stdClass();
    $collapsed_data->keys = array();
    $collapsed_data->values = array();
    array_walk_recursive($data, create_function('$val, $key, $obj', 'array_push($obj->keys, $key); array_push($obj->values, $val);'), $collapsed_data);
    $collapsed_data = array_combine($collapsed_data->keys, $collapsed_data->values);

    foreach ($this->getPinDefaults() as $field => $_d) {
      $row->$field = isset($collapsed_data[$field]) ? $collapsed_data[$field] : '';
    }

    $this->dataObj->updatePins($row, FALSE !== $ret ? 'nid' : array());
  }

  /**
   * Load pin data for the given node.
   *
   * @return array
   *   array('title' => ..., 'summary' => ...).
   */
  public function loadNodeData($node) {
    $fields = $this->getPinDefaults();

    $row = $this->dataObj->loadPins($node->nid);
    if (FALSE !== $row) {
      foreach ($fields as $field => &$val) {
        if (isset($row->$field)) {
          $val = $row->$field;
        }
      }
    }

    return $fields;
  }

  /**
   * Create the Pin button link.
   *
   * @param array $node
   *   The node
   * @param array $pin_data
   *   The pin data
   */
  public function renderData($node, $pin_data) {

    // Add external script.
    drupal_add_js('http://assets.pinterest.com/js/pinit.js');

    // Default values in case no values set.
    $defaults = $this->getPinDefaults($node, TRUE);

    $pin_data[PinIt::URL] = $defaults[PinIt::URL];
    $pin_data[PinIt::PINCOUNT] = !empty($pin_data[PinIt::PINCOUNT]) ? $pin_data[PinIt::PINCOUNT] : $defaults[PinIt::PINCOUNT];
    $pin_data[PinIt::DESCRIPTION] = !empty($pin_data[PinIt::DESCRIPTION]) ? $pin_data[PinIt::DESCRIPTION] : $defaults[PinIt::DESCRIPTION];
    $pin_data[PinIt::IMAGE] = !empty($pin_data[PinIt::IMAGE]) ? $pin_data[PinIt::IMAGE] : $defaults[PinIt::IMAGE];

    $query = array(
      'url' => $pin_data[PinIt::URL],
      'media' => $pin_data[PinIt::IMAGE],
      'description' => $pin_data[PinIt::DESCRIPTION],
    );

    $links['pinit'] = array(
      'title' => '<img border="0" src="//assets.pinterest.com/images/PinExt.png" title="Pin It" />',
      'href' => 'http://pinterest.com/pin/create/button/',
      'query' => $query,
      'attributes' => array('class' => 'pin-it-button', 'count-layout' => $pin_data[PinIt::PINCOUNT]),
      'html' => TRUE,
    );

    $node->content['links']['pinit'] = array(
      '#theme' => 'links__node__pinit',
      '#links' => $links,
      '#attributes' => array('class' => array('links', 'inline')),
    );
  }

  /**
   * Harvest all images from the given node.
   *
   * @return array
   *   array(array('title' => , 'alt' => , 'url' =>)).
   */
  public function harvestImagesFromNode($node) {
    // Extract image fields.
    $ret = array();
    PinItDrupalLayer::extractImageFields((array) $node, $ret);

    // Extract all images from body content.
    $body = PinItDrupalLayer::getNodeBody($node);
    if (!empty($body)) {
      libxml_use_internal_errors(TRUE);
      $doc = new DOMDocument();
      $doc->loadHTML($body);
      $list = $doc->getElementsByTagName('img');
      for ($i = 0; $list->length > $i; ++$i) {
        $item = $list->item($i);
        if ($item->hasAttribute('src')) {
          $url = $item->getAttribute('src');
          if (!empty($url)) {
            $ret[] = array('title' => $url, 'alt' => $url, 'url' => $url);
          }
        }
      }
      libxml_use_internal_errors(FALSE);
    }

    return $ret;
  }


  /**
   * Log a warning message.
   *
   * @param string $msg
   *   the message.
   */
  public function warn($msg) {
    watchdog('pinit', $msg, array(), WATCHDOG_WARNING);
  }
}

/**
 * Interface to getting/setting config settings.
 */
interface PISettings {
  /**
   * Get a Drupal Value.
   *
   * @param string $name
   *   The value name.
   * @param mixed $default
   *   Default value.
   *
   * @return mixed
   *   The value.
   */
  public function get($name, $default);

  /**
   * Set a Drupal Value.
   *
   * @param string $name
   *   The value name.
   * @param mixed $value
   *   Default value.
   */
  public function set($name, $value);
}

/**
 * Implementation which uses Drupal's variables store.
 */
class PIDrupalSettings implements PISettings {
  /**
   * Implemets get method.
   */
  public function get($name, $default) {
    return variable_get($name, $default);
  }
  /**
   * Implements set method.
   */
  public function set($name, $value) {
    variable_set($name, $value);
  }
}

/**
 * Interface to getting/setting node tag data.
 */
interface PIData {
  /**
   * Load a Pin data from a node
   *
   * @param integer $nid
   *   The node id.
   *
   * @return mixed
   *   The Pin data
   */
  public function loadPins($nid);

  /**
   * Delete Pin data from a node
   *
   * @param integer $nid
   *   The node id.
   */
  public function deletePins($nid);

  /**
   * Update a Pin data from a node
   *
   * @param object $field_data_including_nid
   *   The data.
   * @param array $primary_key
   *   Primary key
   */
  public function updatePins($field_data_including_nid, $primary_key = array());
}
/**
 * Implementation for PIData.
 */
class PIDrupalData implements PIData {
  /**
   * Implements Load Pins.
   */
  public function loadPins($nid) {
    $ret = PinItDrupalLayer::loadPins($nid);
    return $ret;
  }
  /**
   * Implements Delete Pins.
   */
  public function deletePins($nid) {
    PinItDrupalLayer::deletePins($nid);
  }

  /**
   * Implements Update Pins.
   */
  public function updatePins($field_data_including_nid, $primary_key = array()) {
    drupal_write_record(PINIT_TABLE, $field_data_including_nid, $primary_key);
  }
}
/**
 * Implementation which uses Drupal's variables store.
 */
class PinItDrupalLayer {
  /**
   * The construct method.
   */
  protected function __construct() {}

  /**
   * Get all available node content types.
   */
  public static function getNodeTypes() {
    return node_type_get_types();
  }

  /**
   * Get contents of node body.
   *
   * @param object $node
   *   the node object.
   *
   * @return string
   *   empty string if no body found.
   */
  public static function getNodeBody($node) {
    $body = '';

    $lang = field_language('node', $node, 'body');
    if (!$lang) {
      $lang = LANGUAGE_NONE;
    }
    if (
        !empty($node) &&
        !empty($node->body) &&
        !empty($node->body[$lang]) &&
        !empty($node->body[$lang]['0']) &&
        !empty($node->body[$lang]['0']['value'])
      ) {
          $body = $node->body[$lang]['0']['value'];
    }
    return $body;
  }

  /**
   * Harvest images from node's image fields.
   *
   * @param array $fields
   *   The fields.
   * @param array $resultarray
   *   Will hold results.
   */
  public static function extractImageFields($fields, array &$resultarray) {
    $_uri_field = 'uri';

    if (is_array($fields)) {
      if (!empty($fields['filemime']) && FALSE !== stripos($fields['filemime'], 'image') && !empty($fields[$_uri_field])) {
        $url = $fields[$_uri_field];
        $url = image_style_url('large', $fields[$_uri_field]);

        array_push($resultarray, array(
          'title' => !empty($fields['title']) ? $fields['title'] : $url,
          'alt' => !empty($fields['alt']) ? $fields['alt'] : $url,
          'url' => $url,
        ));
      }
      else {
        foreach ($fields as $cv) {
          self::extractImageFields($cv, $resultarray);
        }
      }
    }
  }

  /**
   * Get rendered IMG tag for the PI node image selector.
   *
   * @param array $image
   *   The image.
   */
  public static function themeSelectorImage($image) {
    $attributes = array('class' => 'pinit-thumb');
    $abs_path = url(ltrim($image['url'], '/'),
      array(
        'absolute' => TRUE,
      )
    );
    return theme('image',
      array(
        'path' => $abs_path,
        'alt' => $image['alt'],
        'height' => '60px',
        'attributes' => array_merge($attributes, array('title' => $image['title'])),
      )
    );
  }

  /**
   * Delete pinit info for given node.
   */
  public static function deletePins($nid) {
    db_query("DELETE FROM {" . PINIT_TABLE . "} WHERE nid = :nid", array(':nid' => $nid));
  }


  /**
   * Load pinit info for given node.
   */
  public static function loadPins($nid) {
    $result = db_query("SELECT * FROM {" . PINIT_TABLE . "} WHERE nid = :nid", array(':nid' => $nid));
    if ($result->rowCount() <= 0) {
      return FALSE;
    }
    return $result->fetchObject();
  }

  /**
   * Log a watchdog error related to the Drupal compatibility layer.
   * @static
   *
   * @param string $msg
   *   The Message
   */
  protected static function err($msg) {
    watchdog('pinit', '%class: %msg', array('%class' => __CLASS__, '%msg' => $msg), WATCHDOG_ERROR);
  }
}
