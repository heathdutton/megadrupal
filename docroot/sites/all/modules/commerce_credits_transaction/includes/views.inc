<?php
/**
 * @file
 * Commerce Credits Transaction Views Controller Class.
 */


/**
 * Provide some relationship handler definitions for entities.
 */
class CommerceCreditsTransactionViewsController extends EntityDefaultViewsController {

  public function views_data() {
    $data = parent::views_data();

    $info = &$data['commerce_credits_transaction'];

    $info['from_entity_id']['field']['handler'] = 'commerce_credits_transaction_handler_field_entity_id';
    $info['to_entity_id']['field']['handler'] = 'commerce_credits_transaction_handler_field_entity_id';

    $info['from_entity_type']['field']['handler'] = 'commerce_credits_transaction_handler_field_entity_type';
    $info['to_entity_type']['field']['handler'] = 'commerce_credits_transaction_handler_field_entity_type';

    return $data;
  }

}
