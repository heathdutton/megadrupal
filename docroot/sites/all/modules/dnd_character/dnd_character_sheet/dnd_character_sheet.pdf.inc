<?php

/**
 * @file
 * Include file with PDF page definitions to handle the PDF character sheet
 * display.
 */

function dnd_character_sheet_display($node) {
  global $_dnd_character_sheet_p;
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;
  global $_dnd_character_sheet_row;

  $_dnd_character_sheet_p = new PDFlib();

  // US Letter page size in points
  $_dnd_character_sheet_page_width     = 612.0; //540 -> 612
  $_dnd_character_sheet_page_height    = 792.0; //720 -> 792

  $margin_height  = 36.0;
  $margin_width   = 36.0;

  $_dnd_character_sheet_xoffset        = $margin_width;
  $_dnd_character_sheet_yoffset        = $_dnd_character_sheet_page_height - $margin_height - DND_CHARACTER_SHEET_LINE_HEIGHT;


  $_dnd_character_sheet_row = array(0.0);

  //  open new PDF file; insert a file name to create the PDF on disk
  if ($_dnd_character_sheet_p->begin_document("", "destination={page=1 type=fixed zoom=1} viewerpreferences={printscaling=none centerwindow=true}") == 0) {
    $pdf_error = check_plain($_dnd_character_sheet_p->get_errmsg());
    drupal_set_message(t('Error creating PDF: %msg', array('%msg' => check_plain($pdf_error))), 'error');
    return FALSE;
  }

  // Set main PDF information
  $_dnd_character_sheet_p->set_info("Creator", "http://dnd.tobbyhagler.com");
  $_dnd_character_sheet_p->set_info("Author", "Tobby Hagler");
  $_dnd_character_sheet_p->set_info("Title", "D&D Character Record Sheet");
  $_dnd_character_sheet_p->set_info("Subject", "D&D Character Record Sheet");

  // Page 1
  $_dnd_character_sheet_p->begin_page_ext($_dnd_character_sheet_page_width, $_dnd_character_sheet_page_height, "");
    $_dnd_character_sheet_row = define_rows(1);

    // Display the basic information (name, race, class, etc.)
    _dnd_character_sheet_character_basic_info($node, $_dnd_character_sheet_p, $_dnd_character_sheet_row, $_dnd_character_sheet_xoffset, $_dnd_character_sheet_yoffset);

    // Display the primary stats (Ability Scores, Saves, HP, AC, Initiative, etc.)
    _dnd_character_sheet_character_main_stats($node, $_dnd_character_sheet_row, $_dnd_character_sheet_p);

    // Display the AC and other armor and protection stats
    _dnd_character_sheet_character_armor($node, $_dnd_character_sheet_row, $_dnd_character_sheet_p);

    // Display the combat stats
    _dnd_character_sheet_character_combat_stats($node, $_dnd_character_sheet_row, $_dnd_character_sheet_p);

    // Display the weapon info
    _dnd_character_sheet_character_weapons($node, $_dnd_character_sheet_row, $_dnd_character_sheet_p);

  $_dnd_character_sheet_p->end_page_ext("");


  // Page 2
  $_dnd_character_sheet_p->begin_page_ext($_dnd_character_sheet_page_width, $_dnd_character_sheet_page_height, "");
    $_dnd_character_sheet_row = define_rows(2);

  // Display skill points and skill assignments
  _dnd_character_sheet_character_skills($node, $_dnd_character_sheet_row, $_dnd_character_sheet_p);

  $_dnd_character_sheet_p->end_page_ext("");

  // Page 3
  $_dnd_character_sheet_p->begin_page_ext($_dnd_character_sheet_page_width, $_dnd_character_sheet_page_height, "");
    $_dnd_character_sheet_row = define_rows(3);
    // Display feats
    _dnd_character_sheet_character_feats($node, $_dnd_character_sheet_row, $_dnd_character_sheet_p);

  $_dnd_character_sheet_p->end_page_ext("");

  // Page 4
  $_dnd_character_sheet_p->begin_page_ext($_dnd_character_sheet_page_width, $_dnd_character_sheet_page_height, "");
    $_dnd_character_sheet_row = define_rows(4);

  // Display Equipment
  equipment($node);

  // Display Wealth
  wealth($node);

  // TODO: This PDF sheet has little error checking, so it throws lots of PHP strict warnings
  // about fields with illegal offsets for empty fields. This *CANNOT* leave alpha until this
  // is fixed.
  drupal_get_messages('error', TRUE);

  $_dnd_character_sheet_p->end_page_ext("");

  $_dnd_character_sheet_p->end_document("");

  // Output the PDF to the browser
  $buf = $_dnd_character_sheet_p->get_buffer();
  $len = strlen($buf);

  header("Content-type: application/pdf");
  header("Content-Length: $len");
  header("Content-Disposition: inline; filename=sheet" . time() . ".pdf");
  print $buf;
}



