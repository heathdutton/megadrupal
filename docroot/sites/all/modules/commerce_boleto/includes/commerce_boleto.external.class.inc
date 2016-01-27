<?php
/**
 * @file
 * Implements Cielo's payment web service for use with Drupal Commerce.
 */

if (!function_exists('libraries_get_path')) {
  module_load_include('module', 'libraries', 'libraries');
}
include_once drupal_realpath(libraries_get_path('boleto-lib')) . DIRECTORY_SEPARATOR . 'Boleto.class.php';

class CommerceBoletoLib extends Boleto {
  // Register interface.
}
