<?PHP

/**
 * @file
 * Handles lots of shared form functions
 */

/**
 * Does the block title and description generation.
 *
 * @param string $title
 *   Title to display
 * @param array $description
 *   Description to display.
 */
function logic_block_form_title($title, $description) {

  $form = array();

  $form['title'] = array(
    '#markup' => '<h3>' . t($title) . '</h3>',
  );

  $form['description'] = array(
    '#markup' => '<p>' . t($description) . '</p>',
  );

  return $form;

}

/**
 * Does the block heading and description instructions.
 *
 * @param string $heading
 *   Heading to display
 * @param array $instructions
 *   Instructions to display.
 */
function logic_block_form_instructions($heading, $instructions) {

  $form = array();

  $form['heading'] = array(
    '#markup' => '<h4>' . t($heading) . '</h4>',
  );

  $form['instructions'] = array(
    '#markup' => '<p>' . t($instructions) . '</p>',
  );

  return $form;

}

/**
 * Displays a list of roles and which one was selected previously.
 *
 * @param string $text
 *   Title to display
 * @param string $form_name
 *   Name for this form element.
 * @param integer $selected
 *   Rid of currently selected role.
 */
function logic_block_get_user_roles($text, $form_name, $selected) {

  $result = db_query('SELECT rid, name from {role}');

  $form = array();
  $roles_list = array();

  while ($role = $result->fetch()) {

    if (isset($role->name)) {

      if (trim($role->name) != "") {

        $roles_list[$role->rid] = $role->name;

      }
      else {

        $roles_list["notset"] = "Not set";

      }

    }
    else {

      $roles_list["notset"] = "Not set";

    }

  }

  $form[$form_name] = array(
    '#type'  => 'select',
    '#title' => t($text),
    '#default_value' => $selected,
    '#options' => $roles_list,
  );

  return $form;

}

/**
 * Displays a list of timezones and which one was selected previously.
 *
 * @param string $text
 *   Title to display
 * @param string $form_name
 *   Name for this form element.
 * @param string $selected
 *   Text of currently selected timezone.
 */
function logic_block_get_user_timezone($text, $form_name, $selected) {

  $result = db_query('SELECT distinct timezone from {users}');

  $form = array();
  $timezones_list = array();

  while ($timezone = $result->fetch()) {

    if (isset($timezone->timezone)) {

      if (trim($timezone->timezone) != "") {

        $timezones_list[$timezone->timezone] = $timezone->timezone;

      }
      else {

        $timezones_list["notset"] = "Not set";

      }

    }
    else {

      $timezones_list["notset"] = "Not set";

    }

  }

  $form[$form_name] = array(
    '#type'  => 'select',
    '#title' => t($text),
    '#default_value' => $selected,
    '#options' => $timezones_list,
  );

  return $form;

}

/**
 * Displays a list of language and which one was selected previously.
 *
 * @param string $text
 *   Title to display
 * @param string $form_name
 *   Name for this form element.
 * @param string $selected
 *   Text of currently selected language.
 */
function logic_block_get_user_language($text, $form_name, $selected) {

  $result = db_query('SELECT distinct language from {users}');

  $languages = $result->fetch();

  $form = array();
  $languages_list = array();

  foreach ($languages as $language) {

    if (isset($language->language)) {

      if (trim($language->language) != "") {

        $languages_list[$language->language] = $language->language;

      }
      else {

        $languages_list["notset"] = "Not set";

      }

    }
    else {

      $languages_list["notset"] = "Not set";

    }

  }

  $form[$form_name] = array(
    '#type'  => 'select',
    '#title' => t($text),
    '#default_value' => $selected,
    '#options' => $languages_list,
  );

  return $form;

}

/**
 * Displays a list of users and which one was selected previously.
 *
 * @param string $text
 *   Title to display
 * @param string $form_name
 *   Name for this form element.
 * @param integer $selected
 *   ID of currently selected user id.
 */
function logic_block_get_users($text, $form_name, $selected) {

  $users = entity_load('user');

  $form = array();
  $users_list = array();

  foreach ($users as $user) {

    if (trim($user->name) != "") {

      $users_list[$user->uid] = $user->name;

    }

  }

  $form[$form_name] = array(
    '#type'  => 'select',
    '#title' => t($text),
    '#default_value' => $selected,
    '#options' => $users_list,
  );

  return $form;

}

/**
 * Displays a list of node types.
 *
 * @param string $form_name
 *   Name of form element.
 * @param string $selected
 *   Node type selected
 */
function logic_block_get_node_types($form_name, $selected) {

  $nodes = node_type_get_types();

  $form = array();
  $nodes_list = array();

  foreach ($nodes as $node) {

    $nodes_list[$node->type] = $node->name;

  }

  $form[$form_name] = array(
    '#type'  => 'select',
    '#title' => t("Block start from"),
    '#default_value' => $selected,
    '#options' => $nodes_list,
  );

  return $form;

}

