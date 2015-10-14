<?php // $Id: nodequeue_randomizer.theme.inc,v 1.1 2010/01/21 00:29:22 gekigangaa Exp $
/**
 * @file nodequeue_randomizer.theme.inc
 *
 * @author Greg Field
 * @since Nov 30th, 2009
 **/

/**
 * Theme function for the nodequeue_randomizer_form_admin_weight form
 *
 * @param $form
 *   Array which represents a Drupal form
 *
 * @return
 *   Rendered HTML of the supplied form
 */
function theme_nodequeue_randomizer_form_admin_weight($variables) {
  $form = $variables['form'];

  if (isset($form['subqueue_list'])) {
    return drupal_render($form);
  }

  $rows = array();
  foreach ($form as $key => $value) {
    if (strlen($key) > 5 && substr($key, 0, 5) == 'node_') {
      $row = array();

      $row[] = array(
        'data' => $value['#attributes']['title']
      );
      $row[] = array(
        'data' => drupal_render($form[$key])
      );

      $rows[] = $row;
    }
  }

  $output = theme('table', array(
      'header' => array(t('Node'), t('Weight')),
      'rows'   => $rows
    )
  );
  $output .= '<br>';
  $output .= drupal_render_children($form);

  return $output;
}
