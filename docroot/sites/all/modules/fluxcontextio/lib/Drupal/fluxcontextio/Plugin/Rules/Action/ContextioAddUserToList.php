<?php

/**
 * @file
 * Contains ContextioAddUserToList.
 */

namespace Drupal\fluxcontextio\Plugin\Rules\Action;

//use Drupal\fluxcontextio\Plugin\Entity\ContextioList;
//use Drupal\fluxcontextio\Plugin\Entity\ContextioListInterface;
use Drupal\fluxcontextio\Plugin\Service\ContextioAccountInterface;
use Drupal\fluxcontextio\Rules\RulesPluginHandlerBase;

/**
 * "Add a user to list" action.
 */
class ContextioAddUserToList extends RulesPluginHandlerBase implements \RulesActionHandlerInterface {

  /**
   * Defines the action.
   */
  public static function getInfo() {
    return static::getInfoDefaults() + array(
      'name' => 'fluxcontextio_list_add_user',
      'label' => t('Add a user to a list'),
      'parameter' => array(
        'account' => NULL, //static::getServiceParameterInfo(),  // @todo fix
        'list' => array(
          'type' => 'integer',
          'label' => t('List'),
          'options list' => array(get_called_class(), 'getListOptions'),
        ),
        'user' => array(
          'type' => 'text',
          'label' => t('User to add'),
        ),
      ),
      'group' => t('Contextio'),
    );
  }

  /**
   * Rules 'options list' callback for retrieving a Contextio list options.
   */
  public static function getListOptions($element, $name = NULL) {
    $options = array();
    if (!empty($element->settings['account']) && $account = entity_load_single('fluxservice_account', $element->settings['account'])) {
      $owner = $account->getRemoteIdentifier();
      $lists = $account->client()->getLists(array('id' => $owner));
      $lists = fluxservice_entify_multiple($lists, 'fluxcontextio_list', $account);
      foreach ($lists as $list) {
        if ($list->getUser()->getRemoteIdentifier() != $owner) {
          // Ignore lists that are owned by someone else.
          continue;
        }

        $identifier = $list->getRemoteIdentifier();
        $name = $list->getName();
        $options[$identifier] = $name;
      }
    }
    return $options;
  }

  /**
   * Executes the action.
   */
  public function execute(ContextioAccountInterface $account, $list, $user) {
    $account->client()->listAddMember(array(
      'screen_name' => $user,
      'list_id' => (int) $list,
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function form_alter(&$form, $form_state, $options) {
    $selected = !empty($this->element->settings['account']);

    $form['reload'] = array(
      '#weight' => $form['submit']['#weight'] + 1,
      '#type' => 'submit',
      '#name' => 'reload',
      '#value' => !$selected ? t('Continue') : t('Reload form'),
      '#limit_validation_errors' => array(array('parameter', 'account'), array('parameter', 'list')),
      '#submit' => array('rules_form_submit_rebuild'),
      '#ajax' => rules_ui_form_default_ajax('fade'),
      '#attributes' => array('class' => array('rules-hide-js')),
    );
    // Use ajax and trigger as the reload button.
    $form['parameter']['account']['settings']['account']['#ajax'] = $form['reload']['#ajax'] + array(
      'event' => 'change',
      'trigger_as' => array('name' => 'reload'),
    );

    if (empty($selected)) {
      unset($form['parameter']['list']);
      unset($form['parameter']['user']);
      $form['reload']['#limit_validation_errors'] = array(array('parameter', 'account'));
    }
  }


}
