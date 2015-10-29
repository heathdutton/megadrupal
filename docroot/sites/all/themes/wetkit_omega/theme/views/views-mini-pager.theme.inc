<?php
/**
 * @file
 * views-mini-pager.theme.inc
 */


/**
 * Override or insert variables into the theme links templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_omega_views_mini_pager($vars) {
  global $pager_page_array, $pager_total;

  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];

  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  if ($pager_total[$element] > 1) {

    $li_previous = theme('pager_previous',
      array(
      'text' => (isset($tags[1]) ? $tags[1] : t('‹‹')),
      'element' => $element,
      'interval' => 1,
      'parameters' => $parameters,
      'attributes' => array('class' => 'button button-small button-accent ui-link'),
    )
    );
    if (empty($li_previous)) {
      $li_previous = '<span class="button button-small button-attention button-disabled ui-disabled">&lt;&lt;</span>';
      $li_previous_class = array('pager-previous');
    } else {
      $li_previous_class = array('pager-previous');
    }

    $li_next = theme('pager_next',
      array(
      'text' => (isset($tags[3]) ? $tags[3] : t('››')),
      'element' => $element,
      'interval' => 1,
      'parameters' => $parameters,
      'attributes' => array('class' => 'button button-small button-accent ui-link'),
    )
    );

    if (empty($li_next)) {
      $li_next = '<span class="button button-small button-attention button-disabled ui-disabled">&gt;&gt;</span>';
      $li_next_class = array('pager-next');
    } else {
      $li_next_class = array('pager-next');
    }

    $items[] = array(
      'class' => $li_previous_class,
      'data' => $li_previous,
    );

    $li_current = '<span class="button button-small button-attention button-disabled ui-disabled">';
    $li_current .= t('@current of @max', array('@current' => $pager_current, '@max' => $pager_max));
    $li_current .= '</span>';
    $items[] = array(
      'class' => array('pager-current'),
      'data' =>  $li_current,
    );
    $items[] = array(
      'class' => $li_next_class,
      'data' => $li_next,
    );

    return '<div class="pager-container"><h2 class="wb-invisible">' . t('Pages') . '</h2>' .
      theme('item_list',
        array(
        'items' => $items,
        'title' => NULL,
        'type' => 'ul',
        'attributes' => array('class' => array('pager menu-horizontal')),
        )
      ) . '</div>';
  }
}
