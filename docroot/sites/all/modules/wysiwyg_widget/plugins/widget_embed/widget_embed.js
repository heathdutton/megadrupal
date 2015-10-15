(function ($) {

  Drupal.wysiwyg.plugins.widget_embed = {

    /**
     * Return whether the passed node belongs to this plugin.
     */
    isNode: function(node) {
        return $(node).is('.wysiwyg-widget-embed-img');
    },

    /**
     * Find the comment nodes.
     */
    getComments: function(node, comments) {
      node = node.firstChild;
      while (node) {
        if (node.nodeType === 8) {
          comments.push(node);
        }
        comments = this.getComments(node, comments);
        node = node.nextSibling;
      }
      return comments;
    },

    /**
     * Execute the button.
     */
    invoke: function(data, settings, instanceId) {
      if (data.content !== '') {
        // Pass the data from the img tag to the popup
        settings.data = jQuery(data.content).data('widget');
      }
      // Show the input form.
      this.show_popup(settings, instanceId);
    },

    /**
     * Replace all <!--widget_embed: ... --> tags with images.
     */
    attach: function(content, settings) {

      // Individually replace each widget so they have a chance to be identified.
      var dom = document.createElement('div');
      dom.innerHTML = content;
      var comments = this.getComments(dom, []);
      if (comments.length > 0) {
        for (var i = 0; i < comments.length; i++) {
          var comment = comments[i];
          if (comment.nodeValue.indexOf('widget_embed:') === 0) {
            var data_value = comment.nodeValue.replace('widget_embed:', '');
            // Escaping special symbols.
            var escaped_comment_nodeValue = this.escapeRegExp(comment.nodeValue);
            if (settings.placeholders !== undefined) {
              var reg;
              // Replace with provided placeholders if possible
              for (var placeholder in settings.placeholders) {
                // Add regex flags if provided
                var flags = settings.placeholders[placeholder].regex.flags ? settings.placeholders[placeholder].regex.flags : '';
                reg = new RegExp(settings.placeholders[placeholder].regex.pattern, flags);
                if ((reg.test(comment.nodeValue))) {
                  reg = new RegExp('<!--' + escaped_comment_nodeValue + '-->', 'gi');
                  content = content.replace(
                    reg,
                    settings.placeholders[placeholder].icon_markup.replace('img ', 'img data-widget="' + data_value + '"')
                  );
                }
              }
            }
            // Use the generic placeholder for any that were not found.
            var regex = new RegExp('<!--' + escaped_comment_nodeValue + '-->', 'gi');
            content = content.replace(
              regex, 
              settings.icon_markup.replace('img ', 'img data-widget="' + data_value + '"')
            );
          }
        }
      }

      //var regexp = new RegExp('<!--widget_embed:(.*?)-->', 'gi');
      //content = content.replace(regexp, settings.icon_markup.replace('data-widget=""', 'data-widget="$1"'));
      return content;

    },

    /**
     * Replace images with <!--widget_embed ... -->.
     */
    detach: function(content) {
      var jo = jQuery('<div>' + content + '</div>');
      jo.find('img').each(function() {
        if ( jQuery( this ).data( 'widget' ) ) {
          jQuery( this ).replaceWith( '<!--widget_embed:' + jQuery( this ).data( 'widget' ) + '-->' );
        }
      });
      return jo.html();
    },

    /**
     * Shows the popup.
     */
    show_popup: function(settings, instanceId) {
      // Check if the form is not yet on the DOM.
      if (jQuery('.widget-embed-popup').length === 0) {
        // Print the form on the page.
        jQuery('body').append(settings.form_markup);
      }
      // Display popup centered on screen.
      jQuery('.widget-embed-popup').center().show('fast', 'linear', function() {
        
        if (settings.data !== undefined) {
          jQuery('#edit-widget-embed-body').val(window.unescape(settings.data));
          jQuery('#edit-widget-embed-insert').val('Update');
          // Clear the data now that it's been used.
          delete settings.data;
        }

        // Listeners for buttons.
        jQuery('#edit-widget-embed-cancel').click(function() {
          jQuery('.widget-embed-popup').remove();
        });

        jQuery('#edit-widget-embed-insert').click(function() {
          var body = jQuery('#edit-widget-embed-body').val();

          if (body.length) {
            var content = '';

            if (settings.placeholders !== undefined) {
              var reg;
              // Replace with provided placeholders if possible
              for (var placeholder in settings.placeholders) {
                // Add regex flags if provided
                var flags = settings.placeholders[placeholder].regex.flags ? settings.placeholders[placeholder].regex.flags : '';
                reg = new RegExp(settings.placeholders[placeholder].regex.pattern, flags);
                if ((reg.test(body))) {
                  content = settings.placeholders[placeholder].icon_markup.replace('img ', 'img data-widget="' + window.escape(body) + '"');
                }
              }
            }

            if (content === '') {
              // Allow unknown widgets if placeholders_only is set to false.
              if (settings.placeholders_only === false) {
                  // Use default place holder.
                  content = settings.icon_markup.replace('img ', 'img data-widget="' + window.escape(body) + '"');
              } else {
                // Not allowed to use this widget, so list what they can use.
                var permitted = 'Only the following widgets can be used:\n';

                for (var placeholder in settings.placeholders) {
                  permitted = permitted + "\n" + placeholder;
                }
                jQuery('#edit-widget-embed-body').val(permitted);
                return;
              }
            }

            // Insert content.
            Drupal.wysiwyg.instances[instanceId].insert(content);
          }

          // Close popup.
          jQuery('.widget-embed-popup').remove();
        });

        // Catch keyboard events.
        jQuery(document).keydown(function(e) {
          // Esc key pressed.
          if (e.keyCode === 27) {
            jQuery('.widget-embed-popup').remove();
          }
        });

        jQuery('#edit-widget-embed-body').focus();
      });
    },

    /**
     * Escapes special characters in regexp.
     */
    escapeRegExp: function(str) {
      return str.replace(/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g, "\\$&");
    },

  };

})(jQuery);

/**
 * Center the element on the screen.
 */
if (!jQuery.fn.center) {
  jQuery.fn.center = function () {
    this.css('position', 'absolute');
  this.css('top', Math.max(0, ((jQuery(window).height() -
    jQuery(this).outerHeight()) / 2) +
    jQuery(window).scrollTop()) + 'px');
  this.css('left', Math.max(0, ((jQuery(window).width() -
    jQuery(this).outerWidth()) / 2) +
    jQuery(window).scrollLeft()) + 'px');
    return this;
  };
}