function _dnd_character_sheet_character_basic_info($node, &$_dnd_character_sheet_p, &$_dnd_character_sheet_row, &$_dnd_character_sheet_xoffset, &$_dnd_character_sheet_yoffset) {
  // Character Name
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[0], 310, $_dnd_character_sheet_row[0]);
  label_under($_dnd_character_sheet_p, "Character Name", 0, $_dnd_character_sheet_row[0]);
  underlined_text($_dnd_character_sheet_p, $node->title, 0, $_dnd_character_sheet_row[0]);

  // Race
  underline($_dnd_character_sheet_p, 315, $_dnd_character_sheet_row[0], 420, $_dnd_character_sheet_row[0]);
  label_under($_dnd_character_sheet_p, "Race", 315, $_dnd_character_sheet_row[0]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_race[$node->language][0]['value'], 315, $_dnd_character_sheet_row[0]);

  // Class
  underline($_dnd_character_sheet_p, 425, $_dnd_character_sheet_row[0], 540, $_dnd_character_sheet_row[0]);
  label_under($_dnd_character_sheet_p, "Class", 425, $_dnd_character_sheet_row[0]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_class_level[$node->language][0]['class'], 425, $_dnd_character_sheet_row[0]);

  // Alignment
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[1], 165, $_dnd_character_sheet_row[1]);
  label_under($_dnd_character_sheet_p, "Alignment", 0, $_dnd_character_sheet_row[1]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_alignment[$node->language][0]['value'], 0, $_dnd_character_sheet_row[1]);

  // Deity
  underline($_dnd_character_sheet_p, 170, $_dnd_character_sheet_row[1], 310, $_dnd_character_sheet_row[1]);
  label_under($_dnd_character_sheet_p, "Deity", 170, $_dnd_character_sheet_row[1]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_deity[$node->language][0]['value'], 170, $_dnd_character_sheet_row[1]);

  // Level
  underline($_dnd_character_sheet_p, 315, $_dnd_character_sheet_row[1], 385, $_dnd_character_sheet_row[1]);
  label_under($_dnd_character_sheet_p, "Level", 315, $_dnd_character_sheet_row[1]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_class_level[$node->language][0]['level'], 315, $_dnd_character_sheet_row[1]);

  // Experience
  underline($_dnd_character_sheet_p, 390, $_dnd_character_sheet_row[1], 460, $_dnd_character_sheet_row[1]);
  label_under($_dnd_character_sheet_p, "Experience", 390, $_dnd_character_sheet_row[1]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_xp[$node->language][0]['value'], 390, $_dnd_character_sheet_row[1]);

  // Penalty
  underline($_dnd_character_sheet_p, 465, $_dnd_character_sheet_row[1], 540, $_dnd_character_sheet_row[1]);
  label_under($_dnd_character_sheet_p, "Penalty", 465, $_dnd_character_sheet_row[1]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_xp_penalty[$node->language][0]['value'], 465, $_dnd_character_sheet_row[1]);

  // Gender
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[2], 105, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Gender", 0, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_gender[$node->language][0]['value'], 0, $_dnd_character_sheet_row[2]);

  // Age
  underline($_dnd_character_sheet_p, 110, $_dnd_character_sheet_row[2], 150, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Age", 110, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_age[$node->language][0]['value'], 110, $_dnd_character_sheet_row[2]);

  // Size
  underline($_dnd_character_sheet_p, 155, $_dnd_character_sheet_row[2], 200, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Size", 155, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_size[$node->language][0]['value'], 155, $_dnd_character_sheet_row[2]);

  // Height
  underline($_dnd_character_sheet_p, 205, $_dnd_character_sheet_row[2], 265, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Height", 205, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_height[$node->language][0]['value'], 205, $_dnd_character_sheet_row[2]);

  // Weight
  underline($_dnd_character_sheet_p, 270, $_dnd_character_sheet_row[2], 340, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Weight", 270, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_weight[$node->language][0]['value'], 270, $_dnd_character_sheet_row[2]);

  // Skin
  underline($_dnd_character_sheet_p, 345, $_dnd_character_sheet_row[2], 390, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Skin", 345, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_skin[$node->language][0]['value'], 345, $_dnd_character_sheet_row[2]);

  // Hair
  underline($_dnd_character_sheet_p, 395, $_dnd_character_sheet_row[2], 430, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Hair", 395, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_hair[$node->language][0]['value'], 395, $_dnd_character_sheet_row[2]);

  // Eyes
  underline($_dnd_character_sheet_p, 435, $_dnd_character_sheet_row[2], 490, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Eyes", 435, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_eyes[$node->language][0]['value'], 435, $_dnd_character_sheet_row[2]);

  // Laterality
  underline($_dnd_character_sheet_p, 495, $_dnd_character_sheet_row[2], 540, $_dnd_character_sheet_row[2]);
  label_under($_dnd_character_sheet_p, "Laterality", 495, $_dnd_character_sheet_row[2]);
  underlined_text($_dnd_character_sheet_p, $node->field_dnd_character_laterality[$node->language][0]['value'], 495, $_dnd_character_sheet_row[2]);

  // Biography
  $text = explode("|||", wordwrap(strip_tags($node->body[$node->language][0]['value']), 80, '|||'));
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[3], 320, $_dnd_character_sheet_row[3]);
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[4], 320, $_dnd_character_sheet_row[4]);
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[5], 320, $_dnd_character_sheet_row[5]);
  underlined_text($_dnd_character_sheet_p, $text[0], 0, $_dnd_character_sheet_row[3], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[1], 0, $_dnd_character_sheet_row[4], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[2], 0, $_dnd_character_sheet_row[5], '', 'small');
  label_under($_dnd_character_sheet_p, "Biography", 0, $_dnd_character_sheet_row[5]);

  // History
  $text = explode("|||", wordwrap($node->field_dnd_character_history[$node->language][0]['value'], 54, '|||'));
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[3], 540, $_dnd_character_sheet_row[3]);
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[4], 540, $_dnd_character_sheet_row[4]);
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[5], 541, $_dnd_character_sheet_row[5]);
  underlined_text($_dnd_character_sheet_p, $text[0], 325, $_dnd_character_sheet_row[3], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[1], 325, $_dnd_character_sheet_row[4], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[2], 325, $_dnd_character_sheet_row[5], '', 'small');
  label_under($_dnd_character_sheet_p, "History", 325, $_dnd_character_sheet_row[5]);

  // Goals
  $text = explode("|||", wordwrap($node->field_dnd_character_goals[$node->language][0]['value'], 80, '|||'));
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[6], 320, $_dnd_character_sheet_row[6]);
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[7], 320, $_dnd_character_sheet_row[7]);
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[8], 320, $_dnd_character_sheet_row[8]);
  underlined_text($_dnd_character_sheet_p, $text[0], 0, $_dnd_character_sheet_row[6], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[1], 0, $_dnd_character_sheet_row[7], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[2], 0, $_dnd_character_sheet_row[8], '', 'small');
  label_under($_dnd_character_sheet_p, "Goals", 0, $_dnd_character_sheet_row[8]);

  // Appearance
  $text = explode("|||", wordwrap($node->field_dnd_character_appearance[$node->language][0]['value'], 54, '|||'));
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[6], 540, $_dnd_character_sheet_row[6]);
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[7], 540, $_dnd_character_sheet_row[7]);
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[8], 541, $_dnd_character_sheet_row[8]);
  underlined_text($_dnd_character_sheet_p, $text[0], 325, $_dnd_character_sheet_row[6], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[1], 325, $_dnd_character_sheet_row[7], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[2], 325, $_dnd_character_sheet_row[8], '', 'small');
  label_under($_dnd_character_sheet_p, "Appearance", 325, $_dnd_character_sheet_row[8]);

  // Personality/Behavior
  $text = explode("|||", wordwrap($node->field_dnd_character_personality[$node->language][0]['value'], 80, '|||'));
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[9], 320, $_dnd_character_sheet_row[9]);
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[10], 320, $_dnd_character_sheet_row[10]);
  underline($_dnd_character_sheet_p, 0, $_dnd_character_sheet_row[11], 320, $_dnd_character_sheet_row[11]);
  underlined_text($_dnd_character_sheet_p, $text[0], 0, $_dnd_character_sheet_row[9], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[1], 0, $_dnd_character_sheet_row[10], '', 'small');
  underlined_text($_dnd_character_sheet_p, $text[2], 0, $_dnd_character_sheet_row[11], '', 'small');
  label_under($_dnd_character_sheet_p, "Personality/Behavior", 0, $_dnd_character_sheet_row[11]);

  // Notes
  $text = explode("|||", wordwrap($node->field_dnd_character_notes[$node->language][0]['value'], 54, '|||'));
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[9], 540, $_dnd_character_sheet_row[9]);
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[10], 540, $_dnd_character_sheet_row[10]);
  underline($_dnd_character_sheet_p, 325, $_dnd_character_sheet_row[11], 541, $_dnd_character_sheet_row[11]);
  for ($i = 0; $i < 3; $i ++) {
    if (!empty($text[$i])) underlined_text($_dnd_character_sheet_p, $text[$i], 325, $_dnd_character_sheet_row[9 + $i], '', 'small');
  }
  label_under($_dnd_character_sheet_p, "Notes", 325, $_dnd_character_sheet_row[11], "");
}

