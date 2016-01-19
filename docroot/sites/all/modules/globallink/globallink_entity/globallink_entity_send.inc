<?php

$default_language = language_default();

$GLOBALS['globallink_entity_source_locale'] = $default_language->language;

/**
 * Builds forms on entity send dashboard.
 *
 * @param string $type
 *   The type of task we are working on.
 *
 * @return array
 *   Array of forms for the entity send dashboard.
 */
function globallink_entity_dashboard($type) {
  $_SESSION['globallink_entity_selected_type'] = $type;

  $array = array();
  $array[] = drupal_get_form('globallink_entity_dashboard_filter_form');
  $array[] = drupal_get_form('globallink_entity_dashboard_pager_form');
  $array[] = drupal_get_form('globallink_entity_dashboard_form');

  return $array;
}

/**
 * Builds form to filter entities to send for translation on dashboard.
 */
function globallink_entity_dashboard_filter_form($form, &$form_state) {
  module_load_include('inc', 'globallink', 'globallink');

  $form = array();

  $redirect_nid = isset($_GET['rnid']) ? $_GET['rnid'] : '';

  if (!empty($redirect_nid)) {
    $_SESSION['globallink_entity_source_locale'] = '';
    $_SESSION['globallink_entity_filter'] = array();
    $_SESSION['globallink_entity_source_locale'] = globallink_get_node_source_languages($redirect_nid);
  }

  $locales = globallink_get_mapped_drupal_locales(FALSE);
  $node_types = globallink_get_translatable_node_types(FALSE);

  if (isset($locales) && count($locales) > 1) {
    if (isset($node_types) && count($node_types) > 0) {
      $session = isset($_SESSION['globallink_entity_filter']) ? $_SESSION['globallink_entity_filter'] : array();
      $filters = globallink_build_filters(FALSE);

      $form['filters'] = array(
        '#type' => 'fieldset',
        '#title' => t('FILTER RECORDS'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $selected_arr = array();
      $selected_source_lang = '';

      foreach ($session as $filter) {
        list($type, $value) = $filter;
        $selected_arr[$type] = $value;
      }

      $form['filters']['status']['filters'] = array(
        '#type' => 'container',
        '#attributes' => array('style' => 'padding: 0px;'),
      );

      $module_path = drupal_get_path('module', 'globallink');
      drupal_add_css($module_path . '/css/globallink.css');

      foreach ($filters as $key => $filter) {
        $disabled = FALSE;
        $default = '[Any]';

        switch ($filter['form-type']) {
          case 'select':
            if ($key == 'language_name') {
              $default_language = language_default();
              $default = $default_language->language;

              if (isset($_SESSION['globallink_entity_source_locale']) && $_SESSION['globallink_entity_source_locale'] != '') {
                $default = $_SESSION['globallink_entity_source_locale'];
              }

              $selected_source_lang = $default;

              $form['filters']['status']['filters'][$key] = array(
                '#type' => $filter['form-type'],
                '#options' => $filter['options'],
                '#title' => check_plain($filter['title']),
                '#default_value' => $default,
                '#disabled' => $disabled,
                '#attributes' => array('style' => array('padding:5px;')),
                '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
                '#field_prefix' => '</td><td>',
                '#ajax' => array(
                  'event' => 'change',
                  'callback' => 'globallink_entity_dashboard_filter_target',
                  'wrapper' => 'replace-target',
                ),
              );

              $form['filters']['status']['filters']['node_parent'] = array(
                '#type' => 'checkbox',
                '#return_value' => 1,
                '#default_value' => 1,
                '#prefix' => '<span style="display:block;padding-top:10px;">',
                '#suffix' => '</span></td></tr>',
                '#field_suffix' => 'Show Source Language Only</td>',
              );
            }
            elseif ($key == 'target_language') {
              if (isset($form_state['values']['language_name'])) {
                $form['filters']['status']['filters'][$key] = array(
                  '#type' => $filter['form-type'],
                  '#options' => globallink_entity_get_target_options($form_state['values']['language_name']),
                  '#disabled' => $disabled,
                  '#default_value' => isset($form_state['values']['target_language']) ? $form_state['values']['target_language'] : '',
                  '#attributes' => array('style' => array('padding:0px;')),
                  '#field_prefix' => '<td><div id="replace-target">',
                  '#field_suffix' => '</div></td>',
                );
              }
              else {
                $form['filters']['status']['filters'][$key] = array(
                  '#type' => $filter['form-type'],
                  '#options' => globallink_entity_get_target_options($selected_source_lang),
                  '#title' => check_plain($filter['title']),
                  '#disabled' => $disabled,
                  '#default_value' => isset($form_state['values']['target_language']) ? $form_state['values']['target_language'] : '',
                  '#attributes' => array('style' => array('padding:0px;')),
                  '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
                  '#suffix' => '</tr>',
                  '#field_prefix' => '</td><td><div id="replace-target">',
                  '#field_suffix' => '</div></td>',
                );
              }
            }
            else {
              if ($key == 'status' && module_exists('revisioning')) {
                $default = 'status-1';
              }

              $form['filters']['status']['filters'][$key] = array(
                '#type' => $filter['form-type'],
                '#options' => $filter['options'],
                '#title' => check_plain($filter['title']),
                '#disabled' => $disabled,
                '#default_value' => isset($selected_arr[$key]) ? $selected_arr[$key] : $default,
                '#attributes' => array('style' => array('padding:0px;')),
                '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
                '#suffix' => '</tr>',
                '#field_suffix' => '</td>',
                '#field_prefix' => '</td><td>',
              );
            }

            break;
          case 'modified-after':
            drupal_add_library('system', 'ui.datepicker');
            drupal_add_js('jQuery(document).ready(function(){
            jQuery( ".pickadate" ).datepicker({
            dateFormat: "mm/dd/yy",
            maxDate: 0,
            autoSize: true,
            showOn: "button",
            buttonImage: "' . $GLOBALS['base_url'] . '/' . $module_path . '/css/icon-calendar.png",
            buttonImageOnly: true,
            buttonText: "Click to select date"
            });});', 'inline');

            $form['filters']['status']['filters'][$key] = array(
              '#type' => $filter['form-type'],
              '#title' => check_plain($filter['title']),
              '#disabled' => $disabled,
              '#attributes' => array('class' => array('pickadate'),
              'style' => array('padding:0px;')),
              '#default_value' => isset($selected_arr[$key]) ? format_date($selected_arr[$key], 'custom', 'm/d/Y') : '',
              '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
              '#suffix' => '</tr></table></div>',
              '#field_suffix' => '</td>',
              '#field_prefix' => '</td><td>',
            );

            break;
          case 'modified':
            $form['filters']['status']['filters'][$key] = array(
              '#type' => $filter['form-type'],
              '#title' => check_plain($filter['title']),
              '#options' => $filter['options'],
              '#disabled' => $disabled,
              '#default_value' => isset($selected_arr[$key]) ? $selected_arr[$key] : 0,
              '#attributes' => array('class' => array('container-inline'), 'style' => array('padding:0px;')),
              '#prefix' => '<div class="container-inline"><table><tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
              '#suffix' => '</tr>',
              '#field_suffix' => '</td>',
              '#field_prefix' => '</td><td>',
            );

            break;
          case 'title':
            $form['filters']['status']['filters'][$key] = array(
              '#type' => $filter['form-type'],
              '#title' => check_plain($filter['title']),
              '#disabled' => $disabled,
              '#default_value' => isset($selected_arr[$key]) ? $selected_arr[$key] : '',
              '#attributes' => array('size' => 30),
              '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
              '#suffix' => '</tr>',
              '#field_suffix' => '</td>',
              '#field_prefix' => '</td><td>',
            );

            break;
        }
      }

      $form['filters']['status']['actions'] = array(
        '#type' => 'actions',
        '#attributes' => array('class' => array('container-inline')),
      );

      $disable_submit = FALSE;

      if (isset($_SESSION['globallink_entity_filter'])) {
        if (count($_SESSION['globallink_entity_filter']) > 0) {
          $disable_submit = TRUE;
        }
      }

      $form['filters']['status']['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Filter'),
        '#disabled' => $disable_submit,
      );

      if (count($session)) {
        $form['filter-search'] = array(
          '#type' => 'fieldset',
          '#title' => t('FILTER CRITERIA'),
        );

        $form['filter-search']['filter-markup'] = array(
          '#type' => 'container',
        );

        $form['filter-search']['filter-markup'][] = array('#markup' => '<table>');

        $filters = globallink_build_filters(FALSE);

        foreach ($session as $filter) {
          list($type, $value) = $filter;
          $selected_arr[$type] = $value;
          $label = '';

          if ($type == 'node_parent') {
            continue;
          }

          if (isset($filters[$type]['options'])) {
            $label = $filters[$type]['options'][$value];
          }
          elseif ($filters[$type]['field'] == 'changed') {
            $label = format_date($value, 'custom', 'm/d/Y');
          }
          else {
            $label = $value;
          }

          $form['filter-search']['filter-markup'][] = array('#markup' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;"><b>' . $filters[$type]['title'] . '</b>:&nbsp;</td><td style="text-align: left;width: 85%;vertical-align: middle;border: none;">' . str_replace('&nbsp;', '', $label) . '</td></tr>');
        }

        $form['filter-search']['filter-markup'][] = array('#markup' => '</table>');

        $form['filter-search']['status']['actions'] = array(
          '#type' => 'actions',
          '#attributes' => array('class' => array('container-inline')),
        );

        $form['filter-search']['status']['actions']['reset'] = array(
          '#type' => 'submit',
          '#value' => t('Reset'),
        );
      }
    }
    else {
      $form['tpt_node_types']['no_type'] = array(
        '#type' => 'markup',
        '#markup' => t('<br/><b><i>No Content types enabled for translation.</i></b>'),
      );
    }
  }
  else {
    $form['locale_mapping']['no_mapping'] = array('#type' => 'markup', '#markup' => t('<br/><b><i>No GlobalLink locale mapping found.</i></b>'));
  }

  return $form;
}

/**
 * Gets target options for entity.
 *
 * @param string $selected
 *   The selected entity.
 *
 * @return string
 *   The target options.
 */
function globallink_entity_get_target_options($selected) {
  $lang_filter = array();
  $languages = globallink_get_mapped_drupal_locales(FALSE);

  foreach ($languages as $key => $lang) {
    $lang_filter[$key] = $lang;
  }

  if ($selected) {
    unset($lang_filter[$selected]);
  }

  $any = array('[Any]' => t('Any'));

  return $any + $lang_filter;
}

/**
 * Gets the target language as a filter item.
 *
 * @return string
 *   The target language.
 */
function globallink_entity_dashboard_filter_target($form, &$form_state) {
  return $form['filters']['status']['filters']['target_language'];
}

/**
 * Handles submission of filter form.
 */
function globallink_entity_dashboard_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  $filters = globallink_build_filters(FALSE);

  switch ($op) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        switch ($filter) {
          case 'modified-after':
            if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '') {
              list($month, $day, $year) = explode('/', $form_state['values'][$filter]);
              $time_stamp = mktime(0, 0, 0, $month, $day, $year);
              $_SESSION['globallink_entity_filter'][$filter] = array($filter, $time_stamp);
            }

            break;
          case 'modified':
            if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '') {
              $_SESSION['globallink_entity_filter'][$filter] = array($filter, $form_state['values'][$filter]);
            }

            break;
          case 'title':
            if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '') {
              $_SESSION['globallink_entity_filter'][$filter] = array($filter, $form_state['values'][$filter]);
            }

            break;
          case 'node_parent':
            if (isset($form_state['values']['node_parent']) && $form_state['values']['node_parent'] != '') {
              $_SESSION['globallink_entity_filter']['node_parent'] = array('node_parent', $form_state['values']['node_parent']);
            }

            break;
          default:
            if ($filter == 'language_name') {
              $_SESSION['globallink_entity_source_locale'] = $form_state['values'][$filter];
            }

            if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
              // Merge an array of arrays into one if necessary.
              $options = ($filter == 'type') ? form_options_flatten($filters[$filter]['options']) : $filters[$filter]['options'];

              // Only accept valid selections offered on the dropdown, block bad input.
              if (isset($options[$form_state['values'][$filter]])) {
                if ($filter == 'status') {
                  $_SESSION['globallink_entity_filter'][$filter] = array($filter, $form_state['values'][$filter]);
                }
                else {
                  if (!isset($_SESSION['globallink_entity_filter'][$filter])) {
                    $_SESSION['globallink_entity_filter'][$filter] = array($filter, $form_state['values'][$filter]);
                  }
                }
              }
            }
        }
      }

      break;
    case t('Undo'):
      array_pop($_SESSION['globallink_entity_filter']);

      break;
    case t('Reset'):
      $_SESSION['globallink_entity_source_locale'] = '';
      $_SESSION['globallink_entity_filter'] = array();

      break;
    case t('Update'):
      return;
  }

  $form_state['redirect'] = 'admin/globallink-translations/dashboard/entity';

  return;
}

