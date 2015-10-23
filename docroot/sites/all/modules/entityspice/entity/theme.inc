<?php

/**
 * Returns HTML for a list of available node types for node creation.
 *
 * @param array $variables
 *   An associative array containing:
 *   - content: An array of content types.
 *
 * @ingroup themeable
 */
function theme_entityspice_entity_add_list($variables) {
  $content = $variables['content'];
  $etype = $variables['entity_type'];
  $info = _entityspice_getinfo($etype);
  $bundles_page = $info['parent admin path'];

  $output = '';

  if ($content) {
    $output = '<dl class="node-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $args = array(
      '@create-bundle' => url($bundles_page),
      '@etype' => $variables['entity_type'],
    );
    $output  = '<p>';
    $output .= t('You have not created any @etype yet. Go to the <a href="@create-bundle"> bundle administration page</a> to add a new @etype bundle.', $args);
    $output .= '</p>';
  }

  return $output;
}

/**
 * Theme function for entity list.
 */
function theme_entityspice_entity_list($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $etype = $variables['entity_type'];

  $render['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No @etype entities available.', array('@etype' => $etype)),
  );
  $render['pager'] = array('#theme' => 'pager');

  return render($render);
}

