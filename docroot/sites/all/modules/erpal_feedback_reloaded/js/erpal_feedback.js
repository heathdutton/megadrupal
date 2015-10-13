(function ($) {

  // Attach src content of Screenshot preview.
  Drupal.behaviors.setScreenshotData = {
    attach: function (context, settings) {
      if(feedbackReloaded.screenshotBase64) {
        $("#screenshot_preview", $("#feedback_form_container"))
          .attr('src','data:image/png;base64,'+feedbackReloaded.screenshotBase64.replace('data:image/png;base64,', '')+'');
      }
    }
  };

}(jQuery));
