<?php

/**
 * Implements hook_token_info().
 */
function periodical_token_info() {
  $types['periodical_issue'] = array(
    'name' => t('Generated Issue'),
    'description' => t('Tokens usable during issue generation of a periodical.'),
    'needs-data' => 'periodical_issue',
  );
  $types['periodical_issue_number'] = array(
    'name' => t('Issue number'),
    'description' => t('Sequential issue number.'),
    'needs-data' => 'periodical_issue_number',
  );


  $tokens['periodical_issue']['date'] = array(
    'name' => t("Issue Date"),
    'description' => t("Publication date of an issue"),
    'type' => 'date',
  );
  $tokens['periodical_issue']['number'] = array(
    'name' => t("Issue Number"),
    'description' => t("Sequential issue number"),
    'type' => 'periodical_issue_number',
  );
  $tokens['periodical_issue_number']['pad'] = array(
    'name' => t("Zero Padded Issue Number"),
    'description' => t("Sequential issue number with zero padding. Specify the number of digits after the colon."),
    'dynamic' => true,
  );

  return array(
    'types' => $types,
    'tokens' => $tokens,
  );
}

/**
 * Implements hook_tokens().
 */
function periodical_tokens($type, $tokens, array $data = array(), array $options = array()) {
  if (isset($options['language'])) {
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $replacements = array();

  if ($type == 'periodical_issue' && !empty($data['periodical_issue'])) {
    $periodical_issue = $data['periodical_issue'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'number':
          $replacements[$original] = $periodical_issue['number'];
          break;

        case 'date':
          $replacements[$original] = format_date($periodical_issue['date'], 'medium', '', NULL, $language_code);
          break;
      }
    }

    if ($number_tokens = token_find_with_prefix($tokens, 'number')) {
      $replacements += token_generate('periodical_issue_number', $number_tokens,
        array('periodical_issue_number' => $periodical_issue['number']), $options);
    }

    if ($date_tokens = token_find_with_prefix($tokens, 'date')) {
      $replacements += token_generate('date', $date_tokens,
        array('date' => $periodical_issue['date']), $options);
    }
  }

  if ($type == 'periodical_issue_number' && is_numeric($data['periodical_issue_number'])) {
    $number = $data['periodical_issue_number'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'pad':
          $replacements[$original] = $number;
          break;
      }
    }

    if ($pad_tokens = token_find_with_prefix($tokens, 'pad')) {
      foreach($pad_tokens as $digits => $original) {
        $replacements[$original] = str_pad($number, $digits, '0', STR_PAD_LEFT);
      }
    }
  }

  return $replacements;
}

