<?php
/**
 * @file
 * Stock administration menu items.
 */

/**
 * Form constructor for uc attribute stock form.
 *
 * @param $node
 *   The node to modify.
 *
 * @see uc_attribute_stock_form_submit()
 * @ingroup forms
 */
function uc_attribute_stock_form($form, &$form_state, $node) {
  drupal_set_title($node->title);
  $nid = $node->nid;
  $model = $node->model;

  // Gets all attributes related to the node
  $result = db_query("SELECT pa.nid, pa.aid, pa.ordering, pa.display, a.name, a.ordering, ao.aid, COUNT(po.oid) FROM {uc_product_attributes} AS pa LEFT JOIN {uc_attributes} AS a ON pa.aid = a.aid LEFT JOIN {uc_attribute_options} AS ao ON a.aid = ao.aid LEFT JOIN {uc_product_options} AS po ON ao.oid = po.oid AND po.nid = :po_nid WHERE pa.nid = :pa_nid AND pa.display <> :pa_display GROUP BY ao.aid, pa.aid, pa.display, a.name, pa.ordering, a.ordering, pa.nid HAVING count(po.oid) > 0 ORDER BY pa.ordering, a.ordering", array(':po_nid' => $nid, ':pa_nid' => $nid, ':pa_display' => 3));

  $attribute_names = '';
  $attribute_ids = array();

  foreach ($result as $prod_attr) {
    $attribute_names .= '<th>' . check_plain($prod_attr->name) . '</th>';
    $attribute_ids[] = $prod_attr->aid;
  }

  // Number of attribute types
  $num_prod_attr = count($attribute_ids);

  if ($num_prod_attr) {
    $form['original'] = array(
      '#markup' => '<p><b>' . t('Default product SKU: @sku', array('@sku' => $model)) . '</b></p>',
    );
    $form['default'] = array(
      '#type' => 'value',
      '#value' => 0,
    );
    $form['table'] = array(
      '#prefix' => '<table class="combinations">',
      '#suffix' => '</table>',
    );
    // The head of the table
    $form['table']['head'] = array(
      '#markup' => '<thead><tr><th>' . t('Active') . '</th><th>' . t('Unlimited') . '</th>' . $attribute_names . '<th>' . t('Stock') . '</th><th>' . t('Treshold') . '</th></tr></thead>',
      '#weight' => 0,
    );
    // The body of the table
    $form['table']['body'] = array(
      '#prefix' => '<tbody>',
      '#suffix' => '</tbody>',
      '#weight' => 1,
      '#tree' => TRUE,
    );

    // Gets all options related to the node
    $query = db_select('uc_product_options', "upo");

    $query->leftJoin('uc_attribute_options', "uao", "upo.oid = uao.oid AND upo.nid = :nid", array(':nid' => $nid));
    $query->addField('uao', 'aid', 'aid');
    $query->addField('uao', 'name', 'name');
    $query->addField('uao', 'oid', 'oid');
    $query->addField('upo', 'ordering', 'ordering');

    $query->condition("upo.nid", $nid)
      ->orderBy("upo.ordering")
      ->orderBy("uao.name");

    $result = $query->execute();

    while ($combo = $result->fetchObject()) {
      $options[$combo->aid][] = array('name' => $combo->name, 'oid' => $combo->oid);
    }
    ksort($options);

    // Gets all combinations of options related to the node
    $old_vals = db_select('uc_attribute_stock', 'uas')->condition('nid', $nid, '=')->fields('uas')->execute();

    // Adds the options into the table
    $rows_counter = 0;
    foreach ($old_vals as $ov) {
      $combination = unserialize($ov->combination);
      $form['table']['body'][$rows_counter] = array(
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
      );
      $form['table']['body'][$rows_counter]['active'] = array(
        '#type' => 'checkbox',
        '#default_value' => 1,
        '#prefix' => '<td>',
        '#suffix' => '</td>',
      );

      $form['table']['body'][$rows_counter]['unlimited'] = array(
        '#type' => 'checkbox',
        '#default_value' => $ov->unlimited,
        '#prefix' => '<td>',
        '#suffix' => '</td>',
      );

      foreach ($options as $aid => $value) {
        $select_options[''] = t('Please select');
        foreach ($value as $k => $v) {
          $select_options[$v['oid']] = t($v['name']);
        }

        if (isset($combination[$aid])) {
          $com = $combination[$aid];
        }
        else {
          $com = '';
        }

        $form['table']['body'][$rows_counter]['atributes'][$aid] = array(
          '#type' => 'select',
          '#default_value' => $com,
          '#options' => $select_options,
          '#description' => t('The log.'),
          '#prefix' => '<td>',
          '#suffix' => '</td>',
        );

        $select_options = '';
      }

      $form['table']['body'][$rows_counter]['uas_id'] = array(
        '#type' => 'value',
        '#value' => $ov->id,
      );

      $form['table']['body'][$rows_counter]['stock'] = array(
        '#type' => 'textfield',
        '#default_value' => $ov->stock,
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#size' => 5,
      );
      $form['table']['body'][$rows_counter]['treshold'] = array(
        '#type' => 'textfield',
        '#default_value' => $ov->treshold,
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#size' => 5,
      );
      $rows_counter++;
    }

    // Adds 10 extra rows into the table. It allows a user to define 10 extra combinations
    $total_number_of_rows = $rows_counter + 10;
    for ($rows_counter; $rows_counter < $total_number_of_rows; $rows_counter++) {
      $form['table']['body'][$rows_counter] = array(
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
      );
      $form['table']['body'][$rows_counter]['active'] = array(
        '#type' => 'checkbox',
        '#default_value' => '',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
      );

      $form['table']['body'][$rows_counter]['unlimited'] = array(
        '#type' => 'checkbox',
        '#default_value' => '',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
      );

      foreach ($options as $key => $value) {
        $select_options[''] = t('Please select');
        $aid = $key;
        foreach ($value as $k => $v) {
          $select_options[$v['oid']] = t($v['name']);
        }

        $form['table']['body'][$rows_counter]['atributes'][$aid] = array(
          '#type' => 'select',
          '#options' => $select_options,
          '#description' => t('The log.'),
          '#prefix' => '<td>',
          '#suffix' => '</td>',
        );

        $select_options = '';

      }

      $form['table']['body'][$rows_counter]['uas_id'] = array(
        '#type' => 'value',
        '#value' => 0,
      );

      $form['table']['body'][$rows_counter]['stock'] = array(
        '#type' => 'textfield',
        '#default_value' => 0,
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#size' => 5,
      );
      $form['table']['body'][$rows_counter]['treshold'] = array(
        '#type' => 'textfield',
        '#default_value' => 0,
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#size' => 5,
      );
    }

    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => $nid,
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['error'] = array(
      '#markup' => '<div><br />' . t('This product does not have any attributes that can be used for SKU adjustments.') . '</div>',
    );
  }

  $form['pager'] = array(
    '#theme' => 'pager',
  );

  $form['#validate'][] = 'uc_attribute_stock_form_validate';

  return $form;
}


