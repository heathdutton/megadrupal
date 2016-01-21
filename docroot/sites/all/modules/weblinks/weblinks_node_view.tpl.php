<?php

/**
 * @file
 * weblinks_node_view.tpl.php
 * Theme implementation to display a list of related content.
 *
 * Available variables:
 * - $node: node object that contains the URL.
 * - $options: Options for the URL.
 */
if ($node->url != NULL) {
  switch (variable_get('weblinks_view_as', 'url')) {
    case 'url':
      if (variable_get('weblinks_strip', FALSE)) {
        // Note this also strips a port.
        $parts = parse_url($node->url);
        $parts['host'] = str_replace('www.', '', $parts['host']);
        $url = $parts['host'] . ($parts['path'] == '/' ? '' : $parts['path']);
      }
      else {
        $url = $node->url;
      }
      $title = _weblinks_trim($url, variable_get('weblinks_trim', 0));
      break;

    case 'visit':
      $title_text = variable_get('weblinks_visit_text', t('Visit [title]'));
      if (function_exists('token_replace')) {
        $title = token_replace($title_text, array('node' => $node));
      }
      else {
        $title = str_replace('[title]', $node->title, $title_text);
      }
      break;
  }

  if (variable_get('weblinks_redirect', FALSE)) {
    $node->weblinks_title_link = 'weblinks/goto/'. $node->nid;
  }
  else {
    $node->weblinks_title_link = $node->url;
  }
  $link = l($title, $node->weblinks_title_link, $options);
  if (arg(0) == 'node' || variable_get('weblinks_show_url', TRUE)) {
    echo '<div class="weblinks-linkview">'. $link .'</div><!--class="weblinks-linkview"-->';
  }

  // Make the options available to the theme, just in case the user wants them.
  // <h2 class="title">< ?php print l($title, $node_url, $weblinks_options);? ></h2>
  $node->weblinks_options = $options;
}
