<?php

/**
 * @file
 * Definition of commerce_bpost_handler_bpost_pdf_icon.
 */

/**
 * Display a clickable pdf icon to edit the order and see the label.
 */
class commerce_bpost_handler_bpost_pdf_icon extends views_handler_field {
  /**
   * Add some required fields needed on render().
   */
  function construct() {
    parent::construct();
    $this->additional_fields['bpost_pdf_icon'] = array(
      'table' => 'commerce_order',
      'field' => 'order_id',
    );
  }

  /**
   * Loads additional fields.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $entity = commerce_order_load($values->order_id);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $values->order_id);
    if (!empty($entity->field_bpost_label)) {
      $render = array();
      global $base_url;

      // Display label pdf icon.
      $icon_path = $base_url . '/' . drupal_get_path('module', 'commerce_bpost') . '/icons/application-pdf.png';
      $url = 'display_megarow/' . $values->order_id . '/admin/commerce/orders/' . $values->order_id;
      $label = '<span class="file"><img class="file-icon" alt="" title="label available in the edit order" src="' . $icon_path . '"> </span>';
      $render['pdf'] = array(
        '#markup' => l(
          $label,
          $url,
          array(
            'attributes' =>  array('class' => array('views-megarow-open')),
            'html' => TRUE,
          )),
      );

      // Display track and trace link icon.
      if (!empty($entity->field_barcode)) {
        $barcodes = '';
        foreach ($order_wrapper->field_barcode as $barcode) {
          $barcodes .= $barcode->value() . ',';
        }
        $barcodes = substr($barcodes, 0, -1);
      }
      $languages = array('nl', 'fr', 'en', 'de');
      $get = drupal_http_build_query(array(
        'searchByItemCode' => 'true',
        'itemCodes' => $barcodes,
        'oss_language' => in_array($GLOBALS['language']->language, $languages) ? strtoupper($GLOBALS['language']->language) : 'NL',
      ));
      $url = 'http://track.bpost.be/etr/light/performSearch.do?' . $get;
      $icon_path = $base_url . '/' . drupal_get_path('module', 'commerce_bpost') . '/icons/trackandtrace.png';
      $render['trackandtrace'] = array(
        '#markup' => l(
          '<img class="file-icon" alt="" title="track and trace" src="' . $icon_path . '">',
          $url,
          array(
            'html' => TRUE,
            'attributes' => array(
              'target' => '_blank',
            ),
          )),
      );

      return $render;
    }
  }

}
