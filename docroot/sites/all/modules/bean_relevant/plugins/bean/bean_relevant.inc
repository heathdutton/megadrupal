<?php
/**
 * @file
 * Bean: Relevant Content bean plugin.
 */

class RelevantContentBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'display_options' => array(),
      'filter_options' => array(),
    );
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    // Get the path to the module
    $path = drupal_get_path('module', 'bean_relevant');

    $form['header'] = array(
      '#type' => 'fieldset',
      '#tree' => FALSE,
      '#title' => t('Display Relevant Content'),
    );

    $form['header']['display_options'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Display Options'),
    );

    $form['header']['display_options']['num_posts'] = array(
      '#type' => 'select',
      '#title' => t('Number of nodes'),
      '#description' => t('Maximum number of nodes.'),
      '#options' => drupal_map_assoc(range(1, 20)),
      '#default_value' => isset($bean->display_options['num_posts']) ?
        $bean->display_options['num_posts'] : 3,
    );

    $options['title'] = t('Linked Title');
    $entity_info = entity_get_info();
    foreach ($entity_info['node']['view modes'] as $key => $value) {
      $options[$key] = $value['label'];
    }

    $form['header']['display_options']['node_display'] = array(
      '#type' => 'select',
      '#title' => t('Node display'),
      '#description' => t('Display listed nodes in this format.'),
      '#options' => $options,
      '#default_value' => isset($bean->display_options['node_display']) ?
        $bean->display_options['node_display'] : 'title',
    );

    $form['header']['display_options']['order_on'] = array(
      '#type' => 'select',
      '#title' => t('Order on'),
      '#description' => t('Display listed nodes ordered on this property.
      If "Any date option" is selected, select the sort field for each content
      type.'),
      '#options' => array(
        'created' => 'Created',
        'changed' => 'Updated',
        'any' => 'Any date option'
      ),
      '#default_value' => isset($bean->display_options['node_display']) ?
        $bean->display_options['order_on'] : 'default',
    );

    $date_fields = array();
    $node_info = array();
    $node_data = entity_get_info('node');

    foreach (field_info_instances('node') as $node => $fields) {
      $node_name = $node;
      $date_fields[$node_name] = array();
      $node_info[$node_name] = $node_data['bundles'][$node]['label'];
      $date_fields[$node_name]['created'] = 'Created';
      $date_fields[$node_name]['changed'] = 'Updated';

      foreach ($fields as $field) {
        $field_info = field_info_field($field['field_name']);
        if ($field_info['type'] == 'datetime') {
          $date_fields[$node_name][$field['field_name']] = $field['label'] .
            ' (' . $field['field_name'] . ')';
        }
      }
    }

    foreach ($date_fields as $key => $date_field) {
      $form['header']['display_options']['dates'][$key] = array(
        '#type' => 'radios',
        '#title' => $node_info[$key] . ' (' . $key . ')',
        '#options' => $date_field,
        '#default_value' => isset($bean->display_options['dates'][$key]) ?
          $bean->display_options['dates'][$key] : 'created',
        '#states' => array(
          'visible' => array(
            ':input[name="display_options[order_on]"]' => array('value' => 'any'),
          ),
        ),
      );
    }

    $form['header']['display_options']['sticky'] = array(
      '#type' => 'checkbox',
      '#title' => t('Order by \'Sticky\'.'),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes')
      ),
      '#default_value' => isset($bean->display_options['sticky']) ?
        $bean->display_options['sticky'] : 0,
    );

    $form['header']['display_options']['order'] = array(
      '#type' => 'select',
      '#title' => t('Node order'),
      '#description' => t('Display listed nodes in this order.'),
      '#options' => array(
        'ASC' => t('Ascending'),
        'DESC' => t('Descending')
      ),
      '#default_value' => isset($bean->display_options['order']) ?
        $bean->display_options['order'] : 'default',
    );

    $form['header']['filter_options'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Filter Options'),
    );

    $form['header']['filter_options']['content'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Content'),
    );

    $form['header']['filter_options']['content']['exclude_current'] = array(
      '#type' => 'select',
      '#title' => t('Exclude current node?'),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes')
      ),
      '#default_value' => isset($bean->filter_options['content']['exclude_current']) ?
        $bean->filter_options['content']['exclude_current'] : 1,
    );

    $form['header']['filter_options']['content']['use_nodes'] = array(
      '#type' => 'select',
      '#title' => t('Filter on content type?'),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes')
      ),
      '#default_value' => isset($bean->filter_options['content']['use_nodes']) ?
        $bean->filter_options['content']['use_nodes'] : 0,
    );

    $form['header']['filter_options']['content']['current_node'] = array(
      '#type' => 'select',
      '#title' => t('Filter on same content type as current node?'),
      '#options' => array(
        1 => t('Yes'),
        0 => t('No, select content types')
      ),
      '#default_value' => isset($bean->filter_options['content']['current_node']) ?
        $bean->filter_options['content']['current_node'] : 1,
      '#states' => array(
        'visible' => array(
          ':input[name="filter_options[content][use_nodes]"]' => array('value' => 1),
        ),
      ),
    );

    //Get Node types
    foreach (node_type_get_types() as $node_type) {
      $node_types[$node_type->type] = $node_type->name;
    }

    $form['header']['filter_options']['content']['content_types'] = array(
      '#type' => 'select',
      '#title' => t('Which content types?'),
      '#options' => $node_types,
      '#multiple' => TRUE,
      '#default_value' => isset($bean->filter_options['content']['content_types']) ?
        $bean->filter_options['content']['content_types'] : 0,
      '#states' => array(
        'visible' => array(
          ':input[name="filter_options[content][current_node]"]' =>
          array('value' => (string) 0),
          ':input[name="filter_options[content][use_nodes]"]' =>
          array('value' => (string) 1),
        ),
      ),
    );

    $form['header']['filter_options']['content']['relative_filter'] = array(
      '#type' => 'select',
      '#title' => t('Date filter?'),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes')
      ),
      '#default_value' => isset($bean->filter_options['content']['relative_filter']) ?
        $bean->filter_options['content']['relative_filter'] : 0,
    );

    foreach ($date_fields as $key => $date_field) {
      $form['header']['filter_options']['content']['relative_filter_fields'][$key] =
        array(
          '#type' => 'radios',
          '#title' => $node_info[$key] . ' (' . $key . ')',
          '#options' => $date_field,
          '#default_value' => isset($bean->filter_options['content']['relative_filter_fields'][$key]) ?
            $bean->filter_options['content']['relative_filter_fields'][$key] : 'created',
          '#states' => array(
            'visible' => array(
              ':input[name="filter_options[content][relative_filter]"]' => array('value' => 1),
            ),
          ),
        );
    }

    $form['header']['filter_options']['content']['relative_filter_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Date greater than'),
      '#description' =>
      t('Relative dates are computed when the view is displayed.
        Examples: now, now +1 day, 12AM today, Monday next week. </br>') .
        l(t('More examples of relative date formats in the PHP documentation.'),
          'http://www.php.net/manual/en/datetime.formats.relative.php'),
      '#default_value' => isset($bean->filter_options['content']['relative_filter_value'][$key]) ?
        $bean->filter_options['content']['relative_filter_value'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="filter_options[content][relative_filter]"]' => array('value' => 1),
        ),
      ),
    );

    if (module_exists('taxonomy')) {
      $form['header']['filter_options']['taxonomy'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#title' => t('Taxonomy'),
      );

      $form['header']['filter_options']['taxonomy']['use_vocab'] = array(
        '#type' => 'select',
        '#title' => t('Filter on taxonomy?'),
        '#options' => array(
          0 => t('No'),
          1 => t('Yes')
        ),
        '#default_value' => isset($bean->filter_options['taxonomy']['use_vocab']) ?
          $bean->filter_options['taxonomy']['use_vocab'] : 0,
      );

      $term_depth_options = array_merge(array('' => 'Unlimited'), drupal_map_assoc(range(0, 10)));
      $form['header']['filter_options']['taxonomy']['term_depth'] = array(
        '#type' => 'select',
        '#title' => t('Term depth'),
        '#description' => t("Select the depth of terms to be filtered contextually.
          Select '0' to only return nodes tagged with specific terms and not child terms."),
        '#options' => $term_depth_options,
        '#states' => array(
          'visible' => array(
            ':input[name="filter_options[taxonomy][use_vocab]"]' => array('value' => 1),
          ),
        ),
      );

      $form['header']['filter_options']['taxonomy']['contextual'] = array(
        '#type' => 'select',
        '#title' => t('Filter by taxonomy terms contextually?'),
        '#options' => array(
          0 => t('No'),
          1 => t('Yes')
        ),
        '#default_value' => isset($bean->filter_options['taxonomy']['contextual']) ?
          $bean->filter_options['taxonomy']['contextual'] : 0,
        '#states' => array(
          'visible' => array(
            ':input[name="filter_options[taxonomy][use_vocab]"]' => array('value' => (string) 1),
          ),
        ),
      );

      $vocabs = taxonomy_get_vocabularies();

      $vocab_options = array();
      foreach ($vocabs as $vocab) {
        $vocab_options[$vocab->vid] = $vocab->name;
      }

      $form['header']['filter_options']['taxonomy']['context_vocab'] = array(
        '#type' => 'checkboxes',
        '#title' => 'Vocabularies',
        '#description' => t("Choose vocabularies to filter on, leave blank for all.
          Filter will return nodes that have term(s) in any of these vocabularies."),
        '#options' => $vocab_options,
        '#default_value' => (isset($bean->filter_options['taxonomy']['context_vocab'])) ?
          $bean->filter_options['taxonomy']['context_vocab'] : array(0 => 0),
        '#states' => array(
          'visible' => array(
            ':input[name="filter_options[taxonomy][use_vocab]"]' => array(
              'value' => (string) 1
            ),
            ':input[name="filter_options[taxonomy][contextual]"]' => array(
              'value' => (string) 1
            ),
          ),
        ),
      );

      $form['header']['filter_options']['taxonomy']['vocab'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#title' => t('Vocabularies'),
        '#states' => array(
          'visible' => array(
            ':input[name="filter_options[taxonomy][use_vocab]"]' => array(
              'value' => (string) 1
            ),
            ':input[name="filter_options[taxonomy][contextual]"]' => array(
              'value' => (string) 0
            ),
          ),
        ),
      );

      foreach ($vocabs as $vocab) {

        $form['header']['filter_options']['taxonomy']['vocab'][$vocab->vid] = array(
          '#type' => 'checkbox',
          '#title' => $vocab->name,
          '#machine_name' => $vocab->machine_name,
          '#options' => array(0 => t('Closed'), 1 => t('Active')),
          '#default_value' => (isset($bean->filter_options['taxonomy']['vocab'][$vocab->vid])
            && $bean->filter_options['taxonomy']['vocab'][$vocab->vid] == 1) ?
            $bean->filter_options['taxonomy']['vocab'][$vocab->vid] : 0,
          '#ajax' => array(
            'callback' => 'bean_relevant_terms_callback',
            'wrapper' => 'bean_relevant_' . $vocab->vid . '_terms_container',
            'method' => 'replace',
            'effect' => 'fade',
            'progress' => array(
              'type' => 'throbber',
              'message' => t('Getting Terms.')
            )
          )
        );
        $form['header']['filter_options']['taxonomy']['vocab'][$vocab->vid . 'terms'] = array(
          '#prefix' => '<div id="bean_relevant_' . $vocab->vid . '_terms_container">',
          '#suffix' => '</div>'
        );

        $display = FALSE;
        if (isset($bean->filter_options['taxonomy']['vocab'][$vocab->vid]) &&
          $bean->filter_options['taxonomy']['vocab'][$vocab->vid] == 1
        ) {
          $display = TRUE;
          if (isset($form_state['values']['filter_options']['taxonomy']['vocab'][$vocab->vid])) {
            if ($form_state['values']['filter_options']['taxonomy']['vocab'][$vocab->vid] == 1) {
              $display = TRUE;
            }
          }
        }
        else {
          if (isset($form_state['values']['filter_options']['taxonomy']['vocab'][$vocab->vid])) {
            if ($form_state['values']['filter_options']['taxonomy']['vocab'][$vocab->vid] == 1) {
              $display = TRUE;
            }
          }
        }

        if ($display) {
          $form['header']['filter_options']['taxonomy']['vocab'][$vocab->vid . 'terms'] = array(
            '#type' => 'fieldset',
            '#title' => t('Select Terms'),
            '#collapsible' => FALSE,
            '#id' => 'bean_relevant_' . $vocab->vid . '_terms_container'
          );

          $term_options = array();
          $term_tree = taxonomy_get_tree($vocab->vid, 0);
          foreach ($term_tree as $key => $term) {
            $term_options[$term->tid] = str_repeat('-', $term->depth) . $term->name;
          }

          $form['header']['filter_options']['taxonomy']['vocab'][$vocab->vid . 'terms']['termid'] = array(
            '#type' => 'select',
            '#title' => t($vocab->name . ' Taxonomy'),
            '#options' => $term_options,
            '#multiple' => TRUE,
            '#default_value' => isset($bean->filter_options['taxonomy']['vocab'][$vocab->vid . 'terms']['termid'])
              ? $bean->filter_options['taxonomy']['vocab'][$vocab->vid . 'terms']['termid'] : '',
          );
        }
      }
    }
    else {
      $form['header']['filter_options']['taxonomy']['use_vocab'] = array(
        '#type' => 'hidden',
        '#title' => t('Filter on taxonomy?'),
        '#default_value' => isset($bean->filter_options['taxonomy']['use_vocab']) ?
          $bean->filter_options['taxonomy']['use_vocab'] : 0,
      );
    }

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    $node = menu_get_object();
    $data = new EntityFieldQueryRelevant();
    $result = $data
      ->entityCondition('entity_type', 'node')
      ->range(0, $bean->display_options['num_posts']);

    if ($bean->display_options['order_on'] != 'any') {
      $result->propertyOrderBy($bean->display_options['order_on'],
        $bean->display_options['order']);
    }
    else {
      $fields = array();
      $datesorts = $bean->display_options['dates'];
      foreach ($datesorts as $node_type => $field_name) {
        if ($field_name != 'changed' && $field_name != 'created') {
          $field_data = field_info_field($field_name);
          if ($field_data) {
            $field_table = array_keys($field_data['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
            $field_name = $field_data['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$field_table[0]]['value'];
            $fields[$node_type] = array(
              'table' => $field_table[0],
              'field' => $field_name
            );
          }
          else {
            // if field is no longer available, default to created
            $fields[$node_type] = array(
              'table' => 'node',
              'field' => 'created'
            );
          }
        }
        else {
          $fields[$node_type] = array(
            'table' => 'node',
            'field' => $field_name
          );
        }
      }

      $result->setSortFields($fields);
      $result->setSortFieldsOrder($bean->display_options['order']);
    }

    $relfields = array();
    $relativedates = $bean->filter_options['content']['relative_filter_fields'];
    foreach ($relativedates as $node_type => $field_name) {
      if ($field_name != 'changed' && $field_name != 'created') {
        $field_data = field_info_field($field_name);
        if ($field_data) {
          $field_table = array_keys($field_data['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
          $field_name = $field_data['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$field_table[0]]['value'];
          $relfields[$node_type] = array(
            'table' => $field_table[0],
            'field' => $field_name
          );
        }
        else {
          // if field is no longer available, default to created
          $relfields[$node_type] = array(
            'table' => 'node',
            'field' => 'created'
          );
        }
      }
      else {
        $relfields[$node_type] = array(
          'table' => 'node', 'field' => $field_name
        );
      }
    }

    $result->setRelativeFilterFields($relfields);
    $result->setRelativeFilterValue($bean->filter_options['content']['relative_filter_value']);

    if ($bean->display_options['sticky'] == 1) {
      $result->propertyOrderBy('sticky', 'DESC');
    }

    if ($bean->filter_options['content']['exclude_current'] && $node) {
      $result->propertyCondition('nid', $node->nid, '<>');
    }

    if ($bean->filter_options['content']['use_nodes'] == 1) {
      switch ($bean->filter_options['content']['current_node']) {
        case 1:
          $bundles = ($node) ? array($node->type) : array(0);
          break;

        case 0:
          $bundles = array_keys($bean->filter_options['content']['content_types']);
          break;
      }

      $result->entityCondition('bundle', $bundles, 'IN');
    }

    if ($bean->filter_options['taxonomy']['use_vocab'] == 1 && module_exists('taxonomy')) {
      $term_nodes = array();

      switch ($bean->filter_options['taxonomy']['contextual']) {
        case 1:
          $vid_options = $bean->filter_options['taxonomy']['context_vocab'];

          //get Vocabulary ID's to use
          $vids = array();
          foreach ($vid_options as $vid => $vid_use) {
            if ($vid_use == 1) {
              $vids[$vid] = $vid;
            }
          }

          //get Taxonomy fields
          $taxonomy_fields = array();
          foreach (field_info_fields() as $field => $field_info) {
            if ($field_info['type'] == 'taxonomy_term_reference') {
              $taxonomy_fields[$field_info['id']] = $field_info['field_name'];
            }
          }

          if ($node) {
            foreach ($taxonomy_fields as $tax_field) {
              $field = isset($node->$tax_field) ? $node->$tax_field : array();
              $field_items = field_get_items('node', $node, $tax_field);
              if (!empty($field_items)) {
                foreach ($field_items as $term_info) {
                  $term = taxonomy_term_load($term_info['tid']);
                  $tids = array($term_info['tid']);
                  $depth = ($bean->filter_options['taxonomy']['term_depth'] == '') ? NULL
                    : $bean->filter_options['taxonomy']['term_depth'];
                  foreach (taxonomy_get_tree($term->vid, $term->tid, $depth) as $tree_term) {
                    $tids[] = $tree_term->tid;
                  }
                  foreach ($tids as $tid) {
                    foreach (taxonomy_select_nodes($tid) as $nid) {
                      if ($nid != $node->nid) {
                        if (!empty($vids)) {
                          if (isset($term_info['taxonomy_term']) == 1) {
                            if (array_search($term_info['taxonomy_term']->vid, $vids) != '') {
                              $term_nodes[$nid] = $nid;
                            }
                          }
                        }
                        else {
                          $term_nodes[$nid] = $nid;
                        }
                      }
                    }
                  }
                }
              }

            }
          }

          break;

        case 0:
          foreach ($bean->filter_options['taxonomy']['vocab'] as $vocab_key => $vocab) {
            if (is_numeric($vocab_key) && $vocab == 1) {
              foreach ($bean->filter_options['taxonomy']['vocab'][$vocab_key . 'terms']['termid'] as $tid_selected) {
                $term = taxonomy_term_load($tid_selected);
                $tids = array($tid_selected);
                $depth = ($bean->filter_options['taxonomy']['term_depth'] == '') ? NULL
                  : $bean->filter_options['taxonomy']['term_depth'];
                foreach (taxonomy_get_tree($term->vid, $term->tid, $depth) as $tree_term) {
                  $tids[] = $tree_term->tid;
                }
                foreach ($tids as $tid) {
                  foreach (taxonomy_select_nodes($tid) as $nid) {
                    if ($node) {
                      if ($nid != $node->nid) {
                        $term_nodes[$nid] = $nid;
                      }
                    }
                    else {
                      $term_nodes[$nid] = $nid;
                    }
                  }
                }
              }
            }
          }
          break;
      }
      if (!empty($term_nodes)) {
        $result->propertyCondition('nid', $term_nodes, 'IN');
      }
      else {
        $result->propertyCondition('nid', 0);
      }
    }

    $result = $data->execute();

    if (isset($result['node'])) {

      $nodes = entity_load('node', array_keys($result['node']));
      $items = array();
      foreach ($nodes as $node) {
        $items[$node->nid] = l($node->title, 'node/' . $node->nid,
          array('attributes' => array(
              'class' => array('relevant-content-link')
            )
          )
        );

        if ($bean->display_options['node_display'] != 'title') {
          $display_content = node_view($node, $bean->display_options['node_display']);
          $items[$node->nid] = $display_content;
        }
      }

      if ($bean->display_options['node_display'] == 'title') {
        $content['bean'][$bean->bid]['bean_relevant']['content'] = array(
          '#theme' => 'item_list',
          '#items' => $items,
        );
      }
      else {
        $content['bean'][$bean->bid]['bean_relevant']['content'] = $items;
      }

    }
    else {
      $current_path = current_path();
      // If filtering on current node, dislpay message for Bean view
      if ($bean->filter_options['content']['current_node'] == 1 &&
        $bean->filter_options['content']['use_nodes'] == 1 &&
        $current_path == 'block/' . $bean->delta . '/view'
      ) {
        drupal_set_message(t('Filtering on "current node" will not display any
          data on this view. Please configure this block to be shown with
          node content.')
        );
      }
      $content = array();
      $bean->title = '';
    }
    return $content;
  }

}

/*
 * Relevant terms callback.
 */
function bean_relevant_terms_callback($form, $form_state) {
  $vocab = taxonomy_vocabulary_machine_name_load($form_state['triggering_element']['#machine_name']);
  $term = $vocab->vid;

  return $form['header']['filter_options']['taxonomy']['vocab'][$term . 'terms'];
}

/*
* Relevant Bean EFQ class
*/
class EntityFieldQueryRelevant extends EntityFieldQuery {
  /*
   * @param $fields
   *   An array of Relevant Bean sort fields/properties keyed on content type,
   *   with table name and field name
   */
  public function setSortFields($fields) {
    $this->sortFields = $fields;
  }

  /*
   * @param $order
   *   String for the sort fields sorting order
   */
  public function setSortFieldsOrder($order) {
    $this->sortFieldsOrder = $order;
  }

  /*
   * @param $fields
   *   An array of Relevant Bean filter fields/properties keyed on content type,
   *   with table name and field name
   */
  public function setRelativeFilterFields($fields) {
    $this->relativeFilterFields = $fields;
  }

  /*
   * @param $order
   *   String for the relative filter value
   *   see http://www.php.net/manual/en/datetime.formats.relative.php
   */
  public function setRelativeFilterValue($value) {
    $this->relativeFilterValue = $value;
  }

  /*
   * Override to add Relevant Bean sorts and filters to EFQ Select Query object
   * see EntityFieldQuery::finishQuery
   */
  function finishQuery($select_query, $id_key = 'entity_id') {
    foreach ($this->tags as $tag) {
      $select_query->addTag($tag);
    }
    foreach ($this->metaData as $key => $object) {
      $select_query->addMetaData($key, $object);
    }
    $select_query->addMetaData('entity_field_query', $this);
    if ($this->range) {
      $select_query->range($this->range['start'], $this->range['length']);
    }
    if ($this->count) {
      return $select_query->countQuery()->execute()->fetchField();
    }
    $return = array();

    // Sort fields start
    if (isset($this->sortFields)) {
      $case = '';
      foreach ($this->sortFields as $key => $fields) {
        $field_convert = $fields['field'];
        if ($fields['table'] != 'node') {
          $select_query->leftJoin($fields['table'], $fields['table'],
            $fields['table'] . '.entity_id = node.nid');
          $field_convert = 'UNIX_TIMESTAMP(' . $fields['field'] . ')';
        }
        $case .= ' WHEN \'' . $key . '\' THEN ' . $field_convert;
      }

      $select_query->addExpression('CASE node.type ' .
        $case
        . ' ELSE \'\'
      END', 'sort_date');

      $select_query->orderBy('sort_date', $this->sortFieldsOrder);
    }
    // Sort fields end

    // relative filter fields start
    if (isset($this->relativeFilterFields)) {

      $select_tables = $select_query->getTables();

      $case = 'CASE node.type ';
      foreach ($this->relativeFilterFields as $key => $fields) {

        $field_convert = $fields['field'];
        if ($fields['table'] != 'node' &&
          !array_key_exists($fields['table'], $select_tables)
        ) {
          $select_query->leftJoin($fields['table'], $fields['table'],
            $fields['table'] . '.entity_id = node.nid');
        }
        if ($fields['table'] != 'node') {
          $field_convert = 'UNIX_TIMESTAMP(' . $fields['field'] . ')';
        }
        $case .= ' WHEN \'' . $key . '\' THEN ' . $field_convert;
      }

      $date = date('Y-m-d H:i:s', strtotime($this->relativeFilterValue));
      $case .= ' ELSE \'\' END > UNIX_TIMESTAMP(\'' . $date . '\')';
      $select_query->where($case);

    }

    foreach ($select_query->execute() as $partial_entity) {
      $bundle = isset($partial_entity->bundle) ? $partial_entity->bundle : NULL;
      $entity = entity_create_stub_entity($partial_entity->entity_type,
        array(
          $partial_entity->entity_id,
          $partial_entity->revision_id,
          $bundle
        ));
      $return[$partial_entity->entity_type][$partial_entity->$id_key] = $entity;
      $this->ordered_results[] = $partial_entity;
    }
    return $return;
  }
}
