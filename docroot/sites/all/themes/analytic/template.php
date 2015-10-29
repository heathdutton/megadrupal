<?php
function analytic_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];
  $link['localized_options']['html'] = TRUE;

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="element-invisible">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }    
    $link_text = t('!local-task-title !active', array('!local-task-title' => $link['title'], '!active' => $active));    
  }
  
  $link_text = '<span class="tab">' . $link_text . '</span>';
 
  return '<li' . (!empty($variables['element']['#active']) ? ' class="active"' : '') . '>' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}


function analytic_process_html(&$vars) {  
  if (empty($vars['page_bottom'])) {
    $vars['page_bottom'] = '';
  }
  $vars['page_bottom'] .= '<span class="developer"><strong><a href="http://pixeljets.com" title="Go to pixeljets.com">Drupal theme</a></strong> by        <a href="http://pixeljets.com" title="Go to pixeljets.com">pixeljets.com</a> <span class="version">D7 ver.1.1</span>
</span>';
}