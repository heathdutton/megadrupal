<?php

/**
 * Use this file to provide settings provide the SimpleTest environment with
 * required system settings for VerticalResponse.  This file should be
 * copied or renamed to "settings.php."  The new or renamed settings.php
 * file should remain within this directory.  Its system path will be:
 * .../[path-to-modules]/vr/tests/settings.php
 */

// Login Credentials.
// ** Required for all VR tests **
variable_set('vr_username', NULL);
variable_set('vr_password', NULL);

// Campaign variables.  The lable is the name that is displayed in the
// "from address."  The support email is used as the email address.
// ** All are required ** for testing campaigns.
variable_set('vr_campaign_from_label', NULL);
variable_set('vr_campaign_support_email', NULL);
variable_set('vr_test_email', NULL);
