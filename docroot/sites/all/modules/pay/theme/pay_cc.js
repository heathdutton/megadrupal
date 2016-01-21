/**
 * @file
 * jQuery functions for the credit card form.
 */

Drupal.behaviors.payCcType = function(context) {
  $('.pay-cc-form:not(.pay-cc-processed)', context).each(function() {
    var $payCcForm = $(this).addClass('pay-cc-processed');

    // Hide the radio buttons themselves.
    $payCcForm.find('.pay-cc-type input').hide();

    // Wrap the labels in links so that they may be given keyboard focus.
    $payCcForm.find('.pay-cc-type .form-radios img').each(function() {
      var image = this;
      $(image).wrap('<a href="#"></a>');
      $(image).parent().click(function() {
        $(image).parents('label:first').find('input').click().trigger('change');
        return false;
      });
    });

    // Highlight the selected radio button when clicked.
    $payCcForm.find('.pay-cc-type input').change(function() {
      $payCcForm.find('.pay-cc-type img').css('opacity', '.35');
      $payCcForm.find('.pay-cc-type input:checked').parents('label:first').find('img').css('opacity','1');
      Drupal.pay.setFieldVisibility($payCcForm, $(this).val());
    }).trigger('change');
    var $initial_cctype = $payCcForm.find('.pay-cc-type input:checked').val();
    if ($initial_cctype != undefined && $initial_cctype != null) {
      Drupal.pay.setFieldVisibility($payCcForm, $payCcForm.find('.pay-cc-type input:checked').val());
    }

    // Automatically set the credit card type based on the number.
    $payCcForm.find('.pay-cc-number input').keyup(function() {
      var type = Drupal.pay.ccTypeFromNumber(this.value);
      $payCcForm.find('.pay-cc-type input[value=' + type + ']').click().trigger('change');
    });
  });
}

/**
 * Make sure the pay object is instantiated.
 */
Drupal.pay = Drupal.pay || {};

/**
 * Utility function to get the credit card type from a number.
 */
Drupal.pay.ccTypeFromNumber = function(number) {
  var type = false;

  // Clean-up the number to be numeric only.
  number = number.replace(/[^0-9]/, '');
  if (number.length < 4) {
    return false;
  }

  var prefix1 = parseInt(number.substr(0, 1));
  var prefix2 = parseInt(number.substr(0, 2));
  var prefix3 = parseInt(number.substr(0, 3));
  var prefix4 = parseInt(number.substr(0, 4));
  var prefix5 = parseInt(number.substr(0, 5));
  var prefix6 = parseInt(number.substr(0, 6));

 /**
  * References: http://www.beachnet.com/~hstiles/cardtype.html
  */
  switch (prefix1) {
    case 1:
      // JCB: prefix 1800, length 15
      if (prefix4 == 1800) type = 'jcb';
      break;

    case 2:
      // JCB: prefix 2131, length 15
      if (prefix4 == 2131) type = 'jcb';

      // enRoute: prefix 2014, 2149 length 15
      else if (prefix4 == 2014) type = 'enroute';
      else if (prefix4 == 2149) type = 'enroute';
      break;

    case 3:
      // Diners Club: prefix 300-305, length 14
      if ($.inArray(prefix3, [300, 301, 302, 303, 304, 305]) != -1) type = 'diners';

      // AmEX: prefix 34, 37, length 15
      else if ($.inArray(prefix2, [34, 37]) != -1) type = 'amex';

      // Diners Club: prefix 36, 38, length 14
      else if ($.inArray(prefix2, [36, 38]) != -1) type = 'diners';

      // JCB: prefix 3, length 16
      else type = 'jcb';
      break;

    case 4:
      // Visa: prefix 4, length 13, 16
      type = 'visa';
      // Switch: prefix 4903, 4905, 4911, 4936
      if ($.inArray(prefix4, [4903, 4905, 4911, 4936]) != -1) type = 'switch';
      break;

    case 5:
      // Mastercard: prefix 51-55, length 16
      if ($.inArray(prefix2, [51, 52, 53, 54, 55]) != -1) type = 'mc';
      // Switch: prefix 564182
      if (prefix6 == 564182) type = 'switch';
      // Maestro: prefix 5018, 5020, 5038
      if ($.inArray(prefix4, [5018, 5020, 5038]) != -1) type = 'maestro';
      break;

    case 6:
      // Discover: prefix 6011, length 16
      if (prefix4 == 6011) type = 'discover';
      // Laser: prefix 6304 or 6706 or 6709 or 6771, length 19
      if (prefix4 == 6304 || prefix4 == 6706 || prefix4 == 6709 || prefix4 == 6771) type = 'laser';
      // Switch: prefix 633110, 6333
      if (prefix6 == 633110 || prefix4 == 6333) type = 'switch';
      // Maestro: prefix 6759, 6761, 6763
      if ($.inArray(prefix4, [6759, 6761, 6763]) != -1) type = 'maestro';
      // Solo: prefix 6334 or 6767
      if (prefix4 == 6334 || prefix4 == 6767) type = 'solo';
      break;
  }

  return type;
}

Drupal.pay.getCVVLength = function(type) {
  var length = 0;
  if (!type || type == 'amex') length = 4;
  else if ($.inArray(type, ['visa', 'mc', 'discover', 'diners', 'enroute', 'jcb']) != -1) length = 3;
  return length;
}

Drupal.pay.issueNumberRequired = function(type) {
  var required = false;
  if ($.inArray(type, ['switch']) != -1) required = true;
  return required;
}

Drupal.pay.setFieldVisibility = function($payCcForm, type) {
  var ccv2_length = Drupal.pay.getCVVLength(type);
  if (!ccv2_length) $payCcForm.find('.pay-cc-ccv2').hide();
  else {
    $payCcForm.find('.pay-cc-ccv2 input').attr('maxlength', ccv2_length);
    $payCcForm.find('.pay-cc-ccv2').show();
  }
  var show_issue_number = Drupal.pay.issueNumberRequired(type);
  if (show_issue_number) $payCcForm.find('.pay-cc-issue-number').show();
  else $payCcForm.find('.pay-cc-issue-number').hide();
}
