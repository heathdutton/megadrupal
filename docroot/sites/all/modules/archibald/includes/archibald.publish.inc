<?php

/**
 * @file
 * all what have something to do with publish process
 */
module_load_include('inc', 'archibald', 'includes/archibald.pages');

/**
 * Publish processing page.
 *
 * @param string $lom_id
 *   The lom id
 */
function archibald_content_publish($lom_id) {
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');

  // let run cron
  if (!empty($_GET['run_cron'])) {
    module_load_include('php', 'archibald', 'includes/LomPublish.class');
    $archibald_lom_publish = new ArchibaldLomPublish();
    $archibald_lom_publish->cron();
    drupal_json_output(array('cron_run' => 'ok'));
    exit;
  }

  $status = archibald_get_lom_stats($lom_id);

  $lom = archibald_load($lom_id);
  $archibald_lom_validate = new ArchibaldLomValidate($lom);

  $is_central_configured = FALSE;
  $empty_cp = !empty($status->content_partner);
  $empty_key = !empty($status->content_partner['key']);
  $empty_user = !empty($status->content_partner['username']);
  if ($empty_cp && $empty_key && $empty_user) {
    $is_central_configured = TRUE;
  }

  $ready_to_publish = TRUE;
  if (!$archibald_lom_validate->readyForPublish()) {
    $status->lom_errors = $archibald_lom_validate->getErrors();
    foreach ($status->lom_errors AS $error) {
      drupal_set_message(filter_xss($error), 'error');
    }

    $ready_to_publish = FALSE;
  }

  $publish_type = variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE);

  $archibald_lom = new ArchibaldLom();
  $title = $archibald_lom->determinTitle($lom);
  drupal_set_title(t('Publish description "@title"', array('@title' => $title)));

  /* when action was done, the modal will closed and the current
   * or not current ;-p
   * @see archibald_content_publish_chooser_form_submit()
   * admin resource overview will reloaded
   */

  if (!empty($_REQUEST['admin_content_overview_type'])) {
    $admin_content_overview_type = filter_xss($_REQUEST['admin_content_overview_type']);
  }
  else {
    $admin_content_overview_type = 'to_publish';
  }

  // this can be called as standalone or with an ajax processed modal frame
  if (!empty($_REQUEST['ajax'])) {
    // Include the CTools tools that we need.
    ctools_include('ajax');
    ctools_include('modal');

    drupal_add_http_header('Content-Type', 'application/json');

    $form_state = array(
      'title' => t('Publish description "!title"', array('!title' => $title)),
      'ajax' => TRUE,
      'build_info' => array(
        'args' => array(
          $lom,
          $status,
          $publish_type,
          $is_central_configured,
          $ready_to_publish,
          $admin_content_overview_type,
        ),
      ),
    );
    $commands = ctools_modal_form_wrapper('archibald_content_publish_chooser_form', $form_state);

    if (!empty($form_state['ajax_redirect'])) {
      $commands = array(ctools_modal_command_dismiss());
      $commands[] = ctools_ajax_command_redirect($form_state['ajax_redirect']);
    }

    print ajax_render($commands);
    die();
  }
  else {
    $f = drupal_get_form(
      'archibald_content_publish_chooser_form', $lom, $status, $publish_type, $is_central_configured, $ready_to_publish, $admin_content_overview_type
    );
    return drupal_render($f);
  }
}

/**
 * show publish / unpublish buttons
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 *
 * @param string $status
 *  status array
 *  @see archibald_get_lom_stats()
 * @param integer $publish_type
 *   ARCHIBALD_PUBLISH_TOGETHER
 *   ARCHIBALD_PUBLISH_SEPERATE
 * @param boolean $is_central_configured
 *   is it posible to publish to central catalog
 * @param boolean $ready_to_publish
 *   is the lom ressource validated an ready to publish
 */
