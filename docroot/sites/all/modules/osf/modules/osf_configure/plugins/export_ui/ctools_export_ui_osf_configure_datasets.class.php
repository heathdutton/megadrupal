<?php

class ctools_export_ui_osf_configure_datasets extends ctools_export_ui {

  /**
   * Master entry point for handling a list.
   *
   * It is unlikely that a child object will need to override this method,
   * unless the listing mechanism is going to be highly specialized.
   */
  function list_page($js, $input) {
    
    $this->items = osf_configure_dataset_load_util('all', TRUE);
    
    // Respond to a reset command by clearing session and doing a drupal goto
    // back to the base URL.
    if (isset($input['op']) && $input['op'] == t('Reset')) {
      unset($_SESSION['ctools_export_ui'][$this->plugin['name']]);
      if (!$js) {
        return drupal_goto($_GET['q']);
      }
      // clear everything but form id, form build id and form token:
      $keys = array_keys($input);
      foreach ($keys as $id) {
        if (!in_array($id, array('form_id', 'form_build_id', 'form_token'))) {
          unset($input[$id]);
        }
      }
      $replace_form = TRUE;
    }

    // If there is no input, check to see if we have stored input in the
    // session.
    if (!isset($input['form_id'])) {
      if (isset($_SESSION['ctools_export_ui'][$this->plugin['name']]) && is_array($_SESSION['ctools_export_ui'][$this->plugin['name']])) {
        $input  = $_SESSION['ctools_export_ui'][$this->plugin['name']];
      }
    }
    else {
      $_SESSION['ctools_export_ui'][$this->plugin['name']] = $input;
      unset($_SESSION['ctools_export_ui'][$this->plugin['name']]['q']);
    }

    // This is where the form will put the output.
    $this->rows = array();

    $form_state = array(
      'plugin' => $this->plugin,
      'input' => $input,
      'rerender' => TRUE,
      'no_redirect' => TRUE,
      'object' => &$this,
    );
    if (!isset($form_state['input']['form_id'])) {
      $form_state['input']['form_id'] = 'ctools_export_ui_list_form';
    }

    // If we do any form rendering, it's to completely replace a form on the
    // page, so don't let it force our ids to change.
    if ($js && isset($_POST['ajax_html_ids'])) {
      unset($_POST['ajax_html_ids']);
    }
    
    $form = drupal_build_form('ctools_export_ui_list_form', $form_state);
    $form = drupal_render($form);

    $output = $this->list_header($form_state) . $this->list_render($form_state) . $this->list_footer($form_state);
    
    if (!$js) {
      $this->list_css();
      return $form . $output;
    }

    $commands = array();
    $commands[] = ajax_command_replace('#ctools-export-ui-list-items', $output);
    if (!empty($replace_form)) {
      $commands[] = ajax_command_replace('#ctools-export-ui-list-form', $form);
    }
    
    print ajax_render($commands);
    ajax_footer();
  }

  /**
   * Create the filter form at the top of a list of exports.
   *
   * This handles the very default conditions, and most lists are expected
   * to override this and call through to parent::list_form() in order to
   * get the base form and then modify it as necessary to add search
   * gadgets for custom fields.
   */
  function list_form(&$form, &$form_state) {

    // This forces the form to *always* treat as submitted which is
    // necessary to make it work.
    $form['#token'] = FALSE;
    if (empty($form_state['input'])) {
      $form["#post"] = TRUE;
    }

    // Add the 'q' in if we are not using clean URLs or it can get lost when
    // using this kind of form.
    if (!variable_get('clean_url', FALSE)) {
      $form['q'] = array(
        '#type' => 'hidden',
        '#value' => $_GET['q'],
      );
    }

    $all = array('all' => t('- All -'));

    $form['top row'] = array(
      '#prefix' => '<div class="ctools-export-ui-row ctools-export-ui-top-row clearfix">',
      '#suffix' => '</div>',
    );

    $form['top row']['search'] = array(
      '#type' => 'textfield',
      '#title' => t('Search'),
    );

    $form['top row']['storage'] = array(
      '#type' => 'hidden',
      '#value' => 'all'
    );

    $form['top row']['disabled'] = array(
      '#type' => 'hidden',
      '#value' => 'all'
    );

    $form['top row']['submit'] = array(
      '#type' => 'submit',
      '#id' => 'ctools-export-ui-list-items-apply',
      '#value' => t('Apply'),
      '#attributes' => array('class' => array('use-ajax-submit ctools-auto-submit-click')),
    );

    $form['top row']['reset'] = array(
      '#type' => 'submit',
      '#id' => 'ctools-export-ui-list-items-apply',
      '#value' => t('Reset'),
      '#attributes' => array('class' => array('use-ajax-submit')),
    );

    $form['#prefix'] = '<div class="clearfix">';
    $form['#suffix'] = '</div>';
    $form['#attached']['js'] = array(ctools_attach_js('auto-submit'));
    $form['#attached']['library'][] = array('system', 'drupal.ajax');
    $form['#attached']['library'][] = array('system', 'jquery.form');
    $form['#attributes'] = array('class' => array('ctools-auto-submit-full-form'));
  }

