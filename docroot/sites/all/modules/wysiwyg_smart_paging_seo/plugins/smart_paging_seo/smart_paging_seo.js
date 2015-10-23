(function ($) {
    Drupal.wysiwyg.plugins['smart_paging_seo'] = {

        // Local storage for tags data.
        storage: {},
            
        /**
         * Return whether the passed node belongs to this plugin.
         */
        isNode: function(node) {
            return $(node).is('.wysiwyg-sp-seo-img');
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
            // Display the Add form.
            if (data.content == "") {
                settings.data_id = this._getId();
                settings.placeholder = this._getImgPlaceholder(settings);
                settings.mode = "Add";
            }
            // Display the Edit form.
            else {
                var regexp = new RegExp('<img.*?alt="(\\w+)".*?>', 'gi');
                settings.data_id = data.content.replace(regexp, '$1');
                settings.mode = "Edit";
            }
            // Show the input form.
            this.show_popup(settings, instanceId);
        },

        /**
         * Replace Markup with image.
         */
        attach: function(content, settings, instanceId) {
            // <!--pagingmetatag  MUST be right after to <!--pagebreak-->
            var dom = document.createElement('div');
            dom.innerHTML = content;
            var comments = this.getComments(dom, new Array);
            if (comments.length > 0) {
            	for (var i = 0; i < comments.length; i++) {
            	    var comment = comments[i];
            	    if (comment.nodeValue == 'pagebreak') {
            	        var d = {};
                        // build a string to replace with the image.
                        var replace = '<!--pagebreak-->';
            	        // Are there siblings?
            	        if (comment.nextSibling) {
            	            // Is there a smartpaging sibling?
            	            var sibling = comment.nextSibling;
            	            while (sibling) {
            	                if (sibling && sibling.nodeType === 8) {
            	                    // A comment.
            	                    if (sibling.nodeValue.substr(0, 11) == 'smartpaging') {
            	                        // Gather data from the comments.
                                        if (sibling.nodeValue.substr(0, 15) == 'smartpagingmeta') {
                                          // A smartpagingmeta comment stores data as a JSON string
                                          var json_str = sibling.nodeValue.substr(16, sibling.nodeValue.length);
                                          try {
                                              d = JSON.parse(json_str);
                                              var data_id = d.data_id;
                                          } catch (e) {}
                                          replace = replace + '<!--' + sibling.nodeValue + '-->';
                                        }
                                        else if (sibling.nodeValue.substr(0, 14) == 'smartpagingurl') {
                                          // A smartpagingurl simply stores a string.
                                          // Store the url.
                                          var url = sibling.nodeValue.substr(16, sibling.nodeValue.length - 17);
                                          d.url = url;
                                          replace = replace + '<!--' + sibling.nodeValue + '-->';
                                          break;
                                        }
            	                    }
            	                }
            	                sibling = sibling.nextSibling;
            	            }
                        }
                            // Backwards compatibility for <!--pagebreak--> without <!--smartpagingmeta, <!--smartpagingurl tags.
                            if (typeof d.data_id == 'undefined') {
                              data_id = this._getId();
                              d.data_id = data_id;
                            }
            	            this.storage[data_id] = d;
            	            settings.data_id = data_id;
            	            content = content.replace(replace, this._getImgPlaceholder(settings));
            	       // }
            	    }
            	}
            }
            return content;
        },

        /**
         * Replace images with Markup.
         */
        detach: function(content, settings, instanceId) {
            // Find all Images and extract the ids.
            var regexp = new RegExp('<img[^>]*?class="wysiwyg-sp-seo-img"[^>]*?alt="(\\d+)"[^>]*>', 'gi');
            var data_id_array = content.match(regexp);
            if (data_id_array != null) {
                for (var i = 0; i < data_id_array.length; i++) {
                    var data_id = data_id_array[i].replace(regexp, '$1', 'gi');
                    settings.data_id = data_id;

                    // Generate markup using data from storage.
                    var data = this.storage[settings.data_id];
                    if (typeof data.url != 'undefined') {
                        var url = data.url;
                    }
                    else {
                        var url = "";
                    }
                    var metatag_markup = JSON.stringify(data);

                    // Replace image with markup.
                    content = content.replace(data_id_array[i],
                        settings.pagebreak_markup['prefix'] + settings.pagebreak_markup['suffix'] +
                        settings.metatag_markup['prefix'] + metatag_markup + settings.metatag_markup['suffix'] +
                        settings.url_markup['prefix'] + url + settings.url_markup['suffix']);
                }
            }
            return content;
        },

        /**
         * Shows the Form.
         */
        show_popup: function(settings, instanceId) {
            // Remove any previous popup.
            $('.wysiwyg-sp-seo-popup').remove();
            // Normalize the field names.
            form = settings.form_markup.replace(new RegExp('metatags\\[([\\w|:|-]+)\\]\\[value\\]', 'gi'), 'meta_name_$1_content');
            // Fix robots tags manually. @TODO: use RegExp here too.
            //form = form.replace("robots[", "robots:");
            //form = form.replace("]", "");
            // Print the form on the page.
            jQuery("body").append(form);

            if (settings.mode == "Edit") {
                // Fill in the form values with json data.
                var values = Drupal.wysiwyg.plugins['smart_paging_seo'].storage[settings.data_id];
                if (typeof values == "object") {
                  jQuery.each(values, function(name, value) {
                    // Escape special characters to ensure jQuery Update compatibility.
                    name = name.replace(/(:|\.|\[|\]|,)/g, "\\$1");
                    jQuery('.wysiwyg-sp-seo-popup [name=' + name + ']').val(value);
                  });
                }
            }

            // Display popup centered on screen.
            jQuery(".wysiwyg-sp-seo-popup").center().show(function() {
                jQuery(".wysiwyg-sp-seo-popup form fieldset legend").click(function() {
                    $(this).parent().toggleClass('collapsed');
                });

                // Listeners for buttons.
                jQuery(".wysiwyg-sp-seo-popup .insert").click(function() {
                    // Get key/values from the form.
                    var values = Drupal.wysiwyg.plugins['smart_paging_seo']._getFormValues(settings);

                    // Store the values.
                    Drupal.wysiwyg.plugins['smart_paging_seo'].storage[settings.data_id] = values;

                    // Insert the placeholder image.
                    if (settings.mode == "Add") {
                        Drupal.wysiwyg.instances[instanceId].insert(settings.placeholder);
                    }

                    // Close popup.
                    jQuery(".wysiwyg-sp-seo-popup").remove();
                });

                jQuery(".wysiwyg-sp-seo-popup .cancel").click(function() {
                    jQuery(".wysiwyg-sp-seo-popup").remove();
                });

                // Catch keyboard events.
                jQuery(document).keydown(function(e) {
                    // Esc key pressed.
                    if (e.keyCode == 27) {
                        jQuery(".wysiwyg-sp-seo-popup").remove();
                    }
                });

                jQuery(".wysiwyg-sp-seo-popup *:input[type!=hidden]:first").focus();
            });
        },

        /**
         * Helper function to return a HTML placeholder.
         */
        _getImgPlaceholder: function (settings) {
            return settings.icon_markup.replace('alt=""', 'alt="' + settings.data_id + '"');
        },

        /**
         * Helper function to return all values from the form.
         */
        _getFormValues: function(settings) {
            var values = {};
            values["data_id"] = settings.data_id;

            // Get values from the form. Only get values from input type: text, textarea
            jQuery('.wysiwyg-sp-seo-popup *').filter('input[type=text],textarea').each(function(key, value) {

                // Ignore fields that were not normalized when building the form, they wil have [] symbols.
                if (this.name.indexOf("[") == -1 && this.name.indexOf("]") == -1) {
                    // Only override tags that contain values.
                    if (this.value != "" && typeof this.value != 'undefined' && this.value != 'undefined') {
                        values[this.name] = this.value;
                    }
                }
            });
            return values;
        },

        /**
         * Helper to get new Id.
         */
        _getId: function() {
          var id = new Date().getTime() + Math.floor((Math.random()*1000)+1);
          return id;
        },
    };

})(jQuery);

/**
 * Center the element on the screen.
 */
if (!jQuery.fn.center) {
    jQuery.fn.center = function () {
        this.css("position","absolute");
        this.css("top", Math.max(0, ((jQuery(window).height() -
            jQuery(this).outerHeight()) / 2) +
            jQuery(window).scrollTop()) + "px");
        this.css("left", Math.max(0, ((jQuery(window).width() -
            jQuery(this).outerWidth()) / 2) +
            jQuery(window).scrollLeft()) + "px");
        return this;
    }
}
