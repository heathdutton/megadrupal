<?php

function traction_preprocess_page(&$variables) {
	if (isset($variables['main_menu'])) {
		$variables['primary_nav'] = theme('links__system_main_menu', array(
          'links' => $variables['main_menu'],
          'attributes' => array(
            'id' => 'menu-top-menu',
            'class' => array('nav sf-js-enabled', 'inline', 'clearfix'),
          ),
         
        )); 
  }
  else {
    $variables['primary_nav'] = FALSE;
  }
	if (isset($variables['secondary_menu'])) {
    $variables['secondary_nav'] = theme('links__system_secondary_menu', array(
          														'links' => $variables['secondary_menu'],
          														'attributes' => array(
            													'id' => 'menu-header-navigation',
           													  'class' => array('nav sf-js-enabled', 'inline', 'clearfix'),
          														),
         
       														 ));
  }
  else {
    $variables['secondary_nav'] = FALSE;
  }
  $variables['node_created'] = '';  
  $variables['node_comment_count'] = '';
	$node_value = arg(1);
		if(is_numeric($node_value)) {
			$node = node_load($node_value);
		
			//if($node->type == 'article')
			if(is_object($node) && $node->type == 'article')
			{
			
				$variables['node_created']   = format_date($node->created, 'custom', 'M j , Y');
				$variables['node_comment_count'] = $node->comment_count;
			}
			 
		}	
	
}
 
function traction_preprocess_node(&$variables) {
  
  $variables['thumb'] = '';
  $variables['image_url'] = '';
  $variables['day'] = '';
  $variables['my'] = '';
  $variables['node_article'] = false;
  
  $uid = $GLOBALS['user']->uid;
  if(!$uid) {
    unset($variables['content']['links']['comment']['#links']['comment-add']);
  }
  else {
    $variables['content']['links']['comment']['#links']['comment-add']['title']='Add Comment';
  }
  $d = $variables['created'];
  
  $variables['day']=date('d', $d);
  $variables['my']=date('M', $d);
  
  $variables['submitted'] = 'by ' . $variables['name'];

  $node_value = arg(1);
		if(is_numeric($node_value)) {
			$node = node_load($node_value);
		
			if($node->type == 'article') {
				$variables['node_article'] = true;
				if(!empty($node->field_image)) {
					$image_url =  image_style_url('medium', $node->field_image['und'][0]['uri']);
					$variables['image_url'] = $image_url;
				}
				
			}
			 
		}
	
  if ($variables['view_mode'] == 'teaser') {
  	 if($variables['node']->type == 'article') {
				$variables['node_article'] = true;
		 }
		 
     if(!empty($variables['node']->field_image)) {
 	        $filename = $variables['node']->field_image['und'][0]['uri'];
  	      $newimage= image_style_url('medium', $filename);
  	      $variables['thumb'] = $newimage;
     }
     
     $variables['readmore'] = l('Read more', 'node/' . $variables['node']->nid, array('attributes' => array('class' => array('more-link'))))
;      
   }

}

function traction_preprocess_comment(&$variables) {
	$theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
	$variables['created']   = format_date($variables['comment']->created, 'custom', 'M jS Y');
	$variables['default_photo'] = '<img src="' . base_path( ) . $theme_path . '/images/default.png" />';
	
}
