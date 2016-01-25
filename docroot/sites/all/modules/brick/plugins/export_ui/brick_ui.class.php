<?php

class brick_ui extends ctools_export_ui {
  function init($plugin) {
    parent::init($plugin);
    ctools_include('context');
  }

  function hook_menu(&$info) {
    parent::hook_menu($info);

    $prefix = ctools_export_ui_plugin_base_path($this->plugin);
    $path = $this->plugin['menu']['items']['edit']['path'];

    $info[$prefix .'/'. $path]['context'] = MENU_CONTEXT_INLINE;
    $info[$prefix .'/'. $path]['title callback'] = 'brick_menu_title';
    $info[$prefix .'/'. $path]['title arguments'] = array(4);
  }

  /**
   * Execute the form.
   *
   * Add and Edit both funnel into this, but they have a few different
   * settings.
   */
  function edit_execute_form(&$form_state) {
    ctools_include('context');
    ctools_include('content');

    // Never save the content type plugin into form state as this will overwrite
    // the export_ui plugin itself.
    $plugin = ctools_get_content_type($form_state['item']->content_type);
    $subtype = ctools_content_get_subtype($plugin, $form_state['item']->subtype);

    $form_state += array(
      'contexts' => ctools_context_load_contexts($form_state['item']),
      'subtype' => $subtype,
      'subtype_name' => $form_state['item']->subtype,
      'conf' => $form_state['item']->conf + ctools_content_get_defaults($plugin, $subtype),
      'access' => $form_state['item']->access,
    );

    return parent::edit_execute_form($form_state);
  }

  function get_wizard_info(&$form_state) {
    $form_info = parent::get_wizard_info($form_state);

    ctools_include('context');
    ctools_include('content');

    $op = $form_state['op'];
    $content_form_info = array();
    $plugin = ctools_get_content_type($form_state['item']->content_type);

    $subtype_name = $form_state['subtype_name'];
    $subtype = $form_state['subtype'];
    $conf = $form_state['conf'];
    $step = $form_state['step'];

    if (!empty($subtype[$op .' form'])) {
      _ctools_content_create_form_info($content_form_info, $subtype[$op .' form'], $subtype, $subtype, $op, $step);
    }
    else if (!empty($plugin[$op .' form'])) {
      _ctools_content_create_form_info($content_form_info, $plugin[$op .' form'], $plugin, $subtype, $op, $step);
    }
    // Use the edit form for the add form if add form was completely left off.
    else if (!empty($subtype['edit form'])) {
      _ctools_content_create_form_info($content_form_info, $subtype['edit form'], $subtype, $subtype, $op);
    }
    else if (!empty($plugin['edit form'])) {
      _ctools_content_create_form_info($content_form_info, $plugin['edit form'], $plugin, $subtype, $op);
    }

    // Add the submit handler to copy content type configuration settings to the
    // brick item.
    if (!empty($content_form_info['forms'])) {
      foreach ($content_form_info['forms'] as &$info) {
        if (empty($info['submit'])) {
          $info['submit'] = 'brick_conf_submit';
        }
      }
    }

    return array_merge_recursive($form_info, $content_form_info);
  }

  function list_sort_options() {
    return array(
      'disabled' => t('Enabled, title'),
      'title' => t('Title'),
      'name' => t('Name'),
      'storage' => t('Storage'),
      'content_type' => t('Content type'),
      'subtype' => t('Subtype'),
    );
  }

