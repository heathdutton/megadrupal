<?php

/**
 * @file
 * We customize contributor rendering and we add drupal_nid view's handler.
 */

/**
 * Implements hook_views_data_alter().
 */
function biblio_author_node_views_data_alter(&$viewsdata) {

  /**************** biblio table **************/

  $result = db_query("SELECT f.name,f.type,ftd.title,ft.ftdid FROM {biblio_fields} f
                      INNER JOIN {biblio_field_type} AS ft ON ft.fid = f.fid
                      INNER JOIN {biblio_field_type_data} ftd ON ft.ftdid = ftd.ftdid
                      WHERE ft.tid = 0  and f.type = 'contrib_widget'");

  foreach ($result as $field) {
    $viewsdata['biblio'][$field->name]['field']['handler'] = 'biblio_author_node_handler_field_contributor';
  }

  /**************** biblio_contributor_data table ***********/

  $viewsdata['biblio_contributor_data']['drupal_nid'] = array(
    'field'     => array('handler' => 'views_handler_field'),
    'filter'    => array('handler' => 'biblio_author_node_handler_filter_contributor_nid'),
    'argument'  => array('handler' => 'views_handler_argument_numeric'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'label' => t('node'),
    ),

    'title'     => t('Drupal Node ID'),
    'help'      => t('This is the Drupal node associated with the Biblio Author.'),
  );
}
