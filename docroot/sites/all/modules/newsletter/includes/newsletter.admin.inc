<?php

/**
 * @file
 * Admin page callbacks for the newsletter module.
 */

/**
 * Menu callback; present newsletter statistics.
 */
function newsletter_stats($form, &$form_state) {
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $options['compare'] = t('Compare the selected newsletters');
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'compare',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'nnid'),
    'title' => array('data' => t('Newsletter/List Title'), 'field' => 'title'),
    'last_sent' => array('data' => t('Last Sent'), 'field' => 'last_sent', 'sort' => 'desc'),
    'subscribers_sent' => array('data' => t('E-mails Sent'), 'field' => 'subscribers_sent'),
    'clicks' => array('data' => t('Total Clicks'), 'field' => 'clicks'),
    'ctr' => array('data' => t('CTR')),
    'opened' => array('data' => t('Opened(times)'), 'field' => 'opens'),
    'open_rate' => array('data' => t('Open Rate')),
    'send_again' => array('data' => t('To be sent again'), 'field' => 'send_again'),
  );

  $subquery = db_select('newsletter_newsletter', 's');
  $subquery->fields('s', array('title'));
  $subquery->addExpression('MAX(last_sent)', 'maxts');
  $subquery->groupBy('title');

  $query = db_select('newsletter_newsletter', 't')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query->fields('t');
  $query->join($subquery, 'grp', 'grp.title = t.title AND grp.maxts = t.last_sent');
  $query->leftjoin('newsletter_list', 'list', 't.title = list.title');
  $query->addField('list', 'send_again', 'send_again');
  $query->condition('t.last_sent', 0, '<>');
  $query->limit(50);
  $query->orderByHeader($header);
  $result = $query->execute();

  $options = array();
  foreach ($result as $row) {
    $send_again_timestamp = strtotime($row->send_again);
    $options[$row->nnid] = array(
      'id' => (int) $row->nnid,
      'title' => check_plain($row->title),
      'last_sent' => format_date($row->last_sent, 'short'),
      'subscribers_sent' => (int) $row->subscribers_sent,
      'clicks' => (int) $row->clicks,
      'ctr' => number_format(@($row->clicks/$row->subscribers_sent)*100, 2) . '%',
      'opened' => (int) $row->opens,
      'open_rate' => number_format(@($row->opens/$row->subscribers_sent)*100, 2) . '%',
      'send_again' => is_numeric($send_again_timestamp) ? format_date($send_again_timestamp, 'short') : '-',
    );
  }

  $form['newsletter'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No newsletters sent yet.'),
  );

  $form['pager'] = array('#theme' => 'pager');
  return $form;
}

function newsletter_stats_validate($form, &$form_state) {
  if (!array_filter($form_state['values']['newsletter'])) {
    form_set_error('newsletter', t('Please select newsletters to compare'));
  }
}

function newsletter_stats_submit($form, &$form_state) {
  switch ($form_state['values']['operation']) {
    case 'compare':
      $args['compare'] = '';
      foreach (array_filter($form_state['values']['newsletter']) as $newsletter) {
        $args['compare'] .= $newsletter . ',';
      }
      $args['compare'] = drupal_encode_path(rtrim($args['compare'], ','));
      $form_state['redirect'] = array(
        'admin/config/media/newsletter/stats/graphic',
        array('query' => $args)
      );
      break;
  }
}

/**
 * Menu callback; present newsletter statistics graphs.
 */
