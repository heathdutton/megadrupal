<?php

/**
 * @file
 * Test methods to verify xmlrpctester functionality with.
 */

function xmlrpctester_struct_string($string) {
  return array('first parameter', 'second parameter');
}

function xmlrpctester_struct_struct($struct) {
  return $struct;
}

function xmlrpctester_string_int($int) {
  return gettype($int);
}

function xmlrpctester_int_boolean($boolean) {
  return gettype($boolean);
}
