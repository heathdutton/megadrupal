<?php

/**
 * @file
 * Mass Messaging display plugin.
 */

/**
 * This class contains all the functionality of the PDF display.
 */
class mass_messaging_plugin_display extends views_plugin_display {

  /**
   * Define the display type
   */
  function get_style_type() { return 'message'; }

  /**
   * Disable the breadcrumb
   */
  function uses_breadcrumb() { return FALSE; }

  function use_pager() { return FALSE; }

  function has_path() { return FALSE; }

  function execute() {
    return $this->view->render($this->display->id);
  }

  function render() {
    if (!empty($this->view->result) || !empty($this->view->style_plugin->definition['even empty'])) {
      return $this->view->style_plugin->render($this->view->result);
    }
    return '';
  }

  function uses_exposed() {
    return FALSE;
  }

  function query() {

    // Make sure the id field is included in the results, and save its alias
    // so that references_plugin_style can retrieve it.
    $base_table_alias = $this->view->field[$this->get_option('message_identifier_field')]->table_alias;
    $base_field = $this->view->field[$this->get_option('message_identifier_field')]->field;
    
    
    // Alter the query only if we are not in the preview mode.
    $queue_item_id = $this->get_option('mass_messaging_queue_item_id');
    
    if (empty($queue_item_id)) {
      return;
    }
    
    $base_table = $this->view->query->ensure_table($base_table_alias);
    
    $join = new views_join();
    $left_table = $base_table_alias;
    $left_field = $base_field;
    $right_table = 'mass_messaging_sent_messages';
    $right_field = 'message_id';
    $type = 'LEFT';
    $extra = array(
      array(
        'field' => 'queue_item_id',
        'value' => $queue_item_id,
      )
    );
        
    $alias = $right_table . '_' . $left_table;
    
    $join->construct($right_table, $left_table, $left_field, $right_field, $extra, $type);
    
    $this->view->query->add_relationship($alias, $join, $right_table);  
    $this->view->query->add_where_expression(0, 'message_id IS NULL');
    
        
  }

