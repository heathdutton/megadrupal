<?php

/**
 * @file
 * Administrative page callbacks for the adsense_injector module.
 *
 * @ingroup adsense_injector
 */

/**
 * Implements hook_settings().
 */
function adsense_injector_admin_settings($form, &$form_state) {
  // 'body view' insertion (i.e. show with node complete view).
  $form['adsense_injector_body_view'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Node body ad insertion'),
    '#description' => t('These options control the automatic insertion in node bodies when viewed as an individual page.'),
    //
    // Preserve tree structure of all child elements.  
    // IMPORTANT: '#tree' must be == true in order for the variables to be stored correctly for use by other parts of this module.
    // Changing this will break the module.
    '#tree'=>true, 
  );
  
  $form['adsense_injector_body_view']['enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => _ai_node_body_view_enabled(),
    '#description' => t('Enable auto-insertion in node bodies when other criteria are satisfied.'),
    '#required' => FALSE,
  );
  $form['adsense_injector_body_view']['minwords'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum node body word count'),
    '#default_value' => _ai_node_body_view_minwords(),
    '#description' => t('The minimum node body word count threshold - only inject if node body has at least this many words (NOTE: HTML markup in node body may alter word count calculations.)'),
    '#required' => TRUE,
  );

  $insertion_text_description = t('Ad insertion template. You may insert raw HTML, JavaScript, or AdSense module filter tags if you have enabled the AdSense module. See the <a href="/admin/settings/adsense">adsense.module settings page</a> for a list of supported formats and help with filter tags.  <strong>Note:</strong> no other input filters are applied to this text.');
  
  //
  // Get the body insertion variables.
  $default_body_insertions=_ai_get_default_body_insertions_vars();
  $body_insertions = _ai_get_body_insertions_vars();  
  
  $form['adsense_injector_body_view']['body_insertions'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Individual insertion elements'),
    '#description' => t('Node body insertion elements.  There are currently three elements defined: top, inline, and bottom.<br/>Elements having empty or default values are collapsed by default.'),
  );
  
  foreach ($body_insertions as $key=>$item) {
    $defval = $default_body_insertions['body_insertions'][$key];
    $textparms = array('@name'=>$item['name'], '!default_xpath'=>$defval['xpath'], '@front' => ADSENSE_INJECTOR_DEFAULT_XPATH_FRONT, '@back' => ADSENSE_INJECTOR_DEFAULT_XPATH_BACK);
    $form['adsense_injector_body_view']['body_insertions'][$key] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => (strlen($item['text']) == 0 || strcmp($item['text'], $defval['text']) == 0),
      '#title' => t('@name Insertion', $textparms),
      '#description' => t('@name insertion text.  This text will be processed and inserted at the specified position within the node body.', $textparms),
      '#tree' => true,
    );

    $form['adsense_injector_body_view']['body_insertions'][$key]['text'] = array(
      '#type' => 'textarea',
      '#title' => t('@name ad insertion text', $textparms),
      '#rows' => _ai_admin_textarea_height($item['text']), 
      '#cols' => 40,
      '#default_value' => $item['text'],
      '#description' => $insertion_text_description, 
      '#required' => FALSE,
    );
    // Show the xpath expression if admin enabled.
    $form['adsense_injector_body_view']['body_insertions'][$key]['xpath'] = array(
      '#type' => $item['adminui'] ? 'textfield' : 'hidden',
      '#title' => t('@name insertion token or XPath expression', $textparms),
      '#default_value' => $item['xpath'],
      '#description' => t('The <em>insertion token</em> or <em>XPath expression</em> used to determine the insertion location.<br/><strong>NOTE:</strong> <em>Do not alter this value</em> unless you understand what you are doing, as an invalid XPath expression may break your site. The <strong>default value</strong> is "!default_xpath" (without quotes).  <strong>Valid values</strong> include the insertion tokens <strong>"@front"</strong> and <strong>"@back"</strong> (without quotes), or any valid XPath expression.', $textparms),
      '#required' => TRUE,
      );
  }
  /** END OF body insertion template list **/

  // 'list view' insertion (frontpage, taxonomy, etc).
  $form['list_view'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Node list ad insertion'),
    '#description' => t('Ad insertion in node lists or other non-page view, like front page, taxonomy views.'),
  );
  $form['list_view']['adsense_injector_list_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append an ad after teaser on frontpage and taxonomy lists'),
    '#default_value' => variable_get('adsense_injector_list_view', FALSE),
    '#description' => t('Note: this does not currently support Views module based lists.'),
    '#required' => FALSE,
  );
  
  $list_template = variable_get('adsense_injector_list_view_template', '%teaser<br class="clear"/>[adsense:728x90:0123456789]');
  $form['list_view']['adsense_injector_list_view_template'] = array(
    '#type' => 'textarea',
    '#title' => t('List ad insertion template'),
    '#rows' => _ai_admin_textarea_height($list_template),
    '#cols' => 40,
    '#default_value' => $list_template,
    '#description' => $insertion_text_description,
    '#required' => TRUE,
  );

  // What kinds of nodes do we want to insert on...?
  $form['content_types'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Content types'),
  );
  $form['content_types']['adsense_injector_nodes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#default_value' => variable_get('adsense_injector_nodes', array()),
    '#options' => array_map('check_plain', node_type_get_names()),
    '#description' => t('Select content types to display inline ads.'),
  );

  return system_settings_form($form);
}

/**
 * Validate the admin settings form.
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function adsense_injector_admin_settings_validate($form, &$form_state) {
}

/**
 * Determine textarea auto-height in rows for a bit of text. 
 * @param $text The text to operate on.
 * @return The number of rows we should use for a textarea.
 */
function _ai_admin_textarea_height($text) {
  // 
  // Determine # of lines in textarea
  // http://stackoverflow.com/questions/1743745/count-new-lines-in-textarea-to-resize-container-in-php
  preg_match_all("/(\n)/", $text, $matches);
  $count = count($matches[0]) + 1; // +1 for the last tine
  return max(5, min($count, 20));
}