/**
 * Form validation handler for uc_attribute_stock_form().
 */
function uc_attribute_stock_form_validate($form, &$form_state) {
  foreach ($form_state['values']['body'] as $value) {

    foreach ($value['atributes'] as $aid => $oid) {
      $comb_array[$aid] = $oid;
    }
    $combination = serialize($comb_array);


    if ($value['uas_id']) {
      if ($value['active'] == 1) {
        $count = db_select('uc_attribute_stock', 'uas')
          ->fields('uas')
          ->condition('id', $value['uas_id'], '<>')
          ->condition('nid', $form_state['values']['nid'], '=')
          ->condition('combination', $combination, '=')
          ->execute()
          ->rowCount();

        if ($count > 0) {
          form_set_error('', 'Some combinations are repeated');
        }
      }
    }
    else {
      if ($value['active'] == 1) {
        $count = db_select('uc_attribute_stock', 'uas')
          ->fields('uas')
          ->condition('nid', $form_state['values']['nid'], '=')
          ->condition('combination', $combination, '=')
          ->execute()
          ->rowCount();

        if ($count > 0) {
          form_set_error('', 'Some combinations are repeated');
        }
      }
    }
  }
}

/**
 * Form submission handler for uc_attribute_stock_form().
 */
function uc_attribute_stock_form_submit($form, &$form_state) {

  $prod_stock = 0;
  $prod_thresh = 0;

  foreach ($form_state['values']['body'] as $value) {

    foreach ($value['atributes'] as $aid => $oid) {
      $comb_array[$aid] = $oid;
    }
    $combination = serialize($comb_array);

    if ($value['active'] == 1) {
      $prod_stock = $prod_stock + $value['stock'];
      $prod_thresh += $value['treshold'];
    }

    if ($value['uas_id']) {
      if ($value['active'] == 1) {
        db_update('uc_attribute_stock')
          ->fields(array(
            'combination' => $combination,
            'unlimited' => $value['unlimited'],
            'active' => $value['active'],
            'stock' => $value['stock'],
            'treshold' => $value['treshold'],
          ))
          ->condition('id', $value['uas_id'], '=')
          ->execute();
      }
      else {
        db_delete('uc_attribute_stock')
          ->condition('id', $value['uas_id'])
          ->execute();
      }
    }
    else {
      if ($value['active'] == 1) {
        db_merge('uc_attribute_stock')
          ->key(array(
            'nid' => $form_state['values']['nid'],
            'combination' => $combination,
          ))
          ->fields(array(
            'unlimited' => $value['unlimited'],
            'active' => $value['active'],
            'stock' => $value['stock'],
            'treshold' => $value['treshold'],
          ))
          ->execute();
      }
    }
  }

  $query = db_query("SELECT COUNT(*) as node_count FROM {uc_product_stock} WHERE nid = :nid", array(':nid' => $form_state['values']['nid']));
  $count = $query->fetchField();

  if ($count == 0) {
    db_insert('uc_product_stock')
      ->fields(array(
        'sku' => $form_state['build_info']['args'][0]->model,
        'nid'  => $form_state['values']['nid'],
        'active'  => 0,
        'stock' => $prod_stock,
        'threshold' => $prod_thresh,
      ))
      ->execute();
  }
  else {
    db_update('uc_product_stock')
      ->fields(array(
        'stock' => $prod_stock,
        'threshold' => $prod_thresh,
      ))
      ->condition('nid', $form_state['values']['nid'], '=')
      ->execute();
  }

  drupal_set_message(t('Product adjustments & stock have been saved.'));
}

/**
 * General settings for the uc attribute stock.
 *
 * @ingroup forms
 */
function uc_attribute_stock_settings_form($form, &$form_state) {
  $value = variable_get('uc_attribute_stock_display_stock_level');

  $form['uc_attribute_stock_display_stock_level'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display stock level with options?'),
    '#default_value' => $value,
  );

  return system_settings_form($form);
}
