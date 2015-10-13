<?php
/**
 * @file
 * restmini: Admin layer.
 */


/**
 * ???.
 *
 * @return string
 */
function restmini_admin_help() {
  drupal_add_css(
    drupal_get_path('module', 'restmini') . '/admin/restmini.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  $table_header = array(
    '', // Who's fault.
    array(
      'data' => t('HTTP Request methods !x', array('!x' => '&nbsp; &#x25b6;'), array('context' => 'module:restmini')),
      'colspan' => 2,
      'class' => array('restmini-help-capitalize'),
    ),
    '<b>HEAD</b>',
    '<b>GET</b>',
    '<b>POST</b>',
    '<b>PUT</b>',
    '<b>DELETE</b>',
  );
  $table_rows = array();

  $table_rows[] = array(
    'data' => array(
      array( // Who's fault.
        'data' => '',
        'colspan' => 2,
        'rowspan' => 2,
        'class' => array('restmini-help-odd-even'),
      ),
      array(
        'data' => t('Operations', array(), array('context' => 'module:restmini')),
        'rowspan' => 2,
        'class' => array('restmini-help-odd-even', 'restmini-help-capitalize', 'restmini-help-bold', 'restmini-help-border-right'),
      ),
      t('!bindex!_b!nllist all objects', array('!nl' => '<br/>', '!b' => '<b>', '!_b' => '</b>'), array('context' => 'module:restmini')),
      array(
        'data' => t('!bindex!_b!nllist all objects', array('!nl' => '<br/>', '!b' => '<b>', '!_b' => '</b>'), array('context' => 'module:restmini')),
        'class' => array('restmini-help-border-right'),
      ),
      array(
        'data' => t('!bcreate!_b!nlone or more objects!nlwith request body', array('!nl' => '<br/>', '!b' => '<b>', '!_b' => '</b>'), array('context' => 'module:restmini')),
        'rowspan' => 2,
      ),
      array(
        'data' => t('!bupdate!_b!nlone or more objects!nlby a path parameter!nland request body', array('!nl' => '<br/>', '!b' => '<b>', '!_b' => '</b>'), array('context' => 'module:restmini')),
        'rowspan' => 2,
        'class' => array('restmini-help-border-right'),
      ),
      array(
        'data' => t('!bdelete!_b!nlone or more objects!nlby a path parameter', array('!nl' => '<br/>', '!b' => '<b>', '!_b' => '</b>'), array('context' => 'module:restmini')),
        'rowspan' => 2,
      ),
    ),
  );

  $table_rows[] = array(
    'data' => array(
      t('!bretrieve!_b!nlone or more objects!nlby a path parameter', array('!nl' => '<br/>', '!b' => '<b>', '!_b' => '</b>'), array('context' => 'module:restmini')),
      array(
        'data' => t('!bretrieve!_b!nlone or more objects!nlby a path parameter', array('!nl' => '<br/>', '!b' => '<b>', '!_b' => '</b>'), array('context' => 'module:restmini')),
        'class' => array('restmini-help-border-right'),
      ),
    ),
  );

  $table_rows[] = array(
    'data' => array(
      array( // Who's fault.
        'data' => t('Outcome', array(), array('context' => 'module:restmini')),
        'class' => array('restmini-help-inner-legend', 'restmini-help-border-right'),
      ),
      array(
        'data' => t('Response status !y', array('!y' => '&nbsp; &#x25bc;'), array('context' => 'module:restmini')),
        'class' => array('restmini-help-inner-legend', 'restmini-help-capitalize'),
      ),
      array(
        'data' => t('Notes', array(), array('context' => 'module:restmini')),
        'class' => array('restmini-help-inner-legend', 'restmini-help-border-right'),
      ),
      array(
        'data' => t('•&nbsp; Response body - if request method&mdash;response status combination allowed at all &nbsp;•',
          array(), array('context' => 'module:restmini')),
        'colspan' => 5,
        'class' => array('restmini-help-inner-legend', 'restmini-help-center'),
      ),
    ),
  );


  $status_methods = RestMini::statusCodes();

  // Count 2xx + 3xx, 4xx and 5xx.
  $status_codes = array_keys($status_methods);
  $benign = $client = $server = 0;
  foreach ($status_codes as $code) {
    if ($code < 400) {
      ++$benign;
    }
    elseif ($code < 500) {
      ++$client;
    }
    else {
      ++$server;
    }
  }

  foreach ($status_methods as $code => $methods) {
    $note = '';
    switch ($code) {
      case 201:
        $note = t('when PUT: Updated', array(), array('context' => 'module:restmini'));
        break;
      case 204:
        $note = t('silent success', array(), array('context' => 'module:restmini'));
        break;
      case 304:
        $note = t('client use cache instead', array(), array('context' => 'module:restmini'));
        break;
      case 400:
        $note = t('a parameter!nlfailed validation', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 404:
        $note = t('no such endpoint!nl- OR - no such object', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 405:
        $note = t('endpoint doesn\'t support!nlMETHOD; Allow header', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 406:
        $note = t('endpoint doesn\'t support!nlrequested content type', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 410:
        $note = t('doesn\'t exist any more', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 415:
        $note = t('not application/-!nlx-www-form-urlencoded', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 500:
        $note = t('if RESTmini Service self:!nlbad router cache', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 501:
        $note = t('...yet', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 502:
        $note = t('a background (proxied)!nl service failed', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 503:
        $note = t('if RESTmini Service self:!nlincomplete endpoint', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
      case 504:
        $note = t('request to a proxied!nlservice timed out', array('!nl' => '<br/>'), array('context' => 'module:restmini'));
        break;
    }

    $row = array(
      array(
        'data' => '<b>' . $code . '</b>&emsp;' . RestMini::statusText($code),
        'class' => array(''),
      ),
      array(
        'data' => $note,
        'class' => array('restmini-help-comment', 'restmini-help-border-right'),
      ),
      array(
        'data' => in_array('HEAD', $methods) ? t('no', array(), array('context' => 'module:restmini')) : '',
        'class' => array(in_array('HEAD', $methods) ? 'restmini-help-body-no' : ''),
      ),
      array(
        'data' => in_array('GET', $methods) ?
          ($code == 200 ? t('yes', array(), array('context' => 'module:restmini')) :
            ($code < 400 ? t('no', array(), array('none' => 'module:restmini')) :
              t('optional', array(), array('context' => 'module:restmini')))) : '',
        'class' => array('restmini-help-border-right',
          in_array('GET', $methods) ? ('restmini-help-body-' . ($code == 200 ? 'yes' : ($code < 400 ? 'no' : 'optional'))) : ''),
      ),
      array(
        'data' => in_array('POST', $methods) ?
          ($code <= 201 ? t('yes', array(), array('context' => 'module:restmini')) :
            ($code < 400 ? t('no', array(), array('none' => 'module:restmini')) :
              t('optional', array(), array('context' => 'module:restmini')))) : '',
        'class' => array(in_array('POST', $methods) ? ('restmini-help-body-' . ($code <= 201 ? 'yes' : ($code < 400 ? 'no' : 'optional'))) : ''),
      ),
      array(
        'data' => in_array('PUT', $methods) ?
          ($code <= 201 ? t('yes', array(), array('context' => 'module:restmini')) :
            ($code < 400 ? t('no', array(), array('none' => 'module:restmini')) :
              t('optional', array(), array('context' => 'module:restmini')))) : '',
        'class' => array('restmini-help-border-right',
          in_array('PUT', $methods) ? ('restmini-help-body-' . ($code <= 201 ? 'yes' : ($code < 400 ? 'no' : 'optional'))) : ''),
      ),
      array(
        'data' => in_array('DELETE', $methods) ?
          ($code == 200 ? t('yes', array(), array('context' => 'module:restmini')) :
            ($code < 400 ? t('no', array(), array('none' => 'module:restmini')) :
              t('optional', array(), array('context' => 'module:restmini')))) : '',
        'class' => array(in_array('DELETE', $methods) ? ('restmini-help-body-' . ($code == 200 ? 'yes' : ($code < 400 ? 'no' : 'optional'))) : ''),
      ),
    );

    switch ($code) {
      case 200:
        array_unshift($row, array(
          'data' => t('unconditional!nlsuccess', array('!nl' => '<br/>'), array('context' => 'module:restmini')),
          'rowspan' => $benign,
          'class' => array('restmini-help-odd-even'),
        ));
        break;
      case 400:
        array_unshift($row, array(
          'data' => t('client!nlfailed', array('!nl' => '<br/>'), array('context' => 'module:restmini')),
          'rowspan' => $client,
          'class' => array('restmini-help-fault-client'),
        ));
        break;
      case 500:
        array_unshift($row, array(
          'data' => t('service!nlendpoint!nlfailed', array('!nl' => '<br/>'), array('context' => 'module:restmini')),
          'rowspan' => $server,
          'class' => array('restmini-help-fault-server'),
        ));
        break;
    }

    $table_rows[] = array(
      'data' => $row,
    );
  }

  return theme_table(
    array(
      'header' => $table_header,
      'rows' => $table_rows,
      'attributes' => array(
        'class' => array(
          'restmini',
          'restmini-help',
        ),
      ),
      'caption' => '',
      'colgroups' => array(),
      'sticky' => TRUE,
      'empty' => t(
        'No data',
        array(),
        array('context' => 'module:restmini')
      ),
    )
  );
}
