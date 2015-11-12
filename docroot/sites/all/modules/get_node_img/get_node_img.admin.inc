<?php

/**
 * This file holds the configuration form for the get_node_img module.
 *
 * @package get_node_img
 * @author adnan@codesetter.com
 * @copyright Drupal Association
 * @license GPL v2
 */


/**
 * Returns a form for selecting/updating an image that's sent to the browser
 * everytime a requested image is not found.
 */
function get_node_img_404_selection()
{
    $form = array();

    // File upload elements need this.
    $form['#attributes']['enctype'] = 'multipart/form-data';

    $form['404_img'] = array(
        '#type' => 'fieldset',
        '#title' => '404 Image',
    );

    $form['404_img']['404_img_file'] = array(
        '#type' => 'file',
        '#title' => 'Image Selection',
        '#description' => 'If a new image is chosen, the current image will be
                               replaced upon submitting the form.',
        '#weight' => 9,
    );

    $form['404_img']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
        '#weight' => 12,
    );

    // Do we already have a 404 image?
    if ($img_404_fid = variable_get(GET_NODE_IMG_VAR_NAME, NULL))
    {
        $file_obj = file_load($img_404_fid);
        $img_info = array(
            'path'       => file_create_url($file_obj->uri),
            'alt'        => t('404 image'),
            'title'      => t('404 image'),
            'attributes' => '',
        );

        $form['404_img']['current_404_img'] = array(
            '#type'        => 'item',
            '#title'       => 'Current 404 Image',
            '#description' => theme('image', $img_info),
            '#weight'      => 3,
        );

        $form['404_img']['404_img_rm'] = array(
            '#type' => 'checkbox',
            '#title' => 'Delete image',
            '#return_value' => $img_404_fid,
            '#description' => 'Check this box and then submit the form to delete
                               the current 404 image.',
            '#weight' => 6,
        );
    }

    return $form;
}


/**
 * Form submit handler for 'get_node_img_404_selection' form.
 * This function handles both image deletion and image creation.
 */
function get_node_img_404_selection_submit($form, $form_values)
{
    // Is it an image delete operation?
    if ( ! empty($form_values['values']['404_img_rm']))
    {
        $img_404_fid = (int) $form_values['values']['404_img_rm'];

        variable_del(GET_NODE_IMG_VAR_NAME);

        if (file_delete(file_load($img_404_fid)))
        {
            drupal_set_message('404 image has been deleted.', 'status');
        }
        else
        {
            drupal_set_message('404 image couldn\'t be deleted!', 'status');
        }

        return;
    }

    // The uploaded file must be an image.
    $file_validators = array(
        'file_validate_is_image' => array(),
    );

    $file_dir = variable_get('file_default_schema', 'public') . '://';
    $file = file_save_upload('404_img_file', $file_validators,
                $file_dir, FILE_EXISTS_REPLACE);
    if ($file)
    {
        // The uploaded file is still a temporary file.  Make it permanent.
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);

        variable_set(GET_NODE_IMG_VAR_NAME, $file->fid);
    }
    else
    {
        form_set_error('404_img_file', 'Upload error!');
    }
}


