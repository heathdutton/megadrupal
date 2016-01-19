<?php
/**
 * Implements hook_rules_action_info
 */
function varnish_tag_invalidate_rules_action_info() {
  $actions = array();
  
  $actions['varnish_add_tag'] = array(
    'parameter' => array(
      'tags' => array(
        'label' => t('Tags'),
        'type' => 'text'
      ),
    ),
    'label' => t('Send tags'),
    'group' => t('Varnish tags'),
    'base' => 'varnish_tag_invalidate_add'
  );
  
  $actions['varnish_tag_invalidate'] = array(
    'parameter' => array(
      'tags' => array(
        'label' => t('Tags'),
        'type' => 'text'
      ),
    ),
    'label' => t('Clear tags'),
    'group' => t('Varnish tags'),
    'base' => 'varnish_tag_invalidate_tags'
  );
  
  return $actions;
}

/**
 * Sends purge 
 * 
 * @param array $tags
 */
function varnish_tag_invalidate_tags($tags) {
  module_load_include('inc', 'purge');
  $tags = array_map('trim', explode("\n", $tags));
  varnish_tag_invalidate_purge_tags($tags);
}


/**
 * Add appropriate tags to the current page
 * 
 * @param array $tags
 */
function varnish_tag_invalidate_add($tags) {
  varnish_tag_invalidate_add_tags($tags);
}