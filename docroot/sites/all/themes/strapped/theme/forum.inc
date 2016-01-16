<?php

/**
 * Theme function to display a link, optionally buttonized.
 */
function strapped_advanced_forum_l(&$variables) {
    $text = $variables['text'];
    $path = empty($variables['path']) ? NULL : $variables['path'];
    $options = empty($variables['options']) ? array() : $variables['options'];
    $button_class = empty($variables['button_class']) ? NULL : $variables['button_class'];

    $l = '';
    if (!isset($options['attributes'])) {
        $options['attributes'] = array();
    }
    if (!is_null($button_class)) {

        $bootstrap_button_class = array(
            'large' => 'lg',
            'medium' => 'md',
            'small' => 'sm'
        );

        // Buttonized link: add our button class and the span.
        if (!isset($options['attributes']['class'])) {
            $options['attributes']['class'] = array("btn", "btn-primary", "btn-$bootstrap_button_class[$button_class]");
        }
        else {
            $options['attributes']['class'][] = "btn";
            $options['attributes']['class'][] = "btn-primary";
            $options['attributes']['class'][] = "btn-$bootstrap_button_class[$button_class]";
        }
        $options['html'] = TRUE;
        // @codingStandardsIgnoreStart
        $l = l('<span>' . $text . '</span>', $path, $options);
        // @codingStandardsIgnoreEnd
    }
    else {
        // Standard link: just send it through l().
        $l = l($text, $path, $options);
    }

    return $l;
}


/**
 * Theme function to show list of types that can be posted in forum.
 */
function strapped_advanced_forum_node_type_create_list(&$variables) {
    $forum_id = $variables['forum_id'];

    // Get the list of node types to display links for.
    $type_list = advanced_forum_node_type_create_list($forum_id);

    $output = '';
    if (is_array($type_list)) {
        foreach ($type_list as $type => $item) {
            $output .= '<div class="forum-add-node forum-add-' . $type . '">';
            $output .= theme('advanced_forum_l', array(
                'text' => t('New @node_type', array('@node_type' => $item['name'])),
                'path' => $item['href'],
                'options' => NULL,
                'button_class' => 'medium',
            ));
            $output .= '</div>';
        }
    }
    else {
        // User did not have access to create any node types in this fourm so
        // we just return the denial text / login prompt.
        $output = $type_list;
    }

    return $output;
}


function _forum_form_alter(&$form, &$form_state, $form_id) {


    if ($form_id == 'forum_node_form') {

        $form['title']['#form_type'] = 'horizontal';
        $form['title']['#title_display'] = 'inline';
        $form['title']['#title_grid'] = array(
          'columns' => array(
              'md' => '2'
          )
        );
        $form['title']['#element_grid'] = array(
            'columns' => array(
                'md' => '6'
            )
        );

        $form['taxonomy_forums']['und']['#form_type'] = 'horizontal';
        $form['taxonomy_forums']['und']['#title_display'] = 'inline';
        $form['taxonomy_forums']['und']['#title_grid'] = array(
            'columns' => array(
                'md' => '2'
            )
        );
        $form['taxonomy_forums']['und']['#element_grid'] = array(
            'columns' => array(
                'md' => '3'
            )
        );


        $form['body']['und'][0]['#form_type'] = 'horizontal';
        $form['body']['und'][0]['#title_display'] = 'inline';
        $form['body']['und'][0]['#title_grid'] = array(
            'columns' => array(
                'md' => '2'
            )
        );
        $form['body']['und'][0]['#element_grid'] = array(
            'columns' => array(
                'md' => '8'
            )
        );


    }

}