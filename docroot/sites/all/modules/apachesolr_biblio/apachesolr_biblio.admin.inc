<?php

/**
 * Create settings form.
 *
 * @return array.
 *   Form with current apachesolr_biblio settings.
 */
function apachesolr_biblio_fields_form($form, $form_state) {

  $defaults = _apachesolr_biblio_get_fields();
  $form['contributors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Contributors'),
    '#description' => t('Should authors be indexed? This feature is either on or off. No control is given to index some author types and not others.'),
    '#default_value' => $defaults['contributors'],
  );

  $fields = _apachesolr_biblio_get_raw_fields();
  $form['configured_flds'] = array('#tree' => 1);
  foreach ($fields as $key => $fld) {
    // If a string contains the word "author", it's
    // a multiple string that gets packed into "contributors".
    // These get handled all at once, basically, so we ignore them.
    if (strpos($fld['name'], 'author')) {
      continue;
    }

    $form['configured_flds'][$key]['display_name'] = array(
      '#type' => 'item',
      '#markup' => $fld['name'],
      '#weight' => $fld['weight'],
    );
    $form['configured_flds'][$key]['name'] = array(
      '#type' => 'value',
      '#value' => $fld['name'],
    );
    $form['configured_flds'][$key]['title'] = array(
      '#type' => 'item',
      '#markup' => $fld['title'],
      '#weight' => $fld['weight'],
    );
    $options = array();
    if (isset($defaults[$key]['indexed']) && $defaults[$key]['indexed']) {
      $options[] = 'indexed';
    }
    if (isset($defaults[$key]['multiple']) && $defaults[$key]['multiple']) {
      $options[] = 'multiple';
    }
    if (isset($defaults[$key]['sortable']) && $defaults[$key]['sortable']) {
      $options[] = 'sortable';
    }
    if (isset($defaults[$key]['facetable']) && $defaults[$key]['facetable']) {
      $options[] = 'facetable';
    }
    $form['configured_flds'][$key]['checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => array(
        'indexed' => '',
        'multiple' => '',
        'sortable' => '',
        'facetable' => '',
      ),
      '#default_value' => $options,
      '#weight' => $fld['weight'],
    );
    $form['configured_flds'][$key]['index_type'] = array(
      '#type' => 'select',
      '#options' => array(
        'string' => t('String'),
        'text' => t('Text'),
        'date' => t('Date'),
      ),
      '#multiple' => FALSE,
      '#default_value' => (isset($defaults[$key]['index_type']) ? $defaults[$key]['index_type'] : 'string'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Theme apachesolr_biblio settings form.
 */
function theme_apachesolr_biblio_fields_form($variables) {
  $form = $variables['element'];
  $output = drupal_render($form['contributors']);

  $conf_table = array();
  foreach (element_children($form['configured_flds']) as $fld) {
    $form['configured_flds'][$fld]['weight']['#attributes']['class'] = 'weight';
    $conf_row = array();
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['display_name']));
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['title']));
    foreach (element_children($form['configured_flds'][$fld]['checkboxes']) as $oid) {
      if (is_array($form['configured_flds'][$fld]['checkboxes'])) {
        $conf_row[] = array(
          'data' => drupal_render($form['configured_flds'][$fld]['checkboxes'][$oid]),
          'title' => $oid,
        );
      }
    }
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['index_type']));
    $conf_table[] = array('data' => $conf_row);
  }

  $header = array(
    t('Field Name'),
    t('Title'),
    t('Index'),
    t('Multiple'),
    t('Sortable'),
    t('Facetable'),
    t('Data type'),
  );

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $conf_table,
    'attributes' => array('id' => 'field-table'),
  ));
  $output .= drupal_render($form['submit']);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Save form values to database.
 */
function apachesolr_biblio_fields_form_submit($form, $form_state) {
  $biblio_index_records = array();

  foreach ($form_state['values']['configured_flds'] as $fid => $field) {
    $record = new stdClass();
    $record->fid = $fid;

    // Retrieve form values.
    $record->name = $field['name'];
    $record->index_type = $field['index_type'];
    $record->indexed = ($field['checkboxes']['indexed'] == '0' ? 0 : 1);
    $record->multiple = ($field['checkboxes']['multiple'] == '0' ? 0 : 1);
    $record->sortable = ($field['checkboxes']['sortable'] == '0' ? 0 : 1);
    $record->facetable = ($field['checkboxes']['facetable'] == '0' ? 0 : 1);

    // Always index if sortable of facetable is enabled.
    if ($record->sortable || $record->facetable) {
      $record->indexed = 1;
    }

    // Save record to variable table.
    variable_set('apachesolr_biblio_index_' . $record->name, json_encode($record));
  }

  // Author.
  variable_set('apachesolr_biblio_index_authors', $form_state['values']['contributors']);

  drupal_set_message(t("Apachesolr Biblio Settings have been updated. If you added new fields to the index you have to reindex."), "status");
}
