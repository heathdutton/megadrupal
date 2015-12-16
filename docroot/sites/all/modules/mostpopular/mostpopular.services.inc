<?php
/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright © 2009-2012 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com -or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */

/**
 * @file
 * Provides an admin GUI for configuring services.
 */

/**
 * Renders a form for configuring the blocks and services.
 */
function mostpopular_services_admin_form($form, &$form_state) {
  $form += array(
    '#tree' => TRUE,
    'blocks' => array(
  		'#theme' => 'mostpopular_admin_services_table',
    ),
  );
  
  $blocks = mostpopular_blocks_local();
  $form_state['blocks'] = $blocks;
  
  $block_options = array();
  foreach ($blocks as $bid => $block) {
    $block_options[$bid] = $block->title;
  }
  
  foreach ($blocks as $bid => $block) {
    $form['blocks'][$bid] = array(
      '#type' => 'fieldset',
      '#title' => t('Block: @title', array(
        '@title' => $block->title,
      )),
      
      'bid' => array(
        '#type' => 'hidden',
        '#value' => $bid,
      ),
      'services' => array(
        '#tree' => TRUE,
      ),
    );
    
    $services = mostpopular_service_load_by_block($bid, FALSE);
    $form_state['services'][$bid] = $services;
    
    foreach ($services as $sid => $service) {
      $form['blocks'][$bid]['services'][$sid] = array(
        'id' => array(
          '#markup' => !empty($service->sid) ? $service->sid : t('New'), 
        ),
        'name' => array(
          '#markup' => check_plain($service->name),
        ),
        'enabled' => array(
          '#type' => 'checkbox',
          '#default_value' => $service->enabled,
        ),
        'status' => array(
          '#theme' => 'mostpopular_service_status',
          '#status' => $service->status,
        ),
        'title' => array(
          '#type' => 'textfield',
          '#size' => 32,
          '#default_value' => check_plain($service->title),
        ),
        'weight' => array(
          '#type' => 'textfield',
          '#size' => 3,
          '#default_value' => $service->weight,
        ),
        'bid' => array(
          '#type' => 'select',
          '#default_value' => $bid,
          '#options' => $block_options,
        ),
        'config' => array(
          'configure' => array(
            '#type' => 'link',
            '#title' => t('Configure'),
            '#href' => "admin/config/mostpopular/services/$sid/edit",
            '#suffix' => '&nbsp; ',
          ),
          'delete' => array(
            '#type' => 'link',
            '#title' => t('Delete'),
            '#href' => "admin/config/mostpopular/services/$sid/delete",
          ),
        ),
      );
    }
  }
  
  // Add a section for adding a new service
  $info = mostpopular_service_info();
  $options = array();
  foreach ($info as $module => $inf) {
    foreach ($inf as $delta => $in) {
      $options[$module]["$module|$delta"] = $in['name'];
    }
  }
  
  if (count($blocks) > 0) {
    $form['add_service'] = array(
      '#tree' => TRUE,
      '#type' => 'container',
      '#attributes' => array(),
      
      'service' => array(
        '#type' => 'select',
        '#multiple' => FALSE,
        '#options' => $options,
      ),
      'button' => array(
        '#type' => 'submit',
        '#value' => t('Add Service'),
        '#submit' => array( 'mostpopular_services_admin_form_add_service' ),
      ),
    );
  }
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#submit' => array( 'mostpopular_services_admin_form_submit' ),
  );

  // Add a button to clear caches
  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear all cached values'),
    '#submit' => array( 'mostpopular_services_admin_form_submit_clear' ),
  );
  return $form;
}

function mostpopular_services_admin_form_validate($form, $form_state) {
  foreach ($form_state['values']['blocks'] as $bid => $block) {
    if (isset($block['services'])) {
      foreach ($block['services'] as $sid => $service) {
        $title = $service['title'];
        $enabled = $service['enabled'];
        if ($enabled && empty($title)) {
          form_set_error("blocks][$bid][$sid][title", t('You must choose a title to be displayed this service.'));
        }
      }
    }
  }
}

function mostpopular_services_admin_form_submit($form, $form_state) {
  $services = $form_state['services'];
  
  foreach ($form_state['values']['blocks'] as $bid => $block) {
    if (isset($block['services'])) {
      foreach ($block['services'] as $sid => $values) {
        if (isset($services[$bid][$sid])) {
          $service = $services[$bid][$sid];
          
          foreach ($values as $key => $val) {
            $service->$key = $val;
          }
          mostpopular_service_save($service);
        }
      }
    }
  }
  drupal_set_message(t('Your service configuration has been saved.'));
}

