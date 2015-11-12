<?php


/**
 * Allow the user to validate this toon by equiping or unequiping an item in-game to prove they own this character.
 *
 * @param array $form
 * @param array $form_state
 * @param WoWToon $toon
 */
function wowguild_wowtoon_validate_form($form, $form_state, $toon) {
  $form = array();
  
  global $user;
  if ($toon->type == 'wowtoon') {
  
    // Define what items can be equiped / unequiped.
    $item_slots = array (0 => 'head', 1 => 'neck', 2 => 'shoulder', 14 => 'back', 4 => 'chest', 3 => 'shirt', 18 => 'tabard', 8 => 'wrist', 9 => 'hands', 5 => 'waist', 6 => 'legs', 7 => 'feet');
  
    $items = $toon->active_set->items;
    
    if (empty($toon->data['validate'])) {
      $toon->data['validate'] = array();
    }
  
    // See if we have tried to validate this toon in the last hour.  If for some reason two users are trying to validate at the same time
    // make sure that they are changing different items.
    if (isset($toon->data['validate'][$user->uid]) && ($toon->data['validate'][$user->uid]['validatetime'] >= REQUEST_TIME - 60*60)) {
      // We have previously set the item.
    }
    else {
      // Trying a new validation.  Select a random open slot.
      $keys = array_keys($item_slots);
      shuffle($keys);
      
      do {
        // Choose a random key.
        $key = array_pop($keys);
        // Make sure no other user is trying to validate using this key.
        $found = FALSE;
        foreach ($toon->data['validate'] as $uid => $data) {
          if ($data['slot'] == $key) {
            $found = TRUE;
          }
        }
      } while ($found === TRUE && count($keys));
      
      if (empty($keys)) {
        $form['error']['#markup'] = t('Something terrible has happened.  It seems that more than @count users are trying to validate this character at the same time!!  We cannot continue.  Try again in a hour and see if this issue has cleared up.', array('@count' =>  count($item_slots)));
        return $form;
      }
      
      $item = $items[$key];
      $toon->data['validate'][$user->uid] = array(
          'slot' => $item['data-id'],
          'validatetime' => REQUEST_TIME
      );
      $toon->data['validate'][$user->uid]['equip'] = !isset($item['id']);
      // Save our validation data.
      avatar_save($toon);
    }
  
    $form['directions']['#markup'] = t('To validate that you own this character, please log into this character in World of Warcraft and <strong>@equip</strong> the item in your <strong>@slot</strong> slot.  Log out of World of Warcraft and click the Verify Character button to verify.  Note it may take a few minutes for the armory to update after you logout of WoW.',
    array(
          '@equip' => $toon->data['validate'][$user->uid]['equip']?'equip':'remove',
          '@slot' => $item_slots[$toon->data['validate'][$user->uid]['slot']],
    ));
  
    $form['actions'] = array(
          '#type' => 'container',
          '#attributes' => array('class' => array('form-actions')),
    );
    $form['actions']['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Verify Character!'),
    );
    
    
    
    if ($toon->data['validate'][$user->uid]['equip'] === TRUE) {
      $toon->active_set->items[$toon->data['validate'][$user->uid]['slot']]['equipme'] = 1;
    }
    else {
      $toon->active_set->items[$toon->data['validate'][$user->uid]['slot']]['unequipme'] = 1;
    }
    //$output['kpr'] = kpr($build, true);
    //$toon->sets[1]->items[4]['equipme'] = 1;
    //$toon->activeset->items[4]['equipme'] = 1;
    $form['toon'] = avatar_view($toon, 'validate', 4);
  
    
    // Save this toon in the form_state so it is available to subsequent functions.
    $form_state['toon'] = $toon;
    
    return $form;
  
  
  }
}


function wowguild_wowtoon_validate_form_validate($form, $form_state) {
  global $user;
  
  $toon = $form_state['toon'];
  
  if ($toon->validateToon($toon->data['validate'][$user->uid]['slot'], $toon->data['validate'][$user->uid]['equip'])) {
    // Validated Properly!
  }
  else {
    form_set_error('', t("I don't see the proper item @equip.  You can check your !armory page to be sure that it has been updated, otherwise try again a in few minutes to give the armory time to update.", array(
      '@equip' => $toon->data['validate'][$user->uid]['equip']?'equiped':'removed',
      '!armory' => l('armory', $toon->url),  // TODO: Open in a new window.
    )));
  }
}

function wowguild_wowtoon_validate_form_submit($form, $form_state) {
  global $user;

  $toon = $form_state['toon'];
  
  $toon->uid = $user->uid;
  unset($toon->data['validate'][$user->uid]);
  $toon->status = WOWTOON_STATUS_VALIDATED;
  
  avatar_save($toon);
  
  drupal_set_message(t('You have sucessfully validated @name!', array('@name' => $toon->name)));
  
  $path = avatar_uri($toon);
  drupal_goto($path['path']);
}

