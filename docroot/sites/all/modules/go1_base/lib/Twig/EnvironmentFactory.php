<?php
namespace Drupal\go1_base\Twig;

class EnvironmentFactory {
  private static $twig;
  private static $loader;
  private $options;

  /**
   * Factory for @twig.core
   *
   * Return \Twig_Environment
   */
  public function getObject() {
    if (!self::$twig) {
      // Autoloading
      require_once go1_library('twig') . '/lib/Twig/Autoloader.php';
      \Twig_Autoloader::register();

      $this->options = array(
        'debug' => go1_debug(),
        'auto_reload' => go1_debug(),
        'autoescape' => FALSE,
        'cache' => variable_get('file_temporary_path', FALSE),
      );

      // Init the object
      self::$twig = new \Twig_Environment(NULL, $this->options);
      self::$twig->addExtension(new \Drupal\go1_base\Twig\Extension());
    }

    return self::$twig;
  }

  /**
   * Factory for @twig
   *
   * Return \Twig_Environment
   */
  public function getFileService($twig) {
    return clone $twig;
  }

  /**
   * Factory for @twig.string
   *
   * Return \Twig_Environment
   */
  public function getStringService($twig) {
    return clone $twig;
  }

  /**
   * Factory method for @twig.file_loader
   * @return \Twig_Loader_Filesystem
   */
  public function getFileLoader() {
    $root = DRUPAL_ROOT;

    return go1_cache('atwig:file_loader, + 1 year', function() use ($root) {
      $loader = new \Twig_Loader_Filesystem($root);

      foreach (array('go1_base' => 'go1_base') + go1_modules('go1_base') as $module) {
        $dir = $root . '/' . drupal_get_path('module', $module);
        if (is_dir($dir)) {
          $loader->addPath($dir, $module);
        }
      }

      return $loader;
    });
  }
}
