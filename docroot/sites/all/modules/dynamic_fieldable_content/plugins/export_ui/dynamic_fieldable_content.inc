<?php

$plugin = array(
    'schema' => 'dynamic_fieldable_content',
    'access' => 'administer dynamic content',

    'menu' => array(
        'menu item' => 'dynamic-content',
        'menu title' => 'Dynamic content panes',
        'menu description' => 'Add, edit or delete dynamic content panes.',
    ),

    'title singular' => t('fieldable content pane'),
    'title singular proper' => t('Fieldable Content pane'),
    'title plural' => t('fieldable content panes'),
    'title plural proper' => t('Fieldable Content panes'),

    'handler' => 'dynamic_fieldable_content_ui',
);

