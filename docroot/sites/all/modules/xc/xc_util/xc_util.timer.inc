<?php
/**
 * @file
 * Performance timing helper functions.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

function xc_timer_get_description($key) {
  static $descriptions;

  if (!isset($descriptions)) {
    $descriptions = array(
      '01 step1' => t('Harvesting'),
      '01 step1/01 oai' => t('Full OAI-PMH harvesting'),
      '01 step1/01 oai/00 batch' => t('Overhead of the batch API'),
      '01 step1/01 oai/01 fetch_all' => t('Fetch content from the server/cache/FS'),
      '01 step1/01 oai/03 oai_dom' => t('The XML processing (XML->DOM)'),
      '01 step1/01 oai/04 foreach' => t('Processing of each records'),
      '01 step1/01 oai/04 foreach/01 records' => t('Parse records'),
      '01 step1/01 oai/05 hook request_processed' => t('Request processed hook'),
      '01 step1/02 bridge' => t('Processing of each records inside XC OAI Harvester Bridge'),
      '01 step1/02 bridge/01 initialization' => t('Initialize a record (creating an initial XCEntity object)'),
      '01 step1/02 bridge/03 _xc_store/06 hook_xc_store/sql_xc_store' => t('Store metadata'),
      '01 step1/02 bridge/03 sql_xc_store' => t('Store metadata'),
      '01 step1/02 bridge/04 xc_schema' => t('Parsing records as XC schema records'),
      '01 step1/02 bridge/04 xc_schema/01 dom' => t('Creating PHP array from DOM object'),
      '01 step1/02 bridge/04 xc_schema/02 find in array' => t('Finding the xc:entity element in the PHP array'),
      '01 step1/02 bridge/04 xc_schema/relationships' => t('Finding and recording relationships of the XCEntity object'),
      '02 step2' => t('Importing to MySQL'),
      '02 step2/02 load data' => t('Loading CSV data into MySQL'),
      '02 step2/02 load data/xc_entity_properties' => t('Loading xc_entity_properties'),
      '02 step2/02 load data/xc_entity_relationships' => t('Loading xc_entity_relationships'),
      '02 step2/02 load data/xc_sql_metadata' => t('Loading xc_sql_metadata'),
      '03 step3' => t('Solr indexing'),
      '03 step3/00 preparation' => t(''),
      '03 step3/01 SELECT metadata_id' => t(''),
      '03 step3/02 iterator' => t('Iterating over entities'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer' => t('Solr document creation'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/01 metadata merge' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/01 xc_solr_get_metadata' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet' => t('Creating format facet'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/01 prepare' => t('Preparation'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/02 Online' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/03 Microform' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/04 Journals' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/05 Maps' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/06 Musical scores' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/07 dcterms:type' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/08 Films, videos' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/09 DVDs' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet/10 errors' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/03 conditional facets 1 - hooks' => t('Conditional facets: hooks'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/04 conditional facets 2 - forms' => t('Conditional facets: forms'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/05 sortable fields' => t('Sortable fields'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr' => t(''),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/01 new' => t('new Document'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/02 common fields' => t('Adding common fields'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/03 main value' => t('Main field values'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/04 value_as_value' => t('Value as value'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/05 value_as_key' => t('Value as key'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/06 as_key_exclusively' => t('Exclusive key'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/07 date_facet' => t('Creating date facet'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/07 date_facet/xc_solr_to_date' => t('Convert to Solr date format'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/08 facet' => t('Creating normal facets'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/08 facet/01 get values' => t('Get values'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/08 facet/02 add fields' => t('Add to Document'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/09 text' => t('Catch-all field'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/10 addField' => t('Document->addField()'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent' => t('Get parent'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_entity_get_parent_identifiers' => t('Get parent identifiers'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier' => t('Get metadata by identifier'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier/sql' => t('SQL'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier/unserialize' => t('unserialize'),
      '03 step3/02 iterator/02 send to solr' => t('Sending to Solr'),
      '03 step3/02 iterator/02 send to solr/01 XML' => t('Creating XML'),
      '03 step3/02 iterator/02 send to solr/02 send' => t('HTTP'),
      '03 step3/02 iterator/02 send to solr/02 send/01 context' => t('context'),
      '03 step3/02 iterator/02 send to solr/02 send/02 post' => t('POST'),
      '03 step3/02 iterator/02 send to solr/02 send/02 post/01 Solr process' => t('Solr process'),
      '03 step3/02 iterator/02 send to solr/02 send/03 new Apache_Solr_ResponseXc' => t('create response object'),
      '03 step3/02 iterator/02 send to solr/02 send/all' => t('All'),
      '03 step3/03 commit' => t('Commit changes'),
      '03 step3/04 optimize' => t('Optimize database'),
      '04 step4' => t('Node creation'),
      '04 step4/01 preparation' => t('First preparation'),
      '04 step4/02 normal preparation' => t('Subsequent preparations'),
      '04 step4/03 main task' => t('Main task'),
      '04 step4/03 main task/01 sql select' => t('SQL selection'),
      '04 step4/03 main task/02 iteration' => t('Iteration of records'),
      '04 step4/03 main task/02 iteration/01 xc_metadata_create_node' => t('xc_metadata_create_node()'),
      '04 step4/03 main task/02 iteration/01 xc_metadata_create_node/01 node' => t('Node creation'),
      '04 step4/03 main task/02 iteration/01 xc_metadata_create_node/02 update' => t('Update entity properties table'),
      // legacy paths
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/new' => t('new Document'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/common fields' => t('Adding common fields'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/main value' => t('Main field values'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/value_as_value' => t('Value as value'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/value_as_value' => t('Value as value'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/as_key_exclusively' => t('Exclusive key'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/date_facet' => t('Creating date facet'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/date_facet/xc_solr_to_date' => t('Convert to Solr date format'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/facet' => t('Creating normal facets'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/facet/get values' => t('Get values'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/facet/add fields' => t('Add to Document'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/text' => t('Catch-all field'),
      '03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/addField' => t('Document->addField()'),
      // simple values
      'oai' => t('Full OAI-PMH harvesting'),
      'batch' => t('Overhead of the batch API'),
      'oai_response' => t('OAI-PMH response'),
      'http_response' => t('HTTP response time'),
      'oai_dom' => t('The XML processing (XML->DOM)'),
      'foreach' => t('Processing of each records'),
      'hook request_processed' => t('Request processed hook'),
      'bridge' => t('Processing of each records inside XC OAI Harvester Bridge'),
      'record_init' => t('Initialize a record (creating an initial XCEntity object)'),
      '_xc_build_store' => t('Process the record with _xc_build_store function'),
      'set locations' => t('Set location property for XCEntity'),
      'hook_xc_build' => t('Call hook_xc_build, which finalize the XCEntity object'),
      'set object' => t('Set object'),
      'xc_cache_set' => t('Caching XCEntity object'),
      'xc_entity_set_properties' => t('Set XCEntity properties'),
      'set udated' => t('Set the updated property'),
      'set locks' => t('Set the locks property'),
      'set properties' => t('Set properties property'),
      'node_save' => t('Save node'),
      'save SQL' => t('Saving new xc_entity_properties record'),
      'insert SQL' => t('Creating the SQL command for creating new xc_entity_properties record'),
      'xc_entity_set_relationships' => t('Set relationships between entities'),
      'set params' => t('Set parameters'),
      'hook_xc_store' => t('Call hook_xc_store to store metadata'),
      'sql_xc_store' => t('Creating new xc_sql_metadata record'),
      'commit (total)' => t('Total time of hook_xc_commit'),
      'commit (Solr)' => t('Commit in Solr'),
      'xc_schema' => t('Parsing records as XC schema records'),
      'dom' => t('Creating PHP array from DOM object'),
      'find in array' => t('Finding the xc:entity element in the PHP array'),
      'relationships' => t('Finding and recording relationships of the XCEntity object'),
      'init' => t('The initialization part of XCEntity constructor'),
      'metadata_id' => t('Using metadata_id parameter as identifier'),
      'identifier' => t('Getting metadata identifier by OAI identifier'),
      'node_id' => t('Getting metadata identifier by node id'),
      'nid' => t('Getting metadata identifier by nid'),
    );
  }

  return isset($descriptions[$key]) ? $descriptions[$key] : $key;
}

function xc_timer($collection, $key, $signal = 0) {

  if ($signal == 0) {
    $GLOBALS[$collection][$key]['s'] = microtime(TRUE);
  }
  elseif ($signal == 1) {
    $GLOBALS[$collection][$key]['t'] += (microtime(TRUE) - $GLOBALS[$collection][$key]['s']);
  }
  elseif ($signal == 2) {
    $GLOBALS[$collection][$key]['c']++;
  }
}

