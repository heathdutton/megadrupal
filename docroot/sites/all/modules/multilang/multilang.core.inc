<?php
/**
 * @file
 * Multilang module core functions.
 */

/* --------------------------------------------------------------- BASIC DATA */
global $language, $base_path, $_multilang_langs;

/* Build languages lists:
  - $_multilang_langs: array(code => name) (also used in multilang.module)
  - $_multilang_lang_set: array([code]), to build MULTILANG_LANG_SET regex.
*/
$default_language = language_default();
// Place default first.
$_multilang_langs[$default_language->language] = $default_language->native;
foreach (language_list() as $lang) {
  if ($lang->language <> $default_language->language) {
    $_multilang_langs[$lang->language] = $lang->native;
  }
  $_multilang_lang_set[] = '\[' . $lang->language . '\]';
}

define('MULTILANG_LANG_SET', implode('|', $_multilang_lang_set));
define('MULTILANG_CUR_LANG', $language->language);
define('MULTILANG_DEF_LANG', language_default()->language);
define('MULTILANG_DEF_LEN', strlen(MULTILANG_DEF_LANG));
define('MULTILANG_PATH', $base_path);
define('MULTILANG_SP', '(?>&nbsp;|\s|\xc2\xa0)*');
define('MULTILANG_BR', '(?><\/p>' . MULTILANG_SP . '<p>|<br ?\/?>)*');
define('MULTILANG_DIRTY',
  '(?>'. MULTILANG_SP . MULTILANG_BR . MULTILANG_SP . ')*');
define('MULTILANG_HIGHLIGHT', 'multilang-highlight');
define('MULTILANG_HIGHLIGHT_BEG', '[multilang-highlight]');
define('MULTILANG_HIGHLIGHT_END', '[/multilang-highlight]');
define('MULTILANG_CUSTOM', '[multilang-custom]');

module_load_include('inc', 'multilang', 'multilang.conf');
define('MULTILANG_FALLBACK_KEEPEMPTY',
  (_multilang_conf(MULTILANG_FALLBACK_EMPTY) == 'empty'));
define('MULTILANG_FALLBACK_CUSTOM',
  (_multilang_conf(MULTILANG_FALLBACK_OPTION) == 'message'));
/* --------------------------------------------------------------- MAIN FUNCS */
/**
 * Looks for "multi" segments and reduces each one to the current language part.
 *
 * @param string $text
 *   The piece of text to be processed.
 *
 * @param array $options (optional)
 *   Default:
 *    'use_curLang' => TRUE; (otherwise use defLang)
 *    'can_highlight' => TRUE;
 *    'process_links' => TRUE;
 *   NOTE: when the target language block is found but is empty, AND if the
 *   configuration option MULTILANG_FALLBACK_EMPTY is set to "empty", then
 *   does NOT fallback and returns the empty text.
 *
 * @return string
 *   The resulting content of given $text after processing.
 */
function _multilang_process($text, $options = NULL) {
  
  // Merge options with default options:
  $opts = [
    'use_curLang' => TRUE,
    'can_replace' => TRUE,
    'can_highlight' => FALSE,
    'process_links' => FALSE,
  ];
  if ($options) {
    foreach ($options as $name => $option) {
      $opts[$name] = $option;
    }
  }

  // Define required and fallback languages.
  global $_multilang_process;
  if ($opts['use_curLang']) {
    $_multilang_process['required'] = MULTILANG_CUR_LANG;
    $_multilang_process['fallback'] = MULTILANG_DEF_LANG;
  } else {
    $_multilang_process['required'] = MULTILANG_DEF_LANG;
    $_multilang_process['fallback'] = 'en';
  }
  $_multilang_process['can_replace'] = $opts['can_replace'];
  $_multilang_process['highlight'] = $opts['can_highlight'];
  
  // Normalize newlines (\n only).
  $text = str_replace(array("\r\n", "\r"), "\n", $text);
  
  /* Normalize "multi" tags into [multi]:
    . <multi> and </multi> (direct SPIP migration, or Multilang plugin)
    . &lt;multi&gt; and &lt;/multi&gt; (input SPIP syntax through CKEditor)
    . suppress any wrapper like "<p>[multi]</p>" */
  $text = preg_replace(
    '`(?:<p>)?\[(/?)multi\](?:</p>)?`s',
    '[$1multi]',
    $text);
  
  // Extract segments marked with the required language.
  $text = preg_replace_callback(
    '`\[multi\](.*?)\[\\\\?/multi\]`is',
    // ("\\\\?" above: to match "[\/multi]" in some Ajax requested parts)
    '_multilang_process_segment',
    $text);
    
  // Process links, if required.
  if ($opts['process_links']) {
    // Ensure to have the current language as URL prefix.
    $text = preg_replace_callback(
      // Look for href value.
      '`href\s*=\s*"([^"]*)"`is',
      '_multilang_process_link',
      $text);
  }
  
  // Return processed text.
  return $text;
}

