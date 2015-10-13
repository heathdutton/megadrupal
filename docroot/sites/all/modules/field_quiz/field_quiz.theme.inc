<?php
/**
 * @file
 * Theming functions for the field for creating a single multiple choice question with answers in a node
 */


/**
 * Theme the sorry, wrong answer page
 */
function theme_field_quiz_page_wrong_answer($vars) {
  $content = '';
  $content .= '<h2>' . t('Please try again!') . '</h2>';
  $content .= "<br />\n";
  $content .= "<br />\n";
  $content .= l(t('Retry'), 'node/' . $vars['nid']);
  return $content;
}


/**
 * correct answer page
 */
function theme_field_quiz_page_correct_answer($vars) {
    $content = '';
    $content .= '<h2>'. t('Correct!').'</h2>';
    $content .= t('Congratulations. This was the correct answer.');
    return $content;
}