function _dnd_character_sheet_character_main_stats($node, &$_dnd_character_sheet_row, &$_dnd_character_sheet_p) {
  // Ability Scores
  label_filled($_dnd_character_sheet_p, "Ability Scores", 0, $_dnd_character_sheet_row[12], 125);
  label_over($_dnd_character_sheet_p, "Score", 30, $_dnd_character_sheet_row[13] + 9, 23);
  label_over($_dnd_character_sheet_p, "Mod", 53, $_dnd_character_sheet_row[13] + 9, 22);
  label_over($_dnd_character_sheet_p, "Temp", 79, $_dnd_character_sheet_row[13] + 2, 22);
  label_over($_dnd_character_sheet_p, "Mod", 79, $_dnd_character_sheet_row[13] + 9, 22);
  label_over($_dnd_character_sheet_p, "Temp", 102, $_dnd_character_sheet_row[13] + 2, 23);
  label_over($_dnd_character_sheet_p, "Mod", 102, $_dnd_character_sheet_row[13] + 9, 23);
  $abilities = array("str", "dex", "con", "int", "wis", "chr");
  for ($i = 0; $i < 6; $i ++) {
    label_filled($_dnd_character_sheet_p, $abilities[$i], 0, $_dnd_character_sheet_row[$i + 14], 25);
      cell($_dnd_character_sheet_p, $node->field_dnd_character_abilities[$node->language][$i]['score'], 30, $_dnd_character_sheet_row[$i + 14], 23);
      $value = ($node->field_dnd_character_abilities[$node->language][$i]['mod'] > 0)
        ? '+' . $node->field_dnd_character_abilities[$node->language][$i]['mod']
        : $node->field_dnd_character_abilities[$node->language][$i]['mod'];
      cell($_dnd_character_sheet_p, $value, 53, $_dnd_character_sheet_row[$i + 14], 22);

      $value = ($node->field_dnd_character_abilities[$node->language][$i]['tempscore'] > 0)
        ? '+' . $node->field_dnd_character_abilities[$node->language][$i]['tempscore'] : '';
      cell($_dnd_character_sheet_p, $value, 79, $_dnd_character_sheet_row[$i + 14], 23, '', 'dashed');

      $value = ($node->field_dnd_character_abilities[$node->language][$i]['tempmod'] > 0)
        ? '+' . $node->field_dnd_character_abilities[$node->language][$i]['tempmod'] : '';
      cell($_dnd_character_sheet_p, $value, 102, $_dnd_character_sheet_row[$i + 14], 22, '', 'dashed');
  }

  // Saving Throws
  label_filled($_dnd_character_sheet_p, "Saving Throws", 130, $_dnd_character_sheet_row[12], 220);
  label_over($_dnd_character_sheet_p, "Total", 179, $_dnd_character_sheet_row[13] + 9, 30);
  label_over($_dnd_character_sheet_p, "Base", 208, $_dnd_character_sheet_row[13] + 2, 30);
  label_over($_dnd_character_sheet_p, "Save", 208, $_dnd_character_sheet_row[13] + 9, 30);
  label_over($_dnd_character_sheet_p, "Abilty", 236, $_dnd_character_sheet_row[13] + 2, 30);
  label_over($_dnd_character_sheet_p, "Mod", 236, $_dnd_character_sheet_row[13] + 9, 30);
  label_over($_dnd_character_sheet_p, "Magic", 264, $_dnd_character_sheet_row[13] + 2, 30);
  label_over($_dnd_character_sheet_p, "Mod", 264, $_dnd_character_sheet_row[13] + 9, 30);
  label_over($_dnd_character_sheet_p, "Misc", 292, $_dnd_character_sheet_row[13] + 2, 30);
  label_over($_dnd_character_sheet_p, "Mod", 292, $_dnd_character_sheet_row[13] + 9, 30);
  label_over($_dnd_character_sheet_p, "Temp", 321, $_dnd_character_sheet_row[13] + 2, 30);
  label_over($_dnd_character_sheet_p, "Score", 321, $_dnd_character_sheet_row[13] + 9, 30);

  $labels = array('fort', 'ref', 'will');
  $mods   = array('(Con)', '(Dex)', '(Wis)');

  for ($i = 0; $i < 3; $i ++) {
    label_filled($_dnd_character_sheet_p, $labels[$i], 130, $_dnd_character_sheet_row[$i + 14], 30);
    label_middle($_dnd_character_sheet_p, $mods[$i], 160, $_dnd_character_sheet_row[$i + 14], 20);

    // total save
    cell($_dnd_character_sheet_p, $node->field_dnd_character_saves[$node->language][$i]['total'], 180, $_dnd_character_sheet_row[$i + 14], 27);
    label_middle($_dnd_character_sheet_p, "+", 207, $_dnd_character_sheet_row[$i + 14], 5);

    // base save
    cell($_dnd_character_sheet_p, $node->field_dnd_character_saves[$node->language][$i]['base'], (212 + (0 * 28)), $_dnd_character_sheet_row[$i + 14], 22);
    label_middle($_dnd_character_sheet_p, "+", (234 + (0 * 28)), $_dnd_character_sheet_row[$i + 14], 6);

    // ability mod
    cell($_dnd_character_sheet_p, $node->field_dnd_character_saves[$node->language][$i]['abilitymod'], (212 + (1 * 28)), $_dnd_character_sheet_row[$i + 14], 22);
    label_middle($_dnd_character_sheet_p, "+", (234 + (1 * 28)), $_dnd_character_sheet_row[$i + 14], 6);

    // magic mod
    cell($_dnd_character_sheet_p, $node->field_dnd_character_saves[$node->language][$i]['magicmod'], (212 + (2 * 28)), $_dnd_character_sheet_row[$i + 14], 22);
    label_middle($_dnd_character_sheet_p, "+", (234 + (2 * 28)), $_dnd_character_sheet_row[$i + 14], 6);

    // misc mod
    cell($_dnd_character_sheet_p, $node->field_dnd_character_saves[$node->language][$i]['miscmod'], (212 + (3 * 28)), $_dnd_character_sheet_row[$i + 14], 22);
    label_middle($_dnd_character_sheet_p, "+", (234 + (3 * 28)), $_dnd_character_sheet_row[$i + 14], 6);

    label_middle($_dnd_character_sheet_p, "+", 318, $_dnd_character_sheet_row[$i + 14], 6);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_saves[$node->language][$i]['temp'], 324, $_dnd_character_sheet_row[$i + 14], 25, "", "dashed");
  }
  label_over($_dnd_character_sheet_p, "Saving Throw Modifiers", 130, $_dnd_character_sheet_row[17] + 4, 220);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_save_mods[$node->language][0]['value'], 130, $_dnd_character_sheet_row[18], 220);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_save_mods[$node->language][1]['value'], 130, $_dnd_character_sheet_row[19], 220);

  // Hit Points
  label_filled($_dnd_character_sheet_p, "Hit Points", 355, $_dnd_character_sheet_row[12], 125);
  cell($_dnd_character_sheet_p, str_pad($node->field_dnd_character_hp[$node->language][0]['value'], 14), 480, $_dnd_character_sheet_row[12], 60);
  divider($_dnd_character_sheet_p, 512, $_dnd_character_sheet_row[12] + 2, 508, $_dnd_character_sheet_row[12] + 13);

  // Encumberance
  $capacity = _dnd_character_sheet_get_carry_load($node);
  label_over($_dnd_character_sheet_p, "Lift Overhead", 385, $_dnd_character_sheet_row[14] - 3, 30);
  label_over($_dnd_character_sheet_p, "(1xMax Load)", 385, $_dnd_character_sheet_row[14] + 4, 30);
  cell($_dnd_character_sheet_p, $capacity['heavy']['weight'], 385, $_dnd_character_sheet_row[15], 30);

  label_over($_dnd_character_sheet_p, "Lift Off Ground", 435, $_dnd_character_sheet_row[14] - 3, 30);
  label_over($_dnd_character_sheet_p, "(2xMax Load)", 435, $_dnd_character_sheet_row[14] + 4, 30);
  cell($_dnd_character_sheet_p, intval($capacity['heavy']['weight']) * 2, 435, $_dnd_character_sheet_row[15], 30);

  label_over($_dnd_character_sheet_p, "Push/Drag", 485, $_dnd_character_sheet_row[14] - 3, 30);
  label_over($_dnd_character_sheet_p, "(5xMax Load)", 485, $_dnd_character_sheet_row[14] + 4, 30);
  cell($_dnd_character_sheet_p, intval($capacity['heavy']['weight']) * 5, 485, $_dnd_character_sheet_row[15], 30);

  label_over($_dnd_character_sheet_p, "Load", 355, $_dnd_character_sheet_row[16] + 4, 30);
  cell($_dnd_character_sheet_p, "", 355, $_dnd_character_sheet_row[17], 30);

  label_over($_dnd_character_sheet_p, "Speed", 355, $_dnd_character_sheet_row[18] + 4, 30);
  cell($_dnd_character_sheet_p, $capacity['light']['speed'], 355, $_dnd_character_sheet_row[19], 30);

  label_over($_dnd_character_sheet_p, "Speed", 410, $_dnd_character_sheet_row[16] + 4, 26);
  label_over($_dnd_character_sheet_p, "Max", 436, $_dnd_character_sheet_row[16] -3, 26);
  label_over($_dnd_character_sheet_p, "Load", 436, $_dnd_character_sheet_row[16] + 4, 26);
  label_over($_dnd_character_sheet_p, "Max", 462, $_dnd_character_sheet_row[16] -3, 26);
  label_over($_dnd_character_sheet_p, "Dex", 462, $_dnd_character_sheet_row[16] + 4, 26);
  label_over($_dnd_character_sheet_p, "Check", 488, $_dnd_character_sheet_row[16] - 3, 26);
  label_over($_dnd_character_sheet_p, "Pen", 488, $_dnd_character_sheet_row[16] + 4, 26);
  label_over($_dnd_character_sheet_p, "Run", 514, $_dnd_character_sheet_row[16] + 4, 26);
  $encumberances = array('L', 'M', 'H');

  $i = 0;
  label_middle($_dnd_character_sheet_p, $encumberances[$i], 402, $_dnd_character_sheet_row[$i + 17], 8);
  cell($_dnd_character_sheet_p, $capacity['light']['speed'], 26 * 0 + 410, $_dnd_character_sheet_row[$i + 17], 26);
  cell($_dnd_character_sheet_p, $capacity['light']['weight'], 26 * 1 + 410, $_dnd_character_sheet_row[$i + 17], 26);
  cell($_dnd_character_sheet_p, '-', 26 * 0 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');
  cell($_dnd_character_sheet_p, '-', 26 * 1 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');
  cell($_dnd_character_sheet_p, intval($capacity['light']['speed']) * 4, 26 * 2 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');

  $i = 1;
  label_middle($_dnd_character_sheet_p, $encumberances[$i], 402, $_dnd_character_sheet_row[$i + 17], 8);
  cell($_dnd_character_sheet_p, $capacity['medium']['speed'], 26 * 0 + 410, $_dnd_character_sheet_row[$i + 17], 26);
  cell($_dnd_character_sheet_p, $capacity['medium']['weight'], 26 * 1 + 410, $_dnd_character_sheet_row[$i + 17], 26);
  cell($_dnd_character_sheet_p, $capacity['medium']['max_dex'], 26 * 0 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');
  cell($_dnd_character_sheet_p, $capacity['medium']['check_penalty'], 26 * 1 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');
  cell($_dnd_character_sheet_p, intval($capacity['medium']['speed']) * 4, 26 * 2 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');

  $i = 2;
  label_middle($_dnd_character_sheet_p, $encumberances[$i], 402, $_dnd_character_sheet_row[$i + 17], 8);
  cell($_dnd_character_sheet_p, $capacity['heavy']['speed'], 26 * 0 + 410, $_dnd_character_sheet_row[$i + 17], 26);
  cell($_dnd_character_sheet_p, $capacity['heavy']['weight'], 26 * 1 + 410, $_dnd_character_sheet_row[$i + 17], 26);
  cell($_dnd_character_sheet_p, $capacity['heavy']['max_dex'], 26 * 0 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');
  cell($_dnd_character_sheet_p, $capacity['heavy']['check_penalty'], 26 * 1 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');
  cell($_dnd_character_sheet_p, intval($capacity['heavy']['speed']) * 3, 26 * 2 + 462, $_dnd_character_sheet_row[$i + 17], 26, '', 'dashed');
}

function _dnd_character_sheet_character_armor($node, &$_dnd_character_sheet_row, &$_dnd_character_sheet_p) {
  // Armor Class
  label_filled($_dnd_character_sheet_p, "Armor Class", 0, $_dnd_character_sheet_row[20], 85);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_armor_bonus[$node->language][0]['value'], 85, $_dnd_character_sheet_row[20], 22);
  label_over($_dnd_character_sheet_p, "Normal", 85, $_dnd_character_sheet_row[21] - 5, 22);
  label_middle($_dnd_character_sheet_p, '=', 107, $_dnd_character_sheet_row[20], 6);
  cell($_dnd_character_sheet_p, '10', 113, $_dnd_character_sheet_row[20], 22);
  $ac_modifiers1 = array("Armor", "Shield", "Dex Mod", "Size Mod", "Misc Mod");
  $ac_modifiers2 = array("Bonus", "Bonus", "", "", "");

  $i = 0;
  label_middle($_dnd_character_sheet_p, '+', $i * 28 + 135, $_dnd_character_sheet_row[20], 6);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_armor_bonus[$node->language][0]['value'], $i * 28 + 135 + 6, $_dnd_character_sheet_row[20], 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers1[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] - 5, 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers2[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] + 1, 22);

  $i = 1;
  label_middle($_dnd_character_sheet_p, '+', $i * 28 + 135, $_dnd_character_sheet_row[20], 6);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_shield_bon[$node->language][0]['value'], $i * 28 + 135 + 6, $_dnd_character_sheet_row[20], 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers1[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] - 5, 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers2[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] + 1, 22);

  $i = 2;
  label_middle($_dnd_character_sheet_p, '+', $i * 28 + 135, $_dnd_character_sheet_row[20], 6);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_abilities[$node->language][1]['mod'], $i * 28 + 135 + 6, $_dnd_character_sheet_row[20], 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers1[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] - 5, 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers2[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] + 1, 22);

  $i = 3;
  label_middle($_dnd_character_sheet_p, '+', $i * 28 + 135, $_dnd_character_sheet_row[20], 6);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_size_mod[$node->language][0]['value'], $i * 28 + 135 + 6, $_dnd_character_sheet_row[20], 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers1[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] - 5, 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers2[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] + 1, 22);

  $i = 4;
  label_middle($_dnd_character_sheet_p, '+', $i * 28 + 135, $_dnd_character_sheet_row[20], 6);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_misc_modifier[$node->language][0]['value'], $i * 28 + 135 + 6, $_dnd_character_sheet_row[20], 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers1[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] - 5, 22);
  label_over($_dnd_character_sheet_p, $ac_modifiers2[$i], $i * 28 + 135 + 6, $_dnd_character_sheet_row[21] + 1, 22);

  cell($_dnd_character_sheet_p, $node->field_dnd_character_arcane_fail[$node->language][0]['value'], 20, $_dnd_character_sheet_row[22], 25);
  label_over($_dnd_character_sheet_p, 'Arcane', 20, $_dnd_character_sheet_row[23] - 5, 25);
  label_over($_dnd_character_sheet_p, 'Spell Fail', 20, $_dnd_character_sheet_row[23] + 1, 25);

  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_flat_foot[$node->language][0]['value'], 65, $_dnd_character_sheet_row[22], 25);
  label_over($_dnd_character_sheet_p, 'Flat Foot', 65, $_dnd_character_sheet_row[23] - 5, 25);

  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_check_pen[$node->language][0]['value'], 20, $_dnd_character_sheet_row[24], 25);
  label_over($_dnd_character_sheet_p, 'Armor', 20, $_dnd_character_sheet_row[25] - 5, 25);
  label_over($_dnd_character_sheet_p, 'Check Pen', 20, $_dnd_character_sheet_row[25] + 1, 25);

  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_vs_touch[$node->language][0]['value'], 65, $_dnd_character_sheet_row[24], 25);
  label_over($_dnd_character_sheet_p, 'Vs Touch', 65, $_dnd_character_sheet_row[25] - 5, 25);

  label_over($_dnd_character_sheet_p, 'AC Modifiers', 107, $_dnd_character_sheet_row[22] + 5, 168);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_modifiers[$node->language][0]['value'], 107, $_dnd_character_sheet_row[23], 168);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_modifiers[$node->language][1]['value'], 107, $_dnd_character_sheet_row[24], 168);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_ac_modifiers[$node->language][2]['value'], 107, $_dnd_character_sheet_row[25], 168);

  // Armor and Protection
  label_filled($_dnd_character_sheet_p, "Armor & Protection", 280, $_dnd_character_sheet_row[20], 120);
  /*Type    Armor Bonus    Max Dex    Chk Pen    Spell Fail    Speed    Wgt Lbs*/
  label_over($_dnd_character_sheet_p, 'Type', 400, $_dnd_character_sheet_row[20] + 5, 20);
  label_over($_dnd_character_sheet_p, 'Armor', 420, $_dnd_character_sheet_row[20] - 1, 20);
  label_over($_dnd_character_sheet_p, 'Bonus', 420, $_dnd_character_sheet_row[20] + 5, 20);
  label_over($_dnd_character_sheet_p, 'Max', 440, $_dnd_character_sheet_row[20] - 1, 20);
  label_over($_dnd_character_sheet_p, 'Dex', 440, $_dnd_character_sheet_row[20] + 5, 20);
  label_over($_dnd_character_sheet_p, 'Check', 460, $_dnd_character_sheet_row[20] -1, 20);
  label_over($_dnd_character_sheet_p, 'Pen', 460, $_dnd_character_sheet_row[20] + 5, 20);
  label_over($_dnd_character_sheet_p, 'Spell', 480, $_dnd_character_sheet_row[20] - 1, 20);
  label_over($_dnd_character_sheet_p, 'Fail', 480, $_dnd_character_sheet_row[20] + 5, 20);
  label_over($_dnd_character_sheet_p, 'Speed', 500, $_dnd_character_sheet_row[20] + 5, 20);
  label_over($_dnd_character_sheet_p, 'Wgt', 520, $_dnd_character_sheet_row[20] - 1, 20);
  label_over($_dnd_character_sheet_p, 'Lbs', 520, $_dnd_character_sheet_row[20] + 5, 20);

  for ($i = 0; $i < 5; $i ++) {
    $node->field_dnd_character_armor[$node->language][$i]['type'] = ''; // Not used; set here to keep from throwing strict warnings
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['armor'], 280, $_dnd_character_sheet_row[21 + $i], 120);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['type'], 0 * 20 + 400, $_dnd_character_sheet_row[21 + $i], 20);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['bonus'], 1 * 20 + 400, $_dnd_character_sheet_row[21 + $i], 20);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['max_dex'], 2 * 20 + 400, $_dnd_character_sheet_row[21 + $i], 20);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['check_pen'], 3 * 20 + 400, $_dnd_character_sheet_row[21 + $i], 20);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['spell_fail'], 4 * 20 + 400, $_dnd_character_sheet_row[21 + $i], 20);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['speed'], 5 * 20 + 400, $_dnd_character_sheet_row[21 + $i], 20);
    cell($_dnd_character_sheet_p, $node->field_dnd_character_armor[$node->language][$i]['weight'], 6 * 20 + 400, $_dnd_character_sheet_row[21 + $i], 20);
  }
}

