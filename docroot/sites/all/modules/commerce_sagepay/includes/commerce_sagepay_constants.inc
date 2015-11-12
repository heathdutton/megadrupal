<?php

/**
 * @file
 * commerce_sagepay_constants.
 * Provides constants for most other Commerce modules
 */

// Define variable names used to store variables.
define('SAGEPAY_INTEGRATION_MODE', 'sagepay_integration_mode');
define('SAGEPAY_SETTINGS', 'sagepay_settings');

// Define variables to indicate the transaction mode.
define('SAGEPAY_TXN_MODE_LIVE', 'live');
define('SAGEPAY_TXN_MODE_TEST', 'test');
define('SAGEPAY_TXN_MODE_SHOWPOST', 'showpost');
define('SAGEPAY_TXN_MODE_SIMULATION', 'simulation');

// Which methods are enabled
define('SAGEPAY_SETTING_DIRECT_ENABLED', 'sagepay_direct_enabled');
define('SAGEPAY_SETTING_FORM_ENABLED', 'sagepay_form_enabled');
define('SAGEPAY_SETTING_SERVER_ENABLED', 'sagepay_server_enabled');


// Define variable to indicate which version of the SagePay protocol.
define('SAGEPAY_PROTOCOL', '3.0');

// Define Server URLs for all integration methods.
define('SAGEPAY_DIRECT_SERVER_LIVE', 'https://live.sagepay.com/gateway/service/vspdirect-register.vsp');
define('SAGEPAY_DIRECT_SERVER_TEST', 'https://test.sagepay.com/gateway/service/vspdirect-register.vsp');
define('SAGEPAY_DIRECT_SERVER_SHOWPOST', 'https://test.sagepay.com/showpost/showpost.asp');
define('SAGEPAY_DIRECT_SERVER_SIMULATION', 'https://test.sagepay.com/Simulator/VSPDirectGateway.asp');
define('SAGEPAY_DIRECT_SERVER_3D_SECURE_CALLBACK_LIVE', 'https://live.sagepay.com/gateway/service/direct3dcallback.vsp');
define('SAGEPAY_DIRECT_SERVER_3D_SECURE_CALLBACK_TEST', 'https://test.sagepay.com/gateway/service/direct3dcallback.vsp');
define('SAGEPAY_FORM_SERVER_LIVE', 'https://live.sagepay.com/gateway/service/vspform-register.vsp');
define('SAGEPAY_FORM_SERVER_TEST', 'https://test.sagepay.com/gateway/service/vspform-register.vsp');
define('SAGEPAY_FORM_SERVER_SIMULATION', 'https://test.sagepay.com/Simulator/VSPFormGateway.asp');
define('SAGEPAY_SERVER_SERVER_LIVE', 'https://live.sagepay.com/gateway/service/vspserver-register.vsp');
define('SAGEPAY_SERVER_SERVER_TEST', 'https://test.sagepay.com/gateway/service/vspserver-register.vsp');
define('SAGEPAY_SERVER_SERVER_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorRegisterTx');

define('SAGEPAY_SERVER_REPEAT_SERVER_LIVE', 'https://live.sagepay.com/gateway/service/repeat.vsp');
define('SAGEPAY_SERVER_REPEAT_SERVER_TEST', 'https://test.sagepay.com/gateway/service/repeat.vsp');
define('SAGEPAY_SERVER_REPEAT_SERVER_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorRepeatTx');

define('SAGEPAY_SHARED_ABORT_TRANSACTION_LIVE', 'https://live.sagepay.com/gateway/service/abort.vsp');
define('SAGEPAY_SHARED_ABORT_TRANSACTION_TEST', 'https://test.sagepay.com/gateway/service/abort.vsp');
define('SAGEPAY_SHARED_ABORT_TRANSACTION_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorReleaseTx');

