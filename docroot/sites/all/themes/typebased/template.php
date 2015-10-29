<?php
// $Id: template.php,v 1.6 2010/12/31 15:00:09 jarek Exp $

/**
 * Implements hook_css_alter().
 */
function typebased_css_alter(&$css) {
  unset($css[drupal_get_path('module', 'system') . '/admin.css']);
  unset($css[drupal_get_path('module', 'aggregator') . '/aggregator.css']);
  unset($css[drupal_get_path('module', 'block') . '/block.css']);
  unset($css[drupal_get_path('module', 'book') . '/book.css']);
  unset($css[drupal_get_path('module', 'comment') . '/comment.css']);
  unset($css[drupal_get_path('module', 'field') . '/theme/field.css']);
  unset($css[drupal_get_path('module', 'filter') . '/filter.css']);
  unset($css[drupal_get_path('module', 'forum') . '/forum.css']);
  unset($css[drupal_get_path('module', 'locale') . '/locale.css']);
  unset($css[drupal_get_path('module', 'node') . '/node.css']);
  unset($css[drupal_get_path('module', 'openid') . '/openid.css']);
  unset($css[drupal_get_path('module', 'poll') . '/poll.css']);
  unset($css[drupal_get_path('module', 'search') . '/search.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.base.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.behavior.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.theme.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.menus.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.messages.css']);
  unset($css[drupal_get_path('module', 'user') . '/user.css']);
  unset($css['misc/vertical-tabs.css']);
}

/**
 * Implements template_preprocess_html().
 */
function typebased_preprocess_html(&$variables) {
  // Add reset CSS
  drupal_add_css($data = path_to_theme() . '/reset.css', $options['type'] = 'file', $options['weight'] = CSS_SYSTEM - 1);

  // Add conditional stylesheet for IEs
  drupal_add_css(path_to_theme() . '/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE)); 

  // Add body classes with information about number of columns.
  foreach ($variables['classes_array'] as $i => $value) {
    if ($variables['classes_array'][$i] == 'one-sidebar sidebar-first' || $variables['classes_array'][$i] == 'one-sidebar sidebar-second' || $variables['classes_array'][$i] == 'two-sidebars' || $variables['classes_array'][$i] == 'no-sidebars') {
      unset($variables['classes_array'][$i]); 
    }
  }
  if ( !empty($variables['page']['sidebar']) ) {
    $variables['classes_array'][] = 'one-sidebar';
  }
  else {
    $variables['classes_array'][] = 'no-sidebars';
  }
}

/**
 * Implements template_process_html().
 */
function typebased_process_html(&$variables) {
  // Hook into color module
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Implements template_preprocess_page().
 */
function typebased_preprocess_page(&$variables) {
  // Generate dynamic styles
  $base_font_size = theme_get_setting('base_font_size');
  $layout_1_width = theme_get_setting('layout_1_width');
  $layout_1_min_width = theme_get_setting('layout_1_min_width');
  $layout_1_max_width = theme_get_setting('layout_1_max_width');
  $layout_2_width = theme_get_setting('layout_2_width');
  $layout_2_min_width = theme_get_setting('layout_2_min_width');
  $layout_2_max_width = theme_get_setting('layout_2_max_width');
  $dynamic_styles = "html { font-size: $base_font_size; } body.no-sidebars #wrapper { width: $layout_1_width; min-width: $layout_1_min_width; max-width: $layout_1_max_width;} body.one-sidebar #wrapper { width: $layout_2_width; min-width: $layout_2_min_width; max-width: $layout_2_max_width;}";
  drupal_add_css($data = $dynamic_styles, $options['type'] = 'inline', $options['preprocess'] = TRUE );
}

/**
 * Implements template_process_page().
 */
function typebased_process_page(&$variables) {
}

/**
 * Implements template_preprocess_block().
 */
function typebased_preprocess_block(&$variables) {
  // Remove "block" class from blocks in "Main page content" region
  if ($variables['elements']['#block']->region == 'content') {
    foreach ($variables['classes_array'] as $key => $val) {
      if ($val == 'block') {
        unset($variables['classes_array'][$key]);
      }
    }
  }
  $variables['classes_array'][] = 'block-content';
}

/**
 * Overrides theme_more_link().
 */
function typebased_more_link($variables) {
  // Append arrow
  return '<div class="more-link">' . t('<a href="@link" title="@title">More ›</a>', array('@link' => check_url($variables['url']), '@title' => $variables['title'])) . '</div>';
}

/**
 * Overrides theme_more_messages().
 */
function typebased_messages($variables) {
  // Separate the status messages out into their own divs.
  $display = $variables['display'];
  $output = '';
  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
    }
    foreach ($messages as $message) {
      $output .= "<div class=\"messages message-$type\">\n";
      $output .= $message;
      $output .= "</div>\n";
    }
  }
  return $output;
}

