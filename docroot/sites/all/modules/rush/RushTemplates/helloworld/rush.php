<?php
/**
 * @file
 * A demo rush file to print a rush message with a custom name.
 *
 * Print 'Hello Name' if the user_first_name param is set.
 */

if (empty($params)) {
  $params = array();
}
if (!empty($params['env']['user_first_name'])) {
  $rush[]['m'] = 'Hello ' . $params['env']['user_first_name'];
}