/**
 * Builds form to add pagination to entity send dashboard.
 */
function globallink_entity_dashboard_pager_form() {
  $form = array();
  $locales = globallink_get_mapped_drupal_locales(FALSE);

  if (isset($locales) && count($locales) > 1) {
    $form['page_counter']['markup'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('tpt-center')),
    );

    $form['page_counter']['markup']['p_count'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#default_value' => isset($_SESSION['globallink_entity_send_page_count']) ? $_SESSION['globallink_entity_send_page_count'][0] : TPT_PAGER_LIMIT,
      '#field_prefix' => t('Show') . '&nbsp;&nbsp;',
      '#field_suffix' => '&nbsp;&nbsp;' . t('records') . '&nbsp;&nbsp;&nbsp;&nbsp;',
      '#prefix' => '<div class="container-inline">',
    );

    $form['page_counter']['markup']['action'] = array(
      '#type' => 'submit',
      '#value' => t('Go'),
      '#suffix' => '</div>',
      '#limit_validation_errors' => array(),
      '#submit' => array('globallink_entity_dashboard_pager_form_submit'),
      '#access' => TRUE,
    );
  }

  return $form;
}

/**
 * Handles submission of pager form.
 */
function globallink_entity_dashboard_pager_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op != t('Go')) {
    return;
  }

  $page_count = $form_state['input']['p_count'];

  if (!is_numeric($page_count) || $page_count < 1) {
    form_set_error('', t('Invalid Page Count.'));
  }
  else {
    $_SESSION['globallink_entity_send_page_count'] = array($page_count);

    return;
  }
}

