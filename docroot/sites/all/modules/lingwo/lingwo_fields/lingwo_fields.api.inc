<?php

/**
 * @file
 * API functions for dealing with entries
 */

require_once DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry')) . '/includes/settings.inc';

class LingwoFieldsSettings extends LingwoSettings {
  protected $base_name = 'lingwo_fields';

  private $defaults = array(
    'fields' => array(
      'javascript' => "// Generate alphabet for this language\nlang.alphabet = Language.generateAlphabet('abcdefghijklmnopqrstuvwxyz', ['ch','sh','th'], function (l) { return 'aeiou'.indexOf(l) == -1 ? ['consonant'] : ['vowel'] });\n\n// Here you can declare some functions for later use!\n\n",
    ),
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

class LingwoFields implements IteratorAggregate {
  /*
   *
   * Static properties/methods
   * =========================
   *
   */

  public static $settings;

  public static function searchForms($text, $options = array()) {
    $results = array();

    $query = db_select('lingwo_field_fields', 'f');
    $e = $query->join('lingwo_entry', 'e', 'f.nid = e.nid');
    $query
      ->fields('f', array('nid', 'form_name'))
      ->fields($e, array('headword', 'pos', 'language'))
      ->condition('f.form', $text);

    if (!empty($options['language'])) {
      $query->condition("$e.language", $options['language']);
    }
    if (!empty($options['pos'])) {
      $query->condition("$e.pos", $options['pos']);
    }

    $res = $query->execute();
    foreach ($res as $obj) {
      $obj->exact = TRUE;
      $results[] = $obj;
    }

    return $results;
  }

  // creates a special map of FAPI items for use by the themes
  public static function mapItems(&$items) {
    $map = array(
      ':classes' => array(),
      ':options' => array(),
      ':forms' => array(),
    );

    foreach (element_children($items) as $index) {
      // From the formatter them function we get $items[$index]['#item'] but if we just toss
      // $node->lingwo_fields in here, we get $items[$index] -- both modes are supported.
      $item_info = $items[$index]['#item'] ? $items[$index]['#item'] : $items[$index];
      $type = $item_info['type'];
      $name = $item_info['name'];

      // put into the basic flat map
      $map[$name] = &$items[$index];
      // and group by type
      $type .= ($type == 'class') ? 'es' : 's';
      $map[':' . $type][] = &$items[$index];
    }

    return $map;
  }

  // TODO: we need to do more about security here!  All the identifiers need to be verified as
  // JavaScript safe here or even better, before we get to this point.
  public static function generateJavascript($language_name, $language) {
    $data = $language->getValue(self::$settings->name('fields'), self::$settings->fields);

    $out = '';
    $out .= "define('lingwo/languages/$language_name', ['lingwo/Language'], function (Language) {\n";
    $out .= "var lang = Language.defineLanguage('$language_name');\n";
    $out .= $data['javascript'];

    if (!empty($data['pos'])) {
      foreach (array_keys($data['pos']) as $pos) {
        $out .= "lang.fields['" . _lingwo_fields_check_js($pos) . "'] = {\n";
        $first = TRUE;
        foreach ($data['pos'][$pos] as $name => $field) {
          if ($first) {
            $first = FALSE;
          }
          else {
            $out .= ",\n";
          }

          $out .= "'" . _lingwo_fields_check_js($name) . "': {\n";
          $out .= "type: '" . _lingwo_fields_check_js($field['type']) . "'";
          $out .= ",\nlabel: '" . _lingwo_fields_check_js($field['label']) . "'";

          // write options if necessary
          if ($field['type'] == 'option' and !empty($field['options'])) {
            $out .= ",\noptions: " . drupal_json_encode(_lingwo_name_value_list($field['options']));
          }

          // write out javascript functions
          foreach (array('automatic', 'show') as $field_name) {
            if (!empty($field[$field_name])) {
              $out .= ",\n$field_name: function (entry) {\n";
              $out .= $field[$field_name];
              $out .= "\n}";
            }
          }
          $out .= "\n}";
        }
        $out .= "\n};\n";
      }
    }
    $out .= "return lang;\n";
    $out .= "});\n\n";

    $path = "public://lingwo/languages";
    $uri = "$path/$language_name.js";
    file_prepare_directory($path, FILE_CREATE_DIRECTORY);
    file_unmanaged_save_data($out, $uri, FILE_EXISTS_REPLACE);
  }

  /*
   *
   * Instance properties/methods.
   * ===========================
   *
   */

  private $entry;
  private $node;

  function __construct($entry) {
    $this->entry = $entry;
    $this->node = &$entry->getNode();

    if (!isset($this->node->lingwo_fields)) {
      $this->node->lingwo_fields = array();
    }
  }

  public function getFields() {
    $this->clean();
    $items = $this->node->lingwo_fields;

    // get the definitions for this language/pos pair
    $field_definitions = _lingwo_language_value(self::$settings, 'fields', $this->node->language);
    $field_definitions = isset($field_definitions['pos'][$this->node->pos]) ?
      $field_definitions['pos'][$this->node->pos] : array();

    $fields = array();
    foreach ($field_definitions as $name => $field) {
      if (isset($items[$name])) {
        $item = $items[$name];
        unset($items[$name]);
      }
      else {
        // setup some defaults from the definition
        $item = array(
          'type' => $field['type'],
          'name' => $name,
          'value' => NULL,
          'automatic' => TRUE,
        );
      }

      $item['definition'] = $field;

      $fields[$name] = $item;
    }
    $fields += $items;

    return $fields;
  }

  public function clean() {
    _lingwo_fields_clean($this->node->lingwo_fields);
  }

  public function getIterator() {
    return new ArrayIterator($this->getFields());
  }

  public function getEntry() {
    return $this->entry;
  }

  public function exists($name) {
    return isset($this->node->lingwo_fields[$name]);
  }

  public function getType($name) {
    if (isset($this->node->lingwo_fields[$name])) {
      return $this->node->lingwo_fields[$name]['type'];
    }
  }

  public function getOne($name) {
    if (isset($this->node->lingwo_fields[$name])) {
      return $this->node->lingwo_fields[$name]['value'];
    }
  }

  public function getMany($name) {
    if (isset($this->node->lingwo_fields[$name])) {
      if (isset($this->node->lingwo_fields[$name]['value'])) {
        $values = array($this->node->lingwo_fields[$name]['value']);
      }
      else {
        $values = array();
      }
      if (isset($this->node->lingwo_fields[$name]['alt'])) {
        $values = array_merge($values, $this->node->lingwo_fields[$name]['alt']);
      }
      return $values;
    }
  }

  public function set($name, $type, $value, $automatic = FALSE) {
    if (!isset($this->node->lingwo_fields[$name])) {
      $field = array(
        'name' => $name,
      );
      $this->node->lingwo_fields[$name] = $field;
    }

    $field = &$this->node->lingwo_fields[$name];
    $field['type']      = $type;
    $field['value']     = $value;
    $field['automatic'] = $automatic;
  }

  public function addForm($name, $value, $automatic = FALSE) {
    if (is_null($name)) {
      $name = '_noname_';
      $automatic = FALSE;
    }
    if (isset($this->node->lingwo_fields[$name])) {
      $this->node->lingwo_fields[$name]['alt'][] = $value;
    }
    else {
      $this->node->lingwo_fields[$name] = array(
        'name' => $name,
        'type' => 'form',
        'automatic' => $automatic,
      );
      if ($name == '_noname_') {
        $this->node->lingwo_fields[$name]['alt'][] = $value;
      }
      else {
        $this->node->lingwo_fields[$name]['value'] = $value;
      }
    }
  }
}

LingwoFields::$settings = LingwoFieldsSettings::get();

function _lingwo_fields_check_js($s) {
  // prevent from breaking out of the single quoted string we use
  $s = str_replace("\\", "\\\\", $s);
  $s = str_replace("'",   "\\'",   $s);
  return $s;
}