/**
* Show the guild_application content type pages.
*
* @param string (or FALSE) $show
*   This parameter is passed from hook_menu, and denotes which field_data_application_status to show.
*
* @return Array
*   Renderable array of applications and tasks related to them.
*/
function wowguild_applications_view($show = FALSE) {
  drupal_add_js('var wowguildUpdateStatusUrl =' . drupal_json_encode(url('guild_admin/update_guild_application_status')) . ';', 'inline');
  drupal_add_js(drupal_get_path('module', 'wowguild') . '/js/wowguild.review-guild-applications.js');
  $header = array(array('data' => t('Name'), 'field' => 'n.title'));
  if ($show === FALSE) {
    $header[] = t('Status');
  }
  $header[] = array('data' => t('Date'), 'field' => 'n.created', 'sort' => 'desc');
  $header[] = t('Actions');


  $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header);
  $query->addTag('node_access');
  $query->fields('n', array('nid', 'title', 'created'));
  $query->condition('n.type', 'guild_application');
  if ($show) {
    $query->join('field_data_application_status', 's', "s.entity_id = n.nid AND s.bundle = 'guild_application' AND deleted = 0");
    $query->condition('s.application_status_value', $show);
  }
  $query->condition('n.status', 1);
  $query->limit(20);
  $nids = $query->execute()->fetchCol();

  $applications = node_load_multiple($nids);

  // Get available application statuses.
  $field = field_info_field('application_status');
  $options = $field['settings']['allowed_values'];

  $rows = array();
  foreach ($applications as $app) {

    $row = array();
    if (!empty($app->postedby)) {
      $img = sprintf('<img src="%s" width="25" height="25" /> ', wowtoon_get_render($app->postedby, 'avatar'));
      $col = l($img, 'toon/' . $app->postedby->tid, array('html' => TRUE));
      $col .= l($app->title, 'node/' . $app->nid);
      $row[] = $col;
    }
    else {
      $row[] = l($app->title, 'node/' . $app->nid);
    }

    if ($show === FALSE) {
      // Display a fake 'pulldown' to change status that only displays IF user has javascript enabled.
      $col = '<div id="wowguild-change-application-status-nid' . $app->nid . '" class="wowguild-change-application-status" rel="' . $app->nid . '">' . $app->application_status[LANGUAGE_NONE][0]['value'];
      $col .= '<div id="wowguild-change-application-status-pulldown-nid' . $app->nid . '" class="wowguild-change-application-pulldown"><ul>';
      foreach ($options as $key => $value) {
        $col .= '<li>' . l($value, 'guild_admin/update_guild_application_status/' . $app->nid . '/' . $value,
        array('attributes' => array(
          'class' => array('wowguild-change-application-status-pulldown-status'),
          'rel' => $app->nid,
        ))
        ) . '</li>';
      }
      $col .= '</ul></div></div>';
      $row[] = $col;
    }

    $row[] = format_date($app->created);
    $col = t('app:') . ' ' .
    l(t('view'), 'node/' . $app->nid) . ' | ' .
    l(t('edit'), 'node/' . $app->nid . '/edit', array('query' => array('destination' => 'guild_admin/guild_applications'))) . ' | ' .
    l(t('delete'), 'node/' . $app->nid . '/delete', array('query' => array('destination' => 'guild_admin/guild_applications'))) . '<br />';
    if (!empty($app->uid)) {
      $col .= t('user:') . ' ' . l(t('edit'), 'user/' . $app->uid . '/edit');
      if (module_exists('mmo_notification') && !empty($app->postedby)) {
        $col .= ' | ' . l(t('mail'), 'inbox/compose', array('query' => array('sender_tid' => wowtoon_get_current_tid(), 'receiver_tid' => $app->postedby->tid)));
      }
    }
    else {
      // $col .= ' | ' . l(t('email'), 'mailto:' . check_plain($app->application_email[LANGUAGE_NONE][0]['value']));
    }
    $row[] = $col;
    $rows[] = $row;
  }

  $build = array();
  if (user_access('administer site configuration')) {
    $build['customize_app']['#markup'] = l(t('Customize guild application fields'), 'admin/structure/types/manage/guild-application/fields');
  }
  $build['apps'] = array(
    '#attributes' => array('id' => 'wowguild-review-applications-table'),
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows
  );
  $build['apps_pager'] = array('#theme' => 'pager');
  return $build;

}
/**
* Allows admins to quickly change the application status of a guild_application.
*
* @param $node
*   The node being updated.
*
* @param $status
*   The new status for the passed node.
*
* @return
*   If $_GET['ajax'] is passed, then the output array is json_encoded and returned, otherwise, returns the user to the guild_admin/guild_applications page.
*/

function wowguild_update_application_status($node, $status) {
  if ($node->type == 'guild_application') {
    $node->application_status[$node->language][0]['value'] = check_plain($status);
    node_save($node);
    if (isset($_GET['ajax'])) {
      $results = array(
        'results' => 1,
        'nid' => $node->nid,
        'status' => check_plain($status)
      );
      drupal_json_output($results);
    }
    else {
      drupal_set_message(t('Updated @name application status to @status', array('@name' => $node->title, '@status' => $status)));
      drupal_goto('guild_admin/guild_applications');
    }
  }
}

