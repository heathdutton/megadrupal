<?php

function mean_integration_settings_form() {
  $form['mean_integration_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Integration Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  $form['mean_integration_settings']['mean_integration_settings_protocol'] = array(
    '#type' => 'select',
    '#title' => t('Protocol'),
    '#options' => array(
      'http' => t('http'),
      'https' => t('https')
    ),
    '#default_value' => variable_get('mean_integration_settings_protocol', MEAN_DEFAULT_PROTOCOL),
  );

  $form['mean_integration_settings']['mean_integration_settings_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#description' => t('Put here the domain, i.e.: localhost'),
    '#default_value' => variable_get('mean_integration_settings_host', MEAN_DEFAULT_ADDRESS),
  );

  $form['mean_integration_settings']['mean_integration_settings_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => variable_get('mean_integration_settings_port', MEAN_DEFAULT_PORT)
  );

  return system_settings_form($form);
}

/**
 * JSON callback function.
 * @types:
 *   view - loads view
 *     $arg1 - display_name
 *     $arg2 - nodes per page (-1 for unlimited)
 *     $arg3 - arguments divided by + without whitespaces
 * 
 *   menu - output from menu_tree_all_data() function
 * 
 *   load - looks for load_$name function and returns its output (node_load(), user_load(), menu_load() ...)
 *     $arg1 - ID or name to pass to load function
 * 
 *   user
 *     user/logout - logging current user out
 *     user/session - returns $_SESSION
 *     user/me - returns global user object
 *
 *   taxonomy
 *     returns output of taxonomy_get_tree($name), $name is vocabulary ID
 *     if term_fields module is enabled and there are fields, they will be attached
 *     to $term->fields, if the field is file, its absolute URL will be returned instead of FID
 *
 *   role
 *     return output of user_roles().
 *
 *   get-mean-packet
 *     $name - packet name, returns set of JSON object, set at admin/build/mean-packets
 * 
 * if $return_as_json = false, PHP object will be returned
 */
