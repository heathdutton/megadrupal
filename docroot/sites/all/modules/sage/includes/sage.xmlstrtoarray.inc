<?php

/**
 * @file
 * Helper functions to convert xml string to php array.
 *
 * Useful to get a serializable value.
 */

/**
 * Converts xml string to php array.
 *
 * @param string $xmlstr
 *   A structured xml string.
 *
 * @return array
 *   Php array.
 */
function sage_xmlstr_to_array($xmlstr) {

  $doc = new DOMDocument();
  $doc->loadXML($xmlstr);
  $root = $doc->documentElement;
  $output = sage_domnode_to_array($root);
  $output['@root'] = $root->tagName;

  return $output;
}

/**
 * XML parser.
 *
 * @param string $node
 *   Xml string.
 *
 * @return array
 *   Php array.
 */
function sage_domnode_to_array($node) {

  $output = array();
  switch ($node->nodeType) {

    case XML_CDATA_SECTION_NODE:
    case XML_TEXT_NODE:
        $output = trim($node->textContent);
      break;

    case XML_ELEMENT_NODE:
        for ($i = 0, $m = $node->childNodes->length; $i < $m; $i++) {
        $child = $node->childNodes->item($i);
        $v = sage_domnode_to_array($child);
        if (isset($child->tagName)) {
          $t = $child->tagName;
          if (!isset($output[$t])) {
            $output[$t] = array();
          }
          $output[$t][] = $v;
        }
        elseif ($v || $v === '0') {
          $output = (string) $v;
        }
        }
        // Has attributes but isn't an array.
        if ($node->attributes->length && !is_array($output)) {
          // Change output into an array.
          $output = array('@content' => $output);
        }
        if (is_array($output)) {
          if ($node->attributes->length) {
            $a = array();
            foreach ($node->attributes as $attrName => $attrNode) {
              $a[$attrName] = (string) $attrNode->value;
            }
            $output['@attributes'] = $a;
          }
          foreach ($output as $t => $v) {
            if (is_array($v) && count($v) == 1 && $t != '@attributes') {
              $output[$t] = $v[0];
            }
          }
        }
      break;
  }
  return $output;
}
