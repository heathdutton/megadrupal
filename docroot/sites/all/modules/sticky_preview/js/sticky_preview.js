(function ($) {
  /**
   * Class containing functionality for Sticky Preview.
   */
  Drupal.sticky_preview = {};

  /**
   * Process input object.
   */
  Drupal.sticky_preview.stickyPreviewProcessInput = function(name) {
    var selector = '',
        text = '';

    if (name == 'title') {
      // For title 'field' we should use strict match by 'name' attr.
      selector = '[name="' + name + '"]';
    } else {
      // Selector by 'name' attribute like '[name*="some_field_name["]'.
      // We need last bracket in selector to prevent false positive matches.
      // Fore example: 'some_field_name' and 'some_field_name_sub_str'.
      selector = '[name*="' + name + '["]'
    }

    var $field_input = $(selector).filter(':not([type=hidden])');

    // Get result text from each of input types.
    switch ($field_input.attr('type')) {
      case 'textarea':
      case 'text':
        text = Drupal.sticky_preview.stickyPreviewProcessTextWidgets($field_input);
        break;

      case 'select-one':
      case 'select-multiple':
        text = Drupal.sticky_preview.stickyPreviewProcessSelectWidgets($field_input);
        break;

      case 'radio':
        text = Drupal.sticky_preview.stickyPreviewProcessRadioWidgets($field_input);
        break;

      case 'checkbox':
        text = Drupal.sticky_preview.stickyPreviewProcessCheckboxWidgets($field_input);
        break;
    }

    return text;
  };

  /**
   * Get value from text widgets.
   */
  Drupal.sticky_preview.stickyPreviewProcessTextWidgets = function($field_input) {
    var values = [];

    // Rid of selectors with 'weights' for multiple-value text widgets.
    $field_input.filter('textarea, input[type="text"]').each(function() {
      var value = $(this).val();

      if (value) {
        values.push(value);
      }
    });

    return Drupal.sticky_preview.stickyPreviewGetTextResult(values);
  };

  /**
   * Get value from select widgets.
   */
  Drupal.sticky_preview.stickyPreviewProcessSelectWidgets = function($field_input) {
    var values = [];

    $field_input.each(function() {
      $(this).find('option:selected').each(function(i, selected) {
        values.push($(selected).text());
      });
    });

    return Drupal.sticky_preview.stickyPreviewGetTextResult(values);
  };

  /**
   * Get value from radio widgets.
   */
  Drupal.sticky_preview.stickyPreviewProcessRadioWidgets = function($field_input) {
    var values = [];

    values.push($field_input.parent().find('input:radio:checked').next('label:first').text());

    return Drupal.sticky_preview.stickyPreviewGetTextResult(values);
  };

  /**
   * Get value from checkbox widgets.
   */
  Drupal.sticky_preview.stickyPreviewProcessCheckboxWidgets = function($field_input) {
    var values = [];

    $field_input.each(function() {
      var value = $(this).parent().find('input:checkbox:checked').next('label:first').text();

      if (value) {
        values.push(value);
      }
    });

    if (!values.length) {
      values.push('');
    }

    return Drupal.sticky_preview.stickyPreviewGetTextResult(values);
  };

  /**
   * Returns result text from processed inputs.
   */
  Drupal.sticky_preview.stickyPreviewGetTextResult = function(values) {
    var result = '';

    if (values.length > 1) {
      result = Drupal.sticky_preview.stickyPreviewGenerateList(values);
    } else {
      result = values.pop();
    }

    return result;
  };

  /**
   * Returns html list from array.
   */
  Drupal.sticky_preview.stickyPreviewGenerateList = function(items) {
    var list = '<ul class="items-list">';

    for (var i = 0; i < items.length; i++) {
      list += '<li class="item">' + items[i] + '</li>'
    }

    return list + '</ul>'
  };

  /**
   * Returns different events for different input types.
   */
  Drupal.sticky_preview.getEventForInput = function($field_input) {
    var type = $field_input.attr('type'),
        is_aria_autocomplete = $field_input.attr('aria-autocomplete'),
        event = 'input';

    switch (type) {
      case 'radio':
      case 'checkbox':
        event = 'click';
        break;

      case 'select-multiple':
        event = 'change';
        break;
    }

    // Paste value from auto-complete text widgets only when focus lost.
    if (is_aria_autocomplete) {
      event = 'focusout';
    }

    return event;
  };

  /**
   * Behaviour provides preview functionality.
   */
  Drupal.behaviors.sticky_preview = {
    attach: function (context, settings) {
      // Add preview box to DOM.
      $(settings.sticky_preview.sticky_preview_selector, context).prepend(settings.sticky_preview.sticky_preview_box);

      // Add event bindings for all enabled in preview fields.
      for (var field_name in settings.sticky_preview.sticky_preview_fields) {
        var $field_input = $('[name*="' + field_name + '"]');

        if ($field_input.length) {
          var $placeholder = $(settings.sticky_preview.sticky_preview_fields[field_name].field_placeholder_selector);

          // Default values for preview box.
          $placeholder.html(Drupal.sticky_preview.stickyPreviewProcessInput(field_name));

          // Update values in preview box.
          $field_input.bind(Drupal.sticky_preview.getEventForInput($field_input), function() {
            var this_name = $(this).attr('name').replace(/\[.*?\]/g, ''), // Get rid of [und][0][value] etc.
                $this_placeholder = $(settings.sticky_preview.sticky_preview_fields[this_name].field_placeholder_selector);

            $this_placeholder.html(Drupal.sticky_preview.stickyPreviewProcessInput(this_name));
          });
        }
      }
    }
  };

  /**
   * Behaviour provides show/hide functionality for sticky preview box.
   */
  Drupal.behaviors.sticky_preview_hide = {
    attach: function (context, settings) {
      $('.live_preview-wrapper .button', context).click(function() {
        $(this).toggleClass('minimize');
        $('.live_preview-wrapper .content', context).toggleClass('hidden');
      });
    }
  };
})(jQuery);
