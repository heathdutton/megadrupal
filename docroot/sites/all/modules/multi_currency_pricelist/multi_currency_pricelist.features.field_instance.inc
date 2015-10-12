<?php
/**
 * @file
 * multi_currency_pricelist.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function multi_currency_pricelist_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-pricelist_item-body'
  $field_instances['node-pricelist_item-body'] = array(
    'bundle' => 'pricelist_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-pricelist_item-field_price'
  $field_instances['node-pricelist_item-field_price'] = array(
    'bundle' => 'pricelist_item',
    'default_value' => array(
      0 => array(
        'amount' => 0,
        'currency' => 'EUR',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'money',
        'settings' => array(),
        'type' => 'money_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_price',
    'label' => 'Price',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'money',
      'settings' => array(
        'currencies' => array(
          'allowed_currencies' => array(
            'ADF' => 0,
            'ADP' => 0,
            'AED' => 0,
            'AFA' => 0,
            'AFN' => 0,
            'ALK' => 0,
            'ALL' => 0,
            'AMD' => 0,
            'ANG' => 0,
            'AOA' => 0,
            'AOK' => 0,
            'AON' => 0,
            'AOR' => 0,
            'ARA' => 0,
            'ARL' => 0,
            'ARM' => 0,
            'ARP' => 0,
            'ARS' => 0,
            'ATS' => 0,
            'AUD' => 0,
            'AWG' => 0,
            'AZM' => 0,
            'AZN' => 0,
            'BAM' => 0,
            'BBD' => 0,
            'BDT' => 0,
            'BEC' => 0,
            'BEF' => 0,
            'BEL' => 0,
            'BGL' => 0,
            'BGN' => 0,
            'BHD' => 0,
            'BIF' => 0,
            'BMD' => 0,
            'BND' => 0,
            'BOB' => 0,
            'BOP' => 0,
            'BOV' => 0,
            'BRB' => 0,
            'BRC' => 0,
            'BRE' => 0,
            'BRL' => 0,
            'BRN' => 0,
            'BRR' => 0,
            'BRZ' => 0,
            'BSD' => 0,
            'BTN' => 0,
            'BWP' => 0,
            'BYB' => 0,
            'BYR' => 0,
            'BZD' => 0,
            'CAD' => 0,
            'CDF' => 0,
            'CHE' => 0,
            'CHF' => 0,
            'CHW' => 0,
            'CLE' => 0,
            'CLF' => 0,
            'CLP' => 0,
            'CNY' => 0,
            'COP' => 0,
            'COU' => 0,
            'CRC' => 0,
            'CSD' => 0,
            'CSK' => 0,
            'CUC' => 0,
            'CUP' => 0,
            'CVE' => 0,
            'CYP' => 0,
            'CZK' => 0,
            'DDM' => 0,
            'DEM' => 0,
            'DJF' => 0,
            'DKK' => 0,
            'DOP' => 0,
            'DZD' => 0,
            'ECS' => 0,
            'ECV' => 0,
            'EEK' => 0,
            'EGP' => 'EGP',
            'ERN' => 0,
            'ESA' => 0,
            'ESB' => 0,
            'ESP' => 0,
            'ETB' => 0,
            'EUR' => 'EUR',
            'FIM' => 0,
            'FJD' => 0,
            'FKP' => 0,
            'FRF' => 0,
            'GBP' => 'GBP',
            'GEL' => 0,
            'GHC' => 0,
            'GHS' => 0,
            'GIP' => 0,
            'GMD' => 0,
            'GNE' => 0,
            'GNF' => 0,
            'GNS' => 0,
            'GQE' => 0,
            'GRD' => 0,
            'GTQ' => 0,
            'GWP' => 0,
            'GYD' => 0,
            'HKD' => 0,
            'HNL' => 0,
            'HRD' => 0,
            'HRK' => 0,
            'HTG' => 0,
            'HUF' => 0,
            'IDR' => 0,
            'IEP' => 0,
            'ILP' => 0,
            'ILR' => 0,
            'ILS' => 0,
            'INR' => 0,
            'IQD' => 0,
            'IRR' => 0,
            'ISJ' => 0,
            'ISK' => 0,
            'ITL' => 0,
            'JMD' => 0,
            'JOD' => 0,
            'JPY' => 0,
            'KES' => 0,
            'KGS' => 0,
            'KHR' => 0,
            'KMF' => 0,
            'KPW' => 0,
            'KRH' => 0,
            'KRW' => 0,
            'KWD' => 0,
            'KYD' => 0,
            'KZT' => 0,
            'LAK' => 0,
            'LBP' => 0,
            'LKR' => 0,
            'LRD' => 0,
            'LSL' => 0,
            'LTL' => 0,
            'LTT' => 0,
            'LUF' => 0,
            'LVL' => 0,
            'LVR' => 0,
            'LYD' => 0,
            'MAD' => 0,
            'MAF' => 0,
            'MCF' => 0,
            'MDC' => 0,
            'MDL' => 0,
            'MGA' => 0,
            'MGF' => 0,
            'MKD' => 0,
            'MKN' => 0,
            'MLF' => 0,
            'MMK' => 0,
            'MNT' => 0,
            'MOP' => 0,
            'MRO' => 0,
            'MTL' => 0,
            'MTP' => 0,
            'MUR' => 0,
            'MVQ' => 0,
            'MVR' => 0,
            'MWK' => 0,
            'MXN' => 0,
            'MXP' => 0,
            'MXV' => 0,
            'MYR' => 0,
            'MZE' => 0,
            'MZM' => 0,
            'MZN' => 0,
            'NAD' => 0,
            'NFD' => 0,
            'NGN' => 0,
            'NIO' => 0,
            'NLG' => 0,
            'NOK' => 0,
            'NPR' => 0,
            'NZD' => 0,
            'OMR' => 0,
            'PAB' => 0,
            'PEH' => 0,
            'PEI' => 0,
            'PEN' => 0,
            'PGK' => 0,
            'PHP' => 0,
            'PKR' => 0,
            'PLN' => 0,
            'PLZ' => 0,
            'PTE' => 0,
            'PYG' => 0,
            'QAR' => 0,
            'RHD' => 0,
            'ROL' => 0,
            'RON' => 0,
            'RSD' => 0,
            'RUB' => 0,
            'RUR' => 0,
            'RWF' => 0,
            'SAR' => 0,
            'SBD' => 0,
            'SCR' => 0,
            'SDD' => 0,
            'SDG' => 0,
            'SDP' => 0,
            'SEK' => 0,
            'SGD' => 0,
            'SHP' => 0,
            'SIT' => 0,
            'SKK' => 0,
            'SLL' => 0,
            'SML' => 0,
            'SOS' => 0,
            'SRD' => 0,
            'SRG' => 0,
            'SSP' => 0,
            'STD' => 0,
            'SUR' => 0,
            'SVC' => 0,
            'SYP' => 0,
            'SZL' => 0,
            'THB' => 0,
            'TJR' => 0,
            'TJS' => 0,
            'TMM' => 0,
            'TMT' => 0,
            'TND' => 0,
            'TOP' => 0,
            'TPE' => 0,
            'TRL' => 0,
            'TRY' => 0,
            'TTD' => 0,
            'TWD' => 0,
            'TZS' => 0,
            'UAH' => 0,
            'UAK' => 0,
            'UGS' => 0,
            'UGX' => 0,
            'USD' => 'USD',
            'USN' => 0,
            'USS' => 0,
            'UYI' => 0,
            'UYN' => 0,
            'UYU' => 0,
            'UZS' => 0,
            'VAL' => 0,
            'VEB' => 0,
            'VEF' => 0,
            'VND' => 0,
            'VUV' => 0,
            'WST' => 0,
            'XAF' => 0,
            'XAG' => 0,
            'XAU' => 0,
            'XBA' => 0,
            'XBB' => 0,
            'XBC' => 0,
            'XBD' => 0,
            'XCD' => 0,
            'XDR' => 0,
            'XEU' => 0,
            'XFO' => 0,
            'XFU' => 0,
            'XOF' => 0,
            'XPD' => 0,
            'XPF' => 0,
            'XPT' => 0,
            'XTS' => 0,
            'XXX' => 0,
            'YDD' => 0,
            'YER' => 0,
            'YUD' => 0,
            'YUG' => 0,
            'YUM' => 0,
            'YUN' => 0,
            'YUO' => 0,
            'YUR' => 0,
            'ZAL' => 0,
            'ZAR' => 0,
            'ZMK' => 0,
            'ZMW' => 0,
            'ZRN' => 0,
            'ZRZ' => 0,
            'ZWD' => 0,
            'ZWL' => 0,
            'ZWN' => 0,
            'ZWR' => 0,
          ),
        ),
        'currency_display_mode' => 'a|+|c',
        'currency_select_mode' => 'code',
        'decimals_display_mode' => 'field',
      ),
      'type' => 'money_widget',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-pricelist_item-field_pricelist_section'
  $field_instances['node-pricelist_item-field_pricelist_section'] = array(
    'bundle' => 'pricelist_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_pricelist_section',
    'label' => 'Pricelist section',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Price');
  t('Pricelist section');

  return $field_instances;
}