  /**
   * The option definition.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'message_style';
    $options['style_options']['default'] = array('mission_description' => FALSE, 'description' => '');
    $options['sitename_title']['default'] = FALSE;
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;

    $options['message_from_field']['default'] = '';
    $options['message_subject_field']['default'] = '';
    $options['message_text_body_field']['default'] = '';
    $options['message_html_body_field']['default'] = '';
    $options['message_recipient_fields']['default'] = array();
    $options['message_cc_fields']['default'] = array();
    $options['message_bcc_fields']['default'] = array();
    $options['message_attachment_fields']['default'] = array();
    $options['message_send_method_field']['default'] = '';
    $options['message_send_method']['default'] = '';
    $options['message_identifier_field']['default'] = '';
    
    return $options;
  }

  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);
        
    $categories['message'] = array(
      'title' => t('Mass Messaging Settings'),
      'column' => 'second',
      'build' => array(
        '#weight' => 20,
      ),
    );

    $options['message_recipient_fields'] = array(
      'category' => 'message',
      'title' => t('Recipient Fields'),
      'value' => $this->format_field_option('message_recipient_fields'),
      'desc' => t('Define the recipients for the message.'),
    );

    $options['message_cc_fields'] = array(
      'category' => 'message',
      'title' => t('CC Fields'),
      'value' => $this->format_field_option('message_cc_fields'),
      'desc' => t('Define the carbon copy (CC) recipients of the message.'),
    );

    $options['message_bcc_fields'] = array(
      'category' => 'message',
      'title' => t('BCC Fields'),
      'value' => $this->format_field_option('message_bcc_fields'),
      'desc' => t('Define the blind carbon copy (BCC) recipients of the message.'),
    );

    $options['message_from_field'] = array(
      'category' => 'message',
      'title' => t('From Field'),
      'value' => $this->format_field_option('message_from_field'),
      'desc' => t('Define the field, which is used for the from (sender).'),
    );
    
    $options['message_subject_field'] = array(
      'category' => 'message',
      'title' => t('Subject Field'),
      'value' => $this->format_field_option('message_subject_field'),
      'desc' => t('Define the field, which is used for the subject.'),
    );
    
    $options['message_text_body_field'] = array(
      'category' => 'message',
      'title' => t('Text Body Field'),
      'value' => $this->format_field_option('message_text_body_field'),
      'desc' => t('Define the field for the text (no HTML) body of the message.'),
    );

    $options['message_html_body_field'] = array(
      'category' => 'message',
      'title' => t('HTML Body Field'),
      'value' => $this->format_field_option('message_html_body_field'),
      'desc' => t('Define the field for the HTML body of the message.'),
    );
    
    $options['message_attachment_fields'] = array(
      'category' => 'message',
      'title' => t('Attachment Fields'),
      'value' => $this->format_field_option('message_attachment_fields'),
      'desc' => t('Define the fields, which should be added as files (attachments) to the message.'),
    );

    $options['message_send_method'] = array(
      'category' => 'message',
      'title' => t('Message Send Method'),
      'value' => $this->get_option('message_send_method') != '' ? $this->get_option('message_send_method') : t('No Send Method defined'),
      'desc' => t('Define the field which determines the message send mehtod.'),
    );

    $options['message_identifier_field'] = array(
      'category' => 'message',
      'title' => t('Message Identifier Field'),
      'value' => $this->format_field_option('message_identifier_field'),
      'desc' => t('Define which relation is the base table.'),
    );
    
//    foreach($this->get_option('fields') as $id => $field) {
//      $handler = views_get_handler($field['table'], $field['field'], 'field');
//      if ($handler instanceof views_handler_field_numeric) {
//        echo 'yes: ' . $id;
//      }
//      print_r($handler);
//    }

    
  }

  /**
   * Option form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $columns = $this->display->handler->get_field_labels();
    
    foreach($columns as $key => $column) {
      $columns[$key] = $column . ' (' . $key . ')';
    }
    
    $columnsWithNone = array_merge(array('' => t('none')), $columns);
    
    $tables = array();
    foreach ($this->view->get_base_tables() as $table => $active) {
      if ($active && $table != '#global') {
        $tables[$table] = $table;
      }
      
    }
    
    foreach ($this->get_option('relationships') as $id => $options) {
      $tables[$options['table']] = $options['table'];
    }
    
    
    $sendMethods = array();
    
    foreach(mass_messaging_send_methods() as $name => $info) {
      $sendMethods[$name] = $info['name'];
    }

    switch ($form_state['section']) {
      case 'message_recipient_fields':
        $form['#title'] .= t('Recipient Fields');
        $form['message_recipient_fields'] = array(
          '#type' => 'checkboxes',
          '#options' => $columns,
          '#default_value' => $this->get_option('message_recipient_fields'),
          '#description' => t('Specify here the fields which build the recipient list. Fields which contains a comma, they will be separted automatically.'),
        );
        break;
      case 'message_cc_fields':
        $form['#title'] .= t('CC Recipient Fields');
        $form['message_cc_fields'] = array(
          '#type' => 'checkboxes',
          '#options' => $columns,
          '#default_value' => $this->get_option('message_cc_fields'),
          '#description' => t('Specify here the fields which build the carbon copy (CC) recipient list. Fields which contains a comma, they will be separted automatically.'),
        );
        break;
      case 'message_bcc_fields':
        $form['#title'] .= t('BCC Recipient Fields');
        $form['message_bcc_fields'] = array(
          '#type' => 'checkboxes',
          '#options' => $columns,
          '#default_value' => $this->get_option('message_bcc_fields'),
          '#description' => t('Specify here the fields which build the blind carbon copy (BCC) recipient list. Fields which contains a comma, they will be separted automatically.'),
        );
        break;
      case 'message_attachment_fields':
        $form['#title'] .= t('Attachment Fields');
        $form['message_attachment_fields'] = array(
          '#type' => 'checkboxes',
          '#options' => $columns,
          '#default_value' => $this->get_option('message_attachment_fields'),
          '#description' => t('Specify here the fields which are added as files to the message. File fields are added without any change. All other fields are added as .txt files.'),
        );
        break;
      case 'message_subject_field':
        $form['#title'] .= t('Subject Field');
        $form['message_subject_field'] = array(
          '#type' => 'radios',
          '#options' => $columnsWithNone,
          '#default_value' => $this->get_option('message_subject_field'),
          '#description' => t('Specify here the field which is used as the subject for the message.'),
        );
        break;
      case 'message_from_field':
        $form['#title'] .= t('From Field');
        $form['message_from_field'] = array(
          '#type' => 'radios',
          '#options' => $columnsWithNone,
          '#default_value' => $this->get_option('message_from_field'),
          '#description' => t('Specify here the field which is used as the from for the message.'),
        );
        break;
      case 'message_text_body_field':
        $form['#title'] .= t('Text Body Field');
        $form['message_text_body_field'] = array(
          '#type' => 'radios',
          '#options' => $columnsWithNone,
          '#default_value' => $this->get_option('message_text_body_field'),
          '#description' => t('Specify here the field which is used as the message body (text).'),
        );
        break;
      case 'message_html_body_field':
        $form['#title'] .= t('HTML Body Field');
        $form['message_html_body_field'] = array(
          '#type' => 'radios',
          '#options' => $columnsWithNone,
          '#default_value' => $this->get_option('message_html_body_field'),
          '#description' => t('Specify here the field which is used as the message body (HTML).'),
        );
        break;
      case 'message_send_method':
        $form['#title'] .= t('Send Method');
        $form['message_send_method_field'] = array(
          '#type' => 'radios',
          '#title' => t('Field for Send Method'),
          '#options' => $columnsWithNone,
          '#default_value' => $this->get_option('message_send_method_field'),
          '#description' => t('Specify here the field which is used as the message send method.'),
        );
        $form['message_send_method'] = array(
          '#type' => 'radios',
          '#title' => t('Default Send Method'),
          '#options' => $sendMethods,
          '#default_value' => $this->get_option('message_send_method'),
          '#description' => t('Specify here the send method. You can either use the field or the raw input to determine the send method. If the defined field is not set or it is not valid the default send method is used.'),
        );
        break;
      case 'message_identifier_field':
        $form['#title'] .= t('Message Identifier Field');
        $form['message_identifier_field'] = array(
          '#type' => 'radios',
          '#options' => $columns,
          '#default_value' => $this->get_option('message_identifier_field'),
          '#description' => t('Specify here the field, which identify the message. You should use only integer values. In most cases this would be the user id.'),
        );
        break;
        
    }
  }


  /**
   * Handles the storage of the options.
   *
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'message_recipient_fields':
        $this->set_option('message_recipient_fields', $this->clean_up_checkbox_values($form_state['values']['message_recipient_fields']));
        break;
      case 'message_cc_fields':
        $this->set_option('message_cc_fields', $this->clean_up_checkbox_values($form_state['values']['message_cc_fields']));
        break;
      case 'message_bcc_fields':
        $this->set_option('message_bcc_fields', $this->clean_up_checkbox_values($form_state['values']['message_bcc_fields']));
        break;
      case 'message_attachment_fields':
        $this->set_option('message_attachment_fields', $this->clean_up_checkbox_values($form_state['values']['message_attachment_fields']));
        break;
      case 'message_subject_field':
        $this->set_option('message_subject_field', $form_state['values']['message_subject_field']);
        break;
      case 'message_from_field':
        $this->set_option('message_from_field', $form_state['values']['message_from_field']);
        break;
      case 'message_text_body_field':
        $this->set_option('message_text_body_field', $form_state['values']['message_text_body_field']);
        break;
      case 'message_html_body_field':
        $this->set_option('message_html_body_field', $form_state['values']['message_html_body_field']);
        break;
      case 'message_identifier_field':
        $this->set_option('message_identifier_field', $form_state['values']['message_identifier_field']);
        break;
      case 'message_send_method':
        $this->set_option('message_send_method', $form_state['values']['message_send_method']);
        $this->set_option('message_send_method_field', $form_state['values']['message_send_method_field']);
        break;
    }
  }

  /**
   * This method is used to strip all the empty fields from the 
   * checkboxs values.
   * 
   * @param array $values
   * @return array cleaned $values
   */
  private function clean_up_checkbox_values($values) {
    $returns = array();
    foreach ($values as $key => $value) {
      if ($value != '0') {
        $returns[$key] = $value;
      }
    }
    return $returns;
  }
  
  /**
   * This method formats the fields in a setting array.
   * @param array $fields
   */
  private function format_field_option($option_name) {
    $fields = $this->get_option($option_name);
    
    $columns = $this->display->handler->get_field_labels();
    
    $return = '';
    if (is_array($fields) && count($fields) > 0) {
      foreach ($fields as $field) {
        $return .= $columns[$field] . ', ';
      }
      $return = substr($return, 0, -2);
    }
    elseif (is_string($fields) && !empty($fields)) {
      $return = $columns[$fields];
    }
    else {
      $return = t('Not field defined');
    }
    
    return $return;
  }
  
  function validate() {
    $errors = parent::validate();
    
    $identifier = $this->get_option('message_identifier_field');
    $columns = $this->display->handler->get_field_labels(); 
    if (empty($identifier) || !isset($columns[$identifier])) {
      $errors[] = t('Messaging: For message identification an identifier field must be set.');
    }
    
    $send_method = $this->get_option('message_send_method');
    if (empty($send_method)) {
      $errors[] = t('Messaging: You need to specify a default send method.');
    }

    return $errors;
  }


}


