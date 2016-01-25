<?php
/** 
 * Logo, Site Name, Site Slogan
 * - override om_identity
 */
function om_idream_identity($vars) {
  if (!empty($vars['logo']) || !empty($vars['site_name']) || !empty($vars['site_slogan'])) { 
    $out = '<div id="logo-title">';
    if (!empty($vars['logo'])) { 
      $out .= '<a href="' . $vars['front_page'] . '" title="' . t('Home') . '" rel="home" id="logo">';
      $out .= '<img src="' . $vars['logo'] . '" alt="' . t('Home') . '" />';
      $out .= '</a>';
    }
    if (!empty($vars['site_name']) || !empty($vars['site_slogan'])) { 
      $out .= '<div id="name-and-slogan">';
      if (!empty($vars['site_name'])) {
        $out .= '<h1 id="site-name">';
        $out .= '<a href="' . $vars['front_page'] . '" title="' . t('Home') . '" rel="home">' . $vars['site_name'] . '</a>';
        $out .= '</h1>';
      }
      if (!empty($vars['site_slogan'])) {
        $out .= '<div id="site-slogan">' . $vars['site_slogan'] . '</div>';
      }
      $out .= '</div> <!-- /#name-and-slogan -->';
    }    
    $out .= '</div> <!-- /#logo-title -->';
    return $out;
  }
}
