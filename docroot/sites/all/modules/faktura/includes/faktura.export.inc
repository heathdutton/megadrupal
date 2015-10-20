<?php

function _faktura_export($node, $filetype) {
  switch($node->type) {
    case 'invoice':
      $invoice = $node;
      break;
    
    case 'dunning':
      $invoice = node_load($node->field_invoice[LANGUAGE_NONE][0]['target_id']);
      break;
  }
  
  $company = node_load($node->field_ref_company[LANGUAGE_NONE][0]['target_id']);
  $customer = node_load($node->field_customer[LANGUAGE_NONE][0]['target_id']);
  
  $items = entity_load('field_collection_item', $invoice->field_invoice_item[LANGUAGE_NONE]);
  $invoiceitems = array();
  $i = 1;
  foreach($items as $key => $item) {
    $invoiceitems[$key]['field_invoice_item_position'] = $i;
    $invoiceitems[$key]['field_invoice_item_desc'] = $item->field_invoice_item_desc[LANGUAGE_NONE][0]['value'];
    $invoiceitems[$key]['field_invoice_item_qty'] = $item->field_invoice_item_qty[LANGUAGE_NONE][0]['value'];
    
    if(isset($item->field_invoice_item_unit[LANGUAGE_NONE][0]['tid'])) {
      $unit = taxonomy_term_load($item->field_invoice_item_unit[LANGUAGE_NONE][0]['tid']);
      $invoiceitems[$key]['field_invoice_item_unit'] = $unit->name;
    } else {
      $invoiceitems[$key]['field_invoice_item_unit'] = '';
    }
    // $invoiceitems[$key]['field_invoice_item_tax'] = $item->field_invoice_item_tax[LANGUAGE_NONE][0]['value'];
    $invoiceitems[$key]['field_invoice_item_total'] = $item->field_invoice_item_total[LANGUAGE_NONE][0]['value'];
    $invoiceitems[$key]['field_invoice_item_unit_price'] = $item->field_invoice_item_unit_price[LANGUAGE_NONE][0]['value'];
    
    $i++;
  }
  
  $taxes = entity_load('field_collection_item', $invoice->field_invoice_tax[LANGUAGE_NONE]);
  $invoicetax = array();
  foreach($taxes as $key => $tax) {
    $tax_node = node_load($tax->field_tax[LANGUAGE_NONE][0]['target_id']);
    $invoicetax[$key]['field_tax_value'] = $tax_node->field_tax_value[LANGUAGE_NONE][0]['value'];    
    $invoicetax[$key]['field_taxed_amount'] = $tax->field_taxed_amount[LANGUAGE_NONE][0]['value'];
    $invoicetax[$key]['field_tax_amount'] = $tax->field_tax_amount[LANGUAGE_NONE][0]['value'];
  }
  
  $data = array(
    'user' => variable_get('faktura_pdftemplate_user', ''),
    'template' => variable_get('pdftemplate_' . $company->nid . '_invoice_template', ''),
    'key' => variable_get('pdftemplate_' . $company->nid . '_invoice_key', ''),
    'return' => 'base64',
    'filetype' => $filetype,
    
    'field_company' => $customer->field_company[LANGUAGE_NONE][0]['value'],
    'field_street' => $customer->field_street[LANGUAGE_NONE][0]['value'],
    'field_zip' => $customer->field_zip[LANGUAGE_NONE][0]['value'],
    'field_city' => $customer->field_city[LANGUAGE_NONE][0]['value'],
    
    'field_invoice_number' => $invoice->field_invoice_number[LANGUAGE_NONE][0]['value'],
    'created' => date('d.m.Y', $invoice->created),
    
    'invoiceitems' => $invoiceitems,
    'invoicetaxes' => $invoicetax,
    
    'field_invoice_total' => $invoice->field_invoice_total[LANGUAGE_NONE][0]['value'],
  );
  
  $url = 'http://www.pdftemplate.eu/api/';
  $full_url = url($url, array('query' => $data));
  $response = drupal_http_request($full_url);
 
  header('Content-Description: File Transfer');
  header('Content-Disposition: attachment; filename="invoice.' . $filetype . '"');
  echo base64_decode($response->data);
}
