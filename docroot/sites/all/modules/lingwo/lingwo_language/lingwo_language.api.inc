<?php

/**
 * @file
 * API functions for dealing with language definitions
 */

require_once DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry')) . '/includes/settings.inc';

class LingwoLanguageSettings extends LingwoSettings {
  protected $base_name = 'lingwo_language';

  private $defaults = array(
    'content_type' => 'lingwo_language',
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

  // singleton boilerplate
  private static $settings = NULL;
  public static function get() {
    if (is_null(self::$settings)) {
      self::$settings = new self();
    }
    return self::$settings;
  }
}

class LingwoLanguage {
  public static $settings;

  private $node;

  private function __construct($node) {
    $this->node = $node;
  }

  public static function fromNode($node) {
    if ($node->type == self::$settings->content_type) {
      return new LingwoLanguage($node);
    }
  }

  public static function fromNid($nid) {
    return self::fromNode(node_load($nid));
  }

  public static function fromCode($code) {
    $res = db_query("SELECT ll.nid FROM {lingwo_language} ll WHERE ll.language = :language", array(':language' => $code));
    if ($obj = $res->fetch()) {
      return self::fromNid($obj->nid);
    }
  }

  public function getNode() {
    return $this->node;
  }

  public function getValue($name, $default = NULL) {
    if (isset($this->node->lingwo_language[$name])) {
      return $this->node->lingwo_language[$name];
    }
    return $default;
  }

  public static function getValueQuick($code, $name) {
    $res = db_query("SELECT llr.value FROM {lingwo_language_revisions} llr JOIN {node} n ON llr.vid = n.vid JOIN {lingwo_language} ll ON ll.nid = n.nid WHERE ll.language = :language AND llr.name = :name", array(':language' => $code, ':name' => $name));
    if ($obj = $res->fetch()) {
      return unserialize($obj->value);
    }
  }
}

LingwoLanguage::$settings = LingwoLanguageSettings::get();