function archibald_content_publish_chooser_form($form, $form_state, $lom, $status, $publish_type, $is_central_configured, $ready_to_publish, $admin_content_overview_type) {

  $form['lom_id'] = array(
    '#type' => 'value',
    '#value' => $lom->getLomId(),
  );

  $form['ajax'] = array(
    '#type' => 'hidden',
    '#value' => (!empty($form_state['ajax'])) ? '1' : '',
  );

  $form['admin_content_overview_type'] = array(
    '#type' => 'value',
    '#default_value' => $admin_content_overview_type,
  );


  /*
    /// ERROR handling
    if ($ready_to_publish == FALSE) {
      drupal_set_message(t('The status cannot be changed because the description is incomplete.') . '<br />' . t('Please edit and try again.'), 'error');
      return $form;
    }

    if ($lom->getLifeCycle()->getStatus()->getValue() != ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE) {
      drupal_set_message(t('The status cannot be changed because the description is incomplete.') . '<br />' . t('Please change the status to "final"'), 'error');
      return $form;
    }
  */

  $publishing_possible = ($lom->getLifeCycle()->getStatus() == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE && $ready_to_publish);

  // Status section
  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('State of publication'),
    '#collapsible' => FALSE,
    '#weight' => -15,
  );

  $headers = array(
    t('Catalogue'),
    t('State'),
    '',
  );

  $rows = array();
  $data = array(t('Local'), '', '');

  switch ($status->publication_local_status) {
    case 'none':
      $data[1] = array(
        'data' => t('Not published'),
        'class' => 'warning',
      );
      break;

    case 'ok':
      $data[1] = array(
        'data' => t('Published'),
        'class' => 'ok',
      );
      break;

    case 'old':
      $data[1] = array(
        'data' => t('Old revision published'),
        'class' => 'warning',
      );
      break;

    default:
      $data[1] = array(
        'data' => 'Unkown status: "' . $status->publication_local_status . '"',
        'class' => 'warning',
      );
      break;
  }
  $rows[] = $data;


  if (archibald_can_publish_central()) {

    $data = array(t('National'), '', '');
    if ($is_central_configured == TRUE) {

      switch ($status->publication_central_status) {
        case 'none':
          $data[1] = array(
            'data' => t('Not published'),
            'class' => 'warning',
          );
          break;
        case 'ok':
          $data[1] = array(
            'data' => t('Published'),
            'class' => 'ok',
          );
          break;
        case 'old':
          $data[1] = array(
            'data' => t('Old revision published'),
            'class' => 'warning',
          );
          break;
        case 'waiting':
          $data[1] = array(
            'data' => t(
              'Publishing, please wait ....'
            ),
            'class' => 'ok',
          );
          break;
        case 'processing':
          $data[1] = array(
            'data' => t('Publication process: @percent',
              array(
                '@percent' => $status->publication_progress,
              )
            ),
            'class' => 'ok',
          );
          break;
        case 'error':
          $data[1] = array(
            'data' => t('An error occured while publishing'),
            'class' => 'warning',
          );
          break;
      }

      if (!empty($status->publication_version)) {
        $data[2] = l(t('Details'), 'archibald/' . $lom->getLomId() . '/publish/status/central');
      }
    }
    else {
      // there is a content partner who can publish central
      // but the selected can not
      $data[1] = array(
        'data' => t('The assigned content partner has no rights to publish to the national catalogue.'),
        'class' => 'warning',
      );
    }

    $rows[] = $data;
  }

  $form['status']['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#attributes' => array(
      'class' => array('archibald_publish_status'),
    ),
  );


  // buttons

  if ($publish_type == ARCHIBALD_PUBLISH_TOGETHER) {

    /// BOTH section  TOGETHER
    if ($is_central_configured == TRUE) {
      if (user_access('publish archibald content local') && user_access('publish archibald content central')) {

        $form['both'] = array(
          '#type' => 'fieldset',
          '#title' => t('Local and national'),
          '#collapsible' => FALSE,
          '#weight' => 10,
        );

        // this ressouce can and have to be published to national catalog

        if ($status->publication_central_status == 'none' || $status->publication_central_status == 'old') {
          if ($publishing_possible === FALSE) {
            $form['both']['info'] = array(
              '#type' => 'item',
              '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
            );
          } else {
            if ($status->publication_central_status == 'none') {
              $form['both']['publish_both'] = array(
                '#type' => 'submit',
                '#value' => t('Publish locally and nationally'),
              );
            }
            elseif ($status->publication_central_status == 'old') {
              $form['both']['publish_both'] = array(
                '#type' => 'submit',
                '#value' => t('Republish locally and nationally'),
              );
            }
          }
        } else {
          if ($status->publication_central_status == 'ok') {
            // unpublish locally only, central is not published
            $form['both']['unpublish_both'] = array(
              '#type' => 'submit',
              '#value' => t('Unpublish locally and nationally'),
            );
          }
          elseif ($status->publication_central_status == 'error') {
            $form['both']['reset_both'] = array(
              '#type' => 'submit',
              '#value' => t('Abort national publication process and unpublish locally'),
            );

            $form['both']['info'] = array(
              '#markup' => '<div>' . t('Publication into the national catalogue failed.') . '<br />' .
              t('Please check details for further information') . '</div>',
            );
          }
          elseif (in_array($status->publication_central_status, array('processing', 'waiting'))) {
            $form['both']['info'] = array(
              '#markup' => t('Publication to the national catalogue in progress.') . '<br />' .
              t('Please wait or check details for further information.'),
            );
          }
          else {
            drupal_set_message(
              t(
                'Error: unknown state of national publication "@status"',
                array(
                  '@status' => $status->publication_central_status,
                )
              ),
              'error'
            );
          }
        }
      }
      else {
        // !user_access('publish archibald content local') ||
        // !user_access('publish archibald content central')
        drupal_set_message(
          t('You don\'t have the permission to publish/unpublish descriptions locally and nationally.') . '<br />' .
          t('You need both permissions because Archibald is configured to publish always locally and nationally.'), 'error'
        );
      }
      /// end of BOTH section
    }
    else {
      /// LOCAL section   TOGETHER mode --->>> Local only <<<----
      //                  because there is no key for central
      $form['local'] = array(
        '#type' => 'fieldset',
        '#title' => t('Local'),
        '#collapsible' => FALSE,
        '#weight' => 10,
      );

      if (user_access('publish archibald content local')) {

        if ($status->publication_local_status == 'none') {
          if ($publishing_possible === FALSE) {
            $form['local']['info'] = array(
              '#type' => 'item',
              '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
            );
          } else {
            $form['local']['publish_local'] = array(
              '#type' => 'submit',
              '#value' => t('Publish locally'),
            );
          }
        }
        elseif ($status->publication_local_status == 'old') {
          if ($publishing_possible === FALSE) {
            $form['local']['info'] = array(
              '#type' => 'item',
              '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
            );
          } else {
            // republish / unpublish locally only, nation is not published
            $form['local']['publish_local'] = array(
              '#type' => 'submit',
              '#value' => t('Republish locally'),
            );
          }

          // Direct unpublish locally because there is no key for national cetalogoue configured.
          $form['local']['unpublish_local'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish locally'),
          );
        }
        elseif ($status->publication_local_status == 'ok') {
          // Direct unpublish locally because there is no key for national cetalogoue configured.
          $form['local']['unpublish_local'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish locally'),
          );
        }
        else {
          drupal_set_message(
            t(
              'Error: unknown state of local publication "@status"',
              array(
                '@status' => $status->publication_local_status,
              )
            ),
            'error'
          );
        }
      }
      else {
        // !user_access('publish archibald content local')
        drupal_set_message(t('You have no publication rights.'), 'error');
      }
      /// end of LOCAL section
    }
  }
  else {
    // $publish_type==ARCHIBALD_PUBLISH_SEPERATE
    /// LOCAL section SEPARATE
    $form['local'] = array(
      '#type' => 'fieldset',
      '#title' => t('Local'),
      '#collapsible' => FALSE,
      '#weight' => 10,
    );

    if (user_access('publish archibald content local')) {

      if ($status->publication_local_status == 'none') {
        if ($publishing_possible === FALSE) {
          $form['local']['info'] = array(
            '#type' => 'item',
            '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
          );
        } else {
          $form['local']['publish_local'] = array(
            '#type' => 'submit',
            '#value' => t('Publish'),
          );
        }
      }
      elseif ($status->publication_local_status == 'old') {

        if (in_array($status->publication_central_status, array('old', 'progress', 'waiting'))) {

          if ($publishing_possible === FALSE) {
            $form['local']['info'] = array(
              '#type' => 'item',
              '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
            );
          } else {
            // republish / unpublish locally and nationally.
            $form['local']['publish_both'] = array(
              '#type' => 'submit',
              '#value' => t('Republish locally and nationally'),
            );
          }

          // unpublish locally and nationally
          $form['local']['unpublish_both'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish locally and nationally'),
          );
        }
        else {

          if ($publishing_possible === FALSE) {
            $form['local']['info'] = array(
              '#type' => 'item',
              '#markup' => '<div>' . t('No republishing is possible (Description is incomplete)') . '</div>',
            );
          } else {
            // republish / unpublish locally only, nationally is not published
            $form['local']['publish_local'] = array(
              '#type' => 'submit',
              '#value' => t('Republish'),
            );
          }

          // unpublish locally only, nationally is not published
          $form['local']['unpublish_local'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish'),
          );
        }
      }
      elseif ($status->publication_local_status == 'ok') {

        if (in_array($status->publication_central_status, array('ok', 'old', 'progress', 'waiting'))) {

          // unpublish locally and nationally
          $form['local']['unpublish_both'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish locally and nationally'),
          );
        }
        else {
          // unpublish locally only, nationally is not published
          $form['local']['unpublish_local'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish'),
          );
        }
      }
      else {
        drupal_set_message(
          t(
            'Error: unknown local status "@status"',
            array(
              '@status' => $status->publication_local_status,
            )
          ),
          'error'
        );
      }
    }
    else {
      // !user_access('publish archibald content local')
      drupal_set_message(t('You have no publication rights.'), 'error');
    }
    /// end of LOCAL section
    /// CENTRAL section   SEPERATE
    if ($is_central_configured == TRUE) {

      $form['central'] = array(
        '#type' => 'fieldset',
        '#title' => t('National'),
        '#collapsible' => FALSE,
        '#weight' => 15,
      );

      if (user_access('publish archibald content central')) {

        if ($status->publication_central_status == 'none') {

          if ($status->publication_local_status == 'ok') {
            if( $publishing_possible == FALSE ) {
              $form['central']['info'] = array(
                '#type' => 'item',
                '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
              );
            } else {
              $form['central']['publish_central'] = array(
                '#type' => 'submit',
                '#value' => t('Publish'),
              );
            }
          }
          else {
            if( $publishing_possible == FALSE ) {
              $form['central']['info'] = array(
                '#type' => 'item',
                '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
              );
            } else {
              if (user_access('publish archibald content local')) {
                if ($status->publication_local_status == 'old') {
                  $form['central']['publish_both'] = array(
                    '#type' => 'submit',
                    '#value' => t('Republish locally and publish nationally'),
                  );
                }
                else {
                  $form['central']['publish_both'] = array(
                    '#type' => 'submit',
                    '#value' => t('Publish locally and nationally'),
                  );
                }
              }
              else {
                drupal_set_message(
                  t('The description needs to be published locally before it can be published nationally.'),
                  'error'
                );
              }
            }
          }
        }
        elseif ($status->publication_central_status == 'old') {

          if (user_access('publish archibald content local')) {
            if( $publishing_possible == FALSE ) {
              $form['central']['info'] = array(
                '#type' => 'item',
                '#markup' => '<div>' . t('No publishing is possible (Description is incomplete)') . '</div>',
              );
            } else {
              $form['central']['publish_both'] = array(
                '#type' => 'submit',
                '#value' => t('Republish locally and nationally'),
              );
            }
          }
          else {
            drupal_set_message(
              t('The description needs to be republished locally before it can be republished nationally.'),
              'warning'
            );
          }

          $form['central']['unpublish_central'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish'),
          );
        }
        elseif ($status->publication_central_status == 'ok') {
          $form['central']['unpublish_central'] = array(
            '#type' => 'submit',
            '#value' => t('Unpublish'),
          );
        }
        elseif ($status->publication_central_status == 'error') {
          $form['central']['reset_central'] = array(
            '#type' => 'submit',
            '#value' => t('Abort publication process'),
          );
          $form['central']['info'] = array(
            '#markup' => '<div>' . t('Publication into the national catalogue failed.') . '<br />' .
            t('Please check details for further information') . '</div>',
          );
        }
        elseif ($status->publication_central_status == 'processing' ||
          $status->publication_central_status == 'waiting'
        ) {
          $form['central']['info'] = array(
            '#markup' => t('Publication to the national catalogue in progress.') . '<br />' .
            t('Please wait or check details for further information.'),
          );
        }
        else {
          drupal_set_message(
            t(
              'Error: unknown state of national publication "@status"',
              array(
                '@status' => $status->publication_central_status,
              )
            ),
            'error'
          );
        }
      }
      elseif (!user_access('publish archibald content local')) {
        drupal_set_message(t('You don\'t have the permission to publish/unpublish descriptions to the national catalogue.'), 'error');
      }

      /// end of CENTRAL section
    }

    // end of $publish_type==ARCHIBALD_PUBLISH_SEPERATE
  }

  return $form;
}

