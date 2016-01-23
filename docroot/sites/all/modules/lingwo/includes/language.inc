<?php

/**
 * @file
 * Helper code for dealing with multiple language support
 */

class _LingwoLanguageNull {
  public function hasValue($name) {
    return FALSE;
  }
  public function getValue($name, $default = NULL) {
    return $default;
  }
}

// Gets the form from hook_lingwo_language for the given module
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_language_form($func = NULL) {
  $lang = new _LingwoLanguageNull();
  $form = $func($lang);

  // if we are using lingwo_language, tell the user these are just defaults
  if (module_exists('lingwo_language')) {
    foreach (array_keys($form) as $key) {
      if (isset($form[$key]['#type']) && $form[$key]['#type'] != 'markup') {
        if (empty($form[$key]['#description'])) {
          $form[$key]['#description'] = '';
        }
        else {
          $form[$key]['#description'] .= ' ';
        }
        $form[$key]['#description'] .= '<strong>' . t('Configured per language in its definition.  These are the defaults for new language definitions.') . '</strong>';
      }
    }
  }

  foreach (array('submit', 'validate') as $type) {
    $callback = $func . '_' . $type;
    if (function_exists($callback)) {
      $form["#$type"][] = $callback;
    }
  }

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_language_value($settings, $name, $language) {
  $value = NULL;
  if (module_exists('lingwo_language') && !empty($language)) {
    if (is_string($language)) {
      $language = LingwoLanguage::fromCode($language);
    }
  }
  if (!empty($language) && is_a($language, 'LingwoLanguage')) {
    $value = $language->getValue($settings->name($name));
  }
  if (is_null($value)) {
    $value = $settings->__get($name);
  }
  return $value;
}

