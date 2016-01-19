<?php
/**
 * @file
 * MMS configuration.
 */

define('MMS_DRUPAL_VERSION', 'mms_drupal_version');
define('MMS_UIOPTIONS_FOLLOW', 'mms_uioptions_follow');
define('MMS_UIOPTIONS_RAW', 'mms_uioptions_raw');
define('MMS_MULTIFY_FROMLANG', 'mms_multify_fromlang');
define('MMS_MULTIFY_BACKLANG', 'mms_multify_backlang');
define('MMS_NO_WIDGET', 'mms_no_widget');
define('MMS_FALLBACK_EMPTY', 'mms_fallback_empty');
define('MMS_FALLBACK_OPTION', 'mms_fallback_option');
define('MMS_FALLBACK_MESSAGE', 'mms_fallback_message');
define('MMS_FALLBACK_TIP', 'mms_fallback_tip');
define('MMS_FALLBACK_CSS', 'mms_fallback_css');
define('MMS_EXCLUDE_PATHS', 'mms_exclude_paths');
define('MMS_LINEBREAKS_USE', 'mms_linebreaks_use');
define('MMS_TOKENS', 'mms_tokens');
define('MMS_FIELD_TYPES', 'mms_field_types');
define('MMS_UITRANSLATE_OPERATION', 'mms_uitranslate_operation');
/* =================================================== DEFAULT CONFIG GETTERS */

// DATA ENTRY / INTERFACE
function _mms_uioptions_follow_default() {
  return array((int)TRUE); // or FALSE
}
function _mms_uioptions_raw_default() {
  return array((int)FALSE); // or TRUE
}

// DATA ENTRY / MULTIFY
function _mms_multify_fromlang_default() {
  return array('default'); // or 'current', or 'ask'
}
function _mms_multify_backlang_default() {
  return array('default'); // or 'current', or 'ask'
}

// DATA ENTRY / NO WIDGET
function _mms_no_widget_default() {
  // CSS selectors to exclude elements from attaching widget (client side).
  return array(
  
    // In whole forms:
    '#contact-site-form *',
    '#ctools-export-ui-list-form *',
    '#locale-languages-edit-form *',
    '#pathauto-patterns-form *',
    '#user-login *',
    '#user-pass *',
    '#user-register-form *',
    
    // In several forms:
    '#edit-weight', // most forms
    '#edit-date', // most forms
    '#edit-pages', // block/admin/configure
    '#edit-path-alias', // pathauto: node-form, taxonomy-form-term
    // In types/manage/.../fields/...:
    '#edit-instance-settings-file-directory',
    '#edit-instance-settings-file-extensions',
    '#edit-instance-settings-max-filesize',
    '#edit-instance-widget-settings--2 input',
    '#edit-instance-widget-settings-rows',
    
    // specific elements:
    '#system-site-information-settings #edit-front-page *',
    '#system-site-information-settings #edit-error-page *',
    '#mms-rendering-form textarea',
    '#menu-edit-menu #edit-menu-name', // 
    '.node-form .link-field-url input',
    #'#edit-search-block-form--2', ???
    '#edit-link-path', // main menu
    #'#edit-menu-link-title', // taxonomy term:pathauto ???
    '#edit-module-filter-name', // system_modules
    '#edit-page-path', // views_ui_add_form
    // ??? links: may be to keep ???
  );
}

// RENDERING / FALLBACK
function _mms_fallback_empty_default() {
  return array('replace'); // or 'empty'
}
function _mms_fallback_option_default() {
  return array('default'); // or 'message'
}
function _mms_fallback_message_default() {
  // CAUTION: get UNTRANSLATED "multi" segment from mms.data.inc
  module_load_include('inc', 'mms', 'mms.data');
  return array(_mms_fallback_message_data());
}
function _mms_fallback_tip_default() {
  // CAUTION: get UNTRANSLATED "multi" segment from mms.data.inc
  module_load_include('inc', 'mms', 'mms.data');
  return array(_mms_fallback_tip_data());
}
function _mms_fallback_css_default() {
  return array(
    'background-color: #aaf;',
  );
}

