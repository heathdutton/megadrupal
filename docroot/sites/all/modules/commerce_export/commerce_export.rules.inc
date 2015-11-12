<?php

/**
 * @file
 * Contains rules integration for the Commerce Export module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Action: Export a Commerce order.
 */
class CommerceExportActionOrderExport extends RulesActionHandlerBase {

  /**
   * Defines the action.
   */
  public static function getInfo() {
    return array(
      'name' => 'commerce_export',
      'label' => t('Export commerce orders'),
      'group' => t('Commerce Order'),
      'parameter' => array(
        'commerce_orders' => array(
          'label' => t('List of orders to export'),
          'type' => 'list<commerce_order>',
          'wrapped' => TRUE,
        ),
        'template' => array(
          'label' => t('Dynamic template name'),
          'type' => 'text',
          'description' => t('The dynamic portion of the template name to be used.'),
          'optional' => TRUE,
        ),
        'export_name' => array(
          'label' => t('Export file name'),
          'type' => 'text',
          'save' => TRUE,
          'default value' => 'order-[commerce-order:order-id].txt'
        ),
      ),
      'provides' => array(
        'export_file' => array(
          'label' => t('Export file'),
          'type' => 'file',
        ),
      ),
      'help' => t('Exports a list of Commerce Orders.'),
    );
  }

  /**
   * Executes the action.
   */
  public function execute($orders, $template, $name) {
    // Generate a list of possible theme hooks.
    $hook = 'commerce_export_orders';
    if ($template) {
      $hook .= '__' . check_plain($template);
    }

    // Create the export string.
    $export = theme($hook, array('orders' => $orders));
    // Create the export file.
    $file = file_save_data($export, "public://$name");
    $file->type = ($type = file_get_type($file)) ? $type : 'document';

    return array('export_file' => $file);
  }

}

/**
 * @}
 */
