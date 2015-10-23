<?php
function swipe_photo_gallery_field_info() {
    $fields['field_slide_photo'] = array(
        'label' => t('Slide'),
        'description' => t('gallery slide photo'),
        'settings' => array(),
        'instance_settings' => array(),
    );
    $fields['field_slide_weight'] = array(
        'label' => t('Position'),
        'description' => t('Slide position in album'),
        'settings' => array(),
        'instance_settings' => array(),
    );
    $fields['field_tax_series'] = array(
        'label' => t('Album'),
        'description' => 'Album name',
        'settings' => array(),
        'instance_settings' => array(),
        'default_formatter' => 'plain',
    );
    $fields['field_main_slide_position'] = array(
        'label' => t('Horisontal position of albums cover'),
        'description' => '',
        'settings' => array(),
        'instance_settings' => array(),
        'default_formatter' => 'plain',
    );
    $fields['field_first_slide'] = array(
        'label' => t('First Slide'),
        'description' => t('Albums first slide'),
        'settings' => array(),
        'instance_settings' => array(),
    );
    return $fields; # Get all field types with: field_info_field_types()
}
