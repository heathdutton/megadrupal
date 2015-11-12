<?php
/**
 * @file
 * XC NCIP provider account functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implementation of xc_auth_method_hook_xc_account()
 */
function xc_auth_method_ncip_provider_xc_account($op, $account, $values, $auth_user, $auth_type) {
  global $user;
  static $cache;

  // Return if current logged in user is not the same as account
  if ($user->uid != $account->uid) {
    xc_log_error('xc_auth', 'Current logged in user is not the same as account.');
    return array();
  }


  // Get user Id from the current session or return
  // TODO: problem: $auth_type->tid is stored in DB independently from the session
  if (!$user_id = $_SESSION['xc_auth_types'][$auth_type->tid]['data']['ncip_provider']['UserIdentifierValue']) {
    xc_log_error('xc_auth', 'There is no user id in the current session for %name.', array('%name' => $auth_type->name));
    return array();
  }

  // NCIP provider Id from the authentication type
  $ncip_provider_id = $auth_type->data['ncip_provider']['ncip_provider_id'];
  $ncip_provider = xc_ncip_provider_load($ncip_provider_id);

  //
  // TODO: Not sure if this is good [yet]
  //
//  // If NCIP provider has already been done for this user in this page load
//  // then don't do it again
//  if ($cache[$account->uid][$ncip_provider_id]) {
//    xc_log_error('xc_auth', 'NCIP provider has already been done for this user in this page load');
//    return array();
//  }
//  else {
//    $cache[$account->uid][$ncip_provider_id] = TRUE;
//  }
  //

  if ((int) $ncip_provider->version == 1) {
    $data = array('Visible User Id');
  }
  elseif ((int) $ncip_provider->version == 2) {
    $data = array('Name Information', 'User Address Information');
  }
  else {
    $data = array();
  }

  // TODO: check $values, something not fair here
  $auth = array(
    'user' => (isset($values['user']) ? $values['user'] : ''),
    'pass' => (isset($values['pass']) ? $values['pass'] : ''),
    'type' => $auth_type->data['ncip_provider']['ncip_provider_auth_type'],
  );

  $results = xc_ncip_provider_lookup_user($ncip_provider_id, $user_id, $data, TRUE, TRUE, TRUE, $auth);
  if (isset($results['error']) || !isset($results['results'])) {
    // A problem has occurred
    xc_log_error('xc_auth', 'Problem occured during NCIP account call: %error', array(
      '%error' => (isset($results['error']) ? $results['error'] : '')));
    return array();
  }
  else {
    switch ($op) {
      case 'personal info':
        // Name
        if (isset($results['results'][$user_id]['FullName'])) {
          $personal_info['name'] = $results['results'][$user_id]['FullName'];
        }

        // Address
        if (is_array($results['results'][$user_id]['Address']['Physical'])) {
          foreach ($results['results'][$user_id]['Address']['Physical'] as $physical_address) {
            $personal_info['address'][] = array(
              'type' => $physical_address['Type'],
              'address' => is_array($physical_address['Address'])
                        ? theme('xc_ncip_provider_structured_physical_address', array('0' => $physical_address['Address']))
                        : $physical_address['Address'],
            );
          }
        }

        // Email
        if (is_array($results['results'][$user_id]['Address']['Electronic'])) {
          foreach ($results['results'][$user_id]['Address']['Electronic'] as $email) {
            $personal_info['email'][] = array(
              'type' => $email['Type'],
              'email' => $email['Address'],
            );
          }
        }

        // Totals -- loaned and requested items and account balance
        if (is_array($results['results'][$user_id]['LoanedItems'])) {
          $personal_info['num_loaned_items'] = count($results['results'][$user_id]['LoanedItems']);
        }
        if (is_array($results['results'][$user_id]['RequestedItems'])) {
          $personal_info['num_requested_items'] = count($results['results'][$user_id]['RequestedItems']);
        }
        if (isset($results['results'][$user_id]['FiscalAccount']['Balance'])
          && is_array($results['results'][$user_id]['FiscalAccount']['Balance'])) {
          $personal_info['account_balance'] = xc_ncip_provider_display_amount(
            $results['results'][$user_id]['FiscalAccount']['Balance']['CurrencyCode'],
            $results['results'][$user_id]['FiscalAccount']['Balance']['MonetaryValue']
          );
        }
        return $personal_info;

      case 'loaned items':
        $items = array();
        if (isset($results['results'][$user_id]['LoanedItems'])
          && is_array($results['results'][$user_id]['LoanedItems'])) {
          $item_ids = array();
          $bib_ids = array();
          foreach ($results['results'][$user_id]['LoanedItems'] as $item) {
            $item_ids[] = $item['ItemId'];
            $bib_ids[] = $item['BibId'];
          }

          switch ($ncip_provider->version) {
            case 1:
            case 1.01:
              $availability = xc_ncip_provider_xc_get_availability($ncip_provider_id, array('item' => $item_ids));
              break;
            case 2:
            case 2.01:
            default:
              $availability = xc_ncip_provider_lookup_item_set($ncip_provider_id, array('bib' => $bib_ids), array('Circulation Status', 'Location', 'Bibliographic Description'));
          }

          foreach ($results['results'][$user_id]['LoanedItems'] as $item) {
            // Item ID
            if (isset($availability['results'])) {
              if ($item['ItemId']) {
                $item_item_id = $item['ItemId'];
              }
              else if ($item['BibId']) {
                $item_item_id = $availability['results']['bib'][$item['BibId']][0]['ItemId'];
              }
            }
            else {
              $item_item_id = NULL;
            }

            // Bibliographic ID
            if (isset($availability['results'])) {
              if ($item['ItemId']) {
                $item_bib_id = $availability['results']['item'][$item['ItemId']][0]['BibId'];
              }
              else if ($item['BibId']) {
                $item_bib_id = $item['BibId'];
              }
            }
            else {
              $item_bib_id = NULL;
            }

            // Bibliographic Description
            if (isset($availability['results'])) {
              if ($item['ItemId']) {
                $item_bib_desc = $availability['results']['item'][$item['ItemId']][0]['BibliographicDescription'];
              }
              else if ($item['BibID']) {
                $item_bib_desc = $availability['results']['bib'][$item['BibId']][0]['BibliographicDescription'];
              }
            }
            else {
              $item_bib_desc = NULL;
            }

            $items[] = array(
              'type' => 'ncip_provider',
              'ncip_provider_id' => $ncip_provider_id,
              'item_id' => $item_item_id,
              'bib_id' => $item_bib_id,
              'bib_desc' => $item_bib_desc,
              'date_due' => @strtotime($item['DateDue']),
              'renew' => array(
                'path' => 'admin/xc/ncip/provider/' . $ncip_provider_id . '/renew/' . $user_id . '/' . $item['ItemId'],
                'options' => array('query' => 'destination=' . $_GET['q']),
              ),
              'title' => isset($item['Title']) ? $item['Title'] : NULL,
            );
          }
        }
        return $items;

      case 'requested items':
        $items = array();
        if (is_array($results['results'][$user_id]['RequestedItems'])) {
          $item_ids = array();
          $bib_ids = array();
          foreach ($results['results'][$user_id]['RequestedItems'] as $item) {
            $item_ids[] = $item['ItemId'];
            $bib_ids[] = $item['BibId'];
          }

          switch ($ncip_provider->version) {
            case 1:
            case 1.01:
              $availability = xc_ncip_provider_xc_get_availability($ncip_provider_id, array('item' => $item_ids));
              break;
            case 2:
            case 2.01:
            default:
              $availability = xc_ncip_provider_lookup_item_set($ncip_provider_id, array('bib' => $bib_ids), array('Circulation Status', 'Location', 'Bibliographic Description'));
          }

          foreach ($results['results'][$user_id]['RequestedItems'] as $item) {
            // Item ID
            if (isset($availability['results'])) {
              if ($item['ItemId']) {
                $item_item_id = $item['ItemId'];
              }
              else if ($item['BibId']) {
                $item_item_id = $availability['results']['bib'][$item['BibId']][0]['ItemId'];
              }
            }
            else {
              $item_item_id = NULL;
            }

            // Bibliographic ID
            if (isset($availability['results'])) {
              if ($item['ItemId']) {
                $item_bib_id = $availability['results']['item'][$item['ItemId']][0]['BibId'];
              }
              else if ($item['BibId']) {
                $item_bib_id = $item['BibId'];
              }
            }
            else {
              $item_bib_id = NULL;
            }

            // Bibliographic Description
            if (isset($availability['results'])) {
              if ($item['ItemId']) {
                $item_bib_desc = $availability['results']['item'][$item['ItemId']][0]['BibliographicDescription'];
              }
              else if ($item['BibID']) {
                $item_bib_desc = $availability['results']['bib'][$item['BibId']][0]['BibliographicDescription'];
              }
            }
            else {
              $item_bib_desc = NULL;
            }

            $items[] = array(
              'type' => 'ncip_provider',
              'ncip_provider_id' => $ncip_provider_id,
              'item_id' => $item_item_id,
              'bib_id' => $item_bib_id,
              'bib_desc' => $item_bib_desc,
              'date_due' => @strtotime($item['DateDue']),
              'date_placed' => @strtotime($item['DatePlaced']),
              'title' => isset($item['Title']) ? $item['Title'] : NULL,
            );
          }
        }
        return $items;
    }
  }
}