function mostpopular_services_admin_form_add_service(&$form, &$form_state) {
  $type = $form_state['values']['add_service']['service'];
  list($module, $delta) = explode('|', $type);
  
  if (!empty($module) && !empty($delta)) {
    // Load the defaults for this service.
    $service = mostpopular_service_info($module, $delta);
    
    // Attach the service to the last block
    $blocks = $form_state['blocks'];
    $block = reset(array_reverse($blocks));
    
    if ($service && $block) {
      $service += array(
        'bid' => $block->bid,
        'enabled' => 0,
        'status' => MOSTPOPULAR_SERVICE_STATUS_DISABLED,
        'title' => $service['name'],
        'weight' => 0,
      );
      
      // Save the service
      mostpopular_service_save($service);
    }
  }
  $form['rebuild'] = TRUE;
}

/**
 * Defines a theme function for rendering the services form.
 * 
 * @param array $variables 
 *   - form: The form to render.
 */
function theme_mostpopular_admin_services_table(&$variables) {
  $elements = $variables['element'];
  
  $header = array(
    array('data' => '', 'width' => 2),
    t('ID'),
    t('Service Name'),
    t('Enabled'), 
    t('Status'), 
    t('Title'),
    t('Block'), 
    t('Weight'),
    t('Options')
  );
  $rows = array();
  $output = '';

  foreach (element_children($elements) as $bid) {
    $block = $elements[$bid];
    
    $block['bid']['#attributes']['class'][] = 'mostpopular-block-bid';
    
    $title = $block['#title'];
    $rows[] = array(
      'data' => array(
        array(
          'data' => '<strong>' . $title . '</strong>' . drupal_render($block['bid']),
          'colspan' => 9,
        ),
      ),
    );
    foreach (element_children($block['services']) as $sid) {
      $item = $block['services'][$sid];

      // Add class to group weight fields for drag and drop
      $item['bid']['#attributes']['class'][] = "mostpopular-bid";
      $item['bid']['#attributes']['class'][] = "mostpopular-bid-$bid";
      $item['weight']['#attributes']['class'][] = "mostpopular-weight";
      $item['weight']['#attributes']['class'][] = "mostpopular-weight-$bid";
  
      $row = array();
      $row[] = '';
      $row[] = drupal_render($item['id']);
      $row[] = drupal_render($item['name']);
      $row[] = drupal_render($item['enabled']);
      $row[] = drupal_render($item['status']);
      $row[] = drupal_render($item['title']);
      $row[] = drupal_render($item['bid']);
      $row[] = drupal_render($item['weight']);
      $row[] = drupal_render($item);
      
      $rows[] = array(
        'data' => $row, 
        'class' => array( 'draggable' ),
      );
    }
    
    // Add tabledrag behavior to this region
    drupal_add_tabledrag('mostpopular-admin-services', 'match', 'parent', 'mostpopular-bid', "mostpopular-bid-$bid", 'mostpopular-block-bid', FALSE);
    drupal_add_tabledrag('mostpopular-admin-services', 'order', 'sibling', 'mostpopular-weight', "mostpopular-weight-$bid");
  }
  $output .= theme('table', array( 
    'header' => $header, 
    'rows' => $rows, 
    'attributes' => array(
      'id' => 'mostpopular-admin-services'
    ),
  ));
  
  drupal_add_css(drupal_get_path('module', 'mostpopular') . '/css/mostpopular-admin.css');
  
  return $output;
}

/**
 * Themes the status of a service.
 * 
 * @param array An array containing:
 *   - status: The status of the service as an integer. 
 */
function theme_mostpopular_service_status($variables) {
  $status = $variables['status'];
  
  $out = "<span class='mostpopular--service-status mostpopular--service-status-$status'>";
  switch ($status) {
    case MOSTPOPULAR_SERVICE_STATUS_OK:
      $out .= t('OK');
      break;

    case MOSTPOPULAR_SERVICE_STATUS_CONFIGURED:
      $out .= t('Configured');
      break;
      
    case MOSTPOPULAR_SERVICE_STATUS_PENDING:
      $out .= t('Pending');
      break;
      
    case MOSTPOPULAR_SERVICE_STATUS_ERROR:
      $out .= t('Error');
      break;

    default:
      $out .= '&nbsp;';
  }
  $out .= "</span>";
  return $out;
}

/* ----------------------------------------------------------------------------
 * Service Config Form
 * --------------------------------------------------------------------------*/
