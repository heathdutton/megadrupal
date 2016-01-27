<?php
/**
 * @file
 * Subs Views integration.
 */

/**
 * Subs Views Controller class.
 */
class SubsViewsController extends EntityDefaultViewsController {

  /**
   * Add extra fields to views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    // Add a field to view the subscription.
    $data['subs']['view'] = array(
      'field' => array(
        'title' => t('Link'),
        'help' => t('Provide a simple link to view the subscription.'),
        'handler' => 'subs_handler_view',
      ),
    );

    // Add a field to edit the subscription.
    $data['subs']['edit'] = array(
      'field' => array(
        'title' => t('Edit link'),
        'help' => t('Provide a simple link to edit the subscription.'),
        'handler' => 'subs_handler_edit',
      ),
    );

    // Add a field to delete the subscription.
    $data['subs']['delete'] = array(
      'field' => array(
        'title' => t('Delete link'),
        'help' => t('Provide a simple link to delete the subscription.'),
        'handler' => 'subs_handler_delete',
      ),
    );

    // Add a field for subscription operations.
    $data['subs']['operations'] = array(
      'field' => array(
        'title' => t('Operations'),
        'help' => t('Provide simple links to subscription operations (view/edit/delete).'),
        'handler' => 'subs_handler_operations',
      ),
    );

    // Specify custom handler for start and end date properties.
    $data['subs']['date_start']['field']['handler'] = 'subs_handler_date';
    $data['subs']['date_end']['field']['handler'] = 'subs_handler_date';


    // Not sure why status was not recognized as an option list.
//    $data['subs']['status']['field']['handler'] = 'views_handler_field_machine_name';
//    $data['subs']['status']['field']['options callback'] = array('EntityDefaultViewsController', 'optionsListCallback');
//    $data['subs']['status']['field']['options arguments'] = array('subs', 'status', 'view');

    return $data;
  }

}