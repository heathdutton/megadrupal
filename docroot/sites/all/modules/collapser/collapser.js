(function($) {
  /**
   * @file
   * Javascript for collapser module.
   */

  /**
   * Bind a handler function to be processed as the very first one.
   *
   * Halfway hack to change event handlers order.
   * Based on internal jQuery functions, so maybe incompatible
   * with future jQuery versions.
   * Use just as jQuery.bind():
   *
   * @param myObject
   *  The object to have the new event listener.
   * @param name
   *   The event name.
   * @param fn
   *   The handler function.
   *
   * @see http://stackoverflow.com/questions/2360655
   */
  $.fn.bindFirst = function(name, fn) {
    this.bind(name, fn);
    var handlers = this.data('events')[name.split('.')[0]];
    var handler = handlers.pop();
    handlers.splice(0, 0, handler);
  };

  /**
   * Drupal API initialisation.
   */
  Drupal.behaviors.collapser = {
    attach: function(context) {
      $("form fieldset.collapsible.collapser-processible a.fieldset-title:not(.collapser-processed)", context).each(function(){
        $(this).bindFirst('click', function(event) {
          // Collect information about the element status that has been changed.
          var fieldset = $(this).parents("fieldset").first();
          var cuid = Drupal.collapser.translateUid(fieldset.attr('class'));
          if (cuid) {
            var status = fieldset.is('.collapsed');
            Drupal.collapser.remember(cuid, status);
          }
        });
      }).addClass('collapser-processed');
      if(!Drupal.settings.collapser.remoteSave) {
        // Only process cookies for anonymous users or users
        // without server side saving.
        $("form fieldset.collapsible.collapser-processible:not(collapser-processed)").each(function(){
          var cuid = Drupal.collapser.translateUid($(this).attr('class'));
          if (cuid) {
            var status = Drupal.collapser.cookie('Drupal.collapser.c' + cuid);
            // Only work if a cookie has ever been set.
            if (status !== null && status != $(this).is('.collapsed')) {
              $(this).find("a.fieldset-title").first().click();
            }
          }
        }).addClass("collapser-processed");
      }
    }
  }

  Drupal.collapser = {

    /**
     * Translate a collapser-uid-classname into a unique form/element ID.
     */
    translateUid: function(domClass) {
      uid = domClass.match(/\bcollapser-uid-([0-9]+)\b/);
      return typeof uid == 'object' ? uid[1] : false;
    },

    /**
     * Remember a fieldset status.
     *
     * @param cuid
     *   Unique collapser ID for the fieldset.
     * @param status
     *   Whether the fieldset was collapsed at click time.
     */
    remember: function(cuid, status) {
      // We have received the status before it was toggled.
      status = status ? 0 : 1;

      if (Drupal.settings.collapser.remoteSave) {
        // 1 Save server-side for user based use.

        $.ajax({
          url: Drupal.settings.collapser.ajaxPath,
          error: function (a, b, c) {alert(b);},
          success: function (a, b, c) {},
          type: 'POST',
          data: {
            cuid: cuid,
            status: status
          }
        });
        // Ajax post to remote URL:
        // *Post raw data as above
        // *handle exceptions silently
      }
      else {
        // 2) Set a cookie for anonymous/client based use.
        Drupal.collapser.cookie('Drupal.collapser.c' + cuid, status, Drupal.settings.basePath, 365);
      }
    },

    /**
     * Cookie wrapper.
     *
     * Wrapper to jQuery.cookie to keep all other code maintainable
     * ($.cookie is not available in D6).
     *
     * @param name
     * @param value
     * @param path
     * @param expires
     *   The expiration value in seconds from now.
     */
    cookie: function(name, value, path, expires) {
      path = path === undefined ? '/' : path;
      expires = expires === undefined ? 0 : path;
      if (value !== undefined) {
        return $.cookie(
          name,
          value,
          {
            path: path,
            expires: expires
          }
        );
      }
      else {
        return $.cookie(name);
      }
    }
  }

})(jQuery);
