<?php

/**
 * @file
 * Builds placeholder replacement tokens for suggestion.
 */

/**
 * Implements hook_token_info().
 */
function opac_suggestions_token_info() {
  $types['suggestion'] = array(
    'name' => t('Suggestion'),
    'description' => t('Tokens related to opac suggestions.'),
    'needs-data' => 'suggestion',
  );
  $suggestion['id'] = array(
    'name' => t("Suggestion ID"),
    'description' => t("Unique identifier for the suggestion in ILS."),
  );
  $suggestion['branchcode'] = array(
    'name' => t("Branch code"),
    'description' => t("The library or site code the suggestion is for."),
  );
  $suggestion['branchname'] = array(
    'name' => t("Branch name"),
    'description' => t("The library or site name the suggestion is for."),
  );
  $suggestion['title'] = array(
    'name' => t("Title"),
    'description' => t("Suggestion title."),
  );
  $suggestion['author'] = array(
    'name' => t("Author"),
    'description' => t("Suggestion author."),
  );
  $suggestion['publicationyear'] = array(
    'name' => t("Publication year"),
    'description' => t("Year the suggestion has been published."),
  );
  $suggestion['isbn'] = array(
    'name' => t("Standard number"),
    'description' => t("Standard number of document."),
  );
  $suggestion['publisher'] = array(
    'name' => t("Publisher name"),
    'description' => t("Suggestion publisher name."),
  );
  $suggestion['collection'] = array(
    'name' => t("Collection title"),
    'description' => t("Collection title of the document."),
  );
  $suggestion['publisherplace'] = array(
    'name' => t("Publisher place"),
    'description' => t("Suggestion publisher place."),
  );
  $suggestion['doctype'] = array(
    'name' => t("Document type code"),
    'description' => t("Suggestion document type."),
  );
  $suggestion['doctypename'] = array(
    'name' => t("Document type label"),
    'description' => t("Suggestion document type label."),
  );
  $suggestion['note'] = array(
    'name' => t("Notes"),
    'description' => t("Suggestion notes."),
  );
  return array(
    'types' => $types,
    'tokens' => array('suggestion' => $suggestion),
  );
}

/**
 * Implements hook_tokens().
 */
function opac_suggestions_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'suggestion' && !empty($data['suggestion'])) {
    $suggestion = $data['suggestion'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = isset($suggestion->suggestionid) ? $suggestion->suggestionid : '';
          break;

        case 'branchname':
          $search = isset($suggestion->opac_suggestion_branchcode) ? $suggestion->opac_suggestion_branchcode : '';
          $replacements[$original] = '';
          $vid = variable_get('suggestions_branch_vocabulary', '');
          if ($vid) {
            $field = variable_get('suggestions_branch_field', 'name');
            if ($field) {
              module_load_include('inc', 'opac', "includes/helper.taxonomy");
              $tid = _opac_taxonomy_get_term($vid, $field, $search);
              $term = taxonomy_term_load($tid);
              if (is_object($term)) {
                $replacements[$original] = $term->name;
              }
            }
          }
          break;

        case 'doctypename':
          $search = isset($suggestion->opac_suggestion_doctype) ? $suggestion->opac_suggestion_doctype : '';
          $replacements[$original] = '';
          $vid = variable_get('suggestions_doctype_vocabulary', '');
          if ($vid) {
            $field = variable_get('suggestions_doctype_field', 'name');
            if ($field) {
              module_load_include('inc', 'opac', "includes/helper.taxonomy");
              $tid = _opac_taxonomy_get_term($vid, $field, $search);
              $term = taxonomy_term_load($tid);
              if (is_object($term)) {
                $replacements[$original] = $term->name;
              }
            }
          }
          break;

        default:
          $value = isset($suggestion->{"opac_suggestion_$name"}) ? $suggestion->{"opac_suggestion_$name"} : '';
          $replacements[$original] = $value;
      }
    }
  }

  return $replacements;
}
