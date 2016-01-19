<?php
/**
 * @file
 * Magic form config list form.
 */

/**
 * The list form callback.
 */
function magic_forms_ui_list_form($form, &$form_state) {
  $form = array();
  $rows = array();

  $query = db_select('magic_forms', 'mf')->extend('PagerDefault');
  $configs = $query
    ->fields('mf', array('name', 'form_id', 'title', 'description', 'status'))
    ->limit(25)
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  foreach ($configs as $config) {
    $rows[] = array(
      $config['status'] == TRUE ? t('Enabled') : t('Disabled'),
      "{$config['title']} ({$config['name']})",
      $config['form_id'],
      $config['description'],
      implode(' | ', array(
        l(t('edit'), MAGIC_FORMS_UI_BASE_PATH . "/{$config['name']}/edit"),
        l(t('delete'), MAGIC_FORMS_UI_BASE_PATH . "/{$config['name']}/delete"),
        l(t('export'), MAGIC_FORMS_UI_BASE_PATH . "/{$config['name']}/export"),
      )),
    );
  }

  $form['forms'] = array(
    '#theme' => 'table',
    '#header' => array(
      array('data' => t('Status'), 'width' => 80),
      array('data' => t('Title'), 'width' => 150),
      array('data' => t('Form ID'), 'width' => 150),
      t('Description'),
      array('data' => t('Operations'), 'width' => 140),
    ),
    '#rows' => $rows,
    '#empty' => t(
      'There are no configs available, !add or !import one.',
      array(
        '!add' => l(t('add'), MAGIC_FORMS_UI_BASE_PATH . '/add'),
        '!import' => l(t('import'), MAGIC_FORMS_UI_BASE_PATH . '/import'),
      )
    ),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
  );

  return $form;
}
