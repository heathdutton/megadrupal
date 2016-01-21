<?php

class CommerceQbWebconnectQbExportEntityController extends EntityAPIController {
  function attachLoad(&$queried_exports, $revision_id = FALSE){
    foreach ($queried_exports as &$export) {
      $export->data = drupal_json_decode($export->data);
    }
  }
}
