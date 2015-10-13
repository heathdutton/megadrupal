<?php

/**
 * @file
 * Validator functions for the Affiliates Suite Themekey module.
 */

/*
 * name: ms_affiliates_themekey_validator_aid
 * Validates a Theme Switching Rule that depends on the affiliate ID of Moneysuite Affiliates Suite.
 * Allowed Operators: "=", "!"
 * Allowed values: string of digits (numbers)
 *
 *
 * @param $rule
 *   A Theme Switching Rule as associative array:
 *   - property: ThemeKey property as string (e.g., "drupal:path")
 *   - wildcard: optional string, only used if property is "drupal:path:wildcard"
 *   - operator: ThemeKey operator as string ("=", "!", "<". "<=", ">", ">=", "~")
 *   - value: ThemeKey property value as string
 *
 * @return
 *   An associative array of errors:
 *   - property: translated error message as string
 *     describing a problem with the property
 *   - wildcard: translated error message as string
 *     describing a problem with the wildcard
 *   - operator: translated error message as string
 *     describing a problem with the operator
 *   - value: translated error message as string
 *     describing a problem with the value
 *   If no errors detected the array is empty.
 * 
 */
function ms_affiliates_themekey_validator_aid($rule){
	$errors = array();
	
	switch ($rule['operator']) {
		case '*':
		case '!*':
		case '<':
		case '<=':
		case '>':
		case '>=':
		case '~':
		case '!~':
			$errors['operator'] = t('Possible operators are "=", "!"');
			break;
		case '=':
			if ('0' === $rule['value']) {
				$errors['value'] = t('Use a value different from "0" to compare to an affiliate id');
			}
			elseif (!ctype_digit($rule['value'])) {
				$errors['value'] = t('Affiliate id value must be a number');
			}
			break;
		case '!':
			if ('0' !== $rule['value']) {
				$errors['value'] = t('Use value "0" to identify all users with affiliation');
			}
			break;
		default:
			$errors['property']=t('Unknown error');
	}
	return $errors;
}
