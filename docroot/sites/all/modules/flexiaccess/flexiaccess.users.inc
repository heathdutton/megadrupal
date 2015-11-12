<?php

/**
 * @file
 * Form handling for per user ACL.
 */

/**
 * Build form to handle ACLs for user.
 */
function flexiaccess_user($form, &$form_state, $user) {
  if (module_exists('acl')) {
    // See http://passingcuriosity.com/2011/drupal-7-forms-tables/

    // first, a utility function to help code re-use:
    function addrow($row, &$form) {
      //dpm($row);
      $cell_node = array(
        '#type' => 'markup',
        '#markup' => '<strong>' . t('#!nid: !title', array('!nid'=>$row->nid,'!title'=>$row->title)) . '</strong>',
      );
      $cell_view = array(
        '#type' => 'checkbox',
        '#title' => t('View'),
        '#default_value' => $row->grant_view,
      );
      $cell_update = array(
        '#type' => 'checkbox',
        '#title' => t('Update'),
        '#default_value' => $row->grant_update,
      );
      $cell_delete = array(
        '#type' => 'checkbox',
        '#title' => t('Delete'),
        '#default_value' => $row->grant_delete,
      );

      $form['acl'][] = array(
        'node' => &$cell_node,
        'nid' => array('#type' => 'value', '#value' => $row->nid),
        'view' => &$cell_view,
        'update' => &$cell_update,
        'delete' => &$cell_delete,
      );

      // See: http://drupal.stackexchange.com/questions/90282/d7-fapi-unexpected-bahviour-when-combining-ajax-checkbox-and-a-table-theme
      /*
       $form['acl']['#rows'][] = array(
         array('data' => &$cell_node),
         array('data' => &$cell_view),
         array('data' => &$cell_update),
         array('data' => &$cell_delete),
       );
      */

      /*
      unset($cell_node);
      unset($cell_view);
      unset($cell_update);
      unset($cell_delete);
      */
    } // function addrow

    // Get the current permissions from the database
/*
SELECT a.acl_id
AS acl_id, n.nid AS nid, n.grant_view AS grant_view, n.grant_update AS grant_update, n.grant_delete AS grant_delete, node.title AS title 
FROM {acl_user} u 
INNER JOIN {acl} a ON a.acl_id = u.acl_id 
INNER JOIN {acl_node} n ON a.acl_id = n.acl_id 
INNER JOIN {node} node ON n.nid = node.nid 
WHERE (a.module = :module) AND (u.uid = :uid)
*/
    $query = db_select('acl_user', 'u');
    $query->join('acl', 'a', 'a.acl_id = u.acl_id');
    $query->join('acl_node', 'n', 'a.acl_id = n.acl_id');
    $query->join('node', 'node', 'n.nid = node.nid');
    $query
    ->fields('a', array('acl_id'))
    ->fields('n', array('nid'))
    ->fields('node', array('title'));
    $query->addExpression('SUM(n.grant_view)', 'grant_view');
    $query->addExpression('SUM(n.grant_update)', 'grant_update');
    $query->addExpression('SUM(n.grant_delete)', 'grant_delete');
    $query->condition('a.module', 'flexiaccess')
    ->condition('u.uid', $user->uid)
    ->groupBy('n.nid');
    $result = $query->execute();

    $form_state['user'] = $user;

/*
    $form['acnode'] = array(
      '#type' => 'fieldset',
      '#title' => t('Manage the ACL of !user.', array('!user' => $user->name)),
      '#description' => t('Click &#8220;Apply&#8221; to save changes to checkboxes, or to add a node.', array('!user' => $user->name)),
      '#collapsible' => TRUE,
      '#tree' => TRUE,
      '#rows' => array(),
    );
*/

/*
    $form['fs'] = array(
      '#type' => 'fieldset',
      '#title' => t('User based ACL'),
      '#description' => t('Manage individual users\' access to this node.  Remember to press &#8220;Commit updates&#8221; when done (otherwise, your changes will not be saved).'),
      '#collapsible' => TRUE,
      '#tree' => TRUE,
     );
*/

    // The permissions table:
    $form['acl'] = array(
      '#title' => t('Node based ACL'),
      '#prefix' => '<div id="flexiaccess-user-acl-table"><p><em>'.t('Manage the nodes which this user has access to.  Remember to press &#8220;Commit updates&#8221; when done (otherwise, your changes will not be saved).').'</em></p>',
      '#suffix' => '</div>',
      '#tree' => TRUE,
      //       See: http://drupal.stackexchange.com/questions/90282/d7-fapi-unexpected-bahviour-when-combining-ajax-checkbox-and-a-table-theme
      //       '#theme' => 'table',
      '#header' => array(t("Node"), t("View"), t("Update"), t("Delete")),
      '#rows' => array(),
    );

    $form['add'] = array(
      '#type' => 'textfield',
      '#title' => t('Add node'),
      '#size' => 60,
      '#autocomplete_path' => 'flexiaccess/node_autocomplete',
    );
    $form['add_button'] = array(
      '#type' => 'button',
      '#name' => 'acl_user_'+$user->uid,
      '#value' => t('Add Node'),
      '#ajax' => array(
        'callback' => 'flexiaccess_user_ajax_callback',
        'wrapper' => 'flexiaccess-user-acl-table',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    foreach ($result as $row) {
      addrow($row, $form);
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Commit updates'),
      '#weight' => 10,
      '#submit' => array('flexiaccess_user_submit'),
    );

    // determine whether we are rebuilding the form
    if ($form_state['rebuild']) { // This is 1 when a row is being added via ajax. I hope I'm using this correctly?

      // make sure the input text is an integer
      $nid = (empty($form_state['values']['add'])) ? 0 : (int) $form_state['values']['add'];

      // check if there are previously added, but uncommitted rows
      if (empty($form_state['uncommitted'])) {
        $form_state['uncommitted'] = array();
      }

      // check if node exists
      if (node_load($nid)) {
        $form_state['uncommitted'][] = $nid;
      }
      // else {
      //   invalid node number
      //   TODO: error msg. But how?
      // }

      // find the correct node titles
      // TODO: It seems silly to re-query the titles every time. is there another way...?
      if (!empty($form_state['uncommitted'])) {
        $result = db_query('SELECT nid,title, 0 AS grant_view, 0 AS grant_update, 0 AS grant_delete FROM {node} WHERE nid IN (:nids)', array(':nids' => $form_state['uncommitted']));

        // add the rows
        if ($result->rowCount()) {
          foreach($result as $row) {
            addrow($row,$form);
          }
        }
      }
    }

    $form['#tree'] = TRUE;
    return $form;
  }
}

/**
 * AJAX callback to add a node to the table on the user page
 */
function flexiaccess_user_ajax_callback($form, $form_state) {
  // return the part of the (rebuilt) form to be replaced
  //dpm($form);
  return $form['acl'];
}

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing nodes.
 */
function flexiaccess_node_autocomplete($string = '') {
  // TODO: What if range(0,50) does not show the required nodes?
  // TODO: maybe break up search string in words and add separate OR'd conditions with foreach?

  $matches = array();
  if ($string) {
    // Get active content types that have Flexi Access managing their ACL.
    $types = array_filter(variable_get('flexiaccess_types', array()));

    if (!empty($types)) {
      $result = db_select('node')->fields('node', array('title','nid'))->condition('type',$types,'IN')->condition('title', '%' . db_like($string) . '%', 'LIKE')->range(0, 50)->execute();
      foreach ($result as $node) {
        $matches[$node->nid] = check_plain("$node->nid : $node->title");
      }
    }
  }

  drupal_json_output($matches);
}

/**
 * Commit updates from user page
 */
function flexiaccess_user_submit($form, &$form_state) {
  //dpm($form_state['values']['acl'][0]);
  if (empty($form_state['uncommitted'])) {
    $form_state['uncommitted'] = array();
  }

  //dpm($form_state);
  foreach ($form_state['values']['acl'] as $ac) {

    if (in_array($ac['nid'], $form_state['uncommitted'])) {
      // new relationship between user and node
      // create acls for node where necessary
      _flexiaccess_create_acl_rows($ac['nid']);
    }

    // add acls to user
    /*
     * The following is easily accomplished with a single query.
    * The correct way to do it, however, is to use the API, which unfortunately uses many queries...
    * Would a single query be better at avoiding race conditions?
    */
    foreach (array('view','update','delete') as $op) {
      $acl_id = acl_get_id_by_name('flexiaccess', $op.'_'.$ac['nid']);
      //       drupal_set_message("Affected $acl_id : $op _ ".$ac['nid']." : ".$ac[$op]);

      // Doing both add and remove here ensures that the latest form submission takes effect in the db:
      if (1==$ac[$op]) {
        // add permission
        //         drupal_set_message("Adding user ".$form_state['user']->uid." to acl ".$acl_id);
        acl_add_user($acl_id, $form_state['user']->uid);
      } else {
        // This block will only be reached when multiple admins have edited the same permissions
        //remove permission
        //         drupal_set_message("Removing user ".$form_state['user']->uid." from acl ".$acl_id);
        acl_remove_user($acl_id, $form_state['user']->uid);
      }
    }

    // apply changes for the node
    node_access_acquire_grants(node_load($ac['nid']));
  }

  cache_clear_all(); // TODO: why is this necessary?
  drupal_set_message(t('Your changes to the ACL has been saved.'));
}