function mean_json($type, $name, $arg1 = 0, $arg2 = 0, $arg3 = 0, $return_as_json = TRUE) {
  $json = 'ERROR';
  switch ($type) {
    case 'view':
      $json = 'ERROR';
      if ($view = views_get_view($name)) {
        $display_id = empty($arg1) ? 'default' : $arg1;
        // check user access
        if (!$view->access($display_id)) {
          $json = 'VIEW_NO_PERMISSION';
          break;
        }
        // pass arguments
        if ($arg3) {
          $view->set_arguments(explode('+', $arg3));
        }
        // set display if name passed
        if (!empty($arg1)) {
          $view->set_display($arg1);
        }
        else {
          $view->init_display();
        }
        // set items_per_page if passed
        if ($arg2) {
          $view->set_items_per_page($arg2 == -1 ? 0 : $arg2);
        }
  
        //$view->pre_execute();
        $view->execute();
        $json = $view->result;
        if (!empty($json[0]->nid) || !empty($json[0]->users_uid)) {
          foreach ($json as &$res) {
            if (!empty($res->users_uid)) {
              $res->users_path = url("user/$res->users_uid", array('absolute' => TRUE));
            }
            if (!empty($res->nid)) {
              $res->node_path = url("node/$res->nid", array('absolute' => TRUE));
            }
          }
        }
      }
      break;
      
    case 'load': 
      $func_name = "{$name}_load";
      if (function_exists($func_name)) {
        // sanitize user object
        if ($name == 'user' && $arg1) {
          if ($json = user_load($arg1)) {
            $json = sanitize_user_object($json);
          }
        }
        else {
          $json = $arg1 ? $func_name($arg1) : $func_name();
        }
      } 
      break; 
      
    case 'menu': 
      if ($json = menu_tree_all_data($name)) {
        foreach ($json as &$menu_link) {
          if ($menu_link['link']['href'] == '<front>') {
            $menu_link['link']['href'] = $menu_link['link']['link_path'] = variable_get('site_frontpage', 'node');
          }
          // build fragment if name is set
          if (!empty($menu_link['link']['options']['attributes']['name'])) {
            $menu_link['link']['href'] = '!#' . $menu_link['link']['options']['attributes']['name'];
          }
          // get path alias
          else {
            $menu_link['link']['href'] = drupal_get_path_alias($menu_link['link']['href']);
          }
        } 
      }
      $json = empty($json) ? 'ERROR' : $json;
      break;

    case 'taxonomy':
      if ($tree = taxonomy_get_tree($name)) {
        // attach optional fields if available
        if (module_exists('term_fields')) {
          if ($fields_data = term_fields_get_rows($name)) {
            foreach ($tree as &$term) {
              $term->fields = term_fields_get_fields($term);
              foreach ($term->fields as $key => &$t_field) {
                // convert FID into absolute URL for fileds of type 'file'
                if ($fields_data[$key]->type == 'file' && !empty($t_field)) {
                  $file = file_load($t_field);
                  $t_field = url($file['filepath'], array('absolute' => TRUE));
                }
              }
            }
          }
        }
        $json = $tree;
      }
      break;

    case 'role':
      $json = user_roles();
      break;

    case 'user':
      switch ($name) {
        case 'login':
          $json = 'FAILURE';
          if (!empty($_POST['name']) && !empty($_POST['pass'])) {
            $credentials = array('name' => $_POST['name'], 'pass' => $_POST['pass']);
            if ($account = user_load($credentials)) {
              if (user_external_login($account, $credentials)) {
                $json = sanitize_user_object($account);
              }
            }
          }
          break;
          
        case 'logout':
          // copied from user_logout() function, without redirection to home page
          global $user;
          watchdog('user', 'Session closed for %name by mean module.', array('%name' => $user->name));
          // Destroy the current session:
          session_destroy();
          // Only variables can be passed by reference workaround.
          $null = NULL;
          user_module_invoke('logout', $null, $user);       
          // Load the anonymous user
          $user = drupal_anonymous_user();
          
          $json = 'logout';
          break;
          
        case 'session':
          $json = $_SESSION;
          break;
          
        case 'me';
          global $user;
          $json = sanitize_user_object($user);
      }
      break;
      
    case 'get-mean-packet':
      if ($packet = mean_packet_load($name)) {
        
        $json = array();
        foreach ($packet as $type => $data) {
          $json[$type] = array();
          foreach ($data as $line) {
            $args = explode('/', $line);
            if (count($args) > 1) {
              // $type and $name are mandatory, fill only 3 args
              $arg1 = empty($args[2]) ? 0 : $args[2];
              $arg2 = empty($args[3]) ? 0 : $args[3];
              $arg3 = empty($args[4]) ? 0 : $args[4];
              $json[$type][] = mean_json($args[0], $args[1], $arg1, $arg2, $arg3, FALSE);
            }
          }
        }
      }
      $json = empty($json) ? 'ILLEGAL PACKET' : $json;
      break;
      
    case 'help':
      print '<pre>' . file_get_contents(dirname(__FILE__) . '/README.txt') . '</pre>';
      exit();
      
    default:
      // let other modules to add their types
      foreach (module_implements('mean_json_type_info') as $module) {
        $mean_type = module_invoke($module, 'mean_json_type_info');
        if ($type == $mean_type['name']) {
          // call custom function with received args
          $json = $mean_type['menu_callback']($name, $arg1, $arg2, $arg3);
          break;
        }
      }
  }
  
  if ($return_as_json) {
    drupal_json_output($json);
  }
  else {
    return $json;
  }
}

/**
 * Sanitize user object, add session cookie name
 * */
function sanitize_user_object($user) {
  unset($user->pass, $user->mail, $user->init, $user->created, $user->login, $user->access);
  $user->session_cookie_name = session_name();
  return $user;
}

/**
 * Mean administration page. Display a list of existing mean packets.
 */
function mean_admin_page() {
  $packets = array();
  $result = db_select('variable', 'v')
    ->fields('v', array('name')) 
    ->condition('name', 'mean_packet_%', 'LIKE') 
    ->execute() 
    ->fetchAll();
    
  foreach ($result as $row) {
    $packets[str_replace('mean_packet_', '', $row->name)] = variable_get($row->name, array());
  }
  return theme('mean_admin_page', array('packets' => $packets));
}

/**
 * Theme the output for the main mean administration page.
 */
