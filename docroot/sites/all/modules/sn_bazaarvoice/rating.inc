<?php

/**
 * @file
 * Rating related methodolgy.
 * 
 * This file would have all bazaarvoice related functions.
 */

/**
 * This method submits bazaarvoice review.
 */
function sn_bazaarvoice_review_submission($form, $form_state) {
  $data = $form_state['values'];
  $response = '';

  if (isset($data['type'])) {
    $key = variable_get('sn_bazaarvoice_bzrvoice_consumer_key');
    $api = variable_get('sn_bazaarvoice_bzrvoice_api_version');

    switch ($data['type']) {
      // To fetch reviews.
      case 'fetch':
        $url = variable_get('sn_bazaarvoice_bzrvoice_reviews_fetching_url');
        $params = array(
          'passkey' => $key,
          'apiversion' => $api,
        );

        if (isset($data['products'])) {
          $params['Filter'] = 'ProductId:' . $data['products'];
        }

        $query_string = drupal_http_build_query($params);
        $url .= "?" . $query_string;

        // API call to fetch reviews.
        $response = drupal_http_request($url, array('method' => 'GET'));

        break;

      // To submit a review.
      case 'submit':
        global $user;
        // Getting post related variables.
        $review_title = isset($data['review_title']) ? $data['review_title'] : '';
        $review_text = isset($data['review_text']) ? $data['review_text'] : '';
        $rating = isset($data['rating']) ? $data['rating'] : '';
        $product_id = isset($data['product_id']) ? $data['product_id'] : '';
        $nid = isset($data['nid']) ? $data['nid'] : '';
        $url = variable_get('sn_bazaarvoice_bzrvoice_reviews_submission_url');
        $params = array(
          'passkey' => $key,
          'apiversion' => $api,
          'UserId' => $user->name,
          'ReviewText' => strip_tags($review_text),
          'Title' => strip_tags($review_title),
          'Rating' => $rating,
          'usernickname' => $user->name,
          'action' => 'submit',
          'ProductId' => $product_id,
        );

        $query_string = drupal_http_build_query($params);
        $url .= "?" . $query_string;
        // API call to fetch reviews.
        $response = drupal_http_request($url, array('method' => 'POST'));
        // Saving review to database.
        sn_bazaarvoice_save_review($response, $nid);

        break;
    }
  }
}

/**
 * This method would create a review form for a node.
 * 
 * @ingroup forms
 */
function sn_bazaarvoice_review_form() {
  $form = array();

  $form['panel'] = array(
    '#title' => t('Submit your review'),
    '#type' => 'fieldset',
  );

  $form['panel']['rating_widget'] = array(
    '#prefix' => '<div class="form-item"><label>' . t('Provide Rating') . '</label>',
    '#suffix' => '</div>',
    '#title' => t('Provide Rating'),
    '#markup' => theme('sn_bazaarvoice_rating_widget', array(
      'nid' => arg(1),
      'full_node_class' => 'active',
      )
    ),
  );

  $form['panel']['review_title'] = array(
    '#title' => t('Review Title'),
    '#description' => t('Provide review title.'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['panel']['review_text'] = array(
    '#title' => t('Provide your review'),
    '#type' => 'textarea',
    '#required' => TRUE,
    '#resizable' => FALSE,
  );

  $form['panel']['type'] = array(
    '#type' => 'hidden',
    '#value' => 'submit',
  );

  $form['panel']['rating'] = array(
    '#type' => 'hidden',
    '#attributes' => array('class' => 'rating-value'),
  );

  $nodes = node_load(arg(1));
  $form['panel']['product_id'] = array(
    '#type' => 'hidden',
    '#value' => 'node-' . $nodes->type . '-' . $nodes->nid,
  );

  $form['panel']['nid'] = array(
    '#type' => 'hidden',
    '#value' => $nodes->nid,
  );

  $form['panel']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit your review'),
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'sn_bazaarvoice') . '/styles/rating.css',
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'sn_bazaarvoice') . '/scripts/rating.js',
  );
  $form['#submit'] = array('sn_bazaarvoice_review_submission');
  $form['#validate'][] = 'sn_bazaarvoice_review_validate';

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function sn_bazaarvoice_review_validate(&$form, $form_state) {
  $values = $form_state['values'];

  if ($values['rating'] < 1) {
    form_set_error('rating', t('Please provide rating to your review.'));
  }

  if (drupal_strlen($values['review_text']) < 40) {
    form_set_error('review_text', t('Review text cannot be less than 40 characters.'));
  }
}

/**
 * This method would store values for rating in database.
 * 
 * @param mixed $response
 *   Response retrieved from bazaarvoice API call
 * 
 * @param number $nid
 *   Node ID for which review is submitted.
 */
function sn_bazaarvoice_save_review($response, $nid) {
  $data = drupal_json_decode($response->data);

  if (empty($data['Errors']) && empty($data['FormErrors'])) {
    // Insert values to table.
    global $user;
    $nid = db_insert('sn_bazaarvoice_nodes_rating')
      ->fields(array(
        'nid' => $nid,
        'uid' => $user->uid,
        'rating' => $data['Review']['Rating'],
      ))
      ->execute();

    drupal_set_message(t('Your review has been successfully submitted.'), 'status', FALSE);
  }
}

/**
 * This method would get rating for current node.
 * 
 * @param number $nid
 *   Node id for which rating needs to be fetched.
 * 
 * @return number 
 *   Average rating for the node.
 */
function sn_bazaarvoice_get_rating($nid) {
  $query = db_select('sn_bazaarvoice_nodes_rating', 'n')->condition('nid', $nid);
  $query->addExpression('AVG(rating)', 'average');
  $result = $query->execute()->fetch(PDO::FETCH_ASSOC);

  return round($result['average'], 2);
}

/**
 * Fetch reviews from bazaarvoice.
 *
 * @param mixed $node
 *   Node variable.
 */
function sn_bazaarvoice_fetch_reviews($node) {
  $product_id = "node-" . $node->type . "-" . $node->nid;
  $key = variable_get('sn_bazaarvoice_bzrvoice_consumer_key');
  $api = variable_get('sn_bazaarvoice_bzrvoice_api_version');
  $url = variable_get('sn_bazaarvoice_bzrvoice_reviews_fetching_url');

  $query_string = drupal_http_build_query(
    array(
      'passkey' => $key,
      'apiversion' => $api,
    )
  );
  $url .= "?" . $query_string . "&Filter=ProductId:" . $product_id;

  // API call to fetch reviews.
  $response = drupal_http_request($url, array('method' => 'GET'));
  return theme('sn_bazaarvoice_reviews_list', array('reviews' => $response, 'nid' => $node->nid));
}
