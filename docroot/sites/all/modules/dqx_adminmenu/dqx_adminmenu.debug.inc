<?php


function dqx_adminmenu_debug_page() {
  if (!module_exists('devel')) {
    return <<<EOT
      <p>Please install and enable <a href="http://drupal.org/project/devel">Devel</a> to see anything on this debug page.</p>
EOT;
  }
  else {
    module_load_include('inc', 'dqx_adminmenu');
    drupal_add_css(drupal_get_path('module', 'dqx_adminmenu') .'/dqx_adminmenu.debug.css');
    $html = _dqx_adminmenu_tree_html();
    return <<<EOT
      <p>The menu structure revealed, and <i>not</i> from the cache.</p>
      <p>Use this page if you want to debug the dqx_adminmenu module with dpm() and friends.</p>
      <div id="dqx_adminmenu-debug">$html</div>
EOT;
  }
}



