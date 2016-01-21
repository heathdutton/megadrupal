<?php

/**
 * @file
 * The predefined patterns that can be selected in the admin panel
 *
 * @author Glenn De Backer <glenn at coworks dot be>
 *
 * @version 1.1
 */

/**
 * Returns an array of predefined patterns that are available from the administration page
 */
function patterns() {
  // define patterns
  $patterns = array(
      '_none' => 'Select',
      '##.##.##-###.##' => 'Belgian ID (##.##.##-###.##)',
      '##/##/##' => 'Date (##/##/##)',
  );

  return $patterns;
}