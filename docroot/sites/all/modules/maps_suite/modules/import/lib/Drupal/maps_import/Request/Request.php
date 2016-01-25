<?php

/**
 * @file
 * MaPS Import request class.
 *
 * This class intends to retrieve data from MaPS System® web services and
 * store them into the database.
 */

namespace Drupal\maps_import\Request;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Exception\RequestException;

/**
 * This class is only a convenient wrapper around the getData() method.
 */
abstract class Request {

  /**
   * Defines the JSON format for a Web Service response.
   */
  const FORMAT_JSON = 'json';

  /**
   * Defines the XML format for a Web Service response.
   */
  const FORMAT_XML = 'xml';

  /**
   * The Profile instance.
   *
   * @var Profile
   */
  protected $profile;

  /**
   * The request result.
   *
   * @var StdClass
   */
  protected $result;

  /**
   * The parsed data.
   *
   * @var array
   */
  protected $parsed = array();

  /**
   * Drupal HTTP request POST data.
   *
   * @var array
   */
  protected $request_arguments;

  /**
   * Drupal HTTP request options.
   */
  protected $options = array();

  /**
   * Get the related profile.
   *
   * @return Profile
   *   The profile instance.
   */
  public function getProfile() {
    return $this->profile;
  }

  /**
   * Parse the response data.
   */
  public function parse() {
    $method = 'parse' . ucfirst($this->getProfile()->getFormat());

    if (! method_exists($this, $method)) {
      throw new RequestException('The expected method %method does not exist.', 0, array('%method' => $method));
    }

    return call_user_func(array($this, $method));
  }

  /**
   * Parse a JSON return, using associative arrays.
   */
  protected function parseJson() {
    if (empty($this->result->data)) {
      throw new RequestException('The returned data are empty.');
    }

    if (NULL === ($parsed = json_decode($this->result->data, TRUE))) {
      throw new RequestException('The returned data are not a valid JSON string.');
    }

    return $this->parsed = $parsed;
  }

  /**
   * Parse an XML return, using associative arrays.
   */
  protected function parseXml() {
    $parser = xml_parser_create();
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
    xml_parser_set_option($parser, XML_OPTION_TARGET_ENCODING, 'UTF-8');
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);

    $values_array = array();
    xml_parse_into_struct($parser, $this->result->data, $values_array);
    xml_parser_free($parser);

    if (!$values_array) {
      throw new RequestException('The returned data are empty or not a valid XML string.');
    }

    $this->parsed = $parents = array();
    $current = &$this->parsed;

    // Multiple tags with same name will be turned into an array
    $repeated_tag_index = array();

    foreach($values_array as $data) {
      unset($attributes, $value);
      extract($data);
      $result = array();

      if ($tag === 'main') {
        continue;
      }

      if (isset($value)) {
        $result['value'] = $value;
      }

      if (isset($attributes)) {
        foreach($attributes as $attr => $val) {
          $result[$attr] = $val;
        }
      }

      $tag_id = 'id' . $tag;

      if ($type == 'open') {
        $parents[$level - 1] = &$current;

        // Do not create a tag but use the given ID as array key. Note that
        // duplicate IDs will be overriden.
        if (isset($attributes) && array_key_exists($tag_id, $attributes) && is_numeric($attributes[$tag_id])) {
          unset($result[$tag_id]);
          $current[$attributes[$tag_id]] = $result;
          $current = &$current[$attributes[$tag_id]];
        }
        // Create a new tag
        elseif (!is_array($current) || (!in_array($tag, array_keys($current)))) {
          $current[$tag] = $result;
          $repeated_tag_index[$tag . '_' . $level] = 1;
          $current = &$current[$tag];
        }
        // The tag name exists already
        else {
          // If there is a 0th element it is already an array
          if (isset($current[$tag][0])) {
            $current[$tag][$repeated_tag_index[$tag . '_' . $level]] = $result;
            $repeated_tag_index[$tag . '_' . $level]++;
          }
          // This section will make the value an array if multiple tags with the same name appear together
          else {
            // This will combine the existing item and the new item together to make an array
            $current[$tag] = array($current[$tag], $result);
            $repeated_tag_index[$tag . '_' . $level] = 2;
          }

          $last_item_index = $repeated_tag_index[$tag . '_' . $level] - 1;
          $current = &$current[$tag][$last_item_index];
        }
      }
      // Tags that ends in 1 line '<tag />'
      elseif ($type == 'complete') {
        if ($tag === 'value') {
          $index = 0;
          if (isset($attributes['idlanguage']) && is_numeric($attributes['idlanguage'])) {
            unset($result['idlanguage']);
            $index = $attributes['idlanguage'];
          }

          if (!empty($result['idattribute_library'])) {
          	$current['values'][$index][$result['idattribute_library']] = $result;
          }
          else if (!empty($result['idobject_criteria'])) {
            $current['values'][$index][$result['idobject_criteria']] = $result;
          }
          else {
            $current['values'][$index][] = $result;
          }

          continue;
        }

        if (isset($attributes) && array_key_exists($tag_id, $attributes) && is_numeric($attributes[$tag_id])) {
          unset($result[$tag_id]);
          $current[$attributes[$tag_id]] = $result;
        }
        // See if the key is already taken.
        elseif (!isset($current[$tag])) {
          $current[$tag] = $result;
          $repeated_tag_index[$tag . '_' . $level] = 1;
        }
        // If taken, put all things inside a list(array)
        else {
          // If it is already an array...
          if (isset($current[$tag][0]) and is_array($current[$tag])) {
            // ...push the new element into that array.
            $current[$tag][$repeated_tag_index[$tag . '_' . $level]] = $result;
            $repeated_tag_index[$tag . '_' . $level]++;
          }
          // If it is not an array...
          else {
            //...Make it an array using using the existing value and the new value
            $current[$tag] = array($current[$tag], $result);
            $repeated_tag_index[$tag . '_' . $level] = 1;

            // 0 and 1 indexes are already taken
            $repeated_tag_index[$tag . '_' . $level]++;
          }
        }
      }
      // End of tag '</tag>'
      elseif($type == 'close') {
        $current = &$parents[$level - 1];
      }
    }

    return $this->parsed;
  }

  public static function getData(Profile $profile, array $args = array(), array $options = array())
  {
    if ($profile->getFetchMethod() === Profile::FETCH_WS) {
      return Webservice::getData($profile, $args, $options);
    }
    else {
      return Files::getData($profile, $args, $options);
    }
  }

}
