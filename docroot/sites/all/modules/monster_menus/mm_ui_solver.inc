<?php

/**
 * @file
 * Permissions solver
 */

function mm_ui_solver($mmtid, $uid = NULL) {
  if (empty($uid) && $uid !== '0') {
    mm_static('solver_mm_list_callback', TRUE, $mmtid);
    $title = t('Solve Permissions Issues');
    drupal_set_title($title, PASS_THROUGH);
    $content = drupal_get_form('mm_ui_solver_form');
    // This is a dummy call, to make sure the over_text.module inserts its JS
    theme('tooltip', array());
    return "<div class=\"node\">
<h2>$title</h2>
<div class=\"content\">$content<div id=\"mm-solver-table\"></div></div>
</div>";
  }

  $acct = user_load($uid);
  if ($acct === FALSE) {
    print t('<p>Unknown user</p>');
  }
  else {
    print mm_ui_solver_table($mmtid, $acct);
  }
  $GLOBALS['devel_shutdown'] = FALSE;
  exit();
}

function mm_ui_solver_form($form, &$form_state) {
  _mm_ui_userlist_setup(array(-1 => ''), $form, 'solver', t("User's name:"), TRUE, '');
  $form['solver-choose']['#title'] = '';
  $form['solver']['#mm_list_other_callback'] = 'Drupal.mmSolverCallback';
  $form['solver']['#description'] = t('Choose a user to see their actual permissions for this page, taking into account the permissions of all parent pages.');

  return $form;
}

function mm_ui_solver_link(&$form, $mmtid) {
  mm_static('solver_mm_list_callback', TRUE, $mmtid);
  $dummy = array();
  $form['solver_form'] = mm_ui_solver_form(array(), $dummy);
  $form['solver_form']['#prefix'] = '<div style="display: none" id="solver-form">';
  $form['solver_form']['#suffix'] = '<div id="mm-solver-table"></div></div>';
  $title = t('Solve permissions issues');
  $form['mm-solver-link'] = array(
    '#type' => 'link',
    '#weight' => -10,
    '#prefix' => '<div id="mm-solver-link">',
    '#suffix' => '</div>',
    '#title' => $title,
    '#href' => "mm/$mmtid/settings/solver",
    '#options' => array(
      'fragment' => 'TB_inline?height=400&width=500&inlineId=solver-form',
      'attributes' => array(
        'class' => array('thickbox'),
        'title' => $title,
      ),
    ),
  );
}

/**
 * Show permissions for a tree entry
 *
 * @param $mmtid
 *   MM tree ID of entry to query
 * @param $acct
 *   Account being tested
 * @return
 *   The HTML code for the results
 */
