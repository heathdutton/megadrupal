<?php

/**
 * @file
 * Contains Visitors Voice integration for the Search API Solr Search module.
 */

/**
 * Implements hook_search_api_solr_documents_alter().
 *
 * Adds dynamic fields to the indexed documents to encode any customizations for
 * those items.
 */
function visitorsvoice_search_api_solr_documents_alter(array &$documents, SearchApiIndex $index, array $items) {
  /** @var SearchApiSolrDocument[] $item_docs */
  $item_docs = array();
  /** @var SearchApiSolrDocument[] $documents */
  foreach ($documents as $doc) {
    if ($item_id = $doc->getField('item_id')) {
      $item_docs[$item_id['value']] = $doc;
    }
  }
  $customizations = visitorsvoice_customizations_load($index->machine_name, array_keys($item_docs));
  foreach ($customizations as $item_id => $item_customizations) {
    $doc = $item_docs[$item_id];
    /** @var VisitorsvoiceCustomizationInterface $customization */
    foreach ($item_customizations as $customization) {
      switch ($customization->getType()) {
        case VisitorsvoiceCustomizationInterface::TYPE_ADD:
          $doc->setField('bs_visitorsvoice_custom_add_' . $customization->getCustomizationId(), 'true');
          break;
        case VisitorsvoiceCustomizationInterface::TYPE_ORDER:
          $doc->setField('iss_visitorsvoice_custom_order_' . $customization->getCustomizationId(), $customization->getOrder());
          break;
        case VisitorsvoiceCustomizationInterface::TYPE_REMOVE:
          $doc->setField('bs_visitorsvoice_custom_remove_' . $customization->getCustomizationId(), 'true');
          break;
      }
    }
  }
}

/**
 * Implements hook_search_api_solr_query_alter().
 *
 * Alters the Solr query to take the query's active customization into account,
 * if there is one.
 */
function visitorsvoice_search_api_solr_query_alter(array &$call_args, SearchApiQueryInterface $query) {
  if ($option = $query->getOption('visitorsvoice_customization')) {
    $id = (int) $option['id'];
    foreach ($option['customizations'] as $type) {
      switch ($type) {
        case VisitorsvoiceCustomizationInterface::TYPE_ADD:
          if (isset($call_args['query'])) {
            $keys = &$call_args['query'];
          }
          elseif (isset($call_args['params']['q'])) {
            $keys = &$call_args['params']['q'];
          }
          else {
            continue;
          }
          $keys = "($keys) OR bs_visitorsvoice_custom_add_$id:true";
          break;
        case VisitorsvoiceCustomizationInterface::TYPE_ORDER:
          $sort = "iss_visitorsvoice_custom_order_$id asc, ";
          if (!empty($call_args['params']['sort'])) {
            $call_args['params']['sort'] = $sort . $call_args['params']['sort'];
          }
          else {
            $call_args['params']['sort'] = $sort . 'score desc';
          }
          break;
        case VisitorsvoiceCustomizationInterface::TYPE_REMOVE:
          $call_args['params']['fq'][] = "-bs_visitorsvoice_custom_remove_$id:true";
          break;
      }
    }
  }
}