function newsletter_stats_graphic() {
  if (!module_exists('libraries')) {
    return drupal_set_message(t('For graphical statistics
      please install the <a href=@url-lib>Libraries API</a>
      and <a href=@url-jqplot>download the jQplot library</a>.',
    array(
      '@url-lib' => 'http://drupal.org/project/libraries',
      '@url-jqplot' => 'https://bitbucket.org/cleonello/jqplot/downloads/',
    )),
    'warning');
  }
  $path = libraries_get_path('jqplot');
  if (!file_exists($path . '/jquery.jqplot.min.js')) {
    return drupal_set_message(t('jQplot library is not present in your filesystem.
      You need to <a href=@url>download</a> it
      and extract all its files to @path
      folder, so the library is located to
      @path/jquery.jqplot.min.js',
    array(
      '@url' => 'https://bitbucket.org/cleonello/jqplot/downloads/',
      '@path' => $path,
    )),
    'warning');
  }

  // Add css and js files for our charts.
  $iesupport = array(
    '#type' => 'markup',
    '#markup' => '<!--[if lt IE 9]>
      <script language="javascript"
      type="text/javascript" src="' . $path . '/excanvas.js">
      </script><![endif]--> ',
  );

  drupal_add_html_head($iesupport, 'newsletter');

  drupal_add_js($path . '/jquery.jqplot.min.js');
  drupal_add_js($path . '/plugins/jqplot.highlighter.min.js');
  drupal_add_js($path . '/plugins/jqplot.enhancedLegendRenderer.min.js');
  drupal_add_js($path . '/plugins/jqplot.canvasAxisLabelRenderer.min.js');
  drupal_add_js($path . '/plugins/jqplot.dateAxisRenderer.min.js');
  drupal_add_css($path . '/jquery.jqplot.min.css');
  drupal_add_css(drupal_get_path('module', 'newsletter') . '/css/newsletter.admin.css');

  if (isset($_GET['compare'])) {
    $ids = urldecode($_GET['compare']);
    $ids = explode(',', $ids);

  foreach ($ids as $id) {
      $newsletters[] = db_query('SELECT * FROM {newsletter_newsletter}
        WHERE nnid = :id
        AND last_sent <> 0',
        array(':id' => $id))
        ->fetchObject();
    }
  }
  else {
    // Compare all the existing newsletters; might be slow
    // @TODO check performance
    $newsletters = db_query('SELECT *
      FROM {newsletter_newsletter}
      WHERE last_sent <> 0')
      ->fetchAll();
  }

  $ctr_vars = array();
  $or_vars = array();
  $smaller_timestamp = REQUEST_TIME;
  foreach ($newsletters as $newsletter) {
    $ctr = @($newsletter->clicks/$newsletter->subscribers_sent) * 100;
    $timestamp_ms = $newsletter->last_sent * 1000;
    @$ctr_vars[$newsletter->title] .= "[$timestamp_ms, $ctr], ";

    $open_rate = @($newsletter->opens/$newsletter->subscribers_sent) * 100;
    @$or_vars[$newsletter->title] .= "[$timestamp_ms, $open_rate], ";

    $smaller_timestamp = ($newsletter->last_sent < $smaller_timestamp)
      ? $newsletter->last_sent
      : $smaller_timestamp;
  }
  $first_day = format_date($smaller_timestamp, 'custom', 'Y-m-d');
  if (empty($ctr_vars) && empty($or_vars)) {
    return drupal_set_message(t('No statistics have been gathered yet.'), 'warning');
  }

  $jqplot_js = '(function ($) {$(document).ready(function(){
    $.jqplot.config.enablePlugins = true;';
  $i=1;
  $cvars = '';
  $ovars = '';
  $labels = '';
  foreach ($ctr_vars as $title => $string) {
    $jqplot_js .= "var l$i = [$string];\n";
    $cvars .= "l$i, ";
    $labels .= "{label:'$title'}, ";
    $i++;
  }
  $k=$i;
  foreach ($or_vars as $title => $string) {
    $jqplot_js .= "var l$k = [$string];\n";
    $ovars .= "l$k, ";
    $k++;
  }
  $jqplot_js .= "var first_day = '$first_day';";
  $jqplot_js .= "plot1 = $.jqplot('chart1', [$cvars], {";
  $jqplot_js .= 'legend:{show:true, renderer:$.jqplot.EnhancedLegendRenderer},
                  seriesDefaults: {lineWidth:4},';
  $jqplot_js .= "series:[$labels],";
  $jqplot_js .= 'seriesColors:[ "#4bb2c5", "#c5b47f", "#EAA228", "#579575",
                    "#839557", "#958c12", "#953579", "#4b5de4",
                    "#d8b83f", "#ff5800", "#0085cc"],
                  highlighter: {bringSeriesToFront: true},
                  axes:{
                    xaxis:{
                      renderer: $.jqplot.DateAxisRenderer,
                      tickOptions:{formatString:"%d %b %y"},
                      min: first_day,
                      label:"Date Sent",
                    },
                    yaxis:{
                      pad:1.0,
                      numberTicks:5,
                      autoscale:false,
                      label:"CTR (%)",
                    }
                  }
                });';
  $jqplot_js .= "plot2 = $.jqplot('chart2', [$ovars], {";
  $jqplot_js .= 'legend:{show:true, renderer:$.jqplot.EnhancedLegendRenderer},
                  seriesDefaults: {lineWidth:4},';
  $jqplot_js .= "series:[$labels],";
  $jqplot_js .= 'seriesColors:[ "#4bb2c5", "#c5b47f", "#EAA228", "#579575",
                    "#839557", "#958c12", "#953579", "#4b5de4",
                    "#d8b83f", "#ff5800", "#0085cc"],
                  highlighter: {bringSeriesToFront: true},
                  axes:{
                    xaxis:{
                      renderer: $.jqplot.DateAxisRenderer,
                      tickOptions:{formatString:"%d %b %y"},
                      min: first_day,
                      label:"Date Sent",
                    },
                    yaxis:{
                      pad:1.0,
                      numberTicks:10,
                      autoscale:false,
                      label:"Open Rate (%)"
                    }
                  }
              });
          });})(jQuery);';
  drupal_add_js( $jqplot_js, 'inline');

  return '<div id="chart1"></div><div id="chart2"></div>';
}

/**
 * Menu callback; multistep form to create and send a custom newsletter.
 */
function newsletter_create_send($form, &$form_state) {

  if (!empty($form_state['step']) && $form_state['step'] == 2) {
    return newsletter_create_send_form_page_two($form, $form_state);
  }

  $templates = db_query('SELECT ntid, subject
    FROM {newsletter_template}
    WHERE basic = 0')
    ->fetchAllKeyed();
  $lists['all'] = t('All subscribers');
  $lists += db_query('SELECT nlid, title FROM {newsletter_list}')->fetchAllKeyed();

  $form_state['step'] = 1;
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t('A unique newsletter title'),
  );
  $form['template'] = array(
    '#title' => t('Template'),
    '#type' => 'select',
    '#required' => TRUE,
    '#description' => t('Select the template this newsletter will use'),
    '#options' => array_map('check_plain', $templates),
  );
  $form['list'] = array(
   '#title' => t('Subscriber list'),
   '#type' => 'select',
   '#required' => TRUE,
   '#description' => t('Select the subscriber list this newsletter will use'),
   '#options' => array_map('check_plain', $lists),
  );
  $form['preview'] = array(
    '#type' => 'submit',
    '#value' => t('Preview'),
    '#submit' => array('newsletter_create_send_next_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/newsletter/create-send',
  );
  return $form;
}
function newsletter_create_send_next_submit($form, &$form_state) {
  $form_state['page_values'][1] = $form_state['values'];
  $form_state['step'] = 2;
  $form_state['rebuild'] = TRUE;
}
function newsletter_create_send_form_page_two($form, &$form_state) {

  $data = newsletter_create($form_state['values']['list'], $form_state['values']['template'])->preview();
  $form['newsletter'] = array(
    '#type' => 'item',
    '#description' => '<strong>' . t('Please bare in mind that any change here will affect the template directly') . '</strong>',
  );
  $form['newsletter']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#value' => check_plain($data['subject']),
  );
  $form['newsletter']['body'] = array(
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#value' => $data['body'],
    '#format' => $data['body_format'],
  );
  $form['test-email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail to send test mail to'),
    '#description' => t('Leave this empty to use the site e-mail address'),
  );
  $form['send'] = array(
    '#type' => 'submit',
    '#value' => t('Send newsletter now'),
    '#submit' => array('newsletter_create_send_form_page_two_submit'),
  );
  $form['send-test'] = array(
    '#type' => 'submit',
    '#value' => t('Send test e-mail first to address above'),
    '#submit' => array('newsletter_create_send_form_page_two_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/newsletter/create-send',
  );
  return $form;
}

function newsletter_create_send_form_page_two_submit($form, &$form_state) {
  $title = $form_state['page_values'][1]['title'];
  $ntid = $form_state['page_values'][1]['template'];

  $changed_subject = $form_state['input']['subject'] != $form_state['values']['subject'];
  $changed_body = $form_state['input']['body']['value'] != $form_state['values']['body']['value'];
  $changed_format = $form_state['input']['body']['format'] != $form_state['values']['body']['format'];
  if ($changed_subject || $changed_body || $changed_format) {
    $template = newsletter_template_load($ntid);
    $template->subject = $form_state['input']['subject'];
    $template->field_newsletter_body[LANGUAGE_NONE][0] = $form_state['input']['body'];
    entity_get_controller('newsletter_template')->save($template);
  }

  $newsletter = entity_get_controller('newsletter_newsletter')->create($title, $ntid, TRUE);

  $subscribers = ($form_state['page_values'][1]['list'] == 'all')
    ? newsletter_subscriber_load()
    : newsletter_subscribers_by_list($form_state['page_values'][1]['list']);
  if ($form_state['clicked_button']['#value'] == t('Send newsletter now')) {
    newsletter_queue_custom_newsletter($subscribers, $newsletter);
  }
  else {
    $mail = !empty($form_state['input']['test-email']) ? $form_state['input']['test-email'] : NULL;
    newsletter_send_test($ntid, $mail);
    entity_get_controller('newsletter_newsletter')->saveDraft($subscribers, $newsletter);
  }
  $form_state['redirect'] = 'admin/config/media/newsletter/create-send';
}

