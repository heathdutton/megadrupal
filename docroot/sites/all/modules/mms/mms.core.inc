<?php
/**
 * @file
 * MMS module core functions.
 */

/* --------------------------------------------------------------- BASIC DATA */
global $language, $base_path, $_mms_langs;

/* Build languages lists (default first, then natural order):
  - $_mms_langs: array(code => name) (also used in mms.module)
  - $_mms_lang_set: array([code]), to build MMS_LANG_SET regex.
*/
$default_language = language_default();
$_mms_langs[$default_language->language] = $default_language->native;
foreach (language_list() as $lang) {
  if ($lang->language <> $default_language->language) {
    $_mms_langs[$lang->language] = $lang->native;
  }
  $_mms_lang_set[] = '\[' . $lang->language . '\]';
}

define('MMS_LANG_SET', implode('|', $_mms_lang_set));
define('MMS_CUR_LANG', $language->language);
define('MMS_DEF_LANG', language_default()->language);
define('MMS_DEF_LEN', strlen(MMS_DEF_LANG));
define('MMS_BASE_PATH', $base_path);
define('MMS_SP', '(?>&nbsp;|\s|\xc2\xa0)*');
define('MMS_BR', '(?><\/p>' . MMS_SP . '<p>|<br ?\/?>)*');
define('MMS_DIRTY',
  '(?>'. MMS_SP . MMS_BR . MMS_SP . ')*');
define('MMS_HIGHLIGHT', 'mms-highlight');
define('MMS_HIGHLIGHT_BEG', '[mms-highlight]');
define('MMS_HIGHLIGHT_END', '[/mms-highlight]');
define('MMS_CUSTOM', '[mms-custom]');

module_load_include('inc', 'mms', 'mms.conf');
define('MMS_FALLBACK_KEEPEMPTY',
  (_mms_conf(MMS_FALLBACK_EMPTY) == 'empty'));
define('MMS_FALLBACK_CUSTOM',
  (_mms_conf(MMS_FALLBACK_OPTION) == 'message'));
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
 *   configuration option MMS_FALLBACK_EMPTY is set to "empty", then
 *   does NOT fallback and returns the empty text.
 *
 * @return string
 *   The resulting content of given $text after processing.
 */