/**
 * Gets a block's content.
 *
 * @param integer $data
 *   Bid for this block.
 */
function logic_block_get_block_content($data) {

  $result = db_query('SELECT module,delta from {block} where bid = :bid', array(":bid" => $data));

  $block_data = $result->fetch();

  $block = block_load($block_data->module, $block_data->delta);

  $renderable_block = _block_get_renderable_array(_block_render_blocks(array($block)));

  return drupal_render($renderable_block);

}

/**
 * Displays a list of blocks.
 *
 * @param string $form_name
 *   Name of form element.
 * @param string $selected
 *   Node type selected
 */
function logic_block_return_blocks($form_name, $selected) {

  $result = db_query('SELECT bid,module,delta,title from {block} order by module ASC');

  $form = array();
  $blocks = array();

  foreach ($result as $block) {

    $blocks[$block->bid] = $block->module . " - " . $block->delta;

    if ($block->module == "block") {

      $blocks[$block->bid] .= " - " . $block->title;

    }

  }

  $form[$form_name] = array(
    '#type'  => 'select',
    '#title' => t("Block start from"),
    '#default_value' => $selected,
    '#options' => $blocks,
  );

  return $form;

}

/**
 * Displays a list of blocks.
 *
 * @param string $form_name
 *   Name of form element.
 * @param string $selected
 *   Node type selected
 */
function logic_block_second_blocks($form_name, $selected) {

  $result = db_query('SELECT bid,module,delta,title from {block} order by module ASC');

  $form = array();
  $blocks = array();

  $blocks['empty'] = "Empty Block";

  foreach ($result as $block) {

    $blocks[$block->bid] = $block->module . " - " . $block->delta;

    if ($block->module == "block") {

      $blocks[$block->bid] .= " - " . $block->title;

    }

  }

  $form[$form_name] = array(
    '#type'  => 'select',
    '#title' => t("Block to use instead"),
    '#default_value' => $selected,
    '#options' => $blocks,
  );

  return $form;

}

/**
 * Displays a list of logic block blocks.
 *
 * @param string $form_name
 *   Name of form element.
 */
function logic_block_return_logic_block_blocks($form_name) {

  $result = db_query('SELECT bid,module,delta from {block} where module = :module order by module ASC', array(":module" => "logic_block"));

  $blocks = array();

  foreach ($result as $block) {

    $blocks[$block->bid] = $block->module . " - " . $block->delta;

  }

  $form["logic_block"][$form_name] = array(
    '#type'  => 'select',
    '#title' => t("Block start from"),
    '#options' => $blocks,
  );

  return $form;

}

/**
 * Generates the block name machine name form element.
 *
 * @param string $name
 *   Current value for form.
 */
function logic_block_return_machine_name($name) {

  $form = array();

  $form["block_machine_name"] = array(
    '#type' => 'machine_name',
    '#title' => t("Enter the name you'd like for this block"),
    '#description' => t("This is the name of your block in Structure | Blocks. The name must contain only lowercase letters, numbers, and underscores (a machine name)"),
    '#machine_name' => array(
      'exists' => 'logic_block_creation_name_exists',
    ),
    '#default_value' => $name,

  );

  return $form;

}

/**
 * Generates the hidden form element.
 *
 * @param string $name
 *   Element name for form.
 * @param string $value
 *   Current value for form.
 */
function logic_block_return_hidden_element($name, $value) {

  $form = array();

  $form[$name] = array(
    '#type' => 'hidden',
    '#value' => $value,
  );

  return $form;

}

/**
 * Generates the textfield form element.
 *
 * @param string $name
 *   Element name for form.
 * @param string $title
 *   Title for textfield element.
 * @param string $description
 *   Description for textfield element.
 * @param boolean $required
 *   Whether element is required.
 * @param string $value
 *   Value for textfield element.
 */
function logic_block_return_textfield_element($name, $title, $description, $required, $value) {

  $form = array();

  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t($title),
    '#description' => t($description),
    '#required' => $required,
    '#default_value' => $value,
  );

  return $form;

}

/**
 * Handles the database queries for the forms.
 *
 * @param boolean $update
 *   Whether this is an insert new or update action.
 * @param string $name
 *   Block name.
 * @param integer $base_bid
 *   Bid of first block.
 * @param array $data
 *   Array with logic in.
 */
function logic_block_form_handler($update, $name, $base_bid, $data) {

  if (!$update) {

    $record = array(
      'block_name' => $name,
      'base_bid' => $base_bid,
      'logic' => serialize($data),
    );

    drupal_write_record('logic_block_blocks_created', $record);

    drupal_set_message(t("Block created. The block can be managed on the 'Block Management' tab."), 'status');

  }
  else {

    db_update('logic_block_blocks_created')
      ->condition('block_name', $name)
      ->fields(
      array(
        'base_bid' => $base_bid,
        'logic' => serialize($data),
      )
    )
    ->execute();

    drupal_set_message(t("Block updated. The block can be managed on the 'Block Management' tab."), 'status');

  }

}