/**
 * Overrides theme_node_recent_block().
 */
function typebased_node_recent_block($variables) {
  // Make output for "Recent content" block consistent with other blocks
  $output = '';

  foreach ($variables['nodes'] as $node) {
    $items[] = theme('node_recent_content', array('node' => $node));
  }

  return theme('item_list', array('items' => $items));
}

/**
 * Overrides theme_node_recent_content().
 */
function typebased_node_recent_content($variables) {
  // Make output for "Recent content" block consistent with other blocks
  $node = $variables['node'];
  $output = l($node->title, 'node/' . $node->nid);
  $output .= theme('mark', array('type' => node_mark($node->nid, $node->changed)));
  return $output;
}

/**
 * Overrides theme_tablesort_indicator().
 */
function typebased_tablesort_indicator($variables) {
  // Use custom arrow images
  if ($variables['style'] == "asc") {
    return theme('image', array('path' => path_to_theme() . '/images/sort-down.png', 'alt' => t('sort ascending'), 'title' => t('sort ascending')));
  }
  else {
    return theme('image', array('path' => path_to_theme() . '/images/sort-up.png', 'alt' => t('sort descending'), 'title' => t('sort descending')));
  }
}

/**
 * Overrides theme_pager().
 */
function typebased_pager($variables) {
  // Make pager look and behave like on most blogs and community websites
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $pager_width = theme_get_setting('trim_pager');
  global $pager_page_array, $pager_total;
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('‹ previous')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('next ›')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $total_number_of_pages = $pager_total[$element];
  $current_page_number = $pager_page_array[$element] + 1;

  /* If there is just one page we don't need a pager. */
  if ($total_number_of_pages <= 1) {
    return;
  }

  /* Elipsis does not make sense if there is just one page more than the pager width. */
  if ($total_number_of_pages - $pager_width == 1) {
    $pager_width++;
  }

  /* Genarate pager without any elipsis */
  if ($total_number_of_pages <= $pager_width) {
    if ($li_previous) {
      $items[] = array('class' => array('pager-previous'), 'data' => $li_previous,);
    }
    for ($i = 1; $i <= $total_number_of_pages; $i++) {
      if ($i < $current_page_number) {
        $items[] = array('class' => array('pager-item'), 'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($current_page_number - $i), 'parameters' => $parameters)) );
      }
      if ($i == $current_page_number) {
        $items[] = array('class' => array('pager-current'), 'data' => $i);
      }
      if ($i > $current_page_number) {
        $items[] = array('class' => array('pager-item'), 'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $current_page_number), 'parameters' => $parameters)) );
      }
    }
    if ($li_next) {
      $items[] = array('class' => array('pager-next'), 'data' => $li_next );
    }
  }

  /* Genarate pager with elipsis */
  if ($total_number_of_pages > $pager_width) {

    /* Genarate pager with elpisis on right side. */
    if ($current_page_number < $pager_width) {
      if ($li_previous) {
        $items[] = array('class' => array('pager-previous'), 'data' => $li_previous,);
      }
      for ($i = 1; $i <= $pager_width; $i++) {
        if ($i < $current_page_number) {
          $items[] = array('class' => array('pager-item'), 'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($current_page_number - $i), 'parameters' => $parameters)) );
        }
        if ($i == $current_page_number) {
          $items[] = array('class' => array('pager-item pager-current'), 'data' => $i);
        }
        if ($i > $current_page_number) {
          $items[] = array('class' => array('pager-item'), 'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $current_page_number), 'parameters' => $parameters)) );
        }
      }
      $items[] = array(
        'class' => array('pager-ellipsis'),
        'data' => '…',
      );
      $items[] = array('class' => array('pager-item'), 'data' => theme('pager_last', array('text' => $total_number_of_pages, 'element' => $element, 'interval' => 1, 'parameters' => $parameters)) );
      $items[] = array('class' => array('pager-next'), 'data' => $li_next );
    }

    /* Genarate pager with elpisis on both sides. */
    if($current_page_number >= $pager_width && $current_page_number <= $total_number_of_pages - $pager_width + 1) {
      if ($li_previous) {
        $items[] = array('class' => array('pager-previous'), 'data' => $li_previous,);
      }
      $items[] = array('class' => array('pager-item'), 'data' => theme('pager_first', array('text' => "1", 'element' => $element, 'interval' => 1, 'parameters' => $parameters)) );
      $items[] = array(
        'class' => array('pager-ellipsis'),
        'data' => '…',
      );
      function isEven($num){
        return ($num%2) ? TRUE : FALSE;
      }
      if (isEven($pager_width) == TRUE) {
        $a = floor($pager_width/2);
      }
      if (isEven($pager_width) == FALSE) {
        $a = floor($pager_width/2) - 1;
      }
      for ($i = $current_page_number - $a; $i <= $current_page_number + floor($pager_width/2); $i++) {
        if ($i < $current_page_number) {
          $items[] = array('class' => array('pager-item'), 'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($current_page_number - $i), 'parameters' => $parameters)) );
        }
        if ($i == $current_page_number) {
          $items[] = array('class' => array('pager-current'), 'data' => $i);
        }
        if ($i > $current_page_number) {
          $items[] = array('class' => array('pager-item'), 'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $current_page_number), 'parameters' => $parameters)) );
        }
      }
      $items[] = array(
        'class' => array('pager-ellipsis'),
        'data' => '…',
      );
      $items[] = array('class' => array('pager-item'), 'data' => theme('pager_last', array('text' => $total_number_of_pages, 'element' => $element, 'interval' => 1, 'parameters' => $parameters)) );
      $items[] = array('class' => array('pager-next'), 'data' => $li_next );
    }

    /* Genarate pager with elpisis on left side. */
    if($current_page_number >= $pager_width && $current_page_number > $total_number_of_pages - $pager_width + 1) {
      if ($li_previous) {
        $items[] = array('class' => array('pager-previous'), 'data' => $li_previous,);
      }
      $items[] = array('class' => array('pager-item'), 'data' => theme('pager_first', array('text' => "1", 'element' => $element, 'interval' => 1, 'parameters' => $parameters)) );
      $items[] = array(
        'class' => array('pager-ellipsis'),
        'data' => '…',
      );
      for ($i = $total_number_of_pages - $pager_width + 1; $i <= $total_number_of_pages; $i++) {
        if ($i < $current_page_number) {
          $items[] = array('class' => array('pager-item'), 'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($current_page_number - $i), 'parameters' => $parameters)) );
        }
        if ($i == $current_page_number) {
          $items[] = array('class' => array('pager-current'), 'data' => $i);
        }
        if ($i > $current_page_number) {
          $items[] = array('class' => array('pager-item'), 'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $current_page_number), 'parameters' => $parameters)) );
        }
      }
      $items[] = array('class' => array('pager-next'), 'data' => $li_next );
    }
  }

  /* Print generated pager */
  return '<h2 class="element-invisible">' . t('Pages') . '</h2>' . theme('item_list', array('items' => $items, 'title' => NULL, 'type' => 'ul', 'attributes' => array('class' => array('pager'))));
}
