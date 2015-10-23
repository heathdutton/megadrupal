<?php

/**
 * Class BackpackDisplayerPresenter
 * This class allow the developer to implement the Backpack Displayer API into an Opigno LMS instance
 */
class BackpackDisplayerPresenter
{
  /**
   * The displayer API
   */
  const BACKPACK_DISPLAYER_API_URL = 'https://backpack.openbadges.org/displayer';

  /**
   * Alter the user view in order to add a section dedicated to show some public collections
   *
   * @param $build array The build given in the hook_user_view_alter()
   */
  public static function user_view_alter_add_collections_section(&$build)
  {
    $displayerUID = BackpackDisplayerDb::getEntries($build['#account']->uid);
    $temp=array_shift($displayerUID);
    $displayerUID = $temp[BackpackDisplayerDb::FIELD_BACKPACK_USER_ID];

    $displayer = new BackpackDisplayer(self::BACKPACK_DISPLAYER_API_URL);
    $displayer->setUserId($displayerUID);
    $displayerApi = new BackpackDisplayerApi($displayer);

    $collectionsDb = BackpackDisplayerCollectionDb::getEntries($build['#account']->uid);
    if (!empty($collectionsDb))
    {
      $build['backpack_badges'] = array(
        '#type' => 'user_profile_category',
        '#title' => t('Public collections'),
        '#weight' => 5
      );
      $build['backpack_badges'][0] = array(
        '#type' => 'container'
      );

      foreach ($collectionsDb as $collectionEntry)
      {
        $badges = $displayerApi->getBadges($collectionEntry[ BackpackDisplayerCollectionDb::FIELD_COLLECTION_ID ]);
        if ($badges != false)
        {
          foreach ($badges->badges as $badge)
          {
            array_push($build['backpack_badges'][0], array(
              '#markup' => l(
                theme_image(array(
                  'path'   => $badge->imageUrl,
                  'width'  => 100,
                  'height' => 100,
                  'alt'    => t('Backpack badge image'),
                  'title'  => $badge->assertion->badge->name,
                  'attributes' => array(),
                )),
                $badge->assertion->badge->criteria,
                array('html' => true))
            ));
          }
        }
      }
    }
  }

  /**
   * Do the ajax behaviours from the form alter hook.
   *
   * @param $form array The array of the form as expected by drupal_render()
   * @param $form_state array The state of the form
   */
  public static function form_alter_do_ajax_behaviours($form, $form_state)
  {
    if (isset($form_state['triggering_element']['#name'])&&$form_state['triggering_element']['#name'] == 'button_add_collection')
    {
      $backpackDisplayer = new BackpackDisplayer(self::BACKPACK_DISPLAYER_API_URL);
      $backpackApi = new BackpackDisplayerApi($backpackDisplayer);

      $backpackUID = BackpackDisplayerDb::getEntries($form['#user']->uid);
      if (empty($backpackUID))
      {
        $backpackUID = $backpackApi->getUserId($form_state['values']['backpack_displayer_input']);
        if ($backpackUID != -1)
        {
          BackpackDisplayerDb::save($form['#user']->uid, $backpackUID);
        }
        else
        {
          drupal_set_message(t('This email does not exist in Mozilla Backpack'), 'error');
        }
      }
    }
    else if (isset($form_state['triggering_element']['#name'])&&$form_state['triggering_element']['#name'] == 'button_save_collections')
    {
      BackpackDisplayerCollectionDb::clear($form['#user']->uid);

      $checkboxes = $form_state['values']['collections_checkboxes'];
      foreach ($checkboxes as $checkboxId)
      {
        if (!empty($checkboxId))
          BackpackDisplayerCollectionDb::save($checkboxId, $form['#user']->uid);
      }

      drupal_set_message(t('Your configuration has been saved'));
    }
    else if (isset($form_state['triggering_element']['#name'])&&$form_state['triggering_element']['#name'] == 'button_displayer_logout')
    {
      BackpackDisplayerCollectionDb::clear($form['#user']->uid);
      BackpackDisplayerDb::clear($form['#user']->uid);
      drupal_set_message(t('You have been logged out successfully'));
    }
  }


