<?php

/**
 * @file
 * Security Group UI Functions
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Updated by yas   2011/02/14
 * Updated by yas   2011/02/10
 * Updated by yas   2011/02/06
 * Updated by yas   2011/02/02
 */

/**
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_security_group($form_submit = '', $form_state, $cloud_context) {

  $group_name = isset( $_REQUEST['group_name'] ) ? $_REQUEST['group_name'] : FALSE;

  $cloud_menu_path = cloud_get_menu_path( $cloud_context );
  if (empty($group_name) || strlen($group_name) == 0  ) {

    drupal_goto( $cloud_menu_path . '/security_groups' );
    return;
  }

  $txt_disabled = '';
  
  if (user_access($cloud_context . ' setup security group') === FALSE ) {
    $txt_disabled = TRUE;
  }

  $operation_type = cloud_get_filter_value( $form_state, 'AddIPs_TCP_Button' );
  if ( empty($operation_type) ) {

    $operation_type = cloud_get_filter_value( $form_state, 'AddIPs_ICMP_Button' );
    if ( empty($operation_type) ) {

      $operation_type = cloud_get_filter_value( $form_state, 'Add_Grp_Button' );
      if ( empty($operation_type) ) {

        $operation_type = FALSE;
      }
      else {

        $operation_type = 'Add_Grp_Button';
      }
    }
    else {

      $operation_type = 'AddIPs_ICMP_Button';
    }
  }
  else {

    $operation_type = 'AddIPs_TCP_Button';
  }
  

  $form['operation_type'] = array(
    '#type' => 'hidden',
    '#value' => $operation_type,
  );
  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $result = _aws_cloud_get_security_group_query($cloud_context, $group_name);
  $owner_id = '';

  $spacer = array(
    '#type' => 'image_button',
    '#src' => cloud_get_module_base() . '/images/spacer.gif',
    '#attributes' => array(
      'width' => 300,
      'height' => 1,
    ),
  );

  foreach ($result as $key) {

    $form['group_name'] = array(
      '#type' => 'hidden'  ,
      '#name' => 'group_name',
      '#value' => t( $group_name),
    );
    $form['fieldset_SG'] = array(
      '#type' => 'fieldset',
      '#title' => t('Security Group'),
    );

    $form['SG_details']['security_group_label'] = array(
      '#type' => 'item',
      '#title' => t('Security Group'),
    );
    $form['SG_details']['security_group_text'] = array('#markup' => check_plain( $group_name));
    $form['SG_details']['security_group_blank'] = $spacer;

    $form['SG_details']['security_group_id_label'] = array(
      '#type' => 'item',
      '#title' => t('Security Group Id'),
    );
    $form['SG_details']['security_group_id_text'] = array('#markup' => check_plain( $key->group_id));
    $form['SG_details']['security_group_id_blank'] = $spacer;

    $form['SG_details']['description_label'   ] = array(
      '#type' => 'item',
      '#title' => t('Description'),
    );
    $form['SG_details']['description_text'    ] = array('#markup' => check_plain( $key->group_description ));
    $form['SG_details']['description_blank'   ] = $spacer;

    $form['SG_details']['owner_label'         ] = array(
      '#type' => 'item',
      '#title' => t('Owner'),
    );
    $form['SG_details']['owner_id'            ] = array('#markup' => t( $key->owner_id ));
    $form['SG_details']['owner_blank'         ] = $spacer;

    $owner_id = $key->owner_id;
    $security_group_details = _aws_cloud_get_security_groups_details_db( $cloud_context, $group_name );

    $cnt = 0;
    $tmp_table_rows = array();
    $rows = array();

    if (isset($security_group_details[$group_name]) ) {

      $sg_details_list = $security_group_details[$group_name];
      $form['fieldset_permissions'] = array(
        '#type' => 'fieldset',
        '#title' => t('Permissions'),
      );

      foreach ($sg_details_list as $sg_details) {

        $tmp_table_rows = array();
        $form['sg_details_data']['sg_details_' . $cnt]          = array();
        $form['sg_details_data']['sg_details_' . $cnt]          = array();
        $form['sg_details_data']['sg_details_' . $cnt]['label'] = array(
          '#type' => 'item',
          '#title' => t('Allow'),
        );

        $permissions = drupal_strtoupper( $sg_details['ip_protocol'] ) . ' ';

        if ( empty($sg_details['cidr_ip']) === FALSE ) {

          $permissions .= 'IPs: ' . $sg_details['cidr_ip'] . ' ';
        }

        if ( $sg_details['ip_protocol'] !== 'icmp' ) {

          if ($sg_details['from_port'] === $sg_details['to_port'] ) {

            $permissions .= 'Port: ' . $sg_details['from_port'] . ' ';
          }
          else {

            $permissions .= 'Ports: ' . $sg_details['from_port'] . ' .. ' . $sg_details['to_port'] . ' ';
          }
        }
        else { // If protocol is ICMP
          if ($sg_details['from_port'] === $sg_details['to_port'] && $sg_details['to_port'] === '-1') {

            $permissions .= 'Types: Any' . ' ';
          }
          else {

            $permissions .= 'Type: ' . $sg_details['from_port'] . ', Code: ' . $sg_details['to_port'] . ' ';
          }
        }

        if ( empty($sg_details['cidr_ip']) ) { // for group
          $permissions .= 'Owner: ' . $sg_details['user_id'] . ' Group: ' . $sg_details['tgt_group_name'] . ' ';
        }

        $form['sg_details_data']['sg_details_' . $cnt]['content'] = array(
          '#type' => 'item',
          '#markup' => $permissions,
        );
        if ( empty($txt_disabled) ) {

          $form['sg_details_data']['sg_details_' . $cnt]['button'] = array(
            array(
              '#markup' => l( t('Revoke'),
                  $cloud_menu_path . '/security_groups/revoke',
                  array(
                'query' => array(
                  'group_name' => urlencode($group_name),
                  'ip_protocol' => $sg_details['ip_protocol'   ],
                  'from_port' => $sg_details['from_port'     ],
                  'to_port' => $sg_details['to_port'       ],
                  'user_id' => $sg_details['user_id'       ],
                  'tgt_group_name' => $sg_details['tgt_group_name'],
                  'cidr_ip' => $sg_details['cidr_ip'       ],
                ),
              )
                ),
            ),
          );
        }
        $cnt++;
      } // end of foreach
    } // end of if
  } // end of while


  $form['fieldset_add_ips'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add IPs'),
  );

  // Add TCp/UDP details
  $form['SG_details']['AddIPs_TCP_label'] = array(
    '#type' => 'item',
    '#title' => t('TCP / UDP'),
    '#description' => t('Use 0.0.0.0/0 to designate "Any" IP address;<br />Use Ports 0 .. 65535 to designate "Any" Port'),
  );

  $tcp_ips = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $protocol_options = array(
    'tcp' => 'TCP',
    'udp' => 'UDP',
  );

  asort($protocol_options);
  $tcp_ips['protocol'       ] = array(
    '#type' => 'select'   ,
    '#options' => $protocol_options               ,
    '#default_value' => 'tcp'        ,
    '#disabled' => $txt_disabled,
  );
  $tcp_ips['tcp_IPs'        ] = array(
    '#type' => 'textfield',
    '#size' => '14',
    '#title' => t('IPs'  ),
    '#default_value' => '0.0.0.0/32',
    '#disabled' => $txt_disabled,
  );
  $tcp_ips['tcp_Ports_start'] = array(
    '#type' => 'textfield',
    '#size' => '4',
    '#title' => t('Ports'),
    '#default_value' => '0'          ,
    '#disabled' => $txt_disabled,
  );
  $tcp_ips['tcp_Ports_end'  ] = array(
    '#type' => 'textfield',
    '#size' => '4',
    '#field_prefix' => ' .. '    ,
    '#default_value' => '0'          ,
    '#disabled' => $txt_disabled,
  );

  $form['SG_details']['AddIPs_TCP_Content']   = $tcp_ips  ;

  if (user_access($cloud_context . ' setup security group') ) {
    $form['SG_details']['AddIPs_TCP_Button']   = array(
      '#type' => 'submit',
      '#name' => 'AddIPs_TCP_Button',
      '#value' => t('Add'),
    );
  }
  else {
    $form['SG_details']['AddIPs_TCP_Button']   = array(
      '#type' => 'item',
      '#markup' => '',
    );
  }

  // Add ICMP details

  $form['SG_details']['AddIPs_ICMP_label'] = array(
    '#type' => 'item',
    '#title' => t('ICMP'),
    '#description' => t('Use 0.0.0.0/0 to designate "Any" IP address;<br />Use Type: -1 and Code: -1 to designate "Any" Type/Code'),
  );

  $icmp_ips = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $icmp_ips['icmp_IPs'        ] = array(
    '#type' => 'textfield',
    '#size' => '14',
    '#title' => t('IPs' ),
    '#default_value' => '0.0.0.0/32',
    '#disabled' => $txt_disabled,
  );
  $icmp_ips['icmp_Ports_start'] = array(
    '#type' => 'textfield',
    '#size' => '4',
    '#title' => t('Type'),
    '#default_value' => '-1'         ,
    '#disabled' => $txt_disabled,
  );
  $icmp_ips['icmp_Ports_end'  ] = array(
    '#type' => 'textfield',
    '#size' => '4',
    '#title' => t('Code'),
    '#default_value' => '-1'         ,
    '#disabled' => $txt_disabled,
  );

  $form['SG_details']['AddIPs_ICMP_Content']   = $icmp_ips  ;

  if (user_access($cloud_context . ' setup security group') ) {
    $form['SG_details']['AddIPs_ICMP_Button'] = array(
      '#type' => 'submit',
      '#name' => 'AddIPs_ICMP_Button',
      '#value' => t('Add'),
    );
  }
  else {
    $form['SG_details']['AddIPs_ICMP_Button'] = array(
      '#type' => 'item',
      '#markup' => '',
    );
  }

  // Add Group details
  $form['SG_details']['AddIPs_Grp_label'    ] = array(
    '#type' => 'item',
    '#title' => t('Add Group'),
  );

  $grp = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $grp['grp_owner'] = array(
    '#type' => 'textfield',
    '#size' => '24',
    '#title' => t('Owner')                                ,
    '#disabled' => $txt_disabled,
  );
  $grp['grp'      ] = array(
    '#type' => 'textfield',
    '#size' => '24',
    '#title' => t('Group'),
    '#default_value' => 'default',
    '#disabled' => $txt_disabled,
  );

  $grp['grp_owner']['#default_value'] = ($owner_id)
                                      ?  $owner_id
                                      : '';

  $grp_protocol = !empty($form_state['values']['grp_protocol']) ? $form_state['values']['grp_protocol'] : 'tcp';
  // add in select for better user experience
  $grp['grp_protocol'] = array(
    '#type' => 'select',
    '#options' => array('tcp' => t('TCP'), 'udp' => t('UDP'), 'icmp' => t('ICMP')),
    '#disabled' => $txt_disabled,
    '#default_value' => $grp_protocol,
    '#ajax' => array(
      'callback' => 'aws_cloud_group_select_callback',
      'wrapper' => 'group-configuration',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  
  $grp['grp_wrapper'] = array(
    '#prefix' => '<div id="group-configuration">',
    '#suffix' => '</div>',
  );
  if ($grp_protocol == 'icmp') {
    $grp['grp_wrapper']['config']['grp_icmp_IPs'] = array(
      '#type' => 'textfield',
      '#size' => '14',
      '#title' => t('IPs'),
      '#default_value' => !empty($form_state['values']['grp_icmp_IPs']) ? $form_state['values']['grp_icmp_IPs'] : '0.0.0.0/32',
    );
    $grp['grp_wrapper']['config']['grp_icmp_Ports_start'] = array(
      '#type' => 'textfield',
      '#size' => '4',
      '#title' => t('Type'),
      '#default_value' => !empty($form_state['values']['grp_icmp_Ports_start']) ? $form_state['values']['grp_icmp_Ports_start'] : '-1',
    );
    $grp['grp_wrapper']['config']['grp_icmp_Ports_end'] = array(
      '#type' => 'textfield',
      '#size' => '4',
      '#title' => t('Code'),
      '#default_value' => !empty($form_state['values']['grp_icmp_Ports_end']) ? $form_state['values']['grp_icmp_Ports_end'] : '-1',
    );
  }
  else {
    $grp['grp_wrapper']['config']['grp_ips'] = array(
      '#type' => 'textfield',
      '#size' => '14',
      '#title' => t('IPs'),
      '#default_value' => !empty($form_state['values']['grp_ips']) ? $form_state['values']['grp_ips'] : '0.0.0.0/32',
      '#disabled' => $txt_disabled,
    );
    $grp['grp_wrapper']['config']['grp_tcp_Ports_start'] = array(
      '#type' => 'textfield',
      '#size' => '4',
      '#title' => t('Ports'),
      '#default_value' => !empty($form_state['values']['grp_tcp_Ports_start']) ? $form_state['values']['grp_tcp_Ports_start'] : '0',
      '#disabled' => $txt_disabled,
    );
    $grp['grp_wrapper']['config']['grp_tcp_Ports_end'  ] = array(
      '#type' => 'textfield',
      '#size' => '4',
      '#field_prefix' => ' .. '    ,
      '#default_value' => !empty($form_state['values']['grp_tcp_Ports_end']) ? $form_state['values']['grp_tcp_Ports_end'] : '0',
      '#disabled' => $txt_disabled,
    );
  }

  $form['SG_details']['AddIPs_Grp_Content']   = $grp  ;

  if (user_access($cloud_context . ' setup security group') ) {
    $form['SG_details']['Add_Grp_Button'] = array(
      '#type' => 'submit',
      '#name' => 'Add_Grp_Button',
      '#value' => t('Add'),
    );
  }
  else {
    $form['SG_details']['Add_Grp_Button'] = array(
      '#type' => 'item',
      '#markup' => '',
    );
  }

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  if (user_access($cloud_context . ' delete security group') ) {

    $location    = url($cloud_menu_path . '/security_groups/delete') . '&sg_name=' . urlencode($group_name);
    $confirm_msg = cloud_get_messagebox(t('Are you sure you want to delete the Security Group "@group_name" ?', array('@group_name' => $group_name)));
    $form['submit_buttons']['Delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#attributes' => array('onclick' => $confirm_msg),
    );
  }

  $form['submit_buttons']['SG_List'] = array(
    '#type' => 'submit',
    '#value' => t('List Security Groups'),
  );

  return $form;
}



/**
 * Theme security group form
 * @param     $form
 * @return
 */
