<?php
function manage_multiple_terms_get_vocabularies() {
  $vocabularies = db_select('taxonomy_vocabulary', 't')->fields('t')->execute()->fetchAll();
  return $vocabularies;
}

function manage_multiple_terms_form($form_state) {
  drupal_add_css(drupal_get_path('module', 'manage_multiple_terms') . '/manage_multiple_terms.css');
  drupal_add_js(drupal_get_path('module', 'manage_multiple_terms') . '/manage_multiple_terms.js');    
  $vocabularies = manage_multiple_terms_get_vocabularies();

  $form['#vocabularies_list'] = $vocabularies;
  $form['#tree'] = TRUE;
  $form['#parent_fields'] = TRUE;

  foreach($vocabularies as $vocabulary) {
    $terms_list = array();
    $vid = $vocabulary->vid;
    $machine_name = $vocabulary->machine_name;
    $terms = taxonomy_get_tree($vid);
    $list_name = $machine_name . "_items";
    $first_id = $machine_name . "_first_id";

    foreach($terms as $term) {
      $key = $term->tid;
      $terms_list[$key] = $term;      
    }
    if(count($terms)) {
      $form['#' . $first_id] = $terms[0]->tid;
    }

    if (!empty($form_state['input'])) {
      $order = array_flip(array_keys($form_state['input']));
      $terms_list = array_merge($order, $terms_list);
      foreach ($terms_list as $key => $term) {
        if (is_array($form_state['input'][$list_name][$key]) && is_numeric($form_state['input'][$list_name][$key]['tid'])) {
          $terms_list[$list_name][$key]->depth = $form_state['input'][$list_name][$key]['depth'];
        }
        else {
          unset($terms_list[$list_name][$key]);
        }
      }
    }
    $form[$list_name] = array();

    foreach ($terms_list as $key => $term) {
      $form[$list_name][$key]['#term'] = (array) $term;
      if (isset($term->parents)) {
        $form[$list_name][$key]['#term']['parent'] = $term->parent = $term->parents[0];
        unset($form[$list_name][$key]['#term']['parents'], $term->parents);
      }

      $form[$list_name][$key]['view'] = array('#type' => 'link', '#title' => $term->name, '#href' => "taxonomy/term/$term->tid");
        $form['#parent_fields'] = TRUE;
        $form[$list_name][$key]['tid'] = array(
          '#type' => 'hidden',
          '#value' => $term->tid
        );
        $form[$list_name][$key]['name'] = array(
          '#type' => 'textfield',
          '#default_value' => check_plain($term->name),
          '#size' => 20,
          '#maxlength' => 255,
          '#attributes' => array('class' => array('deletable-field')),
        );
        $form[$list_name][$key]['description'] = array(
          '#type' => 'textfield',
          '#default_value' => $term->description,
          '#size' => 50,
          '#maxlength' => 1000,
          '#attributes' => array('class' => array('deletable-field')),
        );        
        $form[$list_name][$key]['parent'] = array(
          '#type' => 'hidden',
          '#default_value' => $term->parent,
        );
        $form[$list_name][$key]['depth'] = array(
          '#type' => 'hidden',
          '#default_value' => $term->depth,
        );
        $form[$list_name][$key]['weight'] = array(
          '#type' => 'weight',
          '#title_display' => 'invisible',
          '#title' => t('Weight for added term'),
          '#default_value' => $term->weight,
        );
        $form[$list_name][$key]['delete'] = array(
          '#markup' => '<a class="delete-term-button" href="#">delete</a>',
        );
      $form[$list_name][$key]['edit'] = array('#type' => 'link', '#title' => t('edit detail'), '#href' => 'taxonomy/term/' . $term->tid . '/edit', '#options' => array('query' => drupal_get_destination()));
    }
    $form[$list_name . '[null][view][]'] = array(
      '#markup' => t('New Term'),
    );
    $form[$list_name . '[null][name][]'] = array(
      '#type' => 'textfield',
      '#default_value' => '',
      '#size' => 20,
      '#maxlength' => 255,
    );
    $form[$list_name . '[null][description][]'] = array(
      '#type' => 'textfield',
      '#default_value' => '',
      '#size' => 50,
    );
    $form[$list_name . '[null][weight][]'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => 1000,
      '#delta' => 10,
      '#title_display' => 'invisible',
      '#attributes' => array('class' => array('edit-weight', 'hidden')),
    );
    $form[$list_name . '[null][delete][]'] = array(
      '#markup' => '<a class="delete-term-button" href="#">&nbsp;</a>',
    );
    $form['#empty_text'] = t('No terms available. <a href="@link">Add term</a>.', array('@link' => url('admin/structure/taxonomy/' . $vocabulary->machine_name . '/add')));
    if ($vocabulary->hierarchy < 2 && count($terms) > 1) {
      $form['actions'] = array('#type' => 'actions', '#tree' => FALSE);
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save')
      );
      $form['actions']['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('manage_multiple_terms_form_cancel'),
      );
      $form_state['redirect'] = array($_GET['q'], (isset($_GET['page']) ? array('query' => array('page' => $_GET['page'])) : array()));
    }
  }
  
  return $form;
}

