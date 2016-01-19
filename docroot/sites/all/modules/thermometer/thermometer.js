// Originally from http://stackoverflow.com/questions/149055/how-can-i-format-numbers-as-money-in-javascript
function formatCurrency(n, c, d, t) {
  "use strict";

  var s, i, j;

  c = isNaN(c = Math.abs(c)) ? 2 : c;
  d = d === undefined ? "." : d;
  t = t === undefined ? "," : t;

  s = n < 0 ? "-" : "";
  i = parseInt(n = Math.abs(+n || 0).toFixed(c), 10) + "";
  j = (j = i.length) > 3 ? j % 3 : 0;

  return s + (j ? i.substr(0, j) + t : "") + i.substr(j).replace(/(\\d{3})(?=\\d)/g, "$1" + t) + (c ? d + Math.abs(n - i).toFixed(c).slice(2) : "");
}

/**
 * Uses the closure to map JQuery variable to $ variable
 */
(function ($) {
  Drupal.behaviors.thermometer = {
    // Originally from:
    // https://www.sitepoint.com/community/t/code-for-a-fundraising-thermometer/21832/4
    attach: function(context) {
      // Set up our vars and cache some jQuery objects.
      var $thermo = $("#thermometer"),
        $progress = $(".progress", $thermo),
        $goal = $(".goal", $thermo),
        percentageAmount;

      // Work out our numbers.
      goalAmount = parseFloat( $goal.text() ),
        progressAmount = parseFloat( $progress.text() ),
        percentageAmount =  Math.min( Math.round(progressAmount / goalAmount * 1000) / 10, 100); //make sure we have 1 decimal point

      // Let's format the numbers and put them back in the DOM.
      $goal.find(".amount").text( "$" + formatCurrency( goalAmount ) );
      $progress.find(".amount").text( "$" + formatCurrency( progressAmount ) );

      // Let's set the progress indicator.
      $progress.find(".amount").hide();
      $progress.animate({
        "height": percentageAmount + "%"
      }, 1200, function(){
        $(this).find(".amount").fadeIn(500);
      });
    }
  };
}(jQuery));