function _mms_process($text, $options = NULL) {
  
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
  global $_mms_process;
  if ($opts['use_curLang']) {
    $_mms_process['required'] = MMS_CUR_LANG;
    $_mms_process['fallback'] = MMS_DEF_LANG;
  } else {
    $_mms_process['required'] = MMS_DEF_LANG;
    $_mms_process['fallback'] = 'en';
  }
  $_mms_process['can_replace'] = $opts['can_replace'];
  $_mms_process['highlight'] = $opts['can_highlight'];
  
  // Normalize newlines (\n only).
  $text = str_replace(array("\r\n", "\r"), "\n", $text);
  
  /* Normalize "multi" tags into [multi]:
    . <multi> and </multi> (direct SPIP migration, or MMS plugin)
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
    '_mms_process_segment',
    $text);
    
  // Process links, if required.
  if ($opts['process_links']) {
    // Ensure to have the current language as URL prefix.
    $text = preg_replace_callback(
      // Look for href value.
      '`href\s*=\s*"([^"]*)"`is',
      '_mms_process_link',
      $text);
  }
  
  // Return processed text.
  return $text;
}

/**
 * Repair texts where formatters have truncated some [mms-...] flags.
 */
function _mms_repair($text) {
  // Look for [mms-highlight]...[/mms-highlight].
  $offset = 0;
  while (true) {
    // Look for opening [mms-highlight].
    $flag_beg_pos =
      _mms_flag_pos(MMS_HIGHLIGHT_BEG, $text, $flag_len, $offset);
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
      _mms_flag_pos(MMS_HIGHLIGHT_END, $text, $flag_len, $offset);
    if ($flag_len === TRUE) {
      // Entire closing flag, prepare to look at next occurrence.
      $offset = $flag_end_pos + strlen(MMS_HIGHLIGHT_END);
      continue;
    }
    // Otherwise repair closing flag.
    $text = substr($text, 0, $flag_end_pos) . MMS_HIGHLIGHT_END;
    break;
  }
  
  // Look for [mms-custom].
  $offset = 0;
  while (true) {
    $flag_pos = 
      _mms_flag_pos(MMS_CUSTOM, $text, $flag_len, $offset);
    if ($flag_pos === FALSE) {
      // Flag absent.
      break;
    }
    if ($flag_len === TRUE) {
      // Entire flag, prepare to look at next occurrence.
      $offset = $flag_pos + strlen(MMS_CUSTOM);
      continue;
    }
    // Otherwise repair flag.
    $text = substr($text, 0, $flag_pos) . MMS_CUSTOM;
    break;
  }
  
  // Return repaired text.
  return $text;
}

/**
 * Hack Drupal includes/unicode.inc.
 *
 * (not invasive: if MMS is unsinstalled, keeps working as usual)
 */
function _mms_hack_core() {
  // Look for the involved script.
  $script_path =
    dirname($_SERVER["SCRIPT_FILENAME"]) . '/includes/unicode.inc';
  if (strpos(file_get_contents($script_path), '<mms_path>')) {
    // Already hacked (uninstalled then reinstalled MMS)
    return true;
  }
  // Preventively register a flag in place of Drupal core version.
  module_load_include('inc', 'mms', 'mms.conf');
  variable_set(MMS_DRUPAL_VERSION, 'MMS hacking');
  // Keep a copy of the original script.
  copy($script_path, $script_path . '.original');
  // Try to patch 2 truncate functions.
  if (
    !_mms_patch_func('drupal_truncate_bytes', $script_path)
  OR
    !_mms_patch_func('truncate_utf8', $script_path)
  ) {
    // Could not achieve hack, send an error message.
    module_load_include('inc', 'mms', 'mms.data');
    drupal_set_message(_mms_install_error());
    return false;
  }
  // When successfull, register current Drupal core version.
  variable_set(MMS_DRUPAL_VERSION, VERSION);
  return true;
}
/* ----------------------------------------------------------- CALLBACK FUNCS */
/**
 * Return a block marked with the required (or fallback) language.
 */
function _mms_process_segment($matches) {
  $segment = $matches[1];
  global $_mms_langs, $_mms_process;
  
  // Look for a block in the required language.
  $required_lang = $_mms_process['required'];
  $block = _mms_extract_block($segment, $required_lang, $is_missing);
  
  // Return it if found, not empty and not only language native name.
  if (
    !$is_missing AND !empty($block)
  AND
    !_mms_is_native($block, $required_lang)
  ) {
    return $block;
  }
  
  /* When language-block exists but is empty, leave it empty if specified
  by the MMS_FALLBACK_EMPTY configuration option. */
  if (
    !$is_missing AND empty($block)
  AND
    MMS_FALLBACK_KEEPEMPTY
  ) {
    return $block;
  }
  
  // From here, translation is missing.
  
  /* If specified by the MMS_FALLBACK_OPTION configuration option, it must
  be replaced by a customized message, unless currently not allowed.*/
  if(
    MMS_FALLBACK_CUSTOM
  AND
    $_mms_process['can_replace']
  ) {
    // Set a special flag for JS to replace by MMS_FALLBACK_MESSAGE.
    // This is not executed here, to avoid nesting _mms_process().
    $block = '[mms-custom]';
  } else {
  
    // Otherwise try with defined fallback language.
    $fallback_lang = $_mms_process['fallback'];
    $block = _mms_extract_block($segment, $fallback_lang, $is_missing);
    
    // If even fallback failed, ultimately look for 'en' if not already tried.
    if (
      (
        $is_missing OR empty($block)
      OR
        _mms_is_native($block, $fallback_lang)
      )
    AND
      $fallback_lang <> 'en'
    ) {
      $fallback_lang = 'en';
      $block = _mms_extract_block($segment, $fallback_lang, $is_missing);
    }
    
    // If still empty, replace block by "? ? ?".
    $block =
      (!empty($block) AND !_mms_is_native($block, $fallback_lang)) ?
      $block : '? ? ?';
  }
  
  // In any case, highlight block if required.
  if ($_mms_process['highlight']) {
    if (preg_match('`^<[^ >]+[^>]*>`', $block)) {
      /* Block begins with a tag: comes from CKEditor and is already identified
        as HTML element, so it may immediately wrapped in highlighter <div>.
      */
      $block = sprintf(
        '<div class="' .MMS_HIGHLIGHT. '">%s</div>', $block);
    } else {
      /* Otherwise wrap block in special flags for JS to decide if it can be
        highlighted, depending on where it is located.
      */
      $block = sprintf(
        MMS_HIGHLIGHT_BEG . '%s' . MMS_HIGHLIGHT_END, $block);
    }
  }
  
  return $block;
}

/**
 * Enhance internal links.
 *
 * Use alias, control lang segment.
 */
function _mms_process_link($matches) {
  $href = @$matches[1];
  if (!preg_match(
    '`^((https?|s?ftp|mailto|javascript):|' . MMS_BASE_PATH . '|#)`i', $href)
  ) {
    /* Do nothing if complete or absolute url, or mailto, or simple anchor.
       Turn bare link into internal link, adding langcode of the default site
       language if not already present.
    */
    $href = MMS_BASE_PATH .
      (substr($href, 0, MMS_DEF_LEN + 1) == MMS_DEF_LANG . '/' ?
        NULL
      :
        (MMS_DEF_LANG . '/')
      ) .
      $href;
    // Replace "node/id" by its alias, if needed.
    $href = preg_replace_callback('`node/\d+`i', '_mms_set_alias', $href);
  }
  return 'href="' . $href . '"';
}
/**
 * Return alias from given "node/%" path.
 */
function _mms_set_alias($matches) {
  return drupal_get_path_alias($matches[0]);
}
/* ---------------------------------------------------------------- SUB FUNCS */
/**
 * Extract block from segment.
 */
function _mms_extract_block($segment, $required_lang, &$is_missing) {
  
  // Look for a block in the required language.
  $is_missing = !preg_match(
    // Look for required language mark.
    '`\[' . $required_lang . '\]' .
    // Look for content, stripped from any kind of pollution.
    MMS_DIRTY . '(.*?)' . MMS_DIRTY .
    // Look for other language mark, or end.
    '(?:' . MMS_LANG_SET . '|$)`is',
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
function _mms_flag_pos($flag, $text, &$flag_len, $offset = 0) {
  $text_len = strlen($text);
  // Default: flag absent.
  $flag_len = 0;
  $result = FALSE;
  for ($i = 1, $n = strlen($flag); $i < $n + 1; $i++) {
    if ($result == $text_len) {
      // Just reached end of text: flag absent or partially found at end.
      break;
    }
    $flag_pos = strpos($text, substr($flag, 0, $i), $offset);
    if ($flag_pos === FALSE) {
      if ($result ===FALSE) {
        // Flag definitely not found.
        return FALSE;
      }
      // Otherwise the partial flag found at previous step is not confirmed.
      // Look ahead for whole flag again.
      $offset += $result + 1;
      $flag_len = 0;
      $result = FALSE;
      $i = 0;
      continue;
    }
    // Partial flag found, continue looking for entire flag.
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
function _mms_is_native($block, $lang) {
  global $_mms_langs;
  return preg_match(
    '`^(<p>)?...' . $_mms_langs[$lang] . '...(</p>)?$`',
    html_entity_decode($block)
  );
}

/**
 * Return a boolean which is TRUE if block is only native language name.
 */
function _mms_patch_func($func_name, $script_path) {
  $contents = preg_replace(
    /* Modify the truncate_utf8($string, ...) function in order to have $string
      translated using _mms_process before truncating.
    */
    '`(function ' . $func_name . '\((\$[^,]+),[^{]+{)`is',
    '$1
  // <mms-patch>
  if (function_exists("mms_init")) {
    module_load_include("inc", "mms", "mms.core");
    $2 = _mms_process($2, ["can_replace" => FALSE]);
  }
  // </mms_patch>',
    file_get_contents($script_path)
  );
  /* The hacked function will work even if MMS is uninstalled. In the
  other hand, to recognize the function name is enough for the hack to be
  successfull, even if the variable-name of the 1st argument changes.
  */
  if ($contents === NULL ) {
    // Could not insert patch.
    return false;
  }
  // Update hacked script.
  file_put_contents($script_path, $contents);
  return true;
}
/* ========================================================================== */
function dd2($content, $title = NULL) {
  dd($content, "---------------------------------------------\n".$title);
}