function manage_multiple_terms_form_cancel($form_state) {
  
}

function theme_manage_multiple_terms_form($variables) {
  $form = $variables['form'];

  $errors = form_get_errors() != FALSE ? form_get_errors() : array();
  $vocabularies = $form['#vocabularies_list'];
  $output = '<a href="" class="expanded-all" id="collapse-expand-all">' . t("Collapse All") . '</a>';
  foreach($vocabularies as $vocabulary) {
    $vid = $vocabulary->vid;
    $machine_name = $vocabulary->machine_name;
    $list_name = $machine_name . "_items";
    $first_id = $machine_name . "_first_id";
    $item_weight_name = $machine_name . "_item_weight";  
    $rows = array();
    $rows[] = array(
      'data' => array(
        array('data' => t('Term Name'),'class' => 'term-name'),
        array('data' => t('Edit Name'), 'class' => 'edit-name'),
        array('data' => t('Edit Description'), 'class' => 'edit-description'),
        array('data' => t('Weight'), 'class' => 'edit-weight'),
        array('data' => t('Edit Detail'), 'class' => 'edit-detail-link'),
        array('data' => t('Delete'), 'class' => 'delete-link'),
      ),
      'class' => array('row-header'),
    );
    foreach (element_children($form[$list_name]) as $key) {
      if (isset($form[$list_name][$key]['#term'])) {
        $term = &$form[$list_name][$key];

        $row = array();
        $row[] = (isset($term['#term']['depth']) && $term['#term']['depth'] > 0 ? theme('indentation', array('size' => $term['#term']['depth'])) : '') . drupal_render($term['view']);
        if ($form['#parent_fields']) {
          $term['tid']['#attributes']['class'] = array('term-id');
          $term['parent']['#attributes']['class'] = array('term-parent');
          $term['depth']['#attributes']['class'] = array('term-depth');
          $row[0] .= drupal_render($term['parent']) . drupal_render($term['tid']) . drupal_render($term['depth']);
        }
        $term['weight']['#attributes']['class'] = array('term-weight');
        $row[] = drupal_render($term['name']);
        $row[] = drupal_render($term['description']);
        $row[] = drupal_render($term['weight']);
        $row[] = drupal_render($term['edit']);
        $row[] = drupal_render($term['delete']);
        $rows[$key] = array(
          'data' => $row,
          'class' => array('draggable', 'vocabulary-items-table'),
        );
      }
    }
 
    $form[$list_name]['null']['weight']['#attributes']['class'] = array($item_weight_name, 'hidden');
    $rows[] = array(
      'data' => array(
        drupal_render($form[$list_name . '[null][view][]']),
        drupal_render($form[$list_name . '[null][name][]']),
        drupal_render($form[$list_name . '[null][description][]']),
        array('data' => drupal_render($form[$list_name . '[null][weight][]']), 'class' => 'edit-weight hidden'),
        '',
        drupal_render($form[$list_name . '[null][delete][]']),
      ),
      'class' => array('term-row-template'),
    );

    $table_id = $machine_name . "_table";
    $header = array(array('data' => $vocabulary->name . '(<a href="#" class="collapse-expand-button">' . t("Collapse") .'</a>)<a href="#" class="add-term-button">add term</a>', 'colspan' => '5'));
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => $table_id,
        'class' => array('manage-multiple-terms-table', 'collapsible', 'expanded'),
      )
    ));

    if ($form['#parent_fields']) {
      drupal_add_tabledrag($table_id, 'match', 'parent', 'term-parent', 'term-parent', 'term-id', FALSE);
      drupal_add_tabledrag($table_id, 'depth', 'group', 'term-depth', NULL, NULL, FALSE);
      drupal_add_js(drupal_get_path('module', 'taxonomy') . '/taxonomy.js');
      drupal_add_css(drupal_get_path('module', 'taxonomy') . '/taxonomy.css');
    }
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'term-weight');
  }
  $output .= drupal_render_children($form);
  return $output;
}

