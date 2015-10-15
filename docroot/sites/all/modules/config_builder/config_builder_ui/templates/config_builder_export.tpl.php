<?php
/**
 * @file
 */
?>
/**
 * Implements hook_menu().
 */
function <?php echo $module ?>_menu() {
  $items = array();

  $items['<?php echo $item->path ?>'] = array(
    'title' => '<?php echo $item->label ?>',
    'type' => MENU_NORMAL_ITEM,
    'page callback' => 'drupal_get_form',
    'page arguments' => array('<?php echo $module ?>_form'),
    'access callback' => '<?php echo $access['type'] ?>',
    'access arguments' => array(<?php echo $access[$access['type']] ?>),
  );

  return $items;
}
<?php if ("{$module}_role_access" == $access['type']) : ?>

/**
 * Access callback for user roles.
 */
function <?php echo $module ?>_role_access($roles) {
  global $user;
  foreach ($roles as $role) {
    if (in_array($role, $user->roles)) {
      return TRUE;
    }
  }
  return FALSE;
}
<?php endif ?>

/**
 * Page callback for <?php echo $module ?>.
 */
function <?php echo $module ?>_form($form, $form_state) {
<?php echo $form ?>
}

/**
 * Implements hook_block_info().
 */
function <?php echo $module ?>_block_info() {
  $blocks = array();

  $blocks['<?php echo $item->name ?>'] = array(
    'info' => '<?php echo $item->label ?>',
  );

  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function <?php echo $module ?>_block_view($delta) {
  if ('<?php echo $item->name ?>' == $delta && <?php echo $access['type'] ?>(<?php echo $access[$access['type']] ?>)) {
    return array(
      'subject' => '<?php echo $item->label ?>',
      'content' => drupal_get_form('<?php echo $module ?>_form'),
    );
  }
}

/**
 * Implements hook_variable_info().
 */
function config_builder_variable_info($options) {
  $variables = array();
<?php foreach ($index as $element) : ?>
  $variables['<?php echo $element->id ?>'] = array(
    'name' => '<?php echo $element->id ?>',
    'title' => '<?php echo $element->title ?>',
    'type' => '<?php echo _config_builder_variable_type_map($element->type) ?>',
    'default' => <?php echo !empty($element->default_value) ? "'{$element->default_value}'" : 'NULL' ?>,
    'group' => '<?php echo $module ?>',
    'module' => '<?php echo $module ?>',
  );
<?php endforeach ?>
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function <?php echo $module ?>_variable_group_info() {
  $groups = array();

  $groups['<?php echo $module ?>'] = array(
    'title' => '<?php echo $item->label ?>',
    'description' => '<?php echo str_replace("'", "\'", $item->description) ?>',
    'path' => '<?php echo $item->path ?>',
<?php if ($access['type'] == 'user_access') : ?>
    'access' => <?php echo $access['user_access'] ?>,
<?php endif ?>
  );

  return $groups;
}