/**
 * Menu callback; send a manual newsletter.
 */
function newsletter_manual_send($list) {
  // Dont send.Just queue it for next cron run
  $remaining = variable_get('newsletter_for_next_cron', array());
  $remaining[$list->nlid] = TRUE;
  variable_set('newsletter_for_next_cron', $remaining);

  drupal_set_message(t('Newsletter queued for the next cron run'));
  return drupal_goto('admin/config/media/newsletter/create-send');
}

/**
 * Menu callback; send a custom newsletter draft.
 */
function newsletter_form_send_draft($form, &$form_state, $newsletter) {
  $form['nnid'] = array(
    '#type' => 'hidden',
    '#value' => $newsletter->nnid,
  );
  $output = t('Send this newsletter draft now?');
  return confirm_form($form, t('Really send?'), 'admin/config/media/newsletter/lists', filter_xss($output));

}

function newsletter_form_send_draft_submit($form, &$form_state) {
  newsletter_send_draft($form_state['values']['nnid']);
}

/**
 * Menu callback; delete a custom newsletter draft.
 */
function newsletter_form_delete_draft($form, &$form_state, $newsletter) {
  $form['nnid'] = array(
    '#type' => 'hidden',
    '#value' => $newsletter->nnid,
  );
  $output = t('Are you sure you want to delete this newsletter draft?');
  $output .= t('This action cannot be undone.');
  return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/lists', filter_xss($output));
}

function newsletter_form_delete_draft_submit($form, &$form_state) {
  $deleted = entity_get_controller('newsletter_newsletter')->deleteDraft($form_state['values']['nnid']);
  ($deleted) ?
    drupal_set_message(t('Draft deleted succesfully')) :
    drupal_set_message(t('Draft failed to delete'), 'error');
}

/**
 * Menu callback; delete a sent custom newsletter from database.
 */
function newsletter_form_delete_newsletter($form, &$form_state, $newsletter) {
  $form['nnid'] = array(
    '#type' => 'hidden',
    '#value' => $newsletter->nnid,
  );
  $output = t('Are you sure you want to delete this newsletter?');
  $output .= '<br />';
  $output .= t('All statistics associated with this newsletter will be deleted too.');
  $output .= '<br />';
  $output .= t('This action cannot be undone.');
  return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/lists', filter_xss($output));
}

function newsletter_form_delete_newsletter_submit($form, &$form_state) {
  $deleted = entity_get_controller('newsletter_newsletter')->delete($form_state['values']['nnid']);
  ($deleted) ?
    drupal_set_message(t('Newsletter deleted succesfully')) :
    drupal_set_message(t('Newsletter failed to delete'), 'error');
}

/**
 * Menu callback; List manual lists and newsletter drafts.
 */
function newsletter_draft_manual_list() {

  $header = array(
    'title' => array('data' => t('Title')),
    'status' => array('data' => t('Status')),
    'template' => array('data' => t('Template')),
    'operations' => array('data' => t('Operations')),
  );
  $query = db_select('newsletter_list', 'lists');
  $query->join('field_data_field_newsletter_template', 't', 't.entity_id = lists.nlid');
  $query->join('newsletter_template', 'templates', 'templates.ntid = t.field_newsletter_template_target_id');
  $query->addField('templates', 'subject', 'template_title');
  $query->fields('lists', array('title'));
  $query->addField('lists', 'nlid', 'id');
  $query->condition('send_rate', 'Manual');

  $manual_lists = $query
    ->execute()
    ->fetchAll();

  $options = array();
  $destination = drupal_get_destination();
  $custom_newsletters = newsletter_load_custom_newsletters();
  $result = array_merge($manual_lists, $custom_newsletters);
  foreach ($result as $key => $row) {

    $operations['data'] = array();

    if (!isset($row->custom)) {
      $status = t('Newsletter List with Manual send rate');
      $send_path = 'admin/config/media/newsletter/create-send/send/';
      $delete_path = 'admin/config/media/newsletter/lists/delete/';
      $class = 'manual';
    }
    elseif ($row->custom && !isset($row->sent)) {
      $status = t('Newsletter draft');
      $send_path = 'admin/config/media/newsletter/create-send/draft/';
      $delete_path = 'admin/config/media/newsletter/create-send/draft/delete/';
      $class = 'draft';
    }
    elseif ($row->custom && $row->sent) {
      $status = t('<strong>Sent</strong> Newsletter Draft');
      $delete_path = 'admin/config/media/newsletter/create-send/newsletter/delete/';
      $class = 'sent-draft';
    }

    if (!isset($row->sent)) {
      $operations['data'][] = array(
        '#type' => 'link',
        '#title' => t('send'),
        '#href' => $send_path . $row->id,
        '#options' => array('query' => $destination),
      );
      $operations['data'][] = array(
        '#markup' => ' | ',
      );
    }

    $operations['data'][] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => $delete_path . $row->id,
      '#options' => array('query' => $destination),
    );

    $options[$key] = array(
      'class' => array($class),
      'data' => array(
        'title' => check_plain($row->title),
        'status' => $status,
        'template' => check_plain($row->template_title),
        'operations' => $operations,
      ),
    );
  }

  $table = array(
    'header' => $header,
    'rows' => $options,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => TRUE,
    'empty' => t('No custom newsletters or drafts available.'),
  );
  return theme_table($table);
}

/**
 * Menu callback; Form to list subscriber lists on a table
 * and provide certain actions.
 */
