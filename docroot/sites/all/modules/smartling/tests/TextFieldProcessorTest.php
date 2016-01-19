<?php

use Drupal\smartling\FieldProcessors\TextFieldProcessor;

class TextFieldProcessorTest extends PHPUnit_Framework_TestCase {

  public function testFestDataFromXML() {
    $xml_str = <<<EOF
<?xml version="1.0" encoding="UTF-8"?>
    <data><localize title="10">
      <string id="body-body-0" quantity="1">sdfsdfsdfsa</string>
      <string id="body-summary-0" quantity="1"></string>
      <string id="field_image-alt-img-0" quantity="1" fid="51"></string>
      <string id="field_image-title-img-0" quantity="1" fid="51"></string>
      <string id="field_image_description-0" quantity="1">Image description en</string>
      <string id="title_field-0" quantity="1">Standalone node EN</string></localize>
      <string id="field_text-value-0" quantity="1">TEXT VALUE</string></localize>
    </data>
EOF;

    $node = $smartling_object = new stdClass();
    $node->title = 'TITLE';
    $node->type = 'page';
    $node->field_text = array('und' => array(array('value' => 'TEXT VALUE')));
    $fieldProcessor = new TextFieldProcessor($node, $node->type, 'field_text', $smartling_object, 'und', 'en');
    $xml = new \DOMDocument();
    $xml->loadXML($xml_str);
    $xpath = new \DOMXPath($xml);
    $this->assertEquals($node->field_text['und'], $fieldProcessor->fetchDataFromXML($xpath));
  }

}