function theme_aws_cloud_display_security_group($form) {

  $form = $form['form'];

  $rows = array(
    array(
      drupal_render($form['SG_details']['security_group_label']),
      drupal_render($form['SG_details']['security_group_text']),
      drupal_render($form['SG_details']['security_group_blank']),
    ),
    array(
      drupal_render($form['SG_details']['security_group_id_label']),
      drupal_render($form['SG_details']['security_group_id_text']),
      drupal_render($form['SG_details']['security_group_id_blank']),
    ),
    array(
      drupal_render($form['SG_details']['description_label'   ]),
      drupal_render($form['SG_details']['description_text'    ]),
      drupal_render($form['SG_details']['description_blank'   ]),
    ),
    array(
      drupal_render($form['SG_details']['owner_label'         ]),
      drupal_render($form['SG_details']['owner_id'            ]),
      drupal_render($form['SG_details']['owner_blank'         ]),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_SG']['#children'] = $table;

  //cf. Waning by Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
  $form['fieldset_SG']['list'] = array(
    '#type' => 'markup',
    '#value' => $table,
  );

  if (isset($form['sg_details_data']) ) {

    $cnt = 0;
    $rows = array();

    foreach ($form['sg_details_data'] as $sg_key => $details_data) {

      if (strpos($sg_key, 'sg_details' ) === FALSE ) {

        continue;
      }

      $rows[] = array(
        drupal_render($form['sg_details_data']['sg_details_' . $cnt]['label'   ]),
        drupal_render($form['sg_details_data']['sg_details_' . $cnt]['content']),
        drupal_render($form['sg_details_data']['sg_details_' . $cnt]['button'  ]),
      );
      $cnt++;
    }
    $table = theme('table', array('header' => NULL, 'rows' => $rows));
    $form['fieldset_permissions']['#children'] = $table;

    // cf. Waning by Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
    // $form['fieldset_permissions']['list'] = array('#type' => 'markup', '#value' => $table);
  }

  $rows = array(
    array(
      drupal_render($form['SG_details']['AddIPs_TCP_label'   ]),
      drupal_render($form['SG_details']['AddIPs_TCP_Content']),
      drupal_render($form['SG_details']['AddIPs_TCP_Button'  ]),
    ),
    array(
      drupal_render($form['SG_details']['AddIPs_ICMP_label'  ]),
      drupal_render($form['SG_details']['AddIPs_ICMP_Content']),
      drupal_render($form['SG_details']['AddIPs_ICMP_Button']),
    ),
    array(
      drupal_render($form['SG_details']['AddIPs_Grp_label'   ]),
      drupal_render($form['SG_details']['AddIPs_Grp_Content']),
      drupal_render($form['SG_details']['Add_Grp_Button'     ]),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_add_ips']['#children'] = $table;

  //cf. Waning by Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
  //$form['fieldset_add_ips']['list'] = array('#type' => 'markup', '#value' => $table);

  $output  = drupal_render($form['cloud_context'       ]);
  $output .= drupal_render($form['operation_type'      ]);
  $output .= drupal_render($form['submit_buttons'      ]);
  $output .= drupal_render($form['fieldset_SG'         ]);
  $output .= drupal_render($form['fieldset_permissions']);
  $output .= drupal_render($form['fieldset_add_ips'    ]);

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Ajax callback to set the appropriate fields for the "Add Group"
 */
function aws_cloud_group_select_callback($form, $form_state) {
  return $form['SG_details']['AddIPs_Grp_Content']['grp_wrapper'];
}

/**
 * Submit function to update security groups
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_display_security_group_submit($form_id, &$form_state) {

  $form_values   = $form_state['values'       ];
  $cloud_context = $form_values['cloud_context'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  $result = TRUE;
  
  if ($form_state['triggering_element']['#name'] == 'AddIPs_TCP_Button') {
    // User Activity Log
    cloud_audit_user_activity( 
      array(
        'type' => 'user_activity',
        'message' => t('TCP IP(s) have been added: @TCP_IPs', array('@TCP_IPs' => $form_values['tcp_IPs'])),
        'link' => '',
      )
    );
    $result = aws_cloud_authorize_security_group($cloud_context, $form_values['group_name'], '', '', $form_values['protocol'], $form_values['tcp_Ports_start'], $form_values['tcp_Ports_end'], $form_values['tcp_IPs']);

  }
  elseif ($form_state['triggering_element']['#name'] == 'AddIPs_ICMP_Button') {
    // User Activity Log
    cloud_audit_user_activity( 
      array(
        'type' => 'user_activity',
        'message' => t('ICMP IP(s) have been added: @ICMP_IP', array('@ICMP_IP' => $form_values['icmp_IPs'])),
        'link' => '',
      )
    );

    $result = aws_cloud_authorize_security_group($cloud_context, $form_values['group_name'], '', '', 'icmp', $form_values['icmp_Ports_start'], $form_values['icmp_Ports_end'], $form_values['icmp_IPs']);
  }
  elseif ($form_state['triggering_element']['#name'] == 'Add_Grp_Button') {
    // Takes the values (IP & Port Range) from the AddIPs_TCP
    if ($form_values['grp_protocol'] == 'icmp') {
      $result = aws_cloud_authorize_security_group($cloud_context, $form_values['group_name'], $form_values['grp'], $form_values['grp_owner'], 'icmp', $form_values['grp_icmp_Ports_start'], $form_values['grp_icmp_Ports_end'], $form_values['grp_icmp_IPs']);
    }
    else {
      $result = aws_cloud_authorize_security_group($cloud_context, $form_values['group_name'], $form_values['grp'], $form_values['grp_owner'], $form_values['grp_protocol'], $form_values['grp_tcp_Ports_start'], $form_values['grp_tcp_Ports_end'], $form_values['grp_ips']);
    }
  }
  elseif ($form_values['op'] == t('List Security Groups')) {
    $form_state['redirect'] = $cloud_menu_path . '/security_groups';
    return;
  }
  elseif ($form_values['op'] == t('Delete')) {
    cloud_audit_user_activity(
      array(
        'type' => 'user_activity',
        'message' => t('@group has been deleted.', array('@group' => $form_values['group_name'])),
        'link' => '',
      )
    );
    $result = _aws_cloud_delete_security_group($cloud_context, $form_values['group_name']);
    if ($result) {
      drupal_set_message( t('Security Group deleted successfully: @sg_name', array('@sg_name' => $form_values['group_name'])));
    }
    else {
      drupal_set_message( check_plain( t('Unable to delete the Security Group: @sg_name', array('@sg_name' => $form_values['group_name']))), 'error');
    }
    $form_state['redirect'] = $cloud_menu_path . '/security_groups';
    return;
  }
  
  if ($result) {
    drupal_set_message(t('Updated: Security Group(s)') );
  }
  else {
    drupal_set_message(t('Error while updating Security Group'), 'error');
  }
  $form_state['redirect'] = url($cloud_menu_path . '/security_groups/describe', array('query' => array('group_name' => $form_values['group_name']), 'absolute' => TRUE));
}


/**
 *  Create Security Group
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_create_security_group($form_submit, $form_state = '', $cloud_context) {

  $form['fieldset_security_group_info'        ] = array('#type' => 'fieldset' /* '#title' => t('Security Group Info') */);

  $form['cloud_context'    ] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $form['GroupName_details']['GroupName_label'] = array(
    '#type' => 'item',
    '#title' => t('Security Group name'),
    '#required' => TRUE,
  );
  $form['GroupName_details']['GroupName_text'] = array('#type' => 'textfield');

  $form['GroupName_details']['GroupDesc_label'] = array(
    '#type' => 'item',
    '#title' => t('Security Group Description'),
    '#required' => TRUE,
  );
  $form['GroupName_details']['GroupDesc_text'] = array('#type' => 'textfield');

  $form['submit_buttons'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<span class="clear"></span><div class="container-inline"><div class="buttons">',
    '#suffix' => '</div></div>',
  );

  $form['submit_buttons']['Create'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  $form['submit_buttons']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}


/**
 *
 * @param     $form
 * @return
 */
function theme_aws_cloud_create_security_group($form) {


  if (isset($form['form'])) { // drupal 7 passed form as an array
    $form = $form['form'];
  }

  $rows = array(
    array(
      drupal_render($form['GroupName_details']['GroupName_label']),
      drupal_render($form['GroupName_details']['GroupName_text']),
    ),
    array(
      drupal_render($form['GroupName_details']['GroupDesc_label']),
      drupal_render($form['GroupName_details']['GroupDesc_text']),
    ),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $rows));
  $form['fieldset_security_group_info']['#children'] = $table;

  //cf. Waning by Coder module: Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use check_plain(), filter_xss() or similar to ensure your $variable is fully sanitized.
  //$form['fieldset_security_group_info']['list'] = array('#type' => 'markup', '#value' => $table);

  $output  = drupal_render($form['fieldset_security_group_info']);
  $output .= drupal_render($form['submit_buttons']);
  $output .= drupal_render($form['cloud_context']);

  $output .= drupal_render_children($form);

  return $output;
}


/**
 *
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_create_security_group_validate($form_id, $form_values) {
  $form_values = $form_values['values'];
  if ($form_values['op'] == t('Create') ) {
    $sg_name = trim( $form_values['GroupName_text'] );
    if (empty($sg_name) || empty($form_values['GroupDesc_text'])) {
      form_set_error('GroupDesc_text', t('You must enter valid Group Name & description'));
    }
    // check to make sure the group doesn't exist
    $result = _aws_cloud_get_security_group_query($form_values['cloud_context'], $sg_name);
    if ($result->rowCount()) {
      form_set_error('GroupName_text', t('Security Group already exists'));
    }
  }
}


/**
 * Form submit for security groups
 * @param     $form_id
 * @param     $form_values
 * @return
 */
function aws_cloud_create_security_group_submit($form_id, &$form_state) {

  $form_values     = $form_state['values'];
  $cloud_context   = $form_values['cloud_context'];
  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  
  if ($form_values['op'] == t('Create')) {
    $sg_name  = trim($form_values['GroupName_text']);
    $sg_desc  = trim($form_values['GroupDesc_text']);
    $result= _aws_cloud_create_security_group($cloud_context, $sg_name, $sg_desc);
    if ($result) {
      drupal_set_message(t('Created a Security Group Successfully.'));
      $form_state['redirect'] = url($cloud_menu_path . '/security_groups/describe', array('query' => array('group_name' => $sg_name), 'absolute' => TRUE));
    }
    else {
      drupal_set_message(t('Unable to create Security Group'), 'error');
    }
  }
  $form_state['redirect'] =  $cloud_menu_path . '/security_groups';
}


/**
 * Delete Security Group
 *
 * @param     $cloud_context
 * @return
 */
function aws_cloud_delete_security_group($cloud_context) {
  $sg_name = isset( $_REQUEST['sg_name'] ) ? $_REQUEST['sg_name'] : FALSE;
  $result = _aws_cloud_delete_security_group($cloud_context, $sg_name);
  if ($result) {
    drupal_set_message( t('Security Group deleted successfully: @sg_name', array('@sg_name' => $sg_name) ) );
  }
  else {
    drupal_set_message( check_plain( t('Unable to delete the Security Group: @sg_name', array('@sg_name' => $sg_name) ) ), 'error' );
  }
  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  drupal_goto($cloud_menu_path . '/security_groups');
}



/**
 * Revoke Security Group
 *
 * @param     $cloud_context
 * @return
 */
function aws_cloud_revoke_security_group($cloud_context) {

  $sg_name      = isset( $_REQUEST['group_name'   ] ) ? $_REQUEST['group_name'    ] : FALSE;
  $sg_name      = urldecode($sg_name);
  $src_name     = isset($_REQUEST['tgt_group_name'] ) ? $_REQUEST['tgt_group_name'] : ''    ;
  $src_owner    = isset($_REQUEST['user_id'       ] ) ? $_REQUEST['user_id'       ] : ''    ;
  $protocol     = isset($_REQUEST['ip_protocol'   ] ) ? $_REQUEST['ip_protocol'   ] : ''    ;
  $from_port    = isset($_REQUEST['from_port'     ] ) ? $_REQUEST['from_port'     ] : ''    ;
  $to_port      = isset($_REQUEST['to_port'       ] ) ? $_REQUEST['to_port'       ] : ''    ;
  $src_ip       = isset($_REQUEST['cidr_ip'       ] ) ? $_REQUEST['cidr_ip'       ] : NULL  ;

  $result = _aws_cloud_revoke_security_group($cloud_context, $sg_name, $src_name, $src_owner, $protocol, $from_port, $to_port, $src_ip);

  if ($result) {
    drupal_set_message( t('Updated Security Group successfully: @sg_name', array('@sg_name' => $sg_name) ) );
  }
  else {
    drupal_set_message( t('There was a problem while updating Security Group: @sg_name', array('@sg_name' => $sg_name) ), 'error' );
  }

  $cloud_menu_path = cloud_get_menu_path($cloud_context );
  drupal_goto( $cloud_menu_path . '/security_groups/describe', array('query' => array('group_name' => $sg_name)));
}


/**
 * Display List of Security Groups
 *
 * @param     $form_submit
 * @param     $cloud_context
 * @return
 */
function aws_cloud_display_security_group_list($form_submit = '', $form_state, $cloud_context) {

  drupal_add_js(cloud_get_module_base() . 'js/cloud.js');

  $form['cloud_context'] = array(
    '#type' => 'hidden',
    '#value' => $cloud_context,
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $filter     = isset($_REQUEST['filter'   ]) ? $_REQUEST['filter'   ] : '';
  $filter     = trim($filter);

  $sql_col = 'group_name';

  if ( isset($filter) ) {
    $query_args[] = $sql_col;
    $query_args[] = $filter;
    $query_args[] = $cloud_context;
  }

  $security_group_count = _aws_cloud_get_security_groups_list_count($cloud_context);
  $filter_disabled      = '';
  if ( $security_group_count < 2 ) {
    $filter_disabled = TRUE;
  }

  $form['options']['label'] = array(
    '#type' => 'item'     ,
    '#title' => t('Filter'),
  );
  $form['options']['filter'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => $filter,
    '#disabled' => $filter_disabled,
  );

  $form['options']['submit'] = array(
    '#type' => 'submit'   ,
    '#value' => t('Apply')                      ,
    '#disabled' => $filter_disabled,
  );

  if (user_access($cloud_context . ' register security group') ) {
    $form['options']['new_key'] = array(
      '#type' => 'submit',
      '#value' => t('Create'),
    );
  }

  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
        array('data' => t('Name'), 'field' => 'group_name', array('class' => 'nickname-column'), 'sort' => 'asc',
      ),      
      array('data' => t('Description'), 'field' => 'group_description'),
      array('data' => t('Owner'), 'field' => 'owner_id'),
      array('data' => t('Permissions'), 'field' => 'permissions'),
      array('data' => t('Actions'), 'class' => 'action-column'),
    ),
  );

  $query    = _aws_cloud_get_describe_security_groups_query($cloud_context);

  $query
      ->condition('cloud_type', $cloud_context, '=')
      ->condition($sql_col, '%' . $filter . '%', 'like')
      ->extend('TableSort')
      ->orderByHeader($form['header']['#value']);

  $result = $query->execute();
  $cloud_menu_path = cloud_get_menu_path($cloud_context );

  $security_group_details   = _aws_cloud_get_security_groups_details_db( $cloud_context );
  $sg_permissions           = array();
  $cnt = 0;
  foreach ($security_group_details as $sg_key => $sg_group_data) {

    $permissions = '';
    foreach ($sg_group_data as $sg_details) {

      $permissions .= drupal_strtoupper( $sg_details['ip_protocol'] ) . ' ';
      if ( empty($sg_details['cidr_ip']) === FALSE ) {
        $permissions .= 'IPs: ' . $sg_details['cidr_ip'] . ' ';
      }

      if ( $sg_details['ip_protocol'] !== 'icmp' ) {
        if ($sg_details['from_port'] === $sg_details['to_port'] ) {
          $permissions .= 'Port: ' . $sg_details['from_port'] . ' ';
        }
        else {
          $permissions .= 'Ports: ' . $sg_details['from_port'] . ' .. ' . $sg_details['to_port'] . ' ';
        }
      }
      else { // If protocol is ICMP
        if ($sg_details['from_port'] === $sg_details['to_port'] && $sg_details['to_port'] === '-1') {
          $permissions .= 'Types: Any' . ' ';
        }
        else {
          $permissions .= 'Type: ' . $sg_details['from_port'] . ', Code:' . $sg_details['to_port'] . ' ';
        }
      }

      if ( empty($sg_details['cidr_ip']) ) { // for group
        $permissions .= 'Owner:' . $sg_details['user_id'] . ' Group: ' . $sg_details['tgt_group_name'] . ' ';
      }

      $permissions .= '<br />'; // New Line
    }

    $sg_permissions[$sg_key] = $permissions;
  }

  foreach ($result as $sg) {
    $form['Name'][$sg->group_name] = array('#markup' => l($sg->group_name, $cloud_menu_path . '/security_groups/describe', array('query' => array('group_name' => $sg->group_name))));
    $form['Description'][$sg->group_name] = array('#markup' => check_plain( $sg->group_description ));
    $form['Owner'][$sg->group_name] = array('#markup' => t( $sg->owner_id ));
    if (isset($sg_permissions[$sg->group_name])) {
      $form['Permissions'][$sg->group_name] = array('#markup' => t( $sg_permissions[$sg->group_name] ));
    }
    else {
      $form['Permissions'][$sg->group_name] = array('#markup' => '');
    }
    $form['Actions'][$sg->group_name] = array('#markup' => aws_cloud_get_security_action($cloud_context, array('group_name' => $sg->group_name)));
  }

  $form['refresh_page'] = array(
    '#type' => 'item',
    '#prefix' => '<div id="link_reload" align="right">',
    '#suffix' => '</div>',
    '#markup' => l(t('- Refresh Page -'), $cloud_menu_path . '/get_security_group_data', array()),
  );
  $form['pager'] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
  $form_state['#redirect']  = FALSE;

  return $form;
}


/**
 * Theme function for security group listing page
 */
function theme_aws_cloud_display_security_group_list($form) {

  $form = $form['form'];
  $rows = array();
  foreach (element_children($form['Name']) as $sg) {
    $rows[] = array(
      array('data' => drupal_render($form['Name'][$sg]), 'class' => array('nickname-column')),
      drupal_render($form['Description'][$sg]),
      drupal_render($form['Owner'][$sg]),
      drupal_render($form['Permissions'][$sg]),
      array('data' => drupal_render($form['Actions'][$sg]), 'class' => array('action-column')),
    );
  }

  $output  = drupal_render($form['new_key']);
  $output .= drupal_render($form['options']);
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));

  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form['refresh_path']);
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Generate action links for a security 
 */