/*
TODO: enlarge max allowed length for labels (currently 128) [and others?]
*/
// RENDERING / EXCLUDE_PATHS
function _mms_exclude_paths_default() {
// Pages where MMS must NOT process "multi" segments (server side).
  return array(
    'types/(add|manage/[^/]+($|(/fields/[^/]+)?(/delete)?$))',
    /* Above regex looks for:
    types/add
    types/manage/..contentType.. (= edit type)
    types/manage/..contentType../delete
    types/manage/..contentType/fields/..fieldName.. (= edit field)
    types/manage/..contentType../fields/..fieldName../delete
      But NOT:
    types/manage/..contentType/fields (fields list)
    types/manage/..contentType/fields/..fieldName../... (field sub-tabs)
    */
    'node/(add|(\d+/(edit|delete)))',
    /* Above regex looks for:
    node/add
    node/..nodeId../edit
    node/..nodeId../delete
    */
    'block/(add|manage/block/\d+/(configure|delete))',
    /* Above regex looks for:
    block/add
    block/manage/block/..blockId../configure (= edit)
    block/manage/block/..blockId../delete
    */
    'menu/(add|(manage/[^/]+|item/\d+)/(edit|delete))',
    /* Above regex looks for:
    menu/add
    menu/manage/..menuName../edit
    menu/manage/..menuName../delete
    menu/manage/..menuName../add (= add item)
    menu/item/..menuItemId../edit
    menu/item/..menuItemId../delete
      But NOT:
    menu/manage/..menuName.. (items list)
    */
    'contact/(add|(edit|delete)/\d+)',
    /* Above regex looks for:
    contact/add
    contact/..contactId../edit
    contact/..contactId../delete
    */
    'taxonomy/(add|[^/]+/(add|edit)|term/\d+/edit)',
    /* Above regex looks for:
    taxonomy/add
    taxonomy/..vocabularyName../edit (offers "delete")
    taxonomy/..vocabularyName../add (= add term)
    taxonomy/term/..termId../edit (offers "delete")
    */
    'config/regional/mms/rendering',
    'config/system/site-information',
    );
}

// RENDERING / LINEBREAKS
function _mms_linebreaks_use_default() {
  // CSS selectors for elements where linebreaks must be effective (client side).
  return array(
    '#main-menu a',
    '.block h2',
    '.node .title a',
    '#post-content .page-title',
  );
}

// TOKENS / CLONING
function _mms_tokens_default() {
  // Raw tokens MMS will clone.
  return array(
    'node' => array(
      'body',
      'log',
      'summary',
      'title',
    ),
    'term' => array(
      'name',
      'description',
      'vocabulary',
    ),
    'vocabulary' => array(
      'name',
      'description',
    ),
  );
}

// TOKENS / FIELDS
function _mms_field_types_default() {
  // Field types allowed for MMS field-tokens cloning.
  foreach (array(
    'taxonomy_term_reference',
    'text',
    'text_long',
    'text_with_summary',
    ) as $field_type
  ) {
    $return[$field_type] = $field_type;
  }
  return $return;
}
/* ========================================================== ANCILIARY FUNCS */

function _mms_conf($var, $glue = "\n", $prefix = NULL, $suffix = NULL) {
  // Return current value of $var option:
  // . as a flat string, depending on $glue, $prefix, and $suffix
  // . or as the raw array, if $glue is false
  $default = '_' . $var . '_default';
  $raw = variable_get($var, $default());
  return $glue ? ($prefix . implode($glue, $raw) . $suffix) : $raw;
}
function _mms_conf_set($string) {
  // Return an array from multiline $string.
  return array_filter(
    // Normalize line-breaks.
    explode("\n", str_replace(array("\r\n", "\r"),"\n", $string)),
    // Drop empty lines.
    function($value) {return (bool)$value;}
  );
}
function _mms_exclude_paths() {
  // Specific: returns regexp built from current MMS_EXCLUDE_PATHS option.
  return _mms_conf(MMS_EXCLUDE_PATHS, '|', '#(', ')#');
}
/* ========================================================================== */
