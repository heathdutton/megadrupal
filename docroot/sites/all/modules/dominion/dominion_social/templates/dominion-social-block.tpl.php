<?php

$links = array();
foreach ($sites['sites'] as $site) {
  if (!empty($site->icon_uri)) {
    $icon = theme('image', array(
      'path' => $site->icon_uri,
      'alt' => $site->name,
    ));
    $links[] = l($icon, $site->href, array('html' => TRUE, 'attributes' => array('class' => $site->class)));
  }
  else {
    $links[] = l($site->name, $site->href, array('attributes' => array('class' => $site->class)));
  }
}

$variables['items'] = $links;
$variables['title'] = '';
$variables['type'] = 'ul';
$variables['attributes'] = array('class' => 'dominion-social');

print theme('item_list', $variables);
