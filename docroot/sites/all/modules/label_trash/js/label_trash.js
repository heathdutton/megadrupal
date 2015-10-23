/**
 * @file label_trash.js
 * 
 * Takes parameters set on the configuration page and invokes floatlabel.js
 */
(function ($) {
  Drupal.behaviors.label_trash_attach = {

    attach: function(context, settings) {

      $(settings.label_trash, context).each(function() {

        // settings is an array of params indexed by selectors as entered on
        // the Label Trash config page. One selector string per decoration.
        $.each(this, function(selector, params) {
          
          $(selector).each(function(index, input) {
            var inputfield = $(input);
            var placeholder = inputfield.attr('placeholder');
            var label = null;
            // If the input element does not already have a placeholder,
            // create one based on its neighbouring label element.
            if (!placeholder) {
              label = inputfield.prev('label'); // or siblings() ?
              if (label.length === 0) {
                // The targeted element may live inside a <div>.
                label = inputfield.parent().siblings('label');
              }
              placeholder = label.html();
            }
            if (!placeholder) {
              // If after all this we still have no placeholder, end like this:
              inputfield.attr('placeholder', Drupal.t('Type here'));
            }
            else {
              // Deal with placeholders containing HTML, like:
              // 'Title <span class="form-required" title="Field is required.">*</span>'
              var split = placeholder.indexOf('<');
              var placeholder_escaped = split < 0 ? placeholder : placeholder.substring(0, split) + $(placeholder.substring(split)).text();
              inputfield.attr('placeholder', placeholder_escaped);

              // Check whether the float flag is set.
              if (params['label-style']) {
                if (label) {
                  // Remove the label, floatlabel() will create a new one.
                  label.remove();
                }
                // Accessibility: as we're removing the label, let's make sure
                // to have at least an input field title.
                // See: http://www.html5accessibility.com/tests/placeholder-labelling.html
                if (!inputfield.attr('title')) {
                  inputfield.attr('title', placeholder_escaped);
                }

                inputfield.attr('data-label', placeholder);
                inputfield.floatlabel({
                  labelStartTop: params['floatlabel-offset-top-start'],
                  labelEndTop: params['floatlabel-offset-top-end'],
                  labelClass: params['label-css']['data-class'],
                  slideInput: params['slide-input'],
                  transitionDuration: params['transition-duration'],
                  transitionEasing: params['transition-easing'],
                });
              }
              else if (label) {
                // Move rather than label.hide(), for accessibility.
                label.css('text-indent', -99999);
              }
            }           
          });
        });
      });
    }
  }
}) (jQuery);
