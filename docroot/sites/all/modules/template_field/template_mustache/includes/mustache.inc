<?php

// Just in case :)
// Do we really need this?
class TemplateApiMustache extends Mustache {
  public function set_data($data) {
    foreach ($data as $key => $value) {
      $this->$key = $value;
    }
  }
}