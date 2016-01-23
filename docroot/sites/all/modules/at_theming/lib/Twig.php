<?php

namespace Drupal\at_theming;

class Twig {
  public static function getEnvironment() {
    static $twig;

    if (!$twig) {
      require_once DRUPAL_ROOT . '/sites/all/libraries/twig/lib/Twig/Autoloader.php';
      \Twig_Autoloader::register();

      $twig = new \Twig_Environment(NULL, self::getOptions());

      // Extension
      if (at_debug()) {
        $twig->addExtension(new \Twig_Extension_Debug());
      }

      // Filters
      foreach (self::getFilters() as $filter) {
        $twig->addFilter($filter);
      }

      // Functions
      $twig->addFunction(new \Twig_SimpleFunction('element_children', function($v) {
        return element_children($v);
      }));
    }

    $twig->setLoader(self::getFileLoader());

    return $twig;
  }

  protected static function getFileLoader() {
    static $loader;

    if (!$loader) {
      $loader = new \Twig_Loader_Filesystem(DRUPAL_ROOT);
      // Add @module shortcuts
      foreach (at_modules('at_theming') as $module_name) {
        $dir = DRUPAL_ROOT . '/' . drupal_get_path('module', $module_name) . '/templates';
        if (is_dir($dir)) {
          $loader->addPath($dir, $module_name);
        }
      }
    }

    return $loader;
  }

  protected static function getFilters() {
    $options['cache_id'] = 'at_theming:twig:filters';
    return at_cache($options, function(){
      return at_id(new \Drupal\at_theming\Twig\Filters())->get();
    });
  }

  protected static function getOptions() {
    $options['debug'] = at_debug();
    $options['auto_reload'] = at_debug();
    $options['autoescape'] = FALSE;
    $options['cache'] = variable_get('file_temporary_path', FALSE);
    return $options;
  }

  public static function render($template_file, $variables) {
    $twig = self::getEnvironment();
    return $twig->render($template_file, $variables);
  }
}