function aws_cloud_get_security_action($cloud_context, $security_info) {
  $cloud_menu_path = cloud_get_menu_path($cloud_context);
  $action_data = array();

  if (user_access($cloud_context . ' delete security group') ) {
    $prop['onclick'] = cloud_get_messagebox('Are you sure you want to delete the Security Group "' . $security_info['group_name'] . '" ?');
    $action_data[] = cloud_display_action('images/icon_delete.png', t('Delete'), $cloud_menu_path . '/security_groups/delete', array('query' => array('sg_name' => $security_info['group_name']), 'html' => TRUE), $prop['onclick']);
  }
  
  if (user_access($cloud_context . ' setup security group') ) {
    $action_data[] = cloud_display_action('images/icon_edit.png', t('Edit'), $cloud_menu_path . '/security_groups/describe', array('query' => array('group_name' => $security_info['group_name']), 'html' => TRUE));
  }
  
  $user_actions = module_invoke_all('aws_cloud_security_action_data', $security_info);
  $action_data = array_merge($action_data, $user_actions);
  return implode($action_data);
  
}


/**
 * Security Group Listing Submit function
 */
function aws_cloud_display_security_group_list_submit($form_id, &$form_state) {
  $form_values =& $form_state['values'];
  $cloud_menu_path = cloud_get_menu_path($form_values['cloud_context']);

  if ( $form_values['op'] == t('Create')) {
    $form_state['redirect'] = $cloud_menu_path . '/security_groups/create';
  }
  elseif ( $form_values['op'] == t('Apply') ) {
    $form_state['redirect'] = array(current_path(), array('query' => array('filter' => $form_values['filter'])));
  }
}
