<?php

require_once (drupal_get_path('module', 'document') . '/document.inc');

function document_search_form($form, $form_state) {
  drupal_add_css(drupal_get_path('module', 'document') . '/document.css');

  drupal_add_js(drupal_get_path('module', 'document') . '/document.js');
  _document_register_validation_token();
  _document_register_status();
  drupal_add_js('doc.initializeDocSearch();', 'inline');

  $ka = '';
  $field = '';
  if (array_key_exists('document_block1_input', $_GET)) {
    $ka = $_GET['document_block1_input'];
  }
  else if (array_key_exists('document_block2_input', $_GET)) {
    $ka = $_GET['document_block2_input'];
    $field = DOCUMENT_SEARCH_AUTHOR_KEYWORDS;
  }

  if (array_key_exists('document_block1_fields', $_GET)) {
    $field = $_GET['document_block1_fields'];
  }

  $docType = '';
  if ($field == -1) {
    $docType = $ka;
  }
  else if (array_key_exists('document_block2_doctype', $_GET)) {
    $docType = $_GET['document_block2_doctype'];
  }

  $form['document_search_fieldset'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#title' => t('Search Documents'),
      '#attributes' => array(
          'class' => array('document_search_fieldset')));

  $form['document_search_fieldset']['lbl_search_for'] = array(
      '#type' => 'item',
      '#title' => t('Search For'),
      '#prefix' => '<div class="search-row-1"><div class="lbl-search-for">',
      '#suffix' => '</div>');

  $form['document_search_fieldset']['search_text'] = array(
      '#type' => 'textfield',
      '#attributes' => array(
          'onkeypress' => 'return(doc.suppressSearchEnter(event));'),
      '#default_value' => ($field == '' || $field == -1) ? '' : $ka,
      '#prefix' => '<div class="search-text">',
      '#suffix' => '</div>');

  $loading = theme_image(array(
            'path' => document_image_url('loading.gif'), 
            'alt' => t('Loading...'),
            'title' => t('Loading...'),
            'attributes' => array(
                'id' => 'document_loading',
                'style' => 'display: none',
                'width' => 16,
                'height' => 16,
                )
            ));

  $form['document_search_fieldset']['search_button'] = array(
      '#type' => 'button',
      '#prefix' => '<div class="search-button form-item">',
      '#value' => t('Search'),
      '#attributes' => array(
          'onclick' => 'return(doc.performSearch());'),
      '#suffix' => '</div>' . $loading . '</div>');

  $form['document_search_fieldset']['lbl_search_in'] = array(
      '#type' => 'item',
      '#title' => t('Search In'),
      '#prefix' => '<div class="search-row-2"><div class="lbl-search-in">',
      '#suffix' => '</div>');

  $searchFields = array(t('Author') . '|', t('Keywords') . '|', t('Both') . '|');
  if (variable_get('document_allow_websearch', FALSE)) {
    $searchFields[] = t('None') . '|';
    $searchFields[] = t('Web');
  }
  else {
    $searchFields[] = t('None');
  }

  $form['document_search_fieldset']['search_fields'] = array(
      '#type' => 'radios',
      '#options' => $searchFields,
      '#default_value' => ($field == '' || $field == -1) ? DOCUMENT_SEARCH_NONE : $field,
      '#prefix' => '<div class="search-fields">',
      '#suffix' => '</div></div>');

  $form['document_search_fieldset']['lbl_search_year'] = array(
      '#type' => 'item',
      '#title' => t('Year of Publication'),
      '#prefix' => '<div class="search-row-3"><div class="lbl-search-year">',
      '#suffix' => '</div>');

  $years = drupal_map_assoc(document_get_years());
  $years[''] = t('--Select Year--');
  $years = array_reverse($years, TRUE);

  $form['document_search_fieldset']['search_year'] = array(
      '#type' => 'select',
      '#options' => $years,
      '#required' => TRUE,
      '#prefix' => '<div class="search-year">',
      '#suffix' => '</div>');

  $form['document_search_fieldset']['lbl_search_doctype'] = array(
      '#type' => 'item',
      '#title' => t('Document Type'),
      '#prefix' => '<div class="lbl-search-doctype">',
      '#suffix' => '</div>');

  $types = document_get_types(FALSE);
  $types[''] = t('--Select Type--');

  $form['document_search_fieldset']['search_doctype'] = array(
      '#type' => 'select',
      '#options' => $types,
      '#required' => TRUE,
      '#default_value' => $docType,
      '#prefix' => '<div class="search-doctype">',
      '#suffix' => '</div></div>');

  $table = NULL;
  if (!empty($ka) || !empty($docType)) {
    if ($field == -1) {
      $table = document_perform_search(DOCUMENT_SEARCH_NONE, $ka, NULL, $docType);
    }
    else {
      $table = document_perform_search($field, $ka, NULL, $docType);
    }
  }
  else {
    $conditions = db_and();
    $conditions->condition('n.status', DOCUMENT_STATUS_PUBLISHED);
    $table = document_search_table($conditions);
  }

  $form['document_results'] = array(
      '#type' => 'markup',
      '#markup' => $table,
      '#prefix' => '<div id="document_search_results">',
      '#suffix' => '</div>');

  return ($form);
}
