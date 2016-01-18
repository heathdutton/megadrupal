<?php


/**
 * @file Contains helper functions to build the clicktale PHP integration
 * library XML configuration file.
 */

define('CLICKTALE_XML_FILENAME', '/ClickTaleScripts.xml');

/**
 * Build the clicktale PHP integration libray XML configuration file.
 *
 * @param string $top
 * @param string $bottom
 * @return DOMDocument
 */
function clicktale_xml_create($top, $bottom) {
  // Outer scripts tag.
  $xml = new DOMDocument('1.0', 'utf-8');
  $scripts = $xml->createElement('scripts');
  $xml->appendChild($scripts);

  // Add top script tag.
  $top = $xml->createCDATASection($top);
  $xml_top = $xml->createElement('script');
  $xml_top->setAttribute('name', 'Top');
  $xml_top->setAttribute('DoNotReplaceCondition', "<!-- ClickTale Top part -->");
  $xml_top->appendChild($top);
  $scripts->appendChild($xml_top);

  // Add bottom script tag.
  $bottom = $xml->createCDATASection($bottom);
  $xml_bottom = $xml->createElement('script');
  $xml_bottom->setAttribute('name', 'Bottom');
  $xml_bottom->setAttribute('DoNotReplaceCondition', '<!-- No ClickTale -->');
  $xml_bottom->setAttribute('InsertBefore', '<!-- ClickTale Bottom part Here -->|</body>');
  $xml_bottom->appendChild($bottom);
  $scripts->appendChild($xml_bottom);
  
  return $xml;
}

/**
 * Write the PHP integration library configuration XML file to disk.
 *
 * @param DOMDocument $xml
 */
function clicktale_xml_write(DOMDocument $xml) {
  // Load basic helper functions
  include_once drupal_get_path('module', 'clicktale') . '/includes/clicktale.helpers.inc';

  // Save the XML to file.
  if ($xml->save(clicktale_get_path() . CLICKTALE_XML_FILENAME)) {
    drupal_set_message(t('Clicktale XML configuration have been saved.'));
  }
  else {
    drupal_set_message(t('Clicktale XML configuration could *not* be saved.'), 'error');
  }
}
