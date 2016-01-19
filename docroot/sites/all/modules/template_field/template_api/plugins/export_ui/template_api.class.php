<?php

class TemplateAPIExportUI extends ctools_export_ui {

  /**
   * Add new filter to template list page
   */
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    //create alter to allow other modules a chance to change the form
    drupal_alter('template_api_list_form', $form, $form_state);

    return;
  }

  /**
   * Handles new filter values added to template list page
   */
  function list_filter($form_state, $item) {
    //if you added a new filter you need to handle the value return here
    foreach (module_implements('template_api_list_filter') as $module) {
      $function = $module . '_template_api_list_filter';
      if (call_user_func($function, $form_state, $item) === TRUE) {
        return TRUE;
      }
    }

    return parent::list_filter($form_state, $item);
  }

  /**
   * Add new column to template list page
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $name = $item->{$this->plugin['export']['key']};
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Note: $item->{$schema['export']['export type string']} should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $name;
        break;
      case 'title':
        $this->sorts[$name] = $item->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array('data' => check_plain($item->{$this->plugin['export']['admin_title']}), 'class' => array('ctools-export-ui-title'));
    }
    $this->rows[$name]['data'][] = array('data' => check_plain($name), 'class' => array('ctools-export-ui-name'));

    if (!$owner = template_api_get_owner_type_controller($item->owner_type)->getLabel($item)) {
      $owner = t('No Owner');
    }

    $this->rows[$name]['data'][] = array('data' => check_plain($owner), 'class' => array('ctools-export-ui-owner'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }

  /**
   * Add table row to template list page
   * If you added a new column to the list page make sure you add the table header
   */
  function list_table_header() {
    $header = array();
    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    }

    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Owner'), 'class' => array('ctools-export-ui-owner'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    //allow other modules a chance to add table headers
    drupal_alter('template_api_list_table_header', $header);


    return $header;

  }

  /**
   * Menu callback to determine if an operation is accessible.
   *
   * This function enforces a basic access check on the configured perm
   * string, and then additional checks as needed.
   *
   * @param $op
   *   The 'op' of the menu item, which is defined by 'allowed operations'
   *   and embedded into the arguments in the menu item.
   * @param $item
   *   If an op that works on an item, then the item object, otherwise NULL.
   *
   * @return
   *   TRUE if the current user has access, FALSE if not.
   */
  function access($op, $item) {
    $base = 'template api ';

    if (!user_access($this->plugin['access'])) {
      return FALSE;
    }

    if ($op !== 'list' && !user_access($base . $op)) {
      return FALSE;
    }

    // If we need to do a token test, do it here.
    if (!empty($this->plugin['allowed operations'][$op]['token']) && (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], $op))) {
      return FALSE;
    }

    if ($item && FALSE === template_api_get_owner_type_controller($item->owner_type)->checkAccess($op, $item)) {
      return FALSE;
    }

    switch ($op) {
      case 'revert':
        return ($item->export_type & EXPORT_IN_DATABASE) && ($item->export_type & EXPORT_IN_CODE);
      case 'delete':
        return ($item->export_type & EXPORT_IN_DATABASE) && !($item->export_type & EXPORT_IN_CODE);
      case 'disable':
        return empty($item->disabled);
      case 'enable':
        return !empty($item->disabled);
      default:
        return TRUE;
    }
  }

  /**
   * Handle the submission of the edit form.
   *
   * At this point, submission is successful. Our only responsibility is
   * to copy anything out of values onto the item that we are able to edit.
   *
   * If the keys all match up to the schema, this method will not need to be
   * overridden.
   */
  function edit_form_submit(&$form, &$form_state) {
    $form_state['item']->name = $form_state['values']['name'];
    $form_state['item']->label = $form_state['values']['label'];
    $form_state['item']->description = $form_state['values']['description'];
    $form_state['item']->tags = empty($form_state['values']['tags']) ? array() :
      explode(',', $form_state['values']['tags']);
    $form_state['item']->renderer = $form_state['values']['renderer'];
    $form_state['item']->owner = $form_state['values']['owner'];
    $form_state['item']->owner_type = $form_state['values']['owner_type'];

    $form_state['item']->content = $form_state['values']['content']['value'];

    $input_values = array();

    // Sort the input fields by weight so that they
    // are saved to the db in the desired order.
    uasort($form_state['values']['inputs'], array('TemplateAPIExportUI', 'sort_cmp'));

    foreach ($form_state['values']['inputs'] as $input_key => $input_definition) {
      if (empty($input_definition['remove'])) {
        if ($input_key === 'new') {
          if (!empty($input_definition['key'])) {
            $key = $input_definition['key'];
          }
          else {
            $key = drupal_clean_css_identifier($input_definition['label']);
          }
          if (!empty($key)) {
            $input_values[$key] = $input_definition;
          }
        }
        else {
          $input_values[$input_key] = $input_definition;
        }
      }
    }

    // Restructure the inputs array from a flat structure to a nested structure.
    // We do it here so that the restructuring doesn't have to happen during the rendering.
    $nested_input_values = TemplateAPIExportUI::make_nested_array($input_values);

    $form_state['item']->inputs = $nested_input_values;

    $attached = $form_state['values']['attached'];
    $attached_values = array();

    $attached_values['js'] = $attached['js']['js_content']['value'];
    $attached_values['css'] = $attached['css']['css_content']['value'];

    $library_values = array_filter($attached['libraries']['libraries']);
    $libraries = array();
    foreach ($library_values as $value) {
      if (!empty($value)) {
        list($module, $library_name) = explode(':::', $value);
        $libraries[] = array('name' => $library_name, 'module' => $module);
      }
    }

    $attached_values['libraries'] = $libraries;

    $form_state['item']->attached = $attached_values;
  }

  /**
   * @static
   * Custom sort handler for sorting the input fields.
   *
   * @param $a
   * @param $b
   * @return int
   */
  static function sort_cmp($a, $b) {
    if ($a['depth'] != $b['depth']) {
      return ($a['depth'] < $b['depth']) ? -1 : 1;
    }
    else {
      if ($a['weight'] != $b['weight']) {
        return ($a['weight'] < $b['weight']) ? -1 : 1;
      }
      else {
        if (empty($a['parent'])) {
          return -1;
        }
        elseif (empty($b['parent'])) {
          return 1;
        }
        else {
          return 0;
        }
      }
    }

    // We shouldn't get here.
    return 0;
  }

  /**
   * @static
   * Converts the flat array structure from the input form into
   * a nested array that can be used for rendering.
   *
   * @param $input_values
   */
  static function make_nested_array($input_values) {
    $return = array();
    foreach ($input_values as $key => $input) {
      if (isset($input['parent']) && !empty($input['parent'])) {
        $parent = $input['parent'];
        $depth = $input['depth'];
        if ($depth == 1) {
          $return[$parent]['children'][$key] = $input;
        }
        elseif ($depth == 2) {
          $parent_parent_key = '';
          foreach ($return as $parent_key => $parent_value) {
            if (isset($parent_value['children'][$parent])) {
              $parent_parent_key = $parent_key;
              break;
            }
          }
          if (!empty($parent_parent_key)) {
            $return[$parent_parent_key]['children'][$parent]['children'][$key] = $input;
          }
        }

      }
      else {
        if (!isset($return[$key])) {
          $return[$key] = $input;
        }
        else {
          $return[$key] = array_merge($return[$key], $input);
        }
      }
    }

    return $return;
  }

  /**
   * Builds the operation links for a specific exportable item.
   */
  function build_operations($item) {
    $plugin = $this->plugin;
    $schema = ctools_export_get_schema($plugin['schema']);
    $operations = $plugin['allowed operations'];
    $operations['import'] = FALSE;

    if ($item->{$schema['export']['export type string']} == t('Normal')) {
      $operations['revert'] = FALSE;
    }
    elseif ($item->{$schema['export']['export type string']} == t('Overridden')) {
      $operations['delete'] = FALSE;
    }
    else {
      $operations['revert'] = FALSE;
      $operations['delete'] = FALSE;
    }
    if (empty($item->disabled)) {
      $operations['enable'] = FALSE;
    }
    else {
      $operations['disable'] = FALSE;
    }

    $allowed_operations = array();

    foreach ($operations as $op => $info) {
      if (!empty($info)) {
        if ($this->access($op, $item)) {

          $allowed_operations[$op] = array(
            'title' => $info['title'],
            'href' => ctools_export_ui_plugin_menu_path($plugin, $op, $item->{$this->plugin['export']['key']}),
          );
          if (!empty($info['ajax'])) {
            $allowed_operations[$op]['attributes'] = array('class' => array('use-ajax'));
          }
          if (!empty($info['token'])) {
            $allowed_operations[$op]['query'] = array('token' => drupal_get_token($op));
          }
        }
      }
    }

    return $allowed_operations;
  }
}


