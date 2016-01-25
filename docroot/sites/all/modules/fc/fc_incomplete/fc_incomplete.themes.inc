<?php
/**
 * @file
 * Field Incomplete - Provides a block displaying of what's currently incomplete on an entity - themes.
 */

function theme_fc_incomplete_complete($vars) {
  return '[' . ($vars['complete'] ? t('complete') : t('incomplete')) . ']';
}

function theme_fc_incomplete_entity_name($vars) {
  $info = entity_get_info($vars['entity_type']);
  return '(' . ($vars['depth']==0 ? t('Top level') : $info['label']) . ')';
}