<?php

function menu_modesty_admin($form, $form_state, $menu_name) {
    $menu_items = db_query("SELECT m.path FROM {menu_modesty} m INNER JOIN {menu_links} l ON (m.path = l.link_path) WHERE l.menu_name = :menu", array(':menu' => $menu_name));

    $options = array(); 
    foreach ($menu_items as $menu_item) {
        $menu = menu_get_item($menu_item->path);
        $options[$menu_item->path] = l($menu['title'], $menu_item->path);
    }

    $form = array();
    if (empty($options)) {
        $form['paths'] = array(
            '#markup' => "<p>It's so hard to find a modest menu these days.</p>",
        );
    } else {
        $form['paths'] = array(
            '#type' => 'checkboxes',
            '#title' => 'Modest menus',
            '#description' => 'The checked menus will have their modesty removed.',
            '#options' => $options,
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Submit',
        );
    }

    return $form;
}

function menu_modesty_admin_submit(&$form, &$form_state) {
    foreach (array_filter($form_state['values']['paths']) as $path) {
        db_query("DELETE FROM {menu_modesty} WHERE path = :path", array(':path' => $path));
        db_query("UPDATE {menu_router} SET type = :type WHERE path = :path", array(':type' => MENU_NORMAL_ITEM, ':path' => $path));
        menu_rebuild();

        drupal_set_message('Modesty has been removed.');
    }
}
