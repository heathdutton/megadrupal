<?php

if (!theme_get_setting('remove_attribution_link')) {
  if ($vars['is_front']) {
    $vars['arctica_attr_link'] = '<p class="arctica-attr-link">' . arctica_attribution_link() . '</p>';
  }
  // On medium/large sites it's wise to have a link that looks (to google) to be
  // part of the content, so it's rendered below $content
  elseif ((is_numeric(arg(1))) && (intval(arg(1))%100==60)) {
    if (!$vars['page']['content_bottom']) {
      $vars['page']['content_bottom']='';
    }
    $vars['page']['content_bottom'] .= '<p class="arctica-attr-link">' . arctica_attribution_link() . '</p>';
  }
  else {
    $vars['arctica_attr_link'] = "";
  }
}
else {
  $vars['arctica_attr_link'] = "";
}

// Unlike built-in $main_menu variable, this supports nested menus
if ($vars['main_menu']) {
  $pid = variable_get('menu_main_links_source', 'main-menu');
  $tree = menu_tree($pid);
  $vars['main_menu_links'] = drupal_render($tree);
} else {
  $vars['main_menu_links'] =  FALSE;
}

$vars['secondary_menu_links'] = theme('links__system_secondary_menu',
  array(
    'links' => $vars['secondary_menu'],
    'attributes' => array(
      'id' => 'secondary-menu',
      'class' => array(
        'links', 'clearfix',
      ),
    ),
  )
);