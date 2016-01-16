<?php

function delta_preprocess_page(&$vars) {

  if(drupal_is_front_page()) {
    unset($vars['page']['content']['system_main']['default_message']);
  }

  if (module_exists('color')) {
    _color_page_alter($vars);
  }

  $slogan_text = $vars['site_slogan'];
  $site_name_text = $vars['site_name'];

  $gmass = array(
    "twitter",
    "facebook",
    "flickr",
    "linkedin",
    "youtube",
    "pinterest",
    "google",
    "dribbble",
    "vimeo",
    "instagram",
    "vk",
    "video_id",
    "show_hide_video",
    "show_hide_icon",
    "show_hide_copyright",
    "copyright_url",
    "copyright_developedby"
  );

  foreach($gmass as $value) {
    $vars[$value] = theme_get_setting($value, 'delta');
  }

}

function delta_links__system_main_menu($vars) {
    $pid = variable_get('menu_main_links_source', 'main-menu');
    $tree = menu_tree($pid);
    return drupal_render($tree);
}
function delta_links__system_secondary_menu(&$vars) {
    $pid = variable_get('menu_secondary_links_source', 'menu-secondary-menu');
    $tree = menu_tree($pid);
    return drupal_render($tree);
}
function delta_process_html(&$vars) {
  if (module_exists('color')) {
    _color_html_alter($vars);
  }

  $tmas = array(
    "b_decor",
    "m_decor",
    "f_decor",
    "b_decor_hover",
    "m_decor_hover",
    "f_decor_hover",
    "layout_pattern",
    "body_font",
    "main_menu_font",
    "body_links_font",
    "footer_links_font",
    "h1_font",
    "h2_font",
    "h3_font",
    "h4_font",
    "h5_font",
    "h6_font"
    );
   foreach($tmas as $value) {
    $vars[$value] = theme_get_setting($value, 'delta');
  }

}