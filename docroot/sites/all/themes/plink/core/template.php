<?php

/**
 * Start your engines!
 * This class is required because it contains the core processing of the plink system
 **/ 
require_once(dirname(__FILE__) . '/engine/plink.class.php');

/** 
 * Default scripts and styles
 **/ 
drupal_add_css(drupal_get_path('theme', 'plink') . '/css/defaults.css', array('weight' => CSS_THEME, 'type' => 'file'));


/** 
 * IE Media Query Polyfill attachment
 **/ 
drupal_add_js(drupal_get_path('theme', 'plink') . '/js/mqpolyfill.js');




// ////////////////////////////////////////////////////////////////////////////////////////////////
// Preprocess Hooks
// ////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Preprocess function 
 **/
function plink_preprocess(&$vars, $hook) { 
	$plink = Plink::singleton();
	
	if(!$plink->is_processed() && !empty($vars['page'])) {
		$plink->init_processes($vars);
	}
	
}


/**
 * Preprocess function 
 **/
function plink_preprocess_html(&$vars) {
	$plink = Plink::singleton();
	
	// Add doctype information to the theme if rdf is enabled
	$vars['doctype'] = '';
	$vars['rdfversion'] = '';
	
	if(module_exists('rdf')) {
	  $vars['doctype'] = 'PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN"';
	  $vars['rdfversion'] = 'version="HTML+RDFa 1.1"';
	}
	
	// Extend some helpful body classes
	
	$body_classes = $plink->get_body_classes();
	$vars['classes_array'] = array_merge($vars['classes_array'], $body_classes);
	
	if (isset($vars['node'])) { $vars['classes_array'][] = ($vars['node']) ? 'full-node' : ''; }
	
	// Lets do some fun useragent stuff to add special classes.
	// This does pretty much what the browser class module does but 
	// Because there is a bit of a reliance on the fact that the ie
	// classes show up we need to add it here
	// CODE CREDIT: to the author of the browserclass module. hosszukalman :D
	
	if(!module_exists('browserclass')) {
		$agent = strtolower($_SERVER['HTTP_USER_AGENT']);

		  if (stristr($agent, 'msie') !== FALSE) {
		    $vars['classes_array'][] = 'ie';

		    // Add ie extra class with the version number
		    $pattern = '/.*msie ([0-9]*)\..*/';
		    $matches = array();

		    preg_match($pattern, $agent, $matches);
		    if ($matches[1]) {
		      $vars['classes_array'][] = 'ie' . $matches[1];
		    }
		  }
	}
	
	//Add the apple touch icon to the header
	if ($plink->theme_settings()->enable_apple_touch) {
	 
	  drupal_add_html_head_link(
	    array( 'rel' => 'apple-touch-icon', 
	           'href' => url($plink->theme_settings()->apple_touch_icon_path,array('absolute' => TRUE)), 
	    )
	  );
	  
	}
	
}


function plink_preprocess_page(&$vars) { 	
	
	// Initialize engine
	$plink = Plink::singleton();
	
	// avoid php notices mostly
	$vars['container_classes'] = '';
	$vars['primary_classes'] = '';
	$vars['secondary_classes'] = '';
	$vars['tertiary_classes'] = '';
	$vars['title_prefix'] = array();
	$vars['title_suffix'] = array();
			
	// hacky hack to be able to place blocks into the title_prefix and suffix variables
	// These only show if the page is not a node.
	$title_blocks = $plink->get_title_blocks();
	
	if(isset($title_blocks['prefix'])) {
	  $vars['title_prefix'] = array_merge($vars['title_prefix'],$title_blocks['prefix']);
  }
  
  if(isset($title_blocks['title_suffix'])) {
    $vars['title_suffix'] = array_merge($vars['title_suffix'],$title_blocks['suffix']);
  }
  
  
	  // Grid Based layouts
	  $vars['primary_classes'] .= $plink->get_content_region_classes('primary');
	  $vars['secondary_classes'] .= $plink->get_content_region_classes('secondary');
	  $vars['tertiary_classes'] .= $plink->get_content_region_classes('tertiary');
	  $vars['container_classes'] .= $plink->get_main_region_classes();
	  $vars['container_classes'] .= ' ' ;
	  $vars['container_classes'] .= $plink->get_media_query_region_classes();

	
	//Set the breadcrumbs flag
	$vars['breadcrumbs_enabled'] = $plink->theme_settings()->enable_breadcrumbs;		
}

/**
 * Preprocess function 
 **/
