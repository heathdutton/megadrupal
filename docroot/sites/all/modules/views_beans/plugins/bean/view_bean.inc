<?php

/**
 * @file
 * Bean Type: View Panes
 */
class ViewBean extends BeanPlugin {
  private $bean;


  /**
   * Declares default block settings.
   * 'type' value determines whether tag query should be inclusive or exclusive
   */
  public function values() {
    $values = array(
      'subtype' => '',
    ) + $this->view_form_values();

    return array_merge(parent::values(), $values);
  }
  private function view_form_values() {
    return array(
      // @todo Any View Pane settings to be used need to be added here.
      'exposed' => array(),
      'items_per_page' => NULL,
      'more_link' => FALSE,
      'link_to_view' => FALSE,
      'use_pager' => FALSE,
      'pager_id' => '',
      'offset' => 0,
    );
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {

    $form = array();

    $form_state['conf'] = isset($bean->data) ? $bean->data : array();

    $form_state['exposed'] = $bean->data['exposed'];
    $form_state['contexts'] = array();
    module_load_include('inc', 'views_content', 'plugins/content_types/views_panes');
    //$type = bean_type_load($this->type);
    $subtypes = $this->get_subtypes($this->getInfo('label'));
    $current_subtype = '';
    if (isset($form_state['values']['subtype'])) {
      $current_subtype = $form_state['values']['subtype'];
    }
    elseif ($bean->bid) {
      $current_subtype = $bean->subtype;
    }
    else {
      $subtype_keys = array_keys($subtypes);
      $current_subtype = array_shift($subtype_keys);
    }
    $form['subtype'] = array(
      '#options' => $subtypes,
      '#type' => 'select',
      '#title' => t('Select View'),
      '#default_value' => $current_subtype,
      '#ajax' => array(
        'callback' => '_ilr_bean_view_ajax_select_display',
        'wrapper' => 'view-pane-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    $form['view_pane'] = array(
      '#prefix' => '<div id="view-pane-div">',
      '#suffix' => '</div>',
      '#type' => 'fieldset',
      '#description' => '<strong>' . $this->get_subtype_description($current_subtype) . '</strong>',
    );
    $form_state['subtype_name'] = $current_subtype;
    $form['view_pane'] += views_content_views_panes_content_type_edit_form($form['view_pane'], $form_state);

    $default_values = $this->view_form_values();
    unset($default_values['subtype']);
    foreach ($default_values as $key => $value) {
      if (!isset($form['view_pane'][$key])) {
        $form['view_pane'][$key] = array(
          '#type' => 'value',
          '#value' => $value,
        );
      }
    }
    if (isset($form['view_pane']['arguments']) && element_children($form['view_pane']['arguments'])) {
      unset($form['view_pane']['arguments']);
      drupal_set_message(t('View Pane arguments are not supported.'));
    }

    return $form;
  }
  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    module_load_include('inc', 'views_content', 'plugins/content_types/views_panes');
    module_load_include('inc', 'ctools', 'includes/content');

    $conf = $bean->data;
    $ctools_content = views_content_views_panes_content_type_render($bean->subtype, $conf, array(), array());
    return array(
      '#markup' => $ctools_content->content,
      '#type' => 'markup',
    );
  }
  /**
   * Utility Function to get bean types.
   *
   * A Bean Type is made for each View Pane Category selected on the admin form.
   * @return array
   */
  private function get_subtypes($category) {
    $used_views = _views_beans_get_used_displays();
    $bean_types = array();
    if ($used_views) {
      foreach ($used_views as $view_name => $display_ids) {
        $view = views_get_view($view_name);
        if ($view) {
          // Ensure displays are actually on View
          $displays = array_intersect_key($view->display, array_flip($display_ids));
          foreach ($displays as $display_id => $display) {
            if (isset($display->display_options['pane_category'])) {
              $bean_type_name = $display->display_options['pane_category']['name'];
            }
            else {
              $bean_type_name = 'View panes';
            }
            if ($bean_type_name == $category){
              $bean_types["$view_name-$display_id"] = $view->human_name . ' : ' . $display->display_title;
            }

          }
        }
      }
    }

    return $bean_types;
  }
  private function get_subtype_description($subtype) {
    list($view_name, $display_id) = explode('-', $subtype);
    $view = views_get_view($view_name);
    if (!empty($view->display[$display_id]->display_options['display_description'])) {
      return $view->display[$display_id]->display_options['display_description'];
    }
    return $view->description;
  }
}
