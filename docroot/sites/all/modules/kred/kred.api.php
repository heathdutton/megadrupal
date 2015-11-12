<?php

/**
 * @file
 * Describe functions provided by Kred module.
 *
 * Please look at the following links in order to have the correct API
 * configuration:
 *
 * - https://developer.peoplebrowsr.com/kred/kredscore
 * - https://developer.peoplebrowsr.com/kred/kredcommunity
 * - https://developer.peoplebrowsr.com/kred/kredentials
 */

/**
 * Connects to Kred and fecth Kred Score information.
 *
 * @see kred_get_users_information()
 */
kred_get_users_information('kred_score', array('123456789'));
