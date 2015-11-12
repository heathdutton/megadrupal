<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */

function cm_airing_grid_get_vocab_terms_array($vid) {
  $terms_res = cm_airing_grid_get_vocab_terms($vid);

  foreach ($terms_res as $term) {
  	$tid = $term['id'];
    $array[$tid] = $term['name'];
  }
  
  return $array;
}

function cm_airing_grid_get_channel_vid() {
	return cm_airing_grid_get_vid_for_name("Channel");
}

function cm_airing_grid_get_airing_type_vid() {
   return cm_airing_grid_get_vid_for_name("Airing Type");
}

function cm_airing_grid_get_device_vid() {
  return cm_airing_grid_get_vid_for_name('Device');
} 

function cm_airing_grid_get_format_vid() {
  return cm_airing_grid_get_vid_for_name("Format");
}

function cm_airing_grid_is_format_live($project_format) {
  if ($project_format == 'LIVE') {
    return true;
  }  
  return false;
}

function cm_airing_grid_get_project_format_tid($project_nid) {
  global $project_formats;
  $format_vid = cm_airing_grid_get_vid_for_name('Format');
  // check if global variable has been instantiated
  if (!is_array($project_formats)) {
    $project_formats = array();
  }

  // check if this project's format tid has already been cached
  if (!array_key_exists($project_nid, $project_formats)) {
    // fetch tid
    $query = "SELECT ti.tid FROM taxonomy_index ti INNER JOIN taxonomy_term_data ttd ON ti.tid = ttd.tid WHERE ttd.vid = :vid AND ti.nid = :nid";
    $result = db_query($query, array(':vid' => $format_vid, ':nid' => $project_nid))->fetchField();
  } 
 
  return $result;
}

function cm_airing_grid_get_airing_series_tid($vid = null) {
  if (!$vid) {
    $vid = cm_airing_grid_get_airing_type_vid();
  }

  $terms_res = cm_airing_grid_get_vocab_terms($vid);
  foreach($terms_res as $term) {
    if ($term['name'] == 'Series') {
      return $term['id'];
    }
  }
  return null;
}
function cm_airing_grid_get_episode_device_tid($episode_nid) {
  if (!$episode_nid) {
    return false;
  }
  global $episode_devices;
  $device_vid = cm_airing_grid_get_vid_for_name('Livefeed');
  // check if global variable has been instantiated
  if (!is_array($episode_devices)) {
    $episode_devices = array();
  }
  // check if this episode's device tid has already been cached
  if (!array_key_exists($episode_nid, $episode_devices)) {
    // fetch tid

    $query = "SELECT ti.tid FROM taxonomy_index ti INNER JOIN taxonomy_term_data ttd ON ti.tid = ttd.tid WHERE ttd.vid = :vid AND ti.nid = :nid";
    $result = db_query($query, array(':vid' => $device_vid, ':nid' => $episode_nid))->fetchField();
    $episode_devices[$episode_nid] = $result;
  }
  else {
    $result = $episode_devices[$episode_nid];
  }

  return $result;
}


function cm_airing_grid_validate_channel($channel) {
  $channels = cm_airing_grid_get_vocab_terms_array(cm_airing_grid_get_channel_vid());
  
  if (!$channel) {
    $channel = current($channels);
  }
  else {
    $channel = "Channel $channel";
    if (!in_array($channel, $channels)) {
      drupal_set_message("$channel is not a valid channel. " .
          "If it should be add it through Admin -> Categories -> Channel.", 'error');
      $channel = current($channels);
    }
  }
  return $channel;
}


/*
 *  major TODO: since we are using taxonomy in crazy ways, we need to create a
 *  .inc file that contains a library of taxonomy-manipulating functions like
 *  this. Or something. Then delete this function, of course.
 */
function cm_airing_grid_extract_tids_by_vid($vid, &$obj) {
  $r = array();
  foreach ( $obj->taxonomy as $t ) {
    if ( $t->vid == $vid ) {
      $r[] = $t;
    }
  }
  return $r;
}

function  cm_airing_grid_get_channel_number($channel){
  return preg_replace("/Channel /", "", $channel);
}


function cm_airing_grid_is_node_live_format(&$node){
  $format_info = cm_airing_grid_get_node_format($node);
  if ($format_info['name'] == "LIVE") {
    return true;
  }
  else {
    return false;
  }
}

/**
 * Take a taxonomy tid and a node object, and "apply" the tid to the node but
 * in the process, blow away the term from that vid that was already there
 */
function cm_airing_grid_flip_taxonomy($tid, $vid, &$node){
  $tids_objs =  cm_airing_grid_extract_tids_by_vid($vid, $node);
  foreach ( $tids_objs as $term ) {
    $tids[$term->tid] = $term->tid;
  }
  foreach ( $node->taxonomy as $t ) {
    if ( is_array($tids) && in_array($t->tid, $tids) ) {
      unset ( $node->taxonomy[$t->tid] );
    }
  }
  $node->taxonomy[] = $tid;
  taxonomy_node_save($node->nid, $node->taxonomy);
}

function cm_airing_grid_get_vocab_terms($vid) {

    $terms = array();

    $result = 
      db_query("SELECT tid, name FROM 
                {taxonomy_term_data} 
                 WHERE vid = :d", 
	       array(':d' => $vid));
    while ($record = $result->fetchObject()) {
      array_push($terms, array('id' => $record->tid, 'name' => $record->name));
    }

    return $terms;
}
 
function cm_airing_grid_get_term_name_for_tid($tid) {

  $result = db_query(
    "SELECT name FROM {taxonomy_term_data} 
     WHERE tid =:t LIMIT 1", 
    array(':t' => $tid))->fetchField();

  return $result;
}
