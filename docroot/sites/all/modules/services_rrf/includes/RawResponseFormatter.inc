<?php

class RawResponseFormatter implements ServicesFormatterInterface {
  public function render($data) {
    if (is_string($data)) {
      return $data;
    }
  }
}