  function list_build_row($item, &$form_state, $operations) {
    ctools_include('content');
    $content_type = ctools_get_content_type($item->content_type);
    $subtype = ctools_content_get_subtype($item->content_type, $item->subtype);

    // Set up sorting
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$item->name] = empty($item->disabled) . $item->admin_title;
        break;
      case 'title':
        $this->sorts[$item->name] = $item->admin_title;
        break;
      case 'name':
        $this->sorts[$item->name] = $item->name;
        break;
      case 'storage':
        $this->sorts[$item->name] = $item->type . $item->admin_title;
        break;
      case 'content_type':
        $this->sorts[$item->name] = $content_type['title'];
        break;
      case 'subtype':
        $this->sorts[$item->name] = $subtype['title'];
        break;
    }

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$item->name] = array(
      'data' => array(
        array('data' => check_plain($item->admin_title), 'class' => array('ctools-export-ui-title')),
        array('data' => check_plain($item->name), 'class' => array('ctools-export-ui-name')),
        array('data' => check_plain($content_type['title']), 'class' => array('ctools-export-ui-content-type')),
        array('data' => check_plain($subtype['title']), 'class' => array('ctools-export-ui-content-type')),
        array('data' => $item->type, 'class' => array('ctools-export-ui-storage')),
        array('data' => $ops, 'class' => array('ctools-export-ui-operations')),
      ),
      'title' => !empty($item->admin_description) ? check_plain($item->admin_description) : '',
      'class' => array(!empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled'),
    );
  }

  function list_table_header() {
    return array(
      array('data' => t('Title'), 'class' => array('ctools-export-ui-title')),
      array('data' => t('Name'), 'class' => array('ctools-export-ui-name')),
      array('data' => t('Content type'), 'class' => array('ctools-export-ui-content-type')),
      array('data' => t('Subtype'), 'class' => array('ctools-export-ui-subtype')),
      array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage')),
      array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations')),
    );
  }

  function edit_form(&$form, &$form_state) {
    // Get the basic edit form
    parent::edit_form($form, $form_state);

    $form['category'] = array(
      '#type' => 'textfield',
      '#size' => 24,
      '#default_value' => $form_state['item']->category,
      '#title' => t('Category'),
      '#required' => TRUE,
      '#description' => t("The category that this block will be grouped into on the block admin page. Only upper and lower-case alphanumeric characters are allowed. If left blank, defaults to 'Brick'."),
    );

    $form['cache'] = array(
      '#type' => 'radios',
      '#default_value' => $form_state['item']->cache,
      '#title' => t('Cache'),
      '#options' => array(
        DRUPAL_NO_CACHE => t('Do not cache'),
        DRUPAL_CACHE_GLOBAL => t('Cache once for everything (global)'),
        DRUPAL_CACHE_PER_PAGE => t('Per page'),
        DRUPAL_CACHE_PER_ROLE => t('Per role'),
        DRUPAL_CACHE_PER_ROLE | DRUPAL_CACHE_PER_PAGE => t('Per role per page'),
        DRUPAL_CACHE_PER_USER => t('Per user'),
        DRUPAL_CACHE_PER_USER | DRUPAL_CACHE_PER_PAGE => t('Per user per page'),
      ),
      '#description' => t('This cache setting only matters if block caching is enabled.'),
    );

    $form['title']['#title'] = t('Title');
    $form['title']['#description'] = t('The title for this brick. It can be overridden in the block configuration.');
  }

  /**
   * Validate submission of the brick edit form.
   */
  function edit_form_validate($form, &$form_state) {
    parent::edit_form_validate($form, $form_state);

    // This method is called for all steps which don't have their own validation
    // handler function. Only perform validation on the basic step.
    if ($form_state['step'] == 'basic') {
      if (preg_match("/[^A-Za-z0-9 ]/", $form_state['values']['category'])) {
        form_error($form['category'], t('Categories may contain only alphanumerics or spaces.'));
      }
    }
  }

  function edit_form_submit(&$form, &$form_state) {
    parent::edit_form_submit($form, $form_state);
    $form_state['item']->category = $form_state['values']['category'];
    $form_state['item']->cache = $form_state['values']['cache'];
  }

  function edit_form_context(&$form, &$form_state) {
    ctools_include('context-admin');
    ctools_context_admin_includes();
    ctools_add_css('ruleset');

    // Set this up and we can use CTools' Export UI's built in wizard caching,
    // which already has callbacks for the context cache under this name.
    $module = 'export_ui::' . $this->plugin['name'];
    $name = $this->edit_cache_get_key($form_state['item'], $form_state['form type']);

    // This is wrong but it works, mostly. Error messages when editing existing
    // argument contexts are not fatal and can probably be ignored.
    ctools_context_add_context_form($module, $form, $form_state, $form['contexts_table'], $form_state['item'], $name);
    ctools_context_add_required_context_form($module, $form, $form_state, $form['required_contexts_table'], $form_state['item'], $name);
    ctools_context_add_relationship_form($module, $form, $form_state, $form['relationships_table'], $form_state['item'], $name);
  }

  function edit_form_context_submit(&$form, &$form_state) {
    // Prevent this from going to edit_form_submit();
  }

  function edit_form_content_type(&$form, &$form_state) {
    $available_types = ctools_content_get_available_types($form_state['contexts']);
    $options = array();
    foreach ($available_types as $plugin => $subtypes) {
      foreach ($subtypes as $subtype_id => $subtype) {

        // For some reason, the category might be an array. This happens with
        // views content panes.
        if (is_array($subtype['category'])) {
          $options[array_shift($subtype['category'])][$plugin .'-'. $subtype_id] = $subtype['title'];
        } else {
          $options[$subtype['category']][$plugin .'-'. $subtype_id] = $subtype['title'];
        }
      }
    }

    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => 'Content type',
      '#default_value' => $form_state['item']->content_type .'-'. $form_state['item']->subtype,
      '#options' => $options,
    );
  }

  function edit_form_content_type_submit(&$form, &$form_state) {
    list($content_type, $subtype_name) = explode('-', $form_state['values']['content_type'], 2);

    $plugin = ctools_get_content_type($content_type);
    $subtype = ctools_content_get_subtype($plugin, $subtype_name);

    $form_state['item']->content_type = $content_type;
    $form_state['item']->subtype = $subtype_name;
    $form_state['item']->conf = ctools_content_get_defaults($plugin, $subtype);
  }

  function edit_form_rules(&$form, &$form_state) {
    // The 'access' UI passes everything via $form_state, unlike the 'context' UI.
    // The main difference is that one is about 3 years newer than the other.
    ctools_include('context');
    ctools_include('context-access-admin');

    $form_state['module'] = 'ctools_export_ui';
    $form_state['callback argument'] = $form_state['object']->plugin['name'] . ':' . $form_state['object']->edit_cache_get_key($form_state['item'], $form_state['form type']);
    $form_state['no buttons'] = TRUE;

    $form = ctools_access_admin_form($form, $form_state);
  }

  function edit_form_rules_submit(&$form, &$form_state) {
    $form_state['item']->access['logic'] = $form_state['values']['logic'];
  }
}