define('SAGEPAY_SHARED_RELEASE_TRANSACTION_LIVE', 'https://live.sagepay.com/gateway/service/release.vsp');
define('SAGEPAY_SHARED_RELEASE_TRANSACTION_TEST', 'https://test.sagepay.com/gateway/service/release.vsp');
define('SAGEPAY_SHARED_RELEASE_TRANSACTION_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorReleaseTx');

define('SAGEPAY_SHARED_REFUND_TRANSACTION_LIVE', 'https://live.sagepay.com/gateway/service/refund.vsp');
define('SAGEPAY_SHARED_REFUND_TRANSACTION_TEST', 'https://test.sagepay.com/gateway/service/refund.vsp');
define('SAGEPAY_SHARED_REFUND_TRANSACTION_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorRefundTx');

define('SAGEPAY_SHARED_VOID_TRANSACTION_LIVE', 'https://live.sagepay.com/gateway/service/void.vsp');
define('SAGEPAY_SHARED_VOID_TRANSACTION_TEST', 'https://test.sagepay.com/gateway/service/void.vsp');
define('SAGEPAY_SHARED_VOID_TRANSACTION_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorVoidTx');

define('SAGEPAY_SHARED_AUTHORISE_TRANSACTION_LIVE', 'https://live.sagepay.com/gateway/service/authorise.vsp');
define('SAGEPAY_SHARED_AUTHORISE_TRANSACTION_TEST', 'https://test.sagepay.com/gateway/service/authorise.vsp');
define('SAGEPAY_SHARED_AUTHORISE_TRANSACTION_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorAuthoriseTx');

define('SAGEPAY_SHARED_CANCEL_TRANSACTION_LIVE', 'https://live.sagepay.com/gateway/service/cancel.vsp');
define('SAGEPAY_SHARED_CANCEL_TRANSACTION_TEST', 'https://test.sagepay.com/gateway/service/cancel.vsp');
define('SAGEPAY_SHARED_CANCEL_TRANSACTION_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorVoidTx');

define('SAGEPAY_SHARED_REPEAT_TRANSACTION_LIVE', 'https://live.sagepay.com/gateway/service/repeat.vsp');
define('SAGEPAY_SHARED_REPEAT_TRANSACTION_TEST', 'https://test.sagepay.com/gateway/service/repeat.vsp');
define('SAGEPAY_SHARED_REPEAT_TRANSACTION_SIMULATION', 'https://test.sagepay.com/Simulator/VSPServerGateway.asp?Service=VendorRepeatTx');

define('SAGEPAY_SERVER_TOKEN_REGISTER_LIVE', 'https://live.sagepay.com/gateway/service/token.vsp');
define('SAGEPAY_SERVER_TOKEN_REGISTER_TEST', 'https://test.sagepay.com/gateway/service/token.vsp');

define('SAGEPAY_DIRECT_TOKEN_REGISTER_LIVE', 'https://live.sagepay.com/gateway/service/directtoken.vsp');
define('SAGEPAY_DIRECT_TOKEN_REGISTER_TEST', 'https://test.sagepay.com/gateway/service/directtoken.vsp');

define('SAGEPAY_TOKEN_REMOVE_LIVE', 'https://live.sagepay.com/gateway/service/removetoken.vsp');
define('SAGEPAY_TOKEN_REMOVE_TEST', 'https://test.sagepay.com/gateway/service/removetoken.vsp');

define('SAGEPAY_PAYPAL_COMPLETION_TEST', 'https://test.sagepay.com/gateway/service/complete.vsp');
define('SAGEPAY_PAYPAL_COMPLETION_LIVE', 'https://live.sagepay.com/gateway/service/complete.vsp');

