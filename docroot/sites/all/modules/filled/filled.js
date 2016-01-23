(function ($) {

  function toggleFilled(element, filled) {
    var formItem = element.closest('.form-item');
    var hasFilled = formItem.hasClass('filled');
    if (hasFilled === filled) {
      return;
    }
    var viewsWidget = formItem.closest('.views-exposed-widget');
    if (filled) {
      formItem.addClass('filled');
      viewsWidget.addClass('filled');
    }
    else {
      formItem.removeClass('filled');
      viewsWidget.removeClass('filled');
    }
  }

  function inputChangeEventHandler(element) {
    var filled = (element.val().length > 0);
    toggleFilled(element, filled);
  }

  function checkboxChangeEventHandler(element) {
    toggleFilled(element, element.is(':checked'));
  }

  function selectChangeEventHandler(element) {
    var value = element.val();
    if ((value === '_none') || ((value === 'All') && (element.closest('.views-exposed-widget').length > 0))) {
      value = '';
    }
    var filled = (value && (value.length > 0));
    toggleFilled(element, filled);
  }

  function bindAndApply(element, event, handler) {
    var timer = 0;
    element.bind(event, function() {
      clearTimeout(timer);
      timer = setTimeout(function() {handler(element);}, 300);
    });
    handler(element);
  }

  Drupal.behaviors.filled_input = {
    attach:function (context, settings) {
      $('input', context).once('filled-input', function() {
        var element = $(this);
        var elementType = element.attr('type');
        var events = {hidden: 'none', submit: 'none', radio: 'none', button: 'none', reset: 'none', image: 'none',
          password: 'change keyup', text: 'change keyup', file: 'change', checkbox: 'click change'};

        var event = events[elementType];

        if (event === 'none') {
          return;
        }

        if (!event) {
          // Unknown input type (e.g. date, email).
          event = 'change keyup';
        }

        bindAndApply(element, event, (element.attr('type') === 'checkbox') ? checkboxChangeEventHandler : inputChangeEventHandler);
      });

      $('textarea', context).once('filled-input', function() {
        bindAndApply($(this), 'change keyup', inputChangeEventHandler);
      });

      $('select', context).once('filled-input', function() {
        bindAndApply($(this), 'change', selectChangeEventHandler);
      });
    }
  };

})(jQuery);
