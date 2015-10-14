/**
 * @file
 * Simple Calculator js.
 */

(function ($) {
    Drupal.behaviors.simple_calculator = {
        attach: function (context, settings) {
            // Only numbers in simple calculator.
            $('#calc-simple').keyup(function (e) {
                if (e.which != 8 && e.which != 0 && (e.which < 48 || e.which > 57)) {
                    // Display error message.
                    var value = $(this).val();
                    value = value.replace(/\D/g, "");
                    $("#errmsg").html("Digits Only").show().fadeOut("slow");
                    $('#calc-simple').val(value);
                    return false;
                }
            });
            // Emi Calculator.
            $('#calculte-emi').unbind().click(function () {
                console.log('ajay');
                var principal = $('#principal-amt').val();
                var interest = $('#int-rate').val();
                var period = $('#principal-period').val();
                // To calculate rate percentage.
                var rate = interest / (12 * 100);
                // To calculate compound interest.
                var prate = (principal * rate * Math.pow((1 + rate), period)) / (Math.pow((1 + rate), period) - 1);
                // To parse emi amount.
                var emi = Math.ceil(prate * 100) / 100;
                var round_emi = Math.round(emi.toFixed(2));
                // To calculate total amount.
                var tot = Math.round(emi * period * 100) / 100;
                var round_total = Math.round(tot);
                var int_amt = round_total - principal;
                $('#output-emi').val(round_emi);
                $('#output-interest').val(int_amt);
                $('#output-total').val(round_total);
                var beginning_balance = principal;
                var i = 0;
                if (principal.length > 0) {
                    var content = "<table class='loan-table'><thead><tr><th># Month</th>" +
                            "<th>Starting Balance</th><th>Rate of Interest</th><th>Amount to be paid</th>" +
                            "<th>Closing Balance</th></tr></thead><tbody></tbody>";
                    for (i = 1; i <= period; i++) {
                        var interest = Math.round(rate * beginning_balance);
                        var start_balance = Math.round(beginning_balance);
                        var remaining_balance = Math.abs(Math.round(beginning_balance - (round_emi - interest)));
                        content += '<tr><td>' + i + '</td><td>' + start_balance + '</td><td>' + interest + '</td><td>' + round_emi + '</td><td>' + remaining_balance + '</td></tr>';
                        beginning_balance -= round_emi - interest;
                    }
                    content += "</table>";
                    $('#table-box').append(content);
                }
            });
            // Print Emi.
            $('#print-emi').unbind().click(function () {
                var printWindow = window.open('', '', 'height=400,width=800');
                printWindow.document.write($("#table-box").html());
                printWindow.document.close();
                printWindow.print();
            });
            // Home loan Calculator.
            $('#homeloan-calc').click(function () {
                var amount = $('#loan-amount').val();
                var tenature = $('#loan-length').val();
                var interest = $('#loan-tenature').val() / 1200;
                if (amount == null || amount.length == 0 ||
                        tenature == null || tenature.length == 0 ||
                        interest.length == 0 || interest == null) {
                    alert('Fill all the values');
                }
                else {
                    var total = amount * interest / (1 - (Math.pow(1 / (1 + interest), tenature)));
                    $('#loan-payment').val(total);
                }
            });
        }
    };
}(jQuery));
