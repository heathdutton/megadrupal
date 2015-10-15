<?php

/**
 * @file
 * Page display functions for the PMP API module.
 */

/**
 * Page callback: Test page PMP API connection/settings.
 *
 * Callback for admin/config/system/pmpapi_test.
 *
 * @return array
 *   An item list of various test API call stats. 
 *
 * @see pmpapi_menu()
 */
function pmpapi_test_api() {
  $pmp = new PMPAPIDrupal();
  return $pmp->report();
}
