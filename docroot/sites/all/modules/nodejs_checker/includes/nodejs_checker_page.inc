<?php
/**
 * @file
 * Page callback for checking the Nodejs server.
 */

/**
 * Simply return the server state variable as JSON.
 */
function nodejs_checker_page() {
  $state = nodejs_checker_check_server();
  drupal_json_output($state);
}
