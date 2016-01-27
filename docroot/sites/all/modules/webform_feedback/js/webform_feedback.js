(function ($) {
  Drupal.behaviors.webform_feedback = {
    attach: function (context, settings) {
      if($('div').is('#block-webform-client-block-' + Drupal.settings.webform_feedback.key)) {
        $('#block-webform-client-block-' + Drupal.settings.webform_feedback.key).addClass('webform-feedback-block');
        $("body").append("<div id='webform_feedback_popup'><div id='fby-screen' class='fby-screen'><div id='fby-mask' class='fby-mask'></div><div id='fby-tab-1553' class='fby-tab fby-tab-l'><a class='webform-feedback-control-button' href='#block-webform-client-block-'" + Drupal.settings.webform_feedback.key + "' >'" + Drupal.settings.webform_feedback.linkText + "</a></div></div></div>");
        $('#webform_feedback_popup .fby-screen').addClass('feedback-button-' + Drupal.settings.webform_feedback.webformPosition);
        $('.webform-feedback-block').append('<div class="logo-space"></div><a href="#" id="formclose" class="webform-feedback-control-button">Close</a>');
        $('#page-wrapper').prepend('<div id="mask-bg"></div>');

        $('a.webform-feedback-control-button').click(function() {
          $('.webform-feedback-block').slideToggle();
          $('#mask-bg').toggle();
          $(".webform-feedback-block input.form-text, .webform-feedback-block textarea").first().focus();
          return false;
        });

        if($(window).width() < 820) {
          $('#formclose').addClass('small');
        }
        $('.webform-feedback-block  .form-actions').append('<div id="ajax-loader"></div>');

        $('.webform-feedback-block .form-submit').click(function() {
          $(this).hide();
          $('.webform-feedback-block #ajax-loader').show();
        });
      }
    }
  };
}(jQuery));