function plink_preprocess_region(&$vars) {
	$plink = Plink::singleton();
	$vars['inner_classes'] = '';
	
	// add grid classes to the regions if grid is enabled
  $vars['classes_array'][] = $plink->get_region_classes($vars['region']);
  $vars['inner_classes'] = $plink->get_region_inner_classes($vars['region']);
  
}

/**
 * Preprocess function 
 **/
// function plink_preprocess_maintenance_page(&$vars) { }


/**
 * Preprocess function 
 **/
function plink_preprocess_node(&$vars) { 
	$node_classes = array();
	global $user;
	$plink = Plink::singleton();
		
	// Get the title blocks from the plink engine when we stored them in preprocess_page
	// We will want to add them to the node title as well for consistency
	$title_blocks = $plink->get_title_blocks();
	if(isset($title_blocks['prefix'])) {
	  $vars['title_prefix'] = array_merge($vars['title_prefix'],$title_blocks['prefix']);
  }
  if(isset($title_blocks['title_suffix'])) {
    $vars['title_suffix'] = array_merge($vars['title_suffix'],$title_blocks['suffix']);
  }
		
		
	// Lets add some classes to the node 	
  if ($vars['sticky']) {
		$node_classes[] = 'sticky';
	}
	if ($vars['promote']) {
		$node_classes[] = 'promoted';
	}
	if (!$vars['node']->status) {
		$node_classes[] = 'node-unpublished';
		$vars['unpublished'] = TRUE;
	}
	else {
		$vars['unpublished'] = FALSE;
	}
	if ($vars['node']->uid && $vars['node']->uid == $user->uid) {
		// Node is authored by current user
		$node_classes[] = 'user-me';
	}
	//Add node's user id
	$node_classes[] = 'user-' . $vars['node']->uid;
	
	if ($vars['teaser']) {
		// Node is displayed as teaser
		$node_classes[] = 'node-teaser';
	}
	else {
		$node_classes[] = 'node-full';
	}
	//odd/even class for node listings
	$node_classes[] = $vars['zebra'];
	//node count for node listings
	$node_classes[] = 'count-' . $vars['id'];
	// Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
	$node_classes[] = 'node node-type-'. $vars['node']->type;
	
	$vars['classes_array'] = array_merge($vars['classes_array'], $node_classes);
	
	//Add custom meta info
  if($plink->theme_settings()->enable_meta_info) {
   $vars['submitted'] = token_replace($plink->theme_settings()->meta_info_string, array('node' => $vars['node']));
  }
  
}


/**
 * Preprocess function 
 **/
function plink_preprocess_block(&$vars) { 
	$plink = Plink::singleton();
	$block_classes = $plink->get_block_classes($vars);
	$vars['classes_array'] = array_merge($vars['classes_array'], $block_classes);
}


/**
 * Preprocess function 
 **/
function plink_preprocess_comment(&$vars) {
  static $comment_counter = array();
  static $comment_odd = TRUE;

  if (!isset($comment_counter[$vars['node']->nid])) {
    $comment_counter[$vars['node']->nid] = 1;
  }
  // Add to array of handy comment classes
  $vars['classes_array'][] = $comment_odd ? 'odd' : 'even';
  $comment_odd = !$comment_odd;

  $vars['classes_array'][] = 'comment-'.$vars['comment']->cid;
  $comment_counter[$vars['node']->nid]++;

  // Add classes based on the role(s) of the comment author
  $account = user_load($vars['comment']->uid);
  foreach ($account->roles as $role) {
    $vars['classes_array'][] = "role-".drupal_clean_css_identifier($role);
  }
}


// ////////////////////////////////////////////////////////////////////////////////////////////////
// Process hooks
// ////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Preprocess function 
 **/
// function plink_process(&$vars, $hook) { }

/**
 * Preprocess function 
 **/
// function plink_process_html(&$vars) { }

/**
 * Preprocess function 
 **/
// function plink_process_page(&$vars) { }

/**
 * Preprocess function 
 **/
// function plink_process_region(&$vars) {}

// function plink_process_node(&$vars) { 
// }

/**
 * Preprocess function 
 **/
// function plink_process_block(&$vars) { }

/**
 * Preprocess function 
 **/
// function plink_process_comment(&$vars) { }


// ////////////////////////////////////////////////////////////////////////////////////////////////
// MISC
// ////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Add default meta tags to head document
 **/
function plink_add_meta_head(&$vars) {
  $data = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => "X-UA-Compatible",
      'content' => "IE=edge,chrome=1",
      )
    );
  
  drupal_add_html_head($data,'plink');
}




