/**
 * @file
 * JavaScript include file for AJAX Update.
 *
 */
(function($) {
  /**
   * Handler for the form serialization.
   *
   * Runs before the beforeSend() handler (see below), and unlike that one, runs
   * before field data is collected.
   */
  Drupal.ajax.prototype.beforeSerialize = function(element, options) {

    // Allow detaching behaviors to update field values before collecting them.
    // This is only needed when field values are added to the POST data, so only
    // when there is a form such that this.form.ajaxSubmit() is used instead of
    // $.ajax(). When there is no form and $.ajax() is used, beforeSerialize()
    // isn't called, but don't rely on that: explicitly check this.form.
    if (this.form) {
      var settings = this.settings || Drupal.settings;
      Drupal.detachBehaviors(this.form, settings, 'serialize');
      var element_id = $(this.form).attr('id');
    } else {
      var element_id = $(this).attr('id');
    }

    // Prevent duplicate HTML ids in the returned markup.
    // @see drupal_html_id()
    options.data['ajax_html_ids[]'] = [];
    $('[id]:not(#' + element_id + ')').each(function() {


      // Ensure that the element has a valid id.
      if (this.id.length > 0) {

          // If the form does not explicity exclude the sending of
          // HTML IDs, add them to the options.data.

          if (!$('form#' + element_id + '.ajax_html_id_exclude #' + this.id).length) {
            options.data['ajax_html_ids[]'].push(this.id);
          }
      }

    });

    // Allow Drupal to return new JavaScript and CSS files to load without
    // returning the ones already loaded.
    // @see ajax_base_page_theme()
    // @see drupal_get_css()
    // @see drupal_get_js()
    options.data['ajax_page_state[theme]'] = Drupal.settings.ajaxPageState.theme;
    options.data['ajax_page_state[theme_token]'] = Drupal.settings.ajaxPageState.theme_token;
    for (var key in Drupal.settings.ajaxPageState.css) {
      options.data['ajax_page_state[css][' + key + ']'] = 1;
    }
    for (var key in Drupal.settings.ajaxPageState.js) {
      options.data['ajax_page_state[js][' + key + ']'] = 1;
    }
  };
})(jQuery);
