<?php

/**
 * @file
 * Helper functions for the power ballad, mostly providing it with content.
 *
 * This include file contains mostly big arrays with ballad content,
 * which can be loaded and displayed by the ballad main functions.
 */


/**
 * Loads an array with a chosen aspect of the power ballad.
 *
 * This function loads a given array with an aspect of the power ballad,
 * such as structure, chords and lyrics. The result is an array with
 * one entry for each cadence. A message is displayed if loading fails.
 *
 * @param &$ballad
 *   The array in which to load aspects.
 *
 * @param $aspect
 *   The aspect to load. Available aspects are (for now) structure, chords
 *   and lyrics.
 *
 * @return
 *   TRUE if the aspect loading was successful, FALSE if otherwise.
 */

function powerballad_load_aspect(&$ballad = array(), $aspect = 'lyrics') {

  switch ($aspect) {
    case 'structure':
      // Load the structure of the ballad
      $pieces = array(
        t('Intro'),
        t(''),
        t(''),
        t(''),
        t('Verse 1'),
        t(''),
        t(''),
        t(''),
        t('Chorus 1'),
        t(''),
        t('Interlude'),
        t('Verse 2'),
        t(''),
        t(''),
        t(''),
        t('Chorus 2'),
        t(''),
        t(''),
        t(''),
        t('Bridge'),
        t(''),
        t(''),
        t(''),
        t('Solo break'),
        t(''),
        t(''),
        t(''),
        t('Interlude'),
        t('Verse 3'),
        t(''),
        t(''),
        t(''),
        t('Chorus 3'),
        t(''),
        t(''),
        t(''),
        t('Power chorus'),
        t(''),
        t(''),
        t(''),
        t('Chorus 4'),
        t(''),
        t(''),
        t(''),
        t('Outro'),
      );
    break;

    case 'chords':
      // Load the chords of the ballad
      $pieces = array(
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('C   D   Em      '),
        t('C   D   Em      '),
        t('C   D   Em      '),
        t('C   D   Em      '),
        t('Em  D   C   D   '),
        t('Em  D   C   D   '),
        t('Em  D   C   D   '),
        t('Em  D   C---    '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
        t('G   Bm  Am  C D '),
      );
    break;

    case 'lyrics':
      // Load the lyrics of the ballad
      $pieces = array(
        t(''),
        t(''),
        t(''),
        t(''),
        t('It feels so lonely late at night /
        wrapping up a piece of code'),
        t('Looking for some beauty in life /
        but there\'s no API for theme_love_load()'),
        t('The code might be clean and it might be safe /
        but there\'s something missing'),
        t('There\'s too little Drupal love /
        my code needs hugging and kissing'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t(''),
        t('Slowly giving up on the Color module /
        and everything just seems blue'),
        t('Can\'t you see, see – \'kay – /
        Can\'t you see that I need you?'),
        t('The patch is green and it\'s performing well'),
        t('But there\'s no pretending – we all can tell /
        I might be smart but it\'s ugly as hell'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I am nothing without you /
        A displayless View /
        I\'m like Drupal 2.2 /
          Yeah'),
        t('I\'m an action with no hook /
        The Drupal course I never took /
        I\'m a pageless book /
          Yeah'),
        t('I\'m a string without a t() /
        I\'m a value without key /
        I am Drupal with a fee /
          Yeah'),
        t('I\'m the drush I didn\'t make /
        The only thought when I\'m awake /
        I\'m a power ballade /
          without a solo break – yeah!'),
        t(''),
        t(''),
        t(''),
        t(''),
        t(''),
        t('Wrapping up some code late at night /
        pushing up a final commit'),
        t('Looking for some beauty in my life /
        but I don\'t know how to get it'),
        t('Time to get some sleep and reboot my brain'),
        t('The keyboard is finally silent /
        but the monitor is screaming of pain'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme'),
        t('I can be your module, you can be my theme /
        Again and again and again…'),
        t(''),
      );
    break;

    default:
      drupal_set_message(t('Could not load aspect %aspect. Not enough power in the ballad yet.', array('%aspect' => $aspect)), 'warning');
      return FALSE;

  }


  // Insert the aspect into the $ballad
  foreach ($pieces AS $row => $piece) {
    $ballad[$row][$aspect] = $piece;
  }

  return TRUE;
}
