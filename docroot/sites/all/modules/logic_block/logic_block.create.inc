<?php

/**
 * @file
 * Handles the creation of logic blocks.
 */

/**
 * Displays all of the available logic forms.
 */
function logic_block_all_filters() {

  $form['intro'] = array(
    '#markup' => '<h3>' . t('Block Logic types available') . '</h3>',
  );

  $form['logic_block'] = array(
    '#prefix' => '<div id="logic_block_built">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'table',
    '#header' => array(t('Block Logic types'), t('Actions available for this type')),
    '#rows' => array(),
  );

  $dir = opendir(dirname(__FILE__) . "/logic_types/");

  while ($file = readdir($dir)) {

    if ($file != "." && $file != "..") {

      $inner_dir = opendir(dirname(__FILE__) . "/logic_types/" . $file);

      $logic = array(
        '#markup' => $file,
      );

      $filter_string = "";

      while ($inner_file = readdir($inner_dir)) {

        if ($inner_file != "." && $inner_file != "..") {

          $filter_string .= l(t(str_replace("_", " ", str_replace(".inc", "", $inner_file))), 'admin/structure/logic_block/create/' . $file . '/' . str_replace(".inc", "", $inner_file)) . " | ";

        }

      }

      $sname = array(
        '#markup' => substr($filter_string, 0, strlen($filter_string) - 3),
      );

      $form['logic_block'][] = array(
        'fname' => &$logic,
        'sname' => &$sname,
      );

      $form['logic_block']['#rows'][] = array(
        array('data' => &$logic),
        array('data' => &$sname),
      );

      unset($logic);
      unset($sname);

    }

  }

  return $form;

}

/**
 * Toggles the creation menu.
 */
function logic_block_create() {

  if (count(arg()) == 4) {

    return logic_block_all_filters();

  }
  elseif (count(arg()) == 6) {

    $path = arg(4) . "/" . arg(5);

    $form = logic_block_filter_form_generate($path);

    return $form;

  }

}

/**
 * Does the edit form generation.
 *
 * @param string $path
 *   path to the file for this logic.
 */
function logic_block_filter_form_generate($path) {

  if (file_exists(dirname(__FILE__) . "/logic_types/" . $path . ".inc")) {

    require_once dirname(__FILE__) . "/library/form_library.php";

    require_once dirname(__FILE__) . "/logic_types/" . $path . ".inc";

    $form  = array();

    $file_name = explode("/", $path . ".inc");

    $func = "logic_block_" . str_replace(".inc", "", $file_name[1]) . "_form_generator";

    return $func($form);

  }

}

/**
 * Check the block name is unique.
 *
 * @param string $value_to_check
 *   Block name to check for.
 */
function logic_block_creation_name_exists($value_to_check) {

  $query = db_query('SELECT n.block_name 
  FROM {logic_block_blocks_created} n 
  WHERE n.block_name = :block_name 
  LIMIT 1',
  array(':block_name' => $value_to_check));

  $exists = $query->fetch();

  if ($exists) {

    drupal_set_message(t("Block name %block_name is already in use. Please choose another name", array("%block_name" => $value_to_check)), 'error');

    return TRUE;

  }

  return FALSE;

}