function theme_mean_admin_page($vars) {
  if (function_exists('dpm') && !empty($_GET['dpm'])) {
    dpm(mean_json('get-mean-packet', $_GET['dpm'], 0, 0, 0, FALSE));
  }
  $output = t('This page lists all the <em>mean packets</em> that are currently defined on this system.');
  if (!count($vars['packets'])) {
    $output .= '<br />' . t('No packets detected, you may !create.', array('!create' => l(t('create one'), 'admin/build/mean-packets/add')));
    return $output;
  }
 
  // Build out the list of normal, database flags.
  foreach ($vars['packets'] as $name => $packet) {
    $ops = array(
      l(t('edit'), "admin/build/mean-packets/edit/$name"),
      l(t('delete'), "admin/build/mean-packets/delete/$name")
    );
    if (function_exists('dpm')) {
      $ops[] = l(t('DPM output'), $_GET['q'], array('query' => array('dpm' => $name)));
    }
   
    $packet_types = array();
    foreach ($packet as $type => $lines) {
      $packet_types[] = "$type (" . count($lines) . ")";
    }
    $json_link = "mean/json/get-mean-packet/$name";
    $rows[] = array(
      $name,
      implode(' | ', $packet_types),
      l($json_link, $json_link, array('absolute' => TRUE, 'attributes' => array('target' => '_BLANK', 'title' => url($json_link, array('absolute' => TRUE))))),
      implode(' | ', $ops),
    );
  }

  $header = array(t('Packet'), t('Services types with number of services'), t('Absolute link'), t('Operations'));
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
    
  return $output;
}

/**
 * Present a form for creating a new flag, setting the type of flag.
 */
