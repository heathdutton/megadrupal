<?php

/**
 * @file
 * This export tries to export the translation node with the source node also,
 * meaning the source and target strings will be paired.
 * There is no guarantee that this export works, as translation node may have
 * a different html strucure than the orginal node.
 */

/**
 * Bilingual export.
 */
function _xliff_bilingual_export($translation_node, &$message) {
  if (!module_exists('translation')) {
    $message = t('Content translation module needs to be enabled.');
    return FALSE;
  }

  if (!translation_supported_type($translation_node->type)) {
    $message = t('This content type is not multilingual.');
    return FALSE;
  }

  if (!user_access('translate content')) {
    $message = t('Translate content permission is required.');
    return FALSE;
  }

  if (empty($translation_node->language)) {
    $message = t('The language of this content must be set before creating a translation.');
    return FALSE;
  }

  if (!$translation_node->tnid) {
    $message = t('Only translated nodes could be exported as bilingual export.');
    return FALSE;
  }

  if ($translation_node->nid == $translation_node->tnid) {
    $message = t('Only translated nodes could be exported as bilingual export.');
    return FALSE;
  }

  $source = xliff_export($translation_node->tnid);
  if (!$source) {
    $message = t('Error at source node export.');
    return FALSE;
  }
  $target = xliff_export($translation_node->nid);
  if (!$target) {
    $message = t('Error at translation node export.');
    return FALSE;
  }

  preg_match_all('/<trans-unit id="(?P<id>.+)">\n\s*<source>(?P<source>.+)<\/source>/', $source, $trans_unit_source);
  preg_match_all('/<trans-unit id="(?P<id>.+)">\n\s*<source>(?P<source>.+)<\/source>/', $target, $trans_unit_target);

  $diff = array_diff_key($trans_unit_source['id'], $trans_unit_target['id']);
  if (!empty($diff)) {
    $message = t('The html structure of source and translation nodes do not match.');
    return FALSE;
  }

  foreach ($trans_unit_source['id'] as $key => $trans_unit_id) {
    $text = $trans_unit_source['source'][$key];
    $pattern = '/(<trans-unit id="' . $trans_unit_id . '">\n\s*<source>)(?P<source>.+)(<\/source>)/';
    $replacement = '${1}' . $text . '$3';
    $target = preg_replace($pattern, $replacement, $target);
  }

  return $target;
}