// Define settings variable names.
define('SAGEPAY_SETTING_VENDOR_NAME', 'sagepay_vendor');
define('SAGEPAY_SETTING_VENDOR_EMAIL', 'sagepay_vendor_email');
define('SAGEPAY_SETTING_TRANSACTION_TYPE', 'sagepay_txn_type');
define('SAGEPAY_SETTING_TRANSACTION_MODE', 'sagepay_txn_mode');
define('SAGEPAY_SETTING_TRANSACTION_PREFIX', 'sagepay_txn_prefix');
define('SAGEPAY_SETTING_ENCRYPTION_KEY', 'sagepay_enc_key');
define('SAGEPAY_SETTING_TEST_ENCRYPTION_KEY', 'sagepay_test_enc_key');
define('SAGEPAY_SETTING_SEND_EMAIL', 'sagepay_send_email');
define('SAGEPAY_SETTING_EMAIL_MESSAGE', 'sagepay_email_message');
define('SAGEPAY_SETTING_ORDER_DESCRIPTION', 'sagepay_order_description');
define('SAGEPAY_SETTING_APPLY_AVS_CV2', 'sagepay_apply_avs_cv2');
define('SAGEPAY_SETTING_APPLY_3D_SECURE', 'sagepay_apply_3d_secure');
define('SAGEPAY_SETTING_SEND_CART_CONTENTS', 'sagepay_send_basket_contents');
define('SAGEPAY_SETTING_GIFT_AID', 'sagepay_gift_aid');
define('SAGEPAY_SETTING_ACCOUNT_TYPE', 'sagepay_account_type');
define('SAGEPAY_SETTING_USE_TEST_DATA', 'sagepay_use_test_data');
define('SAGEPAY_SETTING_BYPASS_COMMERCE_CARD_VALIDATION', 'sagepay_bypass_commerce_card_validation');
define('SAGEPAY_SETTING_DEBUG_MODE', 'sagepay_debug_mode');
define('SAGEPAY_SETTING_ENABLED_CARDS', 'sagepay_enabled_cards');
define('SAGEPAY_SETTING_USE_IFRAME', 'sagepay_use_iframe');
define('SAGEPAY_SETTING_LOW_PROFILE', 'sagepay_use_low_profile_pages');
define('SAGEPAY_SETTING_REQUEST_TOKEN', 'sagepay_request_token');

// Define Settings for integration method.
define('SAGEPAY_FORM', 'form');
define('SAGEPAY_SERVER', 'server');
define('SAGEPAY_DIRECT', 'direct');
define('SAGEPAY_TOKEN', 'token');
define('SAGEPAY_PAYPAL', 'paypal');

// Define validation rule constants.
define('SAGEPAY_VALIDATE_NAME', 'name');
define('SAGEPAY_VALIDATE_ADDRESS', 'address');
define('SAGEPAY_VALIDATE_POSTCODE', 'postcode');
define('SAGEPAY_VALIDATE_EMAIL', 'email');

// Define remote status codes for SagePay.
define('SAGEPAY_REMOTE_STATUS_DEFERRED', 'DEFERRED');
define('SAGEPAY_REMOTE_STATUS_REPEAT_DEFERRED', 'REPEAT_DEFERRED');
define('SAGEPAY_REMOTE_STATUS_AUTHENTICATE', 'AUTHENTICATE');
define('SAGEPAY_REMOTE_STATUS_REGISTERED', 'REGISTERED');
define('SAGEPAY_REMOTE_STATUS_FAIL', 'FAIL');
define('SAGEPAY_REMOTE_STATUS_INVALID', 'INVALID');
define('SAGEPAY_REMOTE_STATUS_STARTED', 'STARTED');
define('SAGEPAY_REMOTE_STATUS_OK', 'OK');
define('SAGEPAY_REMOTE_STATUS_UNKNOWN', 'UNKNOWN');
define('SAGEPAY_REMOTE_STATUS_PAYMENT', 'PAYMENT');
define('SAGEPAY_REMOTE_STATUS_REFUNDED', 'REFUNDED');
define('SAGEPAY_REMOTE_STATUS_CANCELLED', 'CANCELLED');
define('SAGEPAY_REMOTE_STATUS_3D_SECURE', '3DSECURE');
define('SAGEPAY_REMOTE_STATUS_PAYPAL_REDIRECT', 'PPREDIRECT');