/**
 * Repair texts where formatters have truncated some [multilang-...] flags.
 */
function _multilang_repair($text) {
  // Look for [multilang-highlight]...[/multilang-highlight].
  $offset = 0;
  while (true) {
    // Look for opening [multilang-highlight].
    $flag_beg_pos =
      _multilang_flag_pos(MULTILANG_HIGHLIGHT_BEG, $text, $flag_len, $offset);
    if ($flag_beg_pos === FALSE) {
      // Opening flag is absent.
      break;
    }
    if ($flag_len !== TRUE) {
      // Only partial opening flag found, merely suppress it.
      $text = substr($text, $flag_beg_pos);
      break;
    }
    // Entire opening flag found, look for closing one.
    $flag_end_pos =
      _multilang_flag_pos(MULTILANG_HIGHLIGHT_END, $text, $flag_len, $offset);
    if ($flag_len === TRUE) {
      // Entire closing flag, prepare to look at next occurrence.
      $offset = $flag_end_pos + strlen(MULTILANG_HIGHLIGHT_END);
      continue;
    }
    // Otherwise repair closing flag.
    $text = substr($text, 0, $flag_end_pos) . MULTILANG_HIGHLIGHT_END;
    break;
  }
  
  // Look for [multilang-custom].
  $offset = 0;
  while (true) {
    $flag_pos = 
      _multilang_flag_pos(MULTILANG_CUSTOM, $text, $flag_len, $offset);
    if ($flag_pos === FALSE) {
      // Flag absent.
      break;
    }
    if ($flag_len === TRUE) {
      // Entire flag, prepare to look at next occurrence.
      $offset = $flag_pos + strlen(MULTILANG_CUSTOM);
      continue;
    }
    // Otherwise repair flag.
    $text = substr($text, 0, $flag_pos) . MULTILANG_CUSTOM;
    break;
  }
  
  // Return repaired text.
  return $text;
}

/**
 * Hack Drupal includes/unicode.inc.
 *
 * (not invasive: if Multilang is unsinstalled, keeps working as usual)
 */
function _multilang_hack_core() {
  $unicode_script =
    dirname($_SERVER["SCRIPT_FILENAME"]) . '/includes/unicode.inc';
  // Keep a copy of the original script, then hack truncate_utf8() function.
  copy($unicode_script, $unicode_script . '.original');
  $contents = preg_replace(
    /* Modify the truncate_utf8($string, ...) function in order to have $string
      translated using _multilang_process before truncating.
    */
    '`(function truncate_utf8\((\$[^,]+),[^{]+{)`is',
    '$1
    if (function_exists("multilang_init")) {
    module_load_include("inc", "multilang", "multilang.core");
    $2 = _multilang_process($2, ["can_highlight" => FALSE]); //
    }',
    file_get_contents($unicode_script)
  );
  /* The hacked function will work even if Multilang is uninstalled. In the
  other hand, to recognize the function name is enough for the hack to be
  successfull, even if the variable-name of the 1st argument changes.
  */
  if ($contents === NULL ) {
    // Could not achieve hack, send an error message.
    module_load_include('inc', 'multilang', 'multilang.data');
    drupal_set_message(_multilang_install_error());
    return false;
  } else {
    // Update hacked script.
    file_put_contents($unicode_script, $contents);
    // Register current Drupal core version.
    module_load_include('inc', 'multilang', 'multilang.conf');
    variable_set(MULTILANG_DRUPAL_VERSION, VERSION);
    return true;
  }
}
/* ----------------------------------------------------------- CALLBACK FUNCS */
/**
 * Return a block marked with the required (or fallback) language.
 */
