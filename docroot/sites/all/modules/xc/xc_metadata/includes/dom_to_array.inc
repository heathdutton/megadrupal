<?php
/**
 * @file
 * DOM to array transformation
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Iterate over a DOM structure to set up
 *
 * @param $node (DOMElement)
 *   The node to convert to array
 *
 * @return (Array)
 *   The content as array
 */
function dom_to_array($node, $parent_name = NULL) {
  $name = $node->nodeName;
  $metadata = array();

  // copy attributes
  foreach ($node->attributes as $attr) {
    $metadata['@' . $attr->name] = $attr->value;
  }

  // process children
  foreach ($node->childNodes as $child) {
    // process children elements
    if ($child->nodeType == XML_ELEMENT_NODE) {
      $metadata[$child->nodeName][] = dom_to_array($child, $name);
    }
    // copy text value
    elseif ($child->nodeType == XML_TEXT_NODE) {
      $value = trim($child->nodeValue);
      if (!empty($value)) {
        $metadata['#value'] = str_replace("\n", ' ', $value);
      }
    }
  }

  return $metadata;
}
