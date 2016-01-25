<?php

/**
 * @file
 * Rules for all objects that have admin interfaces.
 */

abstract class FmAdmin extends FmBase {
  
  abstract public function table(array $objects);
  abstract public function admin_form();
  abstract public function get_all_as_options_for_form();

  abstract protected function set_values(stdClass $object);
  abstract protected function set_values_from_form_state(array $form_state);
}