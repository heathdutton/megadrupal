<?php
/**
 * @file
 * Module forms.
 */

/**
 * Solr Query builder main form.
 */
function solr_qb_builder_form($form, &$form_state) {
  if (isset($form_state['#message'])) {
    $form['message'] = array(
      '#markup' => $form_state['#message'],
    );
  }
  $form['qt'] = array(
    '#title' => 'qt',
    '#description' => t('Request-Handler'),
    '#type' => 'textfield',
    '#default_value' => '/select',
  );
  $form['common'] = array(
    '#title' => 'common',
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );
  $form['common']['q'] = array(
    '#title' => 'q',
    '#description' => t('The query string'),
    '#type' => 'textarea',
    '#default_value' => '*:*',
  );
  $form['common']['fq'] = array(
    '#title' => t('fq'),
    '#description' => t('Filter query'),
    '#type' => 'textfield',
  );
  $form['common']['sort'] = array(
    '#title' => t('sort'),
    '#description' => t('Sort field or function with asc|desc'),
    '#type' => 'textfield',
  );
  $form['common']['start'] = array(
    '#title' => 'start',
    '#description' => t('Number of leading documents to skip. (Integer)'),
    '#type' => 'textfield',
  );
  $form['common']['rows'] = array(
    '#title' => 'rows',
    '#description' => t('Number of documents to return after "start". (Integer)'),
    '#type' => 'textfield',
  );
  $form['common']['fl'] = array(
    '#title' => 'fl',
    '#description' => t('Field list, comma separated'),
    '#type' => 'textfield',
  );
  $form['common']['df'] = array(
    '#title' => 'df',
    '#description' => t('Default search field'),
    '#type' => 'textfield',
  );
  $form['common']['raw'] = array(
    '#title' => t('Raw Query Parameters'),
    '#type' => 'textfield',
  );
  $form['common']['indent'] = array(
    '#title' => 'indent',
    '#description' => t('Indent results'),
    '#type' => 'checkbox',
  );
  $form['common']['debug_query'] = array(
    '#title' => 'debugQuery',
    '#description' => t('Show timing and diagnostics.'),
    '#type' => 'checkbox',
  );

  // Dismax.
  $form['dismax'] = array(
    '#title' => 'dismax',
    '#type' => 'checkbox',
  );
  $form['dismax_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="dismax"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['dismax_wrapper']['q.alt'] = array(
    '#title' => 'q.alt',
    '#description' => t('Alternate query when "q" is absent.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['qf'] = array(
    '#title' => 'qf',
    '#description' => t('Query fields with optional boosts.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['mm'] = array(
    '#title' => 'mm',
    '#description' => t('Min-should-match expression.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['pf'] = array(
    '#title' => 'pf',
    '#description' => t('Phrase boosted fields.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['ps'] = array(
    '#title' => 'ps',
    '#description' => t('Phrase boost slop.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['qs'] = array(
    '#title' => 'qs',
    '#description' => t('Query string phrase slop.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['tie'] = array(
    '#title' => 'tie',
    '#description' => t('Score tie-breaker. Try 0.1.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['bq'] = array(
    '#title' => 'bq',
    '#description' => t('Boost query.'),
    '#type' => 'textfield',
  );
  $form['dismax_wrapper']['bf'] = array(
    '#title' => 'bf',
    '#description' => t('Boost function (added).'),
    '#type' => 'textfield',
  );

  // eDismax.
  $form['edismax'] = array(
    '#title' => 'edismax',
    '#type' => 'checkbox',
  );
  $form['edismax_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="edismax"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['edismax_wrapper']['q.alt'] = array(
    '#title' => 'q.alt',
    '#description' => t('Alternate query when "q" is absent.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['qf'] = array(
    '#title' => 'qf',
    '#description' => t('Query fields with optional boosts.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['mm'] = array(
    '#title' => 'mm',
    '#description' => t('Min-should-match expression.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['pf'] = array(
    '#title' => 'pf',
    '#description' => t('Phrase boosted fields.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['ps'] = array(
    '#title' => 'ps',
    '#description' => t('Phrase boost slop.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['qs'] = array(
    '#title' => 'qs',
    '#description' => t('Query string phrase slop.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['tie'] = array(
    '#title' => 'tie',
    '#description' => t('Score tie-breaker. Try 0.1.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['bq'] = array(
    '#title' => 'bq',
    '#description' => t('Boost query.'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['bf'] = array(
    '#title' => 'bf',
    '#description' => t('Boost function (added).'),
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['uf'] = array(
    '#title' => 'uf',
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['pf2'] = array(
    '#title' => 'pf2',
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['pf3'] = array(
    '#title' => 'pf3',
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['ps2'] = array(
    '#title' => 'ps2',
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['ps3'] = array(
    '#title' => 'ps3',
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['boost'] = array(
    '#title' => 'boost',
    '#type' => 'textfield',
  );
  $form['edismax_wrapper']['stopwords'] = array(
    '#title' => 'stopwords',
    '#description' => t('Remove stopwords from mandatory "matching" component'),
    '#type' => 'checkbox',
  );
  $form['edismax_wrapper']['lowercaseOperators'] = array(
    '#title' => 'lowercaseOperators',
    '#description' => t('Enable lower-case "and" and "or" as operators'),
    '#type' => 'checkbox',
  );

  // hl.
  $form['hl'] = array(
    '#title' => 'hl',
    '#type' => 'checkbox',
  );
  $form['hl_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="hl"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['hl_wrapper']['hl.fl'] = array(
    '#title' => 'hl.fl',
    '#description' => t('Fields to highlight on.'),
    '#type' => 'textfield',
  );
  $form['hl_wrapper']['hl.simple.pre'] = array(
    '#title' => 'hl.simple.pre',
    '#description' => t('Score tie-breaker. Try 0.1.'),
    '#type' => 'textfield',
  );
  $form['hl_wrapper']['hl.simple.post'] = array(
    '#title' => 'hl.simple.post',
    '#description' => t('Score tie-breaker. Try 0.1.'),
    '#type' => 'textfield',
  );
  $form['hl_wrapper']['hl.requireFieldMatch'] = array(
    '#title' => 'hl.requireFieldMatch',
    '#type' => 'checkbox',
  );
  $form['hl_wrapper']['hl.usePhraseHighlighter'] = array(
    '#title' => 'hl.usePhraseHighlighter',
    '#type' => 'checkbox',
  );
  $form['hl_wrapper']['hl.highlightMultiTerm'] = array(
    '#title' => 'hl.highlightMultiTerm',
    '#type' => 'checkbox',
  );

  // Facet.
  $form['facet'] = array(
    '#title' => 'facet',
    '#type' => 'checkbox',
  );
  $form['facet_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="facet"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['facet_wrapper']['facet.query'] = array(
    '#title' => 'facet.query',
    '#type' => 'textarea',
  );
  $form['facet_wrapper']['facet.field'] = array(
    '#title' => 'facet.field',
    '#type' => 'textfield',
  );
  $form['facet_wrapper']['facet.prefix'] = array(
    '#title' => 'facet.prefix',
    '#type' => 'textfield',
  );

  // Spatial.
  $form['spatial'] = array(
    '#title' => 'spatial',
    '#type' => 'checkbox',
  );
  $form['spatial_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="spatial"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['spatial_wrapper']['pt'] = array(
    '#title' => 'pt',
    '#type' => 'textfield',
  );
  $form['spatial_wrapper']['sfield'] = array(
    '#title' => 'sfield',
    '#type' => 'textfield',
  );
  $form['spatial_wrapper']['d'] = array(
    '#title' => 'd',
    '#type' => 'textfield',
  );

  // Spellcheck.
  $form['spellcheck'] = array(
    '#title' => 'spellcheck',
    '#type' => 'checkbox',
  );
  $form['spellcheck_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="spellcheck"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['spellcheck_wrapper']['spellcheck.build'] = array(
    '#title' => 'spellcheck.build',
    '#type' => 'checkbox',
  );
  $form['spellcheck_wrapper']['spellcheck.reload'] = array(
    '#title' => 'spellcheck.reload',
    '#type' => 'checkbox',
  );
  $form['spellcheck_wrapper']['spellcheck.q'] = array(
    '#title' => 'spellcheck.q',
    '#type' => 'textfield',
  );
  $form['spellcheck_wrapper']['spellcheck.dictionary'] = array(
    '#title' => 'spellcheck.dictionary',
    '#type' => 'textfield',
  );
  $form['spellcheck_wrapper']['spellcheck.count'] = array(
    '#title' => 'spellcheck.count',
    '#type' => 'textfield',
  );
  $form['spellcheck_wrapper']['spellcheck.onlyMorePopular'] = array(
    '#title' => 'spellcheck.onlyMorePopular',
    '#type' => 'checkbox',
  );
  $form['spellcheck_wrapper']['spellcheck.extendedResults'] = array(
    '#title' => 'spellcheck.extendedResults',
    '#type' => 'checkbox',
  );
  $form['spellcheck_wrapper']['spellcheck.collate'] = array(
    '#title' => 'spellcheck.collate',
    '#type' => 'checkbox',
  );
  $form['spellcheck_wrapper']['spellcheck.maxCollations'] = array(
    '#title' => 'spellcheck.maxCollations',
    '#type' => 'textfield',
  );
  $form['spellcheck_wrapper']['spellcheck.maxCollationTries'] = array(
    '#title' => 'spellcheck.maxCollationTries',
    '#type' => 'textfield',
  );
  $form['spellcheck_wrapper']['spellcheck.accuracy'] = array(
    '#title' => 'spellcheck.accuracy',
    '#type' => 'textfield',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Execute Query'),
  );

  return $form;
}

/**
 * Custom query form.
 */
function solr_qb_custom_form($form, &$form_state) {
  if (isset($form_state['#message'])) {
    $form['message'] = array(
      '#markup' => $form_state['#message'],
    );
  }
  $form['query'] = array(
    '#title' => t('Query'),
    '#type' => 'textarea',
    '#description' => t('Example: @example (Do not use "wt" parameter. It will be provided automatically.)', array('@example' => 'q=*:*')),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('solr_qb_builder_form_submit'),
    '#value' => t('Execute Query'),
  );
  return $form;
}

/**
 * Submit for Solr Query builder form.
 */
function solr_qb_builder_form_submit($form, &$form_state) {

  $values = isset($form_state['values']['query']) ? $form_state['values']['query'] : $form_state['values'];

  try {
    $driver = solr_qb_get_driver();
    $response = $driver->query($values);
  }
  catch (Exception $e) {
    $response = $e->getMessage();
  }

  if (module_exists('devel')) {
    $form_state['#message'] = kprint_r($response, TRUE);
  }
  else {
    $form_state['#message'] = print_r($response, TRUE);
  }
  $form_state['rebuild'] = TRUE;
}
