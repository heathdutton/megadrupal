<?php
namespace Drupal\go1_base\Config;

class Resolver implements ResolverInterface {
  /**
   * @var Config
   */
  private $config;

  public function setConfig($config) {
    $this->config = $config;
  }

  /**
   * Get path to configuration file.
   *
   * @return string
   */
  public function getPath() {
    if ($path = $this->getOverridePath()) {
      return $path;
    }
    return $this->getOriginalPath();
  }

  /**
   * Origininal path.
   *
   * @return string|null
   */
  public function getOriginalPath() {
    $config_id = $this->config->getId();

    $path = DRUPAL_ROOT . '/' . conf_path();
    if ($_path = drupal_get_path('module', $this->config->getModule())) {
      $config_id = trim($config_id, '/');
      $config_id = empty($config_id) ? $this->config->getModule() : $config_id;
      $path = DRUPAL_ROOT . '/' . $_path;
    }
    $config_id = trim(str_replace('.', '/', $config_id), '/');
    $path .= '/config/' . $config_id . '.yml';

    if (is_file($path)) {
      return $path;
    }
  }

  /**
   * Get overriden path.
   *
   * @return string
   */
  public function getOverridePath($check_exists = TRUE) {
    $return = variable_get('file_private_path');
    $return = dirname($return);
    $return .= '/config';
    $return .= '/' . $this->config->getModule();
    $return .= '/' . $this->config->getId();
    $return .= '.yml';
    $return = DRUPAL_ROOT . '/' . $return;
    if (!$check_exists) {
      return $return;
    }

    if (is_file($return)) {
      return $return;
    }
  }

  /**
   * Fetch config data.
   *
   * @return mixed
   */
  public function fetchData() {
    if ($path = $this->getPath()) {
      return $this->fetchFile($path);
    }
  }

  /**
   * @param string $path
   */
  private function fetchFile($path) {
    if (!$return = yaml_parse_file($path)) {
      throw new \Exception('Can not parse file: ' . $path);
    }

    if (!empty($return['imports'])) {
      return $this->resolveDataImports($return);
    }

    return $return;
  }

  private function resolveDataImportResources($imports) {
    foreach ($imports as $i => $import) {
      if (FALSE !== strpos($import['resource'], '*')) {
        unset($imports[$i]);
        $resource = DRUPAL_ROOT . '/' . drupal_get_path('module', $this->config->getModule()) . '/config/' . $import['resource'];
        $dir = dirname($resource);
        $name = basename($resource);
        $pattern = str_replace('*', '/.*/', $name);

        $files = file_scan_directory($dir, $pattern);
        if (!empty($files)) {
          foreach ($files as $file) {
            $imports[] = $file->uri;
          }
        }
      }
      else {
        $imports[$i] = DRUPAL_ROOT . '/' . drupal_get_path('module', $this->config->getModule()) . '/config/' . $import['resource'];
      }
    }
    return $imports;
  }

  private function resolveDataImports($return) {
    $_return = array();
    foreach ($this->resolveDataImportResources($return['imports']) as $i => $resource) {
      $data = $this->fetchFile($resource);
      foreach ($data as $k => $v) {
        $_return[$k] = !isset($_return[$k]) ? $v : array_merge($_return[$k], $v);
      }
    }

    unset($return['imports']);

    foreach ($return as $k => $v) {
      $_return[$k] = !isset($_return[$k]) ? $v : array_merge($_return[$k], $v);
    }

    return $_return;
  }

  public function writeData($data) {
    if ($path = $this->getOverridePath(FALSE)) {
      $data = yaml_emit($data);
      @mkdir(dirname($path), 0777, TRUE);
      return file_put_contents($path, $data);
    }
    throw new \Exception('Configuration directory is not writable');
  }
}
