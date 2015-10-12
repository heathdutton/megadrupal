<?php

/**
 * @file
 * Provides constants for most other Commerce modules.
 */

// Define variables to indicate the transaction mode.
define('COMMERCE_SMARTPAY_MODE_LIVE', 'live');
define('COMMERCE_SMARTPAY_MODE_TEST', 'test');

// Define Server URLs.
define('COMMERCE_SMARTPAY_SERVER_LIVE', 'https://live.barclaycardsmartpay.com/hpp/pay.shtml');
define('COMMERCE_SMARTPAY_SERVER_TEST', 'https://test.barclaycardsmartpay.com/hpp/pay.shtml');

// Define remote status codes.
define('COMMERCE_SMARTPAY_REMOTE_STATUS_FAIL', 'FAIL');
define('COMMERCE_SMARTPAY_REMOTE_STATUS_CANCELLED', 'CANCELLED');
define('COMMERCE_SMARTPAY_REMOTE_STATUS_PENDING', 'PENDING');
define('COMMERCE_SMARTPAY_REMOTE_STATUS_OK', 'OK');

// Define settings variable names.
define('COMMERCE_SMARTPAY_MERCHANT_ACCOUNT', 'commerce_smartpay_merchant_account');
define('COMMERCE_SMARTPAY_SKIN_CODE', 'commerce_smartpay_skin_code');
define('COMMERCE_SMARTPAY_SHARED_SECRET_TEST', 'commerce_smartpay_shared_secret_test');
define('COMMERCE_SMARTPAY_SHARED_SECRET_LIVE', 'commerce_smartpay_shared_secret_live');
define('COMMERCE_SMARTPAY_TRANSACTION_MODE', 'commerce_smartpay_transaction_mode');
