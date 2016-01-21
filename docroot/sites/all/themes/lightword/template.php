<?php

function lightword_preprocess_page(&$variables) {
		
  drupal_add_css(path_to_theme() .'/css/style.css', 'theme', 'all', $preprocess);
  		
  //select normal or wider css
  if (theme_get_setting('screen_settings')=='wider'){
  	drupal_add_css(path_to_theme() .'/css/wider.css', 'theme', 'all', $preprocess);
  }else{
  	drupal_add_css(path_to_theme() .'/css/original.css', 'theme', 'all', $preprocess);
  }
  //select cufon type
  if (theme_get_setting('cuf_settings')=='enabled'){
  	drupal_add_js(path_to_theme() .'/js/cufon-yui.js', 'file');
  	drupal_add_js("Cufon.replace(['h1','h2','h3#reply-title'], { fontFamily: 'Vera' });", 'inline');		
  	drupal_add_js(path_to_theme() .'/js/vera.font.js', 'file');
  }else if(theme_get_setting('cuf_settings')=='extra'){
  	drupal_add_js(path_to_theme() .'/js/cufon-yui.js', 'file');
  	drupal_add_js("Cufon.replace(['h1','h2','h3#reply-title'], { fontFamily: 'Vera' });", 'inline');
  	drupal_add_js(path_to_theme() .'/js/vera_extra.font.js', 'file');
  }else if(theme_get_setting('cuf_settings')=='css3'){
  	$pat	=	'@font-face { font-family: Vera; src: url('.path_to_theme().'/font-face/Vera-Bold.ttf);}';
  	drupal_add_css($pat, 'inline');
  }	
}
function lightword_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
  		$form['search_block_form']['#size'] = 11;  // define size of the textfield
  		$form['actions']['submit']['#value'] = t('GO!'); // Change the text on the submit button		
  }
}


function lightword_preprocess_node(&$variables) {
  $uid = $GLOBALS['user']->uid;
  if(!$uid) {
    unset($variables['content']['links']['comment']['#links']['comment-add']);
  }
  else {
    $variables['content']['links']['comment']['#links']['comment-add']['title']='Add Comment';
  }
  $d = $variables['created'];
  $variables['day']=date('d', $d);
  $variables['my']=date('M/y', $d);
}
function lightword_preprocess_field(&$variables) {		
  if($variables['element']['#field_name']=='field_tags'){
  	$variables['element']['#theme']='custom_field';
  }		
}


function lightword_links__system_main_menu(&$variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    $output = '';

    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = array($key);

      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
          && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $link['html']=True;
        $output .= l('<span>'.$link['title'].'</span>', $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
      $output .= "</li>\n";
    }

    $output .= '</ul>';
  }

  return $output;
}