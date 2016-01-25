(function ($) {
  Drupal.behaviors.advancedHelpDialog = {    
    attach: function (context) {
      $('#modal-content a').click(function(){
        // Get rid of the modal content in case the destination is an overlay 
        // page.
        Drupal.CTools.Modal.dismiss();
      });
    }
  };
})(jQuery);
