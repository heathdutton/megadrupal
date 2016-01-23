/**
 * @file
 * Contains LocalStoragePlain class.
 *
 * Each (Local Storage) form element element can issue these events (hooks):
 *
 * html5ls:showPlainBtn
 *    Description:
 *      After the button of Local Storage Plain plugin is displayed.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *
 * html5ls:hidePlainBtn
 *    Description:
 *      After the button of Local Storage Plain plugin is hidden.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *
 * Variable "scope" is an object that is used to share data between
 * LocalStorage class and interactors. Some hooks expect to get data from
 * its properties.
 */

/**
 * Local Storage Plain plugin.
 *
 * @param $el
 * @constructor
 */
function LocalStoragePlain($el) {
  this.$el = $el;
  this.attachEvents();
}

(function ($) {
  LocalStoragePlain.prototype = {
    /**
     * @type LocalStorage
     */
    html5ls: undefined,
    /**
     * @type jQuery
     */
    $el: null,
    /**
     * @type jQuery
     */
    $btnEl: null,
    /**
     * Events to attach to the form element.
     */
    events: {
      /**
       * LocalStorage events to hook on.
       */
      html5ls: {
        'html5ls:getElementValue': function (e, scope) {
          scope.value = $(e.target).val();
        },

        'html5ls:setElementValue': function (e, scope, key, id, value) {
          $(e.target).val(value);
        },

        'html5ls:setRestoredValue:after': function (e, scope, restoredValue, defaultValue) {
          if (restoredValue && restoredValue != defaultValue) {
            this.$el.removeClass('html5ls-default')
              .addClass('html5ls-restored');
          }

          this.setState(false);
        },

        'html5ls:setDefaultValue:after': function (e, scope, restoredValue, defaultValue) {
          if (defaultValue && defaultValue != restoredValue) {
            this.$el.removeClass('html5ls-restored')
              .addClass('html5ls-default');
          }

          this.setState(true);
        },

        'html5ls:setMessage': function (e, scope, msg, type) {
          var $output = $(Drupal.theme('localStorageMsg', msg, type));
          var html5lsMessage = this.$el.html5lsMessage();
          var $message = html5lsMessage.$message;
          if ($message) {
            $message.replaceWith($output);
          }
          else {
            $output.insertBefore(this.$btnEl).hide().fadeIn('slow');
          }
          html5lsMessage.$message = $output;
        },

        'html5ls:save:after': function () {
          this.setState(this.html5ls.getState());
        }
      },

      /**
       * Plugin invoke callback.
       *
       * @param {Event} e
       * @returns {boolean}
       */
      invoke: function (e) {
        e.preventDefault();

        if (this.html5ls.getState()) {
          this.html5ls.setRestoredValue();
        }
        else {
          this.html5ls.setDefaultValue();
        }
        return false;
      },

      /**
       * Event callback to set position of the button element.
       */
      setPosition: function () {
        var pos = this.$el.position();
        var left = pos.left + this.$el.outerWidth() - this.$btnEl.outerWidth();
        if (this.scrollHeight > this.$el.innerHeight()) {
          left -= 18;
        }

        this.$btnEl.css({
          left: left
        });

        if (this.$el.is(':visible')) {
          this.show();
        }
        else {
          this.hide();
        }
      }
    },

    /**
     * Attach plugin events.
     */
    attachEvents: function () {
      var me = this;

      me.$el.bind('html5ls:init', function () {
        me.html5ls = me.$el.html5ls();
        me.init();
        me.show();

        me.$el.unbind('html5ls:setMessage');

        $.each(me.events.html5ls, function (name, callback) {
          me.$el.bind(name, callback.bind(me));
        });
      });
    },

    /**
     * Add button to the element.
     */
    init: function () {
      var me = this;

      me.$btnEl = $('<div></div>').attr({
        class: 'html5ls-plain html5ls-plain-hide',
        id: 'html5ls-plain-' + me.$el.attr('id'),
        title: Drupal.t('Local Storage')
      });

      me.$btnEl.bind('click', me.events.invoke.bind(me));
      me.$btnEl.insertBefore(me.$el);
      me.$el.bind('html5ls:setElementValue:after keyup change',
        me.events.setPosition.bind(me));

      // Show button when user clicks on "Edit summary link".
      me.$el.parents('.field-type-text-with-summary')
        .find('.link-edit-summary').bind('click', function () {
          setTimeout(me.events.setPosition.bind(me), 50);
        });

      me.events.setPosition.bind(me).call();
    },

    /**
     * Show button.
     */
    show: function () {
      if (this.$el.is(':visible')) {
        this.$btnEl.removeClass('html5ls-plain-hide');
        this.html5ls.invoke('html5ls:showPlainBtn', this.$btnEl);
      }

      return this;
    },

    /**
     * Hide button.
     */
    hide: function () {
      if (this.$btnEl) {
        this.$btnEl.addClass('html5ls-plain-hide');
        this.html5ls.invoke('html5ls:hidePlainBtn', this.$btnEl);
      }

      return this;
    },

    /**
     * Set button state.
     *
     * @param state
     */
    setState: function (state) {
      if (state === undefined) {
        state = this.html5ls.getState();
      }

      if (state) {
        this.$btnEl.removeClass('html5ls-plain-restored')
          .addClass('html5ls-plain-default');
      }
      else {
        this.$btnEl.removeClass('html5ls-plain-default')
          .addClass('html5ls-plain-restored');
      }
    },

    /**
     * Set button class.
     * @param className
     */
    'class': function (className) {
      if (className != undefined) {
        if (this.prevTypeClass) {
          this.$btnEl.removeClass(this.prevTypeClass);
        }
        this.$btnEl.addClass(className);
        this.prevTypeClass = className;
      }

      return this.prevTypeClass;
    }
  };

  /**
   * jQuery plugin to help to work with LocalStoragePlain class.
   *
   * @returns {LocalStoragePlain}
   * @constructor
   */
  $.fn.html5lsPlain = function () {
    var html5lsPlain = this.data('html5lsPlain');
    if (!html5lsPlain) {
      html5lsPlain = new LocalStoragePlain(this);
      this.data('html5lsPlain', html5lsPlain);
    }

    return html5lsPlain;
  };
})(jQuery);
