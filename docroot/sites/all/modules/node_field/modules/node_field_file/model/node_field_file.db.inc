<?php

function node_field_file_db_on_insert($fid, $field_id) {
  $field = node_field_load_node_field($field_id);
  $field['value'] = $fid;
  node_field_update_node_field($field);
  db_update('file_managed')
    ->fields(array('status' => 1))
    ->condition('fid', $fid)
    ->execute();
}
