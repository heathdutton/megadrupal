<?php
/**
 * @file
 * iati_feeds.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function iati_feeds_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'iati_activity';
  $feeds_importer->config = array(
    'name' => 'IATI activity',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
        'auto_scheme' => 'http',
        'accept_invalid_cert' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExXml',
      'config' => array(
        'context' => array(
          'value' => '//iati-activity',
        ),
        'sources' => array(
          'title' => array(
            'name' => 'title',
            'value' => 'title',
            'raw' => 0,
            'debug' => 0,
            'weight' => '1',
          ),
          'iati_identifier' => array(
            'name' => 'IATI identifier',
            'value' => 'iati-identifier',
            'raw' => 0,
            'debug' => 0,
            'weight' => '2',
          ),
          'description' => array(
            'name' => 'Description',
            'value' => 'description',
            'raw' => 0,
            'debug' => 0,
            'weight' => '3',
          ),
          'activity_status' => array(
            'name' => 'Activity status',
            'value' => 'activity-status',
            'raw' => 0,
            'debug' => 0,
            'weight' => '4',
          ),
          'colaboration_type' => array(
            'name' => 'Colaboration type',
            'value' => 'collaboration-type',
            'raw' => 0,
            'debug' => 0,
            'weight' => '5',
          ),
          'default_flow_type' => array(
            'name' => 'Default flow type',
            'value' => 'default-flow-type',
            'raw' => 0,
            'debug' => 0,
            'weight' => '6',
          ),
          'default_finace_type' => array(
            'name' => 'Default finace type',
            'value' => 'default-finance-type',
            'raw' => 0,
            'debug' => 0,
            'weight' => '7',
          ),
          'default_aid_type' => array(
            'name' => 'Default aid type',
            'value' => 'default-aid-type',
            'raw' => 0,
            'debug' => 0,
            'weight' => '8',
          ),
          'location_raw' => array(
            'name' => 'Location raw',
            'value' => 'location',
            'raw' => 1,
            'debug' => 1,
            'weight' => '9',
          ),
          'reporting_organisation' => array(
            'name' => 'Reporting organisation',
            'value' => 'reporting-org/@ref',
            'raw' => 0,
            'debug' => 0,
            'weight' => '10',
          ),
        ),
        'display_errors' => 0,
        'debug_mode' => 0,
        'source_encoding' => array(
          0 => 'auto',
        ),
        'use_tidy' => FALSE,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'iati_identifier',
            'target' => 'field_iati_identifier',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'iati_identifier',
            'target' => 'guid',
            'unique' => 1,
          ),
          3 => array(
            'source' => 'default_aid_type',
            'target' => 'field_iati_activity_aid_type',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'default_finace_type',
            'target' => 'field_iati_activity_finance_type',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'default_flow_type',
            'target' => 'field_iati_activity_flow_type',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'iati_activity',
        'update_non_existent' => 'skip',
      ),
    ),
    'content_type' => 'iati_activity_importer',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
    'weight' => 0,
  );
  $export['iati_activity'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'iati_activity_organisation';
  $feeds_importer->config = array(
    'name' => 'IATI activity: organisation',
    'description' => 'Organisations referenced in the IATI file',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
        'auto_scheme' => 'http',
        'accept_invalid_cert' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExXml',
      'config' => array(
        'use_tidy' => FALSE,
        'sources' => array(
          'reporting_organisation_name' => array(
            'name' => 'Reporting organisation name',
            'value' => 'reporting-org',
            'raw' => 0,
            'debug' => 0,
            'weight' => '1',
          ),
          'reporting_organisation_reference' => array(
            'name' => 'Reporting organisation: Reference IATI identifier',
            'value' => 'reporting-org/@ref',
            'raw' => 0,
            'debug' => 0,
            'weight' => '2',
          ),
          'reporting_organisation_type' => array(
            'name' => 'Reporting organisation: type',
            'value' => 'reporting-org/@type',
            'raw' => 0,
            'debug' => 0,
            'weight' => '3',
          ),
        ),
        'context' => array(
          'value' => '//iati-activity',
        ),
        'display_errors' => 0,
        'source_encoding' => array(
          0 => 'auto',
        ),
        'debug_mode' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'reporting_organisation_name',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'reporting_organisation_reference',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'reporting_organisation_reference',
            'target' => 'field_iati_identifier',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'reporting_organisation_type',
            'target' => 'field_iati_org_type',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'iati_organisation',
      ),
    ),
    'content_type' => 'iati_activity_importer',
    'weight' => '0',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['iati_activity_organisation'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'iati_activity_organisation_relation';
  $feeds_importer->config = array(
    'name' => 'IATI activity: organisation relation',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
        'auto_scheme' => 'http',
        'accept_invalid_cert' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExXml',
      'config' => array(
        'use_tidy' => FALSE,
        'sources' => array(
          'reporting_organisation_reference' => array(
            'name' => 'Organisation: Reference IATI identifier',
            'value' => '@ref',
            'raw' => 0,
            'debug' => 0,
            'weight' => '1',
          ),
          'organisation_role' => array(
            'name' => 'Organisation: Role',
            'value' => '@role',
            'raw' => 0,
            'debug' => 0,
            'weight' => '2',
          ),
          'iati_identifier' => array(
            'name' => 'IATI identifier',
            'value' => '../iati-identifier',
            'raw' => 0,
            'debug' => 0,
            'weight' => '3',
          ),
        ),
        'context' => array(
          'value' => '//participating-org',
        ),
        'display_errors' => 0,
        'source_encoding' => array(
          0 => 'auto',
        ),
        'debug_mode' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'RelationFeedsProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'unique_enpoints' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'reporting_organisation_reference',
            'target' => 'target_bundles:node:iati_organisation:guid',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'iati_identifier',
            'target' => 'source_bundles:node:iati_activity:guid',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'organisation_role',
            'target' => 'field_organisation_role',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'iati_organisation_role',
      ),
    ),
    'content_type' => 'iati_activity_importer',
    'weight' => '0',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['iati_activity_organisation_relation'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'iati_activity_other_identifier';
  $feeds_importer->config = array(
    'name' => 'IATI activity: other identifier',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExXml',
      'config' => array(
        'use_tidy' => FALSE,
        'sources' => array(
          'iati_identifier' => array(
            'name' => 'IATI identifier',
            'value' => 'iati-identifier',
            'raw' => 0,
            'debug' => 0,
            'weight' => '1',
          ),
          'other_identifier' => array(
            'name' => 'Other identifier',
            'value' => 'other-identifier',
            'raw' => 0,
            'debug' => 0,
            'weight' => '2',
          ),
          'other_identifier_owner_referenc' => array(
            'name' => 'Other identifier: owner referenc',
            'value' => 'other-identifier/@owner-ref',
            'raw' => 0,
            'debug' => 0,
            'weight' => '3',
          ),
          'other_identifier_owner_name' => array(
            'name' => 'Other identifier: owner name',
            'value' => 'other-identifier/@owner-name',
            'raw' => 0,
            'debug' => 0,
            'weight' => '4',
          ),
        ),
        'context' => array(
          'value' => '//iati-activity',
        ),
        'display_errors' => 1,
        'source_encoding' => array(
          0 => 'auto',
        ),
        'debug_mode' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsFieldCollectionProcessor',
      'config' => array(
        'field_name' => 'field_other_identifier',
        'host_entity_type' => 'node',
        'is_field' => 1,
        'guid_field_name' => 'field_iati_identifier',
        'identifier_field_name' => 'field_identifier_identifier',
        'mappings' => array(
          0 => array(
            'source' => 'other_identifier',
            'target' => 'field_identifier_identifier',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'other_identifier_owner_name',
            'target' => 'field_owner_name',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'other_identifier_owner_referenc',
            'target' => 'field_owner_ref',
            'term_search' => '0',
            'autocreate' => 1,
          ),
          3 => array(
            'source' => 'other_identifier',
            'target' => 'guid',
            'unique' => 1,
          ),
          4 => array(
            'source' => 'iati_identifier',
            'target' => 'host_entity_guid',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'other_identifier',
            'target' => 'identifier_field',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'field_other_identifier',
      ),
    ),
    'content_type' => 'iati_activity_importer',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['iati_activity_other_identifier'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'iati_activity_parcticipating_organisation';
  $feeds_importer->config = array(
    'name' => 'IATI activity: parcticipating organisation',
    'description' => 'Creates new organisations for listed participating orgs that do not yet have an organisation node.',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
        'auto_scheme' => 'http',
        'accept_invalid_cert' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExXml',
      'config' => array(
        'use_tidy' => FALSE,
        'sources' => array(
          'reporting_organisation_name' => array(
            'name' => 'Organisation name',
            'value' => 'text()',
            'raw' => 0,
            'debug' => 0,
            'weight' => '1',
          ),
          'reporting_organisation_reference' => array(
            'name' => 'Organisation: Reference IATI identifier',
            'value' => '@ref',
            'raw' => 0,
            'debug' => 0,
            'weight' => '2',
          ),
          'reporting_organisation_type' => array(
            'name' => 'Organisation: type',
            'value' => '@type',
            'raw' => 0,
            'debug' => 0,
            'weight' => '3',
          ),
        ),
        'context' => array(
          'value' => '//participating-org',
        ),
        'display_errors' => 0,
        'source_encoding' => array(
          0 => 'auto',
        ),
        'debug_mode' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'reporting_organisation_name',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'reporting_organisation_reference',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'reporting_organisation_reference',
            'target' => 'field_iati_identifier',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'reporting_organisation_type',
            'target' => 'field_iati_org_type',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'iati_organisation',
      ),
    ),
    'content_type' => 'iati_activity_importer',
    'weight' => '0',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['iati_activity_parcticipating_organisation'] = $feeds_importer;

  return $export;
}
