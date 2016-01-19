<?php
/**
 * @file
 * Form and page callbacks for the Anki UI module.
 */

/**
 * Page callback to forward Anki user to lowest thing they have access to.
 */
function anki_ui_anki_page() {
  global $user;

  if (user_access('view own anki flashcards online')) {
    return drupal_goto("anki/{$user->uid}");
  }

  if (user_access('review own anki flashcards online')) {
    return drupal_goto("anki/{$user->uid}/review");
  }

  return MENU_ACCESS_DENIED;
}

/**
 * Page callback to act as a placeholder if the list view got disabled.
 */
function anki_ui_view_page() {
  return '';
}

/**
 * Form constructor for reviewing cards in Anki.
 *
 * @see anki_ui_review_form_submit()
 *
 * @ingroup forms
 */
function anki_ui_review_form($form, &$form_state) {
  global $language;

  list ($account, $deck) = $form_state['build_info']['args'];

  $form['#user'] = $account;
  $form['#deck'] = $deck;

  $card = !empty($form_state['storage']['card']) ? $form_state['storage']['card'] : NULL;
  if ($card) {
    $form['#card'] = $card;
  }

  if (!empty($form_state['storage']['finished'])) {
    // clear out storage, so the next time we load this page, it'll start over
    unset($form_state['storage']);

    // If we are out of questions, we tell the user and let them reset the quiz,
    // if they want.
    $form['message'] = array(
      // TODO: we don't currently have a way to forecast tomorrows review
      '#markup' => theme('anki_ui_review_finished', array()),
    );
  }
  elseif (!empty($card)) {
    $form['card'] = array(
      '#prefix' => '<div id="anki-ui-flashcard"><center>',
      '#suffix' => '</center></div>',
    );
    $form['card']['question'] = array(
      '#prefix' => '<div id="anki-ui-flashcard-question">',
      '#suffix' => '</div>',
      '#markup' => $card['question'],
    );
    $form['card']['answer'] = array(
      '#prefix' => '<div id="anki-ui-flashcard-answer">',
      '#suffix' => '</div>',
      '#markup' => $card['answer'],
    );

    $form['ease_buttons'] = array(
      '#prefix' => '<div id="anki-ui-ease-buttons">',
      '#suffix' => '</div>',
    );
    foreach ($card['answer_buttons'] as $button) {
      $form['ease_buttons']['ease_'. $button->ease] = array(
        '#type' => 'submit',
        '#value' => $button->string_label . ' (' . $button->string_interval . ')',
        '#ease' => $button->ease,
      );
    }
  }
  else {
    try {
      $collection = anki_server_collection();
      $collection->setLanguage($language->language);
      $counts = $collection->resetScheduler();
    }
    catch (AnkiServerException $e) {
      anki_ui_show_error($e);
      return $form;
    }

    $form['#original_counts'] = $counts;
    $total_count = array_sum($counts);

    $form['study_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Study options'),
      '#tree' => TRUE,
    );

    $form['study_options']['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => array(
        // somehow explain that this is today's daily study regimin
        'normal' => t('Normal'),
        'cram'   => t('Cram'),
      ),
      '#description' => t("<em>Normal</em> will show you the cards due for today. <em>Cram</em> will randomly select cards regardless of when they are due."),
    );

    // if there are no cards for review today, we remove the option to review
    // for a 'normal' review
    if ($total_count == 0) {
      unset($form['study_options']['type']['#options']['normal']);
      $form['study_options']['type']['#description'] = t('<strong>There are no cards ready to review today!</strong> But you can still do a <em>"cram session"</em> which will randomly select cards regardless of when they are due.');
    }

    $normal_states = array(
        'visible' => array(
          ':input[name="study_options[type]"]' => array('value' => 'normal'),
        ),
    );
    $cram_states = array(
        'visible' => array(
          ':input[name="study_options[type]"]' => array('value' => 'cram'),
        ),
    );

    $form['study_options']['new_cards'] = array(
      '#type' => 'item',
      '#title' => t('New cards'),
      '#markup' => $counts['new_cards'],
      '#states' => $normal_states,
    );

    $form['study_options']['review_cards'] = array(
      '#type' => 'item',
      '#title' => t('Review cards'),
      '#markup' => $counts['review_cards'],
      '#states' => $normal_states,
    );

    $form['study_options']['dyn_cards'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of cards'),
      '#default_value' => 20,
      '#states' => $cram_states,
    );

    $form['start_quiz'] = array(
      '#type' => 'submit',
      '#value' => t('Review'),
    );
  }

  $form['#attached']['css'][] = drupal_get_path('module', 'anki_ui') . '/css/anki-ui-review.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'anki_ui') . '/js/anki-ui-review.js';

  return $form;
}

