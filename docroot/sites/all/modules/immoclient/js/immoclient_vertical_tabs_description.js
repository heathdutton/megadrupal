(function ($) {

/**
 * Update the summary for the module's vertical tab.
 */
Drupal.behaviors.beautifyFieldsetSummaries = {
  attach: function (context) {
    //js throws an error when the selectors aren't there
    if($('.vertical-tabs-pane .fieldset-description').length > 0){
      $('.vertical-tabs-pane .fieldset-wrapper .fieldset-description').css("display","none");
      // Use the fieldset class to identify the vertical tab element
      $('.vertical-tabs-pane', context).drupalSetSummary(function (context) {
        // take the value from the description field, hide this field and send 
        // the value to span summary
        return Drupal.checkPlain($('.fieldset-description', context).text());
        });
      }
    }
  };

})(jQuery);


