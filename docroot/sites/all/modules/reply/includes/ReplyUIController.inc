<?php
/**
 * @file
 * Contains the ReplyBundleUIController class.
 */

/**
 * Custom controller for the administrator UI.
 */
class ReplyUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['title'] = 'Replies';
    $items[$this->path]['description'] = 'Manage replies';
    $items[$this->path]['access callback'] = 'user_access';
    $items[$this->path]['access arguments'] = array('administer replies');
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewForm().
   */
  public function overviewForm($form, &$form_state) {
    // Pages of local tasks don't use the Menu title
    // @see https://www.drupal.org/node/996302
    drupal_set_title(t('Replies'));

    $load_bundles = reply_load_bundles();
    $bundles = array();
    foreach ($load_bundles as $bundle) {
      $bundles[$bundle->bundle] = check_plain($bundle->name);
    }
    unset($load_bundles);

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Filter'),
      '#theme' => 'exposed_filters__reply',
    );

    $form['filter']['status'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('clearfix')),
    );
    $form['filter']['status']['filters'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('filters')),
    );
    $form['filter']['status']['filters']['status'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => array(
        'all' => t('any'),
        1 => t('published'),
        0 => t('not published'),
      ),
      '#default_value' => isset($_GET['status']) ? $_GET['status'] : NULL,
    );
    $form['filter']['status']['filters']['bundle'] = array(
      '#type' => 'select',
      '#title' => t('Bundle'),
      '#options' => array(
        'all' => t('any'),
      ),
      '#default_value' => isset($_GET['bundle']) ? $_GET['bundle'] : NULL,
    );
    foreach ($bundles as $bundle => $name) {
      $form['filter']['status']['filters']['bundle']['#options'][$bundle] = $name;
    }

    $form['filter']['status']['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array('class' => array('container-inline')),
    );
    $form['filter']['status']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Filter'),
    );
    $form['filter']['status']['actions']['reset']['#markup'] = l(t('Reset'), $_GET['q']);

    $form['replies'] = $this->overviewTable();
    $form['pager'] = array('#theme' => 'pager');

    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewFormSubmit().
   */
  public function overviewFormSubmit($form, &$form_state) {
    $args = array();
    if ($form_state['values']['status'] != 'all') {
      $args['status'] = $form_state['values']['status'];
    }
    if ($form_state['values']['bundle'] != 'all') {
      $args['bundle'] = $form_state['values']['bundle'];
    }
    if (!empty($form_state['values']['language']) && $form_state['values']['language'] != 'all') {
      $args['language'] = $form_state['values']['language'];
    }
    $form_state['redirect'] = array(
      'path' => $_GET['q'],
      'options' => array(
        'query' => $args,
      ),
    );
  }

  /**
   * Overrides EntityDefaultUIController::overviewTable().
   */
  public function overviewTable($conditions = array()) {
    $header = array(
      array('data' => t('ID'), 'field' => 'id', 'sort' => 'DESC'),
      array('data' => t('Author'), 'field' => 'r.uid'),
      array('data' => t('Status'), 'field' => 'r.status'),
      array('data' => t('Created'),'field' => 'r.created'),
      array('data' => t('Bundle'), 'field' => 'r.bundle'),
      array('data' => t('Language'), 'field' => 'r.language'),
      array('data' => t('Parent')),
      array('data' => t('Operations')),
    );

    $ids = db_select('reply', 'r')
      ->extend('PagerDefault')->limit(20)
      ->extend('TableSort')->orderByHeader($header)
      ->fields('r', array('id'))
      ->execute()
      ->fetchCol();

    $languages = language_list();
    $bundles = reply_load_bundles();
    $rows = array();
    $replies = reply_load_multiple($ids);

    foreach ($replies as $reply) {
      if ($reply->language == LANGUAGE_NONE || isset($languages[$reply->language])) {
        $language = $reply->language == LANGUAGE_NONE ? t('Language neutral') : $languages[$reply->language]->name;
      }
      else {
        $language = t('Undefined language (@langcode)', array('@langcode' => $reply->language));
      }

      $parent_entity = entity_load_single($reply->entity_type, $reply->entity_id);
      if ($parent_entity) {
        $parent_link = l(entity_label($reply->entity_type, $parent_entity), $reply->entity_type . '/' . $reply->entity_id);
      }
      else {
        $parent_link = t('Missing: @id', array('@id' => $reply->entity_type . '/' . $reply->entity_id));
      }

      $row = array();
      $user = user_load($reply->uid);
      $row[] = $reply->id;
      $row[] = theme('username', array('account' => $user));
      $row[] = ($reply->status == 1) ? t('published') : t('not published');
      $row[] = format_date($reply->created, 'short');
      $row[] = isset($bundles[$reply->bundle]) ? $bundles[$reply->bundle]->name : t('Missing bundle');
      $row[] = $language;
      $row[] = $parent_link;
      $operations = array(
        'view' => array('title' => t('View'), 'href' => 'reply/' . $reply->id . '/view'),
        'edit' => array('title' => t('Edit'), 'href' => 'reply/' . $reply->id . '/edit', 'query' => drupal_get_destination()),
        'delete' => array('title' => t('Delete'), 'href' => 'reply/' . $reply->id . '/delete', 'query' => drupal_get_destination()),
      );
      $row[] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
      $rows[] = $row;
    }

    $render = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No replies available.'),
    );

    return $render;
  }
}
