$Id$

Ubercart eWay Payment Gateway
================

----------------
Description
----------------

This module integrates the Australian payment gateway eWay to Ubercart.
http://www.eway.com.au

Note that the recurring payments implementation uses eWAY's token payments
(http://www.eway.com.au/Developer/eway-api/token-payments.aspx)
instead of their recurring payments
(http://www.eway.com.au/Developer/eway-api/recurring-payments.aspx)
because it integrates into the uc_recurring framework better.

---------------
Installation
---------------

- Create a folder in your modules directory called uc_eway and put the module's files in this directory

- Use of recurring billing (ReBill) requires the NuSOAP library available at http://sourceforge.net/projects/nusoap/
  1. Install the Libraries API module (http://drupal.org/project/libraries)
  2. Install the NuSOAP library to the module's directory so that the directory structure looks like this:
    sites/all/libraries/nusoap
    sites/all/libraries/nusoap/lib
    sites/all/libraries/nusoap/lib/nusoap.php

- Enable the module on the Modules admin page:
      Administer > Site building > Modules

-------------------
Configuration
-------------------

- The setting page is within the Ubercart Store Administration > Payment, under the payment gateway tab, and the section labelled eWay. Here you enter your details given by eWay, also switch between testing and production. 

- Certain licensing may require you to display the eWay logo, or you may wish to display the logo to give confidence to your clients. See below for the options available.

------------------
Displaying the eWay logo
------------------

There are two options for this, the first is to set the 'display logo' option in the eWay settings to 'yes'.

The second option is through overriding the theme_uc_payment_method_credit_form function. Copy and paste the following code into your template.php file:

function garland_uc_payment_method_credit_form($form) {
  // Comment out this function to just straight display the form.
  $form['cc_number']['#title'] = '';
  $form['cc_start_month']['#title'] = '';
  $form['cc_start_year']['#title'] = '';
  $form['cc_exp_month']['#title'] = '';
  $form['cc_exp_year']['#title'] = '';
  $form['cc_issue']['#title'] = '';

  if (arg(1) == 'checkout') {
    $path = base_path() . drupal_get_path('module', 'uc_credit');
    $output = '<table class="inline-pane-table" cellpadding="2">';
    if (strlen($form['cc_policy']) > 0) {
      $output .= '<tr><td colspan="2">'. variable_get('uc_credit_policy', '')
                .'</td></tr>';
    }
    if (variable_get('uc_credit_type_enabled', FALSE)) {
      $form['cc_type']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('Card Type:') .'</td><td>'
               . drupal_render($form['cc_type']) .'</td></tr>';
    }
    if (variable_get('uc_credit_owner_enabled', FALSE)) {
      $form['cc_owner']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('Card Owner:') .'</td><td>'
               . drupal_render($form['cc_owner']) .'</td></tr>';
    }
    $output .= '<tr><td class="field-label">'. t('Card Number:') .'</td><td>'
             . drupal_render($form['cc_number']) .'</td></tr>';
    if (variable_get('uc_credit_start_enabled', FALSE)) {
      $output .= '<tr><td class="field-label">'. t('Start Date:') .'</td><td>'
               . drupal_render($form['cc_start_month']) .' '. drupal_render($form['cc_start_year'])
                .' '. t('(if present)') .'</td></tr>';
    }
    $output .= '<tr><td class="field-label">'. t('Expiration Date:') .'</td><td>'
             . drupal_render($form['cc_exp_month']) .' '
             . drupal_render($form['cc_exp_year']) .'</td></tr>';
    if (variable_get('uc_credit_issue_enabled', FALSE)) {
      $output .= '<tr><td class="field-label">'. t('Issue Number:') .'</td><td>'
               . drupal_render($form['cc_issue']) .' '. t('(if present)').'</td></tr>';
    }
    if (variable_get('uc_credit_cvv_enabled', TRUE)) {
      $form['cc_cvv']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('CVV:') .'</td><td>'. drupal_render($form['cc_cvv'])
                .' <img src="'. $path .'/images/info.png" onclick="cvv_info_popup();" style="cursor: pointer; position: relative; top: 3px;"> <a style="cursor: pointer; font-weight: normal;" onclick="cvv_info_popup();">'
                . t("What's the CVV?") .'</a></td></tr>';
    }
    if (variable_get('uc_credit_bank_enabled', FALSE)) {
      $form['cc_bank']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('Issuing Bank:') .'</td><td>'
               . drupal_render($form['cc_bank']) .'</td></tr>';
    }
    $output .= '</table>';
	
	$imagepath = base_path() . drupal_get_path('module', 'uc_eway');
	$output .= '<img src="'. $imagepath .'/ewayLarge.gif" style="float: right; margin-top: -90px">';
  }
  else {
    $output = '<table class="order-edit-table"><tbody style="border-top: 0px;">';
    if (variable_get('uc_credit_type_enabled', FALSE)) {
      $form['cc_type']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('Card Type:') .'</td><td>'
               . drupal_render($form['cc_type']) .'</td></tr>';
    }
    if (variable_get('uc_credit_owner_enabled', FALSE)) {
      $form['cc_owner']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('Card Owner:') .'</td><td>'
               . drupal_render($form['cc_owner']) .'</td></tr>';
    }
    $output .= '<tr><td class="oet-label">'. t('Card Number:') .'</td><td>'
             . drupal_render($form['cc_number']) .'</td></tr>';
    $output .= '<tr><td class="oet-label">'. t('Expiration Date:') .'</td><td>'
             . drupal_render($form['cc_exp_month']) .' '
             . drupal_render($form['cc_exp_year']) .'</td></tr>';
    if (variable_get('uc_credit_cvv_enabled', TRUE)) {
      $form['cc_cvv']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('CVV:') .'</td><td>'
               . drupal_render($form['cc_cvv']) .'</td></tr>';
    }
    if (variable_get('uc_credit_bank_enabled', FALSE)) {
      $form['cc_bank']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('Issuing Bank:') .'</td><td>'
               . drupal_render($form['cc_bank']) .'</td></tr>';
    }
    $output .= '</td></tr></tbody></table>';
  }

  return $output;
}

If you have already themed this function, just make sure you copy and paste these following lines at the very end of the first 'if' statement after "$output .= '</table>';":

$imagepath = base_path() . drupal_get_path('module', 'uc_eway');
	$output .= '<img src="'. $imagepath .'/ewayLarge.gif" style="float: right; margin-top: -90px">';

