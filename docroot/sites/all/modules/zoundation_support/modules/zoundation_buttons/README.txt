Basic Usage


== Block content as a button

  $block = block_load('system', 'user-menu');
  $button = array(
    '#theme' => 'zoundation_buttons_split_content',
    '#title' => t('My Account'),
    '#button_attributes' => array('href' => '/user'),
    '#content' => drupal_render(_block_get_renderable_array(_block_render_blocks(array($block)))),
  );
  $output = drupal_render($button);
  print $output;

== Menu as a button

  $items = zoundation_support_menu_builder('main-menu', 1, 2);

  $button = array(
    '#theme' => 'zoundation_buttons_list',
    '#title' => t('Main menu'),
    '#items' => $items,
  );
  $output = drupal_render($button);
  print $output;


== Node as Button

  $node = node_load(1);

  $button = array(
    '#theme' => 'zoundation_buttons_split_content',
    '#title' => check_plain($node->title),
    '#button_attributes' => array('href' => node_uri($node)),
    '#content' => drupal_render(node_view($node)),
  );
  $content = drupal_render($button);
  print $content;


