<?php

function mytinytodo_views_data()
{
    $data = array();

    $data['mytinytodo_lists'] = array(
        'table' => array(
        'group' => t('ToDo List'),
    ));

    $data['mytinytodo_lists']['name'] = array(
        'title' => 'Task List',
        'help' => t('The task list title.'),

        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_lists']['d_created'] = array(
        'title' => 'Created date',
        'help' => t('The date the task list was created.'),

        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_date',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_lists']['d_edited'] = array(
        'title' => 'Updated date',
        'help' => t('The date the task list was last updated.'),

        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_date',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_lists']['published'] = array(
        'title' => t('RSS Published'),
        'help' => t('Whether the task lists\'s RSS feed is enabled.'),

        'field' => array(
            'handler' => 'views_handler_field_boolean',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_boolean_operator',
            'label' => t('Active'),
            'type' => 'yes-no',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_lists']['table']['join']['node'] = array(
        'left_table' => 'mytinytodo_fields',
        'left_field' => 'id',
        'field' => 'field_id',
    );

    $data['mytinytodo_fields']['table']['join']['node'] = array(
        'left_field' => 'nid',
        'field' => 'entity_id',
    );


    $data['mytinytodo_todos'] = array(
        'table' => array(
        'group' => t('ToDo Item'),
    ));

    $data['mytinytodo_todos']['title'] = array(
        'title' => 'Todo Item',
        'help' => t('The todo item.'),

        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_todos']['compl'] = array(
        'title' => t('Completed'),
        'help' => t('Whether the todo item is completed.'),

        'field' => array(
            'handler' => 'views_handler_field_boolean',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_boolean_operator',
            'label' => t('Completed'),
            'type' => 'yes-no',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_todos']['d_created'] = array(
        'title' => 'Created date',
        'help' => t('The date the todo item was created.'),

        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_date',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_todos']['d_edited'] = array(
        'title' => 'Updated date',
        'help' => t('The date the todo item was last updated.'),

        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_date',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_todos']['d_completed'] = array(
        'title' => 'Updated date',
        'help' => t('The date the todo item was completed.'),

        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_date',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_todos']['note'] = array(
        'title' => 'Note',
        'help' => t('The todo item note.'),

        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );

    $data['mytinytodo_todos']['prio'] = array(
        'title' => 'Priority',
        'help' => t('The todo item priority.'),

        'field' => array(
            'handler' => 'views_handler_field_todo_priority',
            'click sortable' => TRUE,
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
    );
    $data['mytinytodo_todos']['duedate'] = array(
        'title' => 'Due Date',
        'help' => t('The date the task list is due.'),

        'field' => array(
            'handler' => 'views_handler_field_duedate',
            'click sortable' => TRUE,
            ),
        'filter' => array(
            'handler' => 'views_handler_filter_duedate',
            ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
            ),
        'sort' => array(
            'handler' => 'views_handler_sort_duedate',
            ),
    );

    $data['mytinytodo_todos']['table']['join']['node'] = array(
        'left_table' => 'mytinytodo_lists',
        'left_field' => 'id',
        'field' => 'list_id',
    );

    return $data;
}
