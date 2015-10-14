<?php

/**
 * @file
 * DOM processing functions.
 */

/**
 * Parses an HTML snippet and returns it as a DOM object.
 *
 * This function loads the body part of a partial (X)HTML document and returns
 * a full \DOMDocument object that represents this document.
 *
 * @param string $html
 *   The partial (X)HTML snippet to load. Invalid markup will be corrected on
 *   import.
 *
 * @return \DOMDocument
 *   A \DOMDocument that represents the loaded (X)HTML snippet.
 */
function entity_embed_dom_load_html($html) {
  $document = <<<EOD
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /></head>
<body>!html</body>
</html>
EOD;
  // PHP's \DOMDocument serialization adds straw whitespace in case the markup
  // of the wrapping document contains newlines, so ensure to remove all
  // newlines before injecting the actual HTML body to process.
  $document = strtr($document, array("\n" => '', '!html' => $html));

  $dom = new \DOMDocument();
  // Ignore warnings during HTML soup loading.
  @$dom->loadHTML($document);

  return $dom;
}

/**
 * Replace the contents of a DOMNode.
 *
 * @param \DOMNode $node
 *   A DOMNode or DOMElement object.
 * @param string $content
 *   The text or HTML that will replace the contents of $node.
 */
function entity_embed_set_dom_node_content(\DOMNode $node, $content) {
  // Load the contents into a new DOMDocument and retrieve the element.
  $replacement_node = entity_embed_dom_load_html($content)->getElementsByTagName('body')
    ->item(0)
    ->childNodes
    ->item(0);

  // Import the updated DOMNode from the new DOMDocument into the original
  // one, importing also the child nodes of the replacment DOMNode.
  $replacement_node = $node->ownerDocument->importNode($replacement_node, TRUE);

  // Remove all children of the DOMNode.
  while ($node->hasChildNodes()) {
    $node->removeChild($node->firstChild);
  }

  // Rename tag of container elemet to 'div' if it was 'drupal-entity'.
  if ($node->tagName == 'drupal-entity') {
    $new_node = $node->ownerDocument->createElement('div');

    // Copy all attributes of original node to new node.
    if ($node->attributes->length) {
      foreach ($node->attributes as $attribute) {
        $new_node->setAttribute($attribute->nodeName, $attribute->nodeValue);
      }
    }

    $node->parentNode->replaceChild($new_node, $node);

    $node = $new_node;
  }

  // Finally, append the contents to the DOMNode.
  $node->appendChild($replacement_node);
}

/**
 * Converts the body of a \DOMDocument back to an HTML snippet.
 *
 * The function serializes the body part of a \DOMDocument back to an (X)HTML
 * snippet. The resulting (X)HTML snippet will be properly formatted to be
 * compatible with HTML user agents.
 *
 * @param \DOMDocument $document
 *   A \DOMDocument object to serialize, only the tags below the first <body>
 *   node will be converted.
 *
 * @return string
 *   A valid (X)HTML snippet, as a string.
 */
function entity_embed_serialize(\DOMDocument $document) {
  $body_node = $document->getElementsByTagName('body')->item(0);
  $html = '';

  foreach ($body_node->getElementsByTagName('script') as $node) {
    entity_embed_escape_cdata_element($node);
  }
  foreach ($body_node->getElementsByTagName('style') as $node) {
    entity_embed_escape_cdata_element($node, '/*', '*/');
  }
  foreach ($body_node->childNodes as $node) {
    $html .= $document->saveXML($node);
  }
  return $html;
}

/**
 * Adds comments around a <!CDATA section in a \DOMNode.
 *
 * \DOMDocument::loadHTML() in \Drupal\Component\Utility\Html::load() makes
 * CDATA sections from the contents of inline script and style tags. This can
 * cause HTML4 browsers to throw exceptions.
 *
 * This function attempts to solve the problem by creating a
 * \DOMDocumentFragment to comment the CDATA tag.
 *
 * @param \DOMNode $node
 *   The element potentially containing a CDATA node.
 * @param string $comment_start
 *   (optional) A string to use as a comment start marker to escape the CDATA
 *   declaration. Defaults to '//'.
 * @param string $comment_end
 *   (optional) A string to use as a comment end marker to escape the CDATA
 *   declaration. Defaults to an empty string.
 */
function entity_embed_escape_cdata_element(\DOMNode $node, $comment_start = '//', $comment_end = '') {
  foreach ($node->childNodes as $child_node) {
    if ($child_node instanceof \DOMCdataSection) {
      $embed_prefix = "\n<!--{$comment_start}--><![CDATA[{$comment_start} ><!--{$comment_end}\n";
      $embed_suffix = "\n{$comment_start}--><!]]>{$comment_end}\n";

      // Prevent invalid cdata escaping as this would throw a DOM error.
      // This is the same behavior as found in libxml2.
      // Related W3C standard: http://www.w3.org/TR/REC-xml/#dt-cdsection
      // Fix explanation: http://en.wikipedia.org/wiki/CDATA#Nesting
      $data = str_replace(']]>', ']]]]><![CDATA[>', $child_node->data);

      $fragment = $node->ownerDocument->createDocumentFragment();
      $fragment->appendXML($embed_prefix . $data . $embed_suffix);
      $node->appendChild($fragment);
      $node->removeChild($child_node);
    }
  }
}