/**
 * Builds form to create an entity submission.
 */
function globallink_entity_dashboard_form() {
  module_load_include('inc', 'globallink_entity', 'globallink_entity');

  $default_checked_ids = array();
  $module_path = drupal_get_path('module', 'globallink');

  $form = array();
  $locales = globallink_get_mapped_drupal_locales(FALSE);
  $node_types = globallink_get_translatable_node_types(FALSE);

  if (isset($locales) && count($locales) > 1) {
    if (isset($node_types) && count($node_types) > 0) {
      $redirect_nid = isset($_GET['rnid']) ? $_GET['rnid'] : '';

      if ($redirect_nid != '') {
        $_SESSION['globallink_entity_filter'] = array();
      }

      $sort_order = isset($_GET['order']) ? $_GET['order'] : '';

      $header = array(
        'title' => array(
          'field' => 'title',
          'data' => t('Content'),
        ),
        'type' => array(
          'field' => 't_name',
          'data' => t('Content Type'),
        ),
        'language' => array(
          'field' => 'name',
          'data' => t('Language'),
        ),
        'changed' => array(
          'field' => 'changed',
          'data' => t('Last Modified'),
        ),
        'active' => t('In Active Submission'),
      );

      $node_check = variable_get('globallink_implementation_type', 0);
      $source_lang_arr = globallink_get_mapped_locales_with_drupal_desc(FALSE);
      $target_lang_arr = globallink_get_mapped_locales_with_drupal_desc(FALSE);
      $include_node_arr = array();

      $query = db_select('node', 'n');

      if (empty($_SESSION['globallink_entity_filter'])) {
        $query->condition('language', $GLOBALS['globallink_entity_source_locale'], '=');
      }
      else {
        $filter_arr = isset($_SESSION['globallink_entity_filter']) ? $_SESSION['globallink_entity_filter'] : array();
        transpefect_build_filter_query($query, $filter_arr);
      }

      $query->condition('type', $node_types, 'IN');
      $query->fields('n');
      $results = $query->execute();

      foreach ($results as $item) {
        array_push($include_node_arr, $item->nid);
      }

      $modified = 0;

      if (isset($_SESSION['globallink_entity_filter']['modified'])) {
        $modified = $_SESSION['globallink_entity_filter']['modified'][1];
      }

      $tgt_lang = '';

      if (isset($_SESSION['globallink_entity_filter']['target_language'])) {
        $tgt_lang = $_SESSION['globallink_entity_filter']['target_language'][1];
      }

      $status = '';

      if (module_exists('revisioning')) {
        if (isset($_SESSION['globallink_entity_filter']['status'])) {
          list($key, $value) = explode('-', $_SESSION['globallink_entity_filter']['status'][1], 2);
          $status = $value;
        }
        else {
          $status = '1';
        }
      }

      $source_lang = empty($_SESSION['globallink_entity_source_locale']) ? $GLOBALS['globallink_entity_source_locale'] : $_SESSION['globallink_entity_source_locale'];
      $tpt_locale_code = globallink_get_locale_code($source_lang);
      $drupal_locale_desc = globallink_get_drupal_locale_name($tpt_locale_code);

      $t_count = 0;

      foreach ($target_lang_arr as $key => $value) {
        if ($key == $tpt_locale_code) {
          unset($target_lang_arr[$tpt_locale_code]);

          continue;
        }

        $t_count++;

        if ($t_count % 2 === 0) {
          $target_lang_arr[$key] = '&nbsp;&nbsp;' . $value . '<BR/>';
        }
        else {
          $target_lang_arr[$key] = '&nbsp;&nbsp;' . $value . '&nbsp;&nbsp;&nbsp;&nbsp;';
        }
      }

      if ($tgt_lang != '' && $tgt_lang != '[Any]') {
        $target_lang_arr_keys = array_keys($target_lang_arr);

        foreach ($target_lang_arr_keys as $t_key) {
          if (globallink_get_locale_code($tgt_lang) != $t_key) {
            unset($target_lang_arr[$t_key]);
          }
        }
      }

      if ($tgt_lang != '' && $tgt_lang != '[Any]') {
        $t_query = db_select('globallink_core_entity', 'tc');
        $t_query->condition('target', globallink_get_locale_code($tgt_lang));
        $t_query->fields('tc');
        $t_query->orderBy('nid', 'ASC');

        $results = $t_query->execute();

        foreach ($results as $item) {
          if ($item->status == 'Sent for Translations' || $item->status == 'Error') {
            $key = array_search($item->nid, $include_node_arr);
            unset($include_node_arr[$key]);
          }
        }
      }

      if ($modified == 0) { // Show modified records only
        $t_query = db_select('globallink_core_entity', 'tc');
        $t_query->condition('source', $tpt_locale_code);
        $t_query->fields('tc');
        $t_query->orderBy('nid', 'ASC');
        $results = $t_query->execute();

        $item_arr = array();

        foreach ($results as $item) {
          $key = array_search($item->nid, $include_node_arr);

          if ($key !== FALSE) {
            if (array_key_exists($item->nid, $item_arr)) {
              array_push($item_arr[$item->nid], $item);
            }
            else {
              $item_arr[$item->nid] = array($item);
            }
          }
        }

        $tgt_keys = array_keys($target_lang_arr);

        foreach ($include_node_arr as $key => $nid) {
          if (isset($item_arr[$nid])) {
            $ncount = count($tgt_keys);

            foreach ($item_arr[$nid] as $item) {
              foreach ($tgt_keys as $target) {
                if ($target == $item->target) {
                  if ($item->changed == 0 || $item->changed == 2) {
                    $ncount--;
                  }
                }
              }
            }

            if ($ncount == 0) {
              unset($include_node_arr[$key]);
            }
          }
        }
      }

      $page_count = TPT_PAGER_LIMIT;

      if (isset($_SESSION['globallink_entity_send_page_count'])) {
        $page_count = $_SESSION['globallink_entity_send_page_count'][0];
      }

      $count = 0;
      $rows = array();
      $d_query = db_select('node', 'n')
      ->extend('PagerDefault')
      ->limit($page_count)
      ->extend('TableSort');

      if ($node_check == 1) { // Hook implementation for dashboard
        if ($redirect_nid == '' && count($include_node_arr) > 0) {
          $selected_target_lang = NULL;

          if ($tgt_lang != '' && $tgt_lang != '[Any]') {
            $selected_target_lang = $tgt_lang;
          }

          foreach ($include_node_arr as $key => $nid) {
            if (!globallink_is_node_translatable(node_load($nid), $selected_target_lang)) {
              unset($include_node_arr[$key]);
            }
          }

          if (count($include_node_arr) > 0) {
            $d_query->condition('nid', $include_node_arr, 'IN');
            $d_query->join('node_type', 't', 't.type = n.type');
            $d_query->fields('n', array('nid', 'vid', 'title', 'type', 'changed'));
            $d_query->fields('t', array('name'));

            if ($sort_order == '') {
              $d_query->orderBy('nid', 'DESC');
            }
            else {
              $d_query->orderByHeader($header);
            }

            $results = $d_query->execute();
            $active_subs = globallink_entity_get_active_submission_rows_by_nid($include_node_arr);

            $rows = array();

            foreach ($results as $item) {
              $count++;
              $active = '';
              $node = node_load($item->nid);

              if ($active_subs && array_key_exists($item->nid, $active_subs)) {
                $sub_arr = $active_subs[$item->nid];
                $sub_name = '';
                $tgt_arr = array();

                foreach ($sub_arr as $sub_name => $tgt_arr) {
                  $l_title = globallink_format_display_string($sub_name);
                  $l_href = 'admin/globallink-translations/activeSubmissions/entity';
                  $l_options = array('query' => array('submission' => urlencode($sub_name)));
                  $active .= l($l_title, $l_href, $l_options) . '&nbsp;&nbsp;- &nbsp;&nbsp;';
                  $t_count = 0;

                  foreach ($tgt_arr as $tgt) {
                    $t_count++;
                    $tgt_name = $locales[globallink_get_drupal_locale_code($tgt)];

                    if ($t_count == count($tgt_arr)) {
                      $active .= $tgt_name . ' <BR/> ';
                    }
                    else {
                      $active .= $tgt_name . ', &nbsp;&nbsp;';
                    }
                  }
                }
              }

              if (module_exists('title') && isset($node->title_field[$source_lang])) {
                $title = l(globallink_format_display_string($node->title_field[$source_lang][0]['value']), 'node/' . $item->nid);
              }
              else {
                $title = l(globallink_format_display_string($node->title), 'node/' . $item->nid);
              }

              $rows[$item->nid . '-' . $item->vid] = array(
                'nid' => $item->nid,
                'title' => $title,
                'type' => $item->name,
                'language' => $drupal_locale_desc,
                'changed' => format_date($item->changed, 'custom', 'Y-m-d H:i:s'),
                'active' => $active,
              );
            }
          }
        }
      }
      else { // Regular implementation
        // Request coming from dashboard
        $revisioning = FALSE;

        if (module_exists('revisioning') && $status != '') {
          if ($status == '1') {
            $d_query->condition('status', 1, '=');
          }
          else {
            $revisioning = TRUE;
          }
        }

        if ($redirect_nid == '' && count($include_node_arr) > 0) {
          $d_query->condition('nid', $include_node_arr, 'IN');
          $d_query->join('node_type', 't', 't.type = n.type');
          $d_query->fields('n', array('nid', 'vid', 'title', 'type', 'changed'));
          $d_query->fields('t', array('name'));

          if ($sort_order == '') {
            $d_query->orderBy('nid', 'DESC');
          }
          else {
            $d_query->orderByHeader($header);
          }

          $results = $d_query->execute();

          $rows = array();

          foreach ($results as $item) {
            $count++;
            $active_arr = globallink_entity_get_active_submission_by_nid($item->nid);

            $vid = $item->vid;

            if ($revisioning && $status == 0) {
              $vid = revisioning_get_latest_revision_id($item->nid);
            }

            $node = node_load($item->nid, $vid);
            $active = '';

            if ($active_arr) {
              foreach ($active_arr as $sub_name => $tgt_arr) {
                $l_title = globallink_format_display_string($sub_name);
                $l_href = 'admin/globallink-translations/activeSubmissions/entity';
                $l_options = array('query' => array('submission' => urlencode($sub_name)));
                $active .= l($l_title, $l_href, $l_options) . '&nbsp;&nbsp;- &nbsp;&nbsp;';

                if (count($tgt_arr) > 1) {
                  $t_count = 1;

                  foreach ($tgt_arr as $tgt => $_vid) {
                    $tgt_name = $locales[globallink_get_drupal_locale_code($tgt)];

                    if ($t_count == count($tgt_arr)) {
                      if ($_vid != $vid) {
                        $active .= $tgt_name . ' (Different version)<BR/> ';
                      }
                      else {
                        $active .= $tgt_name . ' <BR/> ';
                      }
                    }
                    else {
                      if ($_vid != $vid) {
                        $active .= $tgt_name . ' (Different version), &nbsp;&nbsp;';
                      }
                      else {
                        $active .= $tgt_name . ', &nbsp;&nbsp;';
                      }
                    }

                    $t_count++;
                  }
                }
                else {
                  foreach ($tgt_arr as $tgt => $_vid) {
                    $tgt_name = $locales[globallink_get_drupal_locale_code($tgt)];

                    if ($_vid != $vid) {
                      $active .= $tgt_name . ' (Different version) <BR/> ';
                    }
                    else {
                      $active .= $tgt_name . ' <BR/> ';
                    }
                  }
                }
              }
            }

            if (module_exists('title') && isset($node->title_field[$source_lang])) {
              $node_title = globallink_format_display_string($node->title_field[$source_lang][0]['value']);
            }
            else {
              $node_title = globallink_format_display_string($node->title);
            }

            if ($status) {
              $title = l($node_title, 'node/' . $item->nid . '/revisions/' . $node->vid . '/view');
            }
            else {
              $title = l($node_title, 'node/' . $item->nid);
            }

            $rows[$item->nid . '-' . $vid] = array(
              'nid' => $item->nid,
              'title' => $title,
              'type' => 'TEST',
              'type' => $item->name,
              'language' => $drupal_locale_desc,
              'changed' => format_date($item->changed, 'custom', 'Y-m-d H:i:s'),
              'active' => $active,
            );
          }
        }
        else { // Request coming from translate tab
          $d_query->condition('nid', $redirect_nid, '=');
          $d_query->join('node_type', 't', 't.type = n.type');
          $d_query->fields('n', array('nid', 'vid', 'title', 'type', 'changed'));
          $d_query->fields('t', array('name'));

          if ($sort_order == '') {
            $d_query->orderBy('title', 'ASC');
          }
          else {
            $d_query->orderByHeader($header);
          }

          $results = $d_query->execute();
          $include_node_arr = array();
          $include_node_arr[] = $redirect_nid;

          $rows = array();

          foreach ($results as $item) {
            $count++;

            $active_arr = globallink_entity_get_active_submission_by_nid($item->nid);

            $vid = $item->vid;

            if ($revisioning) {
              $vid = revisioning_get_latest_revision_id($item->nid);
            }

            $node = node_load($item->nid, $vid);

            $active = '';

            if ($active_arr) {
              foreach ($active_arr as $sub_name => $tgt_arr) {
                $l_title = globallink_format_display_string($sub_name);
                $l_href = 'admin/globallink-translations/activeSubmissions/entity';
                $l_options = array('query' => array('submission' => urlencode($sub_name)));
                $active .= l($l_title, $l_href, $l_options) . '&nbsp;&nbsp;- &nbsp;&nbsp;';

                if (count($tgt_arr) > 1) {
                  $t_count = 1;

                  foreach ($tgt_arr as $tgt => $_vid) {
                    $tgt_name = $locales[globallink_get_drupal_locale_code($tgt)];

                    if ($t_count == count($tgt_arr)) {
                      if ($_vid != $vid) {
                        $active .= $tgt_name . ' (Different version)<BR/> ';
                      }
                      else {
                        $active .= $tgt_name . ' <BR/> ';
                      }
                    }
                    else {
                      if ($_vid != $vid) {
                        $active .= $tgt_name . ' (Different version), &nbsp;&nbsp;';
                      }
                      else {
                        $active .= $tgt_name . ', &nbsp;&nbsp;';
                      }
                    }

                    $t_count++;
                  }
                }
                else {
                  foreach ($tgt_arr as $tgt => $_vid) {
                    $tgt_name = $locales[globallink_get_drupal_locale_code($tgt)];

                    if ($_vid != $vid) {
                      $active .= $tgt_name . ' (Different version) <BR/> ';
                    }
                    else {
                      $active .= $tgt_name . ' <BR/> ';
                    }
                  }
                }
              }
            }

            if (module_exists('title') && isset($node->title_field[$source_lang])) {
              $node_title = l(globallink_format_display_string($node->title_field[$source_lang][0]['value']), 'node/' . $item->nid);
            }
            else {
              $node_title = l(globallink_format_display_string($node->title), 'node/' . $item->nid);
            }

            $rows[$item->nid . '-' . $vid] = array(
              'nid' => $item->nid,
              'title' => $node_title,
              'type' => $item->name,
              'language' => $drupal_locale_desc,
              'changed' => format_date($item->changed, 'custom', 'Y-m-d H:i:s'),
              'active' => $active,
            );

            $default_checked_ids = array($item->nid . '-' . $vid => TRUE);
          }
        }
      }

      $form['#attributes']['class'][] = 'globallink-entity-form-clear';

      $form['table'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#empty' => t('No items available'),
        '#default_value' => $default_checked_ids,
      );

      if ($count > 0) {
        $form['pager'] = array('#markup' => theme('pager'));
      }

      if (user_access(TPT_ROLE_DO_TRANSLATIONS) || user_access(TPT_ROLE_MANAGE_TRANSLATIONS)) {
        if ($count > 0) {
          if ($modified == 0 && $redirect_nid == '') {
            $form['clear_changed_flag'] = array(
              '#type' => 'submit',
              '#value' => t('Clear "Changed" Status'),
              '#limit_validation_errors' => array(),
              '#attributes' => array(
                'class' => array('globallink-entity-form-clear-submit'),
              ),
              '#submit' => array('globallink_entity_form_clear_submit'),
              '#access' => TRUE,
            );
          }

          drupal_add_css($module_path . '/css/globallink.css');
          $proj_arr = variable_get('globallink_pd_projects', array());

          if (count($proj_arr) > 0) {
            $projects = variable_get('globallink_pd_projectid');

            if (!empty($projects)) {
              $arr = explode(',', $projects);

              foreach ($arr as $value) {
                if (isset($proj_arr[$value])) {
                  $proj_arr[$value] = $proj_arr[$value];
                }
                else {
                  $proj_arr[$value] = $value;
                }
              }
            }
          }

          $p_key = '';

          if (count($proj_arr) == 1) {
            $p_key = key($proj_arr);
          }

          $proj_arr[''] = ' -- Select a Project -- ';
          $r_proj_arr = array_reverse($proj_arr);

          $form['send_submission'] = array(
            '#type' => 'fieldset',
            '#title' => t('Create Submission'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
          );

          $form['send_submission']['submission_name'] = array(
            '#type' => 'textfield', '#title' => 'Submission Name',
            '#size' => 40, '#required' => TRUE,
            '#default_value' => variable_get('globallink_pd_submissionprefix', 'DRU_') . REQUEST_TIME,
            '#prefix' => '<div class="container-inline"><table><tr><td style="text-align: left;width: 20%;vertical-align: middle;border: none;">',
            '#suffix' => '</tr>',
            '#field_suffix' => '</td>',
            '#field_prefix' => '<BR/><div class="description">' . t('Name for this new Submission.') . '</div></td><td>',
          );

          $default_date = format_date(REQUEST_TIME + (7 * 24 * 60 * 60), 'custom', 'm/d/Y');

          drupal_add_library('system', 'ui.datepicker');
          drupal_add_js('jQuery(document).ready(function(){
          jQuery( ".pickadate1" ).datepicker({
          dateFormat: "mm/dd/yy",
          defaultDate: "' . $default_date . '",
          minDate: 0,
          autoSize: true,
          showOn: "button",
          buttonImage: "' . $GLOBALS['base_url'] . '/' . $module_path . '/css/icon-calendar.png",
          buttonImageOnly: true,
          buttonText: "Click to select date"
          });});', 'inline');

          $form['send_submission']['submission_due_date'] = array(
            '#type' => 'textfield',
            '#title' => 'Submission Due Date',
            '#required' => TRUE,
            '#attributes' => array(
              'class' => array('pickadate1'),
              'style' => array('padding:0px;'),
            ),
            '#default_value' => $default_date,
            '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
            '#suffix' => '</tr>',
            '#field_suffix' => '</td>',
            '#field_prefix' => '<BR/><div class="description">' . t('Expected Completion Date for this Submission.') . '</div></td><td>',
          );

          $form['send_submission']['submission_pd_project'] = array(
            '#type' => 'select',
            '#title' => t('Project Name'),
            '#required' => TRUE,
            '#options' => $r_proj_arr,
            '#default_value' => $p_key,
            '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
            '#suffix' => '</tr>',
            '#field_suffix' => '</td>',
            '#field_prefix' => '<BR/><div class="description">' . t('GlobalLink Project Name for this Submission') . '</div></td><td>',
          );

          $form['send_submission']['submission_source_locale'] = array(
            '#type' => 'select',
            '#title' => t('Source Language'),
            '#required' => TRUE,
            '#options' => $source_lang_arr,
            '#default_value' => globallink_get_locale_code(empty($_SESSION['globallink_entity_source_locale']) ? $GLOBALS['globallink_entity_source_locale'] : $_SESSION['globallink_entity_source_locale']),
            '#disabled' => TRUE,
            '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
            '#suffix' => '</tr>',
            '#field_suffix' => '</td>',
            '#field_prefix' => '<BR/><div class="description">' . t('Source language for this Submission.') . '</div></td><td>',
          );

          $form['send_submission']['submission_target_locale'] = array(
            '#type' => 'checkboxes',
            '#options' => $target_lang_arr,
            '#title' => t('Target Language(s)'),
            '#required' => TRUE,
            '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
            '#suffix' => '</tr>',
            '#field_suffix' => '</td>',
            '#field_prefix' => '<BR/><div class="description">' . t('Target language(s) for this Submission.') . '</div></td><td>',
          );

          global $user;

          $form['send_submission']['submission_submitter'] = array(
            '#type' => 'textfield',
            '#title' => 'Submitter',
            '#size' => 40,
            '#required' => TRUE,
            '#default_value' => check_plain($user->name),
            '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
            '#suffix' => '</tr>',
            '#field_suffix' => '</td>',
            '#field_prefix' => '<BR/><div class="description">' . t('Username for this new Submission.') . '</div></td><td>',
          );

          $form['send_submission']['submission_instructions'] = array(
            '#type' => 'textarea',
            '#title' => t('Instructions'),
            '#attributes' => array('style' => 'width: 275px;resize: none;'),
            '#rows' => 3,
            '#resizable' => FALSE,
            '#prefix' => '<tr><td style="text-align: left;width: 15%;vertical-align: middle;border: none;">',
            '#suffix' => '</tr></table></div>',
            '#field_suffix' => '</div></td>',
            '#field_prefix' => '<BR/><div class="description">' . t('Other instructions for this Submission.') . '</div></td><td>',
          );

          $form['send_submission']['submit_pd_send'] = array(
            '#type' => 'submit',
            '#value' => t('Send for Translation'),
          );
        }
      }

      drupal_add_css($module_path . '/css/globallink.css');
    }
  }

  return $form;
}

