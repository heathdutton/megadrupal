<?php

/**
 * @file
 * Authorize.net - AIM test code
 */

/**
 * The following are some sample PHP snippets for testing the API wrapper functions.
 * Just paste this into the Devel module's /devel/php to test it out.
 * It may not be SimpleTest, but it's better than nothing.
 */

/**
 * Create a new transaction via AIM.
 */

$sale = array(
  'amount' => '1999.99',
  'card_num' => '6011000000000012',
  'exp_date' => '04/15',
);
$response = authnet_aim_transaction($sale, AUTHNET_METHOD_AUTH_ONLY);
dpm($response);
