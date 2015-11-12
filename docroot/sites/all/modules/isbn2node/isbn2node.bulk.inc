<?php
/**
 * @file
 * Functions for bulk import of ISBN's.
 *
 */


/*
 * Present bulk ISBN form
 */
function isbn2node_bulk_form() {
  $form = array();
  
  if (_isbn2node_check_settings($form)) {
    $form['isbn_bulk'] = array(
      '#title' => t('ISBN\'s'),
      '#type' => 'textarea',
      '#required' => TRUE,
      '#description' => t('Enter a series of ISBN\'s, one on each line.'),
    );

    $form['get_images'] = array(
      '#type' => 'checkbox',
      '#title' => t('Get cover images where possible'),
      '#default_value' => 1,
    );
    
    // Buttons
    $form['buttons']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#weight' => 140,
      );
  
    $form['buttons']['cancel'] = array(
      '#markup' => l(t('Cancel'), 'node/add'),
      '#weight' => 150,
      );
  }
  
  return $form;
}


/*
 * Submit handler for the add bulk ISBN's form
 * Fetches book data for each ISBN and creates a node for each
 */
function isbn2node_bulk_form_submit($form, &$form_state) {
  // Fetch book data for a list of ISBN's
  
  $isbns = explode("\n", $form_state['values']['isbn_bulk']);
  
  foreach ($isbns as  $isbn) {
    // Fetch book data
    $isbn = _isbn2book_normalize_isbn($isbn);
    if ($isbn) {
      // Check for existing book on ISBN
      $result = _isbn2node_isbn_exists($isbn);

      if (!$result) {
        $vals = _isbn2book_fetch_data($isbn);
        if (is_array($vals)) {
          $success = $vals['success'];
        }
        else {
          $success = $vals->success;
        }

        if (!$success) {
          drupal_set_message(t('Didn\'t find any results on the ISBN @isbn', array('@isbn' => $isbn)));
        }
        else {
          $isbn2book_data = _isbn2node_parse($vals, $isbn);
          if ($isbn2book_data) {
            // Insert ISBN
            $isbn2book_data['isbn2node_isbn'] = $isbn;
            
            // Fetch cover and create file object
            if ($form_state['values']['get_images']) {
              if (isset($isbn2book_data['xisbn'])) {
                $xisbn = $isbn2book_data['xisbn'];
              }
              else {
                $xisbn = '';
              }
              $file = _isbn2node_save_cover($isbn, $xisbn);
              if ($file->fid) {
                $isbn2book_data['isbn2node_cover'] = $file;
              }
            }

            // Create and save the node
            _isbn2node_make_node($isbn2book_data);
          }
        }
      }
      else {
        drupal_set_message(t('You already have <a href="@view">a book with the ISBN @isbn</a>.<br /><a href="@edit">Edit it here</a>, or <a href="@delete">delete it</a> before adding it again.',
        array(
          '@isbn' => $isbn,
          '@view' => url('node/' . $result['entity_id']),
          '@edit' => url('node/' . $result['entity_id'] . '/edit'),
          '@delete' => url('node/' . $result['entity_id'] . '/delete'))),
          'warning');
      }
    }
  }
}
