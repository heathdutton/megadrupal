<?php

/**
 * @file
 * API documentation for hooks.
 */


/**
 * Implements this hook to use other validation to check the id true or false.
 *
 * If you use this hook to validate the value. the default validator
 * in this module would be skipped.
 *
 * @param $chinese_identity_card
 *
 * @return bool
 */
function hook_chinese_identity_card_validate($chinese_identity_card) {
  return TRUE;
}