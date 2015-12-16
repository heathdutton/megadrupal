<?php
// $Id$
/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright � 2009-2012 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com -or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */

/**
 * @file
 * Defines the API to extend the Most Popular AddThis.com integration.
 */

/**
 * Defines hook_mostpopular_addthis_service_options_alter().
 * 
 * Alters the list a list of the available services for getting metrics from AddThis.com.
 * 
 * @param array
 *   An array of sharing services.  Add your own options to this list. 
 *   The key must match one of the service keys at http://www.addthis.com/services/list.
 *   The value is the name to display in the options drop-down.
 */
function hook_mostpopular_addthis_service_options_alter(&$services) {
  $services['gmail'] = t('Gmail');
  
  // NOTE: You can aggregate several keys by separating them with commas.
  $services['email,mailto'] = t('Email');
  $services['facebook,facebook_like'] = t('Facebook');
}
