<?php

/**
 * @file
 * Describes hooks defined by SimpleCrop module.
 */

/**
 * Triggers when new crop has been saved.
 *
 * @param $crop
 *   Object with crop information. Contains:
 *   $crop->uri      - URI of image which was cropped.
 *   $crop->data     - Information about crop (coordinates, etc).
 *   $crop->original - Info about crop of the same image before
 *                     save (relevant only for updates).
 */
function hook_simplecrop_crop_insert($crop) {

}

/**
 * Triggers when crop has been updated.
 *
 * @param $crop
 *   Object with crop information. Contains:
 *   $crop->uri      - URI of image which was cropped.
 *   $crop->data     - Information about crop (coordinates, etc).
 *   $crop->original - Info about crop of the same image before save.
 */
function hook_simplecrop_crop_update($crop) {

}

/**
 * Triggers when crop object has been loaded from database.
 *
 * @param $crop
 *   Object with crop information. Contains:
 *   $crop->uri      - URI of image which was cropped.
 *   $crop->data     - Information about crop (coordinates, etc).
 */
function hook_simplecrop_crop_load($crop) {

}

/**
 * Triggers before crop has been deleted from the database.
 *
 * @param $crop
 *   Object with crop information. Contains:
 *   $crop->uri      - URI of image which was cropped.
 *   $crop->data     - Information about crop (coordinates, etc).
 */
function hook_simplecrop_crop_delete($crop) {

}
