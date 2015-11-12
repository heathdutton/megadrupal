<?php

/**
 * @file
 * Libanswers non-admin pages
 */


/**
 * Page callback function from hook_menu().
 * The URL will pass in parameters. The 1st should be the question ID.
 */
function libanswers_page_get_answer($question_id) {
  // Add the search form at the top of the search results (without the tag cloud).
  $output = render(drupal_get_form('libanswers_search_form'));

  if (!empty($question_id) && is_numeric($question_id)) {
    $answer = file_get_contents('http://api.libanswers.com/api_qa.php?iid=' . LIBANSWERS_ID . '&qid=' . $question_id . '&css=1&format=html');
    $answer = strip_tags($answer, '<p><a><ul><li><ol><br><div><span><link><h2><h3><h4>');
    $answer = str_replace('font-family: tahoma,arial,helvetica,sans-serif;', '', $answer);
    $answer = str_replace('font-family: arial,helvetica,sans-serif;', '', $answer);
    $answer = str_replace('font-size: medium;', '', $answer);
    $answer = str_replace('color: black;', '', $answer);
    $answer = str_replace('font-size: small;', '', $answer);
    $answer = str_replace('font-family: \'Cambria\',\'serif\';', '', $answer);
    $answer = str_replace(' style=" "', '', $answer);
    $answer = str_replace(LIBANSWERS_DOMAIN . '/browse.php?tid=', '/ask-us/topic/', $answer);
    if (isset($_SERVER['HTTPS'])) {
      $answer = str_replace('<link rel="stylesheet" type="text/css" href="http:', '<link rel="stylesheet" type="text/css" href="https:', $answer);
    }
    if (module_exists('filter')) {
      $answer = _filter_htmlcorrector($answer);
    }
    $output .= $answer;
  }
  else {
    $output .= t('No answer was found for the given question.');
  }

  $header = FALSE; // Show the secondary header text for answer pages.
  // Add the question form.
  $output .= libanswers_get_question_form($header);
  // Add the tag cloud at the bottom of the search results.
  $output .= libanswers_get_tag_cloud();

  return $output;
}

/**
 * Page callback function from hook_menu().
 * Shows the knowledgebase type search form.
 */