/**
 * Implements hook_xc_login().
 */
function xc_ncip_provider_xc_login($auth, $values, &$data) {
  // Will be changed to call LookupUser and telling it to authenticate
  switch ($auth->method) {
    case 'ncip_provider':
      $auth_values = array(
        'user' => $values['user'],
        'pass' => $values['pass'],
        'type' => $auth->data['ncip_provider']['ncip_provider_auth_type'],
      );
      $result = xc_ncip_provider_lookup_user($auth->data['ncip_provider']['ncip_provider_id'], NULL, array(), FALSE, FALSE, TRUE, $auth_values);
      if (isset($result['auth'])) {
        if ($user_id = $result['auth']['user_id']) {
          // The user is authenticated, now keep track of user identifier number
          $data['ncip_provider']['UserIdentifierValue'] = $user_id;
          return XC_AUTH_LOGIN_SUCCESS;
        }
        else {
          // Authentication was attempted, but not successful
          return XC_AUTH_LOGIN_INCORRECT;
        }
      }
      else {
        // A problem occurred and authentication could not even be attempted
        // successfully
        return XC_AUTH_LOGIN_FAILURE;
      }
      break;
  }
}

/**
 * Implements hook_xc_logout().
 */
function xc_ncip_provider_xc_logout($auth, $data) {
  switch ($auth->method) {
    case 'ncip_provider':
      // Close the NCIP connection... a new connection will be made
      // automatically by the NCIP provider module, when needed
      return xc_ncip_provider_disconnect($auth->data['ncip_provider']['ncip_provider_id']);
      break;
  }
}