function mostpopular_service_config_form($form, &$form_state, $sid) {
  $service = mostpopular_service_load($sid);
  $form_state['service'] = $service;
  
  $form['data'] = array(
    '#tree' => TRUE,
  );
  
  // If the service provides any configuration form, show it first
  $extra = _mostpopular_invoke('config_form', $service, $form_state);
  if ($extra) {
    $form['data'] += $extra;
  }
  
  if (isset($service->entity_types)) {
    $service->data += array(
      'entities_only' => FALSE,
      'entity_types' => array(),
    );
    
    $form['data']['entities_only'] = array(
      '#type' => 'checkbox',
      '#title' => t('Only include entities?'),
      '#description' => t('If checked, only entities will be included in the Most Popular results for this service.  Otherwise, any valid URL could be included.'),
      '#default_value' => $service->data['entities_only'],
      '#weight' => 49,
    );
    
    $form['data']['entity_types'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#title' => t('Entity Types'),
      '#tree' => TRUE,
      '#element_validate' => array('mostpopular_service_config_form_validate_entity_types'),
      '#states' => array(
        'invisible' => array(
          'input[name="data[entities_only]"]' => array('checked' => FALSE),
        ),
      ),
      '#weight' => 50,
    );
    
    if ($service->entity_types === TRUE) {
      $entity_types = entity_get_info();
    }
    else {
      foreach ($service->entity_types as $name) {
        $entity_types[$name] = entity_get_info($name);
      }
    }
    
    foreach ($entity_types as $name => $info) {
      $service->data['entity_types'] += array( $name => array() );
      
      // Only get entities with valid URIs
      if (!isset($info['uri callback']) && !isset($info['view callback'])) {
        continue;
      }
      
      $form['data']['entity_types'][$name] = array(
        '#type' => 'tableselect',
        '#multiple' => TRUE,
        '#header' => array( 
          'title' => $info['label'],
        ),
        '#options' => array(),
        '#default_value' => $service->data['entity_types'][$name],
      );
      foreach ($info['bundles'] as $bundle => $data) {
        $form['data']['entity_types'][$name]['#options'][$bundle] = array(
          'title' => $data['label'],
        );
      }
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Save configuration',
      '#submit' => array( 'mostpopular_service_config_form_submit' ),
      '#weight' => 0,
    ),
    'back' => array(
      '#type' => 'link',
      '#title' => t('Back'),
      '#href' => 'admin/config/mostpopular/services',
      '#weight' => 30,
    ),
  );
  
  $form['#attached']['css'][] = drupal_get_path('module', 'mostpopular') . '/css/mostpopular-admin.css';

  drupal_set_title(mostpopular_service_title($sid), PASS_THROUGH);
  return $form;
}

function mostpopular_service_config_form_validate_entity_types($element, &$form_state) {
  foreach ($form_state['values']['data']['entity_types'] as $name => $data) {
    $data = array_filter($data);
    if (empty($data)) {
      unset($form_state['values']['data']['entity_types'][$name]);
    }
    else {
      $form_state['values']['data']['entity_types'][$name] = $data;
    }
  }
}

function mostpopular_service_config_form_submit($form, &$form_state) {
  $service = $form_state['service'];
  
  // Copy in the values
  foreach ($form_state['values']['data'] as $key => $value) {
    $service->data[$key] = $value;
  }
  $service->status = MOSTPOPULAR_SERVICE_STATUS_CONFIGURED;
  
  // Let the service handle the form submission
  _mostpopular_invoke('config_form_submit', $service, $form_state);
  
  // Save changes to the service
  mostpopular_service_save($service);

  drupal_set_message(t('This service has been configured.'));
  $form_state['redirect'] = 'admin/config/mostpopular/services';
}

function mostpopular_service_delete_form($form, &$form_state, $sid) {
  $service = mostpopular_service_load($sid);
  $form_state['service'] = $service;
  
  return confirm_form($form,
    t('Are you sure you want to delete the @name service?', array(
      '@name' => $service->title,
    )),
    'admin/config/mostpopular/services',
    t('This action cannot be undone.'),
    t('Yes, delete it'),
    t('No, nevermind')
  );
}

function mostpopular_service_delete_form_submit(&$form, &$form_state) {
  $service = $form_state['service'];
  mostpopular_service_delete($service->sid);
  
  drupal_set_message(t('Deleted @name: %title service', array(
    '@name' => $service->name,
    '%title' => $service->title,
  )));
  
  $form_state['redirect'] = 'admin/config/mostpopular/services';
}