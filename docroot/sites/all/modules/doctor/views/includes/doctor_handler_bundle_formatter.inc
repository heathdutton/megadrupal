<?php
/**
 * @file
 * Handler for the doctor listen item bundle.
 */
class doctor_handler_bundle_formatter extends views_handler_field {
  function render($values) {
    return doctor_get_types($values->doctor_type)->name;
  }
}
