<?php
/**
 * @file
 * Commerce Credits Transaction controller classes.
 */


/**
 * Entity Controller for Commerce Credits Transaction entity.
 */
class CommerceCreditsTransactionController extends EntityAPIController {

  /**
   * Finalize all transactions of a specific entity.
   *
   * @param $entity_type
   *   The entity type of the entity below.
   * @param $entity
   *   The entity to finalize transactions.
   * @param $direction
   *   Either 'from' or 'to'.
   * @param $transaction
   *   The transaction type to complete: from, to, or transfer.
   * @return integer
   *   The number of transactions that were completed.
   */
  public function completeTransaction($entity_type, $entity, $direction = 'to', $transaction = 'transfer') {
    $wrapper = entity_metadata_wrapper($entity_type, $entity);   

    $query = db_update('commerce_credits_transaction')
      ->fields(array('status' => 'complete'))
      ->condition('status', 'pending')
      ->condition('transaction', $transaction);

    switch ($direction) {
      case 'to':
        $id_column = 'to_entity_id';
        $type_column = 'to_entity_type';
        break;
      case 'from':
      default:
        $id_column = 'from_entity_id';
        $type_column = 'from_entity_type';
        break;
    }

    $num = $query
      ->condition($id_column, $wrapper->getIdentifier())
      ->condition($type_column, $entity_type)
      ->execute();
    return $num;
  }

  /**
   * Reverse a transaction and return the new transaction.
   *
   * @param $original
   *   The original CommerceCreditsTransaction entity. Using the actual class
   *   does not work for dependency injection here for some reason. DrupalWTF.
   * @return CommerceCreditsTransaction
   *   The original CommerceCreditsTransaction entity for chaining.
   */
  public function reverse(Entity $original) {
    $original->status = 'canceled';
    $credits_controller = entity_get_controller('commerce_credits');

    if (!isset($original->to_cid)) {
      // Add credits back to the from credits entity.
      $to_credits = entity_load('commerce_credits', array($original->from_cid));
      if ($to_credits) {
        $to_credit = array_shift($to_credits);
        $credits_controller->addCredits($to_credit, $original->credits);
      }
    }
    elseif (!isset($original->from_cid)) {
      // Remove credits from the to credits entity.
      $from_credits = entity_load('commerce_credits', array($original->to_cid));
      if ($from_credits) {
        $from_credit = array_shift($from_credits);
        $credits_controller->removeCredits($from_credit, $original->credits);
      }
    }
    else {
      // Transfer credits from the to entity to the from entity.
      $to_credits = entity_load('commerce_credits', array($original->from_cid));
      $from_credits = entity_load('commerce_credits', array($original->to_cid));

      if ($to_credits) {
        $to_credit = array_shift($to_credits);
      }

      if ($from_credits) {
        $from_credit = array_shift($from_credits);
      }

      $credits_controller->transferCreditWithTransaction($from_credit, $to_credit, $original->credits);
    }

    return $original;
  }
}

/**
 * Extends the controller of Commerce Credits in order to add transactions at
 * the right time instead of going through hook interfaces and duplicate
 * write operations.
 */
class CommerceCreditsTransactionCreditsController extends CommerceCreditsEntityController {

  /**
   * Create a commerce credits transaction entity.
   *
   * @param $from_credit
   *   An optional commerce credits entity.
   * @param $to_credit
   *   An optional commerce credits entity.
   * @param $transaction
   *   The transaction operation: in, out, or transfer.
   * @param $status
   *   The status of the operation: pending or complete.
   * @param $credits
   *   The number of credits in the transaction.
   * @param $credit_type
   *   THe Commerce Credits group.
   * @return CommerceCreditsTransaction
   *   The commerce credits transaction entity.
   */
  public function createTransaction($from_credit = NULL, $to_credit = NULL, $transaction = 'in', $status = 'complete', $credits, $credit_type) {
    $values = array(
      'from_cid' => isset($from_credit) ? $from_credit->cid : NULL,
      'from_entity_id' => isset($from_credit) ? $from_credit->entity_id : NULL,
      'from_entity_type' => isset($from_credit) ? $from_credit->entity_type : NULL,
      'to_cid' => isset($to_credit) ? $to_credit->cid : NULL,
      'to_entity_id' => isset($to_credit) ? $to_credit->entity_id : NULL,
      'to_entity_type' => isset($to_credit) ? $to_credit->entity_type : NULL,
      'transaction' => $transaction,
      'status' => $status,
      'credits' => $credits,
      'credit_type' => $credit_type,
      'transaction_date' => time(),
    );

    $transaction = entity_get_controller('commerce_credits_transaction')->create($values);
    return $transaction;
  }

