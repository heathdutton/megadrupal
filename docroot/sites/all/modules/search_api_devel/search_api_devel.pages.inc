<?php
/**
 * @file
 * Provides functions, used as menu callbacks.
 */

/**
 * Immediately reindex entity by search_api.
 */
function search_api_devel_reindex_entity($type, $id) {
  $indexes = search_api_index_load_multiple(FALSE);
  if ($type == 'node') {
    foreach ($indexes as $index) {
      if ($index->enabled) {
        search_api_index_specific_items($index, array($id));
      }
    }

    drupal_set_message(t('Node with nid @id was successfully indexed in all enabled indexes.', array('@id' => $id)));
    drupal_goto("node/$id");
  }
}

/**
 * Main callback of module.
 */
function search_api_devel_page_form($form, $form_state, $type, $id) {
  if ($type == 'node') {
    $form = array();

    $form['info'] = array(
      '#type' => 'markup',
      '#markup' => '',
    );

    $form['entity_type'] = array(
      '#type' => 'hidden',
      '#value' => $type,
    );

    $form['entity_id'] = array(
      '#type' => 'hidden',
      '#value' => $id,
    );

    $index_data = array();

    // Load indexes.
    $indexes = search_api_index_load_multiple(FALSE, array('item_type' => $type, 'enabled' => 1));
    if (!empty($indexes)) {

      $form['reindex'] = array(
        '#type' => 'submit',
        '#value' => t('Reindex'),
      );

      foreach ($indexes as $index) {
        if ($server = search_api_server_load($index->server)) {
          $res = $server->getSolrConnection()->search(
            '',
            array(
              'fq' => array(
                "index_id:$index->machine_name",
                "item_id:$id",
              ),
              'fl' => '*',
            )
          );
          if (!empty($res->response->docs)) {
            $data = (reset($res->response->docs));
            $index_data[$index->machine_name] = kdevel_print_object($data);
          }
        }
      }
    }

    if (!empty($index_data)) {
      $form['info']['#markup'] = t('Now this node has been indexed and contained in these indexes:');
      foreach ($index_data as $index_name => $data) {
        $form['info']['#markup'] .= '<h4>' . t('Index machine name: @index_name', array('@index_name' => $index_name)) . '</h4>';
        $form['info']['#markup'] .= $data;
      }

      $form['clear'] = array(
        '#type' => 'submit',
        '#value' => t('Remove from indexes'),
      );
    }
    else {
      $form['info']['#markup'] = t('Now this node has been not indexed.');
      $form['info']['#markup'] .= '<br />';
    }

    return $form;
  }
}

/**
 * Submit handler for search_api_devel_page_form.
 */
function search_api_devel_page_form_submit($form, $form_state) {
  $indexes = search_api_index_load_multiple(FALSE, array('item_type' => $form_state['values']['entity_type'], 'enabled' => 1));

  // Reindex.
  if ($form_state['values']['op'] == t('Reindex')) {
    if (!empty($indexes)) {
      foreach ($indexes as $index) {
        search_api_index_specific_items($index, array($form_state['values']['entity_id']));
      }
    }
  }
  // Remove from indexes.
  elseif ($form_state['values']['op'] == t('Remove from indexes')) {
    if (!empty($indexes)) {
      foreach ($indexes as $index) {
        if ($index->server) {
          $server = $index->server();
          $server->deleteItems(array($form_state['values']['entity_id']), $index);
        }
      }
    }
  }
}