function _multilang_process_segment($matches) {
  $segment = $matches[1];
  global $_multilang_langs, $_multilang_process;
  
  // Look for a block in the required language.
  $required_lang = $_multilang_process['required'];
  $block = _multilang_extract_block($segment, $required_lang, $is_missing);
  
  // Return it if found, not empty and not only language native name.
  if (
    !$is_missing AND !empty($block)
  AND
    !_multilang_is_native($block, $required_lang)
  ) {
    return $block;
  }
  
  /* When language-block exists but is empty, leave it empty if specified
  by the MULTILANG_FALLBACK_EMPTY configuration option. */
  if (
    !$is_missing AND empty($block)
  AND
    MULTILANG_FALLBACK_KEEPEMPTY
  ) {
    return $block;
  }
  
  // From here, translation is missing.
  
  /* If specified by the MULTILANG_FALLBACK_OPTION configuration option, it must
  be replaced by a customized message, unless currently not allowed.*/
  if(
    MULTILANG_FALLBACK_CUSTOM
  AND
    $_multilang_process['can_replace']
  ) {
    // Set a special flag for JS to replace by MULTILANG_FALLBACK_MESSAGE.
    // This is not executed here, to avoid nesting _multilang_process().
    $block = '[multilang-custom]';
  } else {
  
    // Otherwise try with defined fallback language.
    $fallback_lang = $_multilang_process['fallback'];
    $block = _multilang_extract_block($segment, $fallback_lang, $is_missing);
    
    // If even fallback failed, ultimately look for 'en' if not already tried.
    if (
      (
        $is_missing OR empty($block)
      OR
        _multilang_is_native($block, $fallback_lang)
      )
    AND
      $fallback_lang <> 'en'
    ) {
      $fallback_lang = 'en';
      $block = _multilang_extract_block($segment, $fallback_lang, $is_missing);
    }
    
    // If still empty, replace block by "? ? ?".
    $block =
      (!empty($block) AND !_multilang_is_native($block, $fallback_lang)) ?
      $block : '? ? ?';
  }
  
  // In any case, highlight block if required.
  if ($_multilang_process['highlight']) {
    if (preg_match('`^<[^ >]+[^>]*>`', $block)) {
      /* Block begins with a tag: comes from CKEditor and is already identified
        as HTML element, so it may immediately wrapped in highlighter <div>.
      */
      $block = sprintf(
        '<div class="' .MULTILANG_HIGHLIGHT. '">%s</div>', $block);
    } else {
      /* Otherwise wrap block in special flags for JS to decide if it can be
        highlighted, depending on where it is located.
      */
      $block = sprintf(
        MULTILANG_HIGHLIGHT_BEG . '%s' . MULTILANG_HIGHLIGHT_END, $block);
    }
  }
  
  return $block;
}

/**
 * Enhance internal links.
 *
 * Use alias, control lang segment.
 */
function _multilang_process_link($matches) {
  $href = @$matches[1];
  if (!preg_match(
    '`^((https?|s?ftp|mailto|javascript):|' . MULTILANG_PATH . '|#)`i', $href)
  ) {
    /* Do nothing if complete or absolute url, or mailto, or simple anchor.
       Turn bare link into internal link, adding langcode of the default site
       language if not already present.
    */
    $href = MULTILANG_PATH .
      (substr($href, 0, MULTILANG_DEF_LEN + 1) == MULTILANG_DEF_LANG . '/' ?
        NULL
      :
        (MULTILANG_DEF_LANG . '/')
      ) .
      $href;
    // Replace "node/id" by its alias, if needed.
    $href = preg_replace_callback('`node/\d+`i', '_multilang_set_alias', $href);
  }
  return 'href="' . $href . '"';
}
/**
 * Return alias from given "node/%" path.
 */
function _multilang_set_alias($matches) {
  return drupal_get_path_alias($matches[0]);
}
/* ---------------------------------------------------------------- SUB FUNCS */
/**
 * Extract block from segment.
 */
function _multilang_extract_block($segment, $required_lang, &$is_missing) {
  
  // Look for a block in the required language.
  $is_missing = !preg_match(
    // Look for required language mark.
    '`\[' . $required_lang . '\]' .
    // Look for content, stripped from any kind of pollution.
    MULTILANG_DIRTY . '(.*?)' . MULTILANG_DIRTY .
    // Look for other language mark, or end.
    '(?:' . MULTILANG_LANG_SET . '|$)`is',
    $segment,
    $matches
  );
  
  // Turn response into empty if it only contains blank-like data.
  if (!$is_missing AND preg_match(
    '`^(\s|<p>|</p>|&nbsp;|\xc2\xa0)*$`i',
    // (c2a0 is the UTF8 NO-BREAK-SPACE code sometimes returned by CKEditor)
    $matches[1]
  )) {
    $matches[1] = '';
  }
  
  return @$matches[1];
  // ("@" above: because $matches[1] not set if lang-block not found)
}

/**
 * Look for flag in text, returns it position or FALSE if flag absent.
 *
 * $flag_len is set to TRUE if entire flag found, or to its partial length.
 */
function _multilang_flag_pos($flag, $text, &$flag_len, $offset = 0) {
  $flag_len = 0;
  $result = FALSE;
  for ($i = 1, $n = strlen($flag); $i < $n + 1; $i++) {
    $flag_pos = strpos($text, substr($flag, 0, $i), $offset);
    if ($flag_pos === FALSE) {
      // Partial flag found, or flag absent.
      break;
    }
    $result = $flag_pos;
    $flag_len = $i;
  }
  // Entire flag found.
  $flag_len = TRUE;
  return $result;
}

/**
 * Return a boolean which is TRUE if block is only native language name.
 */
function _multilang_is_native($block, $lang) {
  global $_multilang_langs;
  return preg_match(
    '`^(<p>)?...' . $_multilang_langs[$lang] . '...(</p>)?$`',
    html_entity_decode($block)
  );
}
/* ========================================================================== */
