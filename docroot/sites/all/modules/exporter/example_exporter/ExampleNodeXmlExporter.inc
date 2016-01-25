<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:13
 */

class ExampleNodeXmlExporter extends NodeExporter {

  function renderHeader() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" .
    "<NODES>\n";
  }

  function renderItem($node) {
    $item = "<NODE>\n";
    $item .= "\t<NID>" . $node->nid ."</NID>\n";
    $item .= "\t<TITLE>" . $node->title ."</TITLE>\n";
    $item .= "</NODE>\n";
    return $item;
  }

  function renderFooter() {
    return "</NODES>\n";
  }
}