function newsletter_list($form, &$form_state) {
  if (isset($form_state['storage']['confirm'])) {
    $form['operation'] = array(
      '#type' => 'hidden',
      '#value' => 'delete',
    );
    $form['list'] = array(
      '#type' => 'hidden',
      '#value' => $form_state['values']['list'],
    );
    $lists = newsletter_list_load($form_state['values']['list']);
    $output = t('Are you sure you want to delete the following newsletter lists?');
    $output .= '<ul>';
    foreach ($lists as $list) {
      $output .= !empty($list) ? '<li>' . check_plain($list->title) . '</li>' : '';
    }
    $output .= '</ul>';
    $output .= t('This action cannot be undone.');
    return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/lists', filter_xss($output));
  }

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $options['delete'] = t('Delete the selected lists');
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'nlid'),
    'title' => array('data' => t('Title'), 'field' => 'title'),
    'send_rate' => array('data' => t('Send Rate'), 'field' => 'send_rate'),
    'template' => array('data' => t('Template'), 'field' => 'ntid'),
    'created' => array('data' => t('Created'), 'field' => 'lists.created', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );
  $query = db_select('newsletter_list', 'lists')->extend('PagerDefault')->extend('TableSort');
  $query->join('field_data_field_newsletter_template', 'template_field', 'lists.nlid = template_field.entity_id');
  $query->join('newsletter_template', 'templates', 'template_field.field_newsletter_template_target_id = templates.ntid');
  $query->addField('templates', 'subject', 'template_title');
  $query->fields('lists');
  $result = $query
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  $options = array();
  $destination = drupal_get_destination();
  foreach ($result as $row) {
    $operations = array(
      'data' => array(
        array(
          '#type' => 'link',
          '#title' => t('edit'),
          '#href' => 'admin/config/media/newsletter/lists/edit/' . $row->nlid ,
          '#options' => array('query' => $destination),
        ),
        array(
          '#markup' => ' | ',
        ),
        array(
          '#type' => 'link',
          '#title' => t('delete'),
          '#href' => 'admin/config/media/newsletter/lists/delete/' . $row->nlid,
          '#options' => array('query' => $destination),
        ),
      ),
    );

    $options[$row->nlid] = array(
      'id' => (int) $row->nlid,
      'title' => check_plain($row->title),
      'send_rate' => check_plain($row->send_rate),
      'template' => check_plain($row->template_title),
      'created' => format_date($row->created, 'short'),
      'operations' => $operations,
    );
  }

  $form['list'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No lists available.'),
  );

  $form['pager'] = array('#theme' => 'pager');
  return $form;
}

function newsletter_list_submit($form, &$form_state) {
  $operation = $form_state['values']['operation'];
  $ids = array_filter($form_state['values']['list']);
  if ($ids) {
    switch ($operation) {
      case 'delete':
        if (!isset($form_state['storage']['confirm'])) {
          $form_state['storage']['confirm'] = TRUE;
          $form_state['rebuild'] = TRUE;
          return;
        }
        foreach ($ids as $id) {
          entity_get_controller('newsletter_list')->delete($id);
        }
        drupal_set_message(t('Lists deleted successfully'), 'status');
        break;
    }
  }
  else {
    drupal_set_message(t('Please Select an action to perform.'), 'warning');
  }
}

/**
 * Menu callback; add a subscribers list.
 */
function newsletter_list_add() {
  $list = entity_get_controller('newsletter_list')->create();
  return drupal_get_form('newsletter_list_edit', $list);
}

/**
 * Menu callback; delete a subscribers list.
 */
function newsletter_list_delete($form, &$form_state, $list) {
  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $list->nlid,
  );
  $output = t('Are you sure you want to delete the following newsletter list?');

  $output .= '<ul>';
  $output .= '<li>' . check_plain($list->title) . '</li>';
  $output .= '</ul>';

  $output .= t('This action cannot be undone.');
  return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/lists', filter_xss($output));
}

function newsletter_list_delete_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $deleted = entity_get_controller('newsletter_list')->delete($id);
  isset($deleted)
    ? drupal_set_message(t('List deleted successfully'), 'status')
    : drupal_set_message(t('An unknown error occured,please try again'), 'error');
  $form_state['redirect'] = isset($_GET['destination']) ? drupal_get_destination() : 'admin/config/newsletter/lists';
}

/**
 * Menu callback; edit a subscribers list.
 */
function newsletter_list_edit($form, &$form_state, $list) {

  if (isset($list->nlid)) {
    $form['nlid'] = array(
      '#type' => 'hidden',
      '#value' => (int) $list->nlid,
    );
  }
  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain($list->title),
    '#title' => 'Name',
    '#weight' => -10,
    '#required' => TRUE,
  );
  $form['send_rate'] = array(
    '#type' => 'select',
    '#options' => array(
      'Custom' => t('Custom'),
      'Manual' => t('Manual'),
      'Daily' => t('Daily'),
      'Weekly' => t('Weekly'),
      'Monthly' => t('Monthly'),
    ),
    '#default_value' => check_plain($list->send_rate),
    '#title' => t('Send rate'),
  );
  $form['send_rate_custom'] = array(
    '#attributes' => array('id' => array('send_rate_custom')),
    '#type' => 'textfield',
    '#title' => t('Custom send rate'),
    '#default_value' => is_numeric($list->send_rate) ? $list->send_rate : NULL,
    '#maxlength' => 4,
    '#size' => 4,
    '#description' => t('Only fill this option if you selected "Custom" Send rate'),
    '#field_prefix' => t('Once'),
    '#field_suffix' => t('nodes of this list are published.'),
  );

  field_attach_form('newsletter_list', $list, $form, $form_state);
  // Remove Basic Templates
  $lang = $form['field_newsletter_template']['#language'];
  unset($form['field_newsletter_template'][$lang]['#options'][1]);
  unset($form['field_newsletter_template'][$lang]['#options'][2]);
  unset($form['field_newsletter_template'][$lang]['#options'][3]);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($list->nlid) ? t('Update') : t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/newsletter/lists',
  );
  $form['#list'] = $list;
  return $form;
}

function newsletter_list_edit_validate($form, &$form_state) {
  if ($form_state['values']['send_rate'] == 'Custom') {
    if (empty($form_state['values']['send_rate_custom'])) {
      form_set_error('send_rate_custom', t('You must set the custom send rate first.'));
    }
    elseif (!is_numeric($form_state['values']['send_rate_custom'])) {
      form_set_error('send_rate_custom', t('Custom send rate must be a number.'));
    }
  }
  // Ensure list title is unique
  if ($form['#list']->title != $form_state['values']['title'] || !isset($form_state['values']['nlid'])) {
    $existing = db_query('SELECT title FROM {newsletter_list} WHERE title = :title', array('title' => $form_state['values']['title']))->fetchField();
    if ($existing) {
      form_set_error('title', t('This list name already exists'));
    }
  }
}