/**
 * Form validation handler for anki_ui_review_form().
 *
 * @see anki_ui_review_form()
 */
function anki_ui_review_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Finish')) {
    // We don't mark to rebuild which means the form will be regenerated from new
    return;
  }

  // We continue to rebuild the same form.
  $form_state['rebuild'] = TRUE;

  // Get the ease if the question was answered.
  $ease = isset($form_state['clicked_button']['#ease']) ? $form_state['clicked_button']['#ease'] : 0;

  // Answer the current card and fetch the next one.
  try {
    $collection = anki_server_collection();

    if ($form_state['clicked_button']['#value'] == t('Review')) {
      try {
        switch ($form_state['values']['study_options']['type']) {
          case 'normal':
            $collection->resetScheduler(!empty($form['#deck']) ? array('deck' => $form['#deck']) : array());
            break;

          case 'cram':
            $options = array(
              'count' => $form_state['values']['study_options']['dyn_cards'],
            );
            if (!empty($form['#deck'])) {
              $options['query'] = 'deck:"' . $form['#deck'] . '"';
            }
            dsm($options);
            $collection->createDynamicDeck($options);
            break;
        }
      }
      catch (AnkiServerException $e) {
        if (strpos($e->getMessage(), 'No cards matched the criteria you provided') !== FALSE) {
          drupal_set_message(t("You have no cards to review. Please add some!"), 'warning');
          return;
        }
        else {
          throw $e;
        }
      }

      // only set once we're sure that the $collection->resetScheduler(...)
      // worked.
      $form_state['storage']['study_options'] = $form_state['values']['study_options'];
    }

    // answer the last card
    if (isset($form['#card']) && $ease > 0) {
      $collection->answerCard($form['#card']['id'], $ease, $form['#card']['timerStarted']);
    }

    // get the next one
    $form_state['storage']['card'] = $collection->getNextCard();

    // if there is no card, then the session is over!
    if (empty($form_state['storage']['card'])) {

      if ($form_state['storage']['study_options']['type'] == 'normal') {
        // Show the 'finished' screen which explains that they have to come back
        // tomorrow.
        $form_state['storage']['finished'] = TRUE;
      }
      else {
        // however, in cram mode, we simply return the user to the beginning.
        $form_state['rebuild'] = FALSE;
        drupal_set_message(t('Your session has ended. Please start your review again.'));
      }
    }
  }
  catch (AnkiServerException $e) {
    anki_ui_show_error($e);
  }
}

/**
 * Form constructor for creating/editing notes in Anki.
 *
 * @see anki_ui_note_edit_form_submit()
 *
 * @ingroup forms
 */
function anki_ui_note_edit_form($form, &$form_state, $account) {
  $form['#account'] = $account;

  // TODO: For now, we're hardcoding the model in.
  $model = 'Basic';

  $form['model'] = array(
    '#type' => 'value',
    '#value' => $model,
  );

  $form['front'] = array(
    '#title' => t('Front'),
    '#type' => 'text_format',
  );
  $form['back'] = array(
    '#title' => t('Back'),
    '#type' => 'text_format',
  );
  $form['tags'] = array(
    '#title' => t('Tags'),
    '#type' => 'textfield',
    '#description' => t('A list of tags seperated by spaces.'),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submit handler for anki_ui_note_edit_form().
 *
 * @see anki_ui_note_edit_form()
 */
function anki_ui_note_edit_form_submit($form, &$form_state) {
  $account = $form['#account'];
  $collection = anki_server_collection($account->uid);

  try {
    $collection->addNote(array(
      'model' => $form_state['values']['model'],
      'fields' => array(
        'Front' => $form_state['values']['front']['value'],
        'Back' => $form_state['values']['back']['value'],
      ),
      'tags' => $form_state['values']['tags'],
    ));
  }
  catch (AnkiServerException $e) {
    anki_ui_show_exception($e);
    $form_state['rebuild'] = TRUE;
    return;
  }

  drupal_set_message(t('Flashcard added!'));
}