function _dnd_character_sheet_character_combat_stats($node, &$_dnd_character_sheet_row, &$_dnd_character_sheet_p) {
  // Initiative
  label_filled($_dnd_character_sheet_p, "Initiative", 0, $_dnd_character_sheet_row[26], 80);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_init[$node->language][0]['value'], 80, $_dnd_character_sheet_row[26], 20);
  label_middle($_dnd_character_sheet_p, "=", 100, $_dnd_character_sheet_row[26], 6);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_abilities[$node->language][1]['mod'], 106, $_dnd_character_sheet_row[26], 20);
  label_middle($_dnd_character_sheet_p, "+", 126, $_dnd_character_sheet_row[26], 6);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_init_mod[$node->language][0]['value'], 132, $_dnd_character_sheet_row[26], 20);
  label_over($_dnd_character_sheet_p, "Dex", 106, $_dnd_character_sheet_row[27] - 5, 20);
  label_over($_dnd_character_sheet_p, "Mod", 106, $_dnd_character_sheet_row[27] + 1, 20);
  label_over($_dnd_character_sheet_p, "Misc", 132, $_dnd_character_sheet_row[27] - 5, 20);
  label_over($_dnd_character_sheet_p, "Mod", 132, $_dnd_character_sheet_row[27] + 1, 20);
  label_over($_dnd_character_sheet_p, 'Initiative Modifiers', 0, $_dnd_character_sheet_row[27] + 5, 152);
  for ($i = 0; $i < 3; $i ++) {
      cell($_dnd_character_sheet_p, $node->field_dnd_character_init_mods[$node->language][$i]['value'], 0, $_dnd_character_sheet_row[28 + $i], 152);
  }

  // Attack Bonuses
  label_filled($_dnd_character_sheet_p, "Attack Bonuses", 157, $_dnd_character_sheet_row[26], 105);

  // Base Attack Bonus
  label_filled($_dnd_character_sheet_p, "Base Attack Bonus", 267, $_dnd_character_sheet_row[26], 130);
  cell($_dnd_character_sheet_p, $node->field_dnd_character_base_attack[$node->language][0]['value'], 397, $_dnd_character_sheet_row[26], 23);

  // Attacks
  label_over($_dnd_character_sheet_p, 'Total', 191, $_dnd_character_sheet_row[27] + 5, 152);
  label_over($_dnd_character_sheet_p, 'Base', 222, $_dnd_character_sheet_row[27] - 1, 152);
  label_over($_dnd_character_sheet_p, 'Attack', 222, $_dnd_character_sheet_row[27] + 5, 152);
  label_over($_dnd_character_sheet_p, 'Ability', 249, $_dnd_character_sheet_row[27] - 1, 152);
  label_over($_dnd_character_sheet_p, 'Mod',     249, $_dnd_character_sheet_row[27] + 5, 152);
  label_over($_dnd_character_sheet_p, 'Size',  276, $_dnd_character_sheet_row[27] - 1, 152);
  label_over($_dnd_character_sheet_p, 'Mod',   276, $_dnd_character_sheet_row[27] + 5, 152);
  label_over($_dnd_character_sheet_p, 'Misc', 305, $_dnd_character_sheet_row[27] -1, 152);
  label_over($_dnd_character_sheet_p, 'Mod',  305, $_dnd_character_sheet_row[27] + 5, 152);

  $size_mod = ($node->field_dnd_character_size[$node->language][0]['value'] == 'small') ? 2 : 0;

  label_filled($_dnd_character_sheet_p, "Melee", 157, $_dnd_character_sheet_row[28], 74);
  label_middle($_dnd_character_sheet_p, "(Str)", 231, $_dnd_character_sheet_row[28], 25);

  $i = 0;
  cell($_dnd_character_sheet_p, $node->field_dnd_character_melee[$node->language][0]['value'], 256, $_dnd_character_sheet_row[28 + $i], 22);
  label_middle($_dnd_character_sheet_p, '=', 278, $_dnd_character_sheet_row[28 + $i], 8);
    $j = 0;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_base_attack[$node->language][0]['value'], $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);

    $j = 1;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_abilities[$node->language][0]['mod'], $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);

    $j = 2;
    cell($_dnd_character_sheet_p, $size_mod, $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);

    $j = 3;
    cell($_dnd_character_sheet_p, '', $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);
  cell($_dnd_character_sheet_p, '', 4 * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22, '', 'dashed');


  label_filled($_dnd_character_sheet_p, "Ranged", 157, $_dnd_character_sheet_row[29], 74);
  label_middle($_dnd_character_sheet_p, "(Dex)", 231, $_dnd_character_sheet_row[29], 25);
  $i = 1;
  cell($_dnd_character_sheet_p, $node->field_dnd_character_ranged[$node->language][0]['value'], 256, $_dnd_character_sheet_row[28 + $i], 22);
  label_middle($_dnd_character_sheet_p, '=', 278, $_dnd_character_sheet_row[28 + $i], 8);
    $j = 0;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_base_attack[$node->language][0]['value'], $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);

    $j = 1;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_abilities[$node->language][1]['mod'], $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);

    $j = 2;
    cell($_dnd_character_sheet_p, $size_mod, $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);

    $j = 3;
    cell($_dnd_character_sheet_p, '', $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);
  cell($_dnd_character_sheet_p, '', 4 * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22, '', 'dashed');

  label_filled($_dnd_character_sheet_p, "", 157, $_dnd_character_sheet_row[30], 74);
  label_middle($_dnd_character_sheet_p, "", 231, $_dnd_character_sheet_row[30], 25);
    $i = 2;
    cell($_dnd_character_sheet_p, '', 256, $_dnd_character_sheet_row[28 + $i], 22);
    label_middle($_dnd_character_sheet_p, '=', 278, $_dnd_character_sheet_row[28 + $i], 8);
    for ($j = 0; $j < 4; $j ++) {
      cell($_dnd_character_sheet_p, '', $j * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22);
      label_middle($_dnd_character_sheet_p, '+', $j * 28 + 308, $_dnd_character_sheet_row[28 + $i], 6);
    }
    cell($_dnd_character_sheet_p, '', 4 * 28 + 286, $_dnd_character_sheet_row[28 + $i], 22, '', 'dashed');

  // Attack Bonus Modifiers
  label_over($_dnd_character_sheet_p, "Attack Bonus Modifiers", 425, $_dnd_character_sheet_row[26] + 5, 115);
  for ($i = 0; $i < 4; $i ++) {
    cell($_dnd_character_sheet_p, $node->field_dnd_character_attack_mods[$node->language][$i]['value'], 425, $_dnd_character_sheet_row[27 + $i], 115);
  }
}

