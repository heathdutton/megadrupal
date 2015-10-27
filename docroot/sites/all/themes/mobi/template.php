<?php

/**
 * @return string
 */
function phptemplate_breadcrumb($breadcrumb) {
  if(sizeof($breadcrumb) > 0) {
    return implode(' &raquo; ', $breadcrumb);
	}
	return '';
}
/**
 * Override formatting of primary and secondary links.
 *
 * @return string
 */
function phptemplate_links($links) {
  $output = '';

  if (count($links) > 0) {
    $num_links = count($links);
    $i = 1;

    foreach ($links as $link) {
      $html = isset($link['html']) && $link['html'];

      $link['query'] = isset($link['query']) ? $link['query'] : NULL;
      $link['fragment'] = isset($link['fragment']) ? $link['fragment'] : NULL;

      if (isset($link['href'])) {
        $output .= l($link['title'], $link['href'], array(
          'attributes' => array('accesskey' => $i),
          'query' => $link['query'],
          'fragment' => $link['fragment'],
          'absolute' => FALSE,
          'html' => $html)
        );
      }
      else if ($link['title']) {
        if (!$html) {
          $link['title'] = check_plain($link['title']);
        }
        $output .= $link['title'];
      }
      $output .= ' | ';
      $i++;
    }
  }
  return empty($output) ? '' : substr($output, 0, -3);
}