function mm_ui_solver_table($mmtid, $acct) {
  $list = mm_content_get_parents_with_self($mmtid, FALSE, FALSE);

  $user_access = array();
  $is_admin = $acct->uid == 1;
  foreach (array('administer all menus', 'administer all users', 'administer all groups', 'view all menus') as $access_mode) {
    if ($acct->uid || substr($access_mode, 0, 4) == 'view') {
      $user_access[$access_mode] = user_access($access_mode, $acct);
    }
    else {
      $user_access[$access_mode] = FALSE;
    }
  }
  $is_admin |= $user_access['administer all menus'];

  $is_user = $is_group = $is_recycled = FALSE;
  $can_read = $can_recycle = TRUE;

  $header = array(t('Page'), t('Edit'), t('Add subpages'), t('Add content'), t('Read'));
  $rows = array();
  $all_modes = array(
    MM_PERMS_WRITE => 1,
    MM_PERMS_SUB   => 1,
    MM_PERMS_APPLY => 1,
    MM_PERMS_READ  => 1,
  );
  $everyone = array(
    MM_PERMS_WRITE => t('everyone can edit'),
    MM_PERMS_SUB   => t('everyone can add subpages'),
    MM_PERMS_APPLY => t('everyone can add content'),
    MM_PERMS_READ  => t('everyone can read')
  );
  $readable = array(
    MM_PERMS_WRITE => t('can edit this page'),
    MM_PERMS_SUB   => t('can add subpages to this page'),
    MM_PERMS_APPLY => t('can add content to this page'),
  );
  $yes = theme('mm_ui_mark_yesno', array('yes' => TRUE));
  $no = theme('mm_ui_mark_yesno', array('yes' => FALSE));

  foreach ($list as $entry) {
    $modes = array();
    $reasons = array(
      MM_PERMS_WRITE => array(),
      MM_PERMS_SUB   => array(),
      MM_PERMS_APPLY => array(),
      MM_PERMS_READ  => array(),
    );
    $item = mm_content_get($entry);

    if ($entry == mm_content_users_mmtid()) $is_user = TRUE;
    elseif ($entry == mm_content_groups_mmtid()) $is_group = TRUE;

    if ($item->name == MM_ENTRY_NAME_RECYCLE) $is_recycled = TRUE;

    $name = mm_content_get_name($item);
    if ($entry != 1) {
      $url = mm_content_user_can($entry, MM_PERMS_WRITE) ? "mm/$entry/settings" : "mm/$entry";
      $name = l($name, $url);
    }
    $row = array($name);

    if ($is_admin || ($is_user && $user_access['administer all users'] || $is_group && $user_access['administer all groups'])) {
      $modes = $all_modes;
      $reasons[MM_PERMS_WRITE][] = $reasons[MM_PERMS_SUB][] = $reasons[MM_PERMS_APPLY][] = $reasons[MM_PERMS_READ][] = t('is an administrator');
    }

    if (!$is_admin && $user_access['view all menus']) {
      $modes[MM_PERMS_READ] = 1;
      $reasons[MM_PERMS_READ][] = t('can view all pages');
    }

    if (!$can_read) {
      $reasons[MM_PERMS_WRITE][] = $reasons[MM_PERMS_SUB][] = $reasons[MM_PERMS_APPLY][] = $reasons[MM_PERMS_READ][] = t('cannot read parent');
    }
    else {
      if ($item->uid == $acct->uid) {
        $modes = $all_modes;
        $reasons[MM_PERMS_WRITE][] = $reasons[MM_PERMS_SUB][] = $reasons[MM_PERMS_APPLY][] = $reasons[MM_PERMS_READ][] = t('is the owner');
      }

      foreach ($everyone as $mode => $reason)
        if (strpos($item->default_mode, $mode) !== FALSE) {
          $modes[$mode] = 1;
          $reasons[$mode][] = $reason;
        }

      $select = db_select('mm_tree_access', 'a');
      $select->join('mm_group', 'g', 'g.gid = a.gid');
      $select->leftJoin('mm_tree', 't', 't.mmtid = g.gid');
      $select->leftJoin('mm_virtual_group', 'v', 'v.vgid = g.vgid');
      $select->fields('a', array('mode', 'gid'))
        ->condition('a.mmtid', $entry)
        ->condition(db_or()
          ->condition('v.uid', $acct->uid)
          ->condition(db_and()
            ->condition('g.vgid', 0)
            ->condition('g.uid', $acct->uid)
          )
        );
      $result = $select->execute();
      foreach ($result as $mode_obj) {
        $modes[$mode_obj->mode] = 1;
        $reasons[$mode_obj->mode][] = $mode_obj->gid < 0 ? t('is a listed user') : t('is in the group %name', array('%name' => mm_content_get_name($mode_obj->gid)));
      }

      if ($item->name == MM_ENTRY_NAME_RECYCLE) {
        $can_recycle = $can_recycle && mm_content_user_can_recycle($entry, MM_PERMS_READ, $acct);
        if ($can_recycle) {
          $reasons[MM_PERMS_READ][] = t('can edit something in bin');
          $modes[MM_PERMS_READ] = 1;
        }
        else {
          $reasons[MM_PERMS_READ][] = t('cannot edit anything in bin');
        }
      }
      else {
        if (isset($modes[MM_PERMS_WRITE])) {
          $modes[MM_PERMS_SUB] = $modes[MM_PERMS_APPLY] = $modes[MM_PERMS_READ] = 1;
          $reasons[MM_PERMS_SUB][] = $reasons[MM_PERMS_APPLY][] = $reasons[MM_PERMS_READ][] = $readable[MM_PERMS_WRITE];
        }
        else {
          foreach (array(MM_PERMS_SUB, MM_PERMS_APPLY) as $mode) {
            if (isset($modes[$mode])) {
              $modes[MM_PERMS_READ] = 1;
              $reasons[MM_PERMS_READ][] = $readable[$mode];
            }
          }
        }

        if ($is_recycled) {
          if (!isset($modes[MM_PERMS_READ]) || !$can_recycle) {
            $reasons[MM_PERMS_READ][] = t('cannot edit anything in bin');
          }
        }
      }
    }

    foreach (array_keys($all_modes) as $mode) {
      if (!$reasons[$mode]) $reasons[$mode][] = t('not permitted');
      $tip = '<ul><li>' . join('; </li><li>', $reasons[$mode]) . '</li></ul>';
//      $row[] = (isset($modes[$mode]) ? $yes : $no) . '<br />' . $tip;
      $row[] = theme('tooltip',
        array(
          'text' => isset($modes[$mode]) ? $yes : $no,
          'title' => t('reason(s)'),
          'tip' => $tip,
          'html' => TRUE
        )
      );
    }

    $rows[] = $row;
    $can_read = isset($modes[MM_PERMS_READ]);
  }

  return theme('table',
    array(
     'header' => $header,
     'rows' => $rows,
     'attributes' => array('id' => 'mm-solver'))
    ) . '<div class="description">' . t('Put the mouse over any !yes or !no symbol above to see the reason(s).<br /><b>Note:</b> The table above does not take into account any unsaved changes to settings.', array('!yes' => $yes, '!no' => $no)) . '</div>';
}
