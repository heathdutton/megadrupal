<?php
namespace Drupal\go1_base\Cache\Warming\Warmer;

class ViewWarmer implements WarmerInterface {
  public function __construct() {
    views_include_handlers();
    module_load_include('inc', 'views', 'plugins/views_plugin_cache');
  }

  public function validateTag($tag) {
    return 0 === strpos($tag, 'view:') || 0 === strpos($tag, 'views:');
  }

  public function processTag($tag, $context = array()) {
    @list($module, $view_name, $display_id) = explode(':', $tag);

    if ($view = views_get_view($view_name)) {
      $display_id = $display_id ? $display_id : 'default';
      $display = isset($view->display[$display_id]) ? $view->display[$display_id] : $view->display['default'];
      $cache = new views_plugin_cache($view, $display);
      $cache->cache_flush();
    }
  }
}

if (class_exists('views_plugin_cache')) {
  class views_plugin_cache extends \views_plugin_cache {
    public function __construct($view, $display) {
      $this->view = $view;
      $this->display = $display;
      $this->set_default_options();
    }
  }
}
else {
  class views_plugin_cache {
    public function __construct($view, $display) {
      $this->view = $view;
      $this->display = $display;
      $this->set_default_options();
    }
  }
}