  /**
   * addCredits - add a transaction.
   */
  public function addCredits($entity, $credits) {
    $entity->credits = $entity->credits + $credits;

    $ret =  $this->save($entity);
    $this->createTransaction(NULL, $entity, 'in', 'complete', $credits, $entity->credit_type)->save();

    return $ret;
  }

  /**
   * removeCredits - add a transaction.
   *
   * This does NOT delete the credit entity when it is out of credits.
   */
  public function removeCredits($entity, $credits) {
    $entity->credits = ($entity->credits > $credits) ? $entity->credits - $credits : 0;

    $ret = $this->save($entity);
    $this->createTransaction($entity, NULL, 'out', 'complete', $credits, $entity->credit_type)->save();

    return $ret;
  }

  /**
   * transferCredits - add a transaction. This function is terrible and should
   * be replaced by transferCreditEntities below.
   */
  public function transferCredits(EntityDrupalWrapper $from_entity, EntityDrupalWrapper $to_entity, $credit_type, $credits = 1) {
    // Make sure cid exists.
    $from_credits = $this->loadByEntity($from_entity->getIdentifier(), $from_entity->type(), $credit_type);
    if ($from_credits) {
      $from_credits = reset($from_credits);
      $credits = ($credits == -1) ? $from_credits->credits : $credits;
      if ($from_credits->credits < $credits) {
        // @todo maybe throw exception.
        return FALSE;
      }
      $to_credits = $this->loadByEntity($to_entity->getIdentifier(), $to_entity->type(), $from_credits->credit_type);
      // Create a new credits entity if it doesn't yet exist.
      if (!$to_credits) {
        $to_credits = $this->createByEntity($to_entity->getIdentifier(), $to_entity->type(), $from_credits->credit_type);
      }
      else {
        $to_credits = reset($to_credits);
      }

      $from_credits->credits -= $credits;
      $this->save($from_credits);
      $to_credits->credits += $credits;
      $this->save($to_credits);

      $transaction = $this->createTransaction($from_credits, $to_credits, 'transfer', 'pending', $credits, $credit_type)->save();

      return $to_credits;
    }
    return FALSE;
  }

  /**
   * transferCreditWithTransaction with a transaction status for potential
   * credit transaction reversals.
   */
  public function transferCreditWithTransaction($from_credit, $to_credit, $credits = 0, $status = 'complete') {
    $credits = ($credits) ? $credits : $from_credit->credits;

    if ($credits > $from_credit->credits) {
      throw new Exception('Entity does not have enough credit to complete transaction.');
      return FALSE;
    }

    try {
      $from_credit->credits -= $credits;
      $this->save($from_credit);
      $to_credit->credits += $credits;
      $this->save($to_credit);

      $transaction = $this->createTransaction($from_credit, $to_credit, 'transfer', $status, $credits, $from_credit->credit_type)->save();
    }
    catch (Exception $e) {
      watchdog_exception('commerce_credits', $e, 'Could not complete transfer: %error', array('%error' => $e->getMessage()), WATCHDOG_CRITICAL);
      return FALSE;
    }

    return $to_credit;
  }

  /**
   * Build content.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    $build['entity'] = array(
      '#markup' => t('@type @id', array('@type' => $entity->entity_type, '@id' => $entity->id)),
      '#weight' => -10,
    );

    return $build;
  }
}
