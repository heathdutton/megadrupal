<?php
/**
 * @file
 * Contains the nested Semantic Views plugin.
 */

/**
 * Nested Semantic Views style plugin.
 */
class semanticviews_tree_plugin_style_tree extends semanticviews_plugin_style_default {
  /**
   * Overrides default Semantic Views options.
   */
  function option_definition() {
    $options = parent::option_definition();

    // Adapt list options to container options.
    unset($options['list']);
    $options['container'] = array(
      'contains' => array(
        'element_type' => array('default' => ''),
        'class' => array('default' => ''),
      ),
    );

    // Add tree options.
    $options['tree'] = array(
      'contains' => array(
        'main_field' => array('default' => ''),
        'parent_field' => array('default' => ''),
        'orphan_as_root' => array('default' => FALSE),
      ),
    );
    $options['row']['contains'] += array(
      'root_class' => array('default' => 'root'),
      'leaf_class' => array('default' => 'leaf'),
    );

    return $options;
  }

  /**
   * Modifies Semantic Views configuration for tree-related options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Add option for hierarchy fields.
    $fields = array('' => t('<None>'));
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if ($label = $handler->label()) {
        $fields[$field] = $label;
      }
      else {
        $fields[$field] = $handler->ui_name();
      }
    }
    $form['tree'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tree hierarchy'),
      '#description' => t('Configure the fields for organizing the rows into a hierarchical structure.'),
    );
    $form['tree']['main_field'] = array(
      '#prefix' => '<div class="views-left-50">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#title' => t('Main field'),
      '#options' => $fields,
      '#default_value' => $this->options['tree']['main_field'],
      '#description' => t('Select the field with the unique identifier for each record.'),
    );
    $form['tree']['parent_field'] = array(
      '#prefix' => '<div class="views-left-50">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#title' => t('Parent field'),
      '#options' => $fields,
      '#default_value' => $this->options['tree']['parent_field'],
      '#description' => t('Select the field that contains the unique identifier of the record\'s parent.'),
    );
    $form['tree']['orphan_as_root'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show orphaned elements as root elements'),
      '#default_value' => $this->options['tree']['orphan_as_root'],
    );

    // Adapt list to container options and remove list options.
    unset($form['list']);
    $form['container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rows container'),
      '#description' => t('Rows may be output within a container element. If the output should be a HTML list, specify the element as %ul or %ol. The row element should also be set to %li.', array('%ul' => 'ul', '%ol' => 'ol', '%li' => 'li')),
      '#weight' => 4,
    );
    $form['container']['element_type'] = array(
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#title' => t('Container element'),
      '#type' => 'textfield',
      '#size' => '10',
      '#default_value' => $this->options['container']['element_type'],
    );
    $form['container']['class'] = array(
      '#prefix' => '<div class="views-right-70">',
      '#suffix' => '</div>',
      '#title' => t('Class attribute'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['container']['class'],
    );

    // Add hierarchy options to row.
    $form['row']['hierarchy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Hierarchy classes'),
      '#parents' => array('style_options', 'row'),
      '#description' => t('In a hierarchy of rows, the root elements are those without parents, and leaf elements are those without children.'),
    );
    $form['row']['hierarchy']['root_class'] = array(
      '#title' => t('Root element class attribute'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['row']['root_class'],
    );
    $form['row']['hierarchy']['leaf_class'] = array(
      '#title' => t('Leaf element class attribute'),
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['row']['leaf_class'],
    );
    $form['row']['striping_classes']['#weight'] = 2;

    // Move regular Semantic Views options to the end.
    $form['row']['#weight'] = 5;
  }
}
