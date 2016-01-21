<?php
/**
 * @file
 * Filter handler for taxonomy terms with depth using Taxonomy Edge.
 *
 * @see views_handler_filter_term_node_tid_depth_modifier.inc
 */
class views_handler_filter_field_term_edge_tid_depth extends views_handler_filter_term_node_tid_depth {
  function value_form(&$form, &$form_state) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($this->options['vocabulary']);
    if (empty($vocabulary) && $this->options['limit']) {
      $form['markup'] = array(
        '#markup' => '<div class="form-item">' . t('An invalid vocabulary is selected. Please change it in the options.') . '</div>',
      );
      return;
    }

    if ($this->options['type'] == 'textfield') {
      $default = '';
      if ($this->value) {
        $result = taxonomy_term_load_multiple($this->value);
        foreach ($result as $entity_term) {
          if ($default) {
            $default .= ', ';
          }
          $default .= entity_label('taxonomy_term', $entity_term);
        }
      }

      $form['value'] = array(
        '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)) : t('Select terms'),
        '#type' => 'textfield',
        '#default_value' => $default,
      );

      if ($this->options['limit']) {
        $form['value']['#autocomplete_path'] = 'admin/views/ajax/autocomplete/taxonomy/' . $vocabulary->vid;
      }
    }
    else {
      if (!empty($this->options['hierarchy']) && $this->options['limit']) {
        $tree = taxonomy_edge_get_tree($vocabulary->vid, 0, NULL, TRUE);
        $options = array();

        if ($tree) {
          // Translation system needs full entity objects, so we have access to label.
          foreach ($tree as $term) {
            $choice = new stdClass();
            $choice->option = array($term->tid => str_repeat('-', $term->depth) . entity_label('taxonomy_term', $term));
            $options[] = $choice;
          }
        }
      }
      else {
        $options = array();
        $query = db_select('taxonomy_term_data', 'td');
        $query->innerJoin('taxonomy_vocabulary', 'tv', 'td.vid = tv.vid');
        $query->fields('td');
        $query->orderby('tv.weight');
        $query->orderby('tv.name');
        $query->orderby('td.weight');
        $query->orderby('td.name');
        $query->addTag('term_access');
        if ($this->options['limit']) {
          $query->condition('tv.machine_name', $vocabulary->machine_name);
        }
        $result = $query->execute();

        $tids = array();
        foreach ($result as $term) {
          $tids[] = $term->tid;
        }
        $entities = taxonomy_term_load_multiple($tids);
        foreach ($entities as $entity_term) {
          $options[$entity_term->tid] = entity_label('taxonomy_term', $entity_term);
        }
      }

      $default_value = (array) $this->value;

      if (!empty($form_state['exposed'])) {
        $identifier = $this->options['expose']['identifier'];

        if (!empty($this->options['expose']['reduce'])) {
          $options = $this->reduce_value_options($options);

          if (!empty($this->options['expose']['multiple']) && empty($this->options['expose']['required'])) {
            $default_value = array();
          }
        }

        if (empty($this->options['expose']['multiple'])) {
          if (empty($this->options['expose']['required']) && (empty($default_value) || !empty($this->options['expose']['reduce']))) {
            $default_value = 'All';
          }
          elseif (empty($default_value)) {
            $keys = array_keys($options);
            $default_value = array_shift($keys);
          }
          // Due to #1464174 there is a chance that array('') was saved in the admin ui.
          // Let's choose a safe default value.
          elseif ($default_value == array('')) {
            $default_value = 'All';
          }
          else {
            $copy = $default_value;
            $default_value = array_shift($copy);
          }
        }
      }
      $form['value'] = array(
        '#type' => 'select',
        '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)) : t('Select terms'),
        '#multiple' => TRUE,
        '#options' => $options,
        '#size' => min(9, count($options)),
        '#default_value' => $default_value,
      );

      if (!empty($form_state['exposed']) && isset($identifier) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $default_value;
      }
    }

    if (empty($form_state['exposed'])) {
      // Retain the helper option.
      $this->helper->options_form($form, $form_state);
    }
  }
  
  function query() {
    // If no filter values are present, then do nothing.
    if (count($this->value) == 0) {
      return;
    }
    elseif (is_array($this->value) && count($this->value) == 1) {
      $this->value = current($this->value);
    }

    $this->ensure_my_table();

    $tids = $this->value;
    $subquery = db_select('taxonomy_term_edge', 'te')
        ->fields('te', array('tid'));

    if ($this->options['depth'] == 'all') {
      $subquery->condition('te.parent', $tids);
    }
    elseif ($this->options['depth'] > 0) {
      $subquery
        ->condition('te.parent', $tids)
        ->condition('te.distance', (int) $this->options['depth'], '<=');
    }
    elseif ($this->options['depth'] < 0) {
      $subquery
        ->condition(
          'te.parent',
          db_select('taxonomy_term_edge', 'te2')
            ->fields('te2', array('parent'))
            ->condition('te2.tid', $tids)
            ->condition('distance', abs((int) $this->options['depth']))
        )
        ->condition('te.distance', abs((int) $this->options['depth']), '<=');
    }
    else {
      $subquery->condition('te.tid', $tids);
    }

    $this->query->add_where(0, "$this->table_alias.$this->real_field", $subquery, 'IN');
  }
}
