<?php

$variables = array(
    'path' => $glazed_theme_path . 'features/sooper-header/sooper-header-placeholder.png',
    'alt' => 'Glazed Premium Feature Placeholder',
    'attributes' => array('class' => 'glazed-placeholder'),
    );

$form['glazed_settings']['header'] = array(
  '#title' => t('Header'),
  '#type' => 'fieldset',
  '#description' => '<div class="messages status">' . t('Love the free edition of Glazed Theme? Join SooperThemes and get the <a href="http://bit.ly/1HITXJu" target="_blank">full edition of Glazed</a>, with more features, frequent updates, sub-themes, drag and drop site building and more!') . '</div><a href="http://bit.ly/1HITXJu" target="_blank"><br>' . theme('image', $variables) . '</a>',
);