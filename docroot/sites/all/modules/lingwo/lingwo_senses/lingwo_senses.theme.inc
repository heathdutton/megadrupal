<?php

/**
 * @file
 * Theme functions for lingwo_senses
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_senses_original_form($variables) {
  $form = $variables['form'];
  drupal_add_tabledrag('lingwo-senses-form-table', 'order', 'sibling', 'sense-weight');

  $elements = element_children($form);
  if (empty($elements)) {
    return '';
  }

  $header = array(
    '',
    array(
      'data' => t('Sense'),
      'width' => '100%',
    ),
    t('Weight'),
  );

  if (count($elements) > 1) {
    $header[] = t('Operations');
  }

  $rows = array();
  foreach (element_children($form) as $key) {
    if (!is_int($key)) {
      continue;
    }

    $element = &$form[$key];
    $element['weight']['#attributes']['class'] = array('sense-weight');

    $weight = drupal_render($element['weight']);
    $operations = drupal_render($element['operations']);

    $row = array();
    $row[] = '';
    $row[] = drupal_render($element);
    $row[] = $weight;

    if (count($elements) > 1) {
      $row[] = $operations;
    }

    $row = array('data' => $row);
    $row['class'][] = 'draggable';
    $rows[] = $row;
  }

  drupal_add_css(drupal_get_path('module', 'lingwo_senses') . '/lingwo_senses.css');

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'lingwo-senses-form-table'))) . drupal_render_children($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_senses_translation_form($variables) {
  $form = $variables['form'];
  $header = array(
    t('No.'),
    t('Sense'),
  );

  $rows = array();
  $elements = element_children($form);
  foreach ($elements as $key) {
    if (!is_int($key)) {
      continue;
    }

    $row = array();
    $row[] = ($key + 1);
    $row[] = drupal_render($form[$key]);

    $rows[] = $row;
  }

  drupal_add_css(drupal_get_path('module', 'lingwo_senses') . '/lingwo_senses.css');

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'lingwo-senses-form-table'))) . drupal_render_children($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_senses_original($variables) {
  $node = $variables['node'];
  // TODO: should we pass in an entry?
  $entry = LingwoEntry::fromNode($node);

  $output = '';
  if (count($entry->senses) > 0) {
    $output .= '<ol class="lingwo-senses">';
    foreach ($entry->senses as $sense) {
      $output .= '<li class="lingwo-sense lingwo-sense-id-' . $sense->id . '">';
      if (!empty($sense->is['relationship'])) {
        $output .= '<span class="lingwo-sense-relationship">' . theme('lingwo_senses_relationship', array('node' => $node, 'sense' => $sense)) . '</span>';
      }
      elseif (!empty($sense->difference)) {
        $output .= '<span class="lingwo-sense-difference">' . check_plain($sense->difference) . '</span>';
      }
      else {
        $output .= '<span class="lingwo-sense-no-difference">(';
        $output .= t('No difference given');
        $output .= ')</span>';
      }
      if (!empty($sense->example)) {
        $output .= ': <blockquote class="lingwo-sense-example">' . implode('<br />', array_filter($sense->example, 'check_plain')) . '</blockquote>';
      }
      $output .= '</li>';
    }
    $output .= '</ol>';
  }
  else {
    $output .= '<p>' . t('No sense distinctions.') . '</p>';
  }

  drupal_add_css(drupal_get_path('module', 'lingwo_senses') . '/lingwo_senses.css');
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_senses_translation($variables) {
  $node = $variables['node'];
  $options = $variables['options'];
  $options = (is_null($options) ? array() : $options) +
    array(
    'example' => TRUE,
    'contribute' => TRUE,
  );

  $output = '';
  $output .= '<ol class="lingwo-senses">';

  // TODO: should we just pass an entry to this theme function instead?
  $entry = LingwoEntry::fromNode($node);

  $without_translations = 0;
  foreach ($entry->senses as $sense) {
    if (!empty($sense->same_as)) {
      continue;
    }

    $values = array();

    if ($sense->no_equivalent) {
      $values[] = '<strong><span class="lingwo-sense-no-equivalent">' . t('No equivalent in this language') . '</span></strong>';
    }
    elseif (!empty($sense->is['relationship'])) {
      $values[] = theme('lingwo_senses_relationship', array('node' => $node, 'sense' => $sense));
    }
    elseif (!empty($sense->trans)) {
      foreach ($sense->trans as $value) {
        if (!empty($value)) {
          // TODO: re-instate this!
          //$values[] = l($value, 'entry/' . $trans->target_lang . '/' . $entry->pos . '/' . $value);
          $values[] = check_plain(trim($value));
        }
      }
    }
    if (empty($values)) {
      //$output .= t('No translation');
      $without_translations++;
      continue;
    }

    $output_ids = array();
    foreach ($sense->getIds() as $id) {
      $output_ids[] = 'lingwo-sense-id-' . $id;
    }
    $output_ids = implode(' ', $output_ids);

    $output .= '<li class="lingwo-sense ' . $output_ids . '">';
    if ($sense->clue) {
      $output .= '<span class="lingwo-sense-clue">(' . check_plain($sense->clue) . ')</span> ';
    }

    $output .= join(', ', $values);

    $translated_example = $sense->getTranslatedExample(TRUE);
    if ($options['example'] && count($translated_example) > 0) {
      $output .= '<blockquote class="lingwo-sense-example">';
      if (count($sense->example) == 1 && empty($sense->example_translation)) {
        $output .= check_plain($sense->example[0]);
      }
      else {
        $output .= '<table>';
        foreach ($translated_example as $line) {
          $output .= '<tr><td width="50%">' . check_plain($line[0]) . '</td><td width="50%">' . check_plain($line[1]) . '</td></tr>';
        }
        $output .= '</table>';
      }
      $output .= '</blockquote>';
    }
    $output .= '</li>';
  }
  $output .= '</ol>';

  if ($options['contribute'] && $without_translations > 0 && node_access('update', $node)) {
    $output .= '<p><strong>';
    $output .= t('There are @count senses without translations.', array('@count' => $without_translations));
    $output .= '  ';
    $output .= t('!edit and contribute!', array('!edit' => l(t('Edit this entry'), 'node/' . $node->nid . '/edit')));
    $output .= '</strong></p>';
  }

  drupal_add_css(drupal_get_path('module', 'lingwo_senses') . '/lingwo_senses.css');
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_lingwo_senses_relationship($variables) {
  $node = $variables['node'];
  $sense = $variables['sense'];
  if (_lingwo_is_translation($node)) {
    $source_lang = LingwoEntry::fromNode($node)->getSourceLanguage();
    $target_lang = $node->language;
    $spec = "$source_lang-$target_lang";
  }
  else {
    $spec = $source_lang = $target_lang = $node->language;
  }

  $pos = $sense->is['pos'];
  $headword = $sense->is['headword'];

  // TODO: this should be a function!
  // TODO: this should operate somehow, when we don't have a lookup path!
  $path = implode('/', array(
    LingwoEntry::$settings->lookup_path,
    $spec,
    $headword . '(' . $pos . ')',
  ));

  $entry_text = $headword . ' (' . LingwoEntry::getPosOptions($node->language, TRUE, $pos) . ')';
  // TODO: shouldn't be a link if there is no lookup path and this node doesn't exist!
  $entry_text = l($entry_text, $path, array(
    // setup for working in the korpus reader
    'attributes' => array(
      // TODO: hashing should be a function call!
      'data-entry' => sha1(join(':', array($source_lang, $pos, $headword))),
      'class' => array('anno'),
    ),
  ));

  return t('@rel of !entry', array(
    '@rel' => LingwoSenses::getRelationshipOptions($source_lang, TRUE, $sense->is['relationship']),
    '!entry' => $entry_text,
  ));
}