function newsletter_list_edit_submit($form, &$form_state) {
  $list = (object) $form_state['values'];
  $list->send_rate = ($list->send_rate=='Custom') ? $form_state['values']['send_rate_custom'] : $list->send_rate;
  if (isset($form_state['values']['nlid'])) {
    $list->id = $form_state['values']['nlid'];
  }
  field_attach_submit('newsletter_list', $list, $form, $form_state);
  $updated = entity_get_controller('newsletter_list')->save($list);
  isset($updated)
    ? drupal_set_message(t('List updated successfully'), 'status')
    : drupal_set_message(t('An unknown error occured,please try again'), 'error');
  $form_state['redirect'] = 'admin/config/media/newsletter/lists';
}

/**
 * Menu callback; present an administrative subscriber listing.
 */
function newsletter_subscriber_list($form, &$form_state) {

  if (isset($form_state['storage']['confirm'])) {
    $form['operation'] = array(
      '#type' => 'hidden',
      '#value' => 'delete',
    );
    $form['subscriber'] = array(
      '#type' => 'hidden',
      '#value' => $form_state['values']['subscriber'],
    );
    $output = t('Are you sure you want to delete the following newsletter subscribers?');
    $output .= '<ul>';
    $subscribers = newsletter_subscriber_load($form_state['values']['subscriber']);
    foreach ($subscribers as $subscriber) {
      $output .= !empty($subscriber) ? '<li>' . check_plain($subscriber->email) . '</li>' : '';
    }
    $output .= '</ul>';
    $output .= t('This action cannot be undone.');
    return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/subscribers', filter_xss($output));
  }
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $options['delete'] = t('Delete the selected subscribers');

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'nsid'),
    'email' => array('data' => t('Email'), 'field' => 'email'),
    'info' => array('data' => t('Personal Info')),
    'list_title' => array('data' => t('Subscribed to')),
    'created' => array('data' => t('Created'), 'field' => 'created', 'sort' => 'desc'),
    'confirmed' => array('data' => t('Confirmed')),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('newsletter_subscriber', 'subscribers')->extend('PagerDefault')->extend('TableSort');
  $result = $query
    ->fields('subscribers')
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  $options = array();
  $destination = drupal_get_destination();
  foreach ($result as $row) {

    $info = array('data' => array());
    $info['data'][] = !empty($row->firstname) ? array('#markup' => t('Firstname:') . check_plain($row->firstname) . '<br />') : NULL;
    $info['data'][] = !empty($row->lastname) ? array('#markup' => t('Lastname:') . check_plain($row->lastname) . '<br />') : NULL;
    $info['data'][] = !empty($row->age_group) ? array('#markup' => t('Age group:') . check_plain($row->age_group) . '<br />') : NULL;
    $info['data'][] = !empty($row->job) ? array('#markup' => t('Job:') . check_plain($row->job) . '<br />') : NULL;
    $info['data'][] = !empty($row->gender) ? array('#markup' => t('Gender:') . check_plain($row->gender) . '<br />') : NULL;
    $info['data'][] = !empty($row->receive_format) ? array('#markup' => t('Preferred format:') . check_plain($row->receive_format) . '<br />') : NULL;
    $info['data'][] = !empty($row->language) ? array('#markup' => t('Language:') . check_plain($row->language) . '<br />') : NULL;

    $lists = db_query('SELECT title
      FROM {newsletter_list} list
      JOIN {field_data_field_newsletter_list} sub_index
      ON sub_index.field_newsletter_list_target_id = list.nlid
      WHERE sub_index.entity_id = :id',
      array(':id' => $row->nsid ))
      ->fetchCol();
    $lists = implode(', ', $lists);

    $options[$row->nsid] = array(
      'id' => (int) $row->nsid,
      'email' => check_plain($row->email),
      'info' => $info,
      'list_title' => $lists,
      'created' => format_date($row->created, 'short'),
      'confirmed' => $row->confirmed ? t('Yes, on') . '<br/>' . format_date($row->confirmation_timestamp, 'short') : t('No'),
      'operations' => array(
        'data' => array(
          array(
            '#type' => 'link',
            '#title' => t('edit'),
            '#href' => 'admin/config/media/newsletter/subscribers/edit/' . $row->nsid,
            '#options' => array('query' => $destination),
          ),
          array(
            '#markup' => ' | '
          ),
          array(
            '#type' => 'link',
            '#title' => t('delete'),
            '#href' => 'admin/config/media/newsletter/subscribers/delete/' . $row->nsid,
            '#options' => array('query' => $destination),
          ),
        ),
      ),
    );
  }

  $form['subscriber'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No subscribers yet.'),
  );

  $form['pager'] = array('#theme' => 'pager');
  return $form;
}
function newsletter_subscriber_list_submit($form, &$form_state) {
  $operation = $form_state['values']['operation'];
  $ids = array_filter($form_state['values']['subscriber']);

  if ($ids) {
    switch ($operation) {
      case 'delete':
        if (!isset($form_state['storage']['confirm'])) {
          $form_state['storage']['confirm'] = TRUE;
          $form_state['rebuild'] = TRUE;
          return;
        }
        foreach ($ids as $id) {
          entity_get_controller('newsletter_subscriber')->delete($id);
        }
        drupal_set_message(t('Subscriber deleted successfully'), 'status');
        break;
    }
  }
  else {
    drupal_set_message(t('Please Select an action to perform.'), 'warning');
  }
}

/**
 * Menu callback; add subscribers.
 */
function newsletter_subscriber_add() {
  $subscriber = entity_get_controller('newsletter_subscriber')->create();
  return drupal_get_form('newsletter_subscriber_edit', $subscriber);
}

/**
 * Menu callbacks; add subscribers (mass).
 */
function newsletter_subscriber_add_mass($form, &$form_state) {

  $lists = db_query('SELECT nlid, title FROM {newsletter_list}')->fetchAllKeyed();

  $form['new'] = array(
    '#markup' => '<h3>' . t('Add Subscribers') . '</h3>',
  );
  $form['subscribers'] = array(
    '#type' => 'textarea',
    '#title' => t('Emails'),
    '#required' => TRUE,
    '#description' => t('Enter one e-mail address per line.To add additional info, use the following format using 0 or FALSE to omit a value: %format', array('%format' => 'email;firstname;lastname;gender;newsletter-receive-format')),
  );
  $form['list'] = array(
    '#type' => 'select',
    '#title' => t('Select lists'),
    '#options' => array_map('check_plain', $lists),
    '#required' => TRUE,
    '#multiple' => TRUE,
  );
  $form['submitnew'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/newsletter/subscribers',
  );
  return $form;
}

