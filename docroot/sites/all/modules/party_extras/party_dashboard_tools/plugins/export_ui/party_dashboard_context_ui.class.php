<?php

class party_dashboard_context_ui extends ctools_export_ui {

  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    $context = $form_state['item'];

    $form['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => isset($context->title) ? $context->title: NULL,
      '#description' => t('This is the title that gets displayed on the tab.'),
    );

    $form['settings']['path'] = array(
      '#title' => t('Path'),
      '#type' => 'textfield',
      '#default_value' => isset($context->settings['path']) ? $context->settings['path']: NULL,
      '#description' => t('The path fragment for this context.'),
    );

    $form['settings']['access'] = array(
      '#title' => t('Access'),
      '#type' => 'textarea',
      '#disabled' => TRUE,
      '#default_value' => var_export(isset($context->settings['access']) ? $context->settings['access']: array(), true),
      '#description' => t('The access array for this context. This is currently read only.'),
    );

    $tasks = page_manager_get_tasks_by_type('page');
    $pages = array('operations' => array(), 'tasks' => array());
    module_load_include('inc', 'page_manager', 'page_manager.admin');
    page_manager_get_pages($tasks, $pages);
    if (!empty($pages['rows'])) {
      $form['settings']['pages'] = array(
        '#title' => t('Pages'),
        '#type' => 'checkboxes',
        '#default_value' => isset($context->settings['pages']) ? $context->settings['pages']: array('_all_' => '_all_'),
        '#options' => array('_all_' => t('All Pages')),
        '#description' => t('Select which pages this tab is active on.'),
      );
      foreach ($pages['rows'] as $page) {
        $form['settings']['pages']['#options'][$page['data']['name']['data']] = format_string('@title <em>[@name]</em> <strong>!path</strong>', array(
          '@title' => $page['data']['title']['data'],
          '@name' => $page['data']['name']['data'],
          '!path' => $page['data']['path']['data'],
        ));
      }
    }

  }

  function edit_form_submit(&$form, &$form_state) {
    // We can't cope with access yet...
    unset($form_state['values']['settings']['access']);

    parent::edit_form_submit($form, $form_state);

    // Since items in our settings are not in the schema, we have to do these manually:
    $form_state['item']->settings['path'] = $form_state['values']['path'];
    $form_state['item']->settings['pages'] = $form_state['values']['pages'];
  }

  function list_sort_options() {
    return array(
      'disabled' => t('Enabled, title'),
      'title' => t('Title'),
      'name' => t('Name'),
      'storage' => t('Storage'),
    );
  }

  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$item->name] = empty($item->disabled) . $item->title;
        break;
      case 'title':
        $this->sorts[$item->name] = $item->title;
        break;
      case 'name':
        $this->sorts[$item->name] = $item->name;
        break;
      case 'storage':
        $this->sorts[$item->name] = $item->type . $item->admin_title;
        break;
    }

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$item->name] = array(
      'data' => array(
        array('data' => check_plain($item->name), 'class' => array('ctools-export-ui-name')),
        array('data' => check_plain($item->title), 'class' => array('ctools-export-ui-title')),
        array('data' => check_plain($item->type), 'class' => array('ctools-export-ui-type')),
        array('data' => $ops, 'class' => array('ctools-export-ui-operations')),
      ),
      'title' => check_plain($item->title),
      'class' => array(!empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled'),
    );
  }

  function list_table_header() {
    return array(
      array('data' => t('Name'), 'class' => array('ctools-export-ui-name')),
      array('data' => t('Title'), 'class' => array('ctools-export-ui-title')),
      array('data' => t('Storage'), 'class' => array('ctools-export-ui-type')),
      array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations')),
    );
  }

}
