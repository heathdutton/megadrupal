-- SUMMARY --

Idea behind this module is to have settings for each instance of the site
separated, including multi-site setups. It allows to enable/disable modules
based on variables defined in settings.php file, also it allows to set
variables.

Settings can be applied on each page load or using drush command, based on
module settings.

All config is done through settings.php file.

For example we can have multi-site setup with different sites for different
languages (uk, fr, de). Each of the sites (uk, fr, de) can have instances like
LOCAL, DEVELOPMENT, STAGING, PRODUCTION, so we will have 12 instances with 12
different databases.

Following setup works well with Acquia hosting.

Example settings.php setup:

sites/uk directory:

* sites/uk/settings.php (not ignored by Git!)

// Set environment language.
$conf['instance_settings_language'] = 'uk';

// Required to connect to Acquia DB.
// Not included on other servers.
if (file_exists('/var/www/site-php')) {
  require('/var/www/site-php/YOUR-DB-ON-ACQUIA-settings.inc');
}

// Not included on Acquia servers.
// Put local environment specific settings here (Like DB settings).
// This is only settings file ignored by Git.
@include 'local.settings.php';

// Include environment specific setting files.
@include rtrim(DRUPAL_ROOT, '/') . '/sites/default/include.settings.inc';

* sites/uk/local.settings.php (the only settings file ignored by Git!)
// Set environment.
$conf['instance_settings_environment'] = 'dev';

// Database setup
$databases = array(....)

* /sites/default/include.settings.inc

// Provide environment for Acquia servers.
if (file_exists('/var/www/site-scripts/site-info.php')) {
  require_once '/var/www/site-scripts/site-info.php';
  list($bah_site_name, $bah_site_group, $bah_site_stage, $secret) =
    ah_site_info();

  $conf['instance_settings_environment'] = $bah_site_stage;
}

$settings_dir = rtrim(DRUPAL_ROOT, '/') . '/sites/default/';

// Config related to all sites goes here
@include $settings_dir . 'all.settings.inc';

// Instance (dev, staging prod) specific settings
@include $settings_dir . $conf['instance_settings_environment'] .
  '.settings.inc';

// Language (uk, de, fr) specific settings
@include $settings_dir . $conf['instance_settings_language'] . '.settings.inc';

// Language AND language specific settings (dev.uk.settings.php)
@include $settings_dir . $conf['instance_settings_environment'] . '.' .
  $conf['instance_settings_language'] . '.settings.inc';

* sites/default/all.settings.inc

// Default setting, overridden later if needed.
$update_free_access = FALSE;

// Configuring module pools (useful to enable features depending on instance).
// Only one module can be active in each pool.
$conf['instance_settings_module_pools'] = array(
  // This is a pool, only one of the following modules can be enabled.
  'environment' => array( // Pool name
    'feature_environment_local', // Example feature or module here
    'feature_environment_dev', // Example feature or module here
    'feature_environment_stage', // Example feature or module here
    'feature_environment_prod', // Example feature or module here
  ),

  // This is a pool, only one of the following modules can be enabled.
  'language' => array( // Pool name
    'feature_language_uk', // Example feature or module here
    'feature_language_fr', // Example feature or module here
    'feature_language_de', // Example feature or module here
  ),
);


* sites/default/dev.settings.inc

// Allow anyone to run update.php.
$update_free_access = FALSE;

ini_set('display_errors', TRUE);
ini_set('display_startup_errors', TRUE);

// Define variables.
$conf['instance_settings_variables']['preprocess_css'] = 0;
$conf['instance_settings_variables']['preprocess_js'] = 0;
$conf['instance_settings_variables']['cache'] = 1;
$conf['instance_settings_variables']['block_cache'] = 1;
$conf['instance_settings_variables']['expire_include_base_url'] = 0;
$conf['instance_settings_variables']['page_cache_maximum_age'] = 21600;

$conf['instance_settings_enable_modules'] = array(
  'feature_environment_dev', // Feature module only enabled on dev instance
  'shield',
  'bean_admin_ui',
  'dblog',
  'devel',
  'diff',
  'field_ui',
  'views_ui',
  'acquia_agent',
  'acquia_purge',
  'acquia_spi',
  'search_api_acquia',
);

-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual.

-- CONFIGURATION --

* For configuration go into admin/config/system/instance-settings page

* Configure user permissions in Administration » People » Permissions:

  - Administer instance settings module

    Configure who can administer this module.