   /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   */
  function list_build_row($item, &$form_state, $operations) {

    // Set up sorting
    if (property_exists($item, 'available') && $item->available) {
      $name = $item->uri;
    }
    else {
      $name = $item->{$this->plugin['export']['key']};
    }
    $schema = ctools_export_get_schema($this->plugin['schema']);

    $this->rows[$name]['data'] = array();

    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    if (property_exists($item, 'available') && $item->available) {
      $this->rows[$name]['class'] = array('ctools-export-ui-disabled');
      $this->rows[$name]['data']['available'] = TRUE;
      $this->rows[$name]['data']['scdid'] = $item->uri;
    }
    else {
      $this->rows[$name]['data']['available'] = FALSE;
      $this->rows[$name]['data']['scdid'] = $item->scdid;
    }

    if($item->endpoint) {
      $this->rows[$name]['data']['endpoint'] = $item->endpoint->label;
      $this->rows[$name]['data']['color'] = array('data' => check_plain($item->endpoint->color));
    }

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data']['title'] = array('data' => check_plain($item->{$this->plugin['export']['admin_title']}), 'class' => array('ctools-export-ui-title'));
    }

    $this->rows[$name]['data']['uri'] = array('data' => $item->uri, 'class' => array('osf_configure-uri'));

    $this->rows[$name]['data']['origin'] = array('data' => $item->origin, 'class' => array('osf_configure-origin'));

    global $OSF_CONFIGURE_DATASET_TYPES;
    global $base_url;
    
    $this->rows[$name]['data']['active'] = array('data' => $OSF_CONFIGURE_DATASET_TYPES[$item->active]);

    if (property_exists($item, 'available') && $item->available) {
      $operations = array(
        'add' => array(
          'title' => t('Add'),
          'href' => $_GET['q'] .'/add',
          'query' => array(
            'uri' => urlencode($item->uri),
          ),
        ),
        'delete' => array(
          'title' => t('Delete on OSF'),
          'href' => $_GET['q'] .'/delete/osf',
          'query' => array(
            'uri' => $item->uri,
          ),
        ),
      );
    }
    else
    {
      $operations['add'] = array(
          'title' => t('Permissions'),
          'href' => $base_url.'/admin/people/permissions#edit-create-dataset-'.$item->scdid.'-'.$item->sceid
        ); 
        
      $operations['delete']['title'] = t('Unregister');        
    }
    
    unset($operations['disable']);
    unset($operations['enable']);
    unset($operations['clone']);

    $operations['export']['title'] = t('Export feature');
    