function newsletter_subscriber_add_mass_submit($form, &$form_state) {
  $entries = $form_state['values']['subscribers'];
  $entries = explode("\n", $entries);
  foreach ($entries as $entry) {
    $subscriber = new stdClass();
    $entry = explode(';', $entry);
    $entry = array_map('trim', $entry);
    $subscriber->email = (isset($entry[0]) && !empty($entry[0])) ? $entry[0] : NULL;
    $subscriber->firstname = (isset($entry[1]) && !empty($entry[1])) ? $entry[1] : NULL;
    $subscriber->lastname = (isset($entry[2]) && !empty($entry[2])) ? $entry[2] : NULL;
    $subscriber->gender = (isset($entry[3]) && !empty($entry[3])) ? $entry[3] : NULL;
    $subscriber->receive_format = (isset($entry[4]) && !empty($entry[4])) ? $entry[4] : NULL;
    foreach ($form_state['values']['list'] as $list) {
      $subscriber->field_newsletter_list[LANGUAGE_NONE][]['target_id'] = $list;
    }
    $subscriber->confirmed = 1;
    if (!isset($subscriber->email)) {
      continue;
    }
    entity_get_controller('newsletter_subscriber')->save($subscriber);
  }
  $form_state['redirect'] = 'admin/config/media/newsletter/subscribers';
}

/**
 * Menu callback; edit subscribers.
 */
function newsletter_subscriber_edit($form, &$form_state, $subscriber) {

  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();
  if (isset($subscriber->nsid)) {

    $form['edit'] = array(
      '#markup' => '<h3>' . t('Edit Subscriber') . '</h3>',
    );
    $form['nsid'] = array(
      '#type' => 'hidden',
      '#value' => (int) $subscriber->nsid,
    );
  }
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#required' => TRUE,
    '#default_value' => check_plain($subscriber->email),
    '#weight' => 0,
  );
  $form['additional'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional info'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['additional']['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => check_plain($subscriber->firstname),
  );
  $form['additional']['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => check_plain($subscriber->lastname),
  );
  $form['additional']['gender'] = array(
    '#type' => 'textfield',
    '#title' => t('Gender'),
    '#default_value' => check_plain($subscriber->gender),
  );
  $form['additional']['receive_format'] = array(
    '#type' => 'select',
    '#title' => t('Preferred Format'),
    '#options' => array(
      'html' => t('HTML'),
      'plain' => t('Plain text'),
    ),
    '#default_value' => check_plain($subscriber->receive_format),
  );

  field_attach_form('newsletter_subscriber', $subscriber, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 10,
  );
  $form['actions']['submitnew'] = array(
    '#type' => 'submit',
    '#value' => isset($subscriber->nsid) ? t('Update') : t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/newsletter/subscribers',
  );
  return $form;
}

function newsletter_subscriber_edit_submit($form, &$form_state) {
  $subscriber = (object) $form_state['values'];
  if (isset($form_state['values']['nsid'])) {
    $subscriber->nsid = $form_state['values']['nsid'];
  }
  else {
    $subscriber->confirmed = 1;
  }
  field_attach_submit('newsletter_subscriber', $subscriber, $form, $form_state);
  $updated = entity_get_controller('newsletter_subscriber')->save($subscriber);
  return ($updated)
    ? drupal_set_message(t('Subscriber updated successfully'), 'status')
    : drupal_set_message(t('An unknown error occured,please try again'), 'error');
}

/**
 * Menu callback; delete subscribers.
 */
function newsletter_subscriber_delete($form, &$form_state, $subscriber) {
  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $subscriber->nsid,
  );
  $output = t('Are you sure you want to delete the following newsletter subscriber?');

  $output .= '<ul>';
  $output .= '<li>' . check_plain($subscriber->email) . '</li>';
  $output .= '</ul>';

  $output .= t('This action cannot be undone.');
  return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/subscribers', filter_xss($output));
}

function newsletter_subscriber_delete_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $deleted = entity_get_controller('newsletter_subscriber')->delete($id);
  $deleted
    ? drupal_set_message(t('Subscriber deleted successfully'), 'status')
    : drupal_set_message(t('An unknown error occured,please try again'), 'error');
  $form_state['redirect'] = isset($_GET['destination']) ? drupal_get_destination() : 'admin/config/newsletter/subscribers';
}

/**
 * Menu callback; present an administrative template listing.
 */
function newsletter_template_list($form, &$form_state) {
  if (isset($form_state['storage']['confirm'])) {
    $form['operation'] = array(
      '#type' => 'hidden',
      '#value' => 'delete',
    );
    $form['template'] = array(
      '#type' => 'hidden',
      '#value' => $form_state['values']['template'],
    );
    $templates = newsletter_template_load($form_state['values']['template']);
    $output = t('Are you sure you want to delete the following templates?');
    $output .= '<ul>';
    foreach ($templates as $template) {
      $output .= !empty($template) ? '<li>' . check_plain($template->subject) . '</li>' : '';
    }
    $output .= '</ul>';
    $output .= t('This action cannot be undone.');
    return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/templates', filter_xss($output));
  }
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $options['delete'] = t('Delete the selected templates');

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'ntid'),
    'title' => array('data' => t('Title'), 'field' => 'subject'),
    'exposed' => array('data' => t('Exposed'), 'field' => 'exposed'),
    'created' => array('data' => t('Created'), 'field' => 'created', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('newsletter_template', 'templates')->extend('PagerDefault')->extend('TableSort');
  $result = $query
    ->fields('templates', array('ntid', 'subject', 'exposed', 'created'))
    ->condition('basic', 0)
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  $options = array();
  $destination = drupal_get_destination();
  foreach ($result as $row) {
    $options[$row->ntid] = array(
      'id' => (int) $row->ntid,
      'title' => check_plain($row->subject),
      'exposed' => $row->exposed ? t('Yes') : t('No'),
      'created' => format_date($row->created, 'short'),
      'operations' => array(
        'data' => array(
          array(
            '#type' => 'link',
            '#title' => t('edit'),
            '#href' => 'admin/config/media/newsletter/templates/edit/' . $row->ntid ,
            '#options' => array('query' => $destination),
          ),
          array(
            '#markup' => ' | ',
          ),
          array(
            '#type' => 'link',
            '#title' => t('delete'),
            '#href' => 'admin/config/media/newsletter/templates/delete/' . $row->ntid,
            '#options' => array('query' => $destination),
          ),
        ),
      ),
    );
  }

  $form['template'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No templates available.'),
  );

  $form['pager'] = array('#theme' => 'pager');

  return $form;
}

