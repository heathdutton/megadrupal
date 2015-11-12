<?php
/**
 * @file
 * Field Complete Progress - Provides blocks to display a progress bar of Field completion - themes.
 */

function theme_fc_progress_complete($vars) {
  return '[' . ($vars['complete'] ? t('complete') : t('progress')) . ']';
}

function theme_fc_progress_entity_name($vars) {
  $info = entity_get_info($vars['entity_type']);
  return '(' . ($vars['depth']==0 ? t('Top level') : $info['label']) . ')';
}