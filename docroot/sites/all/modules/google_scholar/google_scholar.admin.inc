<?php
/**
 * @file
 * Google Scholar module settings UI.
 */

/**
 * Admin settings form.
 */
function google_scholar_admin_settings($form, &$form_state) {
    $node_types = array();
    $entities = entity_get_info('node');
    foreach ($entities['bundles'] as $key => $value) {
        $node_types[$key] = t($value['label']);
    }

    $form['google_scholar_node_type'] = array(
        '#type' => 'select',
        '#title' => t('Select Content Type'),
        '#required' => TRUE,
        '#options' => $node_types,
        '#default_value' => variable_get('google_scholar_node_type'),
    );

    $form = system_settings_form($form);

    $form['#submit'][] = 'google_scholar_admin_settings_submit';
    $form['actions']['submit']['#value'] = 'Maps Google Scholar Fields';

    return $form;
}

/**
 * Admin settings form submits.
 */
function google_scholar_admin_settings_submit($form, &$form_state) {
    $form_state['redirect'] = 'admin/config/search/google_scholar/' . $form_state['values']['google_scholar_node_type'];
}

/**
 * Field mapping form for node types.
 */
function google_scholar_field_mappings($form, &$form_state, $node_type) {
    $node_fields = field_info_instances('node', $node_type->type);
    $fields = array(
        '' => t('- Select -'),
        'title' => t('Node Title'),
    );
    foreach ($node_fields as $key => $value) {
        $fields[$key] = t($value['label']);
    }

    $form['google_scholar_citation_title'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation Title Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_title'),
        '#required' => TRUE,
    );

    $form['google_scholar_citation_author'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation Authors Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_author'),
    );

    $form['google_scholar_citation_publication_date'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation Publication Date Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_publication_date'),
    );

    $form['google_scholar_citation_journal_title'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation Journal Title Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_journal_title'),
    );

    $form['google_scholar_citation_volume'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation Volume Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_volume'),
    );

    $form['google_scholar_citation_issue'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation Issue Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_issue'),
    );

    $form['google_scholar_citation_firstpage'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation First Page Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_firstpage'),
    );

    $form['google_scholar_citation_lastpage'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation Last Page Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_lastpage'),
    );

    $form['google_scholar_citation_pdf_url'] = array(
        '#type' => 'select',
        '#title' => t('Select Citation PDF URL Field'),
        '#options' => $fields,
        '#default_value' => variable_get('google_scholar_citation_pdf_url'),
    );

    return system_settings_form($form);
}