<?php

/**
 * @file
 * Ctools export UI file.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'handler'               => 'blockqueue_export_ui',
  // As defined in hook_schema().
  'schema'                => 'blockqueue',
  // Define a permission users must have to access these pages.
  'access'                => 'administer blockqueue',
  'create access'         => 'administer blockqueue create',
  'update access'         => 'administer blockqueue update',
  'delete access'         => 'administer blockqueue delete',
  'import access'         => 'administer blockqueue import',
  'enable access'         => 'administer blockqueue enable',
  // Define the menu item.
  'menu'                  => array(
    'menu item'        => 'blockqueue',
    'menu title'       => t('Block Queue'),
    'menu prefix'      => 'admin/structure',
    'menu description' => t('Administer Blockqueues.'),
    'items'            => array(
      'remove' => array(
        'path'             => 'list/%blockqueue_block/remove/%',
        'title'            => t('Remove block from blockqueue'),
        'page callback'    => 'drupal_get_form',
        'page arguments'   => array('blockqueue_block_remove_form', 4, 6),
        'access callback'  => 'ctools_export_ui_task_access',
        'access arguments' => array(
          'blockqueue_export_ui',
          'remove',
          4,
        ),
        'type'             => MENU_CALLBACK,
        'load arguments'   => array(6),
      ),
    ),
  ),
  // Define user interface texts.
  'title singular'        => t('blockqueue'),
  'title plural'          => t('blockqueues'),
  'title singular proper' => t('Blockqueue'),
  'title plural proper'   => t('Blockqueues'),
  // Define the names of the functions that provide the add/edit forms.
  'form'                  => array(
    'settings' => 'blockqueue_export_ui_form',
    'validate' => 'blockqueue_export_ui_form_validate',
    'submit'   => 'blockqueue_export_ui_form_submit',
  ),
  'callback'              => array(
    'save' => 'blockqueue_export_crud_save',
  ),
);

/**
 * Class blockqueue_export_ui
 */
class blockqueue_export_ui extends ctools_export_ui {
  /**
   * Menu callback to determine if an operation is accessible.
   *
   * This function enforces a basic access check on the configured perm
   * string, and then additional checks as needed.
   *
   * @param string $op
   *   The 'op' of the menu item, which is defined by 'allowed operations'
   *   and embedded into the arguments in the menu item.
   * @param object $item
   *   If an op that works on an item, then the item object, otherwise NULL.
   *
   * @return bool
   *   TRUE if the current user has access, FALSE if not.
   */
  function access($op, $item) {
    if (!user_access($this->plugin['access'])) {
      return FALSE;
    }

    // More fine-grained access control:
    if (($op == 'add' || $op == 'clone') && !user_access($this->plugin['create access'])) {
      return FALSE;
    }

    // More fine-grained access control:
    if (($op == 'edit' || $op == 'remove') && !user_access($this->plugin['update access'])) {
      return FALSE;
    }

    // More fine-grained access control:
    if (($op == 'revert' || $op == 'delete') && !user_access($this->plugin['delete access'])) {
      return FALSE;
    }

    // More fine-grained access control:
    if (($op == 'import' || $op == 'export') && !user_access($this->plugin['import access'])) {
      return FALSE;
    }

    // If we need to do a token test, do it here.
    if (!empty($this->plugin['allowed operations'][$op]['token']) && (!isset($_GET['token']) || !drupal_valid_token(
          $_GET['token'],
          $op
        ))
    ) {
      return FALSE;
    }

    switch ($op) {
      case 'import':
        return user_access('use PHP for settings');

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
   * Master entry point for handling a list.
   *
   * It is unlikely that a child object will need to override this method,
   * unless the listing mechanism is going to be highly specialized.
   */
  function list_page($js, $input) {
    $page = parent::list_page($js, $input);

    ctools_add_css('blockqueue.admin', 'blockqueue');

    return $page;
  }

  /**
   * Create the filter/sort form at the top of a list of exports.
   *
   * This handles the very default conditions, and most lists are expected
   * to override this and call through to parent::list_form() in order to
   * get the base form and then modify it as necessary to add search
   * gadgets for custom fields.
   */
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);

    // Put filter and sort criteria on the same line.
    $form['top row']['#suffix']    = '';
    $form['bottom row']['#prefix'] = '';
  }

  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  function list_table_header() {
    $header = array();

    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    }

    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Count'), 'class' => array('ctools-export-ui-count'));
    $header[] = array('data' => t('Size'), 'class' => array('ctools-export-ui-size'));

    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    return $header;
  }

  /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting.
    $name   = $item->{$this->plugin['export']['key']};
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Note: $item->{$schema['export']['export type string']}
    // should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $name;
        break;

      case 'admin_title':
      case 'title':
        $this->sorts[$name] = $item->admin_title;
        break;

      case 'name':
        $this->sorts[$name] = $name;
        break;

      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    // Build count and size cols.
    $count = empty($item->blocks) ? '<em>' . check_plain(t('Empty')) . '</em>' : count($item->blocks);
    $size  = empty($item->size) ? '<em>' . check_plain(t('Unlimited')) . '</em>' : (int) $item->size;

    $this->rows[$name]['data']  = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array(
        'data'  => check_plain($item->{$this->plugin['export']['admin_title']}),
        'class' => array('ctools-export-ui-title'),
      );
    }
    $this->rows[$name]['data'][] = array('data' => check_plain($name), 'class' => array('ctools-export-ui-name'));
    $this->rows[$name]['data'][] = array(
      'data'  => check_plain($item->{$schema['export']['export type string']}),
      'class' => array('ctools-export-ui-storage'),
    );
    $this->rows[$name]['data'][] = array('data' => $count, 'class' => array('ctools-export-ui-count'));
    $this->rows[$name]['data'][] = array('data' => $size, 'class' => array('ctools-export-ui-size'));

    $ops = theme(
      'links__ctools_dropbutton',
      array(
        'links'      => $operations,
        'attributes' => array('class' => array('links', 'inline')),
      )
    );

    $this->rows[$name]['data'][] = array(
      'data'  => $ops,
      'class' => array('ctools-export-ui-operations'),
    );

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }
}
