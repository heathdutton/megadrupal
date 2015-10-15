<?php

/**
 * @file
 * API for lingwo_senses
 */

require_once DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry')) . '/includes/settings.inc';

class LingwoSensesSettings extends LingwoSettings {
  protected $base_name = 'lingwo_senses';

  private $defaults = array(
    'difference' => 'definition',
    'example' => 'single',
    'relationship_allowed_values' => '',
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

class LingwoSenses implements IteratorAggregate, Countable {
  /*
   *
   * Static properties/methods
   * =========================
   *
   */

  public static $settings;

  public static function getRelationshipOptions($language, $translate = FALSE, $option_name = NULL) {
    return _lingwo_allowed_values(self::$settings, 'relationship', $language, $translate, $option_name);
  }

  /*
   *
   * Instance properties/methods.
   * ===========================
   *
   */

  private $entry;
  private $node;
  private $senses;
  private $children;

  public function __construct($entry) {
    $this->entry = $entry;
    $this->node = &$entry->getNode();

    if (!isset($this->node->lingwo_senses)) {
      $this->node->lingwo_senses = array();
      if (!$this->entry->isTranslation()) {
        // create a single empty sense
        $this->addSense();
      }
    }
  }

  private function buildSenses() {
    if (is_null($this->senses)) {
      $this->senses = array();

      if ($this->entry->isTranslation()) {
        // build our internal map of id => sense
        $trans_map = array();
        foreach ($this->node->lingwo_senses as $trans_sense) {
          $trans_map[$trans_sense['id']] = $trans_sense;
        }

        // add all the senses with originals, in the order of the originals
        foreach ($this->entry->sourceNode->lingwo_senses as $sense) {
          $trans_sense = isset($trans_map[$sense['id']]) ? $trans_map[$sense['id']] : TRUE;
          $this->senses[$sense['id']] = new LingwoSense($this, $sense, $trans_sense);
          unset($trans_map[$sense['id']]);
        }

        // add all the remaining translations without originals
        foreach ($trans_map as $id => $trans_sense) {
          $this->senses[$id] = new LingwoSense($this, NULL, $trans_sense);
        }
      }
      else {
        foreach ($this->node->lingwo_senses as $sense) {
          $this->senses[$sense['id']] = new LingwoSense($this, $sense);
        }
      }
    }
    return $this->senses;
  }

  public function getIterator() {
    return new ArrayIterator($this->buildSenses());
  }

  public function count() {
    return count($this->buildSenses());
  }

  public function addSense($data = array()) {
    if (!$this->entry->isTranslation()) {
      array_push($this->node->lingwo_senses, array('id' => _lingwo_make_uuid(), 'data' => $data, 'new' => TRUE));
    }
  }

  public function getSense($id) {
    $this->buildSenses();
    return $this->senses[$id];
  }

  public function getChildren($id = NULL) {
    if (!$this->entry->isTranslation()) {
      return array();
    }

    if (is_null($this->children)) {
      // First pass: create a bunch of maps
      $same_as_forward = array();
      $same_as_reverse = array();
      foreach ($this->node->lingwo_senses as $sense) {
        if (!empty($sense['data']['same_as'])) {
          $same_as_forward[$sense['id']] = $sense['data']['same_as'];
          $same_as_reverse[$sense['data']['same_as']][] = $sense['id'];
        }
      }

      // Second pass: reduce any entries that point to something, that is also same_as
      // something else
      foreach (array_keys($same_as_reverse) as $pid) {
        while ($dest = $same_as_forward[$pid]) {
          $same_as_reverse[$dest] = array_merge((array) $same_as_reverse[$dest], $same_as_reverse[$pid]);
          // prevent these items from being used again (should also break recursion)
          unset($same_as_reverse[$pid]);
          unset($same_as_forward[$pid]);
          // go the next step deeper
          $pid = $dest;
        }
      }

      $this->children = $same_as_reverse;
    }

    // return the appropriate value
    if (is_null($id)) {
      return $this->children;
    }
    if (isset($this->children[$id])) {
      return $this->children[$id];
    }
    return array();
  }
}

class LingwoSense {
  private $parent;
  private $id;
  private $new;
  private $orig_sense;
  private $trans_sense;
  private $is_translation;
  public $remove;

  // cache veriables
  private $cached_example;
  private $cached_example_translation;

  private static $MAP_ORIG = array(
    'difference' => 'difference',
    'is' => 'is',
    'raw_example' => 'example',
  );
  private static $MAP_TRANS = array(
    'clue' => 'clue',
    'trans' => 'trans',
    'same_as' => 'same_as',
    'no_equivalent' => 'no_equivalent',
    'raw_example_translation' => 'example_translation',
  );
  private static $MAP_FUNC = array(
    'example' => '_getExample',
    'example_translation' => '_getExampleTranslation',
    'children' => '_getChildren',
  );

  public function __construct($parent, $orig_sense, $trans_sense = NULL) {
    $this->parent = $parent;
    $this->orig_sense = $orig_sense;
    $this->trans_sense = is_array($trans_sense) ? $trans_sense : NULL;
    $this->is_translation = (bool) $trans_sense;
    $this->id = !is_null($this->orig_sense) ? $this->orig_sense['id'] : $this->trans_sense['id'];
    $this->new = !$this->is_translation && isset($orig_sense['new']) && $orig_sense['new'];
    if ($this->is_translation) {
      $this->remove = isset($this->trans_sense['remove']) ? (bool)$this->trans_sense['remove'] : FALSE;
    }
    else {
      $this->remove = isset($this->orig_sense['remove']) ? (bool)$this->orig_sense['remove'] : FALSE;
    }
  }

  public function __get($name) {
    if ($name == 'id') {
      return $this->id;
    }
    if ($name == 'new') {
      return $this->new;
    }
    if (is_array($this->orig_sense) && isset(self::$MAP_ORIG[$name]) && ($x = self::$MAP_ORIG[$name])) {
      if (isset($this->orig_sense['data'][$x])) {
        return $this->orig_sense['data'][$x];
      }
    }
    if (is_array($this->trans_sense) && isset(self::$MAP_TRANS[$name]) && ($x = self::$MAP_TRANS[$name])) {
      if (isset($this->trans_sense['data'][$x])) {
        return $this->trans_sense['data'][$x];
      }
    }
    if (isset(self::$MAP_FUNC[$name]) && isset(self::$MAP_FUNC[$name]) && ($x = self::$MAP_FUNC[$name])) {
      return call_user_func(array($this, $x));
    }
  }

  public function __isset($name) {
    if ($name == 'id') {
      return TRUE;
    }
    if (!is_null($this->orig_sense) && isset(self::$MAP_ORIG[$name])) {
      return TRUE;
    }
    if (!is_null($this->trans_sense) && isset(self::$MAP_TRANS[$name])) {
      return TRUE;
    }
    if (isset(self::$MAP_FUNC[$name])) {
      return TRUE;
    }
  }

  private function _fmtExample($text) {
    $example = array();

    switch (LingwoSenses::$settings->example) {
      case 'single':
        $example = array(str_replace("\n", " ", $text));
        break;

      case 'multiple':
        $example = explode("\n", $text);
        break;
    }

    $example = array_filter($example);
    return $example;
  }

  private function _getExample() {
    if (is_null($this->cached_example)) {
      if (is_array($this->orig_sense) && isset($this->orig_sense['data']['example'])) {
        $this->cached_example = $this->_fmtExample($this->orig_sense['data']['example']);
      }
      else {
        $this->cached_example = array();
      }
    }
    return $this->cached_example;
  }

  private function _getExampleTranslation() {
    if (is_null($this->cached_example_translation)) {
      // example translations only make sense if we actually have an original!
      if (!is_null($this->orig_sense) && !is_null($this->trans_sense)) {
        $this->cached_example_translation = $this->_fmtExample(
          isset($this->trans_sense['data']['example_translation']) ? $this->trans_sense['data']['example_translation'] : ''
        );
      }
      else {
        $this->cached_example_translation = array();
      }
    }
    return $this->cached_example_translation;
  }

  private function _getChildren() {
    if (!is_null($this->trans_sense)) {
      return $this->parent->getChildren($this->id);
    }
    return array();
  }

  public function getIds() {
    return array_merge(array($this->id), $this->_getChildren());
  }

  public function isTranslation() {
    return $this->is_translation;
  }

  public function hasOriginal() {
    return !is_null($this->orig_sense);
  }

  public function hasTranslation() {
    return !is_null($this->trans_sense);
  }

  public function getTranslatedExample($full = FALSE) {
    $example = array_map(NULL, $this->_getExample(), $this->_getExampleTranslation());
    if ($full) {
      foreach ($this->_getChildren() as $cid) {
        $example = array_merge($example, $this->parent->getSense($cid)->getTranslatedExample());
      }
    }
    return $example;
  }
}

LingwoSenses::$settings = LingwoSensesSettings::get();

// Returns an array mapping a sense id to a list of children sense ids which are
// marked "same_as" the parent id
// TODO: should be made into a public API somehow
function _lingwo_senses_get_children($senses) {
  // First pass: create a bunch of maps
  $same_as_forward = array();
  $same_as_reverse = array();
  foreach ($senses as $sense) {
    if (!empty($sense['data']['same_as'])) {
      $same_as_forward[$sense['id']] = $sense['data']['same_as'];
      $same_as_reverse[$sense['data']['same_as']][] = $sense['id'];
    }
  }

  // Second pass: reduce any entries that point to something, that is also same_as
  // something else
  foreach (array_keys($same_as_reverse) as $pid) {
    while ($dest = $same_as_forward[$pid]) {
      $same_as_reverse[$dest] = array_merge((array) $same_as_reverse[$dest], $same_as_reverse[$pid]);
      // prevent these items from being used again (should also break recursion)
      unset($same_as_reverse[$pid]);
      unset($same_as_forward[$pid]);
      // go the next step deeper
      $pid = $dest;
    }
  }

  return $same_as_reverse;
}