/**
* Form builder; edit a user account or one of their profile categories.
*
* @ingroup forms
*/
function wowguild_wowguild_edit_form($form, &$form_state, $guild) {
  global $user;

  $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Guild Name'),
      '#default_value' => isset($guild->name) ? $guild->name : '',
      '#maxlength' => 255,
      '#required' => TRUE,
      '#weight' => -5,
  );
  
  // Add the field related form elements.
  $form_state['wowguild'] = $guild;
  field_attach_form('wowguild', $guild, $form, $form_state);
  
  $form['actions'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('form-actions')),
      '#weight' => 400,
  );
  
  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();
  
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }
  
  $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Guild'),
      '#submit' => $submit + array('wowguild_wowguild_edit_form_submit'),
  );

  //$form['#validate'][] = 'user_profile_form_validate';
  // Add the final user profile form submit handler.
  //$form['#submit'][] = 'user_profile_form_submit';

  return $form;
}

/**
 * Validation function for the user account and profile editing form.
 */
function wowguild_wowguild_edit_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  entity_form_field_validate('wowguild', $form, $form_state);
  
  // See if guild exists in database.
  $guild = $form_state['wowguild'];//wowguild_create($values);
  $guild->name = $values['name'];
  $guild->field_wowtoon_realm = $values['field_wowtoon_realm'];
  //$guild->
  //kpr($values);
  //kpr($guild);
  //die();
  //$form_state['guild'] =
}

/**
 * Submit function for the user account and profile editing form.
 */
function wowguild_wowguild_edit_form_submit($form, &$form_state) {
  $guild = $form_state['wowguild'];
  // Remove unneeded values.
  //form_state_values_clean($form_state);
  
  entity_form_submit_build_entity('wowguild', $guild, $form, $form_state);
  
  wowguild_save($guild);

  drupal_set_message(t('The changes have been saved.'));
}


/**
 * Display stats from the table {wowtoon_armory_log}
 */
function wowguild_armory_hit_report() {
  $query = db_select('wowtoon_armory_log', 'l')->extend('PagerDefault');
  $query->fields('l');
  if (isset($_GET['ignore_cached'])) {
    $query->condition('success', WOWTOON_UPDATE_CHARACTER_NOT_MODIFIED, '!=');
  }
  $query->orderBy('timestamp', 'desc');
  $results = $query->execute();
  
  $header = array(
    t('timestamp'),
    t('function'),
    t('success'),
  );
  $rows = array();
  
  foreach ($results as $ele) {
    $row =  array(
      format_date($ele->timestamp),
      $ele->text . ' [' . l('url', $ele->url) . ']'
    );
    
    switch ($ele->success) {
      case WOWTOON_UPDATE_CHARACTER_NOT_AVAILABLE:
        $row['success'] = 'WOWTOON_UPDATE_CHARACTER_NOT_AVAILABLE';
        break;
      case WOWTOON_UPDATE_ARMORY_DOWN:
        $row['success'] = 'WOWTOON_UPDATE_ARMORY_DOWN';
        break;
      case WOWTOON_UPDATE_INVALID_PAGE:
        $row['success'] = 'WOWTOON_UPDATE_INVALID_PAGE';
        break;
      case WOWTOON_UPDATE_CHARACTER_TOO_LOW:
        $row['success'] = 'WOWTOON_UPDATE_CHARACTER_TOO_LOW';
        break;
      case WOWTOON_UPDATE_INVALID_REALM:
        $row['success'] = 'WOWTOON_UPDATE_INVALID_REALM';
        break;
      case WOWTOON_UPDATE_CHARACTER_AVAILABLE:
        $row['success'] = 'WOWTOON_UPDATE_CHARACTER_AVAILABLE';
        break;
      case WOWTOON_UPDATE_CHARACTER_NOT_MODIFIED:
        $row['success'] = 'WOWTOON_UPDATE_CHARACTER_NOT_MODIFIED';
        break;
      case WOWTOON_UPDATE_TOO_MANY_REQUESTS:
        $row['success'] = 'WOWTOON_UPDATE_TOO_MANY_REQUESTS';
        break;
      default:
        $row['success'] = $ele->success;
    }
    $rows[] = $row;
  }
  
  $content = array();
  
  if (isset($_GET['ignore_cached'])) {
    $content['ignore_cache'] = array('#markup' => l('show cache hits', 'admin/reports/wowguild/armory_hits'));
  }
  else {
    $content['ignore_cache'] = array('#markup' => l('hide cache hits', 'admin/reports/wowguild/armory_hits', array('query' => array('ignore_cached' => 1))));
  }
  $content['pager_header'] = array(
    '#theme' => 'pager'
  );
  $content['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows
  );
  $content['pager_footer'] = array(
      '#theme' => 'pager'
  );
  return $content;
}
