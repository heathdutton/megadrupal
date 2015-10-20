<?php

/**
 * @file
 * Orgmode functions.
 */

/**
 * Form to upload org files.
 */
function orgmode_form_upload($form, $form_state) {
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Org File'),
    '#description' => t('Upload an org file, allowed extension: org'),
  );

  $arr = array();

  foreach (node_type_get_types() as $record) {
    if (user_access('create ' . $record->type . ' content')) {
      $arr[] = $record->type;
    }
  }

  $form['content_type'] = array(
    '#type' => 'select',
    '#title' => t('Org Content Type'),
    '#options' => drupal_map_assoc($arr),
    '#description' => t('The content type you want to be managed by org.'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Validate handler for orgmode_form_validate().
 */
function orgmode_form_upload_validate($form, &$form_state) {
  $file = file_save_upload('file', array('file_validate_extensions' => array('org')));
  // If the file passed validation:
  if ($file) {
    // Move the file, into the Drupal file system.
    if ($file = file_move($file, file_build_uri($file->filename))) {
      // Save the file for use in the submit handler.
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * Submit handler for orgmode_form_upload().
 */
function orgmode_form_upload_submit($form, &$form_state) {
  $file = $form_state['storage']['file'];
  // We are done with the file, remove it from storage.
  unset($form_state['storage']['file']);
  // Make the storage of the file permanent.
  $file->status = FILE_STATUS_PERMANENT;
  // Save file status.
  file_save($file);

  if (variable_get('file_default_scheme') == 'private') {
    $lines = file('private://' . $file->filename);
  }
  else {
    $lines = file('public://' . $file->filename);
  }

  $title = "";
  $body = "";
  $teaser = "";
  $author = "";
  foreach ($lines as &$line) {
    $title_reg = "/^#\+TITLE:.*/";
    if (preg_match($title_reg, $line, $matches)) {
      $title = preg_replace("/^#\+TITLE:(.*)$/", "$1\n", $matches[0]);
    }
    $author_reg = "/^#\+AUTHOR:.*/";
    if (preg_match($author_reg, $line, $matches)) {
      $author = preg_replace("/^#\+AUTHOR:(.*)$/", "$1\n", $matches[0]);
    }
    $beg_abstract = "/^#\+begin_abstract/";
    $beg_abstract_p = FALSE;
    if (preg_match($beg_abstract, $line, $matches)) {
      $beg_abstract_p = TRUE;
    }
    $end_abstract = "/^#\+end_abstract/";
    if (preg_match($end_abstract, $line, $matches)) {
      $beg_abstract_p = FALSE;
    }
    if (!(preg_match("/^#\+/", $line, $matches))) {
      $h1reg = "/^\* .*$/";
      if (preg_match($h1reg, $line, $matches)) {
        $line = preg_replace("/^\* (.*)$/", "<h1>$1</h1>\n", $matches[0]);
      }
      $h2reg = "/^\*\* .*$/";
      if (preg_match($h2reg, $line, $matches)) {
        $line = preg_replace("/^\*\* (.*)$/", "<h2>$1</h2>\n", $matches[0]);
      }
      $h3reg = "/^\*\*\* .*$/";
      if (preg_match($h3reg, $line, $matches)) {
        $line = preg_replace("/^\*\*\* (.*)$/", "<h3>$1</h3>\n", $matches[0]);
      }
      if (preg_match("/\[.*\]/", $line, $matches)) {
        if (preg_match("/\[\[.*\]\[.*\]\]/", $line, $matches)) {
          $line = preg_replace("/\[\[(.*)\]\[(.*)\]\]/", "<a href='$1'>$2</a>", $line);
        }
        if (preg_match("/\[\[file:(.)*(jpg|png|gif)\]\]/", $line, $matches)) {
          $line = preg_replace("/\[\[file:(.*)\]\]/", "<img src='$1' alt='text' />", $matches[0]);
        }
      }
      if (!(preg_match($h1reg, $line, $match1)) && !(preg_match($h2reg, $line, $match2)) && !(preg_match($h3reg, $line, $match3))) {
        if ($beg_abstract_p) {
          $teaser .= $line;
        }
        else {
          $body .= $line;
        }
      }
    }
  }
  $type = $form_state['values']['content_type'];
  $msg = orgmode_create_node($title, $author, $body, $teaser, $type);
  // Set a response to the user.
  drupal_set_message($msg);
}

/**
 * Form constructor for orgmode administration.
 */
function orgmode_form_settings($form, $form_state) {
  $form['orgmode_published'] = array(
    // This is an HTML <input>.
    '#type' => 'checkbox',
    '#input' => TRUE,
    '#title' => t('Published'),
    '#default_value' => variable_get('orgmode_published'),
    '#return_value' => TRUE,
    '#title_display' => 'after',
  );

  $form['orgmode_sticky'] = array(
    '#type' => 'checkbox',
    '#input' => TRUE,
    '#title' => t('Sticky'),
    '#default_value' => variable_get('orgmode_sticky'),
    '#return_value' => TRUE,
    '#process' => array('form_process_checkbox', 'ajax_process_form'),
    '#title_display' => 'after',
  );

  $form['orgmode_promote'] = array(
    // This is an HTML <input>.
    '#type' => 'checkbox',
    '#input' => TRUE,
    '#title' => t('Promote to front'),
    '#default_value' => variable_get('orgmode_promote'),
    '#return_value' => TRUE,
    '#title_display' => 'after',
  );

  $form['orgmode_bodyfield'] = array(
    // This is an HTML <input>.
    '#type' => 'textfield',
    '#input' => TRUE,
    '#title' => t('Body Field'),
    '#default_value' => variable_get('orgmode_bodyfield', 'body'),
    '#return_value' => TRUE,
    '#title_display' => 'before',
  );

  $form['orgmode_css'] = array(
    // You can define your own css in your nodes from org.
    '#type' => 'textfield',
    '#input' => TRUE,
    '#title' => t('Css path to export'),
    '#description'  => t('You can define your own css in your nodes from org'),
    '#default_value' => variable_get('orgmode_csspath'),
    '#return_value' => TRUE,
    '#title_display' => 'before',
  );

  return system_settings_form($form);
}

/**
 * Create an org node.
 */
function orgmode_create_node($title, $author, $body, $teaser, $type) {
  $node = new stdClass();
  if (!empty($title)) {
    $node->title = $title;
  }
  $node->author = $author;
  $node->language = LANGUAGE_NONE;
  $field_name = db_query('select field_name from {field_config_instance} where bundle=:type and field_name=:field_name', array(':type' => $type, ':field_name' => variable_get('orgmode_bodyfield')))->fetchField();
  if (empty($title)) {
    form_set_error('file', t('The title in uploaded file is mandatory.'));
  }
  elseif ($field_name == variable_get('orgmode_bodyfield')) {
    $node->body[$node->language][0]['value'] = $body;
    if ($teaser != "") {
      $node->body[$node->language][0]['summary'] = $body;
    }
    else {
      $node->body[$node->language][0]['summary'] = $body;
    }
    // Filtered HTML.
    $node->body[$node->language][0]['format'] = 'filtered_html';
    // Your specified content type.
    $node->type = $type;
    $node->created = REQUEST_TIME;
    $node->changed = $node->created;
    $node->status = variable_get('orgmode_published');
    $node->promote = variable_get('orgmode_promote');
    $node->sticky = variable_get('orgmode_sticky');
    $node->format = 1;
    global $user;
    $node->uid = $user->uid;
    // Save the node in the database.
    node_save($node);
    return t('The form has been submitted and @title has been saved.', array('@title' => $title));
  }
  else {
    form_set_error('file', t("This content type (@type) hasn't a @body field and field_name is @field_name.", array(
                              '@field_name' => $field_name,
                              '@body' => variable_get('orgmode_bodyfield'),
                              '@type' => $type)));
  }
}

/**
 * Export a node to a simple org file.
 */
function orgmode_export($nid) {
  $node = node_load($nid);
  $filename = preg_replace('/ /', '_', $node->title);
  $destination = file_create_filename($filename, base_path() . '/sites/default/files/');
  $content = "#+TITLE: " . $node->title . "\n";
  if ($node->language != 'und') {
    $content .= "#+LANGUAGE: " . $node->language . "\n";
  }
  $user = user_load($node->uid);
  $content .= "#+AUTHOR: ". $user->name . " \n";
  $content .= $node->body['und'][0]['value'];  

  $h1reg = "/<h1>(.*)<\/h1>/";
  if (preg_match($h1reg, $content, $matches)) {
    $content = preg_replace($h1reg, "\n* $1 \n", $content);
  }

  $h2reg = "/<h2>(.*)<\/h2>/";
  if (preg_match($h2reg, $content, $matches)) {
    $content = preg_replace($h2reg, "\n** $1 \n", $content);
  }

  $h3reg = "/<h3>(.*)<\/h3>/";
  if (preg_match($h3reg, $content, $matches)) {
    $content = preg_replace($h3reg, "\n*** $1 \n", $content);
  }

  $h4reg = "/<h4>(.*)<\/h4>/";
  if (preg_match($h4reg, $content, $matches)) {
    $content = preg_replace($h4reg, "\n**** $1 \n", $content);
  }

  $boldreg = "/<b>(.*)<\/b>/";
  if (preg_match($boldreg, $content, $matches)) {
    $content = preg_replace($boldreg, "*$1*", $content);
  }

  $strongreg = "/<strong>(.*)<\/strong>/";
  if (preg_match($strongreg, $content, $matches)) {
    $content = preg_replace($strongreg, "*$1*", $content);
  }

  $italicreg = "/<i>(.*)<\/i>/";
  if (preg_match($italicreg, $content, $matches)) {
    $content = preg_replace($italicreg, "/$1/", $content);
  }

  $emreg = "/<em>(.*)<\/em>/";
  if (preg_match($emreg, $content, $matches)) {
    $content = preg_replace($emreg, "/$1/", $content);
  }

  $ahreg = "/<a href=('(.*)'|\"(.*)\"|(.*))>(.*)<\/a>/";
  if (preg_match($ahreg, $content, $matches)) {
    $content = preg_replace($ahreg, "[[$2$3$4][$5]]", $content);
  }

  $codereg = "/<code>(.*)<\/code>/";
  if (preg_match($codereg, $content, $matches)) {
    $content = preg_replace($codereg, "#+BEGIN_SRC c\n$1\n#+END_SRC\n", $content);
  }

  $blockquotereg = "/<blockquote>(.*)<\/blockquote>/";
  if (preg_match($blockquotereg, $content, $matches)) {
    $content = preg_replace($blockquotereg, "#+BEGIN_EXAMPLE\n$1\n#+END_EXAMPLE\n", $content);
  }

  header('Content-type: text/plain');
  header('Content-Disposition: attachment; filename="' . $filename . '.org"');
  print($content);
  module_invoke_all('exit');
  exit;
}
