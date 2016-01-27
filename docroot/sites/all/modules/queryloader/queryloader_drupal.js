(function ($) {

  Drupal.behaviors.queryloader = {
    attach: function (context) {

      // Define a variable that will hold our context
      var $loadedObject;

      /**
       * Because by default drupal passes HTML as a context (which breaks queryloader),
       * store the body if it exists, otherwise just pass through the context as a jQuery object.
       */
      if (context.body) {
        $loadedObject = $(context.body);
      }
      else if (Drupal.settings.queryloader.onAjax) {
        $loadedObject = $(context);
      }

      // Run the query loader with the selected Drupal admin settings
      if ($loadedObject) {
        Drupal.behaviors.queryloader.run($loadedObject)
      }

    },
    /**
     * If you'd like to call query loader from your own JavaScript code then use
     * Drupal.behaviors.queryloader.run($JqueryObject)
     */
    run: function ($context) {
      $context.queryLoader2(Drupal.settings.queryloader);
    }
  };


})(jQuery);
