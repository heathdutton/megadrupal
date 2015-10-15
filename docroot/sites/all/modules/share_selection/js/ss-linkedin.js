(function($) {
  Drupal.shareSelection = Drupal.shareSelection || {};

  Drupal.shareSelection.onSuccessLinkedin = function(data) {
  }

  Drupal.shareSelection.onErrorLinkedin = function(error) {
    console.log(error);
  }

  Drupal.shareSelection.confirmLinkedin = function() {
    $('#ss-dialog-wrapper').attr('title' , Drupal.t('Share on Linkedin'));
    $('#ss-dialog-wrapper').text(Drupal.shareSelection.selectedText);
    $('#ss-dialog-wrapper').dialog({
      resizable: false,
      height: 250,
      width: 450,
      modal: true,
      draggable: false,
      buttons: [
        {
          text: Drupal.t('Accept'),
          click: function() {
            Drupal.shareSelection.shareLinkedin();
          }
        },
        {
          text: Drupal.t('Cancel'),
          click: function() {
            $(this).dialog('close');
          }
        }
      ],
      create: function(event, ui) {
        $(event.target).parent().css('position', 'fixed');
      },
    });
  }

  Drupal.shareSelection.shareLinkedin = function() {
    // Build the JSON payload containing the content to be shared.
    var payload = {
      "comment": Drupal.shareSelection.selectedText.toString(),
      "visibility": {
        "code": "connections-only"
      }
    };

    IN.API.Raw("/people/~/shares?format=json")
      .method("POST")
      .body(JSON.stringify(payload))
      .result(Drupal.shareSelection.onSuccessLinkedin)
      .error(Drupal.shareSelection.onErrorLinkedin);
  }

  Drupal.shareSelection.onLoadLinkedin = function() {
    $('#share-selection-linkedin').mousedown(function(e) {
      if (Drupal.shareSelection.selectedText) {
        IN.User.authorize(Drupal.shareSelection.confirmLinkedin);
      };
    });
  }

  Drupal.behaviors.ssLinkedin = {
    attach : function(context, settings) {
      $('body').once('ss-linkedin', function() {
        $.getScript("http://platform.linkedin.com/in.js?async=true", function success() {
          IN.init({
            onLoad: "Drupal.shareSelection.onLoadLinkedin",
            api_key: Drupal.settings.shareSelection.linkedinApikey
          });
        });
      });
    }
  };
})(jQuery);
