<?php

/**
 * @file
 *  Results page functions for Blogger Importer.
 */

/**
 * Callback after import batch is done, to view & handle results.
 * Results should be saved in *cache*.
 *
 * Also for viewing *individual* entries.
 */
function blogger_importer_view_results($type = NULL, $id = NULL) {
  $output = '';

  // Pull in status report.
  $tmp = blogger_importer_status_report();
  $output .= drupal_render($tmp);

  $results = blogger_importer_results();

  if (empty($results)) {
    drupal_set_message(t("There is no imported Blogger data yet. Run the Process XML batch to populate the data."), 'error');
    drupal_goto('admin/content/blogger_importer');
  }

  // Individual value.
  if (!is_null($type) && !is_null($id)) {
    if (isset($results[$type][$id])) {
      $entry = $results[$type][$id];
      $elem = array(
        '#type' => 'fieldset',
        '#title' => t('Entry @id (@type)', array('@id' => $id, '@type' => $type)),

        'item' => array(
          '#markup' => '<h3>' . $entry['title'] . '</h3>'
              . '<strong>Date:</strong> ' . $entry['date'] . '<br/>'
              . '<br/><br/>'
              . $entry['value']
        ),
      );
      $output .= l('< Back to all entries', 'admin/content/blogger_importer/results')
            . '<br/>'
            . drupal_render($elem);

      return $output;
    }
    else {
      drupal_set_message( t('Entry @id (@type) not found.', array('@id' => $id, '@type' => $type)), 'error');
    }
  }

  // All values.

  // Buttons to import nodes & comments (triggers batches).
  $tmp = drupal_get_form('blogger_importer_create_nodes_form');
  $output .= drupal_render($tmp);
  $tmp = drupal_get_form('blogger_importer_create_comments_form');
  $output .= drupal_render($tmp);

  // button to delete the cached data
  $tmp = drupal_get_form('blogger_importer_delete_cache_form');
  $output .= drupal_render($tmp);

  $output .= '<h2>Imported Blogger Entries</h2>';

  // alter the entries for display
  foreach($results as $entry_type => $entries) {

    // table headers
    // (put keys to make array_keys possible for mapping to rows)
    $headers = array(
      'id' => t('ID'),
      'title' => t('Title/Description'),
      'value' => t('Value'),
      'date' => t('Date')
    );

    // special columns
    if ($entry_type == 'post') {
      $headers['url'] = t('URL');
      $headers['nid'] = t('Nid');
    }
    if ($entry_type == 'comment') {
      $headers['post_id'] = t('Post ID');
      $headers['nid'] = t('nid');
      $headers['cid'] = t('cid');
      $headers['author'] = t('Author');
    }


    // format rows for display
    $show_entries = $entries;
    foreach($show_entries as &$show_entry) {

      // link to full value page
      $show_entry['value'] = l( htmlentities( truncate_utf8($show_entry['value'], 50, FALSE, TRUE) ),
            'admin/content/blogger_importer/result/' . $entry_type . '/' . $show_entry['id'],
            array('html' => TRUE)
          );

      // format date
      if (!empty($show_entry['date'])) {
        $show_entry['date'] = format_date(strtotime($show_entry['date']), 'custom', 'r');
      }

      // url (for posts) as link
      if (isset($show_entry['url']) && !empty($show_entry['url'])) {
        $show_entry['url'] = l($show_entry['url'], $show_entry['url'], array('absolute' => TRUE, 'attributes' => array('target' => '_blank')));
      }


      // extrapolate post info for comments
      if ($entry_type == 'comment') {

        $show_entry['post_id'] = _blogger_importer_comment_post($show_entry['id'], $results, 'id');

        $show_entry['nid'] = _blogger_importer_comment_post($show_entry['id'], $results, 'nid');

        $show_entry['author'] = '';
          if (!empty($show_entry['author_name'])) {
            $show_entry['author'] = $show_entry['author_name'];

            if (!empty($show_entry['author_uri'])) {
              $show_entry['author'] = l($show_entry['author'], $show_entry['author_uri'], array('absolute' => TRUE));
            }
            // [ignore email for now]
          }

        if (isset($post['nid']) && is_numeric($show_entry['cid'])) {
          $show_entry['cid'] = l($show_entry['cid'], 'node/' . $show_entry['nid'], array('fragment' => 'comment-' . $show_entry['cid']));
        }

      }

      // nid as link or filler space
      if (isset($show_entry['nid']) && !empty($show_entry['nid']) && !is_array($show_entry['nid'])) {
        $show_entry['nid'] = l($show_entry['nid'], 'node/' . $show_entry['nid'], array('absolute' => TRUE, 'attributes' => array('target' => '_blank')));
      }

      // make sure all expected values are in every row
      // & sort in the right order!
      $sorted_entry = array();
      foreach(array_keys($headers) as $header) {
        $sorted_entry[$header] = array_key_exists($header, $show_entry) ? $show_entry[$header] : '';
      }
      $show_entry = $sorted_entry;
    }

    $table = theme('table', array('header' => $headers, 'rows' => $show_entries));
    $classes = !in_array($entry_type, array('post', 'comment')) ? array('collapsed', 'collapsible') : array('collapsible');
    $fieldset = array(
      '#type' => 'fieldset',
      '#title' => t('@type entries', array('@type' => $entry_type)),
      '#attributes' => array('class' => $classes),


      'table' => array(
        '#type' => 'markup',
        '#prefix' => '<h3>' . t('<strong>@type</strong> entries', array('@type' => $entry_type)) . '</h3>',
        '#markup' => $table,
        '#description' => t('Click a value field to see the full entry.'),
      ),
    );
    $output .= drupal_render($fieldset);
  }

  return $output;
}
