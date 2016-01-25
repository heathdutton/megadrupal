<?php

/**
 * @file
 * Important user functions for the exchange links module.
 */

/**
 * Implementation of exchange_links()
 */
function exchange_links_form() {
  $vocabulary = taxonomy_vocabulary_machine_name_load('exchange_links');
  $categories = taxonomy_get_tree($vocabulary->vid);
  $link_category = array();
  if (isset($categories)) {
    foreach ($categories as $category) {
      $link_category[$category->tid] = $category->name;
    }
  }
  $form['link_exchange_info'] = array(
    '#type' => 'item',
    '#markup' => 'Do you own a ' . variable_get('site_name', NULL) . ' related website? Exchange your link with us!<br/>' . variable_get('site_name', NULL) . '.com is a way to get your ' . variable_get('site_name', NULL) . ' related traffic to your website.<br/>All we ask is a link back. <a href="#addlink">Add your website!</a> (' . variable_get('site_name', NULL) . ' related websites only!)<br/>'
  );
  $form['FPV_related_links'] = array(
    '#type' => 'item',
    '#markup' => '<div>&nbsp;</div>' . get_exchange_links() . '<div>&nbsp;</div>',
  );
  $form['related_website'] = array(
    '#type' => 'item',
    '#markup' => '<a id="addlink" name="addlink"></a><br/>Add your ' . variable_get('site_name', NULL) . ' related website:'
  );
  $form['rules'] = array(
    '#type' => 'fieldset',
    '#title' => 'Rules',
    '#collapsible' => FALSE,
  );
  $form['rules']['markup'] = array(
    '#type' => 'item',
    '#markup' => '<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td colspan="3">' . variable_get('exchange_links_rules_desc', NULL) . '</td></tr><tr><td colspan="3">&nbsp;</td></tr><tr><td colspan="3">Play fair link back!</td></tr><tr><td>Linkback</td><td>: </td><td>Required</td></tr><tr><td>Min. PageRank&trade;</td><td>: ' . variable_get('exchange_links_rules_pagerank', NULL) . '</td><td>&nbsp;</td></tr></table>'
  );
  $form['linking_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Our linking details',
    '#collapsible' => FALSE,
  );
  $form['linking_details']['markup'] = array(
    '#type' => 'item',
    '#markup' => '<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td>URL</td><td>:</td><td>' . variable_get('exchange_links_url', NULL) . '</td></tr><tr><td>Site title</td><td>:</td><td>' . variable_get('exchange_links_site_title', NULL) . '</td></tr>
<tr><td>Site description</td><td>:</td><td>' . variable_get('exchange_links_site_desc', NULL) . '</td></tr>
<tr><td>HTML code</td><td>:</td><td>' . variable_get('exchange_links_html_code', NULL) . '</td></tr>
</table>'
  );
  $form['your_linking_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Your linking details',
    '#collapsible' => FALSE,
  );
  $form['your_linking_details']['site_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Site title',
    '#description' => 'Enter site title. Example: Video Sharing Platform',
    '#required' => TRUE,
  );
  $form['your_linking_details']['your_url'] = array(
    '#type' => 'textfield',
    '#title' => 'Your URL',
    '#description' => 'Enter your site URL. Example: http://www.example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['reciprocal_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Reciprocal link URL',
    '#description' => 'Enter reciprocal link. Must be on the same domain as above. Example: http://www.example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Your email',
    '#description' => 'Enter you email address. Example: username@example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['site_desc'] = array(
    '#type' => 'textarea',
    '#title' => 'Site description',
    '#description' => 'Enter site description.',
    '#required' => TRUE,
  );
  $form['your_linking_details']['category'] = array(
    '#type' => 'select',
    '#title' => 'Category',
    '#options' => $link_category,
    '#description' => 'Select link category.',
    '#required' => TRUE,
  );
  $form['add_link'] = array(
    '#type' => 'submit',
    '#value' => 'Add my link',
  );
  return $form;
}
/**
 * Implementation of exchange_links_validate($form, &$form_state)
 */
function exchange_links_form_validate($form, &$form_state) {
  if (!valid_url($form_state['values']['reciprocal_link'], TRUE)) {
    form_set_error('reciprocal_link', 'Please enter valid reciprocal link.');
  }
  elseif (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', 'Please enter a valid email address.');
  }
}
/**
 * Implementation of exchange_links_submit($form, &$form_state)
 */
function exchange_links_form_submit($form, &$form_state) {
  $site_title = $form_state['values']['site_title'];
  $site_url = $form_state['values']['your_url'];
  $reciprocal_url = $form_state['values']['reciprocal_link'];
  $email = $form_state['values']['email'];
  $description = $form_state['values']['site_desc'];
  $category = $form_state['values']['category'];
  $fields = array('id' => NULL, 'site_title' => $site_title, 'site_url' => $site_url, 'reciprocal_url' => $reciprocal_url, 'email' => $email, 'description' => $description, 'category' => $category);
  $flag = db_insert('exchange_links')->fields($fields)->execute();
  if ($flag) {
    drupal_set_message(t('You link has been submitted successfully.'), 'status');
  }
}

 
/**
 * Implementation of get_exchange_links()
 */
function get_exchange_links() {
  $query = db_select('exchange_links', 'el');
  $query->join('taxonomy_term_data', 'td', 'el.category = td.tid');
  $query
    ->fields('el', array('id', 'site_title', 'site_url', 'reciprocal_url', 'email', 'description'))
    ->fields('td', array('name'))
    ->condition('el.status', 1, '=')
    ->orderBy('td.name', 'DESC');
  $result = $query->execute();
  $links = "";
  $check = "";
  foreach ($result as $record) {
    if ($check != $record->name) {
      $check = $record->name;
      $links .= ($links !='')?'</ul>':'';
      $links .= "<div><b>$record->name:</b></div><ul>";
    }
    $links .= "<li>" . l($record->site_title, $record->site_url, array('attributes' => array('title' => $record->site_title, 'target' => '_blank'))) . "</li>";
  }
  $links .= "</ul>";
  return $links;
}