  /**
   * Alter the user form in order to add the Backpack Displayer settings.
   *
   * @param $form array The form given by the hook_form_alter()
   */
  public static function form_alter_add_collections_settings(&$form)
  {
    $form['group_badge_collections'] = self::_get_array_collections_settings($form);
  }

  /**
   * Create the array as exected by drupal_render() with the settings for the Backpack Displayer Settings
   *
   * @param $form array The initial form
   * @return array A form to add in the initial form
   */
  public static function _get_array_collections_settings($form)
  {
    $weight = 1;
    if (!empty($form['#groups']['group_badge_settings']->weight))
      $weight = $form['#groups']['group_badge_settings']->weight + 0.0001;

    $return = array(
      '#type' => 'fieldset',
      '#title' => t('Mozilla Backpack Displayer'),
      '#description' => t('The Mozilla Backpack Displayer allows you to show, on your profile, some public collections '.
        'from your Backpack.'),
      '#prefix' => '<div id="group_badge_collections">',
      '#suffix' => '</div>',
      '#weight' => $weight
    );

    $displayerIDs = BackpackDisplayerDb::getEntries($form['#user']->uid);
    if (empty($displayerIDs))
    {
      $return['backpack_displayer_input'] = array(
        '#type' => 'textfield',
        '#title' => t('Enter your backpack email')
      );
      $return['backpack_displayer_submit'] = array(
        '#type' => 'button',
        '#name' => 'button_add_collection',
        '#value' => t('Get my public collections'),
        '#ajax' => array(
          'callback' => 'ajax_opigno_mozilla_openbadges_app_backpack_collections_settings_callback',
          'wrapper' => 'group_badge_collections',
          'method' => 'replace',
          'effect' => 'fade'
        )
      );
    }
    else
    {
      $backpackDisplayer = new BackpackDisplayer(self::BACKPACK_DISPLAYER_API_URL);
      $backpackDisplayerApi = new BackpackDisplayerApi($backpackDisplayer);
      foreach ($displayerIDs as $displayerValues) {
        $backpackDisplayer->setUserId($displayerValues[ BackpackDisplayerDb::FIELD_BACKPACK_USER_ID ]);

        $collections = $backpackDisplayerApi->getCollections();
        if (!empty($collections)) {
          $collectionsOptions = array();
          foreach ($collections as $collection) {
            $collectionsOptions[ $collection->groupId ] = $collection->name . ' (' . $collection->badges . ')';
          }

          $defaultCollectionsDb = BackpackDisplayerCollectionDb::getEntries($form['#user']->uid);
          $defaultCollections = array();
          foreach ($defaultCollectionsDb as $defaultCollectionEntry) {
            $defaultCollections[] = $defaultCollectionEntry[ BackpackDisplayerCollectionDb::FIELD_COLLECTION_ID ];
          }

          $return['collections_checkboxes'] = array(
            '#title'         => t('Your public collections'),
            '#type'          => 'checkboxes',
            '#description'   => t('Select the collections that you want to show on your profile'),
            '#options'       => $collectionsOptions,
            '#default_value' => $defaultCollections
          );
          $return['backpack_displayer_submit'] = array(
            '#type'  => 'button',
            '#value' => t('Save this configuration'),
            '#name'  => 'button_save_collections',
            '#ajax'  => array(
              'callback' => 'ajax_opigno_mozilla_openbadges_app_backpack_collections_settings_callback',
              'wrapper'  => 'group_badge_collections',
              'method'   => 'replace',
              'effect'   => 'fade'
            ),
          );

        }
        else
        {
          $return['collection_empty'] = array(
            '#markup' => t('There is no public collection in this backpack').'<br >',
          );
        }

      }

      // Add the logout button
      $return['backpack_displayer_logout'] = array(
        '#type'  => 'button',
        '#name'  => 'button_displayer_logout',
        '#value' => t('Disconnect this account'),
        '#ajax'  => array(
          'callback' => 'ajax_opigno_mozilla_openbadges_app_backpack_collections_settings_callback',
          'wrapper'  => 'group_badge_collections',
          'method'   => 'replace',
          'effect'   => 'fade'
        ),
      );
    }

    return $return;
  }
}
