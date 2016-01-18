<?php
function _autordf_service_terms_suggest($nid) {
	$keywords = autordf_get_node_keywords($nid);
  if ($keywords) {
    $tags = array();
    foreach ($keywords as $keyword) {
      $tags[$keyword->vid][] = $keyword->name;
    }
  }
  
  $vocabulary = variable_get('autordf_vocabulary_names', FALSE);
  foreach ($vocabulary as $name => $vid) {
  	$v[$name] = $tags[$vid];
  }

  return $v;
}