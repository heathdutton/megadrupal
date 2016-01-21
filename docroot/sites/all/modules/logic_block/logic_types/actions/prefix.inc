<?PHP

/**
 * @file
 * Handles the prefix logic.
 */

/**
 * Does the block generation.
 *
 * @param string $block
 *   Block name.
 * @param array $data
 *   Array of variables for creating the block.
 */
function logic_block_prefix_block_generator($block, $data) {

  require_once dirname(__FILE__) . "/../../library/form_library.php";

  $output = logic_block_get_block_content($data[1]);

  $block_return = array();

  $block_return['content'] = filter_xss($output) . $data[2];

  return $block_return;

}

/**
 * Does the form generation when creating the block.
 *
 * @param form $form
 *   Drupal form object.
 */
function logic_block_prefix_form_generator($form) {
  
  array_push($form, logic_block_form_title("Block prefix","Add text after a block"));
  
  array_push($form, logic_block_form_instructions("Instructions","Text to display after a block"));

  array_push($form, logic_block_return_machine_name(NULL));

  array_push($form, logic_block_return_blocks("first", NULL));

  array_push($form, logic_block_return_hidden_element('logic_block', 'actions/prefix.inc'));

  array_push($form, logic_block_return_textfield_element('before', 'text before', 'Content to place before block.', TRUE, NULL));

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Finish Block'),
    '#submit' => array('logic_block_prefix_form_management','logic_block_create'),
  );

  return $form;

}

/**
 * Does the form processing.
 *
 * @param array $form
 *   Drupal form array.
 * @param array $form_state
 *   Drupal form_state array.
 */
function logic_block_prefix_form_management($form, $form_state) {

  if (isset($form_state['values']['update'])) {

    $update = TRUE;

  }
  else {

    $update = FALSE;

  }

  logic_block_form_handler(
                         $update,
                         $form_state['values']['block_machine_name'],
                         $form_state['values']['first'],
                         array($form_state['values']['logic_block'],
                           $form_state['values']['first'],
                           $form_state['values']['before'])
                         );

  $form_state['redirect'] = 'admin/structure/logic_block/create';
  drupal_redirect_form($form_state);

}

/**
 * Does the form generation when editting.
 *
 * @param string $name
 *   Block name.
 * @param array $data
 *   Array of variables for creating the block.
 */
function logic_block_prefix_block_edit($name, $data) {

  require_once dirname(__FILE__) . "/../../logic_block.create.inc";
  require_once dirname(__FILE__) . "/../../library/form_library.php";

  $form = array();

  array_push($form, logic_block_form_title("Block prefix","Add text after a block"));
  array_push($form, logic_block_form_instructions("Instructions","Text to display after a block"));
  array_push($form, logic_block_return_machine_name($name));
  array_push($form, logic_block_return_blocks("first", $data[1]));
  array_push($form, logic_block_return_hidden_element('logic_block', 'actions/prefix.inc'));
  array_push($form, logic_block_return_hidden_element('update', TRUE));
  array_push($form, logic_block_return_textfield_element('before', 'text before', 'Content to place before block.', TRUE, $data[2]));

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Finish Block'),
    '#submit' => array('logic_block_prefix_form_management','logic_block_create'),
  );

  return $form;

}
