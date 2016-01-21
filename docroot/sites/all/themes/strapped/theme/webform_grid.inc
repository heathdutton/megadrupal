<?php

function strapped_webform_grid($variables) {
    $element = $variables['element'];
    $right_titles = _webform_grid_right_titles($element);

    $rows = array();
    $title = array('data' => '', 'class' => array('webform-grid-question'));
    $header = array($title);
    // Set the header for the table.
    foreach ($element['#grid_options'] as $option) {
        $header[] = array('data' => webform_filter_xss($option), 'class' => array('checkbox', 'webform-grid-option'));
    }
    if ($right_titles) {
        $header[] = $title;
    }

    foreach (element_children($element) as $key) {
        $question_element = $element[$key];
        $question_titles = explode('|', $question_element['#title'], 2);

        // Create a row with the question title.
        $title = array('data' => webform_filter_xss($question_titles[0]), 'class' => array('webform-grid-question'));
        $row = array($title);

        // Render each radio button in the row.
        $radios = form_process_radios($question_element);
        foreach (element_children($radios) as $key) {
            $radio_title = $radios[$key]['#title'];
            $radios[$key]['#title'] = $question_element['#title'] . ' - ' . $radio_title;
            $radios[$key]['#title_display'] = 'invisible';
            // @strapped - Remove some wrappers which add padding.
            $radios[$key]['#form_group_wrapper'] = FALSE;
            $row[] = array('data' => drupal_render($radios[$key]), 'class' => array('checkbox', 'webform-grid-option'), 'data-label' => array($radio_title));
        }
        if ($right_titles) {
            $row[] = array('data' => isset($question_titles[1]) ? webform_filter_xss($question_titles[1]) : '', 'class' => array('webform-grid-question'));
        }
        $rows[] = $row;
    }

    $option_count = count($header) - 1;
    return theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => $element['#sticky'], 'attributes' => array('class' => array('webform-grid', 'webform-grid-' . $option_count))));
}
