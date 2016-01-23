(function($) {
    $(document).ajaxComplete(function(event, request, settings) {
        $('input[type="submit"]').removeAttr('disabled');
    });
    $(document).ajaxStart(function() {
        $('input[type="submit"]').attr('disabled', 'disabled');
    });
    /**
     * Handler for the "keydown" event.
     */
    Drupal.jsAC.prototype.onkeydown = function(input, e) {
        if (!e) {
            e = window.event;
        }
        switch (e.keyCode) {
            case 40: // down arrow.
//        this.selectDown();
                return false;
            case 38: // up arrow.
                this.selectUp();
                return false;
            default: // All other keys.
                return true;
        }
    };
    /**
     * Hides the autocomplete suggestions.
     */
    Drupal.jsAC.prototype.hidePopup = function(keycode) {
        var hide_popup = false;
        // Select item if the right key or mousebutton was pressed.
        if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
            var selected_value = $(this.selected).data('autocompleteValue');
            if (!in_array(selected_value, Drupal.settings.views_autocomplete_api.views)) {
                this.input.value = $(this.selected).data('autocompleteValue');
                hide_popup = true;
            }

        }
        if (hide_popup) {
            // Hide popup.
            var popup = this.popup;
            if (popup) {
                this.popup = null;
                $(popup).fadeOut('fast', function() {
                    $(popup).remove();
                });
            }
            this.selected = false;
            $(this.ariaLive).empty();
        }

    };
    /**
     * Puts the currently highlighted suggestion into the autocomplete field.
     */
    Drupal.jsAC.prototype.select = function(node) {
        var selected_value = $(node).data('autocompleteValue');
        if (!in_array(selected_value, Drupal.settings.views_autocomplete_api.views) && !empty(selected_value)) {
            this.input.value = $(node).data('autocompleteValue');
        }
    };
    /**
     * Highlights a suggestion.
     */
    Drupal.jsAC.prototype.highlight = function(node) {
        var selected_value = $(node).data('autocompleteValue');
        if (!in_array(selected_value, Drupal.settings.views_autocomplete_api.views)) {
            if (this.selected) {
                $(this.selected).removeClass('selected');
            }
            $(node).addClass('selected');
            if(!empty(selected_value)){
                 this.selected = node;
            }
            $(this.ariaLive).html($(this.selected).html());
        }

    };
}(jQuery));