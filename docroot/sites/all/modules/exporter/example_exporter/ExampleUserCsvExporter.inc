<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 22.7.13
 * Time: 17:37
 */

class ExampleUserCsvExporter extends UserExporter {

  function renderHeader() {
    return "uid,name,email\n";
  }

  function renderItem($node) {
    return "$node->uid,$node->name,$node->mail\n";
  }

  function renderFooter() {
    return "";
  }
}