function newsletter_template_list_submit($form, &$form_state) {
  $ids = array_filter($form_state['values']['template']);
  $operation = $form_state['values']['operation'];
  switch ($operation) {
      case 'delete':
        if (!isset($form_state['storage']['confirm'])) {
          $form_state['storage']['confirm'] = TRUE;
          $form_state['rebuild'] = TRUE;
          return;
        }
        foreach ($ids as $id) {
          entity_get_controller('newsletter_template')->delete($id);
        }
        drupal_set_message(t('Templates deleted successfully'), 'status');
        break;
    }
}

/**
 * Menu callback; add a newsletter template.
 */
function newsletter_template_add() {
  $template = entity_get_controller('newsletter_template')->create();
  return drupal_get_form('newsletter_template_edit', $template);
}

/**
 * Menu callback; delete newsletter templates.
 */
function newsletter_template_delete($form, &$form_state, $template) {
  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $template->ntid,
  );
  $output = t('Are you sure you want to delete the following template?');

  $output .= '<ul>';
  $output .= '<li>' . check_plain($template->subject) . '</li>';
  $output .= '</ul>';

  $output .= t('This action cannot be undone.');
  return confirm_form($form, t('Delete the following?'), 'admin/config/media/newsletter/templates', filter_xss($output));
}

function newsletter_template_delete_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $deleted = entity_get_controller('newsletter_template')->delete($id);
  $deleted
    ? drupal_set_message(t('Template deleted successfully'), 'status')
    : drupal_set_message(t('An unknown error occured,please try again'), 'error');
  $form_state['redirect'] = isset($_GET['destination']) ? drupal_get_destination() : 'admin/config/newsletter/templates';
}

/**
 * Menu callbacks; edit basic or non-basic newsletter templates.
 */
function newsletter_basic_template_edit($ntid) {

  $template = newsletter_template_load($ntid);

  return drupal_get_form('newsletter_template_edit', $template);
}

function newsletter_template_edit($form, &$form_state, $template) {

  if (isset($template->ntid)) {
    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => (int) $template->ntid,
    );
  }
  $form['subject'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain($template->subject),
    '#title' => t('Subject'),
    '#description' => t('This will also used as newsletter title.'),
    '#weight' => -10,
    '#required' => TRUE,
  );
  $form['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => $template->basic ? '' : t('Use [repeat] and [/repeat] for recurring items such as nodes'),
  );
  $form['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => $template->basic
      ? array('site', 'newsletter', 'newsletter_list', 'newsletter_subscriber', 'newsletter_template')
      : array('site', 'node', 'term', 'newsletter', 'newsletter_list', 'newsletter_subscriber', 'newsletter_template'),
    '#recursion_limit' => 2,
  );

  field_attach_form('newsletter_template', $template, $form, $form_state);

  $form['exposed'] = array(
    '#type' => 'checkbox',
    '#default_value' => $template->exposed,
    '#title' => t('Exposed'),
    '#weight' => 9,
  );

  $form['actions'] = array(
    '#type' => 'item',
    '#weight' => 10,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($template->ntid) ? t('Update') : t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/media/newsletter/templates',
  );
  return $form;
}

function newsletter_template_edit_submit($form, &$form_state) {
  $template = (object) $form_state['values'];
  if (isset($form_state['values']['id'])) {
    $template->ntid = $form_state['values']['id'];
  }
  field_attach_submit('newsletter_template', $template, $form, $form_state);
  $updated = entity_get_controller('newsletter_template')->save($template);
  isset($updated)
    ? drupal_set_message(t('Template updated successfully'), 'status')
    : drupal_set_message(t('An unknown error occured,please try again'), 'error');
  $form_state['redirect'] = 'admin/config/media/newsletter/templates';
}

/**
 * Menu callback; present newsletter configuration.
 */
function newsletter_configure($form, &$form_state) {
  $lists = db_query('SELECT nlid, title FROM {newsletter_list}')->fetchAllKeyed();
  $personal_info_form = variable_get('newsletter_personal_info_form');
  $personal_info_options = array(
      'hidden' => t('hide'),
      'show' => t('show'),
      'require' => t('require'),
  );

  $form['test'] = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Send a test Email'),
  );
  $form['test']['newsletter_test_to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => variable_get('newsletter_test_to', variable_get('site_mail')),
  );
  $form['test']['send_test'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('General settings'),
  );
  $form['settings']['newsletter_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('newsletter_from', variable_get('site_mail')),
    '#description' => t('Suggested format: User &lt;user@example.com&gt;'),
  );
  $form['settings']['newsletter_format'] = array(
    '#type' => 'select',
    '#title' => t('Format'),
    '#options' => array(
      'plain' => t('Plain Text'),
      'html' => t('HTML'),
    ),
    '#default_value' => variable_get('newsletter_format', 'html'),
  );
  $form['settings']['newsletter_track_open_rate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track open rate'),
    '#default_value' => variable_get('newsletter_track_open_rate', FALSE),
    '#description' => t('Enable it, only if you really want the email to be tracked,
      since the method used might cause some email clients to mark your emails as spam'),
  );
  $form['settings']['newsletter_cron_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of mails to send per cron run'),
    '#default_value' => variable_get('newsletter_cron_number', 500),
    '#description' => t('The more newsletter lists the smaller this number should be.'),
  );
  $form['settings']['newsletter_node_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of nodes to send per newsletter'),
    '#default_value' => variable_get('newsletter_node_limit', 50),
    '#description' => t('The max number of nodes to send with a newsletter.'),
  );
  $form['settings']['newsletter_send_confirm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send confirmation email'),
    '#default_value' => variable_get('newsletter_send_confirm'),
    '#description' => t('Attention: If you disable confirmation e-mails, users will be able to subscribe others without their permission.'),
  );
  $form['settings']['newsletter_send_welcome'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send welcome email'),
    '#default_value' => variable_get('newsletter_send_welcome'),
  );
  $form['settings']['newsletter_send_unsubscribe'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send unsubscribe email, after a subscriber decides to unsubscribe'),
    '#default_value' => variable_get('newsletter_send_unsubscribe'),
  );
  $form['settings']['newsletter_auto_subscribe_new_users'] = array(
    '#type' => 'select',
    '#title' => t('Automatically subscribe newly registered user to the selected lists:'),
    '#options' => array_map('check_plain', $lists),
    '#multiple' => TRUE,
    '#description' => t('Leave empty to bypass this feature'),
    '#default_value' => variable_get('newsletter_auto_subscribe_new_users', array()),
  );
  $form['settings']['newsletter_use_smtp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable SMTP support'),
    '#default_value' => variable_get('newsletter_use_smtp', FALSE),
    '#description' => t('Requires <a href=@url>SMTP</a> module', array('@url' => 'http://drupal.org/project/smtp')),
  );
  $form['subscribe_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscribe form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['subscribe_form']['newsletter_show_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always show e-mail in the subscribe form.'),
    '#default_value' => variable_get('newsletter_show_email', TRUE),
  );
  $form['subscribe_form']['newsletter_show_email_in_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always show e-mail in the subscribe form block.'),
    '#default_value' => variable_get('newsletter_show_email_in_block', FALSE),
  );
  $form['subscribe_form']['newsletter_always_show_subscribe_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always show subscribe block,even if user is subscribed.'),
    '#default_value' => variable_get('newsletter_always_show_subscribe_block', FALSE),
  );
  $form['subscribe_form']['newsletter_personal_info_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Personal info form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['subscribe_form']['newsletter_personal_info_form']['firstname'] = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'radios',
    '#title' => t('First Name (textfield)'),
    '#options' => $personal_info_options,
    '#default_value' => $personal_info_form['firstname'],
  );
  $form['subscribe_form']['newsletter_personal_info_form']['lastname'] = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'radios',
    '#title' => t('Last Name (textfield)'),
    '#options' => $personal_info_options,
    '#default_value' => $personal_info_form['lastname'],
  );
  $form['subscribe_form']['newsletter_personal_info_form']['gender'] = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'radios',
    '#title' => t('Gender (select)'),
    '#options' => $personal_info_options,
    '#default_value' => $personal_info_form['gender'],
  );
  $form['subscribe_form']['newsletter_personal_info_form']['receive_format'] = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'radios',
    '#title' => t('Preferred Format (select)'),
    '#options' => $personal_info_options,
    '#default_value' => $personal_info_form['receive_format'],
  );
  $form = system_settings_form($form);

  $form['#submit'][] = 'newsletter_configure_submit';
  return $form;
}