function mean_packet_form($form, $form_state, $packet = NULL) {
  $form = array();
  // new packet - edit-mode = FALSE, editing packet - edit-mode = TRUE
  $form['edit_mode'] = array('#type' => 'value', '#value' => !empty($packet));

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mean packet name'),
    '#description' => t('The machine-name for this packet. It may only contain lowercase letters, underscores, and numbers. It will be used in URLs and in all API calls.'),
    '#default_value' => $packet ? arg(4) : '',
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $types = mean_get_json_types();
  // get description of the first type
  $description = reset($types);
  $description = $description['description'];
  $options = '';
  foreach ($types as $type => $descr) {
    $options .= "<option value='$type'>$type</option>\n";
  }
  $add_label = t('Add');
  
  $mean_packet_args = <<<EOM
  <div id="add_new_mean_packet_box">
    <select>
      $options
    </select>
    <input type="text" class="name form-text" size="15" placeholder="NAME" />
    <input type="text" class="arg1 form-text" size="15" placeholder="ARG1" />
    <input type="text" class="arg2 form-text" size="15" placeholder="ARG2" />
    <input type="text" class="arg3 form-text" size="15" placeholder="ARG3" />
    <input type="button" value="$add_label" class="form-submit" />
    <div class="description">$description</div>
  </div>
EOM;
  drupal_add_js(array('meanTypes' => $types), 'setting');
  drupal_add_js(drupal_get_path('module', 'mean') . '/mean.js');
  drupal_add_css(drupal_get_path('module', 'mean') . '/mean.css');

  $text = array();
  if ($packet) {
    foreach ($packet as $type) {
      $text = array_merge($text, $type);
    }
  }

  $form['mean_packets'] = array(
    '#type' => 'textarea',
    '#title' => t('JSONs to be fetched'),
    '#default_value' => implode("\n", $text),
    '#prefix' => $mean_packet_args,
    '#description' => t('The type of content this flag will affect. An individual flag can only affect one type of content. This cannot be changed once the flag is created.'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * mean packets form validation
 * */
function mean_packet_form_validate($form, &$form_state) {
  $name = $form_state['values']['name'];
  if (preg_match('/[^a-z0-9-]/', $name)) {
    form_set_error('name', t('The mean packet name may only consist of lowercase letters, numbers, and hyphens.'));
  }
  // do not allow duplicates when creating new packets
  if (!$form_state['values']['edit_mode'] && variable_get("mean_packet_$name", 'NO_SUCH_RECORD') != 'NO_SUCH_RECORD') {
    form_set_error('name', t('Mean packet <em>!name</em> already exists, please change name.', array('!name' => l($name, "admin/build/mean-packets/edit/$name"))));
  }
}

/**
 * save mean packet to varibles DB table
 * */
function mean_packet_form_submit($form, &$form_state) {
  $data = $ignored_lines = array();
  $types = mean_get_json_types();
  foreach (explode("\n", $form_state['values']['mean_packets']) as $item) {
    $item = trim($item);
    $items = explode('/', $item);
    // illegal line entry
    if (count($items) < 2 || !isset($types[$items[0]])) {
      $ignored_lines[] = $item;
    }
    else {
      // initialize array if needed
      $data[$items[0]] = isset($data[$items[0]]) ? $data[$items[0]] : array();
      $data[$items[0]][] = $item;
    }
  }
  // remove possible duplicates
  foreach ($data as &$type) {
    $type = array_unique($type);
  }
 
  if (count($data)) {
    drupal_set_message('Successful submission');
    variable_set("mean_packet_{$form_state['values']['name']}", $data);
  }
  if (count($ignored_lines)) {
    drupal_set_message(t("Following illegal lines were detected, and not stored to DB:<br /> -- !ignored", array('!ignored' => implode('<br /> -- ', $ignored_lines))), 'warning');
  }
  $form_state['redirect'] = 'admin/build/mean-packets';
}

/**
 * return the list off available JSON types
 * */
function mean_get_json_types() {
  $types = array(
    'view' => array('description' => t('Returns results for requested Drupal view object, arguments: NAME - view\'s name, ARG1 - (optional) display_name, ARG2 - (optional) items per page value (-1 for unlimited, 0 - skip arg), ARG3 - (optional) arguments divided by \'+\' sign without whitespaces')),
    'menu' => array('description' => t('Output from menu_tree_all_data() function')),
    'load' => array('description' => t('Looks for load_[NAME_mandatory] function and returns its output (node_load([ARG1]), user_load([ARG1]), menu_load([ARG1]) ...), arguments: ARG1 - ID or name to pass to load function')),
    'user' => array('description' => t('/mean/json/user/logout - Logging current user out, /mean/json/user/login (along with name and pass values passed as post will log user in and return its object), /mean/json/user/session - returns $_SESSION, /mean/json/user/me - returns global user object')),
    'help' => array('description' => t('/mean/json/help/me will output instruction.')),
  );
  
  // let other modules to add their types
  foreach (module_implements('mean_json_type_info') as $module) {
    $mean_type = module_invoke($module, 'mean_json_type_info');
    $types[$mean_type['name']] = array('description' => $mean_type['description']);
  }
  return $types;
}

/**
 * confirm deletion form
 * */
function mean_packet_delete_confirm($form, $form_state, $name) {
  $mean_packet = mean_packet_load($name);
  if (empty($mean_packet)) {
    drupal_goto('admin/build/mean-packets');
  }

  $form['mean_packet_name'] = array('#type' => 'value', '#value' => $name);

  return confirm_form($form,
    t('Are you sure you want to delete <em>%title</em> mean packet?', array('%title' => $name)),
    !empty($_GET['destination']) ? $_GET['destination'] : 'admin/build/mean-packets',
    '',
    t('Delete'), t('Cancel')
  );
}

/**
 * confirm form submit
 * */
function mean_packet_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    variable_del("mean_packet_{$form_state['values']['mean_packet_name']}");
  }
  drupal_set_message(t('Mean packet <em>@name</em> has been deleted.', array('@name' => $form_state['values']['mean_packet_name'])));
  $form_state['redirect'] = 'admin/build/mean-packets';
}


/**
 * Return data in JSON format.
 *
 * This function should be used for JavaScript callback functions returning
 * data in JSON format. It sets the header for JavaScript output.
 *
 * @param $var
 *   (optional) If set, the variable will be converted to JSON and output.
 */
function drupal_json_mean($var = NULL) {
  // We are returning JavaScript, so tell the browser.
  drupal_set_header('Content-Type: text/javascript; charset=utf-8');

  if (isset($var)) {
    echo drupal_to_js_mean($var);
  }
}


/**
 * Converts a PHP variable into its Javascript equivalent.
 *
 * We use HTML-safe strings, i.e. with <, > and & escaped.
 */
function drupal_to_js_mean($var) {
  switch (gettype($var)) {
    case 'boolean':
      return $var ? 'true' : 'false'; // Lowercase necessary!
    case 'integer':
    case 'double':
      return $var;
    case 'resource':
    case 'string':
      // leave single quotes unaffected
      $var = str_replace("'", '______', $var);
      $var = '"'. str_replace(array("\r", "\n", "<", ">"),
                              array('\r', '\n', '\x3c', '\x3e'),
                              addslashes($var)) .'"';
    return str_replace('______', "'", $var);
    case 'array':
      // Arrays in JSON can't be associative. If the array is empty or if it
      // has sequential whole number keys starting with 0, it's not associative
      // so we can go ahead and convert it as an array.
      if (empty ($var) || array_keys($var) === range(0, sizeof($var) - 1)) {
        $output = array();
        foreach ($var as $v) {
          $output[] = drupal_to_js_mean($v);
        }
        return '[ '. implode(', ', $output) .' ]';
      }
      // Otherwise, fall through to convert the array as an object.
    case 'object':
      $output = array();
      foreach ($var as $k => $v) {
        $output[] = drupal_to_js_mean(strval($k)) .': '. drupal_to_js_mean($v);
      }
      return '{ '. implode(', ', $output) .' }';
    default:
      return 'null';
  }
}