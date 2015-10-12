<?php
/**
 * @file
 * Defaults for new instances of MkDru config form
 */
  error_log("LOADING VEFLSKJ");
$mkdru_defaults['settings']['title'] = '';
$mkdru_defaults['settings']['is_service_proxy'] = 1;
$mkdru_defaults['settings']['disable_ranking'] = 0;
$mkdru_defaults['settings']['mergekey'] = '';
$mkdru_defaults['settings']['rank'] = '';
$mkdru_defaults['settings']['autocomplete']['use_autocomplete'] = '0';
$mkdru_defaults['settings']['autocomplete']['alt_url'] = '';
$mkdru_defaults['settings']['facets']['source']['displayName'] = 'Source';
$mkdru_defaults['settings']['facets']['source']['pz2Name'] = 'xtargets';
$mkdru_defaults['settings']['facets']['source']['multiLimit'] = 0;
$mkdru_defaults['settings']['facets']['source']['max'] = 10;
$mkdru_defaults['settings']['facets']['source']['orderWeight'] = 1;
$mkdru_defaults['settings']['facets']['subject']['displayName'] = 'Subject';
$mkdru_defaults['settings']['facets']['subject']['pz2Name'] = 'subject';
$mkdru_defaults['settings']['facets']['subject']['multiLimit'] = 1;
$mkdru_defaults['settings']['facets']['subject']['max'] = 10;
$mkdru_defaults['settings']['facets']['subject']['orderWeight'] = 2;
$mkdru_defaults['settings']['facets']['author']['displayName'] = 'Author';
$mkdru_defaults['settings']['facets']['author']['pz2Name'] = 'author';
$mkdru_defaults['settings']['facets']['author']['multiLimit'] = 0;
$mkdru_defaults['settings']['facets']['author']['max'] = 10;
$mkdru_defaults['settings']['facets']['author']['orderWeight'] = 3;

$mkdru_defaults['sp_user'] = 'guest';
$mkdru_defaults['sp_pass'] = 'guest';
$mkdru_defaults['sp_server_auth'] = 1;
$mkdru_defaults['pz2_path'] = 'http://spdemo.indexdata.com/';

// Module wide settings
$mkdru_admin_defaults['pz2_js_path'] = 'http://mkdru.indexdata.com/sites/all/libraries/pazpar2-js/pz2.js';