function _dnd_character_sheet_character_weapons($node, &$_dnd_character_sheet_row, &$_dnd_character_sheet_p) {
  label_filled($_dnd_character_sheet_p, "Weapons", 0, $_dnd_character_sheet_row[31], 145);
  $weapon_widths = array(145, 60, 45, 40, 40, 25, 25, 25, 135);
  $weapon_labels = array("", "Attack Bonus", "Damage", "Critical", "Range", "Size", "Type", "Weight", "Notes & Related Feats");
  $current_width = 145;
  for ($i = 1; $i < 9; $i ++) {
    label_over($_dnd_character_sheet_p, $weapon_labels[$i], $current_width, $_dnd_character_sheet_row[31] + 4, $weapon_widths[$i]);
    $current_width += $weapon_widths[$i];
  }
  for ($i = 0; $i < 7; $i ++) {
    $current_width = 0;

    $j =0;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['weapon'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j], '', '', '', 'left');
    $current_width += $weapon_widths[$j];

    $j = 1;
    /*
    $attack_bonus = ($node->character['weapons'][$i])
      ? ($node->character['weapons'][$i]['subcategory'] == 'Ranged Weapons')
        ? $node->character['base_attack_bonuses']['ranged']
        : $node->character['base_attack_bonuses']['melee']
      : '';
    */
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['bonus'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j]);
    $current_width += $weapon_widths[$j];

    $j = 2;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['damage'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j]);
    $current_width += $weapon_widths[$j];

    $j = 3;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['critical'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j]);
    $current_width += $weapon_widths[$j];

    $j = 4;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['range'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j]);
    $current_width += $weapon_widths[$j];

    $j = 5;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['size'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j]);
    $current_width += $weapon_widths[$j];

    $j = 6;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['type'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j]);
    $current_width += $weapon_widths[$j];

    $j = 7;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['weight'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j]);
    $current_width += $weapon_widths[$j];

    $j = 8;
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapons[$node->language][$i]['notes'], $current_width, $_dnd_character_sheet_row[32 + $i], $weapon_widths[$j], '', '', '', 'left');
    $current_width += $weapon_widths[$j];
  }

  // Weapon Proficiencies
  label_filled($_dnd_character_sheet_p, "Weapon Proficiencies", 0, $_dnd_character_sheet_row[39], 155);
  for ($i = 0; $i < 3; $i ++) {
    cell($_dnd_character_sheet_p, $node->field_dnd_character_weapon_prof[$node->language][$i]['value'], 0, $_dnd_character_sheet_row[40 + $i], 240);
  }

  // Combat Notes
  label_filled($_dnd_character_sheet_p, "Combat Notes", 245, $_dnd_character_sheet_row[39], 295);
  for ($i = 0; $i < 3; $i ++) {
    cell($_dnd_character_sheet_p, $node->field_dnd_character_combat_notes[$node->language][$i]['value'], 245, $_dnd_character_sheet_row[40 + $i], 295);
  }
}