/**
 * publish / unpublish button submit handler
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_publish_chooser_form_submit($form, &$form_state) {
  $lom_id       = $form_state['values']['lom_id'];
  $lom          = archibald_load($lom_id);
  $archibald_lom_validate = new ArchibaldLomValidate($lom);

  module_load_include('php', 'archibald', 'includes/LomPublish.class');
  $archibald_lom_publish = new ArchibaldLomPublish();

  $local = 'none';
  $central = 'none';

  // hopfully no one got the stupid idea to let get
  // several buttons the same title="***" attribute
  if (!isset($form_state['values']['publish_local'])) {
    $form_state['values']['publish_local'] = '';
  }
  if (!isset($form_state['values']['publish_central'])) {
    $form_state['values']['publish_central'] = '';
  }
  if (!isset($form_state['values']['publish_both'])) {
    $form_state['values']['publish_both'] = '';
  }

  if (!isset($form_state['values']['unpublish_local'])) {
    $form_state['values']['unpublish_local'] = '';
  }
  if (!isset($form_state['values']['unpublish_central'])) {
    $form_state['values']['unpublish_central'] = '';
  }
  if (!isset($form_state['values']['unpublish_both'])) {
    $form_state['values']['unpublish_both'] = '';
  }

  if (!isset($form_state['values']['reset_central'])) {
    $form_state['values']['reset_central'] = '';
  }
  if (!isset($form_state['values']['reset_both'])) {
    $form_state['values']['reset_both'] = '';
  }


  switch ($form_state['values']['op']) {
    case $form_state['values']['publish_local']:
      $local = 'publish';
      break;

    case $form_state['values']['publish_central']:
      $central = 'publish';
      break;

    case $form_state['values']['publish_both']:
      $local = 'publish';
      $central = 'publish';
      break;

    case $form_state['values']['unpublish_local']:
      $local = 'unpublish';
      break;

    case $form_state['values']['unpublish_central']:
      $central = 'unpublish';
      break;

    case $form_state['values']['unpublish_both']:
      $local = 'unpublish';
      $central = 'unpublish';
      break;

    case $form_state['values']['reset_central']:
      $central = 'reset';
      break;

    case $form_state['values']['reset_both']:
      $local = 'unpublish';
      $central = 'reset';
      break;
  }


  // validate again before action
  // better do it twice, instead of producing an error
  // @see archibald_content_publish_chooser_form()

  $publishing_possible = ($lom->getLifeCycle()->getStatus() == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE && $archibald_lom_validate->readyForPublish());

  if (!$publishing_possible && ($local == 'publish' || $central == 'publish')) {
    drupal_set_message(t('No publishing is possible (Description is incomplete)') . '<br />' . t('Please edit and try again.'), 'error');
    return FALSE;
  }


  // this should never be used, its only to prevent broken ajax
  $redirect = 'admin/archibald/' . $form_state['values']['admin_content_overview_type'];

  // do the real actions
  /// LOCAL
  if ($local == 'publish') {
    ArchibaldLomSaveHandler::setLocalPublished($lom->getLomId(), $lom->getVersion());

    if (!archibald_can_publish_central()) {
      $redirect = 'admin/archibald/published';
    }
    elseif (variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE) == ARCHIBALD_PUBLISH_SEPERATE) {
      $redirect = 'admin/archibald/published/local';
    }
    else {
      $redirect = 'admin/archibald/published';
    }
  }
  elseif ($local == 'unpublish') {

    ArchibaldLomSaveHandler::setLocalPublished($lom->getLomId(), '');
    if (variable_get('archibald_solr_search_activ', 0) == 1 && module_exists('apachesolr')) {
      // Delete from Solr Index
      $solr_search = new ArchibaldLomSolrSearch();
      $solr_search->deleteFromIndex($lom->getLomId());
    }
    if (!archibald_can_publish_central()) {
      $redirect = 'admin/archibald/to_publish';
    }
    elseif (variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE) == ARCHIBALD_PUBLISH_SEPERATE) {
      $redirect = 'admin/archibald/to_publish/local';
    }
    else {
      $redirect = 'admin/archibald/to_publish';
    }
  }

  /// CENTRAL
  if ($central == 'publish') {
    db_update('archibald_lom_stats')
      ->condition('lom_id', $lom->getLomId())
      ->fields(
        array(
          'publication_version' => $lom->getVersion(),
          'publication_progress' => NULL,
          'publication_log' => NULL,
          'republication_required' => 0,
        )
      )->execute();


    $redirect = 'archibald/' . $lom->getLomId() . '/publish/status/central';
  }
  elseif ($central == 'unpublish') {

    if ($archibald_lom_publish->lomApiUnpublish($lom) == TRUE) {
      db_update('archibald_lom_stats')
        ->condition('lom_id', $lom->getLomId())
        ->fields(
          array(
            'publication_version' => NULL,
            'republication_required' => 0,
          )
        )->execute();

      if (variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE) == ARCHIBALD_PUBLISH_SEPERATE) {
        $redirect = 'admin/archibald/to_publish/central';
      }
      else {
        $redirect = 'admin/archibald/to_publish';
      }
    }
    else {
      drupal_set_message(t('Fatal error while unpublishing from the national catalogue, please check the publication log.'), 'error');
      $redirect = 'archibald/' . $lom->getLomId() . '/publish/status/central';
    }
  }
  elseif ($central == 'reset') {
    db_update('archibald_lom_stats')->condition('lom_id', $lom->getLomId())->fields(array(
        'publication_version' => NULL,
      ))->execute();

    $redirect = 'admin/archibald/to_publish';
  }

  global $base_url, $user, $language;

  if (archibald_is_proposal($lom) && $central == 'publish') {
     archibald_send_mail($lom->getLomId(), 'proposal2publish');
  }

  $use_overlay = !isset($user->data['overlay']) || $user->data['overlay'];
  if (path_is_admin($redirect) && $use_overlay && module_exists('overlay')) {
    $enabled_interface_languages = variable_get("language_negotiation_language", array());
    $lang_redirect = "";
    $lang_front_redirect = "";
    if (isset($enabled_interface_languages['locale-url'])) {
      $lang_redirect = $language->language . '/';
      $lang_front_redirect = '/' . $language->language;
    }
    $redirect = urldecode(url($base_url) . $lang_front_redirect . '#overlay=' . $lang_redirect . $redirect);
  }

  if (!empty($form_state['ajax'])) {
    $form_state['ajax_redirect'] = $redirect;
  }
  else {
    $form_state['redirect'] = url($redirect);
  }
}

/**
 * publish statusd page for central
 *
 * @param string $lom_id
 */