    $operations['export_data'] = array(
        'title' => t('Export'),
        'href' => $base_url.'/osf/export'
    ); 
    
    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$name]['data']['ops'] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));
    $this->rows[$name]['data']['storage'] = array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));
    
  /************* Datasets ***********
    * osf_configure_datasets
    ** scdid
    ** label
    ** machine_name
    ** sceid (endpoint)
    ** remote_label
    ** remote_description
    */

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
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
      $header['label'] = array('data' => t('Dataset'), 'class' => array('osf_configure-title'));
    }
    $header['endpoint'] = array('data' => t('Endpoint'), 'class' => array('osf_configure-endpoint'));
    $header['uri'] = array('data' => t('URI'), 'class' => array('osf_configure-uri'));
    $header['active'] = array('data' => t('Type'), 'class' => array('osf_configure-active'));
    $header['ops'] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));
    $header['storage'] = array('data' => t('Feature Status'), 'class' => array('ctools-export-ui-storage'));
    return $header;
  }

  /**
   * Render all of the rows together.
   *
   * By default we place all of the rows in a table, and this should be the
   * way most lists will go.
   *
   * Whatever you do if this method is overridden, the ID is important for AJAX
   * so be sure it exists.
   */
  function list_render(&$form_state) {
    $form = drupal_build_form('osf_configure_datasets_tableselect_form', $form_state);
    return drupal_render($form);
  }

  /**
   * Create the filter/sort form at the top of a list of exports.
   *
   * This handles the very default conditions, and most lists are expected
   * to override this and call through to parent::list_form() in order to
   * get the base form and then modify it as necessary to add search
   * gadgets for custom fields.
   */
  function tableselect_form(&$form, &$form_state) {
    $rows = array();
    foreach($this->rows as $row) {
      $rows[$row['data']['scdid']] = array(
        'endpoint' => array(
          'data' => $row['data']['endpoint'],
          'style' => 'background: #' . $row['data']['color']['data'],
        ),
        'uri' => $row['data']['uri']['data'],
        'active' =>  $row['data']['active']['data'],
        'ops' =>  $row['data']['ops']['data'],
        'storage' =>  $row['data']['storage']['data'],
        '#attributes' => array(
          'class' => $row['class'],
        ),
      );

      if ($row['data']['available']) {
        $rows[$row['data']['scdid']]['label'] = array(
          'data' => array(
            '#markup' => t('Available') .": ". $row['data']['title']['data'],
          ),
        );
      }
      else {
        $rows[$row['data']['scdid']]['label'] = array(
          'data' => array(
            '#markup' => $row['data']['title']['data'],
          ),
        );
      }
    }

    $form = array();
    $form['datasets'] = array(
      '#type' => 'tableselect',
      '#header' =>  $this->list_table_header(),
      '#options' => $rows,
      '#attributes' => array('id' => 'ctools-export-ui-list-items'),
      '#empty' => $this->plugin['strings']['message']['no items'],
    );
  }

  /**
   * Validate the filter/sort form.
   *
   * It is very rare that a filter form needs validation, but if it is
   * needed, override this.
   */
  function tableselect_form_validate(&$form, &$form_state) { }

  /**
   * Submit the filter/sort form.
   *
   * This submit handler is actually responsible for building up all of the
   * rows that will later be rendered, since it is doing the filtering and
   * sorting.
   *
   * For the most part, you should not need to override this method, as the
   * fiddly bits call through to other functions.
   */
  function tableselect_form_submit(&$form, &$form_state) {
  }

  /**
   * Submit the filter/sort form.
   *
   * This submit handler is actually responsible for building up all of the
   * rows that will later be rendered, since it is doing the filtering and
   * sorting.
   *
   * For the most part, you should not need to override this method, as the
   * fiddly bits call through to other functions.
   */
  function list_form_submit(&$form, &$form_state) {
    // Filter and re-sort the pages.
    $plugin = $this->plugin;
    $prefix = ctools_export_ui_plugin_base_path($plugin);
    $this->items = osf_configure_dataset_load_util('all', TRUE);
    foreach ($this->items as $name => $item) {
      // Call through to the filter and see if we're going to render this
      // row. If it returns TRUE, then this row is filtered out.
      if ($this->list_filter($form_state, $item)) {
        continue;
      }

      $operations = $this->build_operations($item);
      $this->list_build_row($item, $form_state, $operations);
    }
  }

}

// -----------------------------------------------------------------------
// Forms to be used with this class.
//
// Since Drupal's forms are completely procedural, these forms will
// mostly just be pass-throughs back to the object.

/**
 * Form callback to handle the filter/sort form when listing items.
 *
 * This simply loads the object defined in the plugin and hands it off.
 */
function osf_configure_datasets_tableselect_form($form, $form_state) {
  $form_state['object']->tableselect_form($form, $form_state);
  return $form;
}

/**
 * Validate handler for ctools_export_ui_osf_configure_tableselect_form.
 */
function osf_configure_datasets_tableselect_form_validate(&$form, &$form_state) {
  $form_state['object']->tableselect_form_validate($form, $form_state);
}

/**
 * Submit handler for ctools_export_ui_osf_configure_tableselect_form.
 */
function osf_configure_datasets_tableselect_form_submit(&$form, &$form_state) {
  $form_state['object']->tableselect_form_submit($form, $form_state);
}
