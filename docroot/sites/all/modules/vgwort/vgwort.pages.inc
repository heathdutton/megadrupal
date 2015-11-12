<?php

function vgwort_node_qa_check($node) {
  if ( isset($node->vgwort) ) {
    $output = theme('table', array('header' => array_keys($node->vgwort), 'rows' => array($node->vgwort)));
  }
  $render = node_view($node, 'vgwort') ;
  $output .= '<h2>' . t('VGWort Text Preview') . '</h2>' ;
  $content = render($render) ;
  $output .= '<pre>' . $content . '</pre>' ;
  $output .= '<h4>' . t('Char count') . ':' . vgwort_count_chars($content) . '</h4>' ;
  $status = module_invoke_all('vgwort_check', 'node', $node) ;
  if ( count($status) > 0 ) {
    $vgwort_stat = FALSE ;
    foreach ( $status as $k => $s ) {
      $errors .= '<div class="warning">' . t('Could not ad vgwort mark. Reason: @reason', array('@reason' => $s['message'])) . '</div>' ;
    }
    $output .= '<div class="messages">' . $errors . '</div>' ;
  }
  return $output ;
}

function vgwort_report() {
  $header = array();
  $header[] = t('VGWort marks in depo') ;
  $header[] = t('Unused VGWort marks') ;
  $header[] = t('Unused Test VGWort marks') ;
  $header[] = t('Not reported marks') ;

  $query = db_select('vgwort', 'v')->fields('v', array('mid'));
  $rows[0][]  = $query->execute()->rowCount();

  $query = db_select('vgwort', 'v')->fields('v', array('mid'))->condition('status', '0')->condition('test', 0);
  $rows[0][]  = $query->execute()->rowCount();

  $query = db_select('vgwort', 'v')->fields('v', array('mid'))->condition('status', '0')->condition('test', 1);
  $rows[0][]  = $query->execute()->rowCount();

  $query = db_select('vgwort', 'v')->fields('v', array('mid'))->condition('status', '1');
  $rows[0][]  = $query->execute()->rowCount();

  return theme('table', array('header' => $header, 'rows' => $rows ,
               'caption' => l('Import new marks', 'admin/config/services/vgwort/import', array( 'query' => array( 'destination' => $_GET['q'] ))))) ;
}

function vgwort_report_entities() {
  $limit = 20 ;
  $sql = 'SELECT private,entity_type,entity_id FROM {vgwort} WHERE status = :status';
  $result = db_query_range($sql, 0, 20, array(':status' => VGWORT_STATUS_ASSIGNED))->fetchAll();
  $rows = array();

  foreach ( $result as $row) {
    $rows[] = (array) $row ;
    $entity = entity_load_single($row->entity_type, $row->entity_id) ;
    if ( $entity ) {
      $eurl = entity_uri($row->entity_type, $entity) ;
      if ( $row->entity_type == 'node' ) {
        $eurl = l($entity->title, $eurl['path']);
      }
      else {
        $eurl = url($eurl['path']);
      }
      $rows[count($rows)-1][] = $eurl ;
    }
  }

  $header = array();
  $header[] = t('VGWort private id') ;
  $header[] = t('Entity type') ;
  $header[] = t('Entity id') ;
  $header[] = t('Entity link') ;
  return theme('table', array('header' => $header, 'rows' => $rows)) ;
}