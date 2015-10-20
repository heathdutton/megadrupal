<?php

/**
 * @file
 * API for lingwo_pron
 */

require_once DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry')) . '/includes/settings.inc';

class LingwoPronSettings extends LingwoSettings {
  protected $base_name = 'lingwo_pron';

  private $defaults = array(
    'enabled_fields' => array('ipa', 'tag', 'audio'),
    'audio_file_extensions' => 'mp3 ogg',
    'audio_file_size_max' => '512K',
    'audio_file_path' => 'lingwo_pron',
    'max_values' => -1,
    'pron_tag_allowed_values' => '',
  );

  public function getDefault($name) {
    return $this->defaults[$name];
  }

  public function getNames() {
    return array_map(array($this, 'name'), array_keys($this->defaults));
  }

  public function isValid($name) {
    return array_key_exists($name, $this->defaults);
  }

  public function isFieldEnabled($name) {
    return in_array($name, $this->enabled_fields);
  }

  // singleton boilerplate
  private static $settings = NULL;
  public static function get() {
    if (is_null(self::$settings)) {
      self::$settings = new self();
    }
    return self::$settings;
  }
}

class LingwoPron implements IteratorAggregate, Countable {
  /*
   *
   * Static properties/methods
   * =========================
   *
   */

  public static $settings;

  public static function getTagOptions($language, $translate = FALSE, $option_name = NULL) {
    return _lingwo_allowed_values(self::$settings, 'pron_tag', $language, $translate, $option_name);
  }

  /*
   *
   * Instance properties/methods.
   * ===========================
   *
   */

  private $entry;
  private $node;

  public function __construct($entry) {
    $this->entry = $entry;
    $this->node = &$entry->getNode();
  }

  public function getItems() {
    return empty($this->node->lingwo_pron) ? array() : $this->node->lingwo_pron;
  }

  public function getIterator() {
    return new ArrayIterator($this->getItems());
  }

  public function count() {
    return empty($this->node->lingwo_pron) ? 0 : count($this->node->lingwo_pron);
  }
}

LingwoPron::$settings = LingwoPronSettings::get();

/*
 * Creates and returns our file path.
 */
function _lingwo_pron_file_path() {
  $dest = 'public://' . LingwoPron::$settings->audio_file_path;
  if (!file_prepare_directory($dest, FILE_CREATE_DIRECTORY)) {
    watchdog('lingwo', 'Unable to create directory %directory for pronunciation files', array('%directory' => $dest));
    form_set_error(implode('][', array('lingwo_pron', $delta, 'audio')), t('The file could not be uploaded.'));
    return NULL;
  }
  return $dest;
}

