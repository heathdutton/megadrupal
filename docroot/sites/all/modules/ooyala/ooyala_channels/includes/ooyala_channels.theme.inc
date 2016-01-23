<?php

/**
 * Theme the table of Ooyala channels on the node edit form.
 *
 * @param $items
 *   An array of lineup items.
 *
 * @return
 *   The themed table.
 */
function theme_ooyala_channels_lineup_items_edit($vars) {
  $items = $vars['items'];
  $rows = array();
  $output = "";

  $operations_heading = false;

  foreach(element_children($items) as $child) {
    $item = $items[$child];
    $row = array();

    // Figure out how many items we have to render.
    if ($child == 'lineup') {
      $item_count = count(array_filter(array_keys($item), 'is_numeric'));

      $delta = 0;

      for ($delta = 0; $delta < $item_count; $delta++) {
        // Render each of the rows here.
        $row = array();

        $row[] = drupal_render($item[$delta]['weight']);
        $row[] = drupal_render($item[$delta]['thumbnail']);
        $row[] = drupal_render($item[$delta]['title']);
        $row[] = drupal_render($item[$delta]['status']);
        $row[] = drupal_render($item[$delta]['length']);

        if (!empty($item[$delta]['#operations'])) {
          $operations_heading = TRUE;
          // If this key has been set, the operations have been rendered for us.
          $row[] = $item[$delta]['#operations'];
        }

        $rows[] = array(
           'data' => $row,
           'class' => array('draggable'),
        );
      }
    }
  }

  // Only display the table if at least one row has been created.
  if (!empty($rows)) {
    $headings = array(t('Weight'), '', t('Title'), t('Status'), t('Length'));
    if ($operations_heading) {
      $headings[] = t('Operations');
    }
    $output .= theme('table', array('header' => $headings, 'rows' => $rows, 'attributes' => array('id' => array('ooyala-channels-lineup-edit'))));
  }

  return $output;
}

/**
 * Function to render the channel lineup. This function splits the radios
 * element up so it is rendered within each row's operations column.
 *
 * @param $element
 *   The form element to render.
 *
 * @return
 *   The themed HTML of the channel lineup.
 */
function theme_ooyala_channels_lineup($vars) {
  $element = $vars['element'];
  // This gives us radio elements from the radios element.
  if (!isset($element['lineup'])) {
    return "";
  }


  $element['thumbnail'] = form_process_radios($element['thumbnail']);

  foreach (element_children($element['lineup']) as $child) {
    // We render the checkbox element (which is keyed by embed code) and then
    // append the Remove checkbox.
    $embed_code = $element['lineup'][$child]['embedCode']['#value'];
    $element['lineup'][$child]['#operations'] = drupal_render($element['thumbnail'][$embed_code]);
    $element['lineup'][$child]['#operations'] .= drupal_render($element['lineup'][$child]['remove']);
  }

  // While we've rendered each radio element, we haven't rendered the radios
  // wrapper element. Set #printed to TRUE so drupal_render() skips the
  // element.
  $element['thumbnail']['#printed'] = TRUE;

  // Note this will call theme_ooyala_channels_lineup_items_edit() for us.
  $element['#theme'] = 'ooyala_channels_lineup_items_edit';

  return drupal_render($element);
}

/**
 * Helper theme function to add tabledrag for the channel lineup table. We use
 * a separate function so that it can be added to a form even if the table is
 * not rendered so tabledrag will work on all #ajax elements.
 */
function theme_ooyala_channels_lineup_tabledrag($element) {
  drupal_add_tabledrag('ooyala-channels-lineup-edit', 'order', 'self', 'ooyala-channels-lineup-edit-weight', NULL, NULL, FALSE, NULL); // orig
//  drupal_add_tabledrag('ooyala-channels-add-to-channel-list', 'order', 'self', 'ooyala-channels-table-lineup-0-weight');
}

