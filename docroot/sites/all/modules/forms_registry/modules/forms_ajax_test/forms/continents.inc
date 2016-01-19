<?php
/**
 * @file
 * A sample for continents config file.
 */

/**
 * Returns all the continents.
 *
 * @return array
 */
function forms_ajax_test_get_continents() {
  return array(
    'north_america' => t('North America'),
    'south_america' => t('South America'),
    'europe' => t('Europe'),
    'africa' => t('Africa'),
    'asia' => t('Asia'),
    'australia' => t('Australia'),
  );
}