function _dnd_character_sheet_character_skills($node, &$_dnd_character_sheet_row, &$_dnd_character_sheet_p) {
  $skill_data = $node->field_dnd_character_skills[$node->language];
  module_load_include('inc', 'dnd_character', 'dnd_character.skills');
  $skill_names = dnd_character_skill_data();

  $top_class = array_shift($node->character['class']['full']);
  //print '<pre>' . print_r($top_class['skill_points'], true);
  //print '<pre>' . print_r($skill_data, true);
  //die;

  label_filled($_dnd_character_sheet_p, "Skills", 0, $_dnd_character_sheet_row[0], 50);
  label_over($_dnd_character_sheet_p, "Skill Points Per Level", 60, $_dnd_character_sheet_row[0], 60);
  cell($_dnd_character_sheet_p, $top_class['skill_points'], 120, $_dnd_character_sheet_row[0], 30);
  label_over($_dnd_character_sheet_p, "+ Int Mod", 150, $_dnd_character_sheet_row[0], 28);
  label_over($_dnd_character_sheet_p, "Class Skill Max Ranks = Character Level + 3", 206, $_dnd_character_sheet_row[0] - 5, 135);
  label_over($_dnd_character_sheet_p, "Cross-Class Skill Max Ranks = (Character Level + 3) / 2", 206, $_dnd_character_sheet_row[0] + 3, 135);
  label_over($_dnd_character_sheet_p, chr(149) . " Can Be Used Untrained", 376, $_dnd_character_sheet_row[0], 60);
  label_over($_dnd_character_sheet_p, "+ Armor Check Penalty Applies", 466, $_dnd_character_sheet_row[0], 60);

  for ($i = 0; $i < 2; $i ++) {
    $startx = ($i % 2) ? 276 : 0;
    label_over($_dnd_character_sheet_p, "Skill", 115 + $startx, $_dnd_character_sheet_row[1] - 1.0, 24);
    label_over($_dnd_character_sheet_p, "Modifier", 115 + $startx, $_dnd_character_sheet_row[1] + 5.0, 24);
    label_over($_dnd_character_sheet_p, "Ranks", 145 + $startx, $_dnd_character_sheet_row[1] + 5.0, 24);
    label_over($_dnd_character_sheet_p, "Ability", 175 + $startx, $_dnd_character_sheet_row[1] - 1.0, 24);
    label_over($_dnd_character_sheet_p, "Modifier", 175 + $startx, $_dnd_character_sheet_row[1] + 5.0, 24);
    label_over($_dnd_character_sheet_p, "Misc", 205 + $startx, $_dnd_character_sheet_row[1] - 1.0, 24);
    label_over($_dnd_character_sheet_p, "Modifier", 205 + $startx, $_dnd_character_sheet_row[1] + 5.0, 24);
    label_over($_dnd_character_sheet_p, "Key", 229 + $startx, $_dnd_character_sheet_row[1] - 1.0, 20);
    label_over($_dnd_character_sheet_p, "Ability", 229 + $startx, $_dnd_character_sheet_row[1] + 5.0, 20);
    label_over($_dnd_character_sheet_p, "Cross", 253 + $startx, $_dnd_character_sheet_row[1] - 1.0, 10);
    label_over($_dnd_character_sheet_p, "Class", 253 + $startx, $_dnd_character_sheet_row[1] + 5.0, 10);
  }

  $skill_count = 0;
  for ($i = 0; $i < 2; $i ++) {
    for ($j = 0; $j < 23; $j ++) {
      $startx = ($i) ? 276 : 0;
      $skill_string = '';
      $skill_ability = '';
      if (!empty($skill_data[$skill_count])) {
        if ($skill_count < (count($skill_data) * ($i + 1) / 2) ) {
          $skill = array(
            'name'        => $skill_names[$skill_data[$skill_count]['skill']]['name'],
            'ability'     => strtoupper($skill_names[$skill_data[$skill_count]['skill']]['ability']),
            'ranks'       => $skill_data[$skill_count]['ranks'],
            'modifier'    => $node->field_dnd_character_abilities[$node->language][$skill_names[$skill_data[$skill_count]['skill']]['ability_key']]['mod'],
            'score'       => intval($node->character['ability_bonus'][strtolower($skill_data[$skill_count]['key_ability'])]),
            'usage'       => ($skill_data[$skill_count]['usage'] == 'cross-class') ? 'X' : '',
            'misc_bonus'  => $skill_data[$skill_count]['misc_bonus'],
          );
          $skill['name'] .= ($skill_data[$skill_count]['trained'] == 'Yes') ? ' ' . chr(149) : '';
          $skill['name'] .= ($skill_data[$skill_count]['armor_check'] == 'Yes') ? ' +' : '';
          $skill['score'] += ($skill['usage'] == 'X') ? intval($skill_data[$skill_count]['ranks']) / 2 : intval($skill_data[$skill_count]['ranks']);
          $skill['score'] = floor($skill['score']) + intval($skill['misc_bonus']);
        }
        else {
          $skill = array();
          $skill_count --;
        }
      }
      else {
        $skill = array();
      }

      label_skill($_dnd_character_sheet_p, $skill['name'], 0 + $startx, $_dnd_character_sheet_row[2 + $j], 115);
      cell($_dnd_character_sheet_p, $skill['score'], 115 + $startx, $_dnd_character_sheet_row[2 + $j], 24);
        label_middle($_dnd_character_sheet_p, '=', 139 + $startx, $_dnd_character_sheet_row[2 + $j], 6);

        underline($_dnd_character_sheet_p, 0 * 30 + 145 + $startx, $_dnd_character_sheet_row[2 + $j] + 13.0, 0 * 30 + 145 + $startx + 24, $_dnd_character_sheet_row[2 + $j] + 13.0);
        label_middle($_dnd_character_sheet_p, '+', 0 * 30 + 145 + $startx + 24, $_dnd_character_sheet_row[2 + $j], 6);
        underlined_text($_dnd_character_sheet_p, $skill['ranks'], 0 * 30 + 145 + $startx + 8, $_dnd_character_sheet_row[3 + $j] - 6);

        underline($_dnd_character_sheet_p, 1 * 30 + 145 + $startx, $_dnd_character_sheet_row[2 + $j] + 13.0, 1 * 30 + 145 + $startx + 24, $_dnd_character_sheet_row[2 + $j] + 13.0);
        label_middle($_dnd_character_sheet_p, '+', 1 * 30 + 145 + $startx + 24, $_dnd_character_sheet_row[2 + $j], 6);
        underlined_text($_dnd_character_sheet_p, $skill['modifier'], 1 * 30 + 145 + $startx + 8, $_dnd_character_sheet_row[3 + $j] - 6);

        underline($_dnd_character_sheet_p, 2 * 30 + 145 + $startx, $_dnd_character_sheet_row[2 + $j] + 13.0, 2 * 30 + 145 + $startx + 24, $_dnd_character_sheet_row[2 + $j] + 13.0);
        underlined_text($_dnd_character_sheet_p, $skill['misc_bonus'], 2 * 30 + 145 + $startx + 8, $_dnd_character_sheet_row[3 + $j] - 6);

        underline($_dnd_character_sheet_p, 2 * 30 + 145 + $startx, $_dnd_character_sheet_row[2 + $j] + 13.0, 2 * 30 + 145 + $startx + 24, $_dnd_character_sheet_row[2 + $j] + 13.0);
        label_middle($_dnd_character_sheet_p, $skill['ability'], 2 * 30 + 145 + $startx + 24, $_dnd_character_sheet_row[2 + $j], 20);

        checkbox($_dnd_character_sheet_p, 2 * 30 + 145 + $startx + 24 + 25, $_dnd_character_sheet_row[2 + $j]);
        label_middle($_dnd_character_sheet_p, $skill['usage'], 2 * 30 + 145 + $startx + 24 + 19, $_dnd_character_sheet_row[2 + $j], 20);
        $skill_count ++;
    }
  }

  // Modifiers and other skills
  label_filled($_dnd_character_sheet_p, 'Skill Modifiers', 0, $_dnd_character_sheet_row[25], 160);
  label_filled($_dnd_character_sheet_p, 'Languages', 165, $_dnd_character_sheet_row[25], 160);
  label_filled($_dnd_character_sheet_p, 'Racial Traits & Class Features', 330, $_dnd_character_sheet_row[25], 210);

  $skills_list  = $skill_data['skills_list_view'];
  $languages    = $skill_data['bonus_languages_view'];
  $traits_list  = _get_traits_list($node);

  for ($i = 0; $i < 16; $i ++) {
    cell($_dnd_character_sheet_p, $skills_list[$i], 0, $_dnd_character_sheet_row[26 + $i], 160, '', '', 'small', 'left');
    cell($_dnd_character_sheet_p, $languages[$i], 165, $_dnd_character_sheet_row[26 + $i], 160, '', '', 'small', 'left');
    cell($_dnd_character_sheet_p, $traits_list[$i], 330, $_dnd_character_sheet_row[26 + $i], 210, '', '', 'small', 'left');
  }
}


function _dnd_character_sheet_character_feats($node, &$_dnd_character_sheet_row, &$_dnd_character_sheet_p) {
  //$feats_list   = _get_feats_list($node);

  label_filled($_dnd_character_sheet_p, 'Feats & Benefits', 0, $_dnd_character_sheet_row[0], 540);
  for ($i = 0; $i < 43; $i ++) {
    cell($_dnd_character_sheet_p, $node->field_dnd_character_feats[$node->language][$i]['value'], 0, $_dnd_character_sheet_row[$i + 1], 540, '', '', 'small', 'left');
  }
}


