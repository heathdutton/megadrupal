/**
 * Script to deal with the various client-side aspects of this module.
 * 
 * This includes calling the custom generated Javascript
 * code by the contact at once
 * API, correctly putting the chat widget a
 * layer underneath the Drupal admin overlay
 * and opening a new chat widget from the block.
 */

(function($) {
  // Best practise use of Drupal behaviours to trigger the chat window from the chat
  // widget block.
  Drupal.behaviors.contactatonce_chat = {
    attach: function (context, settings) { // these are the elements loaded in first
      $('.contactatonce-livechat').click(function(e){
        
        console.log(settings);
        var contactAtOnce = settings.contactatonce;
        var merchantId = contactAtOnce.merchantId;
        var providerId = contactAtOnce.providerId;

        window.open('http://CL5WS2.contactatonce.com/CaoClientContainer.aspx?MerchantId=' + merchantId + '&Providerid=' + providerId + '&PlacementId=4','','resizable=yes,toolbar=no,menubar=no,location=no,scrollbars=no,status=no,height=400,width=600');
        e.preventDefault;
      });
    }
  }

  // Load in chat window on page load
  function wrappedPopin()
  {
    try {
      popIn();
    }
    catch (Exception) {
    }
  }
  
  // Set overlay to a higher index than the chat widget.
  function setOverlayZIndex() {
    document.addEventListener('DOMNodeInserted', function(e) {
      if ($("#overlay-container").length)
        $('#overlay-container').css('z-index', 15000);
    });
  }
  
  setOverlayZIndex();
  wrappedPopin();
  
})(jQuery);
