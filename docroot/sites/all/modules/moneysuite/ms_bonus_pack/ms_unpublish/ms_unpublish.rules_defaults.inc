<?php

/**
 * Sets some default rules.
 */
function ms_unpublish_rules_defaults() {
  $config = array(
    'rules' => array(
      'rules_3' => array(
        '#type' => 'rule',
        '#set' => 'event_ms_unpublish_event_pay_unpublish',
        '#label' => 'Unpublish Node after Payment and Send Email',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          0 => 'Pay to Unpublish',
        ),
        '#status' => 'custom',
        '#conditions' => array(
        ),
        '#actions' => array(
          0 => array(
            '#type' => 'action',
            '#settings' => array(
              'auto_save' => 1,
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_core_node_unpublish_action',
            '#info' => array(
              'label' => 'Unpublish Node',
              'module' => 'Node',
              'arguments' => array(
                'node' => array(
                  'label' => 'Content',
                  'type' => 'node',
                ),
              ),
              'base' => 'rules_core_action_execute',
              'action_name' => 'node_unpublish_action',
              'configurable' => false,
              'label callback' => 'rules_core_node_label_callback',
              'label_skeleton' => 'Unpublish @node',
            ),
            '#weight' => 0,
          ),
          1 => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Log to watchdog',
              'module' => 'System',
              'eval input' => array(
                0 => 'type',
                1 => 'message',
                2 => 'link',
              ),
            ),
            '#name' => 'rules_action_watchdog',
            '#settings' => array(
              'severity' => '5',
              'type' => 'ms_unpublish',
              'message' => 'Unpublished [node:title] because [account:user] paid to have it unpublished.',
              'link' => '[node:node-url]',
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'message' => array(
                    0 => 'account',
                    1 => 'node',
                  ),
                  'link' => array(
                    0 => 'node',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
          2 => array(
            '#type' => 'action',
            '#settings' => array(
              '#argument map' => array(
                'node' => 'node',
                'author' => 'author',
              ),
            ),
            '#name' => 'rules_action_node_load_author',
            '#info' => array(
              'label' => 'Load the Node author',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'new variables' => array(
                'author' => array(
                  'label' => 'node author',
                  'type' => 'user',
                  'label callback' => 'rules_action_node_load_author_variable_label',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
          3 => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Send a mail to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'Recipient',
                ),
              ),
              'module' => 'System',
              'eval input' => array(
                0 => 'subject',
                1 => 'message',
                2 => 'from',
              ),
            ),
            '#name' => 'rules_action_mail_to_user',
            '#settings' => array(
              'from' => '',
              'subject' => 'Node Unpublished',
              'message' => 'The content \'[node:title]\' has been paid to be unpublished by [account:user].',
              '#argument map' => array(
                'user' => 'author',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'message' => array(
                    0 => 'account',
                    1 => 'node',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );

  return $config;
}
