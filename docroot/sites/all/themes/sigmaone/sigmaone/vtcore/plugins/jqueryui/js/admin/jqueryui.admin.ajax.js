(function( $ ) {
  /**
   * Hijack ajax for sane admin theme
   */
  Drupal.behaviors.jqueryUIAjax = {
    attach: function(context, settings) {
      if ($('#edit-jqueryui').length != 0) {
        Drupal.ajax['edit-jquery-theme-selected'].beforeSerialize = function (element, options) {
          Drupal.settings.ajaxPageState.theme = Drupal.settings.adminTheme.theme;
          Drupal.settings.ajaxPageState.theme_token = Drupal.settings.adminTheme.theme_token;

          // Allow detaching behaviors to update field values before collecting them.
          // This is only needed when field values are added to the POST data, so only
          // when there is a form such that this.form.ajaxSubmit() is used instead of
          // $.ajax(). When there is no form and $.ajax() is used, beforeSerialize()
          // isn't called, but don't rely on that: explicitly check this.form.
          if (this.form) {
            var settings = this.settings || Drupal.settings;
            Drupal.detachBehaviors(this.form, settings, 'serialize');
          }
    
          // Prevent duplicate HTML ids in the returned markup.
          // @see drupal_html_id()
          options.data['ajax_html_ids[]'] = [];
          $('[id]').each(function () {
            options.data['ajax_html_ids[]'].push(this.id);
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
      }
    }
  };

})( jQuery );