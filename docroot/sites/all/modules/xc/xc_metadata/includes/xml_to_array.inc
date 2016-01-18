<?php
/**
 * @file
 ******************************************************************************
 *                             XML to ARRAY
 ******************************************************************************
 *
 * Functions to converts an XML string to a structured PHP Array and back to
 * an XML string.
 *
 * Generates a structured PHP array in the form similar to an XML schema. The
 * variable represents the root element and its children can be accessed as
 * follows:
 *
 *  1)  Each child element is an array with the element name in order to group
 *      all children with the same element type together as follows:
 *
 *          $xml['child'][0] = array()
 *          $xml['child'][1] = array()
 *          $xml['child'][2] = array()
 *
 *  2)  Element arrays contain other element arrays, but can also contain a
 *      value and other attributes as follows:
 *
 *          $xml['child'][0]['#value'] = 'Element Value';
 *          $xml['child'][0]['@attribute'] = 'Attribute Value';
 *          $xml['child'][0]['sub_child'] = array();
 *
 *  3)  Since the root is also a element, it can contain values and attributes
 *      as well:
 *
 *          $xml['#value'] = 'Root Value';
 *          $xml['@attribute'] = 'Root Attribute Value';
 *
 * Original source code from:
 * http://mysrc.blogspot.com/2007/02/php-xml-to-array-and-backwards.html
 *
 * Although the functions have been modified a lot to achieve the specific
 * requirements for the eXtensible Catalog's XC Metadata module.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */


/**
 * Converts an XML string into a structured PHP array without stripping out
 * namespaces
 *
 * @param $string
 *    XML string
 * @return
 *    A structured PHP array
 */
function xml_to_array(&$string) {
  $parser = xml_parser_create();
  xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
  xml_parse_into_struct($parser, $string, $values, $index);
  xml_parser_free($parser);

  $main_array = array();
  $array = &$main_array;
  foreach ($values as $curr_value) {
    $_tag = $curr_value['tag'];
    if ($curr_value['type'] == 'open') {
      if (isset($array[$_tag])) {
        if (isset($array[$_tag][0])) {
          $array[$_tag][] = array();
        }
        else {
          $array[$_tag] = array($array[$_tag], array());
        }
        $child_value = &$array[$_tag][count($array[$_tag]) - 1];
      }
      else {
        $child_value = &$array[$_tag];
      }

      if (isset($curr_value['attributes'])) {
        foreach ($curr_value['attributes'] as $k => $v) {
          $child_value["@$k"] = $v;
        }
      }
      $child_value['_p'] = &$array;
      $array = &$child_value;
    }
    elseif ($curr_value['type'] == 'complete') {
      if (isset($array[$_tag])) {
        if (isset($array[$_tag][0])) {
          $array[$_tag][] = array();
        }
        else {
          $array[$_tag] = array($array[$_tag], array());
        }
        $child_value = &$array[$_tag][count($array[$_tag]) - 1];
      }
      else {
        $child_value = &$array[$_tag][];
      }

      if (isset($curr_value['attributes'])) {
        foreach ($curr_value['attributes'] as $k => $v) {
          $child_value['@' . $k] = $v;
        }
      }

      if (isset($curr_value['value'])) {
        $curr_value['value'] = trim($curr_value['value']);
        $child_value['#value'] = (!empty($curr_value['value']) ? $curr_value['value'] : '');
      }

      if (empty($child_value['#value'])) {
        unset($child_value['#value']);
      }
    }
    elseif ($curr_value['type'] == 'close') {
      $array = &$array['_p'];
    }
  }
  _delete_parent_array($main_array);

  return $main_array;
}

/**
 * Internal function used to remove recursion in resulting array
 *
 * @param $array
 *    Input array
 */
function _delete_parent_array(&$array) {
  foreach ($array as $k => $v) {
    if ($k === '_p') {
      unset($array[$k]);
    }
    elseif (is_array($array[$k])) {
      _delete_parent_array($array[$k]);
    }
  }
}

/**
 * Converts an XML array back into an XML string.
 *
 * The XML string returned may not validate as XML since namespaces are not
 * stripped out.
 *
 * @param $cary
 *    Input array
 * @param $d
 *    Depth of element within the array structure
 * @param $forcetag
 *    Use to force a root tag name
 *
 * @return
 *    An XML string
 */
function array_to_xml($curr_array, $root = NULL, $d = 0, $forcetag = '') {
  if (!is_array($curr_array) || empty($curr_array)) {
    return;
  }
  $result = array();
  if ($root) {
    $result[] = '<' . $root;
    foreach ($curr_array as $attribute => $attribute_value) {
      if (!is_array($attribute_value) && $attribute != '#value') {
        $_attribute = drupal_substr($attribute, 1);
        $result[] = ' ' . $_attribute . '="' . $attribute_value . '"';
      }
    }
    $result[] = ">\n";
  }
  foreach ($curr_array as $tag => $curr_value) {
    if (empty($curr_value)) {
      continue;
    }
    if (isset($curr_value[0])) {
      $result[] = array_to_xml($curr_value, NULL, $d, $tag);
    }
    elseif (is_array($curr_value)) {
      if ($forcetag) {
        $tag = $forcetag;
      }
      $sp = str_repeat("\t", $d);
      $result[] = $sp . '<' . $tag;
      foreach ($curr_value as $attribute => $attribute_value) {
        if (!is_array($attribute_value) && $attribute != '#value') {
          $_attribute = drupal_substr($attribute, 1);
          $result[] = ' ' . $_attribute . '="' . $attribute_value . '"';
        }
      }
      $result[] = ">";
      if (is_array($curr_value)) {
        $result[] = array_to_xml($curr_value, NULL, $d + 1);
      }
      if (isset($curr_value['#value'])) {
        $result[] = trim(htmlspecialchars($curr_value['#value']));
      }
      $result[] = (is_array($curr_value) ? $sp : '') . '</' . $tag . '>' . "\n";
    }
  }
  if ($root) {
    $result[] = '</' . $root . '>';
  }
  return implode('', $result);
}

/**
 * Inserts an XML element as an array into an existing XML array, this may
 * not be the best way to do this or a great function to use religiously
 *
 * @param $array
 *    The array
 * @param $element
 *    The element
 * @param $pos
 *    The position within the XML array
 *
 * @return void
 */
function insert_into_array(&$array, $element, $pos) {
  $ar1 = array_slice($array, 0, $pos);
  $ar1[] = $element;
  $array = array_merge($ar1, array_slice($array, $pos));
}

/**
 * Returns the specified array within a multiple-level array by its key
 *
 * @param $key
 *    The key name
 * @param $form
 *    The array to search
 *
 * @return
 *    A reference to the specified array
 */
function &find_array_element_by_key($key, &$form) {
  $value = FALSE;
  if (!is_array($form)) {
    return $value;
  }

  if (array_key_exists($key, $form)) {
    $value = &$form[$key];
    return $value;
  }

  foreach ($form as $k => $v) {
    if (is_array($v)) {
      $value = &find_array_element_by_key($key, $form[$k]);
      if ($value) {
        return $value;
      }
    }
  }
  return $value;
}