function manage_multiple_terms_form_submit($form, &$form_state) {
  $vocabularies = manage_multiple_terms_get_vocabularies();
  $input = $form_state['input'];
  $values = $form_state['values'];
  foreach($vocabularies as $vocabulary) {
    $vid = $vocabulary->vid;
    $machine_name = $vocabulary->machine_name;
    $list_name = $machine_name . "_items";
    $first_id = $machine_name . "_first_id";
    $item_weight_name = $machine_name . "_item_weight";
    if(!empty($form_state['values'][$list_name])) {
      uasort($form_state['values'][$list_name], 'drupal_sort_weight');
    }
    $hierarchy = 0;
    $changed_terms = array();
    $terms = taxonomy_get_tree($vocabulary->vid);

    if (empty($terms)) {
      continue;
    }

    $weight = 0;
    $term = (array) $terms[0];
    while ($term['tid'] != $form['#' . $first_id]) {
      if ($term['parents'][0] == 0 && $term['weight'] != $weight) {
        $term['parent'] = $term['parents'][0];
        $term['weight'] = $weight;
        $changed_terms[$term['tid']] = $term;
      }
      $weight++;
      $hierarchy = $term['parents'][0] != 0 ? 1 : $hierarchy;
      $term = (array) $terms[$weight];
    }
    $level_weights = array();
    foreach ($form_state['values'][$list_name] as $tid => $values) {
      if (isset($form[$list_name][$tid]['#term'])) {
        $term = $form[$list_name][$tid]['#term'];
        if ($values['parent'] == 0 && $term['weight'] != $weight) {
          $term['weight'] = $weight;
          $changed_terms[$term['tid']] = $term;
        }
        elseif ($values['parent'] > 0) {
          $level_weights[$values['parent']] = isset($level_weights[$values['parent']]) ? $level_weights[$values['parent']] + 1 : 0;
          if ($level_weights[$values['parent']] != $term['weight']) {
            $term['weight'] = $level_weights[$values['parent']];
            $changed_terms[$term['tid']] = $term;
          }
        }
        if ($values['parent'] != $term['parent']) {
          $term['parent'] = $values['parent'];
          $changed_terms[$term['tid']] = $term;
        }
        $hierarchy = $term['parent'] != 0 ? 1 : $hierarchy;
        $weight++;
      }
    }

    for ($weight; $weight < count($terms); $weight++) {
      $term = (array) $terms[$weight];
      if ($term['parents'][0] == 0 && $term['weight'] != $weight) {
        $term['parent'] = $term['parents'][0];
        $term['weight'] = $weight;
        $changed_terms[$term['tid']] = $term;
      }
      $hierarchy = $term['parents'][0] != 0 ? 1 : $hierarchy;
    }

    foreach ($changed_terms as $changed) {
      $term = (object) $changed;
      db_update('taxonomy_term_hierarchy')
        ->fields(array('parent' => $term->parent))
        ->condition('tid', $term->tid, '=')
        ->execute();

      db_update('taxonomy_term_data')
        ->fields(array('weight' => $term->weight))
        ->condition('tid', $term->tid, '=')
        ->execute();
    }

    if ($vocabulary->hierarchy != $hierarchy) {
      $vocabulary->hierarchy = $hierarchy;
      taxonomy_vocabulary_save($vocabulary);
    }
  }
  $input = $form_state['input'];
  $values = $form_state['values'];
  foreach($vocabularies as $vocabulary) {
    $list_name = $vocabulary->machine_name . "_items";
    if(isset($values[$list_name])) {
      foreach($values[$list_name] as $tid => $info) {
        if($tid !== 'null') {
          $term = taxonomy_term_load($tid);
          if($info['name'] == '') {
            print_r($info);
            taxonomy_term_delete($term->tid);
          }
          else {
            $term->name = $info['name'];
            $term->description = $info['description'];
            $term->weight = $info['weight'];
            taxonomy_term_save($term);
          }
        }
      }
    }
    if(isset($input[$list_name])) {
      foreach($input[$list_name]['null']['name'] as $key => $value) {
        if($value !== '') {
          $term = new stdClass();
          $term->name = $value;
          $term->vid = $vocabulary->vid;
          $term->description = $input[$list_name]['null']['description'][$key];
          $term->weight = $input[$list_name]['null']['name'][$key];
          taxonomy_term_save($term);
        }
      }
    }    
  }
  drupal_set_message(t('The configuration options have been saved.'));
}
