<?php
/**
 * @file
 * Contextual links module integration.
 */

/**
 * Implements hook_config_builder_token_info_alter() on behalf of
 * contextual.module.
 */
function contextual_config_builder_token_info_alter(&$info) {
  $info['tokens']['config-variable']['contextual'] = array(
    'name' => 'Contextually linked variable',
    'description' => t('Value with contextual link to the default configuration page.'),
  );

  $info['tokens']['config-variable']['contextual:?'] = array(
    'name' => 'Contextually linked variable',
    'description' => t('Value with contextual link to the provided configuration page.'),
  );
}

/**
 * Implements hook_config_builder_tokens_alter() on behalf of contextual.module.
 */
function contextual_config_builder_tokens_alter(&$replacements, $context) {
  if ('config-variable' == $context['type']) {
    foreach ($context['tokens'] as $name => $original) {
      $token = explode(':', $name);
      $variable_data = $context['data']['config-variable'];

      switch ($token[1]) {
        case 'contextual':
          if (is_null($variable_data) && isset($token[2])) {
            $index = config_builder_crud_index_load($token[0], $token[2]);
            $variable_data = $index->default_value;
          }
          else {
            $index = config_builder_crud_index_load($token[0]);
          }
          if ($index !== FALSE) {
            $config = config_builder_crud_load($index->config);
            if ($config !== FALSE) {
              // Build contextually linked element.
              $element = array(
                'markup' => array(
                  '#markup' => $variable_data,
                ),
                'contextual_links' => array(
                  '#type' => 'contextual_links',
                  '#contextual_links' => array(
                    'config_builder' => array($config->path, array()),
                  ),
                  '#element' => array(
                    '#markup' => $variable_data,
                  ),
                ),
                '#prefix' => '<span class="contextual-links-region">',
                '#suffix' => '</span>',
              );
              $replacements[$original] = render($element);
            }
          }
          break;
      }
    }
  }
}

/**
 * Implements hook_menu_contextual_links_alter().
 */
function config_builder_menu_contextual_links_alter(&$links, $router_item, $root_path) {
  // Show configuration page.
  if ('drupal_get_form' == $router_item['page_callback'] && 'config_builder_page_callback' == $router_item['page_arguments'][0]) {
    $links['config_builder-view'] = array_merge(
      $router_item,
      array(
        'title' => 'Edit value',
      )
    );
  }
}
