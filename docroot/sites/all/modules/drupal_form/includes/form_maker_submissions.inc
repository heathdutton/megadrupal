<?php
/**
 * @file
 * Form maker submissions.
 */

/**
 * Form submissions table.
 */
function form_maker_submissions() {
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_calendar-setup.js');
  // drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_cal_function.js');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_cal.js');
  drupal_add_css(drupal_get_path('module', 'form_maker') . '/css/form_maker_cal.css');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_sliding_popup.js');
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_if_gmap.js');
  $external_js = 'http://maps.google.com/maps/api/js?sensor=false';
  drupal_add_js('document.write(unescape("%3Cscript src=\'' . $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', array('type' => 'inline'));
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  // Form entries.
  $group_ids_column = db_select('form_maker_submits_table', 'n')
    ->fields('n', array('group_id'))
    ->condition('n.form_id', $nodeid, '=')
    ->groupBy('n.group_id')
    ->execute()->fetchCol();
  $form['fieldset_form_submissions'] = array(
    '#prefix' => '<table width="95%" rules="none" style="border:none;">
                    <tr>	
                      <td style="width: 100%; text-align:right;font-size:16px; padding:20px; padding-right:50px; right:0; border:none;">
                        <a href="http://web-dorado.com/products/drupal-form-builder.html" target="_blank" style="color:red; text-decoration:none;">
                          <img src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/header.png" border="0" alt="www.web-dorado.com" width="215"><br>
                        Get the full version&nbsp;&nbsp;&nbsp;&nbsp;
                        </a>
                      </td>
                    </tr>
                  </table>',
    '#type' => 'fieldset',
    '#title' => t('Submissions'),
  );
  $form['fieldset_form_submissions']['export_to'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export to'),
  );
  $form['fieldset_form_submissions']['export_to']['csv'] = array(
    '#prefix' => '<input type="button" value="' . t('CSV') . '" onclick=\'alert(Drupal.t("This functionality is disabled in free version. If you need this functionality, you need to buy the commercial version."))\' class="form-submit">&nbsp;
                  <input type="button" value="' . t('XML') . '" onclick=\'alert(Drupal.t("This functionality is disabled in free version. If you need this functionality, you need to buy the commercial version."))\' class="form-submit">',
  );
  $form['fieldset_form_submissions']['reports_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reports'),
  );
  $form['fieldset_form_submissions']['reports_fieldset']['reports'] = array(
    '#prefix' => '<div class="form_maker_reports"><b>' . t('Entries') . '</b><br />' . count($group_ids_column) . '</div><div class="form_maker_reports"><b>' . t('Views') . '</b><br />' . variable_get('form_maker_view_reports' . $nodeid, '0') . '</div><div class="form_maker_reports"><b>' . t('Conversion Rate') . '</b><br />' . round((count($group_ids_column))*100/variable_get('form_maker_view_reports' . $nodeid, '1'), 2) . '%</div>',
  );
  $form['fieldset_form_submissions']['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
  );
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_submissions_search.js');
  drupal_add_js(array(
    'form_maker' => array(
      'filter_img_src' => base_path() . drupal_get_path('module', 'form_maker') . '/images/',
      ),
    ),
    'setting');
  $form['fieldset_form_submissions']['search']['go_button'] = array(
    '#prefix' => '<br /><img id="filter_img" width="40" title="' . t('Search by fields') . '" onclick="form_maker_show_hide_search_row();" style="vertical-align:middle; cursor:pointer" src="' . base_path() . drupal_get_path('module', 'form_maker') . '/images/' . ((variable_get('search_on', '') == '') ? 'filter_show' :  'filter_hide') . '.png">',
    '#type' => 'submit',
    '#submit' => array('form_maker_go_button'),
    '#value' => t('Go'),
  );
  $form['fieldset_form_submissions']['search']['reset_button'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_reset_button'),
    '#value' => t('Reset'),
  );
  $form['fieldset_form_submissions']['edit_submissions'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_edit_checked_submissions'),
    '#value' => t('Edit'),
  );
  $form['fieldset_form_submissions']['delete_submissions'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_delete_submissions'),
    '#value' => t('Delete'),
    '#attributes' => array('onclick' => 'if (!confirm(Drupal.t("Do you want to delete selected submissions?"))) {return false;}'),
  );
  $form['fieldset_form_submissions']['add_remove_columns'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_add_remove_columns'),
    '#value' => t('Add/Remove Columns'),
  );
  $value = db_query("SELECT * FROM {form_maker_submits_table} WHERE form_id=:form_id", array(':form_id' => $nodeid));
  $rows = array();
  while ($a = $value->fetchObject()) {
    $rows[] = $a;
  }
  $n = count($rows);
  $labels = array();
  for ($i = 0; $i < $n; $i++) {
    $row = &$rows[$i];
    if (!in_array($row->element_label, $labels)) {
      array_push($labels, $row->element_label);
    }
  }
  $sorted_labels_id = array();
  $sorted_labels = array();
  $label_titles = array();
  if ($labels) {
    $label_id = array();
    $label_order = array();
    $label_order_original = array();
    $label_type = array();
    $this_form = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
    $label_all = explode('#****#', $this_form->label_order);
    $label_all = array_slice($label_all, 0, count($label_all) - 1);
    foreach ($label_all as $key => $label_each) {
      $label_id_each = explode('#**id**#', $label_each);
      array_push($label_id, $label_id_each[0]);
      $label_oder_each = explode('#**label**#', $label_id_each[1]);
      array_push($label_order_original, $label_oder_each[0]);
      $ptn = "/[^a-zA-Z0-9_]/";
      $rpltxt = "";
      $label_temp = preg_replace($ptn, $rpltxt, $label_oder_each[0]);
      array_push($label_order, $label_temp);
      array_push($label_type, $label_oder_each[1]);
    }
    foreach ($label_id as $key => $label) {
      if (in_array($label, $labels)) {
        array_push($sorted_labels, $label_order[$key]);
        array_push($sorted_labels_id, $label);
        array_push($label_titles, $label_order_original[$key]);
      }
    }
    $i = 0;
    foreach ($sorted_labels_id as $idd) {
      $labelll[$idd] = $label_titles[$i];
      $i++;
    }
  }
  $labels_id = $sorted_labels_id;
  
  // Add to submissions table static headers.
  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'n.id'),
    'date' => array('data' => '<div style="width:260px;">' . t('Submit date') . '</div>', 'field' => 'n.date'),
    'ip' => array('data' => t("Submitter's IP Address"), 'field' => 'n.ip'),
  );
  $header_string = 'id@@&&@@date@@&&@@ip';
  // Add to submissions table additional headers.
  foreach ($labels_id as $key => $label_id) {
    $header[$label_id] = array('data' => $labelll[$label_id], 'field' => 'n.element_value');
    $header_string .= '@@&&@@' . $label_id;
  }
  // Add to submissions table edit header.
  $header['edit'] = array('data' => t('Edit'));
  $header['delete'] = array('data' => t('Delete'));

  // Submissions table all headers as string.
  variable_set('form_maker_header_array_string', $header_string);

  $headers = explode('@@&&@@', $header_string);
  $header_array = $header;
  $table_description = '';
  foreach ($headers as $header_title) {
    if (variable_get('form_maker_is_checked_' . $header_title, 'none') == 2) {
      // Delete unchecked header.
      unset($header_array[$header_title]);
      $table_description = t('To add or remove columns from submissions table click "Add/Remove Columns" button.');
    }
  }
  $checked_header = $header_array;
  // Search row in table.
  $options['search_row'] = array(
    'id' => array(
      'data' => '',
    ),
    'edit' => array(
      'data' => '',
    ),
    'delete' => array(
      'data' => '',
    ),
    'date' => array(
      'data' => array(
        'data_search_from' => array(
          '#type' => 'textfield',
          '#title' => t('From:'),
          '#size' => 15,
          '#value' => variable_get('data_search_from', ''),
          '#attributes' => array(
            'id' => 'data_search_from',
            'name' => 'data_search_from',
          ),
        ),
        'data_search_from_button' => array(
          '#type' => 'submit',
          '#value' => t('...'),
          '#attributes' => array(
            'onclick' => 'return showCalendar("data_search_from","%Y-%m-%d");',
            'class' => array('data_search_from_button'),
          ),
        ),
        'data_search_to' => array(
          '#type' => 'textfield',
          '#title' => t('to:'),
          '#size' => 15,
          '#value' => variable_get('data_search_to', ''),
          '#attributes' => array(
            'id' => 'data_search_to',
            'name' => 'data_search_to',
          ),
        ),
        'data_search_to_button' => array(
          '#type' => 'submit',
          '#value' => t('...'),
          '#attributes' => array(
            'onclick' => 'return showCalendar("data_search_to","%Y-%m-%d");',
            'class' => array('data_search_to_button'),
          ),
        ),
      ),
      'class' => array('date_search_td'),
    ),
    'ip' => array(
      'data' => array(
        '#type' => 'textfield',
        '#size' => 15,
        '#value' => variable_get('ip_search', ''),
        '#attributes' => array(
          'id' => 'ip_search',
          'name' => 'ip_search',
        ),
      ),
    ),
  );
  // Search on or off.
  $options['search_row']['#attributes'] = array('id' => 'search_row_id', 'style' => (variable_get('search_on', '') == '') ? 'display:none;' : '');

  $query = db_select('form_maker_submits_table', 'n')
    ->fields('n', array('group_id'))
    ->condition('n.date', (variable_get('data_search_from', '') == '') ? '1900-01-01' :  variable_get('data_search_from', ''), '>=')
    ->condition('n.date', (variable_get('data_search_to', '') == '') ? '2200-01-01' :  variable_get('data_search_to', ''), '<=')
    ->condition('n.ip', '%' . db_like(variable_get('ip_search', '')) . '%', 'LIKE')
    ->condition('n.form_id', $nodeid, '=')
    ->extend('TableSort')
    ->orderByHeader($checked_header)
    ->groupBy('n.group_id')
    ->extend('PagerDefault')
    ->limit(20);
  foreach ($headers as $header_title) {
    if (($header_title != 'id') && ($header_title != 'date') && ($header_title != 'ip')) {
      $options['search_row'][$header_title] = array(
        'data' => array(
          '#type' => 'textfield',
          '#size' => 15,
          '#value' => variable_get($header_title . '_search', ''),
          '#attributes' => array(
            'id' => $header_title . '_search',
            'name' => $header_title . '_search',
          ),
        ),
      );
      // Dinamically add condition to query.
      $query = $query->condition('n.element_value', '%' . db_like(variable_get($header_title . '_search', '')) . '%', 'LIKE');
      if (variable_get($header_title . '_search', '') != '') {
        $query = $query->condition('n.element_label', $header_title, '=');
      }
    }
  }
  $ids_form_group_id = $query->execute()->fetchCol();
  if (db_query("SELECT id FROM {form_maker_submits_table}")->fetchCol()) {
    foreach ($ids_form_group_id as $id_form_group_id) {
      $row = db_query("SELECT * FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $id_form_group_id))->fetchObject();
      // Add to submissions table row static fields.
      $options[$id_form_group_id] = array(
        'id' => $row->id,
        'date' => $row->date,
        'ip' => $row->ip,
      );
      // Add to submissions table row additional fields of form.
        foreach ($labels_id as $keys => $label_id) {
          $element_value = db_query("SELECT element_value FROM {form_maker_submits_table} WHERE element_label=:element_label AND group_id=:group_id", array(':element_label' => $label_id, ':group_id' => $id_form_group_id))->fetchField();
          if (strpos($element_value, "***map***")) {
            $map_params = explode('***map***', $element_value);
            $longit = $map_params[0];
            $latit = $map_params[1];
            $element_value = '<a class="thickbox-preview" href=\'javascript:form_maker_createpopup("' . url('node/' . $nodeid . '/submissions/map_preview', array('query' => array('longit' => $longit, 'latit' => $latit), 'absolute' => FALSE)) . '", navigator.userAgent.indexOf("Opera") > -1?580:jQuery(window).height() - 70, 1, "testpopup", 5);\'>' . t('Show on Map') . '</a>';
          }
          if (strpos($element_value, "*@@url@@*")) {
            $new_file = str_replace("*@@url@@*", '', str_replace('***br***', '<br />', $element_value));
            $new_filename = explode('/', $new_file);
            $element_value = '<a target="_blank" href="' . $new_file . '">' . $new_filename[count($new_filename) - 1] . '</a>';
          }
          if ($element_value == '::' || $element_value == '--') {
            $element_value = '';
          }
          $element_value = str_replace('***br***', '<br />', $element_value);
          $options[$id_form_group_id][$label_id] = $element_value;
        }
      // Add to submissions table row edit link.
      $options[$id_form_group_id]['edit'] = array('data' => array(
          '#type' => 'link',
          '#title' => t('Edit'),
          '#href' => url('node/' . $nodeid . '/submissions/edit', array('query' => array('id' => $row->id, 'group_id' => $id_form_group_id), 'absolute' => TRUE)),
        ),
      );
      $options[$id_form_group_id]['delete'] = array('data' => array(
          '#type' => 'link',
          '#title' => t('Delete'),
          '#href' => url('node/' . $nodeid . '/submissions/delete', array('query' => array('id' => $row->id, 'group_id' => $id_form_group_id), 'absolute' => TRUE)),
        ),
      );
    }
  }
  else {
    $options = array();
  }
  $form['fieldset_form_submissions']['submissions_table'] = array(
    '#type' => 'tableselect',
    '#prefix' => '<div class="description">' . $table_description . '</div>',
    '#header' => $checked_header,
    '#options' => $options,
    '#empty' => t('No submissions available.'),
    '#suffix' => theme('pager', array('tags' => array())),
  );
  $form['fieldset_form_submissions']['submissions_table']['search_row']['#disabled'] = TRUE;

  $label_order_field = db_query("SELECT label_order 	 FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchField();
  $exploe_label_order = explode('#****#', $label_order_field);
  $choices_header = array(
    'choices' => array('data' => t('Choices'), 'width' => 300),
    'percentage' => array('data' => t('Percentage')),
    'count' => array('data' => t('Count'), 'width' => 40),
  );
  foreach ($exploe_label_order as $label_orders) {
    $label = explode('#**label**#', $label_orders);
    if (isset($label[1]) && ($label[1] == 'type_own_select')) {
      $label_name = explode('#**id**#', $label[0]);
      // $element_value_col = db_query("SELECT element_value FROM {form_maker_submits_table} WHERE element_label=:element_label AND form_id=:form_id", array(':element_label' => $label_name[0], ':form_id' => $nodeid))->fetchCol();
      $query = db_select('form_maker_submits_table', 'n')
        ->fields('n', array('element_value'))
        ->condition('n.date', (variable_get('data_search_from', '') == '') ? '1900-01-01' :  variable_get('data_search_from', ''), '>=')
        ->condition('n.date', (variable_get('data_search_to', '') == '') ? '2200-01-01' :  variable_get('data_search_to', ''), '<=')
        ->condition('n.ip', '%' . db_like(variable_get('ip_search', '')) . '%', 'LIKE')
        ->condition('n.element_label', $label_name[0], '=')
        ->condition('n.form_id', $nodeid, '=');
      foreach ($headers as $header_title) {
        if (($header_title != 'id') && ($header_title != 'date') && ($header_title != 'ip')) {
          $options['search_row'][$header_title] = array(
            'data' => array(
              '#type' => 'textfield',
              '#size' => 15,
              '#value' => variable_get($header_title . '_search', ''),
              '#attributes' => array(
                'id' => $header_title . '_search',
                'name' => $header_title . '_search',
              ),
            ),
          );
          // Dinamically add condition to query.
          $query = $query->condition('n.element_value', '%' . db_like(variable_get($header_title . '_search', '')) . '%', 'LIKE');
          if (variable_get($header_title . '_search', '') != '') {
            $query = $query->condition('n.element_label', $header_title, '=');
          }
        }
      }
      $element_value_col = $query->execute()->fetchCol();
      $select_choices_array = array();
      foreach ($element_value_col as $element_value) {
        if (isset($select_choices_array[$element_value])) {
          $select_choices_array[$element_value] += 1; 
        }
        else {
          $select_choices_array[$element_value] = 1;
        }
      }
      // $id_col = db_query("SELECT id FROM {form_maker_submits_table} WHERE element_label=:element_label AND form_id=:form_id", array(':element_label' => $label_name[0], ':form_id' => $nodeid))->fetchCol();
      $query = db_select('form_maker_submits_table', 'n')
        ->fields('n', array('id'))
        ->condition('n.date', (variable_get('data_search_from', '') == '') ? '1900-01-01' :  variable_get('data_search_from', ''), '>=')
        ->condition('n.date', (variable_get('data_search_to', '') == '') ? '2200-01-01' :  variable_get('data_search_to', ''), '<=')
        ->condition('n.ip', '%' . db_like(variable_get('ip_search', '')) . '%', 'LIKE')
        ->condition('n.element_label', $label_name[0], '=')
        ->condition('n.form_id', $nodeid, '=');
      foreach ($headers as $header_title) {
        if (($header_title != 'id') && ($header_title != 'date') && ($header_title != 'ip')) {
          $options['search_row'][$header_title] = array(
            'data' => array(
              '#type' => 'textfield',
              '#size' => 15,
              '#value' => variable_get($header_title . '_search', ''),
              '#attributes' => array(
                'id' => $header_title . '_search',
                'name' => $header_title . '_search',
              ),
            ),
          );
          // Dinamically add condition to query.
          $query = $query->condition('n.element_value', '%' . db_like(variable_get($header_title . '_search', '')) . '%', 'LIKE');
          if (variable_get($header_title . '_search', '') != '') {
            $query = $query->condition('n.element_label', $header_title, '=');
          }
        }
      }
      $id_col = $query->execute()->fetchCol();
      $total = count($id_col);
      $i = 0;
      $percentage_color = '#2CBADE';
      foreach ($select_choices_array as $choices_value => $choices_count) {
        if ($choices_value != '#*first_value*#') {
          if ($choices_value == '') {
            $choices_value = t('Unanswered');
          }
          $select_choices_options[$i . '_type_own_select' . $label_name[1]]['choices'] = $choices_value;
          $select_choices_options[$i . '_type_own_select' . $label_name[1]]['percentage'] = '<div style="width:' . ($choices_count * 100) / $total . '%; height:18px; background-color:' . $percentage_color . '; border-radius:10px;"></div>';
          $select_choices_options[$i . '_type_own_select' . $label_name[1]]['count'] = $choices_count;
          $i++;
          if ($percentage_color == '#2CBADE') {
            $percentage_color = '#FE6400';
          }
          else {
            $percentage_color = '#2CBADE';
          }
        }
      }
      $select_choices_options['total_type_own_select' . $label_name[1]]['choices'] = '';
      $select_choices_options['total_type_own_select' . $label_name[1]]['percentage'] = t('Total');
      $select_choices_options['total_type_own_select' . $label_name[1]]['count'] = $total;      
      $form['fieldset_form_submissions']['choices_type_own_select' . $label_name[1]] = array(
        '#prefix' => '<b>' . $label_name[1] . '</b>',
        '#theme' => 'table',
        '#header' => $choices_header,
        '#rows' => $select_choices_options,
        '#tree' => TRUE,
      );      $select_choices_options = array();
    }
  }
  foreach ($exploe_label_order as $label_orders) {
    $label = explode('#**label**#', $label_orders);
    if (isset($label[1]) && ($label[1] == 'type_radio')) {
      $label_name = explode('#**id**#', $label[0]);
      // $element_value_col = db_query("SELECT element_value FROM {form_maker_submits_table} WHERE element_label=:element_label AND form_id=:form_id", array(':element_label' => $label_name[0], ':form_id' => $nodeid))->fetchCol();
      $query = db_select('form_maker_submits_table', 'n')
        ->fields('n', array('element_value'))
        ->condition('n.date', (variable_get('data_search_from', '') == '') ? '1900-01-01' :  variable_get('data_search_from', ''), '>=')
        ->condition('n.date', (variable_get('data_search_to', '') == '') ? '2200-01-01' :  variable_get('data_search_to', ''), '<=')
        ->condition('n.ip', '%' . db_like(variable_get('ip_search', '')) . '%', 'LIKE')
        ->condition('n.element_label', $label_name[0], '=')
        ->condition('n.form_id', $nodeid, '=');
      foreach ($headers as $header_title) {
        if (($header_title != 'id') && ($header_title != 'date') && ($header_title != 'ip')) {
          $options['search_row'][$header_title] = array(
            'data' => array(
              '#type' => 'textfield',
              '#size' => 15,
              '#value' => variable_get($header_title . '_search', ''),
              '#attributes' => array(
                'id' => $header_title . '_search',
                'name' => $header_title . '_search',
              ),
            ),
          );
          // Dinamically add condition to query.
          $query = $query->condition('n.element_value', '%' . db_like(variable_get($header_title . '_search', '')) . '%', 'LIKE');
          if (variable_get($header_title . '_search', '') != '') {
            $query = $query->condition('n.element_label', $header_title, '=');
          }
        }
      }
      $element_value_col = $query->execute()->fetchCol();
      $radio_choices_array = array();
      foreach ($element_value_col as $element_value) {
        if (isset($radio_choices_array[$element_value])) {
          $radio_choices_array[$element_value] += 1; 
        }
        else {
          $radio_choices_array[$element_value] = 1;
        }
      }
      // $id_col = db_query("SELECT id FROM {form_maker_submits_table} WHERE element_label=:element_label AND form_id=:form_id", array(':element_label' => $label_name[0], ':form_id' => $nodeid))->fetchCol();
      $query = db_select('form_maker_submits_table', 'n')
        ->fields('n', array('id'))
        ->condition('n.date', (variable_get('data_search_from', '') == '') ? '1900-01-01' :  variable_get('data_search_from', ''), '>=')
        ->condition('n.date', (variable_get('data_search_to', '') == '') ? '2200-01-01' :  variable_get('data_search_to', ''), '<=')
        ->condition('n.ip', '%' . db_like(variable_get('ip_search', '')) . '%', 'LIKE')
        ->condition('n.element_label', $label_name[0], '=')
        ->condition('n.form_id', $nodeid, '=');
      foreach ($headers as $header_title) {
        if (($header_title != 'id') && ($header_title != 'date') && ($header_title != 'ip')) {
          $options['search_row'][$header_title] = array(
            'data' => array(
              '#type' => 'textfield',
              '#size' => 15,
              '#value' => variable_get($header_title . '_search', ''),
              '#attributes' => array(
                'id' => $header_title . '_search',
                'name' => $header_title . '_search',
              ),
            ),
          );
          // Dinamically add condition to query.
          $query = $query->condition('n.element_value', '%' . db_like(variable_get($header_title . '_search', '')) . '%', 'LIKE');
          if (variable_get($header_title . '_search', '') != '') {
            $query = $query->condition('n.element_label', $header_title, '=');
          }
        }
      }
      $id_col = $query->execute()->fetchCol();
      $total = count($id_col);
      $i = 0;
      $percentage_color = '#2CBADE';
      foreach ($radio_choices_array as $choices_value => $choices_count) {
        if ($choices_value != '#*first_value*#') {
          if ($choices_value == '') {
            $choices_value = t('Unanswered');
          }
          $radio_choices_options[$i . '_type_radio' . $label_name[1]]['choices'] = $choices_value;
          $radio_choices_options[$i . '_type_radio' . $label_name[1]]['percentage'] = '<div style="width:' . ($choices_count * 100) / $total . '%; height:18px; background-color:' . $percentage_color . '; border-radius:10px;"></div>';
          $radio_choices_options[$i . '_type_radio' . $label_name[1]]['count'] = $choices_count;
          $i++;
          if ($percentage_color == '#2CBADE') {
            $percentage_color = '#FE6400';
          }
          else {
            $percentage_color = '#2CBADE';
          }
        }
      }
      $radio_choices_options['total_type_radio' . $label_name[1]]['choices'] = '';
      $radio_choices_options['total_type_radio' . $label_name[1]]['percentage'] = t('Total');
      $radio_choices_options['total_type_radio' . $label_name[1]]['count'] = $total;
      $form['fieldset_form_submissions']['choices_type_radio' . $label_name[1]] = array(
        '#prefix' => '<b>' . $label_name[1] . '</b>',
        '#theme' => 'table',
        '#header' => $choices_header,
        '#rows' => $radio_choices_options,
        '#tree' => TRUE,
      );      $radio_choices_options = array();
    }
  }
  foreach ($exploe_label_order as $label_orders) {
   $label = explode('#**label**#', $label_orders);
    if (isset($label[1]) && ($label[1] == 'type_checkbox')) {
      $label_name = explode('#**id**#', $label[0]);
      // $element_value_col = db_query("SELECT element_value FROM {form_maker_submits_table} WHERE element_label=:element_label AND form_id=:form_id", array(':element_label' => $label_name[0], ':form_id' => $nodeid))->fetchCol();
      $query = db_select('form_maker_submits_table', 'n')
        ->fields('n', array('element_value'))
        ->condition('n.date', (variable_get('data_search_from', '') == '') ? '1900-01-01' :  variable_get('data_search_from', ''), '>=')
        ->condition('n.date', (variable_get('data_search_to', '') == '') ? '2200-01-01' :  variable_get('data_search_to', ''), '<=')
        ->condition('n.ip', '%' . db_like(variable_get('ip_search', '')) . '%', 'LIKE')
        ->condition('n.element_label', $label_name[0], '=')
        ->condition('n.form_id', $nodeid, '=');
      foreach ($headers as $header_title) {
        if (($header_title != 'id') && ($header_title != 'date') && ($header_title != 'ip')) {
          $options['search_row'][$header_title] = array(
            'data' => array(
              '#type' => 'textfield',
              '#size' => 15,
              '#value' => variable_get($header_title . '_search', ''),
              '#attributes' => array(
                'id' => $header_title . '_search',
                'name' => $header_title . '_search',
              ),
            ),
          );
          // Dinamically add condition to query.
          $query = $query->condition('n.element_value', '%' . db_like(variable_get($header_title . '_search', '')) . '%', 'LIKE');
          if (variable_get($header_title . '_search', '') != '') {
            $query = $query->condition('n.element_label', $header_title, '=');
          }
        }
      }
      $element_value_col = $query->execute()->fetchCol();
      $choices_labels = array();
      $choices_count = array();
      $unanswered = 0;
      foreach ($element_value_col as $key1 => $element_value) {
        // Get selected checkboxes labels array for every submission.
        $choices = explode('***br***', $element_value);
        // Delete from end of checkboxes labels array whitespase.
        $choices = array_slice($choices, 0, -1, TRUE);
        foreach ($choices as $key2 => $choice) {
          if (!in_array($choice, $choices_labels)) {
            // Add checkbox new label to checkboxes labels array.
            array_push($choices_labels, $choice);
            array_push($choices_count, 0);
          }
          // Increase existing checkbox label count.
          $choices_count[array_search($choice, $choices_labels)]++;
        }
      }
      array_multisort($choices_count, SORT_DESC, $choices_labels);
      // Add UNANSWERED to checkbox labels if nothing was checked.
      foreach ($element_value_col as $key1 => $element_value) {
        if ($element_value == '') {
          $choices_labels['unanswered'] = t('Unanswered');
          $choices_count['unanswered'] = ++$unanswered;
        }
      }
      $total = count($element_value_col);
      $i = 0;
      $percentage_color = '#2CBADE';
      foreach ($choices_labels as $key => $choices_label) {
        // if ($choices_value != '#*first_value*#') {
        $checkbox_choices_options[$i . '_type_checkbox' . $label_name[1]]['choices'] = str_replace('***br***', '<br />', $choices_label);
        $checkbox_choices_options[$i . '_type_checkbox' . $label_name[1]]['percentage'] = '<div style="width:' . ($choices_count[$key] * 100) / $total . '%; height:18px; background-color:' . $percentage_color . '; border-radius:10px;"></div>';
        $checkbox_choices_options[$i . '_type_checkbox' . $label_name[1]]['count'] = $choices_count[$key];
        $i++;
        if ($percentage_color == '#2CBADE') {
          $percentage_color = '#FE6400';
        }
        else {
          $percentage_color = '#2CBADE';
        }
        // }
      }
      $checkbox_choices_options['total_type_checkbox' . $label_name[1]]['choices'] = '';
      $checkbox_choices_options['total_type_checkbox' . $label_name[1]]['percentage'] = t('Total');
      $checkbox_choices_options['total_type_checkbox' . $label_name[1]]['count'] = $total;
      $form['fieldset_form_submissions']['choices_type_checkbox' . $label_name[1]] = array(
        '#prefix' => '<b>' . $label_name[1] . '</b>',
        '#theme' => 'table',
        '#header' => $choices_header,
        '#rows' => $checkbox_choices_options,
        '#tree' => TRUE,
      );      $checkbox_choices_options = array();
    }
  }
  return $form;
}

/**
 * Add/Remove Columns button function.
 */
function form_maker_add_remove_columns($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/submissions/add_remove', array('absolute' => TRUE));
}

/**
 * Checkboxes form to add/remove columns from submissions table view.
 */
function form_maker_add_remove() {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $value = db_query("SELECT * FROM {form_maker_submits_table} WHERE form_id=:form_id", array(':form_id' => $nodeid));
  $rows = array();
  while ($a = $value->fetchObject()) {
    $rows[] = $a;
  }
  $n = count($rows);
  $labels = array();
  for ($i = 0; $i < $n; $i++) {
    $row = &$rows[$i];
    if (!in_array($row->element_label, $labels)) {
      array_push($labels, $row->element_label);
    }
  }
  $sorted_labels_id = array();
  $sorted_labels = array();
  $label_titles = array();
  if ($labels) {
    $label_id = array();
    $label_order = array();
    $label_order_original = array();
    $label_type = array();
    $this_form = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
    $label_all = explode('#****#', $this_form->label_order);
    $label_all = array_slice($label_all, 0, count($label_all) - 1);
    foreach ($label_all as $key => $label_each) {
      $label_id_each = explode('#**id**#', $label_each);
      array_push($label_id, $label_id_each[0]);
      $label_oder_each = explode('#**label**#', $label_id_each[1]);
      array_push($label_order_original, $label_oder_each[0]);
      $ptn = "/[^a-zA-Z0-9_]/";
      $rpltxt = "";
      $label_temp = preg_replace($ptn, $rpltxt, $label_oder_each[0]);
      array_push($label_order, $label_temp);
      array_push($label_type, $label_oder_each[1]);
    }
    foreach ($label_id as $key => $label) {
      if (in_array($label, $labels)) {
        array_push($sorted_labels, $label_order[$key]);
        array_push($sorted_labels_id, $label);
        array_push($label_titles, $label_order_original[$key]);
      }
    }
    $i = 0;
    foreach ($sorted_labels_id as $idd) {
      $labelll[$idd] = $label_titles[$i];
      $i++;
    }
  }
  $labels_id = $sorted_labels_id;
  drupal_add_js(drupal_get_path('module', 'form_maker') . '/js/form_maker_check_all.js');
  $headers = explode('@@&&@@', variable_get('form_maker_header_array_string', 'none'));
  $form['select_columns'] = array(
      '#type' => 'item',
      '#title' => '<h2>' . t('Select Columns') . '</h2>',
      '#description' => t("Select column, which you want to see."),
    );
  $form['check_all_box'] = array(
      '#type' => 'checkbox',
      '#title' => t('All'),
      '#attributes' => array('onclick' => 'form_maker_check_all("edit-check-all-box", "' . variable_get('form_maker_header_array_string', 'none') . '", "checkbox_");'),
      '#default_value' => variable_get('form_maker_is_checked_all', 1),
    );
  foreach ($headers as $header) {
    if ($header == 'ip') {
      $title = t("Submitter's IP Address");
    }
    elseif ($header == 'date') {
      $title = t('Submit date');
    }
    elseif ($header == 'id') {
      $title = t('ID');
    }
    else {
      $title = $labelll[$header];
    }
    $form['checkbox_' . $header] = array(
      '#type' => 'checkbox',
      '#title' => $title,
      '#default_value' => variable_get('form_maker_is_checked_' . $header, 1),
      '#attributes' => array('id' => 'checkbox_' . $header, 'onclick' => 'form_maker_checked_one("' . $header . '", "' . variable_get('form_maker_header_array_string', 'none') . '");'),
    );
  }
  $form['done'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_done'),
    '#value' => t('Done'),
  );
  $form['cancel_checking'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_cancel_checking'),
    '#value' => t('Cancel'),
  );
  return $form;
}

/**
 * Done add/remove.
 */
function form_maker_done($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $headers = explode('@@&&@@', variable_get('form_maker_header_array_string', 'none'));
  if ($form_state['values']['check_all_box'] == 1) {
    // Checked all headers.
    variable_set('form_maker_is_checked_all', 1);
  }
  else {
    variable_set('form_maker_is_checked_all', 0);
  }
  foreach ($headers as $header) {
    if ($form_state['values']['checkbox_' . $header] == 1) {
      // Check the header.
      variable_set('form_maker_is_checked_' . $header, 1);
    }
    else {
      // Uncheck the header.
      variable_set('form_maker_is_checked_' . $header, 2);
    }
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/submissions', array('absolute' => TRUE));
}

/**
 * Cancel checking columns.
 */
function form_maker_cancel_checking($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/submissions', array('absolute' => TRUE));
}

/**
 * Edit checked submissions.
 */
function form_maker_edit_checked_submissions($form, &$form_state) {
  if (db_query("SELECT group_id FROM {form_maker_submits_table}")) {
    if (arg(0) == 'node' && is_numeric(arg(1))) {
      $nodeid = arg(1);
    }
    $group_id_col = db_query("SELECT group_id FROM {form_maker_submits_table}")->fetchCol();
    $flag = FALSE;
    foreach ($group_id_col as $group_id) {
      if (isset($_POST['submissions_table'][$group_id])) {
        $flag = TRUE;
        $id = db_query("SELECT id FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $group_id))->fetchField();
        $form_state['redirect'] = url('node/' . $nodeid . '/submissions/edit', array('query' => array('id' => $id, 'group_id' => $group_id), 'absolute' => TRUE));
      }
    }
    if ($flag == FALSE) {
      drupal_set_message(t('You must check a submission.'), 'warning', FALSE);
    }
  }
}

/**
 * Edit submissions.
 */
function form_maker_edit_submissions() {
  if (isset($_GET['id'])) {
    $id = check_plain($_GET['id']);
  }
  if (isset($_GET['id'])) {
    $group_id = check_plain($_GET['group_id']);
  }
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $form['fieldset_form_submission_edit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit submission') . ' ' . $id,
  );
  $form['fieldset_form_submission_edit']['edit_submissions_save'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_edit_submissions_save'),
    '#value' => t('Save'),
  );
  $form['fieldset_form_submission_edit']['edit_submissions_apply'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_edit_submissions_apply'),
    '#value' => t('Apply'),
  );
  $form['fieldset_form_submission_edit']['edit_submissions_cancel'] = array(
    '#type' => 'submit',
    '#submit' => array('form_maker_edit_submissions_cancel'),
    '#value' => t('Cancel'),
  );
  $header = array('field' => array('data' => t('Field')), 'value' => array('data' => t('Value')));
  $row = db_query("SELECT * FROM {form_maker_submits_table} WHERE id=:id", array(':id' => $id))->fetchObject();
  $options[0] = array('field' => t('ID :') . ' ', 'value' => $row->id);
  $options[1] = array('field' => t('Submit date :') . ' ', 'value' => $row->date);
  $options[2] = array('field' => t('Submitter\'s IP :') . ' ', 'value' => $row->ip);
  $value = db_query("SELECT * FROM {form_maker_submits_table} WHERE form_id=:form_id", array(':form_id' => $nodeid));
  $rows = array();
  while ($a = $value->fetchObject()) {
    $rows[] = $a;
  }
  $n = count($rows);
  $labels = array();
  for ($i = 0; $i < $n; $i++) {
    $row = &$rows[$i];
    if (!in_array($row->element_label, $labels)) {
      array_push($labels, $row->element_label);
    }
  }
  $sorted_labels_id = array();
  $sorted_labels = array();
  $label_titles = array();
  if ($labels) {
    $label_id = array();
    $label_order = array();
    $label_order_original = array();
    $label_type = array();

    $this_form = db_query("SELECT * FROM {form_maker_table} WHERE vid=:vid", array(':vid' => $nodeid))->fetchObject();
    $label_all = explode('#****#', $this_form->label_order);
    $label_all = array_slice($label_all, 0, count($label_all) - 1);
    foreach ($label_all as $key => $label_each) {
      $label_id_each = explode('#**id**#', $label_each);
      array_push($label_id, $label_id_each[0]);
      $label_oder_each = explode('#**label**#', $label_id_each[1]);
      array_push($label_order_original, $label_oder_each[0]);
      $ptn = "/[^a-zA-Z0-9_]/";
      $rpltxt = "";
      $label_temp = preg_replace($ptn, $rpltxt, $label_oder_each[0]);
      array_push($label_order, $label_temp);
      array_push($label_type, $label_oder_each[1]);
    }
    foreach ($label_id as $key => $label) {
      if (in_array($label, $labels)) {
        array_push($sorted_labels, $label_order[$key]);
        array_push($sorted_labels_id, $label);
        array_push($label_titles, $label_order_original[$key]);
      }
    }
    $i = 0;
    foreach ($sorted_labels_id as $idd) {
      $labelll[$idd] = $label_titles[$i];
      $i++;
    }
  }
  $labels_id = $sorted_labels_id;
  $element_labels = db_query("SELECT element_label FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $group_id))->fetchCol();
  foreach ($element_labels as $key => $element_label) {
    $element_value = db_query("SELECT element_value FROM {form_maker_submits_table} WHERE group_id=:group_id and element_label=:element_label", array(':group_id' => $group_id, ':element_label' => $element_label))->fetchField();
    // Don't show map field on submission edit page.
    if (!strpos($element_value, '***map***')) {
      if ($element_value == '::' || $element_value == '--') {
        $element_value = '';
      }
      $options[$labelll[$element_label]] = array('field' => $labelll[$element_label], 'value' => '<input class="form-text" type="textfield" name="' . $element_label . '_lid" id="' . $element_label . '_lid" value ="' . $element_value . '">');
    }
  }
  $form['fieldset_form_submission_edit']['submission_fields_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#tree' => TRUE,
  );
  foreach ($options as $key => $value) {
    $form['fieldset_form_submission_edit']['submission_fields_table'][$key]['#disabled'] = TRUE;
  }
  return $form;
}

/**
 * Update submissions.
 */
function form_maker_edit_submissions_save($form, &$form_state) {
  if (isset($_GET['id'])) {
    $id = check_plain($_GET['id']);
  }
  if (isset($_GET['id'])) {
    $group_id = check_plain($_GET['group_id']);
  }
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $element_labels = db_query("SELECT element_label FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $group_id))->fetchCol();
  foreach ($element_labels as $key => $element_label) {
    db_query("UPDATE {form_maker_submits_table} SET element_value=:element_value where element_label=:element_label and group_id=:group_id", array(
      ':element_value' => check_plain($_POST["" . $element_label . "_lid"]),
      ':element_label' => $element_label,
      ':group_id' => $group_id,
      ));
  }
  drupal_set_message(t('Submission @get_submission_id Successfully Saved', array('@get_submission_id' => $id)), 'status', FALSE);
  $form_state['redirect'] = url('node/' . $nodeid . '/submissions', array('absolute' => TRUE));
}

/**
 * Apply submissions.
 */
function form_maker_edit_submissions_apply($form, &$form_state) {
  if (isset($_GET['id'])) {
    $id = check_plain($_GET['id']);
  }
  if (isset($_GET['id'])) {
    $group_id = check_plain($_GET['group_id']);
  }
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $element_labels = db_query("SELECT element_label FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $group_id))->fetchCol();
  foreach ($element_labels as $key => $element_label) {
    db_query("UPDATE {form_maker_submits_table} SET element_value=:element_value where element_label=:element_label and group_id=:group_id", array(
      ':element_value' => check_plain($_POST["" . $element_label . "_lid"]),
      ':element_label' => $element_label,
      ':group_id' => $group_id,
      ));
  }
  drupal_set_message(t('Submission @get_submission_id Successfully Saved', array('@get_submission_id' => $id)), 'status', FALSE);
}

/**
 * Cancel editing submission.
 */
function form_maker_edit_submissions_cancel($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/submissions', array('absolute' => TRUE));
}

/**
 * Delete selected submissions.
 */
function form_maker_delete_submissions($form, &$form_state) {
  if (db_query("SELECT group_id FROM {form_maker_submits_table}")) {
    $group_id_col = db_query("SELECT group_id FROM {form_maker_submits_table}")->fetchCol();
    $flag = FALSE;
    foreach ($group_id_col as $group_id) {
      if (isset($_POST['submissions_table'][$group_id])) {
        $flag = TRUE;
        db_query("DELETE FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $group_id));
        drupal_set_message(t('Submission successfully deleted.'), 'status', FALSE);
      }
    }
    if ($flag == FALSE) {
      drupal_set_message(t('You must check a submission.'), 'warning', FALSE);
    }
  }
}

/**
 * Delete submission.
 */
function form_maker_delete_submission() {
  if (isset($_GET['id'])) {
    $id = check_plain($_GET['id']);
  }
  if (isset($_GET['id'])) {
    $group_id = check_plain($_GET['group_id']);
  }
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  db_query("DELETE FROM {form_maker_submits_table} WHERE group_id=:group_id", array(':group_id' => $group_id));
  drupal_goto(url('node/' . $nodeid . '/submissions', array('absolute' => TRUE)));
  drupal_set_message(t('Submission successfully deleted.'), 'status', FALSE);
}

/**
 * Search submissions.
 */
function form_maker_go_button($form, &$form_state) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  if (isset($_POST['data_search_from']) && (check_plain($_POST['data_search_from']) != '')) {
    variable_set('data_search_from', check_plain($_POST['data_search_from']));
    variable_set('search_on', 1);
  }
  else {
    variable_set('data_search_from', '');
    // variable_set('data_search_from', '1900-01-01');
  }
  if (isset($_POST['data_search_to']) && (check_plain($_POST['data_search_to']) != '')) {
    variable_set('data_search_to', check_plain($_POST['data_search_to']));
    variable_set('search_on', 1);
  }
  else {
    variable_set('data_search_to', '');
    // variable_set('data_search_to', '2200-01-01');
  }
  if (isset($_POST['ip_search'])) {
    variable_set('ip_search', check_plain($_POST['ip_search']));
    variable_set('search_on', 1);
  }
  else {
    variable_set('ip_search', '');
  }
  $headers = explode('@@&&@@', variable_get('form_maker_header_array_string', 'none'));
  foreach ($headers as $header_title) {
    if (($header_title != 'date') && ($header_title != 'ip') && ($header_title != 'id')) {
      if (isset($_POST[$header_title . '_search'])) {
        variable_set($header_title . '_search', check_plain($_POST[$header_title . '_search']));
        variable_set('search_on', 1);
      }
      else {
        variable_set($header_title . '_search', '');
      }
    }
  }
  $form_state['redirect'] = url('node/' . $nodeid . '/submissions', array('absolute' => TRUE));
}

/**
 * Cancel submissions search.
 */
function form_maker_reset_button($form, &$form_state) {
  variable_del('data_search_from');
  variable_del('data_search_to');
  variable_del('ip_search');
  $headers = explode('@@&&@@', variable_get('form_maker_header_array_string', 'none'));
  foreach ($headers as $header_title) {
    if (($header_title != 'date') && ($header_title != 'ip') && ($header_title != 'id')) {
      variable_del($header_title . '_search');
    }
  }
  variable_del('search_on');
}