function equipment($node) {
  global $_dnd_character_sheet_row;
  global $_dnd_character_sheet_p;

  $labels = array('Worn Equipment', 'Container', 'Container');
  for ($i = 0; $i < 17; $i ++) {
    for ($j = 0; $j < 3; $j ++) {
      $first = ($j) ? 0 : 5;
      $notfirst = ($j) ? 5 : 0;
      if ($i) {
        cell($_dnd_character_sheet_p, '', $j * 180 + $notfirst, $_dnd_character_sheet_row[$i], 150 + $first);
        cell($_dnd_character_sheet_p, '', $j * 180 + 155, $_dnd_character_sheet_row[$i], 25);
      }
      else {
        if ($j) {
          label_filled($_dnd_character_sheet_p, $labels[$j], $j * 180 + $notfirst, $_dnd_character_sheet_row[0], 60 + $first);
          cell($_dnd_character_sheet_p, '', $j * 180 + $notfirst + 60, $_dnd_character_sheet_row[0], 90 + $first);
          label_over($_dnd_character_sheet_p, 'Weight', $j * 180 + 155, $_dnd_character_sheet_row[0] + 5.0, 25);
        }
        else {
          label_filled($_dnd_character_sheet_p, $labels[$j], $j * 180 + $notfirst, $_dnd_character_sheet_row[0], 150 + $first);
          label_over($_dnd_character_sheet_p, 'Weight', $j * 180 + 155, $_dnd_character_sheet_row[0] + 5.0, 25);
        }

      }
    }
  }
}

function wealth() {
  global $_dnd_character_sheet_row;
  global $_dnd_character_sheet_p;

  $labels = array('Wealth & Valuables', 'Other Valuables');
  for ($i = 0; $i < 10; $i ++) {
    for ($j = 0; $j < 2; $j ++) {
      $first = ($j) ? 0 : 5;
      $notfirst = ($j) ? 5 : 0;
      if ($i) {
        cell($_dnd_character_sheet_p, '', $j * 270 + $notfirst, $_dnd_character_sheet_row[17 + $i], 190 + $first);
        cell($_dnd_character_sheet_p, '', $j * 270 + 195, $_dnd_character_sheet_row[17 + $i], 50);
        cell($_dnd_character_sheet_p, '', $j * 270 + 245, $_dnd_character_sheet_row[17 + $i], 25);
      }
      else {
        label_filled($_dnd_character_sheet_p, $labels[$j], $j * 270 + $notfirst, $_dnd_character_sheet_row[17], 190 + $first);
        label_over($_dnd_character_sheet_p, 'Location', $j * 270 + 195, $_dnd_character_sheet_row[17] + 5.0, 50);
        label_over($_dnd_character_sheet_p, 'Weight', $j * 270 + 245, $_dnd_character_sheet_row[17] + 5.0, 25);
      }
    }
  }
}