function archibald_content_publish_central_status($lom_id) {

  // let run cron
  if (!empty($_GET['run_cron'])) {
    module_load_include('php', 'archibald', 'includes/LomPublish.class');
    $archibald_lom_publish = new ArchibaldLomPublish();
    $archibald_lom_publish->cron();
    drupal_json_output(array('cron_run' => 'ok'));
    exit;
  }

  $status = archibald_get_lom_stats($lom_id);
  $lom = archibald_load($lom_id, $status->publication_version);

  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');

  $is_central_configured = FALSE;
  if (!empty($status->content_partner) && $status->content_partner['key']) {
    $is_central_configured = TRUE;
  }


  if ($lom === NULL) {
    drupal_set_message(t('Could not load description'), 'error');
    return 'error';
  }

  if (empty($status->publication_version)) {
    drupal_set_message(t('This description is not published'), 'error');
    return 'error';
  }

  $archibald_lom = new ArchibaldLom();
  $title = $archibald_lom->determinTitle($lom);
  drupal_set_title(t('State of publication for "@title"', array('@title' => $title)));

  // So far, no ssl zeitifkat uploaded to authenticate on the archibald api
  if ($is_central_configured != TRUE) {
    drupal_set_message(
      t('Publication to the national catalouge is impossible, please check configuration.'),
      'error'
    );
  }

  // will execution it was a serious error
  // the full publication progress was stoped.
  // edit and save configuration to reset this status
  // or use  drush archibald_reset_upload_err ; drush cron
  // to reset erro and get further informations
  if (variable_get('archibald_upload_error', 0) != 0) {
    drupal_set_message(
      t('Publication process has been aborted due to fatal error, please check publication log.'),
      'error'
    );

    drupal_set_message(
      t('To reset the publication errors and restart the publication process
        use drush command "archibald_reset_upload_err" and  "drush cron --verbose"') . '<br />',
      'warning'
    );
  }

  if (empty($status->publication_log) || empty($status->publication_log->lom_status)) {
    // log was not inited, fake it
    $status->publication_log->lom_status = 'waiting';
  }


  return theme(
    'archibald_content_publish_central_status',
    array(
      'lom' => $lom,
      'status' => $status,
    )
  );
}


/**
 * theme the stauts page for resource central publish
 *
 * @param array $variables
 *  lom ArchibaldLom Object
 *  status Object
 *    lom_id string
 *    publication_version string
 *    publication_log Object
 *      lom_status string
 *        files array
 *        ....
 *      publication_progress Interger
 *
 * @return string html
 */
function theme_archibald_content_publish_central_status($variables) {
  $html = '';

  // reload page interval 2 sec
  $lom_id = $variables['lom']->getLomId();
  $is_ajax = !empty($_GET['ajax']);
  if (!$is_ajax && empty($_GET['no_ajax_reload'])) {

    if (!empty($_GET['no_cron'])) {
      // Include the CTools tools that we need.
      ctools_include('ajax');
      ctools_include('modal');
    }
    $html .= '<div id="to_publish_frame">';
  }

  switch ($variables['status']->publication_log->lom_status) {
    case 'waiting':
      $message_color = 'yellow';
      $message = t('Waiting for publication process');
      break;

    case 'success':
      $message_color = 'green';
      $message = t(
        'Description has been successfully published',
        array(
          '@percent' => $variables['status']->publication_progress,
        )
      );
      break;

    case 'processing files':
      $message_color = 'orange';
      $message = t(
        'Publication in process - uploading files',
        array(
          '@percent' => $variables['status']->publication_progress,
        )
      );
      break;

    case 'files not ready':
      $message_color = 'red';
      $message = t(
        'Publication process has been aborted due to error while uploading file, please check publication log.',
        array(
          '@percent' => $variables['status']->publication_progress,
        )
      );
      drupal_set_message($message, 'error');
      break;

    case 'error':
    default:
      $message_color = 'red';
      $message = t(
        'An error occured while publishing',
        array(
          '@percent' => $variables['status']->publication_progress,
        )
      );
      drupal_set_message($message, 'error');
      break;
  }

  $html .= '<div class="publish_status_message message_' . $message_color . '"><b>Status:</b> ' . $message . '</div>';

  if (!empty($variables['action'])) {
    $html .= '<span style="float:right">' . $variables['action'] . '</span>';
  }
  $html .= '<div class="clear"></div><br />';


  if ((INT) $variables['status']->publication_progress < 100) {
    $html .= theme(
      'progress_bar',
      array(
        'percent' => $variables['status']->publication_progress,
        'message' => $message,
      )
    );

    $html .= '<br />';
  }

  if (isset($variables['status']->publication_log->files) && !empty($variables['status']->publication_log->files)) {
    $table = array(
      'header' => array(
        t('File'),
        t('Size'),
        t('Progress'),
      ),
      'rows' => array(),
    );


    foreach ($variables['status']->publication_log->files as $file) {
      if (drupal_substr($file->url, 0, 6) == 'error:') {
        $error = trim(drupal_substr($file->url, 7));
      }
      else {
        $error = FALSE;
      }

      $table['rows'][] = array(
        $file->filename,
        format_size($file->filesize),
        (!empty($error)) ? $error : theme('progress_bar', array('percent' => $file->progress)),
      );
    }

    $html .= '<b>' . t('Uploading files in progress') . '</b><br />';
    $html .= theme('table', $table);
    $html .= '<br />';
  }

  $render = archibald_dblog_overview($lom_id, 'file_upload,lom_upload');
  $html .= drupal_render($render);

  $url = url('archibald/' . $lom_id . '/publish/status/central');

  if (empty($_GET['no_cron'])) {
    drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_publish_status_central.js');
    drupal_add_js("archibald_publish_urls['" . $lom_id . "'] = '" . $url . "';", "inline");
    drupal_add_js("archibald_publish_start('" . $lom_id . "', '1');", "inline");

  }
  else {
    $html .= '<script type="text/javascript">';
    $html .= "archibald_publish_urls['" . $lom_id . "'] = '" . $url . "';";
    $html .= "archibald_publish_start('" . $lom_id . "', '0');";
    $html .= '</script>';
  }

  if (!$is_ajax) {
    $html .= '</div>';
    if (!empty($_GET['no_cron'])) {
      $commands = array(ctools_modal_command_display(t('Detail status'), $html));
      drupal_add_http_header('Content-Type', 'application/json');
      print ajax_render($commands);
      exit;
    }
  }
  else {
    drupal_json_output(
    array(
        'content' => $html,
        'messages' => theme('status_messages'),
      )
    );
    die();
  }
  return $html;
}

/**
 * Publish status page for central.
 */
function archibald_content_publish_central_status_all() {

  // let run cron
  if (!empty($_GET['run_cron'])) {
    module_load_include('php', 'archibald', 'includes/LomPublish.class');
    $archibald_lom_publish = new ArchibaldLomPublish();
    $archibald_lom_publish->cron();
    drupal_json_output(array('cron_run' => 'ok'));
    exit;
  }

  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');

  // will execution it was a serious error
  // the full publication progress was stoped.
  // edit and save configuration to reset this status
  // or use  drush archibald_reset_upload_err ; drush cron
  // to reset erro and get further informations
  if (variable_get('archibald_upload_error', 0) != 0) {
    drupal_set_message(
      t('Publication process has been aborted due to fatal error, please check publication log.'),
      'error'
    );

    drupal_set_message(
      t('To reset the publication errors and restart the publication process
        use drush command "archibald_reset_upload_err" and  "drush cron --verbose"') . '<br />',
      'warning'
    );
    return '';
  }

  drupal_set_title(t('State of publication for all descriptions'));

  $resources_to_publish = array();

  $res = db_query("SELECT * FROM {archibald_lom_stats} WHERE (publication_progress < 100 OR publication_progress IS NULL) AND (publication_version IS NOT NULL OR publication_version <> '')");
  while ($status = $res->fetchObject()) {
    $status->content_partner = archibald_load_content_partner($status->content_partner_id);

    if (empty($status->content_partner) || empty($status->content_partner['key'])) {
      continue;
    }
    $lom = archibald_load($status->lom_id, $status->publication_version);
    if ($lom === NULL) {
      continue;
    }

    if (empty($status->publication_log) || empty($status->publication_log->lom_status)) {
      // Log was not inited, fake it.
      $status->publication_log = new stdClass();
      $status->publication_log->lom_status = 'waiting';
    }

    $resources_to_publish[] = array(
      'title' => ArchibaldLom::determinTitle($lom),
      'content_partner' => $status->content_partner,
      'lom' => $lom
    );
  }

  return theme('archibald_content_publish_central_status_all', array(
    'resources' => $resources_to_publish,
  ));
}

function archibald_content_publish_central_status_all_status() {
  $resources = filter_xss($_POST['resources']);

  $output = array();

  // will execution it was a serious error
  // the full publication progress was stoped.
  // edit and save configuration to reset this status
  // or use  drush archibald_reset_upload_err ; drush cron
  // to reset erro and get further informations
  if (variable_get('archibald_upload_error', 0) != 0) {
    $output['messages'][] = array(
      'type' => 'error',
      'message' => t('Publication process has been aborted due to fatal error, please check publication log.'),
    );
    $output['messages'][] = array(
      'type' => 'warning',
      'message' => t('To reset the publication errors and restart the publication process
        use drush command "archibald_reset_upload_err" and  "drush cron --verbose"') . '<br />',
    );

    $output['status'] = t("Canceled");
  }
  else {

    $main_progress = 0;
    foreach ($resources AS $resource) {

      $status = db_query("SELECT publication_progress FROM {archibald_lom_stats} WHERE lom_id = :lom_id", array(
        ':lom_id' => $resource,
      ))->fetchAssoc();

      if (empty($status['publication_progress'])) {
        $status['publication_progress'] = 0;
      }

      $output['status_' . $resource] = theme('progress_bar', array(
        'percent' => $status['publication_progress'],
      ));
      $main_progress += $status['publication_progress'];
    }
    $output['publish_status_main_progress'] = theme('progress_bar', array(
      'percent' => (INT) $main_progress/count($resources),
    ));

    if ($main_progress >= 100) {
      $output['messages'][] = array(
        'type' => 'success',
        'message' => t('Description has been successfully published') . '<br />',
      );
    }
  }
  drupal_json_output($output);
  die();
}

/**
 * Theme the status page for all resource central publish.
 *
 * @param array $variables
 *  resources
 *    an array with all resources which we want to publish
 *
 * @return string
 *   html
 */
function theme_archibald_content_publish_central_status_all($variables) {
  $html = "";

  ctools_include('ajax');
  ctools_include('modal');
  // Add CTools' javascript to the page.
  ctools_modal_add_js();
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_publish_status_central_all.js');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_publish_status_central.js');

  $table = array(
    'header' => array(
      t('Title of description'),
      t('Progress'),
    ),
    'rows' => array(),
  );

  $resources = array();
  foreach ($variables['resources'] AS $resource) {
    //$url = url('archibald/' . $resource['lom']->getLomId() . '/publish/status/central');
    $resources[] = $resource['lom']->getLomId();

    $table['rows'][] = array(
      l($resource['title'], 'archibald/' . $resource['lom']->getLomId() . '/publish/status/central', array(
        'query' => array('no_cron' => 1),
        'attributes' => array('id' => 'link_' . $resource['lom']->getLomId())
      )),
      array(
        'data' => theme('progress_bar', array('percent' => 0)),
        'id' => 'status_' . $resource['lom']->getLomId(),
      )
    );
  }

  $init_message = (empty($resources)) ? t('No descriptions to publish') : t('Waiting for publication process');
  $html .= '<div id="to_publish_frame_all">';
  $html .= '<div id="publish_status_messages"><div class="publish_status_message message_yellow"><b>Status:</b> ' . $init_message . '</div></div>';
  $html .= '<div class="clear"></div><br />';
  $html .= '<div id="publish_status_main_progress">' . theme('progress_bar', array(
    'percent' => 0,
    'message' => $init_message,
  ));

  $html .= '</div><br />';




  drupal_add_js(array('archibald_publish_all_resources' => $resources), "setting");
  drupal_add_js(array('archibald_publish_all_resources_urls' => array(
    'status_check' => url('archibald/publish/status/central/status'),
    'cron_check' => url('archibald/all/publish/status/central'),
  )), "setting");

  $html .= '<b>' . t('Publication progress') . '</b><br />';
  $html .= theme('table', $table);
  $html .= '<br />';
  $html .= '</div>';

  return $html;
}

/**
 * determin publishing status code search result row
 *
 * @param object $res_row
 *
 * @return string
 *   the icon code
 *   its consist of 3 parts
 *   1.) status
 *   2.) is local published
 *      yes   current revision si
 *      no    no not in any revision
 *      old   an old revision is
 *   3.) is central published
 *      yes   current revision si
 *      no    no not in any revision
 *      old   an old revision is
 *   or
 *    progress_error
 *   or
 *    processing
 */
function archibald_publish_status_icon_code($res_row) {
  switch ($res_row->status) {
    case 'draft':
      if (!empty($res_row->local_published)) {
        if (!empty($res_row->publication_version)) {
          // status:draft
          // old revision local published
          // old revision published
          return 'draft_old_old';
        }
        else {
          // status:draft
          // old revision local published
          // not remote published
          return 'draft_old_no';
        }
      }
      else {
        // status:draft
        // not local published
        // not remote published
        return 'draft_no_no';
      }
      break;

    case 'final':
      if (!empty($res_row->local_published)) {
        // status:final
        $icon_code = 'final_';

        if ($res_row->local_published == $res_row->version) {
          // yes local published
          $icon_code .= 'yes_';
        }
        else {
          // old revision local published
          $icon_code .= 'old_';
        }

        if (!empty($res_row->publication_version)) {

          if ($res_row->publication_version == $res_row->version) {

            if ($res_row->publication_progress < 100) {
              if (isset($res_row->publication_log->lom_status) && in_array($res_row->publication_log->lom_status, array('error', 'files not ready'))) {
                return 'progress_error';
              }
              else {
                return 'processing';
              }
            }
            else {
              // yes remote published
              $icon_code .= 'yes';
            }
          }
          else {
            // old revision remote published
            $icon_code .= 'old';
          }
        }
        else {
          // not remote published
          $icon_code .= 'no';
        }
        return $icon_code;
      }
      else {
        // stauts:final
        // not local published
        // not remote published
        return 'final_no_no';
      }
    case 'undefined':
    default:
      // stauts:proposal
      // not local published
      // not remote published
      return 'proposal_no_no';
  }
}

/**
 * get description by publishing status code
 *
 * @param string $icon_code
 *
 * @return string
 */
function archibald_publish_status_by_icon_code($icon_code) {
  switch ($icon_code) {
    case 'draft_no_no':
      return t('Draft: The description is assigned to an "editor". It has never been published.');

    case 'draft_old_no':
      return t('Draft: The description is assigned to an "editor". A previous revision of the description is published locally.');

    case 'draft_old_old':
      return t('Draft: The description is assigned to an "editor". A previous revision of the description is published nationally.');

    case 'final_no_no':
      return t('Final: The description has never been published.');

    case 'final_old_no':
      return t('Final: A previous revision of the description is published locally.');

    case 'final_old_old':
      return t('Final: A previous  revision of the description is published nationally.');

    case 'final_yes_no':
      return t('Final: The current revision of the description is published locally.');

    case 'final_yes_yes':
      return t('Final:  The current revision of the description is published nationally.');

    case 'processing':
      return t('Status is "final" and the publishing is in progress.');

    case 'progress_error':
      return t('Status is "final" and the publishing is in progress. But faulty.');

    case 'proposal_no_no':
    default:
      return t('Proposal: The resource has been proposed. It is not yet assigned to an "editor".');
  }
}

/**
 * theme the content publish status item
 *
 * @param array $variables
 *
 * @return string
 */
function theme_archibald_admin_content_publish_status($variables) {
  $icon_code = archibald_publish_status_icon_code($variables['res_row']);
  $code = archibald_publish_status_by_icon_code($icon_code);
  return '<span class="publish_status publish_status_' . $icon_code . '" title=\'' . $code . '\'>&nbsp;</span>';
}