/**
 * Handles clearing the "Changed" status on entities that have been sent.
 */
function globallink_entity_form_clear_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op != t('Clear "Changed" Status')) {
    return;
  }

  $nids = array_filter($form_state['input']['table']);

  if (count($nids) == 0) {
    form_set_error('', t('No items selected.'));
  }
  else {
    $tgt_arr = $form_state['input']['submission_target_locale'];
    $source = globallink_get_locale_code($GLOBALS['globallink_entity_source_locale']);
    $nid_arr = array();

    foreach ($nids as $nvid) {
      list($nid, $vid) = explode('-', $nvid, 2);
      array_push($nid_arr, $nid);
    }

    globallink_entity_update_change_flag($nid_arr, $source, $tgt_arr);
    drupal_set_message(t('Status has been cleared successfully for selected record(s).'));
  }
}

/**
 * Validates entity form input.
 */
function globallink_entity_dashboard_form_validate($form, &$form_state) {
  module_load_include('inc', 'globallink', 'globallink_settings');
  
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $pd4 = globallink_get_project_director_details();

  globallink_validate_project_director_details($pd4);

  $source_locale = $form_state['values']['submission_source_locale'];
  $target_locale_arr = $form_state['values']['submission_target_locale'];

  if ($op != t('Send for Translation')) {
    return;
  }

  $nids = array_filter($form_state['values']['table']);

  if (count($nids) == 0) {
    $nids = array_filter($form_state['values']['table']);
    form_set_error('', t('No items selected.'));
  }

  if (!empty($form_state['values']['submission_due_date'])) {
    $due_date = (globallink_convert_date_to_timestamp($form_state['values']['submission_due_date'])) / 1000;

    if ($due_date < REQUEST_TIME) {
      form_set_error('', t('The specified date is in past.'));
    }
  }

  if (count($nids) == 0) {
    return;
  }

  if (!isset($source_locale) || !isset($target_locale_arr)) {
    return;
  }

  foreach ($target_locale_arr as $key => $target) {
    if ($target == FALSE) {
      unset($target_locale_arr[$key]);
    }
  }

  $message = FALSE;

  if (count($target_locale_arr) == 0) {
    return;
  }

  foreach ($nids as $nid) {
    list($nid, $vid) = explode('-', $nid, 2);
    $node = node_load($nid, $vid);

    if ($node->tnid != 0 && $node->tnid != $node->nid) {
      $nid = $node->tnid;
    }

    $rows = globallink_entity_get_sent_rows_by_nid($nid);

    foreach ($rows as $row) {
      if ($row->source == $source_locale && array_search($row->target, $target_locale_arr)) {
        $message = TRUE;
      }
    }
  }

  if ($message) {
    drupal_set_message(t('Ignored record(s) that has already been sent out for translation.'), 'warning');
  }
}