function underline($_dnd_character_sheet_p, $startx, $starty, $endx, $endy) {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;


  // Initialize line style
  $color = rgb2cmyk(hex2rgb('#666666'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(DND_CHARACTER_SHEET_UNDERLINE_WIDTH);

  // And finally, draw the damn line
  $_dnd_character_sheet_p->moveto($_dnd_character_sheet_xoffset + $startx,  $_dnd_character_sheet_yoffset - $starty - (DND_CHARACTER_SHEET_UNDERLINE_WIDTH / 2) - 2.0);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $endx, $_dnd_character_sheet_yoffset - $endy - (DND_CHARACTER_SHEET_UNDERLINE_WIDTH / 2) - 2.0);
  $_dnd_character_sheet_p->stroke();
}

function underlined_text($_dnd_character_sheet_p, $text, $x, $y, $options = '', $size = '') {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;


  $text_size = ($size == 'small') ? DND_CHARACTER_SHEET_UNDERLINE_TEXT_SMALL : DND_CHARACTER_SHEET_UNDERLINE_TEXT;

  // Initialize text style
  $font = $_dnd_character_sheet_p->load_font("Helvetica-Bold", "winansi", "");
  $_dnd_character_sheet_p->setfont($font, $text_size);
  $color = rgb2cmyk(hex2rgb('#333333'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);

  // Output the text at the desired coordinates
  $_dnd_character_sheet_p->fit_textline($text, $_dnd_character_sheet_xoffset + $x, $_dnd_character_sheet_yoffset  - $y - 1.0, $options);
}

function label_under($_dnd_character_sheet_p, $text, $x, $y, $options = '') {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;


  // Initialize text style
  $font = $_dnd_character_sheet_p->load_font("Helvetica", "winansi", "");
  $_dnd_character_sheet_p->setfont($font, DND_CHARACTER_SHEET_UNDERLINE_HEADER);
  $color = rgb2cmyk(hex2rgb('#999999'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);

  // Output the Label text at the desired coordinates
  $_dnd_character_sheet_p->fit_textline(strtoupper($text), $_dnd_character_sheet_xoffset + $x, $_dnd_character_sheet_yoffset - DND_CHARACTER_SHEET_UNDERLINE_HEADER - $y - 2.0, $options);
}

function label_over($_dnd_character_sheet_p, $text, $startx, $starty, $width, $options = "", $border = "") {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;


  // Initialize text style
  $font = $_dnd_character_sheet_p->load_font("Helvetica", "winansi", "");
  $_dnd_character_sheet_p->setfont($font, DND_CHARACTER_SHEET_UNDERLINE_HEADER);
  $color = rgb2cmyk(hex2rgb('#333333'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);

  $options .= " position=center";
  $options .= " boxsize={{$width} 0}";

  // Output the Label text at the desired coordinates
  $_dnd_character_sheet_p->fit_textline($text, $_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - DND_CHARACTER_SHEET_UNDERLINE_HEADER - $starty - 2.0, $options);
}

function label_middle($_dnd_character_sheet_p, $text, $startx, $starty, $width, $options = "", $border = "") {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;


  // Initialize text style
  $font = $_dnd_character_sheet_p->load_font("Helvetica", "winansi", "");
  $_dnd_character_sheet_p->setfont($font, DND_CHARACTER_SHEET_UNDERLINE_HEADER);
  $color = rgb2cmyk(hex2rgb('#333333'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);

  $options .= " position=center";
  $options .= " boxsize={{$width} 0}";

  // Output the Label text at the desired coordinates
  $_dnd_character_sheet_p->fit_textline($text, $_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - DND_CHARACTER_SHEET_UNDERLINE_HEADER - $starty - 2.0, $options);
}

function label_filled($_dnd_character_sheet_p, $text, $startx, $starty, $width, $options = "") {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;


  // Initialize line style
  $color = rgb2cmyk(hex2rgb('#666666'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(0.2);
  $color = rgb2cmyk(hex2rgb('#eeeeee'));
  $_dnd_character_sheet_p->setcolor("fill", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(0.2);

  // Draw the filled/stroked rectangle
  $_dnd_character_sheet_p->moveto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $width, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $width, $_dnd_character_sheet_yoffset - (DND_CHARACTER_SHEET_FILLED_HEADER * 2.0 )- $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - (DND_CHARACTER_SHEET_FILLED_HEADER * 2.0 ) - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->fill_stroke();


  // Initialize text style
  $font = $_dnd_character_sheet_p->load_font("Helvetica", "winansi", "");
  $_dnd_character_sheet_p->setfont($font, DND_CHARACTER_SHEET_FILLED_HEADER);
  $color = rgb2cmyk(hex2rgb('#333333'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);

  $options .= " position=center";
  $options .= " boxsize={{$width} 0}";

  // Output the Label text at the desired coordinates
  $_dnd_character_sheet_p->fit_textline(strtoupper($text), $_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - DND_CHARACTER_SHEET_FILLED_HEADER - $starty, $options);
}
function label_skill($_dnd_character_sheet_p, $text, $startx, $starty, $width, $options = "") {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;



  // Initialize line style
  $color = rgb2cmyk(hex2rgb('#666666'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(0.2);
  $color = rgb2cmyk(hex2rgb('#eeeeee'));
  $_dnd_character_sheet_p->setcolor("fill", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(0.2);

  // Draw the filled/stroked rectangle
  $_dnd_character_sheet_p->moveto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $width, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $width, $_dnd_character_sheet_yoffset - (DND_CHARACTER_SHEET_FILLED_HEADER * 2.0 )- $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - (DND_CHARACTER_SHEET_FILLED_HEADER * 2.0 ) - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->fill_stroke();

  // Initialize text style
  $font = $_dnd_character_sheet_p->load_font("Helvetica", "winansi", "");
  $_dnd_character_sheet_p->setfont($font, DND_CHARACTER_SHEET_UNDERLINE_HEADER);
  $color = rgb2cmyk(hex2rgb('#333333'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);

  $options .= " position={left center}";
  $options .= " boxsize={{$width} 0}";
  $options .= " margin={3 0}";

  // Output the Label text at the desired coordinates
  $_dnd_character_sheet_p->fit_textline($text, $_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - DND_CHARACTER_SHEET_FILLED_HEADER - $starty, $options);
}

function cell($_dnd_character_sheet_p, $text, $startx, $starty, $width, $options = "", $border = "", $size = '', $align='center') {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;

  $text_size = ($size == 'small') ? DND_CHARACTER_SHEET_UNDERLINE_TEXT_SMALL : DND_CHARACTER_SHEET_UNDERLINE_TEXT;

  // Initialize line style
  if ($border == 'dashed') {
    $color = rgb2cmyk(hex2rgb('#666666'));
    $_dnd_character_sheet_p->setdash(1, 2);
  }
  else {
    $color = rgb2cmyk(hex2rgb('#666666'));
  }

  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(0.2);

  // Draw the filled/stroked rectangle
  $_dnd_character_sheet_p->moveto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $width, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $width, $_dnd_character_sheet_yoffset - (DND_CHARACTER_SHEET_FILLED_HEADER * 2.0 )- $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - (DND_CHARACTER_SHEET_FILLED_HEADER * 2.0 ) - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->stroke();
  $_dnd_character_sheet_p->setdash(0, 0);

  // Initialize text style
  $font = $_dnd_character_sheet_p->load_font("Helvetica-Bold", "winansi", "");
  $_dnd_character_sheet_p->setfont($font, $text_size);
  $color = rgb2cmyk(hex2rgb('#333333'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);

  // Set text options for the cell
  $options .= " position=${align}";
  $options .= " boxsize={{$width} 0}";

  $text_variations = ($size == 'small') ? 2 : 0;
  $text_variations = ($align == 'left') ? 3 : $text_variations;
  $_dnd_character_sheet_p->fit_textline($text, $_dnd_character_sheet_xoffset + $startx + $text_variations, $_dnd_character_sheet_yoffset - DND_CHARACTER_SHEET_FILLED_HEADER - $starty - $text_variations, $options);
}

function divider($_dnd_character_sheet_p, $startx, $starty, $endx, $endy) {
  // Set globals
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;


  $_dnd_character_sheet_p->moveto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $endx, $_dnd_character_sheet_yoffset - $endy);
  $_dnd_character_sheet_p->stroke();
}

function checkbox($_dnd_character_sheet_p, $startx, $starty, $checked = '') {
  global $_dnd_character_sheet_xoffset;
  global $_dnd_character_sheet_yoffset;
  $size = 8.0;

  // Initialize line style
  $color = rgb2cmyk(hex2rgb('#666666'));
  $_dnd_character_sheet_p->setcolor("both", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(0.2);
  $color = ($checked == 'checked') ? rgb2cmyk(hex2rgb('#111111')) : rgb2cmyk(hex2rgb('#eeeeee'));
  $_dnd_character_sheet_p->setcolor("fill", "cmyk", $color['c'], $color['m'], $color['y'], $color['k']);
  $_dnd_character_sheet_p->setlinewidth(0.2);

  // Draw the filled/stroked rectangle
  $_dnd_character_sheet_p->moveto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - 4.0 - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $size, $_dnd_character_sheet_yoffset - 4.0 - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx + $size, $_dnd_character_sheet_yoffset - 4.0 - ($size)- $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - 4.0 - ($size) - $starty);
  $_dnd_character_sheet_p->lineto($_dnd_character_sheet_xoffset + $startx, $_dnd_character_sheet_yoffset - 4.0 - $starty);
  $_dnd_character_sheet_p->fill_stroke();
}



function hex2rgb($hex) {
  $color = str_replace('#', '', $hex);
  $rgb = array(
    'r' => hexdec(substr($color, 0, 2)),
    'g' => hexdec(substr($color, 2, 2)),
    'b' => hexdec(substr($color, 4, 2)),
  );
  return $rgb;
}

function rgb2cmyk($var1, $g = 0, $b = 0) {
  if (is_array($var1)) {
    $r = $var1['r'];
    $g = $var1['g'];
    $b = $var1['b'];
  }
  else {
    $r = $var1;
  }
  $cyan    = 255 - $r;
  $magenta = 255 - $g;
  $yellow  = 255 - $b;
  $black   = min($cyan, $magenta, $yellow);
  $cyan    = @(($cyan    - $black) / (255 - $black)) * 255;
  $magenta = @(($magenta - $black) / (255 - $black)) * 255;
  $yellow  = @(($yellow  - $black) / (255 - $black)) * 255;

  return array(
    'c' => $cyan / 255,
    'm' => $magenta / 255,
    'y' => $yellow / 255,
    'k' => $black / 255,
  );
}










/**
 * Row definitions
 */
function define_rows($_dnd_character_sheet_page) {

  $_dnd_character_sheet_row = array(0.0);

  switch ($_dnd_character_sheet_page) {
    case 1:
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT; //1
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT; //5
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT; //10
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //15
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 8.0; //20
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //25
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 8.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //30
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 8.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //35
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 8.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //40
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //45
      break;
    case 2:
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 2.0; //1
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //5
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //10
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //15
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //20
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 8.0; //25
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //30
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //35
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //40
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //45
      break;
    case 3:
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //1
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //5
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //10
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //15
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //20
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //25
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //30
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //35
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //40
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //45
      break;
    case 4:
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //1
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //5
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //10
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //15
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT + 8.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //20
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //25
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //30
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0;
      $_dnd_character_sheet_row[] = $_dnd_character_sheet_row[count($_dnd_character_sheet_row) - 1] + DND_CHARACTER_SHEET_LINE_HEIGHT + DND_CHARACTER_SHEET_HEADER_HEIGHT - 2.0; //35
      break;
  }

  return $_dnd_character_sheet_row;
}

























function _get_traits_list($node) {
  $traits = array();
  $rawtraits = explode("\n", $node->character['race']['data']['traits_list']);
  foreach ($rawtraits as $trait) {
    $temp = explode("|||", wordwrap($trait, 50, '|||'));
    foreach ($temp as $key => $value) {
      $traits[] = ($key) ? '  ' . $value : $value;
    }

  }

  return $traits;
}




function _get_feats_list($node) {

  $feats = array();
  $rawfeats = $node->character['feats'];
  foreach ($rawfeats as $feat) {
    $benefit = $feat['benefit'];
    $benefit = str_replace('</p>', '</p> ', $benefit);
    $benefit = strip_tags($benefit);
    $benefit = str_replace('Benefit: ', '', $benefit);
    $benefit = trim($benefit);
    $feat_text  = $feat['name'] . ': ' . $benefit;

    $temp = explode("|||", wordwrap($feat_text, 133, '|||'));
    foreach ($temp as $key => $value) {
      $line = $value;
      $feats[] = ($key) ? '  ' . $line : $line;
    }

  }

/*
  print '<pre style="background:#fff;color:#000;font-family:consolas;font-size:16px;">';
  print_r($feats);
  print '<a name="bottom"></a>';
  die;
*/
  return $feats;
}







function _dnd_character_sheet_get_carry_load($node) {
  $capacities = array(
    array(0, 0, 0),
    array(3, 6, 10),
    array(6, 13, 20),
    array(10, 20, 30),
    array(13, 26, 40),
    array(16, 33, 50),
    array(20, 40, 60),
    array(23, 46, 70),
    array(26, 53, 80),
    array(30, 60, 90),
    array(33, 66, 100),
    array(38, 76, 115),
    array(43, 86, 130),
    array(50, 100, 150),
    array(58, 116, 175),
    array(66, 133, 200),
    array(76, 153, 230),
    array(86, 173, 260),
    array(100, 200, 300),
    array(116, 233, 350),
    array(133, 266, 400),
    array(153, 306, 460),
    array(173, 346, 520),
    array(200, 400, 600),
    array(233, 466, 700),
    array(266, 533, 800),
    array(306, 613, 920),
    array(346, 693, 1, 040),
    array(400, 800, 1, 200),
    array(466, 933, 1, 400),
  );

  $slow = 20;
  $speed = (empty($node->field_dnd_character_speed[$node->language][0]['value'])) ? 30 : $node->field_dnd_character_speed[$node->language][0]['value'];
  switch ($node->field_dnd_character_speed[$node->language][0]['value']) {
    case 40:
      $slow = 30;
      break;
    case 30:
      $slow = 20;
      break;
    case 20:
      $slow = 15;
      break;
  }

  $carry  = array(
    'light' => array(
      'weight' => $capacities[$node->field_dnd_character_abilities[$node->language][0]['score']][0],
      'max_dex' => NULL,
      'check_penalty' => NULL,
      'speed' => $speed,
    ),
    'medium' => array(
      'weight' => $capacities[$node->field_dnd_character_abilities[$node->language][0]['score']][1],
      'max_dex' => 3,
      'check_penalty' => -3,
      'speed' => $slow,
    ),
    'heavy' => array(
      'weight' => $capacities[$node->field_dnd_character_abilities[$node->language][0]['score']][2],
      'max_dex' => 1,
      'check_penalty' => -6,
      'speed' => $slow,
    ),
  );

  return $carry;
}
