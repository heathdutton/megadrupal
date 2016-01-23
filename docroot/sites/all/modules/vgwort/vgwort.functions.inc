<?php

function vgwort_import_codes() {
  $service = vgwort_service_load('vgwort_service_pixel');
  $count = 0 ;
  try {
    $pixels = $service->orderPixel(array('count' => VGWORT_IMPORT ));
    $domain = $pixels->domain ;
    $count = count($pixels->pixels) ;
    foreach ( $pixels->pixels->pixel as $pixel ) {
      $record = array(
        'domain' => $domain,
        'private' => $pixel->privateIdentificationId,
        'public' => $pixel->publicIdentificationId,
        'entity_id' => '',
        'entity_type' => '',
        'modified' => time(),
        'status' => 0,
        'test'   => variable_get('vgwort_test', 0)
      );
      $success = drupal_write_record('vgwort', $record) ;
      if ( $success === FALSE ) {
      }
    }
  } catch (Exception $ex) {
    drupal_set_message(t('Problem occured')) ;
  }
  drupal_set_message(t('Imported @count marks', array('@count' => $count )));
  if ( ($dest = drupal_get_destination()) != $_GET['q'] ) {
    drupal_goto($dest) ;
  }
  return t('Imported @count marks', array('@count' => $count )) ;

}

function vgwort_count_unused_pixels() {
  $query = db_select('vgwort', 'v')->fields('v', array('mid'))->condition('status', '0');
  if ( variable_get('vgwort_test', 1) == 1 ) {
    $query->condition('test', 1);
  }
  else {
    $query->condition('test', 0);
  }
  return $query->execute()->rowCount();
}

function vgwort_get_next_messages() {
  $time = time() - variable_get('vgwort_time_to_report', VGWORT_TIME_TO_REPORT) ;
  $time = time();
  $query = db_select('vgwort', 'v')->fields('v', array('mid', 'entity_type', 'entity_id'))
                                  ->condition('status', VGWORT_STATUS_ASSIGNED)
                                  ->condition('modified', $time, '<');
  if ( variable_get('vgwort_test', 1) == 1 ) {
    $query->condition('test', 1);
  }
  else {
    $query->condition('test', 0);
  }
}

function vgwort_get_next_pixel($entity_type, $entity_id) {
  $test = variable_get('vgwort_test', 0) ;
  $transaction = db_transaction();
  try {
    $next = db_select('vgwort', 'v')
            ->fields('v')
            ->condition('test', variable_get('vgwort_test', 0))
            ->condition('status', VGWORT_STATUS_NEW)
            ->execute()
            ->fetchAssoc() ;
    if ( !$next ) {
      drupal_set_message(t('No codes avalaible'));
      return FALSE ;
    }
    $next['entity_type'] = $entity_type ;
    $next['entity_id'] = $entity_id ;
    $next['status'] = VGWORT_STATUS_ASSIGNED ;
    drupal_write_record('vgwort', $next, 'mid') ;

    return $next;
  }  catch (Exception $e) {
    $transaction->rollback();
  }
}

function vgwort_publish_message($data, $entity_type, $entity) {
  $service = vgwort_service_load('vgwort_service_message');
  try {
    $result = $service->newMessage($data) ;
    if ( isset($result->status) && 'OK' == $result->status ) {
      vgwort_set_status($data->privateidentificationid, VGWORT_STATUS_PUBLISHED) ;
      watchdog('vgwort', 'Publish entity @type to vgwort', array('@type' => $entity_type), WATCHDOG_INFO) ;
      return TRUE ;
    }
    else {
      vgwort_set_status($data->privateidentificationid, VGWORT_STATUS_ERROR) ;
      watchdog('vgwort', 'Error publish message to WS', array(), WATCHDOG_ERROR) ;
      return FALSE ;
    }

  } catch ( Exception $ex ) {
    watchdog('wsclient', $ex->__toString());
    watchdog('vgwort', $ex->getMessage()) ;
  }
}

function vgwort_set_status($private_id, $status) {
  db_update('vgwort')
            ->fields( array('status' => $status) )
            ->condition('private', $private_id )
            ->execute();
}

function vgwort_service_load($service_name) {
  if ( variable_get('vgwort_test', 0) == 1 ) {
    $service_name .= '_test' ;
  }
  return wsclient_service_load($service_name);
}

