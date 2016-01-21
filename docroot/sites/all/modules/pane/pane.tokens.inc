<?php

/**
 * Implements hook_token_info
 */
function pane_token_info() {
  $type = array(
    'name' => t('Panes'),
    'description' => t('Tokens for custom content panes.'),
    'needs-data' => 'pane'
  );
  
  $pane['name'] = array(
    'name' => t('Machine name'),
    'description' => t('The machine name of a pane')
  );
  
  return array(
    'types' => array(
      'pane' => $type
    ),
    'tokens' => array(
      'pane' => $pane
    )
  );
}

/**
 * Implements hook_tokens
 */
function pane_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  
  if ($type == 'pane' && ! empty($data['pane'])) {
    $pane = $data['pane'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'name' :
          $replacements[$original] = $pane->container;
          break;
      }
    }
  }
  
  return $replacements;
}
