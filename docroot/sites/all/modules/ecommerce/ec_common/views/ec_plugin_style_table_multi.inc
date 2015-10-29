<?php
/**
 * @file
 */

class ec_plugin_style_table_multi extends views_plugin_style_table {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['#theme'] = 'ec_common_style_plugin_table';
    
    foreach (element_children($form['info']) as $field) {
      $safe = str_replace(array('][', '_', ' '), '-', $field);
      // the $id of the column for dependency checking.
      $id = 'edit-style-options-columns-' . $safe;
      
      $form['info'][$field]['row'] = array(
        '#type' => 'textfield',
        '#size' => 10,
        '#default_value' => isset($this->options['info'][$field]['row']) ? $this->options['info'][$field]['row'] : '',
        '#dependency' => array($id => array($field)),
      );
      $form['info'][$field]['rowspan'] = array(
        '#type' => 'textfield',
        '#size' => 10,
        '#default_value' => isset($this->options['info'][$field]['rowspan']) ? $this->options['info'][$field]['rowspan'] : '',
        '#dependency' => array($id => array($field)),
      );
      $form['info'][$field]['colspan'] = array(
        '#type' => 'textfield',
        '#size' => 10,
        '#default_value' => isset($this->options['info'][$field]['colspan']) ? $this->options['info'][$field]['colspan'] : '',
        '#dependency' => array($id => array($field)),
      );
    }
  }
}