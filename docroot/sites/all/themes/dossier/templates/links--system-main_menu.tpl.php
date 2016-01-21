<?php
// $Id: links--system-main_menu.tpl.php,v 1.2 2010/11/09 18:54:49 chevy Exp $

/**
 * @file links--system-main_menu.tpl.php
 * Links tpl override for the system main_menu region
 */

?>

<?php
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

    $output .= '<div' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    $active = FALSE;
    $prevactive = FALSE;
    foreach ($links as $key => $link) {
      $class = array($key, 'floatleft');
      $aclass = array('floatleft');
      $bclass = array('floatleft');
      $cclass = array('floatleft');

      // Add first, last and active classes to the list of links to help out themers.
      $prevactive = $active;
      $active = FALSE;
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
          && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active-tab';
        $active = TRUE;
      }
      else {
        $class[] = 'nonactive-tab';
      }

      if ($i == 1) {
        $aclass[] = ($active) ? 'active-first':'nonactive-first';
        $output .= '<div' . drupal_attributes(array('class' => $aclass)) . '></div>';
      }
      else {
        if ($prevactive) {
          $bclass[] = 'right-over';
          $output .= '<div' . drupal_attributes(array('class' => $bclass)) . '></div>';
        }
        else {
          $bclass[] = ($active) ? 'left-over':'left-under';
          $output .= '<div' . drupal_attributes(array('class' => $bclass)) . '></div>';
        }
      }

      $output .= '<div' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        //$output .= 'test';
        $output .= l($link['title'], $link['href'], $link);
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

      $output .= '</div>';
      if ($i == $num_links) {
        $cclass[] = ($active) ? 'active-last':'nonactive-last';
        $output .= '<div' . drupal_attributes(array('class' => $cclass)) . '></div>';
      }
      $output .= "\n";
      $i++;
    }
    $output .= '<div class="tabs-border"></div>';

    $output .= '</div>';
  }
  print $output;
?>