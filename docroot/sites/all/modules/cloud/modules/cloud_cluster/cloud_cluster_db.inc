<?php


/**
 * @file
 * Provides cluster feature which enables to bundle server templates.
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


function _cloud_cluster_get_clusters_query() {

  return db_select(CLOUD_CLUSTER_TABLE, 'a');
}


function _cloud_cluster_get_count_db() {

  $cluster_count = 0;
  $query =  'SELECT count(*) as cluster_count FROM {' . CLOUD_CLUSTER_TABLE . '} c  ';
  $result = db_query( $query);
  foreach ($result as $cluster) {

    $cluster_count = $cluster->cluster_count;
  }

  return $cluster_count;
}


function _cloud_cluster_get_cluster_by_id_query() {

  return $query =  'SELECT c.cluster_id ,  c.cluster_nickname,  c.description,  c.creator FROM {' . CLOUD_CLUSTER_TABLE . '} c  WHERE cluster_id= :cluster_id  ';
}

function _cloud_cluster_get_cluster_by_filter_query() {

  return $query =  'SELECT c.cluster_id ,  c.cluster_nickname,  c.description,  c.creator FROM {' . CLOUD_CLUSTER_TABLE . '} c  WHERE  cluster_nickname = :cluster_nickname  ';
}

/**
 * Insert a cluster in database
 * This function insert entry in database and audit logs as well.
 *
 * @param $form_values
 *        This is the form-values submitted by new cluster form
 * @return return a last inserted cluster-id
 */
function _cloud_cluster_save_cluster_db($form_values, $creator) {

  $nid = db_insert(CLOUD_CLUSTER_TABLE)
    ->fields(array(
    'cluster_nickname' => $form_values['nickname_text'   ],
    'description' => $form_values['description_text'],
    'creator' => $creator,
  ))
  ->execute();


  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Cluster has been added: @cluster_name', array('@cluster_name' => $form_values['nickname_text'])),
    'link' => '',
  )
  );


  // Get the id of newly inserted cluster

  $cluster_id = NULL;
  $result = db_select(CLOUD_CLUSTER_TABLE, 'n')
    ->fields('n')
    ->condition('cluster_nickname', $form_values['nickname_text'   ], '=')
    ->condition('description', $form_values['description_text'], '=')
    ->condition('creator', $creator, '=')
    ->execute();

  foreach ($result as $res_obj) {

    $cluster_id = $res_obj->cluster_id;
  }

  return $cluster_id;
}


function _cloud_cluster_update_cluster_db($form_values, $cluster_id) {

  $num_updated = db_update(CLOUD_CLUSTER_TABLE)
    ->fields(array(
    'cluster_nickname' => $form_values['nickname_text'],
    'description' => $form_values['description_text'],
  ))
    ->condition('cluster_id', $cluster_id, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Cluster has been modified: @cluster_nickname', array('@cluster_nickname' => $form_values['nickname_text'])),
    'link' => '',
  )
  );

  return;
}



/**
 * Delete a Cluster using Cluster Id
 *
 * @param $cluster_id
 * @return
 */
function _cloud_cluster_delete_by_id($cluster_id) {

  $num_deleted = db_delete(CLOUD_CLUSTER_SERVER_TABLE)
    ->condition('cluster_id', $cluster_id)
    ->execute();

  $num_deleted = db_delete(CLOUD_CLUSTER_TABLE)
    ->condition('cluster_id', $cluster_id)
    ->execute();

  // User Activity Log
  cloud_audit_user_activity(array(
    'type' => 'user_activity',
    'message' => t('Cluster has been deleted: @cluster_id', array('@cluster_id' => $cluster_id)),
    'link' => '',
  )
  );

  return TRUE;
}


/**
 * Return all cluster list
 *
 * @param $cloud
 * @return
 */
function _cloud_cluster_get_all_clusters_query($cloud) {

  return ' select * from {' . CLOUD_CLUSTER_TABLE . '}  ';
}


// End : DB functions Related to Cluster


