<?php

/**
 * @file
 * API functions for dealing with entries
 */

require_once DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry')) . '/includes/settings.inc';

class LingwoEntrySettings extends LingwoSettings {
  protected $base_name = 'lingwo_entry';

  private $defaults = array(
    'lookup_path' => 'dictionary',
    'node_title_munging' => array(
      'language' => TRUE,
      'pos' => TRUE,
    ),
    'redirect_node' => TRUE,
    'content_type' => 'lingwo_entry',
    'pos_allowed_values' => "noun|Noun\nadjective|Adjective\nverb|Verb\nadverb|Adverb\nother|Other",
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

class LingwoEntry {
  /*
   *
   * Static properties/methods
   * =========================
   *
   */

  public static $settings;
  private static $MAP_NODE = array(
    'headword' => 'title',
    'pos' => 'pos',
    'nid' => 'nid',
    'language' => 'language',
    'targetLanguage' => 'language',
  );
  private static $MAP_FUNC = array(
    'node' => 'getNode',
    'sourceLanguage' => 'getSourceLanguage',
    'sourceNode' => 'getTranslationSource',
  );
  private static $MAP_EXTS = NULL;

  public static function isEntryNode($node) {
    return $node->type == self::$settings->content_type;
  }

  private static function getTranslationForNid($nid, $target_lang) {
    if (module_exists('translation')) {
      $trans = translation_node_get_translations($nid);
      if ($trans[$target_lang]) {
        return $trans[$target_lang]->nid;
      }
    }
  }

  public static function lingwo2nid($id) {
    list($source_lang, $headword) = explode('/', $id);
    list($source_lang, $target_lang) = explode('-', $source_lang);
    if (preg_match('/([^\(]+)\(([^\)]+)\)/', $headword, $matches)) {
      $headword = trim($matches[1]);
      $pos  = $matches[2];
    }

    $nid = self::key2nid($source_lang, $pos, $headword);

    if ($target_lang) {
      $nid = self::getTranslationForNid($nid, $target_lang);
    }

    return $nid;
  }

  public static function hash2nid($hash, $target_lang = NULL, $fallback = FALSE) {
    $res = db_query("SELECT nid FROM {lingwo_entry} WHERE entry_hash = :entry_hash", array(':entry_hash' => $hash));
    if ($obj = $res->fetchObject()) {
      $nid = $obj->nid;
      if (!is_null($target_lang)) {
        $tnid = self::getTranslationForNid($nid, $target_lang);
        $nid = empty($tnid) ? ($fallback ? $nid : NULL) : $tnid;
      }
      return $nid;
    }
  }

  public static function key2nid($language, $pos, $headword) {
    $res = db_query("SELECT nid FROM {lingwo_entry} WHERE language = ? AND pos = ? AND headword = ?", array($language, $pos, $headword));

    if ($obj = $res->fetchObject()) {
      return $obj->nid;
    }
    return NULL;
  }

  public static function searchHeadword($text, $options = array()) {
    $query = db_select('lingwo_entry', 'e')
      ->fields('e', array('nid', 'headword', 'pos', 'language'))
      ->condition('e.headword', $text);

    if (!empty($options['language'])) {
      $query->condition('e.language', $options['language']);
    }
    if (!empty($options['pos'])) {
      $query->condition('e.pos', $options['pos']);
    }

    $results = array();
    foreach ($query->execute() as $obj) {
      $obj->exact = TRUE;
      $results[] = $obj;
    }

    return $results;
  }

  public static function buildLingwoId($language, $headword, $pos = NULL) {
    if (is_array($language)) {
      $language = implode('-', $language);
    }
    if (!empty($pos)) {
      $headword .= '(' . $pos . ')';
    }
    return implode('/', array($language, $headword));
  }

  public static function buildPath($language, $headword, $pos = NULL) {
    if ($lookup_path = self::$settings->lookup_path) {
      return implode('/', array(
        $lookup_path,
        self::buildLingwoId($language, $headword, $pos),
      ));
    }
  }

  public static function search($text, $options = array()) {
    // first get all the headword matches
    $results = self::searchHeadword($text, $options);

    // then get all the fields matches
    if (module_exists('lingwo_fields')) {
      $ret = LingwoFields::searchForms($text, $options);
      $results = array_merge($results, $ret);
    }

    return $results;
  }

  public static function getPosOptions($language, $translate = FALSE, $option_name = NULL) {
    return _lingwo_allowed_values(self::$settings, 'pos', $language, $translate, $option_name);
  }

  /*
   *
   * Constructors
   * ============
   *
   */

  protected function __construct(&$node) {
    $this->node = &$node;
  }

  public static function fromNode(&$node) {
    if (!$node || !self::isEntryNode($node)) {
      return NULL;
    }
    if (is_a($node, 'LingwoEntry')) {
      return $node;
    }
    return new LingwoEntry($node);
  }

  public static function fromNid($nid) {
    if ($nid) {
      return self::fromNode(node_load($nid));
    }
  }

  public static function fromLingwoId($id) {
    return self::fromNid(self::lingwo2nid($id));
  }

  public static function fromKey($language, $pos, $headword) {
    return self::fromNid(self::key2nid($language, $pos, $headword));
  }

  public static function fromHash($hash, $target_lang = NULL, $fallback = FALSE) {
    return self::fromNid(self::hash2nid($hash, $target_lang, $fallback));
  }

  /*
   *
   * Instance properties/methods.
   * ===========================
   *
   */

  private $node;
  private $sourceNode;
  private $exts = array();

  public function &getNode() {
    return $this->node;
  }

  public function __get($name) {
    if (isset(self::$MAP_NODE[$name])) {
      $x = self::$MAP_NODE[$name];
      if (!empty($x)) {
        return $this->node->$x;
      }
    }

    if (isset(self::$MAP_FUNC[$name])) {
      $x = self::$MAP_FUNC[$name];
      if (!empty($x)) {
        return call_user_func(array($this, $x));
      }
    }

    if (is_null(self::$MAP_EXTS)) {
      self::$MAP_EXTS = module_invoke_all('lingwo_entry_properties');
    }
    if (isset(self::$MAP_EXTS[$name])) {
      $x = self::$MAP_EXTS[$name];
      if (!empty($x)) {
        if (!isset($this->exts[$name])) {
          if (class_exists($x['class'])) {
            $x = $x['class'];
            $this->exts[$name] = new $x($this);
          }
          elseif (is_callable($x['callable'])) {
            // we don't cache callables!  They might need to be calculated every time
            return call_user_func($x['callable'], $this);
          }
        }
        return $this->exts[$name];
      }
    }
  }

  public function isTranslation() {
    return _lingwo_is_translation($this->node);
  }

  public function getTranslationSource() {
    if (isset($this->sourceNode)) {
      return $this->sourceNode;
    }

    $node = $this->node;
    if (isset($node->translation_source)) {
      return $node->translation_source;
    }

    if ($this->isTranslation()) {
      $this->sourceNode = node_load($node->tnid);
      return $this->sourceNode;
    }

    return NULL;
  }

  public function getTranslation($target_lang) {
    if (!$this->isTranslation()) {
      return self::fromNid(self::getTranslationForNid($this->node->nid, $target_lang));
    }
  }

  public function getLanguages($for_humans = FALSE) {
    $node = $this->node;
    if ($source_node = $this->getTranslationSource()) {
      $res = array($source_node->language, $node->language);
    }
    else {
      $res = array($node->language);
    }

    if ($for_humans) {
      $languages = language_list();
      for ($i = 0; $i < count($res); $i++) {
        if (isset($languages[$res[$i]])) {
          // TODO: we need to translate the names to the current interface!
          $res[$i] = $languages[$res[$i]]->name;
        }
      }
    }

    return $res;
  }

  public function getSourceLanguage($for_humans = FALSE) {
    $languages = $this->getLanguages($for_humans);
    return $languages[0];
  }

  public function getPos($for_humans = FALSE) {
    $node = $this->node;
    if ($for_humans) {
      return self::getPosOptions($this->getSourceLanguage(), TRUE, $node->pos);
    }

    return $node->pos;
  }

  public function getLingwoId() {
    return self::buildLingwoId($this->getLanguages(), $this->node->title, $this->getPos());
  }

  public function getPath() {
    $node = $this->node;

    // if we can build a path from self::$settings->lookup_path
    if ($path = self::buildPath($this->getLanguages(), $this->node->title, $this->getPos())) {
      return $path;
    }

    return isset($node->alias) ? $node->alias : ('node/' . $node->nid);
  }

  // Gets the potentially munged title - already safe to inject into HTML.
  public function getTitleHtml($use_icons = TRUE) {
    $munging = LingwoEntry::$settings->node_title_munging;
    $title = check_plain($this->node->title);

    if ($munging['language']) {
      $languages_names = $this->getLanguages();
      $languages_human = array_map('check_plain', $this->getLanguages(TRUE));

      if ($use_icons && ($icons = _lingwo_entry_languageicons($languages_names))) {
        $lang_spec = implode(' &#8594; ', $icons);
      }
      else {
        $lang_spec = '[' . implode(' &#8594; ', $languages_human) . ']';
      }

      $title = $lang_spec . ' ' . $title;
    }

    if ($munging['pos']) {
      $title .= ' (' . check_plain($this->getPos(TRUE)) . ')';
    }

    return $title;
  }
}

LingwoEntry::$settings = LingwoEntrySettings::get();

/*
 *
 * Internal API functions.
 *
 * These are internal functions for use by all the lingwo_* modules.
 *
 */

// Multiple language helper code
require_once DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry')) . '/includes/language.inc';

// Miscellaneous helper code
require_once DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry')) . '/includes/misc.inc';

