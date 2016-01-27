<?php

/**
 * @file
 * Views field handler for Site Disclaimer module.
 */

/**
 * Field handler to site_disclaimer version.
 */
class site_disclaimer_handler_field_tou_version extends views_handler_field_prerender_list {
  function render($values) {
    $data = unserialize($values->{$this->field_alias});
    $tou_version = $data['site_disclaimer'];
    return check_plain($tou_version);
  }
}


