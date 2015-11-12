<?php 

/**
 * Page preprocess hook
 * @param array pointer $variables
 */

function mystique_theme_preprocess_page(&$variables) {
  
  // selecing the color style based on theme settings
  if (theme_get_setting('color_settings')=='green') {
    drupal_add_css(path_to_theme() .'/css/color-green.css', 'theme', 'all');
  }else if(theme_get_setting('color_settings')=='blue') {
    drupal_add_css(path_to_theme() .'/css/color-blue.css', 'theme', 'all');
  }
  else if(theme_get_setting('color_settings')=='grey') {
    drupal_add_css(path_to_theme() .'/css/color-grey.css', 'theme', 'all');
  }
  else if(theme_get_setting('color_settings')=='red') {
    drupal_add_css(path_to_theme() .'/css/color-red.css', 'theme', 'all');
  }

  if (theme_get_setting('font_settings')=='arial') {
    drupal_add_css('body *{font-family:"Helvetica Neue",Helvetica,Arial,Geneva,"MS Sans Serif",sans-serif}',$option['type'] = 'inline');
  }
  elseif (theme_get_setting('font_settings')=='georgia') {
    drupal_add_css('body *{font-family:Georgia,"Nimbus Roman No9 L",serif}',$option['type'] = 'inline');
  }
  elseif (theme_get_setting('font_settings')=='lucida') {
    drupal_add_css('body *{font-family:"Lucida Grande","Lucida Sans","Lucida Sans Unicode","Helvetica Neue",Helvetica,Arial,Verdana,sans-serif}',$option['type'] = 'inline');
  }
  
  $search_box = drupal_render(drupal_get_form('search_form'));
  $variables['search_box'] = $search_box;
}


/**
 * Implementing hook form_alter for search form
 * @param unknown_type $form
 * @param unknown_type $form_state
 * @param unknown_type $form_id
 */

function mystique_theme_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
  	$form['search_block_form']['#attributes']['placeholder'] =  t('Search');
  }
}

/**
 * Hook for theme main menu
 * @param array pointer $variables
 * @return string
 */
function mystique_theme_links__system_main_menu(&$variables) {
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
        $output .= l('<span class="title">'. $link['title'] .'</span>
                                <span class="pointer"></span>
                                <span class="hover" style="opacity: 0;">
                                </span>', $link['href'], $link);
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

function mystique_theme_preprocess_html(&$vars) {
	if (theme_get_setting('page_width') == 'fixed') {
    drupal_add_css(path_to_theme() .'/fixed.css');
  }
  else {
    drupal_add_css(path_to_theme() .'/fluid.css');
  }
  if (theme_get_setting('page_width') == 'fixed') {
    $vars['classes_array'][] = 'fixed ';
  }
  else {
    $vars['classes_array'][] = 'fluid ';
  }
	$right = (isset($vars['page']['sidebar_first'])) ? $vars['page']['sidebar_first'] : '';
	$left  = (isset($vars['page']['sidebar_second'])) ? $vars['page']['sidebar_second'] : '';
	if ($right && $left){
    $vars['classes_array'][] = 'col-3 ';
  }
  elseif($right && !$left) {
  	$vars['classes_array'][] = 'col-2-right ';
  }
  elseif($left && !$right) {
  	$vars['classes_array'][] = 'col-2-left ';
  }
  elseif(!$right && !$left) {
  	$vars['classes_array'][] = 'col-1 ';
  }
}
