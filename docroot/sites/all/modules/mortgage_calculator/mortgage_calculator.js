/**
 * @file Javascript behaviors for the Mortgage Calculator module.
 */

(function ($) {
  // Make sure our objects are defined.
  Drupal.MCalculator = Drupal.MCalculator || {};

  /**
   * Calculation after a click
   */
  Drupal.MCalculator.cmdCalc_Click = function(form) {
    if (form.loan_amount_2.value == 0 || form.loan_amount_2.value.length == 0) {
      alert (Drupal.t("The Price field can't be 0!"));
      form.loan_amount_2.focus();
      form.result_2.value = '';
    }
    else if (form.mortgage_rate_2.value == 0 || form.mortgage_rate_2.value.length == 0) {
      alert (Drupal.t("The Interest Rate field can't be 0!"));
      form.mortgage_rate_2.focus();
      form.result_2.value = '';
    }
    else if (form.years_to_pay_2.value == 0 || form.years_to_pay_2.value.length == 0) {
      alert (Drupal.t("The years_to_pay_2 field can't be 0!"));
      form.years_to_pay_2.focus();
      form.result_2.value = '';
    }
    else {
      if (null == form.payment_frequency_2 || form.payment_frequency_2.value == 0 || form.payment_frequency_2.value.length == 0) {
        payment_frequency = 'monthly';
      }
      else {
        payment_frequency = form.payment_frequency_2.value;
      }
      results = Drupal.MCalculator.mortgageCalculation(form.loan_amount_2.value, form.mortgage_rate_2.value, form.years_to_pay_2.value, 0, payment_frequency);
      form.result_2.value = results['calculation'];
    }
  }

  /**
   * Mortgage calculation
   */
  Drupal.MCalculator.mortgageCalculation = function(loanAmount, mortgageRate, yearsToPay, downPayment, paymentFrequency) {
    princ = loanAmount - downPayment;

    if(paymentFrequency == 'monthly') {
      intRate = (mortgageRate / 100) / 12;
      numberOfPayments = yearsToPay * 12;
    }
    else if(paymentFrequency == 'yearly') {
      intRate = mortgageRate / 100;
      numberOfPayments = yearsToPay;
    }
    else if(paymentFrequency == 'semi-monthly') {
      intRate = (mortgageRate / 100) / 24;
      numberOfPayments = yearsToPay * 24;
    }
    else if(paymentFrequency == 'bi-weekly') {
      intRate = (mortgageRate / 100) / 26;
      numberOfPayments = yearsToPay * 26;
    }
    else if(paymentFrequency == 'weekly') {
      intRate = (mortgageRate / 100) / 52;
      numberOfPayments = yearsToPay * 52;
    }
    else if(paymentFrequency == 'accelerated_bi-weekly') {
      intRate = (mortgageRate / 100) / 24;
      numberOfPayments = yearsToPay * 26;
    }
    else if(paymentFrequency == 'accelerated_weekly') {
      intRate = (mortgageRate / 100) / 48;
      numberOfPayments = yearsToPay * 52;
    }

    results = new Array();
    results['calculation'] = Math.floor((princ*intRate)/(1-Math.pow(1+intRate,(-1*numberOfPayments)))*100) / 100;
    results['princ'] = princ;
    results['numberOfPayments'] = numberOfPayments;

    return results;
  }

  Drupal.behaviors.mortgageCalculator = {
    attach: function (context, settings) {

      $("#edit-calculate-2", context).click(function(event) {
        Drupal.MCalculator.cmdCalc_Click(this.form);
        return false;
      });

      $( "#edit-calculate-2" ).once('mortgageCalculator', function() {
        if(!(this.form.loan_amount_2.value == 0 || this.form.loan_amount_2.value.length == 0) && !(this.form.mortgage_rate_2.value == 0 || this.form.mortgage_rate_2.value.length == 0) && !(this.form.years_to_pay_2.value == 0 || this.form.years_to_pay_2.value.length == 0)) {
          Drupal.MCalculator.cmdCalc_Click(this.form);
        }
      })

    }
  };
})(jQuery);
