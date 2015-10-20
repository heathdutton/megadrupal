<?php

/**
 * @file
 * PMB reader suggestions.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_reader_suggestions($user) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $suggestions = $pmb_data->reader_get_suggestions();

  drupal_set_title(t('My reader account: Suggestions'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account'), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = t('Suggestions');
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_reader_suggestions', array(
    'reader' => $user,
    'suggestions' => $suggestions,
    'parameters' => array(),
  ));
}

function pmb_reader_suggestion_edit($user, $suggestion_id) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $categ_and_sources = $pmb_data->reader_get_suggestion_sources_and_categories();

  drupal_set_title(t('View, add or edit a suggestion'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account'), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = l(t('Suggestions'), _pmb_p('reader/') . $user->uid . _pmb_p('/suggestion'));
  $breadcrumb[] = ($suggestion_id == _pmb_p('add')) ? t('Add') : t('Edit');
  drupal_set_breadcrumb($breadcrumb);

  $suggestion = array();
  // Get suggestion to be edited before going to form.
  if ($suggestion_id != _pmb_p('add')) {
    $suggestion_id += 0;
    if ($suggestion_id) {
      $suggestions = $pmb_data->reader_get_suggestions();

      foreach ($suggestions as $asuggestion) {
        if ($asuggestion->sugg_id == $suggestion_id) {
          $suggestion = $asuggestion;
          break;
        }
      }

      if (!$suggestion) {
        drupal_set_title(t('Suggestion not found!'));
        drupal_set_message(t('Suggestion not found!'), 'error');
        return '';
      }
    }
  }

  $result = drupal_get_form('pmb_reader_suggestion_edit_form', $suggestion, $categ_and_sources);

  return $result;
}
