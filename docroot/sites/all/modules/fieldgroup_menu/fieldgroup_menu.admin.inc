<?php

/**
 * @file
 *   fieldgroup_menu_admin.inc
 *   Provide a sub-menu item from fieldgroup attached to an entity
 *
 * @developers:
 *    Gabor Szanto <hello@szantogabor.com>
 *    http://szantogabor.com
 *
 */

/**
 * Form builder when add new fieldgroup to a submenu
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fieldgroup_menu_edit_form($form, $form_state, $group_name) {
  $menu_settings = variable_get("fieldgroup_menu", NULL);
  if (isset($menu_settings[$group_name])) {
    $path = $menu_settings[$group_name]['path'];
  }
  else {
    $path = '';
  }

  $form["fieldgroup_menu_$group_name"]['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#required' => TRUE,
    '#default_value' => $path,
  );
  $form['group_name'] = array(
    '#type' => 'hidden',
    '#value' => $group_name,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function fieldgroup_menu_edit_form_submit($form, &$form_state) {
  $name = 'fieldgroup_menu';
  $value = array(
    $form_state['values']['group_name'] => array(
      'path' => $form_state['values']['path'],
    )
  );
  $form_state['redirect'] = 'admin/structure/fieldgroup_menu/';
  variable_set($name, $value);
}


function fieldgroup_menu_admin_page() {
  if ($fieldgroups = field_group_read_groups()) {
    $menu_settings = variable_get("fieldgroup_menu", NULL);

    $header = array('Label', 'Entity type', 'Bundle', 'Group machine name', 'Path', 'Links');
    $rows = array();

    foreach ($fieldgroups as $entity) {
      foreach ($entity as $bundle) {
        foreach ($bundle as $mode) {
          foreach ($mode as $group) {
            if (($group->format_type == 'tab' || $group->format_type == 'htab') && $group->mode == 'default') {
              $link = l(t('Edit'), "admin/structure/fieldgroup_menu/edit/$group->group_name");
              if (isset($menu_settings[$group->group_name])) {
                $path = $menu_settings[$group->group_name]['path'];
              }
              else {
                $path = '';
              }

              $rows[] = array(
                $group->label,
                $group->entity_type,
                $group->bundle,
                $group->group_name,
                $path,
                $link,
              );
            }
          }
        }
      }
    }
//    dsm(get_defined_vars());
    $output = theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output = t('There aren\'t any fieldgroup');
  }
  return ($output);
}
