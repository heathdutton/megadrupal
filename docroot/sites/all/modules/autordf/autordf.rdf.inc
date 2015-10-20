<?php

/**
 * Implementation of hook_filter_info()
 */
function autordf_filter_info() {
	$filters['autordf_rdf'] = array(
    'title' => t('RDF Content'),
    'description' => t('This filter will fetch node tags and will rdf the content on nodes of enabled type.'),
    'weight' => 10,
    'process callback' => '_autordf_filter_process',
  );
  return $filters;
}

function _autordf_filter_process($text, $filter) {
  // Only Apply on node Body
  if (arg(0) != 'node' || !is_numeric(arg(1)) || arg(2)) {
  	return $text;
  }

  $keywords = autordf_get_node_keywords(arg(1));
  $tags = array();
  $regex = '';  
  if ($keywords) {     
    $vocabulary = variable_get('autordf_vocabulary_names', FALSE);
    $vids = array_flip($vocabulary);
 
    foreach ($keywords as $keyword) {
      $tags[$keyword->name] = $vids[$keyword->vid];
      $regex .= $keyword->name . '|';
    }
    
    // Removing trailing '|'
    $regex = substr($regex, 0, -1);
  }
  
  if ($regex) {
 
    // $text = preg_replace("/(<a[^><]*>)?($regex)(<\/[\s]*a>)?/e", '(("\1" && "\3")? "\0": "<span>\2</span>")', $text);
    $text = preg_replace_callback("/(<a[^><]*>\s*)?($regex)(<\/[\s]*a>)?/", '_autordf_regex_span', $text);  
  }

	$html_dom = filter_dom_load($text);
  

  $links = $html_dom->getElementsByTagName('a');
  foreach ($links as $link) {
    $link_content = '';
    foreach ($link->childNodes as $child_node) {
      $link_content .= $html_dom->saveXML($child_node);
    }
    
    $vocab = isset($tags[$link_content])? $tags[$link_content]: '';
    if ($vocab) {
      _autordf_prepare_rdf_attributes($link, $vocab, 'a');
    }
    
  }
  
  // Not valid for email address and URL as they are anchored
  $spans = $html_dom->getElementsByTagName('span');
  foreach ($spans as $span) {
    $span_content = '';
    foreach ($span->childNodes as $child_node) {
      $span_content .= $html_dom->saveXML($child_node);
    }
    
    $vocab = isset($tags[$span_content])? $tags[$span_content]: '';
    if ($vocab) {
      _autordf_prepare_rdf_attributes($span, $vocab, 'span');
    }
    
  }

  return filter_dom_serialize($html_dom);
}

function _autordf_regex_span($match) {
  if($match[1] && $match[3]) {
    return $match[0];
  }
  else {
    $str = isset($match[1])? $match[1]: ''; 
    $str .= "<span>".$match[2]."</span>";
    $str .= isset($match[3])? $match[3]: ''; 
    return $str;
  }
}

/**
 * Prepare RDF attributes
 */
function _autordf_prepare_rdf_attributes($element, $vocab, $tag = 'span') {
  switch($vocab) {
    // Add an attribute to link to dbpedia.org
    case 'Person':
      if ($tag == 'a') {
        $element->setAttribute('typeof', 'foaf:Person');
        $element->setAttribute('rel', 'foaf:name');
      }
      else if ($tag == 'span') {
        $element->setAttribute('typeof', 'foaf:Person');
        $element->setAttribute('property', 'foaf:name');      
      }
      break;
      
    case 'EmailAddress':
      if ($tag == 'a') {
      	$element->setAttribute('rel', 'foaf:mbox');
      }
  }
}

/**
 * Implementation of hook_node_view
 */
