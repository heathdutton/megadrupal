<?php
/**
 * @file
 * Transformer functions back and forth schema and Solr fields
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Convert the properties to array of Solr parameters
 * @param $params (Object)
 *   The properties record
 * @param $facet_name (String)
 *   The name of the facet (default: NULL).
 * @return (Array)
 *   The array of Solr parameters
 */
function xc_search_field_facet_properties2solr_params($params, $facet_name = NULL) {

  $prefix = (is_null($facet_name) ? '' : 'f.' . $facet_name . '.')
    . 'facet.';

  return array(
    $prefix . 'prefix' => $params->prefix,
    $prefix . 'sort' => $params->sort,
    $prefix . 'limit' => $params->limitation,
    $prefix . 'offset' => $params->offset,
    $prefix . 'mincount' => $params->end,
    $prefix . 'missing' => $params->missing,
    $prefix . 'method' => $params->method,
  );
}

/**
 * Convert the properties to array of Solr parameters
 * @param $params (Object)
 *   The properties record
 * @param $facet_name (String)
 *   The name of the facet (default: NULL).
 * @return (Array)
 *   The array of Solr parameters
 */
function xc_search_date_facet_properties2solr_params($params, $facet_name = NULL) {

  return xc_solr_date_facet_factory($facet_name, $params->start, $params->end, $params->gap);

  /*
   $prefix = is_null($facet_name) ? '' : 'f.' . $facet_name . '.facet.';
   return array(
   'facet.date'             => $facet_name, // TODO: not sure about this
   $prefix . 'date.start'   => $params->start,
   $prefix . 'date.end'     => $params->end,
   $prefix . 'date.gap'     => $params->gap,
   $prefix . 'date.hardend' => $params->hardend ? 'true' : 'false',
   $prefix . 'date.other'   => $params->other,
   );
   */
}

/**
 * Convert the properties to array of Solr parameters
 * @param $params (Object)
 *   The properties record
 * @param $facet_name (String)
 *   The name of the facet (default: NULL).
 * @return (Array)
 *   The array of Solr parameters
 */
function xc_search_query_facet_properties2solr_params($params, $facet_name = NULL) {

  $prefix = (is_null($facet_name) ? '' : 'f.' . $facet_name . '.')
    . 'facet.';

  return array(
    $prefix . 'prefix' => $params->prefix,
    $prefix . 'sort' => $params->sort,
    $prefix . 'limit' => $params->limitation,
    $prefix . 'offset' => $params->offset,
    $prefix . 'mincount' => $params->end,
    $prefix . 'missing' => $params->missing,
    $prefix . 'method' => $params->method,
  );
}

