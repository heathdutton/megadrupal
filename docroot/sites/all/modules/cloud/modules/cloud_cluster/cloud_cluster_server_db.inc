<?php

/**
 * @file
 * Provides cluster feature which enables to bundle server templates.
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


// Start : DB functions Related to Cluster_server

function _cloud_cluster_save_cluster_server_db($form_values) {

  $cluster_id = $form_values['cluster_id'];
  $failover_select = isset($form_values['failover_select']) ? $form_values['failover_select'] : FALSE;
  if ($failover_select) {
    $ip_select = $form_values['elastic_ip_select'];
  }
  else {
    $ip_select = '';
  }

  $nid = db_insert(CLOUD_CLUSTER_SERVER_TABLE)
    ->fields(array(
    'cluster_id' => $cluster_id,
    'template_id' => $form_values['template_select'        ],
    'server_nickname' => $form_values['nickname_text'          ],
    'server_sshkey' => $form_values['ssh_keys_select'        ],
    'server_security_group' => @implode(',', $form_values['SG_select'] ),
    'server_zone' => $form_values['zone_select'            ],
    'failover_flag' => $failover_select == FALSE ? 0 : $failover_select,
    'elastic_ip' => $ip_select,
  ))
    ->execute();

  if ($failover_select) {
    $server_id = _cloud_cluster_get_last_added_server_db($form_values);
    _cloud_failover_save_server_scenario( $server_id, $form_values['failover_scenario_select']);
  }

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Cluster Server has been added: @cluster_name', array('@cluster_name' => $form_values['nickname_text'])),
    'link' => '',
  )
  );

  return;
}


function _cloud_cluster_save_cluster_template_db($values) {

  $nid = db_insert(CLOUD_CLUSTER_SERVER_TABLE) // Table name no longer needs {}
    ->fields(array(
    'cluster_id' => $values['cluster_id'       ],
    'template_id' => $values['template_select'  ],
    'server_nickname' => $values['nickname_text'    ],
    'server_sshkey' => $values['ssh_keys_select'  ],
    'server_security_group' => $values['SG_select'        ],
    'server_zone' => $values['zone_select'      ],
    'failover_flag' => $values['failover_select'  ],
    'elastic_ip' => '',
    'instance_id' => $values['instance_id'      ],
  ))
    ->execute();

  return;
}


function _cloud_cluster_servers_get_query() {

  $query = db_select(CLOUD_CLUSTER_SERVER_TABLE, 'e')
              ->fields('e');

  return $query;
}


function _cloud_cluster_server_get_query() {

  return $query = ' select * from
              {' . CLOUD_CLUSTER_SERVER_TABLE . '} e WHERE e.cluster_id=:cluster_id and e.server_id=:server_id ';
}


function _cloud_cluster_templates_get_query() {

  return $query = ' select e.instance_id, e.server_nickname as server_nickname , tmpl.cloud_type as cloud_type, tmpl.template_nickname as template_nickname , e.cluster_id, tmpl.template_id as template_id, e.template_id, e.server_id , tmpl.count_instances from
              {' . CLOUD_CLUSTER_SERVER_TABLE . '} e
              LEFT JOIN {' . CLOUD_SERVER_TEMPLATES_TABLE . '} tmpl
              on e.template_id = tmpl.template_id
              WHERE e.cluster_id=:cluster_id ';
}

function _cloud_cluster_templates_server_get_query() {

  return $query = ' select e.instance_id, e.server_nickname as server_nickname , tmpl.cloud_type as cloud_type, tmpl.template_nickname as template_nickname , e.cluster_id, tmpl.template_id as template_id, e.template_id, e.server_id , tmpl.count_instances from
              {' . CLOUD_CLUSTER_SERVER_TABLE . '} e
              LEFT JOIN {' . CLOUD_SERVER_TEMPLATES_TABLE . '} tmpl
              on e.template_id = tmpl.template_id
              WHERE e.cluster_id=:cluster_id and e.server_id=:server_id ';
}

function _cloud_cluster_servers_get_count_db($cluster_id) {

  $query = ' select count(*) cnt from
              {' . CLOUD_CLUSTER_SERVER_TABLE . '} e WHERE e.cluster_id=:cluster_id ';

  $query_args = array(
    ':cluster_id' => $cluster_id,
  );

  $result     = db_query( $query, $query_args );

  foreach ($result as $cnt_res) { // Cluster server object
    return $cnt_res->cnt;
  }

  return 0;
}


function _cloud_cluster_get_server_query() {

  $query = 'select * from  {' . CLOUD_CLUSTER_SERVER_TABLE . '}
                  where server_id=:server_id
          ';

  return $query;
}


function _cloud_cluster_get_last_added_server_db($form_values) {

  $query = 'SELECT * FROM {' . CLOUD_CLUSTER_SERVER_TABLE . "} c  WHERE cluster_id= :cluster_id and template_id= :template_id and server_nickname= :server_nickname
        ORDER BY server_id DESC LIMIT 1 ";
  $query_args = array(
    ':cluster_id' => $form_values['cluster_id'],
    ':template_id' => $form_values['template_select'],
    ':server_nickname' => $form_values['nickname_text'  ],
  );

  $result = db_query( $query, $query_args );


  foreach ($result as $key) {

    return $key->server_id;
  }

}


function _cloud_cluster_delete_server_db($server_id) {

  $num_deleted = db_delete(CLOUD_CLUSTER_SERVER_TABLE)
    ->condition('server_id', $server_id)
    ->execute();

  if (module_exists('cloud_inputs')) {

    $num_deleted = db_delete(CLOUD_INPUTS_PARAMETER_VALUES_TABLE)
      ->condition('server_id', $server_id)
      ->execute();
  }

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => 'Cluster Server has been deleted.',
    'link' => '',
  )
  );

  return;
}


function _cloud_cluster_delete_all_server_db($cluster_id) {

  $num_deleted = db_delete(CLOUD_CLUSTER_SERVER_TABLE)
    ->condition('cluster_id', $cluster_id)
    ->execute();

  if (module_exists('cloud_inputs')) {
    $num_deleted = db_delete(CLOUD_INPUTS_PARAMETER_VALUES_TABLE)
      ->condition('cluster_id', $cluster_id)
      ->execute();
  }

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => 'Cluster Servers have been deleted.',
    'link' => '',
  )
  );

  return;
}


function _cloud_cluster_update_cluster_server_query($instance_id, $server_id) {


  $num_updated = db_update(CLOUD_CLUSTER_SERVER_TABLE)
    ->fields(array(
    'instance_id' => $instance_id,
  ))
    ->condition('server_id', $server_id, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => 'Cluster server has been modified.',
    'link' => '',
  )
  );


  return;
}

/**
 * Get server information based on instance id
 */
function cloud_cluster_get_server($instance_id) {
  $result = db_select(CLOUD_CLUSTER_SERVER_TABLE, 'c')
    ->fields('c')
    ->condition('instance_id', $instance_id, '=')
    ->execute()
    ->fetchAssoc();

  return $result;
}

// End : DB functions Related to Cluster_server
