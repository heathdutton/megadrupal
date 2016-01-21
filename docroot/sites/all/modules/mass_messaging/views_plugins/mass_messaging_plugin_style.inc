<?php

/**
 * @file
 * Mass Messaging style plugin
 */

/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class mass_messaging_plugin_style extends views_plugin_style {
  function render() {
    
    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    $results = array();
    $this->view->row_index = 0;
    foreach ($sets as $title => $records) {
      foreach ($records as $row_index => $values) {
        $rendered_values = array();
        
        
        // We do not want to do the rendering in the field plugin,
        // it is much simpler if we do it here.
        foreach ($this->view->field as $id => $field) {
          $rendered_values[$id] = $field->theme($values);
        }
        
        // Set message Id:
        $message_id_field = $this->view->field[$this->display->handler->get_option('message_identifier_field')]->field_alias;
        if (isset($values->{$message_id_field})) {
          $message_id = $values->{$message_id_field};
        }
        else {
          $message_id = $this->view->row_index;
        }
        $results[$this->view->row_index] = new stdClass();
        $results[$this->view->row_index]->message_id = $message_id;
        
        // Set Subject
        $results[$this->view->row_index]->subject = '';
        if ($this->display->handler->get_option('message_subject_field') != '') {
          $results[$this->view->row_index]->subject = $rendered_values[$this->display->handler->get_option('message_subject_field')];
        }
        
        // Set recipients
        $results[$this->view->row_index]->recipients = array();
        if (is_array($this->display->handler->get_option('message_recipient_fields'))) {
          foreach($this->display->handler->get_option('message_recipient_fields') as $recipient_field) {
            $results[$this->view->row_index]->recipients = array_merge($results[$this->view->row_index]->recipients, explode(",", $rendered_values[$recipient_field]));
          }
        }
        
        // Set carbon copy
        $results[$this->view->row_index]->carbon_copy = array();
        if (is_array($this->display->handler->get_option('message_cc_fields'))) {
          foreach($this->display->handler->get_option('message_cc_fields') as $cc_field) {
            $results[$this->view->row_index]->carbon_copy = array_merge($results[$this->view->row_index]->carbon_copy, explode(",", $rendered_values[$cc_field]));
          }          
        }

        
        // Set blind carbon copy
        $results[$this->view->row_index]->blind_carbon_copy = array();
        if (is_array($this->display->handler->get_option('message_bcc_fields'))) {
          foreach($this->display->handler->get_option('message_bcc_fields') as $bcc_field) {
            $results[$this->view->row_index]->blind_carbon_copy = array_merge($results[$this->view->row_index]->blind_carbon_copy, explode(",", $rendered_values[$bcc_field]));
          }
        }
        
        // Set From
        $results[$this->view->row_index]->from = '';
        if ($this->display->handler->get_option('message_from_field') != '') {
          $results[$this->view->row_index]->from = $rendered_values[$this->display->handler->get_option('message_from_field')];
        }
        
        // Set HTML Body
        $results[$this->view->row_index]->html_body = '';
        if ($this->display->handler->get_option('message_html_body_field') != '') {
          $results[$this->view->row_index]->html_body = $rendered_values[$this->display->handler->get_option('message_html_body_field')];
        }
      
        // Set Text Body
        $results[$this->view->row_index]->text_body = '';
        if ($this->display->handler->get_option('message_text_body_field') != '') {
          $results[$this->view->row_index]->text_body = $rendered_values[$this->display->handler->get_option('message_text_body_field')];
        }
      
        // Set the attachments
        $results[$this->view->row_index]->attachments = array();
        if (is_array($this->display->handler->get_option('message_attachment_fields'))) {
          foreach($this->display->handler->get_option('message_attachment_fields') as $attachment_field) {
            $info = field_read_field($attachment_field);
            if (isset($info['type']) && ($info['type'] == 'file' || $info['type'] == 'image' | $info['type'] == 'media')) {
              $raw_field = $values->{'field_' . $attachment_field};              
              foreach ($raw_field as $file) {
                $real_path = drupal_realpath($file['raw']['uri']);
                $results[$this->view->row_index]->attachments[] = array(
                  'filecontent' => file_get_contents($real_path),
                  'filemime' => $file['raw']['filemime'],
//                  'filepath' => $file['raw']['uri'],
//                  'filepath' => drupal_realpath(file_build_uri('/')),
//                  'filename' => basename($real_path),
                  'filename' => 'test.pdf',
                );    
              }
            }
            
//            else {
//              $results[$this->view->row_index]->attachments[] = array(
//                $rendered_values[$attachment_field],
//              );
//            }
            
          }
        }
            

              
        // Set Send Method
        $results[$this->view->row_index]->send_method = '';
        $send_method_field = $this->display->handler->get_option('message_send_method_field');
        $send_method = NULL;
        if ($send_method_field != '') {
          $send_method = mass_messaging_send_methods($rendered_values[$send_method_field]);
        } 
        if ($send_method == NULL) {
          $send_method = mass_messaging_send_methods($this->display->handler->get_option('message_send_method'));
        }
        $results[$this->view->row_index]->send_method = $send_method;
        
        $this->view->row_index++;
      }
    }
    unset($this->view->row_index);

        
    // Check if we are in the preview mode. If so then produce a preview.
    $queue_item_id = $this->display->handler->get_option('mass_messaging_queue_item_id');
    if (empty($queue_item_id)) {
      return theme('mass_messaging_view_preview', array('view' => $this->view, 'results' => $results));
    }
    
    
    
    return $results;
  }
}
