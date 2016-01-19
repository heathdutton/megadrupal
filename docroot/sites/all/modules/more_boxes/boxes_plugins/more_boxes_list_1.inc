<?php
/**
 * @file
 */

/**
 *
 * @param $vid
 * @return All terms 
 */
function more_boxes_get_term_options($vid) {
  $term_options['all'] = t('All');
  if (is_numeric($vid)) {
    foreach (taxonomy_get_tree($vid) as $term_obj) {
      $term_options[$term_obj->tid] = check_plain($term_obj->name);
    }
  }
  return $term_options;
}

/**
 * Ajax callback for more_boxes_list_1
 * Needs to add ['options'] to the form before the element.
 * Thats how boxes_box build it
 */
function more_boxes_ajax_callback($form, $form_state) {
  return $form['options']['filters']['term'];
}

class more_boxes_list_1 extends boxes_box {

  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {
    return array(
      'layout' => array(
        'use_pager' => FALSE,
        'items_per_page' => 5,
        'list_type' => 'block_1',
      ),
      'filters' => array(
        'node_type' => 'all',
        'vocabulary' => 'all',
        'term' => 'all',
      ),
      'exposed_filters' => array(
        'show_form' => FALSE,
        'toggle_search' => FALSE,
        'exposed_vocabulary' => FALSE,
      ),
      'sorts' => array(
        'sort_type' => 0,
        'sort_order' => 1,
      ),
    );
  }

  /**
   * Implementation of boxes_box::options_form().
   */
  public function options_form(&$form_state) {
    $view = views_get_view('more_boxes_dynamic_view');
 
    // Content types list options.
    $type_options['all'] = t('All');
    foreach (node_type_get_names() as $node_type => $type_name) {
      $type_options[$node_type] = $type_name;
    }
    // Taxonomy's vocabularies list options.
    $vocab_options_mn['all'] = t('All');
    $vocab_options_vid['all'] = t('All');
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $vocab_options_mn[$vocab->machine_name] = $vocab->name;
      $vocab_options_vid[$vocab->vid] = $vocab->name;
    }
    
    // Layout
    $form['layout'] = array(
      '#type' => 'fieldset',
      '#title' => t('Page info'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['layout']['list_type'] = array(
      '#type' => 'radios',
      '#title' => t('Choose list type'),
      '#options' => array(
        'block_1' => t('Teasers'),
        'block_2' => t('Table'),
        //'table_no_links' => t('Table No links'),
      ),
      '#default_value' => $this->options['layout']['list_type'],
    );
    $form['layout']['use_pager'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use pager'),
      '#default_value' => $this->options['layout']['use_pager'],
    );
    $form['layout']['items_per_page'] = array(
      '#type' => 'textfield',
      '#title' => t('Items per page'),
      '#default_value' => $this->options['layout']['items_per_page'],
    );
    
    //filters
    $form['filters'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filters'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#prefix' => '<div id="filters-info-wrapper">', // This is our wrapper div.
      '#suffix' => '</div>',
    );
    $form['filters']['node_type'] = array(
      '#type' => 'select',
      '#title' => t('Node Type'),
      '#options' => $type_options,
      '#default_value' => $this->options['filters']['node_type'],
    );
    $form['filters']['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#options' => $vocab_options_vid,
      '#default_value' => $this->options['filters']['vocabulary'],
      '#ajax' => array(
        'callback' => 'more_boxes_ajax_callback',
        'wrapper' => 'replace_div', 
      ),
    );
    // Get the value of the selected vocabulary, to build the terms options array by ajax.
    $val = (!empty($form_state['values']['filters']['vocabulary']) ? $form_state['values']['filters']['vocabulary'] : $this->options['filters']['vocabulary']);
    $form['filters']['term'] = array(
      '#type' => 'select',
      '#title' => t('Term'),
      '#options' => more_boxes_get_term_options($val),
      '#default_value' => $this->options['filters']['term'],
      '#prefix' => '<div id="replace_div">',
      '#suffix' => '</div>',
    );

    // exposed filters
    $form['exposed_filters'] = array(
      '#type' => 'fieldset',
      '#title' => t('Exposed filters'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['exposed_filters']['show_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show exposed form'),
      '#default_value' => $this->options['exposed_filters']['show_form'],
    );
    $form['exposed_filters']['toggle_search'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show exposed search box'),
      '#default_value' => $this->options['exposed_filters']['toggle_search'],
    );
    $form['exposed_filters']['exposed_vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Exposed vocabulary'),
      '#options' => $vocab_options_mn,
      '#default_value' => $this->options['exposed_filters']['exposed_vocabulary'],
    );
    
    //sorts
    $form['sorts'] = array(
      '#type' => 'fieldset',
      '#title' => t('Sort'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $sort_options = array_keys($view->display['default']->display_options['sorts']);
    $sort_options = array_combine($sort_options, $sort_options);
    $form['sorts']['sort_type'] = array(
      '#type' => 'select',
      '#title' => t('Sort type'),
      '#options' => $sort_options,
      '#default_value' => $this->options['sorts']['sort_type'],
    );
    $form['sorts']['sort_order'] = array(
      '#type' => 'select',
      '#title' => t('Sort order'),
      '#options' => array("asc", "desc"),
      '#default_value' => $this->options['sorts']['sort_order'],
    );
    
    $form_state['box']->options['additional_classes'] .= " " . $form_state['box']->options['filters']['node_type'];
    return $form;
  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $block['title'] = $title;
    //show title inside exposed views form if form is enabled
   // $block['subject'] = ($this->options['exposed_filters']['show_form']) ? '' : $title;
    $block['delta'] = $this->delta;
         
    // Create an array of arguments.
    $args = array(
      // Layout.  
      $this->options['layout']['use_pager'],
      $this->options['layout']['items_per_page'],
      // Sort  
      $this->options['sorts']['sort_type'],
      $this->options['sorts']['sort_order'],
      // Exposed Filter.
      $this->options['exposed_filters']['show_form'],
      $this->options['exposed_filters']['exposed_vocabulary'],
      $this->options['exposed_filters']['toggle_search'],
      // Filters.  
      $this->options['filters']['node_type'],
      $this->options['filters']['vocabulary'],
      $this->options['filters']['term'],
    ); 
    
    //Get the view object
    $view = views_get_view('more_boxes_dynamic_view');
    $view->set_arguments($args);

    $list_type = $this->options['layout']['list_type'];
    $block['content'] = $view->preview($list_type, $args);
    return $block;
  }
}