function autordf_node_view($node, $view_mode) {
  
  $node->content['autordf'] = array(
    '#markup' => autordf_list_topics(),
    '#weight' => 10,
  );

/**  
	$body =& $node->content['body'][0]['#markup'];
  $keywords = autordf_get_node_keywords($node->nid);
  
  $vocabulary = variable_get('autordf_vocabulary_names', FALSE);
  $person_vid = $vocabulary['Person'];

  $tags = array();  
  if ($keywords) {
    foreach ($keywords as $keyword) {
      $tags[$keyword->vid][] = $keyword->name;
    }
  }

  // Testing: Rdfing with foaf:Person   
  $person = isset($tags[$person_vid])? $tags[$person_vid]: array();
  $body = autordf_autordf_person($body, $person);
  **/
}


function autordf_autordf_person($content, $keywords) {
  $nids = array(); 
  foreach ($keywords as $word) {
    $node_id = autordf_make_machinename($word);
    
 //   if (isset($nids[$node_id]) || $word == autordf_derivative($nids)) {
    if (isset($nids[$node_id])) {
      $regex = "/$word/";
      $content = preg_replace($regex, "<foaf:name>$0</foaf:name>", $content);
    }
    else {
      $nids[$node_id] = $word;
      $regex = "/(\.?)([^\.<>]*)($word)([^\.<>]*\.)/";
      $content = preg_replace($regex, "$1<foaf:Person rdf:nodeID=\"$node_id\">$2<foaf:name>$3</foaf:name>$4</foaf:Person>", $content);
    }
  }
  return $content;
}



// Mapping RDF with Fields API 
function autordf_rdf_mapping() {
	return array();
}

function autordf_display_tags($node) {
	$keywords = autordf_get_node_keywords($node->nid);

  $tags = array();  
  if ($keywords) {
    foreach ($keywords as $keyword) {
      $tags[$keyword->vid][] = $keyword->name;
    }
  }
  
  $vocabulary = variable_get('autordf_vocabulary_names', FALSE);
  $vids = array_flip($vocabulary);
  
}

function autordf_block_info() {
  $blocks['topics'] = array(
    'info' => t('Topics in this article'),
    'cache' => DRUPAL_CACHE_PER_PAGE,
    'weight' => 0,    
  );
  
	return $blocks;
}

function autordf_block_configure($delta = '') {
	if ($delta == 'topics') {
		$form['vids'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#default_value' => variable_get('autordf_visible_vocabulary', FALSE),
      '#options' => variable_get('autordf_vocabulary_names', FALSE),
    );
    return $form;
	}
}

function autordf_block_save($delta = '', $edit = array()) {
	if ($delta == 'topics') {
		variable_set('autordf_visible_vocabulary', $edit['vids']);
	}
}

function autordf_block_view($delta = '') {
	if ($delta == 'topics') {
    if ('node' == arg(0) && is_numeric(arg(1)) && !arg(2)) {
		  $block['subject'] = t('Topics in this article');
      $block['content'] = autordf_list_topics();
    }
	}
  return $block;
}

function autordf_list_topics() {
  drupal_add_css(drupal_get_path('module', 'autordf'). '/autordf.css');
  
  $nid = arg(1);
  $keywords = autordf_get_node_keywords($nid);
  
  $vocabulary = variable_get('autordf_vocabulary_names', FALSE);
  $vids = array_flip($vocabulary);
  
  $tags = array();  
  if ($keywords) {
    foreach ($keywords as $keyword) {
      $tags[$vids[$keyword->vid]][] = l($keyword->name, 'taxonomy/term/'.$keyword->tdid);
    }
  }
  
  return theme('autordf_topics', array('tags' => $tags));
}


function theme_autordf_topics($variables) {
  $voc = $variables['tags'];
  $r = "<div id='autordf-topics'>";
  $r .= '<h2>'.t('Topics in this article').'</h2>';
  
	foreach ($voc as $vname => $tags) {  
    $list = array(
      'items' => $tags,
      'title' => $vname,
      'type' => 'ul',
      'attributes' => array('class' => 'autordf-terms'),
    );

	 	$r .= "<div id='autordf-$vname' class='autordf-wrapper'>"; 
    $r .= "<div class='terms'>".theme('item_list', $list)."</div>";
    $r .= "</div>";
	}
  $r .= "</div>";

  return $r;
}