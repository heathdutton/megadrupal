<?php
/**
 * @file
 * Context Meta Module.
 */

/**
 * Admin form to add context meta tags context_meta_form.
 */
function context_meta_list() {
  $header = array('Page URL', 'Meta Attribute', 'Tag', 'Status' , 'Operation');
  $rows = array();
  $context_path = 'admin/config/context-meta';
  $meta_tags = context_meta_get_tags('all');

  $output = l(t('Add new tag'), 'admin/config/context-meta/add');

  if ($meta_tags) {
    foreach ($meta_tags as $meta_tag) {
      $tag = '&lt;meta ' . check_plain($meta_tag->meta_attr) . '="' . check_plain($meta_tag->meta_attr_value) . '" content="' . check_plain($meta_tag->meta_content) . '"&gt;';
      $op = l(t('Edit'), $context_path . '/edit/' . check_plain($meta_tag->meta_id));
      $op .= ' | ';
      $op .= l(t('Remove'), $context_path . '/remove/' . check_plain($meta_tag->meta_id));
      $rows[] = array(
        $meta_tag->meta_url,
        $meta_tag->meta_attr,
        $tag,
        ($meta_tag->status) ? 'Enable' : 'Disable',
        $op,
      );
    }
  }
  else {
    $rows[] = array(array('data' => 'No tag exists.', 'colspan' => 5));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Implements hook_form().
 */
function context_meta_form($form, &$form_state, $edit = NULL) {
  $form = array();
  $form['description'] = array(
    '#prefix' => '<div>',
    '#markup' => 'The Context Meta module provides an interface for adding meta tags. This will add the created meta tags to the html head of all non-admin pages on the basis of defined page URL. Be careful with the http-equiv meta attribute, these are added to all defined <strong>PAGES</strong>, so you could very easily break your site with redirects, expires, etc.<br /><br />Formatting for tags <em>&lt;meta [Meta Attribute]="[Attribute Value]" content="[Content value]"&gt;</em>',
    '#suffix' => '</div>',
  );
  $form['meta_url'] = array(
    '#title' => t('Page URL'),
    '#type' => 'textfield',
    '#description' => t('Page URL for which context meta is defined'),
    '#default_value' => isset($edit->meta_url) ? $edit->meta_url : '',
    '#required' => TRUE,
  );
  $form['meta_attr'] = array(
    '#title' => t('Meta attribute'),
    '#type' => 'select',
    '#default_value' => isset($edit->meta_attr) ? $edit->meta_attr : '',
    '#options' => array('' => t('- Select -') , 'property' => t('property') , 'name' => t('name') , 'http-equiv' => t('http-equiv')),
    '#required' => TRUE,
  );
  $form['meta_attr_value'] = array(
    '#title' => t('Attribute value'),
    '#type' => 'textfield',
    '#description' => t('Value for the attribute defined above'),
    '#default_value' => isset($edit->meta_attr_value) ? $edit->meta_attr_value : '',
    '#required' => TRUE,
  );
  $form['meta_content'] = array(
    '#title' => t('Content value'),
    '#type' => 'textfield',
    '#description' => t('Value for the meta content defined above'),
    '#default_value' => isset($edit->meta_content) ? $edit->meta_content : '',
    '#required' => TRUE,
  );

  $form['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node', 'term', 'user'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
    '#dialog' => TRUE,
    '#prefix' => '<div style="margin-bottom: 1em">',
    '#suffix' => '</div>',
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => isset($edit->status) ? $edit->status : '',
  );

  $form['meta_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($edit->meta_id) ? $edit->meta_id : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Implements hook_form_validate().
 */
function context_meta_form_validate($form, &$form_state) {
  $atvalue = $form_state['values']['meta_attr_value'];
  $metacontent = $form_state['values']['meta_content'];
  if (empty($atvalue)) {
    form_set_error('meta_attr_value', t('Must provide a value for meta attribute'));
  }
  if (empty($metacontent)) {
    form_set_error('meta_content', t('Must provide a value for content'));
  }

  $item = &$form_state['values'];
  $normal_path = drupal_get_normal_path($item['meta_url']);
  if ($item['meta_url'] != $normal_path) {
    drupal_set_message(t('The menu system stores system paths only, but will use the URL alias for display. %link_path has been stored as %normal_path', array('%link_path' => $item['meta_url'], '%normal_path' => $normal_path)));
    $item['meta_url'] = $normal_path;
  }
  if (!trim($item['meta_url']) || !drupal_valid_path($item['meta_url'], TRUE)) {
    form_set_error('meta_url', t("The path '@link_path' is either invalid or you do not have access to it.", array('@link_path' => $item['meta_url'])));
  }

  $meta_id = isset($form_state['values']['meta_id']) ? $form_state['values']['meta_id'] : 0;
  $query = db_select('context_meta', 'cm');
  $query->fields('cm', array('meta_id'));
  $query->condition('cm.meta_url', $normal_path, '=');
  $query->condition('cm.meta_attr', $form_state['values']['meta_attr'], '=');
  $query->condition('cm.meta_attr_value', $atvalue, '=');
  $query->condition('cm.meta_id', $meta_id, '<>');
  $id = $query->execute()->fetchField();
  if ($id) {
    form_set_error('meta_url', t('Duplicate Page URL for same attribute'));
  }
}

/**
 * Implements hook_form_submit().
 */
function context_meta_form_submit(&$form, &$form_state) {
  if (empty($form_state['values']['meta_id'])) {
    drupal_set_message(t('Context tag for <em>@link</em> is save successfully.', array('@link' => $form_state['values']['meta_url'])));
    drupal_write_record('context_meta', $form_state['values']);
  }
  else {
    drupal_write_record('context_meta', $form_state['values'], array('meta_id'));
    drupal_set_message(t('Context tag for <em>@link</em> updated successfully.', array('@link' => $form_state['values']['meta_url'])));
  }
  $form_state['redirect'] = 'admin/config/context-meta/list';
}
