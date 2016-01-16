<?php
// Theme name: Grunge Pure
// Theme by: Worthapost
// Website: http://www.worthapost.com
// Author name: Mohd. Sakib
//
// Visit our website to rate this theme and see our Ppremium Themes.




function get_sidebar_state($sidebar_first, $sidebar_last, $right_dark){

	if($sidebar_first && ($sidebar_last || $right_dark)){
		return ' both_sides';
	}
	if($sidebar_first && !$sidebar_last && !$right_dark){
		return ' left_side';
	}
	if(!$sidebar_first && ($sidebar_last || $right_dark)){
		return ' right_side';
	}
	if(!$sidebar_first && !$sidebar_last && !$right_dark){
		return ' no_side';
	}

}



function login_register_links(){
	$login = url(drupal_get_path_alias('user'));
	$register = url(drupal_get_path_alias('user/register'));
	return "<a href=\"$login\">" . t('Login') . '</a>' . " | <a href=\"$register\">" . t('Register') . "</a>";
}


function admire_grunge_welcome_user(){
	global $user;
	$usr_path = 'user/'.$user->uid;
	$myAccount = drupal_get_path_alias($usr_path);
	$logout = drupal_get_path_alias('user/logout');
	if($user->uid)
	{
		$output = theme('item_list', $vars = array 
    		(
        	'items' =>	array(
                		l(t('My account |'), $myAccount, array('title' => t('My account'))),
                		l(t(' Sign out'), $logout)),
                        
            'type' => 'ul',
      		
            )           
        );
		return $output;
	}

	return;
}



function admire_grunge_preprocess_node(&$variables) {
  if ($variables['submitted']) {
    $variables['submitted'] = t('<span class="larger">!day</span> <br/> !datetime', array('!day' => format_date($variables['created'],'custom','D'), '!datetime' => format_date($variables['created'],'custom','m/d/y')));
  }
}




function admire_grunge_comment_submitted($comment) {
    return t('@day @datetime by !username',
    array(
      '!username' => theme('username', $comment),
      '@day' => format_date($comment->timestamp,'custom','D'),
      '@datetime' => format_date($comment->timestamp,'custom','m/d/y'),
    ));
}

function admire_grunge_preprocess_comment(&$variables) {
    $created = $variables['created'];
    $author = $variables['author'];
    $variables['submitted'] = "on $created by $author";
}

function admire_grunge_preprocess_block(&$variables) {
  if ($variables['block_html_id'] === 'desired-id') {
    // do something for this block
  }
}

//modify comment form
function admire_grunge_form($variables) {
	if($variables['element']['#id'] == 'comment-form'){
		  $element = $variables['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }
  // Anonymous DIV to satisfy XHTML compliance.
  return '<form' . drupal_attributes($element['#attributes']) . '><div> <div class="content1"><div class="content2"><div class="content3"><div class="content4">' . $element['#children'] . '</div></div></div></div></div></form>';
		
	}
	else{
		$element = $variables['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }
  // Anonymous DIV to satisfy XHTML compliance.
  return '<form' . drupal_attributes($element['#attributes']) . '><div>' . $element['#children'] . '</div></form>';
	}
}