function libanswers_search_form() {
  $minimal_domain = str_replace('http://', '', str_replace('https://', '', LIBANSWERS_DOMAIN));
  $output = '';
  $output .= '<link rel="stylesheet" href="//ajax.googleapis.com/ajax/libs/jqueryui/1.8.11/themes/base/jquery-ui.css">' . "\n";
  $output .= '<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>' . "\n";
  $output .= '<script src="//ajax.googleapis.com/ajax/libs/jqueryui/1.8.11/jquery-ui.min.js"></script>' . "\n";
  $output .= '<script type="text/javascript">
    (function ($) {
    $(document).ready(function(){

      $("#edit-libanswers-autocomplete").autocomplete({
        source: function(request, response) {
          $.ajax({
            url: "//api.libanswers.com/api_query.php",
            dataType: "jsonp",
            data: {
							iid: ' . LIBANSWERS_ID . ',
							limit: 20,
							format: "json",
							q: request.term
						},
            success: function(data) {
              //console.log(data.query.results);
              response($.map(data.query.results, function(item) {
                return {
									label: item.question,
									value: item.question,
									qid: item.qid
								}
              }));
            }
          });
        },
        minLength: 3,
        select: function(event, ui) {
          if (ui.item.qid > 0) {
						window.open("/ask-us/answer/"+ui.item.qid, "_self");
					}
					else {
						return false;
					}
        },
        open: function() {
          $(this).removeClass("ui-corner-all").addClass("ui-corner-top");
          // Fix the position when the list is opened.
          var position = $("#edit-libanswers-autocomplete").offset(), left = position.left, top = position.top;
          var height = $("#edit-libanswers-autocomplete").height();
          $("ul.ui-autocomplete").css({
            left: left + "px",
            top: top + height + 4 + "px"
          });
        },
        close: function() {
          $(this).removeClass("ui-corner-top").addClass("ui-corner-all");
        }
      });
      // Below is just a fix for an error with jquery UI.
      // $.curCSS is not a function
      $.curCSS = $.css;
    });
    })(jQuery);
  </script>';
  $form['#prefix'] = $output;
  $form['libanswers_autocomplete'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#size' => 40,
    '#prefix' => '<div class="ui-widget">',
    '#suffix' => '</div>',
    '#default_value' => t('Search Frequently Asked Questions'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  if ($_GET['q'] == 'ask-us') {
    $form['#suffix'] = '';
    $form['#suffix'] .= libanswers_get_question_form();
    $form['#suffix'] .= libanswers_get_tag_cloud();
    $form['#suffix'] .= '<h2>Featured Answers</h2>';
    $questions = file_get_contents('http://api.libanswers.com/api_answers.php?iid=' . LIBANSWERS_ID . '&type=featured&limit=10&showdet=0&showans=0&break=li&format=html');
    $questions = str_replace(' target="_blank"', '', $questions);
    $questions = str_replace(LIBANSWERS_DOMAIN . '/a.php?qid=', '/ask-us/answer/', $questions);
    $form['#suffix'] .= $questions;
    $libanswers_description = variable_get('libanswers_description');
    if (!empty($libanswers_description)) {
      $form['#suffix'] .= $libanswers_description;
    }
  }
  return $form;
}

/**
 * Submit handler for form ID 'libanswers_search_form'.
 */
function libanswers_search_form_submit($form, &$form_state) {
  // Redirect to results page.
  $form_state['redirect'] = array(
    'ask-us/search-results', array('query' => array(
      'query' => $form_state['values']['libanswers_autocomplete'],
    ))
  );
}

/**
 * Page title callback function from hook_menu().
 * The URL will pass in parameters. The 1st should be the topic ID.
 */
function libanswers_page_get_topic_title($topic_id) {
  // Pull from 24 hr cache.
  $cid = 'libanswers:topics';
  $cached = cache_get($cid, 'cache');
  if ($cached && time() < $cached->expire) {
    $topics_json = $cached->data;
  }
  else {
    // Not in cache. Let's run the query.
    $topics_json = file_get_contents('http://api.libanswers.com/api_topics.php?iid=' . LIBANSWERS_ID . '&list=cloud&sort=name&count=0&limit=100&incempty=0&minsize=80&maxsize=150&format=json');
    cache_set($cid, $topics_json, 'cache', time() + 86400); // Cache the query result for 24 hours.
  }
  $topics_object = json_decode($topics_json);
  foreach ($topics_object->topics as $topic) {
    if ($topic->id == $topic_id) {
      return ucwords($topic->name);
    }
  }
}

/**
 * Page callback function from hook_menu().
 */
function libanswers_page_get_topic_list() {
  // Add the search form at the top of the search results (without the tag cloud).
  $output = render(drupal_get_form('libanswers_search_form'));

  $topics = file_get_contents('http://api.libanswers.com/api_topics.php?iid=' . LIBANSWERS_ID . '&list=list&sort=name&count=1&limit=100&incempty=0&break=li&format=html');
  $topics = str_replace(LIBANSWERS_DOMAIN . '/browse.php?tid=', '/ask-us/topic/', $topics);
  $output .= $topics;

  return $output;
}

/**
 * Page callback function from hook_menu().
 * The URL will pass in parameters. The 1st should be the topic ID.
 */
function libanswers_page_get_topic_answers($topic_id) {
  // Add the search form at the top of the search results (without the tag cloud).
  $output = render(drupal_get_form('libanswers_search_form'));

  if (!empty($topic_id) && is_numeric($topic_id)) {
    $answers = file_get_contents('http://api.libanswers.com/api_answers.php?iid=' . LIBANSWERS_ID . '&type=topic&id=' . $topic_id . '&sort=popular&limit=20&showdet=0&showans=0&break=li&format=html');
    $answers = strip_tags($answers, '<p><a><ul><li><ol><br><div><span><link><h2><h3><h4>');
    $answers = str_replace(LIBANSWERS_DOMAIN . '/a.php?qid=', '/ask-us/answer/', $answers);
    if (module_exists('filter')) {
      $answers = _filter_htmlcorrector($answers);
    }
    $output .= '<h2>' . t('Answers related to: "') . libanswers_page_get_topic_title($topic_id) . '"</h2>' . $answers;
  }
  else {
    $output .= t('No answers were found for the given topic.');
  }

  // Add the question form.
  $output .= libanswers_get_question_form();
  // Add the tag cloud at the bottom of the search results.
  $output .= libanswers_get_tag_cloud();

  return $output;
}

/**
 * Page callback function from hook_menu().
 */
function libanswers_search_results() {
  // Add the search form at the top of the search results (without the tag cloud).
  $output = render(drupal_get_form('libanswers_search_form'));

  $query = check_plain($_REQUEST['query']);
  $query_encoded = urlencode(urldecode($query)); // Get it into the right format for the LibAnswers API submission.
  if (!empty($query_encoded)) {
    $answers = file_get_contents('http://api.libanswers.com/api_query.php?iid=' . LIBANSWERS_ID . '&q=' . $query_encoded . '&limit=20&format=html');
    $answers = strip_tags($answers, '<p><a><ul><li><ol><br><div><span><link><h2><h3><h4>');
    $answers = str_replace(LIBANSWERS_DOMAIN . '/a.php?qid=', '/ask-us/answer/', $answers);
    if (module_exists('filter')) {
      $answers = _filter_htmlcorrector($answers);
    }
    $output .= t('<h2>Answers related to: "') . $query . '"</h2>' . $answers;
  }
  else {
    $output .= t('No answers were found for the given search.');
  }

  // Add the question form.
  $output .= libanswers_get_question_form();
  // Add the tag cloud at the bottom of the search results.
  $output .= libanswers_get_tag_cloud();

  return $output;
}

/**
 * Page callback function from hook_menu().
 * Same as the function above, but only prints the HTML and exits. No themed page is created. Just a partial page for use by the Javascript.
 * e.g. /ask-us/search-results-unthemed?query=hours
 */
function libanswers_search_results_unthemed() {
  $query = check_plain($_REQUEST['query']);
  $query_encoded = urlencode(urldecode($query)); // Get it into the right format for the LibAnswers API submission.
  if (!empty($query_encoded)) {
    $answers = file_get_contents('http://api.libanswers.com/api_query.php?iid=' . LIBANSWERS_ID . '&q=' . $query_encoded . '&limit=20&format=html');
    $answers = strip_tags($answers, '<p><a><ul><li><ol><br><div><span><link><h2><h3><h4>');
    $answers = str_replace(LIBANSWERS_DOMAIN . '/a.php?qid=', '/ask-us/answer/', $answers);
    if (module_exists('filter')) {
      $answers = _filter_htmlcorrector($answers);
    }
    if (strstr($answers, 'No results found') == FALSE) {
      print '<h2>' . t('Is this what you need?') . '</h2>' . $answers . '<p><a href="/ask-us">' . t('Show me more') . ' &raquo;</a></p><a href="javascript:;" class="button" id="libanswers-please-send">' . t('No. Please send.') . '</a>';
    }
    else {
      print '<h2>' . t('No Answers found') . '</h2><p>' . t('No relevant answers to your question were found in the system.') . '<p><a href="/ask-us">' . t('Show me other answers') . ' &raquo;</a></p><a href="javascript:;" class="button" id="libanswers-please-send">' . t('Please send my question.') . '</a>';
    }
  }
  else {
    print t('No answers were found for the given search.');
  }
  drupal_exit();
}

/**
 * Page callback function from hook_menu().
 * Only prints the HTML and exits. No themed page is created. Just a partial page for use by the Javascript.
 * e.g. /ask-us/block-unthemed
 */
function libanswers_block_unthemed() {
  print libanswers_get_answers_block();
  drupal_exit();
}
