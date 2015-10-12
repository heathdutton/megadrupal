<?php

/**
 * @file
 *
 * Parish info XML file endpoint.
 */

/**
 * Menu callback to deliver parish XML feed.
 *
 * Returns an XML file conforming to the standards set forth by Open Source
 * Catholic for open parish data sharing.
 *
 * @see http://www.opensourcecatholic.com/wiki/117/xml-format-parish-directo
 */
function parish_info_xml_endpoint() {
  $namespaces = array('xmlns' => 'http://www.opensourcecatholic.com/parishdata');

  // Get all parishes from the database.
  $parish_data = db_query("SELECT * FROM {parish_info}")->fetchAll();

  $parish_xml = '';

  // Loop through parishes and build array of data to be rendered.
  foreach ($parish_data as $key => $parish) {
    // Start tag.
    $parish_xml .= "  <parish>\n";

    // Basic parish info.
    $parish_xml .= "    <name>" . $parish->name . "</name>\n";
    $parish_xml .= "    <diocese>" . $parish->diocese . "</diocese>\n";
    $parish_xml .= "    <url>" . $parish->website . "</url>\n";
    if (!empty($parish->phone)) {
      $parish_xml .= "    <phone>" . $parish->phone . "</phone>\n";
    }
    if (!empty($parish->fax)) {
      $parish_xml .= "    <fax>" . $parish->fax . "</fax>\n";
    }
    if (!empty($parish->email)) {
      $parish_xml .= "    <email>" . $parish->email . "</email>\n";
    }
    $parish_xml .= "    <lastUpdated>" . date('c', $parish->updated) . "</lastUpdated>\n";

    // Parish location data.
    $parish_xml .= "    <location>\n";
    $parish_xml .= "      <street>" . $parish->address_street . "</street>\n";
    if (!empty($parish->address_street_2)) {
      $parish_xml .= "      <street>" . $parish->address_street . "</street>\n";
    }
    $parish_xml .= "      <city>" . $parish->address_city . "</city>\n";
    $parish_xml .= "      <state>" . $parish->address_state . "</state>\n";
    $parish_xml .= "      <zip>" . $parish->address_zip . "</zip>\n";
    $parish_xml .= "    </location>\n";

    // Parish event data.
    // TODO

    // End tag.
    $parish_xml .= "  </parish>\n";
  }

  // Render the parish data to the XML file.
  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<parishdata" . drupal_attributes($namespaces) . ">\n";
  $output .= $parish_xml;
  $output .= "</parishdata>";

  // Add proper headers for XML output and print the XML.
  drupal_add_http_header('Content-Length', strlen($output));
  drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');
  print $output;

  // Exit so we don't do any extra/unnecessary rendering.
  drupal_exit();
}
