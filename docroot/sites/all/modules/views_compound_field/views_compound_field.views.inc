<?php
/**
 * @file views_compound_field.views.inc
 * Contains implementations of Views hooks.
 */

/**
 * Implementation of hook_views_data_alter().
 */
function views_compound_field_views_data_alter(&$data) {
  $data['views']['views_gathering'] = array(
    'field' => array(
      'title' => t('Gathering field'),
      'help' => t("A gathering field combines the output of several prior fields into one, thus allowing more complex theming."),
      'handler' => 'views_compound_field_handler_field_gathering',
    ),
  );
  $data['views']['views_collapsing'] = array(
    'field' => array(
      'title' => t('Collapsing field'),
      'help' => t("A collapsing field shows only the first value it encounters from a set of fields."),
      'handler' => 'views_compound_field_handler_field_collapsing',
    ),
  );
}
