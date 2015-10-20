<?php
/**
 * @file
 * File for HandlebarTemplate class.
 */

/**
 * Handlers functionality for handlebar template files.
 */
class HandlebarTemplate {
  /**
   * Prints the content of the template file.
   */
  private $content;
   
  /**
   * Template file name, including extension.
   */
  private $template;

  /**
   * Full file path and file name.
   */
  private $file;

  /**
   * File path relative to Drupal root.
   */
  private $path;

  /**
   * Calculated value from filename without .tpl.hbr.
   */
  private $id;

  /**
   * Array of data from arguments.
   */
  private $info;

  public function __construct($args = array()) {
    $this->info = (Object) $args;
  }

  /**
   * Check to see the file exists.
   */
  public function fileCheck() {
    if (!file_exists($this->file())) {
      drupal_set_message(t('File !file does not exist', array('!file' => $this->file())), 'error');
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Load Handlebars core functionality.
   */
  protected function load() {
    static $loaded;
    if (!$loaded) {
      if (!drupal_add_library('handlebars', 'drupal')) { 
        drupal_set_message(t('Handlebars library not properly configured / installed.'), 'error');
      }
      $loaded = TRUE;
    }
  }

  /**
   * Get statically cached template file name with extension.
   */
  protected function template() {
    if (!$this->template) {
      $this->template = (strpos($this->info->template, HANDLEBARS_EXT) !== FALSE)
        ? $this->info->template
        : $this->info->template . '.tpl.' . HANDLEBARS_EXT;
    }
    return $this->template;
  }

  /**
   * Calculate id from filename.
   */
  protected function id() {
    if (!$this->id) {
      $this->id = (strpos($this->info->template, HANDLEBARS_EXT) !== FALSE)
        ? str_replace('.tpl.' . HANDLEBARS_EXT, '', $this->info->template)
        : $this->info->template;
    }
    return $this->id;
  }

  /**
   * Full file path and filename.
   */
  protected function file() {
    if (!$this->file) {
      $this->file = DRUPAL_ROOT . '/' . $this->info->path . '/' . $this->info->template;
    }
    return $this->file;
  }

  /**
   * Get file content.
   */
  protected function content() {
    if (!$this->content) {
      $this->content = ($this->fileCheck()) ? file_get_contents($this->file()) : '';
    }
    return $this->content;
  }

  /**
   * Register helper functions needed for this template.
   */
  protected function registerHelpers() {
    $js = '';
    if (!empty($this->info->helpers)) {
      foreach ($this->info->helpers as $id => $helper) {
        $js .= "Handlebars.registerHelper(\"$id\", $helper);\r\n";
      }
      drupal_add_js($js, 'inline');
    }
  }

  /**
   * Main function: adds current template file to registry.
   */
  public function addTemplate(&$handlebars) {
    $this->load();
    if ($this->fileCheck()) {
      $handlebars[$this->id()] = $this;
    }
    $setting = array('handlebars' => array(
      'inventory' => array($this->id => $this->id),
    ));
    drupal_add_js($setting, 'setting');
    $this->registerHelpers();
  }

  /**
   * Displays template file with script tags.
   */
  public function render() {
    $attributes = array(
      'type' => 'text/x-handlebars-template',
      'id' => $this->id(),
    );

    $output = '<script ' . drupal_attributes($attributes) . '>';
    $output .= $this->content();
    $output .= '</script>';

    return $output;
  }
}
