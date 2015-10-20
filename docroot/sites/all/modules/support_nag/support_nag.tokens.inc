<?php

/**
 * @file
 * Implementation of token module hooks.
 *
 * @ingroup token
 */

/**
 * Implements hook_tokens().
 */
function support_nag_tokens($type, $tokens, $data = array(), $options = array()) {
  $replacements = array();

  // TODO: Handle empty?
  if ($type == 'support-notification' && !empty($data['support-notify'])) {
    $nids = $data['support-notify'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'tickets':
          $tickets  = "Client:               Ticket title:                   State:     Priority:\n";
          $tickets .= "--------------------------------------------------------------------------\n";
          $first = TRUE;
          foreach ($nids as $nid) {
            if ($first) {
              $first = FALSE;
            }
            else {
              $tickets .= "\n";
            }
            $node = node_load($nid);
            $tickets .= _support_nag_truncate(_support_client($node->client), 20, TRUE);
            $tickets .= '  '. _support_nag_truncate($node->title, 30, TRUE);
            $tickets .= '  '. _support_nag_truncate(_support_state($node->state), 9, TRUE);
            $tickets .= '  '. _support_nag_truncate(_support_priorities($node->priority), 9, TRUE);
            $tickets .= "\n". url("node/$node->nid", array('absolute' => TRUE)) ."\n";
          }
          $tickets .= "--------------------------------------------------------------------------";
          $replacements[$original] = $tickets;
          break;

        case 'tickets-per-priority':
          $tickets = '';
          $result = db_query("SELECT n.nid FROM {node} n LEFT JOIN {support_ticket} jt ON n.nid = jt.nid WHERE n.nid IN (:nids) ORDER BY jt.priority DESC, jt.state ASC, jt.client ASC", array(':nids' => $nids));
          $priority = 0;
          foreach ($result as $nid) {
            $node = node_load($nid->nid);
            if ($node->priority != $priority) {
              if ($priority) {
                $tickets .= " --------------------------------------------------------------\n";
              }
              $priority = $node->priority;
              $tickets .= "\nPriority: ". _support_nag_truncate(_support_priorities($node->priority), 10) ."\n\n";
              $tickets .= " Client:              Ticket title:                   State:\n";
              $tickets .= " -----------------------------------------------------------\n";
            }
            else if ($priority) {
              $tickets .= "\n";
            }
            $tickets .= ' '. _support_nag_truncate(_support_client($node->client), 19, TRUE);
            $tickets .= '  '. _support_nag_truncate($node->title, 30, TRUE);
            $tickets .= '  '. _support_nag_truncate(_support_state($node->state), 9, TRUE);
            $tickets .= "\n ". url("node/$node->nid", array('absolute' => TRUE)) ."\n";
          }
          $tickets .= " -----------------------------------------------------------";
          $replacements[$original] = $tickets;
          break;

        case 'tickets-per-state':
          $tickets = '';
          $result = db_query("SELECT n.nid FROM {node} n LEFT JOIN {support_ticket} jt ON n.nid = jt.nid WHERE n.nid IN (:nids) ORDER BY jt.state ASC, jt.priority DESC, jt.client ASC", array(':nids' => $nids));
          $state = 0;
          foreach ($result as $nid) {
            $node = node_load($nid->nid);
            if ($node->state != $state) {
              if ($state) {
                $tickets .= " --------------------------------------------------------------\n";
              }
              $state = $node->state;
              $tickets .= "\nState: ". _support_nag_truncate(_support_state($node->state), 10) ."\n\n";
              $tickets .= " Client:              Ticket title:                   Priority:\n";
              $tickets .= " --------------------------------------------------------------\n";
            }
            else if ($state) {
              $tickets .= "\n";
            }
            $tickets .= ' '. _support_nag_truncate(_support_client($node->client), 19, TRUE);
            $tickets .= '  '. _support_nag_truncate($node->title, 30, TRUE);
            $tickets .= '  '. _support_nag_truncate(_support_priorities($node->priority), 9, TRUE);
            $tickets .= "\n ". url("node/$node->nid", array('absolute' => TRUE)) ."\n";
          }
          $tickets .= " -----------------------------------------------------------";
          $replacements[$original] = $tickets;
          break;

        case 'tickets-per-client':
          $tickets = '';
          $result = db_query("SELECT n.nid FROM {node} n LEFT JOIN {support_ticket} jt ON n.nid = jt.nid WHERE n.nid IN (:nids) ORDER BY jt.client, jt.state ASC, jt.priority DESC", array(':nids' => $nids));
          $client = 0;
          foreach ($result as $nid) {
            $node = node_load($nid->nid);
            if ($node->client != $client) {
              if ($client) {
                $tickets .= " --------------------------------------------------------------\n";
              }
              $client = $node->client;
              $tickets .= "\nClient: ". _support_nag_truncate(_support_client($node->client), 50) ."\n\n";
              $tickets .= " Ticket title:                             State:     Priority:\n";
              $tickets .= " --------------------------------------------------------------\n";
            }
            else if ($client) {
              $tickets .= "\n";
            }
            $tickets .= ' '. _support_nag_truncate($node->title, 40, TRUE);
            $tickets .= '  '. _support_nag_truncate(_support_state($node->state), 9, TRUE);
            $tickets .= '  '. _support_nag_truncate(_support_priorities($node->priority), 9, TRUE);
            $tickets .= "\n ". url("node/$node->nid", array('absolute' => TRUE)) ."\n";
          }
          $tickets .= " --------------------------------------------------------------";
          $replacements[$original] = $tickets;
          break;
      }
    }
  }
  return $replacements;
}

/**
 * Implements hook_token_info().
 */
function support_nag_token_info() {
  return array(
    'types' => array(
      'support-autoclose' => array(
        'name' => t('Support Ticket autoclose'),
        'description' => t(''),
        'needs-data' => 'support-autoclose',
      ),
      'support-notification' => array(
        'name' => t('Support ticket notify'),
        'description' => t(''),
        'needs-data' => 'support-notify',
      ),
    ),
    'tokens' => array(
      'support-autoclose' => array(
        'current-state' => array(
          'name' => t('Current state'),
          'description' => t('Current ticket state'),
        ),
        'closed-state' => array(
          'name' => t('Closed state'),
          'description' => t('Closed ticket state'),
        ),
        'priority' => array(
          'name' => t('Priority'),
          'description' => t('Ticket priority'),
        ),
        'client' => array(
          'name' => t('Client'),
          'description' => t('Client the ticket is assigned to'),
        ),
        'inactive-time' => array(
          'name' => t('Inactive time'),
          'description' => t('How long the ticket has been inactive'),
        ),
        'inactive-limit' => array(
          'name' => t('Inactive limit'),
          'description' => t('Maximum time ticket allowed to be inactive'),
        ),
        'owner-name' => array(
          'name' => t('Owner name'),
          'description' => t('Name of ticket owner'),
        ),
        'owner-email' => array(
          'name' => t('Owner email'),
          'description' => t('Email of ticket owner'),
        ),
      ),
      'support-notification' => array(
        'tickets' => array(
          'name' => t('Tickets'),
          'description' => t('All matching tickets'),
        ),
        'tickets-per-client' => array(
          'name' => t('Tickets per client'),
          'description' => t('All matching tickets, divided by client'),
        ),
        'tickets-per-priority' => array(
          'name' => t('Tickets per priority'),
          'description' => t('All matching tickets, divided by priority'),
        ),
        'tickets-per-state' => array(
          'name' => t('Tickets per state'),
          'description' => t('All matching tickets, divided by state'),
        ),
      ),
    ),
  );
}
