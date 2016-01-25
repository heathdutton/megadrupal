<?php
/**
 * @file
 * ces_bank view.
 */

/**
 * Views for CES Banks creates ces_exchange view. 
 * @defgroup ces_bank_view Ces Bank View
 * @ingroup ces_bank
 * @{
 * View from bank.
 */
function ces_bank_views_data() {
  // Ces Exchange.
  $data['ces_exchange']['table']['group'] = t('Ces Exchange');
  $data['ces_exchange']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Ces Exchange table'),
    'help' => t('Ces Exchange view.'),
    'weight' => -10,
  );
  $data['ces_exchange']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'admin',
    ),
  );
  $data['ces_exchange']['id'] = array(
    'title' => t('Ces exchange Id'),
    'help' => t('Ces exchange id.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'relationship' => array(
      'base' => 'ces_message',
      'base field' => 'exchange',
      'handler' => 'views_handler_relationship',
      'label' => t('Ces exchange message relationship'),
      'title' => t('Ces exchange message relationship'),
      'help' => t('More information on this relationship'),
    ),
  );

  $data['ces_exchange']['shortname'] = array(
    'title' => t('Exchange Shortname'),
    'help' => t('Exchange Shortname.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  return $data;
}
/**
 * @} End of "defgroup ces_bank_view".
 */
