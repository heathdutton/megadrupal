<?php

function hatch_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#title'] = t('Search this site'); // Change the text on the label element
    $form['search_block_form']['#title_display'] = 'invisible'; // Toggle label visibilty
    $form['search_block_form']['#size'] = 40;  // define size of the textfield
    $form['search_block_form']['#default_value'] = t('Search this site'); // Set a default value for the textfield
    
    $form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search this site';}";
    $form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search this site') {this.value = '';}";
  }
}
 
function hatch_form_comment_form_alter(&$form, &$form_state) {
 global $user;
   //shows original $form array
  /*$form['author']['#type'] = 'fieldset';
  $form['author']['#title'] = '';
  $form['author']['#collapsible'] = FALSE;*/
  // Add the author name field depending on the current user.
  //if ($is_admin) {
    /*$form['author']['name'] = array(
      '#type' => 'textfield', 
      '#title' => t('Authored by'), 
      '#default_value' => $author, 
      '#maxlength' => 60, 
      '#size' => 30, 
      '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))), 
      '#autocomplete_path' => 'user/autocomplete',
    );
  /*}
  elseif ($user->uid) {
    $form['author']['_author'] = array(
      '#type' => 'item', 
      '#title' => t('Name'), 
      '#markup' => theme('username', array('account' => $user)),
    );
    $form['author']['name'] = array(
      '#type' => 'value', 
      '#value' => $author,
    );
  }*/
  //else {
    
  //}
 $form['author']['name'] = array(
      '#type' => 'textfield', 
      '#title' => t('Name'), 
      '#default_value' => $author, 
      '#maxlength' => 60, 
      '#size' => 30, 
      
      );
  $form['author']['mail'] = array(
    '#type' => 'textfield', 
    '#title' => t('E-mail'), 
    '#default_value' => $comment->mail, 
    '#maxlength' => 64, 
    '#size' => 30, 
  );
  $form['author']['website'] = array(
    '#type' => 'textfield', 
    '#title' => t('Website'), 
    '#default_value' => $comment->mail, 
    '#maxlength' => 64, 
    '#size' => 30, 
  );
  
  unset($form['subject']);
  $form['author']['homepage']['#access'] = FALSE;
  $form['actions']['submit'] = array('#type' => 'submit', 
    '#value' => t('Post Comment→'), 
    '#access' => ($comment->cid && user_access('administer comments')) || variable_get('comment_preview_' . $node->type, DRUPAL_OPTIONAL) != DRUPAL_REQUIRED || isset($form_state['comment_preview']), 
    '#weight' => 6,
  );
  $form['author']['mail']['#required'] = TRUE;
  $form['author']['name']['#required'] = TRUE;
  
 
  /*unset($form['actions']['preview']);*/
}
function hatch_preprocess_node(&$variables) {
  $variables['view_mode'] = 'teaser';
  $variables['common_pages'] = ''; 
  $variables['page'] = TRUE;
  $node = $variables['node'];
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  $variables['article']='';
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $node = node_load(arg(1));
    if($node->type == 'article') {
      $variables['article'] = true;
    }
    if($node->type == 'page') {
      $variables['common_pages'] = true;
    }
    $variables['nextandprev']=nextandprev();
  }
  if (!isset($node->field_image['und'][0]['uri'])){
    $variables['image'] = base_path() . $theme_path . '/images/archive_image_placeholder.png';
  }
  else {
  	$img = $node->field_image['und'][0]['uri'];
  	$teaser = $variables['teaser'];
  	if ($teaser){
  	  $variables['image'] = image_style_url('medium',$img);
  	 }
  	 else{
  	  $variables['image']=image_style_url('large',$img);
  	 }
  }
}
function hatch_preprocess_page(&$variables){ 
  
  $variables['pages'] = ''; 
   if (arg(0) == 'comment' && arg(1) == 'reply'){
	 	$node = node_load(arg(3));
	 	$variables['theme_hook_suggestions'][] =  'page__reply';
	 	}
    if (arg(0) == 'node' && is_numeric(arg(1))) {
      $node = node_load(arg(1));
      if ($node->type == 'article') {
        $variables['article'] = true;
      //page--article.tpl.php
        $variables['theme_hook_suggestions'][] =  'page__article';
      }
	  }	
}

function nextandprev(){
	$node_id = arg(1);
		if (is_numeric($node_id)) {
			$node = node_load($node_id);
			$next = node_sibling($node, 'next');
			$previous = node_sibling($node, 'prev');
			if ($previous != '') {
				$prev_node=node_load($previous);
				
			  $output = l('←' . $prev_node->title, 'node/' . $previous,array('attributes' => array('class' => array('prev')))); 
				$output .='<br/>';
			} else { 
				$output =false;	
			}
			
			if ($next != '') {	
				$next_node=node_load($next);
				$output .= l($next_node->title . ' →', 'node/' . $next,array('attributes' => array('class' => array('next'))));
				$output .='<br/>';
			} else {
				$output .= false;
			}
		
			return $output;
			
    }
}
function node_sibling($node, $dir='next') {
  $query = 'SELECT n.nid, n.title FROM {node} n WHERE '. 'n.created ' . ($dir == 'prev' ? '<' : '>') . ' :created AND
n.type = :type AND n.status = 1 '
              . "AND language IN (:lang, 'und') "
   . 'ORDER BY n.created ' . ($dir == 'prev' ? 'DESC' : 'ASC') . '
LIMIT 1';
 //use fetchObject to fetch a singlerow
 $row = db_query($query, array(':created' => $node->created, ':type' =>
 $node->type, ':lang' => $node->language))->fetchObject();

 if ($row) {
   return $row->nid;
 } else {
   return FALSE;
 }
}
function hatch_preprocess_comment(&$vars) {
	$theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  $vars['submitted'] = $vars['created'] ;
   $vars['default_photo'] = '<img src="' . base_path( ) . $theme_path . '/images/default.png" />';
}
/**
 * Override of theme('field').
 */
function hatch_field($variables) {
 
  $output = '';
  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }
 
  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
 
  if ($variables['element']['#field_name'] == 'field_tags') {
    // For tags, concatenate into a single, comma-delimitated string.
    foreach ($variables['items'] as $delta => $item) {
      $rendered_tags[] = drupal_render($item);
    }
    $output .= implode(', ', $rendered_tags);
  } else {
    // Default rendering taken from theme_field().
    foreach ($variables['items'] as $delta => $item) {
      $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
      $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
    }
  }
  $output .= '</div>';
 
  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';
 
  return $output;
}

