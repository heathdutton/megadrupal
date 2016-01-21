<?php
/**
 * @file
 * Page callback for ManyMail main page.
 */

/**
 * Custom page callback function.
 *
 * @see manymail_menu()
 */
function _manymail_page() {
  $page = array();

  $page['manymail_intro'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('This page is used as a portal for all ManyMail functionality.<br />Click any of the tabs to get started.'),
  );

  $page['manymail_intro_warning'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('<strong>Warning:</strong><br />Check everything you enter thoroughly.<br />Once you click any "Send" button, there is no going back!'),
  );

  return $page;
}