function newsletter_configure_submit($form, &$form_state) {

  if ($form_state['clicked_button']['#value'] == t('Send')) {

    $status = newsletter_create()->sendTest(variable_get('newsletter_test_to', variable_get('site_mail')));
    if ($status) {
      drupal_set_message(t('Test mail sent successfully!'), 'status');
    }
    else {
      drupal_set_message(t('Your test mail failed to be delivered.Please check your server logs for more information.'), 'error');
    }
  }
}

/**
 * Queues a custom newsletter for cron run.
 *
 * @param $subscribers
 *   An array containing the subscribers this newsletter must be delivered.
 * @param $ntid
 *   The id of the newsletter template to be used.
 * @param $nnid
 *   The id of the newsletter.
 */
function newsletter_queue_custom_newsletter($subscribers, $newsletter) {
    $custom_queue = variable_get('newsletter_custom_for_next_cron', array());
    $custom_queue[] = array(
      'subscribers' => $subscribers,
      'ntid' => $newsletter->ntid,
      'nnid' => $newsletter->nnid,
    );
    variable_set('newsletter_custom_for_next_cron', $custom_queue);
    drupal_set_message(t('Newsletter queued for the next cron run'));
}


/**
 * Sends a test newsletter to site administrator.
 *
 * @param $ntid
 *   The newsletter template id.
 * @param $email
 *   The email to send this newsletter to.NULL to use the site_mail instead.
 */
function newsletter_send_test($ntid, $mail = NULL) {
  if (!isset($mail)) {
    $mail = variable_get('site_mail');
  }
  $message = drupal_mail('newsletter', 'test-draft', $mail, language_default()->language, array(), $mail, FALSE);
  $message = array_merge($message, newsletter_create(array(), $ntid)->preview());
  $message['format'] = variable_get('newsletter_format', 'html');
  $message['body'] = array($message['body']);
  $system = drupal_mail_system('newsletter', 'test-draft');
  $message = $system->format($message);
  $status = $system->mail($message);
  return $status ? drupal_set_message(t('Test mail sent succesfully')) : drupal_set_message(t('Test mail failed to send'));
}

/**
 * Load draft newsletters as object containing all needed information.
 *
 * @param $sent
 *   Whether to load the sent custom Newlsetters.
 * @return
 *   an array with newsletter draft objects with the following properties:
 *   id: the draft key in the drupal variable system.
 *   title: the newsletter title.
 *   template_title: the template title this draft uses.
 *   subscribers: the array with subscriber objects that this newsletter will be
 *   sent to
 *   draft: boolean TRUE.
 */
function newsletter_load_custom_newsletters($sent = TRUE) {
  $draft_newsletters = variable_get('newsletter_custom_drafts', array());
  $custom_newsletters = array();
  foreach ($draft_newsletters as $nnid => $newsletter) {
    $draft = new stdClass();
    $draft->id = $nnid;
    $draft->title = newsletter_newsletter_load($nnid)->title;
    $draft->template_title = newsletter_template_load($newsletter['ntid'])->subject;
    $draft->custom = TRUE;
    $custom_newsletters[] = $draft;
  }
  if ($sent) {
    $query = db_select('newsletter_newsletter', 'newsletters');
    $query->join('newsletter_template', 'templates', 'newsletters.ntid = templates.ntid');
    $query->addField('templates', 'subject', 'template_title');
    $query->addField('newsletters', 'nnid', 'id');
    $query->fields('newsletters', array('title'));
    $query->condition('custom', 1);

    $manual_lists = $query->execute()->fetchAll();
    foreach ($manual_lists as $manual) {
      $manual->custom = TRUE;
      if (array_search($manual, $custom_newsletters) === FALSE) {
        $manual->sent = TRUE;
        $custom_newsletters[] = $manual;
      }
    }
  }
  return $custom_newsletters;
}

/**
 * Send a custom newsletter draft.
 *
 * @param $nnid
 *   The draft's key in Drupal's variable system, which is the newsletter's id.
 */
function newsletter_send_draft($nnid) {
  $drafts = variable_get('newsletter_custom_drafts', array());
  if (isset($drafts[$nnid]) && !empty($drafts[$nnid])) {
    $newsletter = newsletter_newsletter_load($nnid);
    newsletter_queue_custom_newsletter($drafts[$nnid]['subscribers'], $newsletter);
    unset($drafts[$nnid]);
    variable_set('newsletter_custom_drafts', $drafts);
  }
}
