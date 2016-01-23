<?php
function chocotheme_links__system_main_menu(&$variables) {
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

      // Add first, last and active classes to the list of links to help out themers .
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

/**
 * Override of the Search Box for d6
 * first, select the form ID
 */
function chocotheme_theme() {
    return array(
    // The form ID.
    'search_block_form' => array(
      // Forms always take the form argument.
      'arguments' => array('form' => NULL),
    ),
  );
}
/**
 * Implementing hook form_alter for search form
 * @param unknown_type $form
 * @param unknown_type $form_state
 * @param unknown_type $form_id
 */

/*function chocotheme_search_block_form($form) {
    $form['search_block_form'] ['#title']=NULL;
    $form['search_block_form']['#attributes']=array("class" => "field");
    $form['submit']['#value']=NULL;
    $form['submit']['#attributes']= array("class" => "button");
    $output .= drupal_render($form);
    return $output;
}*/

function chocotheme_form_alter(&$form, &$form_state, $form_id) {
	 if ($form_id == 'search_block_form') {
      $form['actions']['submit']['#value'] = ''; // Change the text on the submit button
   		$form['search_block_form']['#attributes']['placeholder'] =  t('Search');
  }
	}

function chocotheme_comment_submitted($comment) {
    return t('<p class="author">!username</p><p class="comment-meta" >@date at about @time.</p>', array(
    '!username' => theme('username', $comment),
    '@date' => format_date($comment->timestamp, 'custom', 'd M Y'),
    '@time' => format_date($comment->timestamp, 'custom', 'H:i')
    ));
}