/**
 * Handles entity form submission.
 */
function globallink_entity_dashboard_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $submission_details = array();
  $pd4 = globallink_get_project_director_details();

  if ($op != t('Send for Translation')) {
    return;
  }

  try {
    $nids = array_filter($form_state['values']['table']);
    $submission_name = $form_state['values']['submission_name'];
    $source_locale = $form_state['values']['submission_source_locale'];
    $target_locale_arr = $form_state['values']['submission_target_locale'];
    $submission_details['instructions'] = $form_state['values']['submission_instructions'];
    $submission_details['submitter'] = $form_state['values']['submission_submitter'];

    foreach ($target_locale_arr as $key => $target) {
      if ($target == FALSE) {
        unset($target_locale_arr[$key]);
      }
    }

    $due_date = globallink_convert_date_to_timestamp($form_state['values']['submission_due_date']);
    $project_code = $form_state['values']['submission_pd_project'];

    $updated_globalLink_arr = globallink_entity_send_for_translations($nids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details);

    if (count($updated_globalLink_arr) > 0) {
      globallink_entity_update_ticket_id($updated_globalLink_arr, $project_code);
      drupal_set_message(t('Content has been successfully sent for translations.'));
    }
    else {
      if (empty($_SESSION['messages']['warning'])) {
        drupal_set_message(t('Content is empty and has not been sent for translations.'), 'warning');
      }
    }

    if (isset($_GET['rnid'])) {
      unset($_GET['rnid']);
    }
  }
  catch (SoapFault $se) {
    watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
    form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
  }
  catch (Exception $e) {
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
    form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
  }
}
